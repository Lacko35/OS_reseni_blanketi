#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BYTES_NO 200
#define TERMINATE_WORD "!1QUIT1!"

int main(int argc, char* argv[]) {
    if(argc < 3) {
        perror("Doslo je do greske prilikom kreiranja datavoda\n");
        exit(EXIT_FAILURE);
    }

    int pd[2];

    if(pipe(pd) < 0) {
        perror("Doslo je do greske prilikom kreiranja datavoda\n");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if(pid < 0) {
        perror("Doslo je do greske prilikom kreiranja novog procesa\n");
        exit(EXIT_FAILURE);
    }
    
    if(pid != 0) {
        close(pd[0]);

        FILE* f = fopen(argv[1], "rb");
        char bafer[201];

        while(fread(bafer, BYTES_NO, 1, f) == 1) {
            write(pd[1], bafer, BYTES_NO);
        }

        fclose(f);

        strcpy(bafer, TERMINATE_WORD);
        write(pd[1], bafer, strlen(bafer) + 1);

        wait(NULL);

        close(pd[1]);
    }
    else {
        close(pd[1]);

        FILE* f = fopen(argv[2], "wb");
        char bafer[201];

        while(1) {
            read(pd[0], bafer, BYTES_NO);

            if(strcmp(bafer, TERMINATE_WORD) == 0) {
                break;
            }

            for(int i = 0; i < BYTES_NO; i++) {
                bafer[i] ^= ((rand() % 255) + 1);
            }

            fwrite(bafer, BYTES_NO, 1, f);
        }

        fclose(f);

        close(pd[0]);

        exit(EXIT_SUCCESS);
    }

    return 0;
}