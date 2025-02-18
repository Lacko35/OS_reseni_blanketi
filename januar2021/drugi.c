#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

#define ITERATIONS 5

int main(int argc, char* argv[]) {
	int fd1[2];
	int fd2[2];
	
	if(pipe(fd1) < 0 || pipe(fd2) < 0 ) {
		perror("Doslo je do greske prilikom kreiranja datavoda!\n");
		exit(1);
	}
	
	pid_t procc = fork();
	
	if(procc < 0) {
		perror("Doslo je do greske prilikom kreiranja procesa!\n");
		exit(1);	
	}
	else if(procc != 0) {
		close(fd1[0]);
		close(fd2[1]);
		
		for(int i = 0; i < ITERATIONS; i++) {
			int broj = rand() % 100;
			write(fd1[1], &broj, sizeof(broj));
		}
		
		int brParnih = 0;
		int br;
		
		while(read(fd2[0], &br, sizeof(br)) != 0) {
			if(br % 2 == 0) {
				brParnih++;
			}
		}
		
		printf("Broj parnih brojeva je: %d\n", brParnih);
		
		wait(NULL);
		
		close(fd[1]);
		close(fd2[0]);
	}
	else {
		close(fd1[1]);
		close(fd2[0]);
		
		for(int i = 0; i < ITERATIONS; i++) {
			int broj;
			read(fd1[0], &broj, sizeof(broj));
			
			if(broj % 2 == 0) {
				write(fd2[1], &broj, sizeof(broj));
			}
		}
		
		close(fd1[0]);
		close(fd2[1]);
		
		exit(0);
	}
	
	return 0;
}
