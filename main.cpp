// WORK IN PROGRESS.
// 22nd April Snapshot. Probably the last day.
// https://github.com/DavidusRidero/ThreadPool

#include <bits/stdc++.h>

class scoped_thread {
    std::thread Thread;

public:
    //initialization with member lists, with a sanity check for non-threads.
    explicit scoped_thread(std::thread Thread_) : Thread(std::move(Thread_)) {
        if (!Thread.joinable())
            throw std::logic_error("No Thread.");
    }

    //Destructor that joins. Sanity check for std::threads that
    //have no execution threads assigned to them.
    ~scoped_thread() {
        if (Thread.joinable())
            Thread.join();
    }

    //Disable the Copy constructor & Copy Assignment constructor
    scoped_thread(const scoped_thread&) noexcept = delete;
    scoped_thread& operator=(const scoped_thread&) noexcept = delete;

    //Default to the Move & Move Assignment constructor.
    scoped_thread(scoped_thread&&) noexcept = default;
    scoped_thread& operator=(scoped_thread&&) noexcept = default;
};

class TaskQueue {
private:
    std::mutex guard;
    std::condition_variable cv;
    std::queue<std::function<void()>> Tasks;
    bool stop = false;

public:
    void push(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(guard);
            Tasks.push(std::move(task));
        }
        cv.notify_one();
    }

    std::function<void()> pop() {
        std::unique_lock<std::mutex> lock(guard);

        //the conditional variable "wakes" the thread, if the task is empty or shutdown is called.
        cv.wait(lock, [this]{ return !Tasks.empty() or stop;});

        //{} is an empty std function. Signals the worker to exit.
        if (stop && Tasks.empty()) return {};

        std::function<void()> temp = Tasks.front();
        Tasks.pop();
        return temp;
    }

    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(guard);
            stop = true;
        }
        cv.notify_all();
    }
};

void worker_function( TaskQueue& Queue ) {
    while (true) {
        auto th = Queue.pop();
        if (!th) break;
        th();
    }
}

class ThreadPool {
private:
    TaskQueue queue;
    std::vector<std::thread> workers;

public:
    explicit ThreadPool(const int n) {
        for (int i = 0; i < n; i++)
            //You forgot the std::ref
            workers.emplace_back(worker_function, std::ref(queue));
        //Thread copies the arguments by default.
    }

    //Manually, we would have to initialize a promise and a future.
    //Then we had to link those two.
    //Then we had to execute the callable somehow.
    //Then set the result into the promise.

    //Packaged Task takes the callable and sets the promise automatically.
    //std::packaged_task<int()> pt(task);
    //std::future<int> fu = pt.get_future();
    //pt();

    //LLM Task 2: Upgrade the submit to work with packaged_task.
    template<typename F>
    auto submit (F&& task) -> std::future<std::invoke_result_t<F>> {
        //Alias. What type does F return when it's called.
        using R = std::invoke_result_t<F>;

        //Callable that returns R. As for task, move if possible, copy if not.
        std::packaged_task<R()> pt(std::forward<F>(task));

        //We create a future associated with the pt's promise.
        //Task returns value. Value goes in promise. Future becomes ready.
        std::future<R> future = pt.get_future();

        //pt is move-only, and std::function requires copyability.
        //So pt is put on the heap through shared_ptr, which is copyable,
        //allowing function to hold a lambda.
        auto shared_pt = std::make_shared<std::packaged_task<R()>> (std::move(pt));

        //dereference invokes the packaged task, running the original callable.
        //The return value automatically gets stored into the promise, making the future ready.
        queue.push([shared_pt]{ (*shared_pt)(); });

        return future;

        //PREVIOUS IMPLEMENTATION
        //queue.push(std::move(input));
        //Make sure to use std::move to transfer ownership
    }

    ~ThreadPool() {
        std::cout << "ThreadPool destructed.\n";
        queue.shutdown();
        //Threads are move only. Use references.
        for (auto& th:workers)
            th.join();
    }
};

void print_hello() {
    std::atomic<int> static counter{1};
    std::mutex cout_mutex;

    //Keeping the mutex lock here, guarantees the order.
    std::lock_guard<std::mutex> lock(cout_mutex);

    const int id = counter++;

    //Keeping the mutex here guarantees the output
    //is a single line, unbroken by other threads

    std::cout << "Hello World by Thread " << id << "\n";
}
void main_subfunction_1() {
    //Direct manipulation of threads
    std::vector<scoped_thread> threads;
    std::cout << "Enter the number of threads to be spawned: ";
    int count = 0;
    std::cin >> count;

    for (unsigned i = 0; i < count; i++)
        threads.emplace_back(std::thread(print_hello));
}
void main_subfunction_2() {
    //Thread manipulation through TaskQueue
    TaskQueue Queue;
    Queue.push( []() { std::cout << "Task 1\n"; } );
    Queue.push( []() { std::cout << "Task 2\n"; } );

    auto th1 = Queue.pop(); th1();
    auto th2 = Queue.pop(); th2();

    Queue.shutdown();

    auto t3 = Queue.pop();
    if (!t3) std::cout << "Received empty function. Shutdown works correctly.\n";
}
void main_subfunction_3() {
    //Thread manipulation through Threadpool

    ThreadPool pool(4);
    uint8_t counter = 0;
    std::mutex print_mutex;

    //Race condition on output stream.
    //Caught something that Claude missed. adding the mutex to capture list.
    for (int i = 0; i < 10; i++)
        pool.submit([i, &print_mutex]() {
            std::lock_guard<std::mutex> lock_guard(print_mutex);
            std::cout << "Task " << i << " by thread" << std::this_thread::get_id() << ".\n";
        });

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
void main_subfunction_4() {
    //Thread manipulation, now with packaged tasks
    ThreadPool pool(4);

    auto f1 = pool.submit([]{ return 42; });
    auto f2 = pool.submit([]{ return std::string("Hello from pool.\n"); });

    std::cout << f1.get() << "\n";
    std::cout << f2.get();
}

int main() {
    main_subfunction_4();
    return 1;
}

//GG