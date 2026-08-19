#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        printf("Hello world from child: PID=%ld, PPID=%ld\n",
               (long)getpid(), (long)getppid());
        return EXIT_SUCCESS;
    }

    printf("Hello world from parent: PID=%ld, child PID=%ld\n",
           (long)getpid(), (long)child_pid);

    int status;
    pid_t waited_pid = wait(&status);

    if (waited_pid == -1) {
        perror("wait");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf("Parent: child %ld exited with status %d\n",
               (long)waited_pid, WEXITSTATUS(status));
    } else {
        printf("Parent: child %ld did not exit normally\n",
               (long)waited_pid);
    }

    return EXIT_SUCCESS;
}
