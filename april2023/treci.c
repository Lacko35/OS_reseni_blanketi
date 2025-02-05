#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_LEN 84
#define MSG_KEY 10101

struct message {
    long type;
    char text[MSG_KEY];
};

int main() {
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    struct message buffer;

    int uneti_brojevi = 0;

    if(fork() == 0) {
        int broj;

        while(1) {
            msgrcv(msgid, &buffer, MSG_LEN, 1, 0);

            broj = atoi(buffer.text);
            uneti_brojevi++;

            if(broj == 0) {
                break;
            }

            printf("Odgovarajuci ASCII karakter je: %c\n----------------------------------------------\n", broj);
        }

        printf("Broj unetih brojeva je: %d\n", uneti_brojevi);
        exit(EXIT_SUCCESS);
    }
    else {
        int broj;

        while(1) {
            printf("Unosite brojeve u opsegu [0-127], 0 za izlaz iz programa: ");
            fflush(stdout);
            scanf("%d", &broj);

            sprintf(buffer.text, "%d", broj);
            buffer.type = 1;
            msgsnd(msgid, &buffer, sizeof(buffer.text), 0);

            if(broj == 0) {
                break;
            }
        }

        wait(NULL);

        msgctl(msgid, IPC_RMID, NULL);

        return 0;
    }
}