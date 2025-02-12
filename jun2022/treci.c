#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_KEY 10101
#define MSG_LEN 20

struct message {
    long tip;
    char sadrzaj[MSG_LEN];
};

int main() {
    int msgid = msgget((key_t)MSG_KEY, IPC_CREAT | 0666);

    struct message poruka;

    if(fork() != 0) {
        //proces roditelj

        for(int i = 0; i < 10; i++) {
            int broj;

            printf("Unesite ceo broj: ");
            scanf("%d", &broj);

            poruka.tip = 1;
            sprintf(poruka.sadrzaj, "%d", broj);

            msgsnd(msgid, &poruka, sizeof(poruka.sadrzaj), 0);
        }

        wait(NULL);

        msgctl(msgid, IPC_RMID, NULL);
    }
    else {
        //proces dete

        for(int i = 0; i < 10; i++) {
            int broj;

            msgrcv(msgid, &poruka, MSG_LEN, 1, 0);
            broj = atoi(poruka.sadrzaj);

            int suma = 0;
            while(broj > 0) {
                suma += broj % 10;
                broj /= 10;
            }

            printf("Suma cifara prosledjenog broja je: %d\n-----------------------------------------------\n", suma);
            fflush(stdout);
        }

        exit(EXIT_SUCCESS);
    }

    return 0;
}