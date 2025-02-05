#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_LEN 256
#define MSG_KEY 10101

struct message {
    long type;
    char text[MSG_LEN];
};

int main(int argc, char* argv[]) {
    if(argc < 3) {
        perror("Nedovoljan broj argumenata komandne linije!\n");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    struct message bafer;

    int tip = atoi(argv[1]);

    FILE* f = fopen(argv[2], "w");

    while(1) {
        msgrcv(msgid, &bafer, MSG_LEN, tip, 0);
        char tmp[255];
        strcpy(tmp, bafer.text);

        if(strcmp(tmp, "11KRAJ11") == 0) {
            break;
        }

        if(tip == 1) {
            for(int i = 0; i < strlen(tmp); i++) {
                tmp[i] -= ('a' - 'A');
            }
        }
        else {
            for(int i = 0; i < strlen(tmp); i++) {
                tmp[i] += ('a' - 'A');
            }
        }

        fprintf(f, "%s\n", tmp);
        fflush(f);
    }

    fclose(f);

    exit(EXIT_SUCCESS);
}