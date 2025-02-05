#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define ARG_SIZE 100

void exe_cmd(char **args, int bg); 
void input_parse(char *in);
void redirect_handler(char **args);
void pipe_handler(char **args);

int main() {
    char in[BUF_SIZE];

    while (1) {
        printf("mysh> "); fflush(stdout);
        if (!fgets(in, BUF_SIZE, stdin)) break;
        in[strcspn(in, "\n")] = '\0';
        if (strcmp(in, "exit") == 0) { printf("Exiting...\n"); break; }
        input_parse(in);
    }
    return 0;
}

void input_parse(char *in) {
    char *args[ARG_SIZE]; int bg = 0, x = 0;
    char *tok = strtok(in, " ");
    while (tok != NULL) {
        if (strcmp(tok, "&") == 0) bg = 1;
        else args[x++] = tok;
        tok = strtok(NULL, " ");
    }
    args[x] = NULL;
    if (x == 0) return;
    for (int y = 0; y < x; y++) if (strcmp(args[y], "|") == 0) { pipe_handler(args); return; }
    for (int z = 0; z < x; z++) if (strcmp(args[z], ">") == 0 || strcmp(args[z], "<") == 0) { redirect_handler(args); return; }
    exe_cmd(args, bg);
}

void exe_cmd(char **args, int bg) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args); perror("Exec failed"); exit(1);
    } else if (pid > 0) { if (!bg) wait(NULL); }
    else perror("Fork failed");
}

void redirect_handler(char **args) {
    int x = 0;
    while (args[x] != NULL) {
        if (strcmp(args[x], ">") == 0) {
            args[x] = NULL;
            int fd = open(args[x + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("File error"); return; }
            pid_t pid = fork();
            if (pid == 0) { dup2(fd, STDOUT_FILENO); close(fd); execvp(args[0], args); perror("Exec failed"); exit(1); }
            else { close(fd); wait(NULL); }
            return;
        }
        if (strcmp(args[x], "<") == 0) {
            args[x] = NULL;
            int fd = open(args[x + 1], O_RDONLY);
            if (fd < 0) { perror("File error"); return; }
            pid_t pid = fork();
            if (pid == 0) { dup2(fd, STDIN_FILENO); close(fd); execvp(args[0], args); perror("Exec failed"); exit(1); }
            else { close(fd); wait(NULL); }
            return;
        }
        x++;
    }
}

void pipe_handler(char **args) {
    int fds[2], x = 0;
    while (args[x] != NULL && strcmp(args[x], "|") != 0) x++;
    args[x] = NULL;
    if (pipe(fds) == -1) { perror("Pipe error"); return; }
    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]); close(fds[1]);
        execvp(args[0], args); perror("Exec failed"); exit(1);
    }
    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]); close(fds[1]);
        execvp(args[x + 1], &args[x + 1]); perror("Exec failed"); exit(1);
    }
    close(fds[0]); close(fds[1]); wait(NULL); wait(NULL);
}
