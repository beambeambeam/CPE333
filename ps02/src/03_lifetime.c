#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int child_exits_first(void)
{
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        printf("Child: PID=%ld, PPID=%ld; exiting in 1 second\n",
               (long)getpid(), (long)getppid());
        fflush(stdout);
        sleep(1);
        printf("Child: exiting now\n");
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    printf("Parent: PID=%ld, child PID=%ld; waiting 5 seconds before waitpid()\n",
           (long)getpid(), (long)child_pid);
    fflush(stdout);
    sleep(5);

    int status;
    if (waitpid(child_pid, &status, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    printf("Parent: child reaped\n");
    return EXIT_SUCCESS;
}

static int parent_exits_first(void)
{
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        printf("Child: PID=%ld, initial PPID=%ld; sleeping 5 seconds\n",
               (long)getpid(), (long)getppid());
        fflush(stdout);
        sleep(5);
        printf("Child: PPID after parent exit=%ld\n", (long)getppid());
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    printf("Parent: PID=%ld, child PID=%ld; exiting in 1 second\n",
           (long)getpid(), (long)child_pid);
    fflush(stdout);
    sleep(1);
    printf("Parent: exiting now\n");
    return EXIT_SUCCESS;
}

static void print_usage(const char *program)
{
    fprintf(stderr, "Usage: %s child-first|parent-first\n", program);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "child-first") == 0) {
        return child_exits_first();
    }

    if (strcmp(argv[1], "parent-first") == 0) {
        return parent_exits_first();
    }

    print_usage(argv[0]);
    return EXIT_FAILURE;
}
