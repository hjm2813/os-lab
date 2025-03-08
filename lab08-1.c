#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t child_pid;
int allow_terminate = 0;

void handle_parent_sigint(int sig) {
    if (allow_terminate) {
        printf("\n[Parent] Received Ctrl+C. Terminating child process...\n");
        kill(child_pid, SIGUSR1);
        wait(NULL);
        printf("[Parent] Child process terminated. Goodbye!\n");
        exit(0);
    } else {
        printf("\n[Parent] System protected! Ctrl+C is disabled for now.\n");
    }
}

void handle_child_sigint(int sig) {
    printf("[Child] Ctrl+C ignored! You can't kill me!\n");
}

void handle_child_terminate(int sig) {
    printf("[Child] Received termination signal from parent. Exiting...\n");
    exit(0);
}

int main() {
    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (child_pid == 0) {
        signal(SIGINT, handle_child_sigint);
        signal(SIGUSR1, handle_child_terminate);

        while (1) {
            sleep(1);
        }
    } else {
        signal(SIGINT, handle_parent_sigint);
        
        printf("[Parent] Parent PID: %d, Child PID: %d\n", getpid(), child_pid);
        printf("[Parent] Ctrl+C protection enabled for 10 seconds...\n");

        sleep(10);
        allow_terminate = 1;
        printf("[Parent] You can now terminate the parent with Ctrl+C\n");

        while (1) {
            sleep(1);
        }
    }

    return 0;
}
