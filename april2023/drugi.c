#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if(argc < 3) {
        perror("Nedovoljan broj argumenata komandne linije!\n");
        exit(EXIT_FAILURE);
    }

    if(fork() == 0) {
        execlp(argv[1], argv[1], argv[2], NULL);
    }
    else {
        int exit_code;

        wait(&exit_code);
        printf("Kod sa kojim je proces dete izasao: %d\n", exit_code);
    }

    return 0;
}