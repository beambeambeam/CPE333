#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int pipe_fds[2];

    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        close(pipe_fds[0]);

        printf("Child: Child PID: %ld\n", (long)getpid());
        fflush(stdout);

        char message[128];
        int message_length = snprintf(
            message, sizeof(message), "Hello from child PID: %ld", (long)getpid());

        if (message_length < 0 || (size_t)message_length >= sizeof(message)) {
            fprintf(stderr, "Message did not fit in buffer\n");
            close(pipe_fds[1]);
            _exit(EXIT_FAILURE);
        }

        ssize_t bytes_written = write(
            pipe_fds[1], message, (size_t)message_length);

        if (bytes_written == -1) {
            perror("write");
            close(pipe_fds[1]);
            _exit(EXIT_FAILURE);
        }

        if (bytes_written != message_length) {
            fprintf(stderr, "Partial pipe write\n");
            close(pipe_fds[1]);
            _exit(EXIT_FAILURE);
        }

        close(pipe_fds[1]);
        _exit(EXIT_SUCCESS);
    }

    close(pipe_fds[1]);

    printf("Parent: Parent PID: %ld\n", (long)getpid());
    fflush(stdout);

    char buffer[128];
    ssize_t bytes_read = read(pipe_fds[0], buffer, sizeof(buffer) - 1);

    if (bytes_read == -1) {
        perror("read");
        close(pipe_fds[0]);
        waitpid(child_pid, NULL, 0);
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0';
    printf("Parent: %s\n", buffer);

    close(pipe_fds[0]);
    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
