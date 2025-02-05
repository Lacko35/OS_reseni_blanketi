#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define LINE_LEN 101

void dirSearch(char* absPath, int n, int m) {
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuff;

    if((dp = opendir(absPath)) == NULL) {
        perror("Doslo je do greske prilikom otvaranja zadatog direktorijuma!\n");
        exit(EXIT_FAILURE);
    }

    while((dirp = readdir(dp)) != NULL && n > 0) {
        char obj[512];
        strcpy(obj, absPath);
        strcat(obj, "/");
        strcat(obj, dirp->d_name);

        if(stat(obj, &statbuff) < 0) {
            perror("Ne mozemo prikupiti informacije o ovom objektu!\n");
            continue;
        }

        if(S_ISREG(statbuff.st_mode)) {
            FILE* f = fopen(obj, "r");
            int pom = m;
            char tmp[LINE_LEN];

            while(pom > 0) {
                fgets(tmp, LINE_LEN, f);
                tmp[strlen(tmp)] = '\0';

                printf("Linija: [%s]\n", tmp);

                pom--;
            }

            fclose(f);
            n--;
        }
        else {
            continue;
        }
    }

    closedir(dp);
}

int main(int argc, char* argv[]) {
    if(argc < 4) {
        perror("Nedovoljan broj argumenata komandne linije!\n");
        exit(EXIT_FAILURE);
    }

    dirSearch(argv[1], atoi(argv[2]), atoi(argv[3]));

    return 0;
}