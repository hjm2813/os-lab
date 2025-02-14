#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#define KEY 1234
#define SIZE 512

struct Message {
    long type;
    char text[SIZE];
};

void processA() {
    int msqid;
    msqid = msgget(KEY, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }

    Message msg;
    while (true) {
        std::cout << "Process A: ";
        std::cin.getline(msg.text, SIZE);
        msg.type = 1;

        if (msgsnd(msqid, &msg, strlen(msg.text) + 1, 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        if (strcmp(msg.text, "Exit") == 0) break;

        if (msgrcv(msqid, &msg, SIZE, 2, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        std::cout << "Received from B: " << msg.text << std::endl;

        if (strcmp(msg.text, "Exit") == 0) break;
    }

    msgctl(msqid, IPC_RMID, NULL);
}

void processB() {
    int msqid;
    while ((msqid = msgget(KEY, 0666)) == -1) {
        usleep(100000);
    }

    Message msg;
    while (true) {
        if (msgrcv(msqid, &msg, SIZE, 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        std::cout << "Received from A: " << msg.text << std::endl;

        if (strcmp(msg.text, "Exit") == 0) break;

        std::cout << "Process B: ";
        std::cin.getline(msg.text, SIZE);
        msg.type = 2;

        if (msgsnd(msqid, &msg, strlen(msg.text) + 1, 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        if (strcmp(msg.text, "Exit") == 0) break;
    }
}

int main() {
    std::cout << "Start as (A/B): ";
    char role;
    std::cin >> role;
    std::cin.ignore();

    if (role == 'A') {
        processA();
    } else if (role == 'B') {
        processB();
    } else {
        std::cout << "Invalid input!" << std::endl;
    }

    return 0;
}
