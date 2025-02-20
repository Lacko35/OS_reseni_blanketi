#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

#define ITERATIONS 10

int main(int argc, char* argv[]) {
	int fd[2];
	if(pipe(fd) < 0) {
		perror("Doslo je do greske prilikom kreiranja datavoda!\n");
		exit(1);
	}
	
	pid_t proccess = fork();
	if(proccess < 0) {
		perror("Doslo je do greske prilikom kreiranja procesa deteta!\n");
		exit(1);	
	}
	else if(proccess != 0) {
		//proces roditelj
		
		close(fd[0]);
		
		for(int i = 0; i < ITERATIONS; i++) {
			int broj = rand() % 100;
			write(fd[1], &broj, sizeof(broj));
		}
		
		wait(NULL);
		
		close(fd[1]);
	}
	else {
		//proces dete
		
		close(fd[1]);
		
		for(int i = 0; i < ITERATIONS; i++) {
			int broj;
			read(fd[0], &broj, sizeof(broj));
			
			if(broj % 2 == 0) {
				printf("PARAN BROJ!\n");
			}
			else {
				printf("%d\n", broj);
			}
		}
		
		close(fd[0]);
		
		exit(0);
	}
	
	return 0;
}
