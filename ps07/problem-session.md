# Problem Session 7: Concurrency and Threads

**CPE 333 Operating Systems — 1/2026**  
King Mongkut's University of Technology Thonburi, Faculty of Engineering, Department of Computer Engineering

## Objective

Gain experience writing multithreaded programs and explain how a race condition affects a shared variable.

## Instructions

Write a multithreaded program that has a race condition. Choose an example of your own that differs from the example discussed in lecture.

Compile and run the program to show that its behavior is not deterministic: results may differ between runs. You may need to encourage an untimely context switch between threads by calling a yield or sleep function.

### Untimely context switch

1. Write a multithreaded program in which multiple threads increase or decrease the same variable concurrently, without proper synchronization.
2. Compile and run the program. For example:

   ```bash
   gcc -o ps7 ps7.c -lpthread
   ./ps7
   ```

3. Run the program with only one thread. Check that it behaves correctly and save the output as **Task 1**.
4. Modify the program to create more than one thread.
5. Run it without forcing context switches and save the output as **Task 2**. Run it several times if needed to observe nondeterministic results. For example, an update may take the form:

   ```c
   counter = counter - 1;
   ```

6. Modify the program to encourage a context switch while a thread is updating the shared variable. For example, separate the read, update, and write:

   ```c
   int reg = counter;  // load the shared value
   reg = reg + 1;      // update the local copy
   yield();            // encourage a context switch
   counter = reg;      // write the value back
   ```

7. Run the program multiple times and save the outputs as **Task 3**. Show that the final shared-variable value can differ between runs because of the race condition.
8. Discuss what happens in your program and explain the cause of its race condition.

## Submission

Submit the program code and a report to LEB2. The report must be a PDF with a short explanation of the code and examples of its output, including the race-condition program and results from running it.

Students should work in their existing groups of up to 3–4 students.
