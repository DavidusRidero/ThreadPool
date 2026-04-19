#include <bits/stdc++.h>

//int counter = 1;
std::atomic<int> counter{0};

std::mutex cout_mutex;

void print_hello() {
    int id = counter++;
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Hello World by Thread " << counter++ << "\n";
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