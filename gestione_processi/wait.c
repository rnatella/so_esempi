#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>


int main() {

	pid_t pid;
	int status;
	
	pid=fork(); 
		

	if ( pid > 0 ) {

                printf("\nSono il processo padre, con PID=%d, e Parent PID=%d\n", getpid(), getppid());

		/* Utilizzare "wait(NULL)" se non interessa il codice di ritorno del figlio */

		wait(&status);

		printf("\nIl figlio %d ha terminato l'esecuzione, ", pid);

		if (WIFEXITED(status))
			printf("con stato: %d\n\n", WEXITSTATUS(status));
		else
			printf("involontariamente!\n\n");
		
	}
	else if ( pid == 0 ) {

                printf("\nSono il processo figlio, con PID=%d, e Parent PID=%d\n", getpid(), getppid());

		printf("Attendo prima di terminare...\n");
	
		sleep(5);

		printf("Termino con codice di ritorno '3'\n");

		exit(3);
	}
	if ( pid == -1 ) {

		perror("Generazione del processo fallita");
		return -1;
	}

	return 0;
}


