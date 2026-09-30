# PS07 experiment guide — concurrency and threads

This guide turns the PS07 requirements into a repeatable experiment. Write your own example, compile and run it on a POSIX system, capture the three required tasks, and use the observations in your report.

## 1. Prepare the workspace

From the CPE333 repository root:

```bash
cd /path/to/CPE333/ps07
mkdir -p results
```

Keep your source in `ps07.c` and save raw output and screenshots in `results/`. Name screenshots with a two-digit sequence prefix (`00_`, `01_`, `02_`, …). Capture separate tasks and separate runs in separate PNGs; do not combine distinct experiments into one image. Keep the submitted report PDF in the group's submission workspace and submit it to LEB2; do not treat the guide's example commands or another machine's output as your experimental results.

Record the machine and compiler used, since thread scheduling and results depend on the environment:

```bash
{
    printf '$ uname -a\n'
    uname -a
    printf '$ gcc --version\n'
    gcc --version | head -1
    printf '$ date\n'
    date
} | tee results/environment.txt
```

Capture the environment command and output as `results/00_environment.png`.

This lab requires GCC (or a compatible C compiler) and POSIX threads. On Debian/Ubuntu, install GCC if needed with `sudo apt update && sudo apt install build-essential`. Use a Linux machine, WSL2, or another Unix-like environment with pthread support; native Windows GCC environments may not provide the POSIX thread API used by the assignment.

## 2. Choose and implement an example

Pick a shared value and operation that are distinct from the lecture example. Keep the example small enough that you can calculate the correct single-thread result. A shared inventory quantity is one possible domain; use an example that is genuinely your group's own and explain its expected result.

The key requirement is that multiple threads access the same shared variable, at least one access modifies it, and there is no mutex or other synchronization protecting the read-modify-write operation. Ensure the program waits for all worker threads to finish (for example, with `pthread_join`) before printing the final value.

For the forced-switch version, make the update visibly consist of separate steps: copy the shared value to a local variable, update the local copy, yield or briefly sleep, then write it back. A POSIX implementation can call `sched_yield()` from `<sched.h>`; a short `nanosleep()` from `<time.h>` is another option. These encourage scheduling between the read and write, but neither guarantees a particular interleaving on every run. The assignment's `yield()` is illustrative pseudocode; use the corresponding function available on your system.

> `register` is only a C storage-class hint; it does not guarantee that a value lives in a CPU register or cause a context switch. A local variable such as `int reg = counter;` makes the separate read/update/write steps explicit. `volatile` is not a substitute for synchronization.

## 3. Build and check the single-thread version — Task 1

The assignment's example build command is:

```bash
gcc -o ps7 ps7.c -lpthread
```

On systems that recommend the compiler's pthread option, this equivalent form is also suitable:

```bash
gcc -Wall -Wextra -O0 -pthread -o ps7 ps7.c
```

Use one build command consistently, and record it. `-O0` makes the lab's separate operations easier to inspect, but it does not make a data race well-defined.

Run the version with only one executing worker (or with the main thread doing the work), and compare the final value with the result you calculate from the initial value and operations. Save the command and complete output:

```bash
{
    printf '$ ./ps7  # Task 1: one thread\n'
    ./ps7
} | tee results/task1-single-thread.txt
```

Capture this run separately, showing the command and complete output, as `results/01_task1_single_thread.png`.

## 4. Run multiple threads without an added yield — Task 2

Change the program to use more than one worker thread while leaving the shared update unsynchronized. Run without an added yield or sleep. Capture each trial separately, with its command and complete output visible:

```bash
{
    printf '$ ./ps7  # Task 2, run 1\n'
    ./ps7
} | tee results/task2-run1.txt
```

Save the screenshot as `results/02_task2_no_yield_run1.png`. Repeat the same command for runs 2 and 3, changing the label and text-log filename; save those screenshots as `results/03_task2_no_yield_run2.png` and `results/04_task2_no_yield_run3.png`. Run more trials if useful, continuing the two-digit sequence. Save the exact output; do not replace it with an expected or hand-written result. In the report, compare the observed final values with the single-thread result and explain any lost or unexpected updates.

## 5. Encourage a context switch during the update — Task 3

Add `sched_yield()` or a short sleep between reading the shared value and writing the updated value. Rebuild using the same compiler options, then run the same multithreaded workload multiple times:

```bash
gcc -Wall -Wextra -O0 -pthread -o ps7 ps7.c
{
    printf '$ ./ps7  # Task 3, run 1\n'
    ./ps7
} | tee results/task3-run1.txt
```

Capture run 1 separately as `results/05_task3_context_switch_run1.png`. Repeat for runs 2 and 3, changing the label and text-log filename; save them as `results/06_task3_context_switch_run2.png` and `results/07_task3_context_switch_run3.png`. If the final values remain the same, record that honestly and run additional trials or adjust the workload/delay to make interleavings more likely, continuing the two-digit sequence. A yield or sleep is a scheduling hint, not a promise that a particular output will occur.

## 6. Explain the results and submit

In the report, include:

- The program code and the example's initial value, thread count, and operation count.
- Task 1's output and the expected single-thread result.
- Several actual Task 2 and Task 3 outputs, with the compiler command and environment.
- An explanation of how a read-modify-write can lose an update: two threads can read the same old value, independently compute a new value, and overwrite one another's result.
- Why adding a yield/sleep can make the interleaving easier to observe, and why it still does not guarantee a particular result.

In C, unsynchronized concurrent access where threads conflict and at least one access is a write is a data race; under the C memory model its behavior is undefined. Therefore, observed output is specific to the compiler, build, and machine, and nondeterministic output is not guaranteed on every run. State this limitation rather than presenting one output as guaranteed.

Submit the source code and the short PDF report with output examples to LEB2. Work with your existing group of up to 3–4 students.
