## Chapter 2 Prompt, courtesy of NotebookLM
```markdown
"Generate a set of C++ coding exercises based on the following problem specifications. 
For each problem, provide the objective, a buggy or incomplete code snippet if applicable, 
and the specific task the user needs to complete. 
Do not provide the solutions immediately; wait for the user to attempt them."
```

### Problem 1: Launching Threads and the Vexing Parse

- **Concept**: Basic thread creation using function objects and lambdas.
- **Task**:
  - Present a code snippet where a user attempts to launch a thread using a temporary function object, but accidentally triggers *C++'s most vexing parse* (declaring a function instead of a variable).
  - Fix the compilation error using **uniform initialization syntax (braces)**.
  - Rewrite the same thread launch using a **C++11 lambda expression**.

### Problem 2: Lifetime Issues with Detached Threads

- **Concept**: Waiting for a thread (`join`) vs leaving it to run (`detach`), and dangling references.
- **Task**:
  - Provide a function that spawns a thread to process a local variable by reference, but calls `detach()` right before exiting the function.
  - Explain why this causes **undefined behavior**.
  - Fix the issue by replacing `detach()` with the appropriate method to wait for the thread to finish.

### Problem 3: Exception-Safe Thread Joining (RAII)

- **Concept**: Ensuring threads are joined even when exceptions are thrown.
- **Task**:
  - Provide a code snippet where a thread is launched, followed by a function call that might throw an exception, and finally a call to `join()`.
  - Implement a `thread_guard` RAII class that takes a reference to a `std::thread` and automatically checks `joinable()` and calls `join()` in its destructor.
  - Update the original snippet to use this new class.

### Problem 4: Passing Arguments to Threads Correctly

- **Concept**: Handling implicit conversions and references in `std::thread` constructors.
- **Task**:
  - Present two separate compilation/runtime errors for the user to fix:
    1. A thread is passed a pointer to a local `char buffer[]` while the target function expects a `std::string`. Fix this by explicitly casting the buffer to `std::string` in the thread constructor to avoid dangling pointers.
    2. A thread targets a function expecting a non-const reference (`widget_data&`), which fails to compile because `std::thread` copies arguments and passes them as rvalues. Fix this by wrapping the argument in `std::ref()`.

### Problem 5: Moving Threads and Transferring Ownership

- **Concept**: `std::thread` instances are movable but not copyable.
- **Task**:
  - Write two functions:
    - `std::thread spawn_worker()`: Creates a background thread and returns it by value.
    - `void consume_worker(std::thread t)`: Takes a thread by value and joins it.
  - Write a `main()` function that demonstrates transferring ownership by passing the output of `spawn_worker()` into `consume_worker()` using `std::move()`.

### Problem 6: Hardware Concurrency and Thread Vectors

- **Concept**: Grouping threads and choosing thread counts at runtime.
- **Task**:
  - Write a program that queries `std::thread::hardware_concurrency()` to find the number of supported concurrent threads.
  - If it returns `0`, default to `2`.
  - Create a `std::vector<std::thread>`, spawn that exact number of threads to print `"Hello from worker"`, and then use a range-based `for` loop to `join()` all threads in the vector before the program exits.

### Problem 7: Thread Identification

- **Concept**: Utilizing `std::thread::id` to distinguish thread behaviors.
- **Task**:
  - Provide a skeleton for an algorithm where a master thread spawns several worker threads.
  - Store the master thread's ID using `std::this_thread::get_id()` before launching the workers.
  - Inside the shared thread function, implement an `if/else` check that compares the current executing thread's ID against the stored master ID, performing **unique work** if it is the master, and **common work** otherwise.