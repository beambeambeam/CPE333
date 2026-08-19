#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        printf("Hello world from child: PID=%ld, PPID=%ld\n",
               (long)getpid(), (long)getppid());
    } else {
        printf("Hello world from parent: PID=%ld, child PID=%ld\n",
               (long)getpid(), (long)pid);
    }

    return EXIT_SUCCESS;
}
