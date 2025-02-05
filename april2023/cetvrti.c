#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void dirSeach(char* absPath, int lvl, int* maxLen, char* dat) {
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuff;

    if((dp = opendir(absPath)) == NULL) {
        perror("Doslo je do greske prilikom otvaranja programa!\n");
        exit(EXIT_FAILURE);
    }

    while((dirp = readdir(dp)) != NULL) {
        char obj[512];
        strcpy(obj, absPath);
        strcat(obj, "/");
        strcat(obj, dirp->d_name);

        if(stat(obj, &statbuff) < 0) {
            perror("Doslo je do greske prilikom prikupljanja informacija o objektu!\n");
        }

        if(S_ISREG(statbuff.st_mode) && statbuff.st_size > *maxLen) {
            *maxLen = statbuff.st_mode;
            strcpy(dat, dirp->d_name);
        }

        if(strcmp("..", dirp->d_name) != 0 && strcmp(".", dirp->d_name) != 0 && lvl < 5) {
            dirSeach(obj, lvl + 1, maxLen, dat);
        }
    }

    closedir(dp);
}

int main() {
    char naziv[86];
    int duzina = 0;

    dirSeach("/home/osboxes/Desktop/C", 0, duzina, naziv);

    printf("Naziv najvece regularne datoteke je: %s\n", naziv);

    return 0;
}