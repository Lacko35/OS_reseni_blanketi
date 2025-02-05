#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_LEN 256
#define MSG_KEY 10101

struct message {
    long type;
    char text[MSG_LEN];
};

int main() {
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    struct message bafer;

    int rbr = 1;
    if(fork() == 0) {
        execl("treci2", "treci2", "1", "../../sredjeno.txt", NULL);
    }
    else if(fork() == 0) {
        execl("treci2", "treci2", "2", "../../sredjeno.txt", NULL);
    }
    else {
        FILE* f = fopen("../../poruka.txt", "r");
        char buff[MSG_LEN];

        while(fgets(buff, MSG_LEN, f)) {
            strcpy(bafer.text, buff);

            if(rbr % 2 == 0) {
                bafer.type = 1;
            }
            else {
                bafer.type = 2;
            }

            msgsnd(msgid, &bafer, MSG_LEN, 0);
        }

        strcpy(bafer.text, "11KRAJ11");
        
        bafer.type = 1;
        msgsnd(msgid, &bafer, 9, 0);

        bafer.type = 2;
        msgsnd(msgid, &bafer, 9, 0);

        fclose(f);

        wait(NULL);
        wait(NULL);

        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}