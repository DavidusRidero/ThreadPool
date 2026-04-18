#include <bits/stdc++.h>

class ThreadPool {


    private:
        std::vector<std::thread> Threads;
        std::queue<std::function<void()>> Tasks;


};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}