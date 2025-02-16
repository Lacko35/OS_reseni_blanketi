#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define DUZINA_LINIJE 86

int main(int argc, char* argv[]) {
		int fd1[2];
		int fd2[2];
		
		if(pipe(fd1) < 0 || pipe(fd2) < 0) {
			perror("Doslo je do greske prilikom kreiranja datavoda!\n");
			exit(EXIT_FAILURE);
		}
		
		if(fork() < 0) {
			perror("Doslo je do greske prilikom kreiranja novog procesa!\n");
			exit(EXIT_FAILURE);
		}
		else if(fork() != 0) {
			close(fd1[0]);
			close(fd2[1]);
			
			char putanja[100];
			char kljucnaRec[20];
			
			printf("Unesite putanju do datoteke: ");
			scanf("%s", putanja);
			
			printf("Unesite kljucnu rec pretrage: ");
			scanf("%s", kljucnaRec);
			
			write(fd1[1], putanja, strlen(putanja) + 1);
			write(fd1[1], kljucnaRec, strlen(kljucnaRec) + 1);
			
			int redovi[100];
			int velicina = 0;
			read(fd2[0], redovi, sizeof(redovi));
			read(fd2[0], &velicina, sizeof(velicina));
			
			printf("-------------------------------------\nRedovi u okviru kojih se nalazi kljucna rec: ");
			for(int i = 0; i < velicina; i++) {
				printf("%d ", redovi[i]);
			}
			printf("\n-------------------------------------\n");
		
			wait(NULL);
			
			close(fd1[1]);
			close(fd2[0]);
		}
		else {
			close(fd1[1]);
			close(fd2[0]);
			
			char putanja[100];
			char kljucnaRec[20];
			
			read(fd1[0], putanja, 100);
			read(fd1[0], kljucnaRec, 20);
			
			FILE* f = fopen(putanja, "r");
			char recenica[DUZINA_LINIJE];
			
			int index = 0;
			int niz[100];
			int rbr = 1;
			
			while(fgets(recenica, DUZINA_LINIJE, f)) {
				if(strstr(recenica, kljucnaRec) != NULL) {
						niz[index++] = rbr;
				}
				rbr++;
			}
			
			write(fd2[0], niz, sizeof(niz));
			write(fd2[0], &index, sizeof(index));
			
			fclose(f);
			
			close(fd1[0]);
			close(fd2[1]);
			
			exit(EXIT_SUCCESS);
		}
		
		return 0;
}
