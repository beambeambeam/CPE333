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
        const char message[] = "Hello from sender";
        const size_t message_length = sizeof(message) - 1;

        printf("Sender: PID=%ld\n", (long)getpid());
        fflush(stdout);

        ssize_t bytes_written = write(
            pipe_fds[1], message, message_length);

        if (bytes_written == -1) {
            perror("write");
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            _exit(EXIT_FAILURE);
        }

        if ((size_t)bytes_written != message_length) {
            fprintf(stderr, "Partial pipe write\n");
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            _exit(EXIT_FAILURE);
        }

        char buffer[128];
        ssize_t bytes_read = read(pipe_fds[0], buffer, message_length);

        if (bytes_read == -1) {
            perror("read");
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            _exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        printf("Sender: read its own message: %s\n", buffer);
        fflush(stdout);

        close(pipe_fds[0]);
        close(pipe_fds[1]);
        _exit(EXIT_SUCCESS);
    }

    close(pipe_fds[0]);
    close(pipe_fds[1]);

    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
