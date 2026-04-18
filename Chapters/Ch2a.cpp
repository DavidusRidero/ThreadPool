#include <ranges>
#include <bits/stdc++.h>

void do_something() {
   std::cout << "Did something.\n";
}

void do_something_else() {
   std::cout << "Did something else.\n";
}

class background_task {
public:
   void operator() () const {
      do_something();
      do_something_else();
   }
};

int main() {
   background_task f;          // f is an object. It will be COPIED by the thread constructor.

   // std::thread thr(f);     // Thread constructor always calls f().
   // std::thread thr{f};

   // Alternatively, to dodge "Most Vexing Parse".
   // Tip: Pass the class, not the object.
   // std::thread thr ((background_task()));   //Old Way
   // std::thread thr {background_task()};   //New Way

   // Even more alternatively, pass in a lambda expression
   std::thread thr([] {
      do_something();
      do_something_else();
   });

   // Have to explicitly decide to join or detach the thread.
   // At the end of the program, if neither is specified, thread is terminated/destroyed.
   thr.join();

   return 1;
}

// It's a bad idea to create a thread within a function,
// that has access to the local variables in that function,
// unless the thread is guaranteed to finish before the function exits.

// For if the function terminates, after detaching the thread,
// the thread may access the deallocated local variables,
// leading to undefined behaviour.
// Standard Use-After-Free mistake.