#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_KEY 10101
#define MSG_LEN 80

struct message {
    long type;
    char text[MSG_LEN];
};

int main() {
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    struct message bafer;

    if(fork() == 0) {
        int parni = 0;
        int neparni = 0;

        while(1) {
            msgrcv(msgid, &bafer, MSG_LEN, 1, 0);

            int broj = atoi(bafer.text);

            if(broj == -1) {
                break;
            }
            else if(broj % 2 == 0) {
                parni++;
            }
            else {
                neparni++;
            }

            printf("Broj je: %d\n--------------------------------------------\n", broj);
        }

        printf("Broj parnih prosledjenih brojeva je [%d] a broj neparnih prosledjenih brojeva [%d]\n", parni, neparni);

        exit(EXIT_SUCCESS);
    }
    else {
        int n = (rand() % 10) + 1;

        for(int i = 0; i < n; i++) {
            int broj = (rand() % 50) + 1;

            sprintf(bafer.text, "%d", broj);
            bafer.type = 1;

            msgsnd(msgid, &bafer, sizeof(bafer.text), 0);
        }

        sprintf(bafer.text, "%d", -1);
        bafer.type = 1;

        msgsnd(msgid, &bafer, sizeof(bafer.text), 0);

        wait(NULL);

        msgctl(msgid, IPC_RMID, NULL);        
    }

    return 0;
}