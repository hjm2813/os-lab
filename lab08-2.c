#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void print_gibberish() {
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
    int len = sizeof(charset) - 1;

    printf("\n*** BOOM! The time bomb exploded! ***\n");
    
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 80; j++) {
            printf("%c", charset[rand() % len]);
        }
        printf("\n");
    }

    printf("\n[Time Bomb] Waiting for 10 seconds before exit...\n");
    sleep(10);
    printf("[Time Bomb] Goodbye!\n");
    exit(0);
}

void handle_sigint(int sig) {
    print_gibberish();
}

int main() {
    srand(time(NULL));
    signal(SIGINT, handle_sigint);
    
    printf("[Time Bomb] Running... Press Ctrl+C to activate the bomb!\n");

    while (1) {
        pause();
    }

    return 0;
}
