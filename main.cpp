// WORK IN PROGRESS. 20th April Snapshot.
// Check https://github.com/DavidusRidero/ThreadPool for the current version.

#include <bits/stdc++.h>

std::atomic<int> counter{1};
std::mutex cout_mutex;

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

//LLM: Step 1 — Task queue : a thread-safe queue that holds callables
class Task_queue {
private:
    std::mutex guard;
    std::condition_variable cv;
    std::queue<std::function<void()>> Tasks;
    bool stop = false;

public:
    //push(task) callable.
    void push(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(guard);
            Tasks.push(std::move(task));
        }
        cv.notify_one();
    }

    //pop callable.
    std::function<void()> pop() {
        //Mutex is the resource, unique_lock is the RAII wrapper.
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
        //Someway to add an empty function to the beginning of the queue?
        //No need to do that. Just inform all threads
        cv.notify_all();
    }
};

void worker_loop() {

}
void print_hello() {
    //Keeping the mutex lock here, guarantees the order.
    std::lock_guard<std::mutex> lock(cout_mutex);

    const int id = counter++;

    //Keeping the mutex here guarantees
    //the output is a single line, unbroken by other threads

    std::cout << "Hello World by Thread " << id << "\n";
    //std::this_thread::get_id doesn't output a number.
    //It returns something like 89283492879287359.
    //only useful for debugging.
}


void main_subfunction_1() {
    std::vector<scoped_thread> threads;
    std::cout << "Enter the number of threads to be spawned: ";
    int count = 0;
    std::cin >> count;

    for (unsigned i = 0; i < count; i++)
        threads.emplace_back(std::thread(print_hello));
}

void main_subfunction_2() {
    Task_queue Queue;
    Queue.push( []() { std::cout << "Task 1\n"; } );
    Queue.push( []() { std::cout << "Task 2\n"; } );

    auto th1 = Queue.pop(); th1();
    auto th2 = Queue.pop(); th2();

    Queue.shutdown();

    auto t3 = Queue.pop();
    if (!t3) std::cout << "Received empty function. Shutdown works correctly.\n";
}

int main() {
    main_subfunction_2();

    return 0;
}