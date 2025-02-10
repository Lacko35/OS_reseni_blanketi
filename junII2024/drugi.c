#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    if(argc == 1) {
        perror("Nedovoljan broj argumenata komandne linije!\n");
        exit(EXIT_FAILURE);
    }
    
    int pd1[2];
    int pd2[2];

    if(pipe(pd1) < 0) {
        perror("Doslo je do greske prilikom kreiranja datavoda pd1!\n");
        exit(EXIT_FAILURE);
    }

    if(pipe(pd2) < 0) {
        perror("Doslo je do greske prilikom kreiranja datavoda pd2!\n");
        exit(EXIT_FAILURE);
    }

    int proc_child1 = fork();
    int proc_child2 = fork();

    if(proc_child1 < 0) {
        perror("Doslo je do greske prilikom kreiranja procesa deteta!\n");
        exit(EXIT_FAILURE);
    }

    if(proc_child2 < 0) {
        perror("Doslo je do greske prilikom kreiranja procesa deteta!\n");
        exit(EXIT_FAILURE);
    }

    int params = argc;

    if(proc_child1 == 0) {
        close(pd1[1]);
        close(pd2[0]);

        char fname[86];
        read(pd1[0], fname, 86);

        char line[86];
        char param[86];

        for(int i = 1; i < argc; i++) {
            read(pd1[0], param, 86);
            int bool = 1;
            int lineNO = 1;

            FILE* f = fopen(fname, "r");

            while(fgets(line, 86, f)) {
                if(strstr(line, param)) {
                    sprintf(param, "%d", lineNO);
                    write(pd2[1], param, sizeof(param));
                    
                    bool = 0;
                }

                lineNO++;

                if(!bool) {
                    break;
                }
            }

            fclose(f);
        }  

        wait(NULL); 

        close(pd1[0]);
        close(pd2[1]);
    }
    else if(proc_child2 == 0) {
        close(pd1[0]);
        close(pd1[1]);
        close(pd2[1]);

        int suma = 0;
        char buff[86];

        while(read(pd2[0], buff, 86) != 0) {
            suma += atoi(buff);
        }

        printf("Suma rednih brojeva datoteka je: %d\n", suma);

        close(pd2[0]);
    }
    else {
        close(pd1[0]);
        close(pd2[0]);
        close(pd2[1]);

        for(int i = 1; i < argc; i++) {
            char buff[86];
            strcpy(buff, argv[i]);

            write(pd1[1], buff, strlen(buff) + 1);
        }

        wait(NULL);

        close(pd1[1]);
    }

    return 0;
}