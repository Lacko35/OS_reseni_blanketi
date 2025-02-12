#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define KB 1024
#define MAX_DEPTH 3
#define SUBSTR "log"

void dirSearch(char* path, int depth) {
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuff;

    if((dp = opendir(path)) == NULL) {
        perror("Doslo je do greske prilikom otvaranja direktorijuma!\n");
        exit(EXIT_FAILURE);
    }

    while((dirp = readdir(dp)) != NULL) {
        char obj[512];
        strcpy(obj, path);
        strcat(obj, "/");
        strcat(obj, dirp->d_name);

        if(stat(obj, &statbuff) < 0) {
            perror("Doslo je do greske prilikom prikupljanja informacija!\n");
            continue;
        }

        if(S_ISREG(statbuff.st_mode) && statbuff.st_size > 15*KB) {
            if(strstr(dirp->d_name, SUBSTR) != NULL) {
                if(fork() == 0) {
                    execlp("rm", "rm", obj, NULL);
                }
                else {
                    wait(NULL);
                }
            }
        }

        if(strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0 && depth < MAX_DEPTH) {
            dirSearch(obj, depth + 1);
        }
    }

    closedir(dp);
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        perror("Nedovoljan broj argumenata!\n");
        exit(EXIT_FAILURE);
    }

    dirSearch(argv[1], 0);

    return 0;
}