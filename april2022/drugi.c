#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pd1[2], pd2[2];

    if(pipe(pd1) < 0 || pipe(pd2) < 0) {
        perror("Doslo je do greske prilikom kreiranja datavoda!\n");
        exit(EXIT_FAILURE);
    }

    if(fork() == 0) {
        close(pd1[1]);
        close(pd2[0]);

        for(int i = 0; i < 10; i++) {
            char bafer[80];

            read(pd1[0], bafer, 80);

            int broj = atoi(bafer);

            if(broj % 3 == 0) {
                write(pd2[1], bafer, 80);
            }
        }

        close(pd1[0]);
        close(pd2[1]);

        exit(EXIT_SUCCESS);
    }
    else {
        close(pd1[0]);
        close(pd2[1]);

        for(int i = 0; i < 10; i++) {
            int broj = (rand() % 100) + 200;
            char bafer[80];

            sprintf(bafer, "%d", broj);
            write(pd1[1], bafer, 80);

            read(pd2[0], bafer, 80);
            printf("Broj je: %s\n-----------------------------\n", bafer);
        }

        wait(NULL);

        close(pd1[1]);
        close(pd2[0]);
    }

    return 0;
}