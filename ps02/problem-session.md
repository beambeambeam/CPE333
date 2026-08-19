# Problem Session 2: Process Creation & Pipes

## 1. `fork()` examples

Write the following two programs, based on the `fork()` examples from the process lecture slides:

1. A `helloworld` program.
2. A modified version that uses `wait()`.

Run both programs. Observe and discuss the differences in their output.

## 2. Process lifetime and parent processes

Use `sleep()` to control whether the parent exits before or after the child. Use `ps` to examine process status. You may also use `ps -ef` to examine each process’s parent.

Examine the process list in both situations:

1. The child has exited while the parent is still running.
2. The parent has exited while the child is still running.

Discuss the differences between the two situations. Report the child’s parent process ID in each situation.

In some situations, processes may be tagged as `<defunct>`. Explain:

- What `<defunct>` means.
- Its causes.
- Its consequences.

## 3. Maximum number of forked processes

### 3.1. Count successful `fork()` calls

Write a program that forks until `fork()` fails. The program must report how many times `fork()` was called successfully.

### 3.2. Repeat the experiment

1. Run programs that remain active in the system.
2. Run the fork-counting program again.
3. Stop all programs created after the first fork-counting experiment.
4. Run the fork-counting program again.
5. Discuss the results.

### Hint

A recursive function can be used: the parent waits for the child, while the child creates another child until `fork()` fails. Keep a count in the child and report the last count when `fork()` is unsuccessful.

> **Caution:** An uncontrolled fork program can crash or exhaust system resources. Keep the experiment controlled.

## 4. Communication through a pipe

Write a program that:

1. Creates a pipe with `pipe()`.
2. Creates a child process with `fork()`.
3. Uses the pipe for communication between the parent and child.

The pipe file descriptors are shared by the parent and child. Before communicating:

- The sender must close the pipe’s read end.
- The receiver must close the pipe’s write end.

The child is the sender and the parent is the receiver. The child must:

1. Print `Child` and its PID.
2. Send a message through the pipe containing `Hello from child` and its PID.

The parent must:

1. Print `Parent` and its PID.
2. Read the message.
3. Print the received message.

### Expected output

```text
Child: Child PID: xxxxxx
Parent: Parent PID: yyyyyy
Parent: Hello from child PID: xxxxxx
```

## 5. Pipe behavior experiments

Test and discuss the following situations:

1. The sender tries to read back its own message.
2. The receiver reads before the sender sends.
3. The sender sends several messages before the receiver reads.

Write down your observations and discuss the results.

## Reference system calls

### `pipe()`

```c
int pipe(int fd[2]);
```

`pipe()` takes an array of two integers. On success:

- `fd[0]` is the read file descriptor.
- `fd[1]` is the write file descriptor.

It returns `0` on success and `-1` on error, with `errno` set appropriately.

### `read()`

```c
ssize_t read(int fd, void *buf, size_t count);
```

`read()` reads up to `count` bytes from file descriptor `fd` into `buf`. It returns the number of bytes read on success and `-1` on error, with `errno` set appropriately.

### `write()`

```c
ssize_t write(int fd, const void *buf, size_t count);
```

`write()` writes up to `count` bytes from `buf` to file descriptor `fd`. It returns the number of bytes written on success and `-1` on error, with `errno` set appropriately.

### `close()`

```c
int close(int fd);
```

`close()` closes file descriptor `fd`. It returns `0` on success and `-1` on error, with `errno` set appropriately.

## Submission

Submit a PDF report to LEB2 before the deadline. The report must contain:

- Copies of the C source code, with an overview explaining how it works.
- Results.
- Discussions.

There is no need to submit the C files.
