#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    char putanja[80];
    char kljucnaRec[80];

    printf("Unesite putanju do datoteke: ");
    scanf("%s", putanja);

    printf("Unesite kljucnu rec koju trazimo u datoteci: ");
    scanf("%s", kljucnaRec);

    int pd[2];

    if(pipe(pd) < 0) {
        perror("Doslo je do greske prilikom kreiranja datavoda!\n");
        exit(EXIT_FAILURE);
    }

    if(fork() < 0) {
        perror("Doslo je do greske prilikom kreiranja procesa deteta!\n");
        exit(EXIT_FAILURE);        
    }

    if(fork() != 0) {
        close(pd[0]);

        write(pd[1], putanja, strlen(putanja) + 1);
        write(pd[1], kljucnaRec, strlen(kljucnaRec) + 1);

        wait(NULL);

        close(pd[1]);
    }
    else {
        close(pd[1]);

        char keyword[80];
        char path[80];

        read(pd[0], path, 80);
        read(pd[0], keyword, 80);

        FILE* f = fopen(path, "r");

        char linija[86];
        int redniBrojevi[100];
        int rbr = 1;
        int ind = 0;

        while(fgets(linija, 86, f)) {
            if(strstr(linija, keyword) != NULL) {
                redniBrojevi[ind++] = rbr;
            }
            rbr++;
        }

        printf("Redni broj linija u okviru kojih se nalazi kljucna rec [%s] u datoteci [%s]: ", keyword, path);
        for(int i = 0; i < ind; i++) {
            printf("%d. ", redniBrojevi[i]);
        }
    
        fclose(f);

        close(pd[0]);

        exit(EXIT_SUCCESS);
    }

    return 0;
}