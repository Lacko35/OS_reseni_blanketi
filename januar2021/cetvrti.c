#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void searchDir(char* path, int N, int M) {
	DIR* dp;
	if((dp = opendir(path))	== NULL) {
		perror("Doslo je do greske prilikom otvaranja direktorijuma!\n");
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
		
		if(S_ISREG(statbuff.st_mode)) {
			if(statbuff.st_size > N && strlen(obj) > M) {
				printf("Apsolutna putanja je <%s> a velicina datoteke je <%d>\n", obj, statbuff.st_size);
			}
		}
		else if(S_ISDIR(statbuff.st_mode) && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
			searchDir(obj, N, M);
		}
	}
	
	closedir(dp);
}

int main(int argc, char* argv[]) {
	if(argc < 4) {
		perror("Nedovoljan broj argumenata komandne linije!\n");
		exit(1);
	}
	
	searchDir(argv[1], argv[2], argv[3]);

	return 0;
}
