#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void readSubdir(char* fname, int velicina, char* dirPath) {
    DIR* dp = opendir(dirPath);
    struct dirent* dirp;
    struct stat statbuff;

    while((dirp = readdir(dp)) != NULL) {
        char obj[512];
        strcpy(obj, dirPath);
        strcat(obj, "/");
        strcat(obj, dirp->d_name);

        stat(obj, &statbuff);

        if(S_ISDIR(statbuff.st_mode) && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
            readSubdir(fname, velicina, obj);
        }

        if(strcmp(fname, dirp->d_name) == 0 && velicina == statbuff.st_size) {
            printf("Naziv datoteke: %s\n", fname);
        }
    }

    closedir(dp);
}

void dirSearch(char* absPath1, char* absPath2) {
    DIR* dp1, *dp2;
    struct dirent* dirp1, *dirp2;
    struct stat statbuff1, statbuff2;

    if((dp1 = opendir(absPath1)) == NULL || (dp2 = opendir(absPath2)) == NULL) {
        perror("Doslo je do greske prilikom otvaranja jedog od direktorijuma!\n");
        exit(EXIT_FAILURE);
    }

    while((dirp1 = readdir(dp1)) != NULL) {
        char obj[512];
        strcpy(obj, absPath1);
        strcat(obj, "/");
        strcat(obj, dirp1->d_name);

        if(stat(obj, &statbuff1) < 0) {
            perror("Doslo je do greske pri prikupljanju informacija o prvom objektu!\n");
            continue;
        }

        while((dirp2 = readdir(dp2)) != NULL) {
            char obj2[512];
            strcpy(obj, absPath2);
            strcat(obj, "/");
            strcat(obj, dirp2->d_name);

            if(stat(obj2, &statbuff2) < 0) {
                perror("Doslo je do greske pri prikupljanju informacija o prvom objektu!\n");
                continue;
            }

            if(S_ISDIR(statbuff2.st_mode) && strcmp(dirp2->d_name, ".") != 0 && strcmp(dirp2->d_name, "..") != 0) {
                readSubdir(dirp1->d_name, statbuff1.st_size, obj2);
            }

            if(strcmp(dirp1->d_name, dirp2->d_name) == 0 && statbuff1.st_size == statbuff2.st_size) {
                printf("Naziv datoteke je: %s\n", dirp1->d_name);
            }
        }
    }

    closedir(dp1);
    closedir(dp2);
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        perror("Nedovoljan broj argumenata komandne linije!\n");
        exit(EXIT_FAILURE);
    }
    
    dirSearch(argv[1], argv[2]);

    return 0;
}