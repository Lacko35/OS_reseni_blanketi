#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define NUMBER_OF_LINE 10

void readFile(char* file) {
	int rbr = 0;
	FILE* f = fopen(file, "r");
				
	while(rbr < NUMBER_OF_LINE) {
		char line[86];
					
		fgets(line, 86, f);
		printf("%s\n", line);
					
		rbr++;
	}
				
	fclose(f);
}

void dirSearch(char* path, int maxLvl) {
	DIR* dp;
	if((dp = opendir(path)) == NULL) {
		perror("Doslo je do greske prilikom otvaranja direktorijuma za zadatom putanjom!\n");
		exit(1);
	}
	
	struct stat statbuff;
	
	struct dirent* dirp;
	while((dirp = readdir(dp)) != NULL) {
		char obj[512];
		strcpy(obj, path);
		strcat(obj, "/");
		strcat(obj, dirp->d_name);
		
		if(stat(obj, &statbuff) < 0) {
			perror("Doslo je do greske prilikom prikupljanja informacija!\n");
			continue;
		}
		
		if(maxLvl == 0) {
			if(S_ISREG(statbuff.st_mode)) {
				readFile(obj);
			}
			
			if(S_ISDIR(statbuff.st_mode) && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
				dirSearch(obj, maxLvl);
			}
		}
		else {
			if(S_ISREG(statbuff.st_mode)) {
				readFile(obj);
			}
			
			if(S_ISDIR(statbuff.st_mode) && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0 && maxLvl > 0) {
				dirSearch(obj, maxLvl--);
			}
		}
	}
	
	closedir(dp);
}

int main(int argc, char* argv[]) {
	if(argc < 3) {
		perror("Nedovoljan broj argumenata!\n");
		exit(1);
	}
	
	dirSearch(argv[1], atoi(argv[2]));
	
	return 0;
}
