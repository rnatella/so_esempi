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
		
		sleep(20);	/* Durante questo intervallo di tempo, il figlio ha terminato l'esecuzione
				   ed è nello stato zombie */

		wait(&status);

		printf("\nIl figlio %d ha terminato l'esecuzione, ", pid);

		if (WIFEXITED(status))
			printf("con stato: %d\n\n", WEXITSTATUS(status));
		else
			printf("involontariamente!\n\n");
		
	}
	else if ( pid == 0 ) {

                printf("\nSono il processo figlio, con PID=%d, e Parent PID=%d\n", getpid(), getppid());

		printf("Termino subito\n");
		exit(3);

	}
	if ( pid == -1 ) {

		perror("Generazione del processo fallita");
		return -1;
	}

	return 0;
}


