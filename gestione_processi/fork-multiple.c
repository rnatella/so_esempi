#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>


int main() {

	pid_t pid;
	int status;
	
	for(int i=0; i<5; i++) {

		pid=fork(); 
		

		if ( pid == 0 ) {

			printf("Sono il processo figlio, con PID=%d, e Parent PID=%d\n", getpid(), getppid());

			sleep(5);

			exit(0);
		}
		
	}


	printf("Sono il processo padre, attendo la terminazione dei figli...\n");

	for(int i=0; i<5; i++) {

		pid = wait(&status);

		printf("Terminato figlio con PID=%d\n", pid);
	}

	return 0;
}


