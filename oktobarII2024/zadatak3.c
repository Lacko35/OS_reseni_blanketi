#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_KEY 10101
#define MSG_LEN 86

struct message {
	long type;
	char text[MSG_LEN];
};

int main(int argc, char* argv[]) {
		int msgid = msgget((key_t)MSG_KEY, IPC_CREAT | 0666);
		
		if(msgid < 0) {
			perror("Doslo je do greske prilikom kreiranja/otvaranja reda poruka!\n");
			exit(EXIT_FAILURE);
		}
		
		struct message buffer;
		
		if(fork() == 0) {
			//prvi proces dete 
			FILE* f = fopen("../../prva.txt", "r");
			
			char line[MSG_LEN];
			while(fgets(line, MSG_LEN, f)) {
				strcpy(buffer.text, line);
				buffer.type = 1;
				
				msgsnd(msgid, &buffer, strlen(line) + 1, 0);
			}
			
			fclose(f);
			
			exit(EXIT_SUCCESS);
		}
		else if(fork() == 0) {
			//drugi proces dete
			FILE* f = fopen("../../druga.txt", "r");
			
			char line[MSG_LEN];
			while(fgets(line, MSG_LEN, f)) {
				strcpy(buffer.text, line);
				buffer.type = 2;
				
				msgsnd(msgid, &buffer, strlen(line) + 1, 0);
			}
			
			fclose(f);
			
			exit(EXIT_SUCCESS);			
		}
		else {
			char line[MSG_LEN];
			
			while(msgrcv(msgid, &buffer, MSG_LEN, 0, 0)) {
				strcpy(line, buffer.text);
				
				if(buffer.type == 1) {
						FILE* f = fopen("../../prva_mod.txt", "w");
						
						for(int i = 0; i < strlen(line); i++) {
								if(line[i] >= 'a' && line[i] <= 'z') {
										line[i] -= 32;
								}
						}
						
						fputs(line, f);
						fflush(f);
						
						fclose(f);
				}
				else if(buffer.type == 2) {
						FILE* f = fopen("../../druga_mod.txt", "w");
						
						for(int i = 0; i < strlen(line); i++) {
								if(line[i] >= 'A' && line[i] <= 'Z') {
										line[i] += 32;
								}
						}
						
						fputs(line, f);
						fflush(f);
						
						fclose(f);				
				}
			}
			
			wait(NULL);
			wait(NULL);
			
			msgctl(msgid, IPC_RMID, NULL);
		}
		
		return 0;
}
