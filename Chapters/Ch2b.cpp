//Explanation Source File. Do not compile.

#include <bits/stdc++.h>

// Joining the thread, ensures that the thread has
// completed execution before the function exits.

// If it's important to ensure that the thread
// completes before the function exits - whether
// because it has a reference to a local variable
// or some other reason - then it's important to
// ensure this is the case for all exit paths.

// It becomes desirable to provide a simple
// and concise mechanism to do so. Enter
// Resource Acquisition is Initialisation idiom.
// This is done by providing a class that does
// the join() in its destructor.

class thread_guard {
    std::thread& thread;

public:
    explicit thread_guard (std::thread& thread_): thread(thread_){}
    // thread(thread_) is called Member List Initialization.
    // Serves 2 purposes:
    // a) to initialize value at creation
    // b) only way to assign constants to references

    ~thread_guard() {
        if (thread.joinable())
            thread.join();
    }

    // Forbidding creation of copy constructor
    // i.e. can't clone a thread
    thread_guard (thread_guard const&) = delete;

    // Forbidding creation of assignment constructor
    // i.e. can't pass ownership
    thread_guard& operator= (thread_guard const&) =delete;
};

struct func
{
    int& i;

    // Constructor
    func(int& i_):i(i_){}

    // Operations
    void operator()()
    {
        for(unsigned j=0;j<1000000;++j)
            //do_something(i)
            ;
    }
};

void f() {
    int local_variable = 0;
    func my_func (local_variable);
    std::thread thread(my_func);
    thread_guard guard(thread);
    //do_something_in_current_thread
}

// When the execution reaches the end of f()
// the local variables are deconstructed in the
// reverse order of initialization.
// Guard is the first to go, and with it thread.
// This also happens if last line throws an exception.
