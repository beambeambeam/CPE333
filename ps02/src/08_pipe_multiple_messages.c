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
        const char *messages[] = {"ONE", "TWO", "THREE"};
        const size_t message_count = sizeof(messages) / sizeof(messages[0]);

        close(pipe_fds[0]);

        for (size_t index = 0; index < message_count; ++index) {
            size_t message_length = 0;

            while (messages[index][message_length] != '\0') {
                ++message_length;
            }

            ssize_t bytes_written = write(
                pipe_fds[1], messages[index], message_length);

            if (bytes_written == -1) {
                perror("write");
                close(pipe_fds[1]);
                _exit(EXIT_FAILURE);
            }

            if ((size_t)bytes_written != message_length) {
                fprintf(stderr, "Partial pipe write\n");
                close(pipe_fds[1]);
                _exit(EXIT_FAILURE);
            }
        }

        printf("Sender: wrote %zu messages\n", message_count);
        fflush(stdout);

        close(pipe_fds[1]);
        _exit(EXIT_SUCCESS);
    }

    close(pipe_fds[1]);

    sleep(1);

    char buffer[6];
    ssize_t bytes_read;

    while ((bytes_read = read(pipe_fds[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Receiver: read %zd bytes: %s\n", bytes_read, buffer);
        fflush(stdout);
    }

    if (bytes_read == -1) {
        perror("read");
        close(pipe_fds[0]);
        waitpid(child_pid, NULL, 0);
        return EXIT_FAILURE;
    }

    printf("Receiver: EOF\n");
    close(pipe_fds[0]);

    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
