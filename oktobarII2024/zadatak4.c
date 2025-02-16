#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

void searchDir(char* path) {
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
			
			if(S_ISREG(statbuff.st_mode)) {
				if(strstr(dirp->d_name, ".sh") != NULL) {
					if(fork() == 0) {
						execlp("sh", "sh", obj, NULL);
					}
					else {
						wait(NULL);
					}
				}
			}
			
			if(S_ISDIR(statbuff.st_mode) && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
				searchDir(obj);
			}
		}
		
		closedir(dp);
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		perror("Nedovoljan broj argumenata komandne linije!\n");
		exit(EXIT_FAILURE);
	}
	
	searchDir(argv[1]);
	
	return 0;
}
