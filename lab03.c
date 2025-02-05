#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_COUNT 100

void execute_command(char **args, int background);
void parse_input(char *input);
void handle_redirection(char **args);
void handle_pipes(char **args);

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("mysh> ");  
        fflush(stdout);

        if (!fgets(input, MAX_INPUT_SIZE, stdin)) {
            break;
        }

        if (strcmp(input, "exit\n") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        parse_input(input);
    }

    return 0;
}


void parse_input(char *input) {
    char *args[MAX_ARG_COUNT];
    int background = 0;

    input[strcspn(input, "\n")] = '\0';  
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            background = 1;  
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }

    args[i] = NULL;

    if (i == 0) return;  


    for (int j = 0; j < i; j++) {
        if (strcmp(args[j], "|") == 0) {
            handle_pipes(args);
            return;
        }
    }

   
    for (int j = 0; j < i; j++) {
        if (strcmp(args[j], ">") == 0 || strcmp(args[j], "<") == 0) {
            handle_redirection(args);
            return;
        }
    }

    execute_command(args, background);
}


void execute_command(char **args, int background) {
    pid_t pid = fork();

    if (pid == 0) {
        execvp(args[0], args);
        perror("Command execution failed");
        exit(1);
    } else if (pid > 0) {
        if (!background) {
            wait(NULL);  
        }
    } else {
        perror("Fork failed");
    }
}


void handle_redirection(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            args[i] = NULL;
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Failed to open file");
                return;
            }

            pid_t pid = fork();
            if (pid == 0) {
                dup2(fd, STDOUT_FILENO);
                close(fd);
                execvp(args[0], args);
                perror("Command execution failed");
                exit(1);
            } else {
                close(fd);
                wait(NULL);
            }
            return;
        }

        if (strcmp(args[i], "<") == 0) {
            args[i] = NULL;
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("Failed to open file");
                return;
            }

            pid_t pid = fork();
            if (pid == 0) {
                dup2(fd, STDIN_FILENO);
                close(fd);
                execvp(args[0], args);
                perror("Command execution failed");
                exit(1);
            } else {
                close(fd);
                wait(NULL);
            }
            return;
        }

        i++;
    }
}


void handle_pipes(char **args) {
    int pipefd[2];
    pid_t pid1, pid2;
    int i = 0;

    while (args[i] != NULL && strcmp(args[i], "|") != 0) {
        i++;
    }
    args[i] = NULL;

    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return;
    }

    pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args[0], args);
        perror("Command execution failed");
        exit(1);
    }

    pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args[i + 1], &args[i + 1]);
        perror("Command execution failed");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);
}
