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

		printf("Termino subito\n");
		
		exit(3);
	}
	else if ( pid == 0 ) {

		sleep(5);

                printf("\nSono il processo figlio, con PID=%d, e Parent PID=%d\n", getpid(), getppid());

	}
	if ( pid == -1 ) {

		perror("Generazione del processo fallita");
		return -1;
	}

	return 0;
}


