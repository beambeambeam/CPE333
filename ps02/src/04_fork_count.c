#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int fork_until_failure(unsigned long successful_forks)
{
    pid_t child_pid = fork();

    if (child_pid == -1) {
        int error_number = errno;

        fprintf(stderr,
                "fork() failed after %lu successful call(s): %s\n",
                successful_forks, strerror(error_number));
        printf("Successful fork calls: %lu\n", successful_forks);

        return (error_number == EAGAIN || error_number == ENOMEM)
                   ? EXIT_SUCCESS
                   : EXIT_FAILURE;
    }

    if (child_pid == 0) {
        return fork_until_failure(successful_forks + 1);
    }

    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(void)
{
    return fork_until_failure(0);
}
