#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_LEN 50
#define MSG_KEY 10101

struct message {
    long type;
    char text[MSG_LEN];
};

int main() {
    int msgid = msgget((ssize_t)MSG_KEY, IPC_CREAT | 0666);
    struct message msg;

    if(fork() == 0) {
        FILE* f = fopen("../zbirovi_cifara.txt", "a");

        while(1) {
            msgrcv(msgid, &msg, MSG_LEN, 1, 0);

            int broj = atoi(msg.text);

            if(broj == 0) {
                break;
            }

            int suma = 0;
            while(broj != 0) {
                suma += broj % 10;
                broj /= 10;
            }

            fprintf(f, "zbir cifara broja %d je [%d]\n", atoi(msg.text), suma);
        }

        fclose(f);

        exit(EXIT_SUCCESS);
    }
    else {
        int broj;

        do {
            printf("Unesite broj: ");
            fflush(stdout);
            scanf("%d", &broj);

            if((broj >= 100 && broj <= 999) || broj == 0) {
                msg.type = 1;
                sprintf(msg.text, "%d", broj);

                msgsnd(msgid, &msg, sizeof(msg.text), 0);
            }
        }
        while(broj != 0);

        wait(NULL);

        printf("\n---------------KRAJ PROGRAMA---------------\n");

        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}