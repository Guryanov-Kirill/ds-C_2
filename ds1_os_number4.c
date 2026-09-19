#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe error");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork error");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (pid == 0) {
        if (close(pipefd[1]) == -1) {
            perror("child: close pipefd[1] failed");
            exit(1);
        }

        char buffer[128];
        ssize_t bytes_read;

        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            if (write(1, buffer, bytes_read) == -1) {
                perror("child: write to stdout failed");
                exit(1);
            }
        }

        if (bytes_read == -1) {
            perror("child: read failed");
            exit(1);
        }

        if (close(pipefd[0]) == -1) {
            perror("child: close pipefd[0] failed");
            exit(1);
        }

        exit(0);
    } else {
        if (close(pipefd[0]) == -1) {
            perror("parent: close pipefd[0] failed");
            exit(1);
        }

        for (int i = 1; i < argc; i++) {
            if (write(pipefd[1], argv[i], strlen(argv[i])) == -1) {
                perror("parent: write arg failed");
                exit(1);
            }
            if (write(pipefd[1], "\n", 1) == -1) {
                perror("parent: write newline failed");
                exit(1);
            }
        }

        if (close(pipefd[1]) == -1) {
            perror("parent: close pipefd[1] failed");
            exit(1);
        }

        int status;
        if (wait(&status) == -1) {
            perror("parent: wait failed");
            return 1;
        }

        return 0;
    }
}
