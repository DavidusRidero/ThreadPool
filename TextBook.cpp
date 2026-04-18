#include <bits/stdc++.h>

void hello() {
    std::cout << "Hello World.\n";
}

void c_hello() {
    std::cout << "Hello Concurrent World.\n";
}

int main() {
    // hello();
    std::thread thr(c_hello);
    thr.join();

    return 1;
}