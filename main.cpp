#include <bits/stdc++.h>

//int counter = 1;
std::atomic<int> counter{1};

std::mutex cout_mutex;

void print_hello() {
    //Keeping the mutex lock here, guarantees the order.
    std::lock_guard<std::mutex> lock(cout_mutex);

    const int id = counter++;

    //Keeping the mutex here guarantees
    //the output is a single line, unbroken by other threads

    std::cout << "Hello World by Thread " << id << "\n";
    //std::this_thread::get_id doesn't output a number.
    //It returns something like 89283492879287359.
}

int main() {
    std::vector<std::thread> threads;
    std::cout << "Enter the number of threads to be spawned: ";
    int count = 0;
    std::cin >> count;

    for (unsigned i = 0; i < count; i++)
        threads.emplace_back(print_hello);

    for (auto& thread :threads)
        thread.join();

    return 0;
}