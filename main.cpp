#include <bits/stdc++.h>

//int counter = 1;
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

int main() {
    std::vector<scoped_thread> threads;
    std::cout << "Enter the number of threads to be spawned: ";
    int count = 0;
    std::cin >> count;

    for (unsigned i = 0; i < count; i++)
        threads.emplace_back(std::thread(print_hello));

    return 0;
}