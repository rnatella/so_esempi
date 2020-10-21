#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {

	pid_t pid;
	int status;
	
	pid = fork(); 
		
	if (pid == -1) {

		perror("Generazione del processo fallita");
		exit(1);

	}	
	if (pid == 0) {

		execl("/bin/ls", "ls", "-l", NULL);

		/* Se exec() va a buon fine, il codice da questo
		 * punto non verrà mai eseguito */

		perror("Exec fallita!!\n");

		exit(1);

	}
	else if (pid > 0) {

		wait(&status);

		printf("\nIl figlio %d ha terminato l'esecuzione ", pid);

		if ( WIFEXITED(status) ) {
			printf("con stato: %d\n\n", WEXITSTATUS(status));
		} else {
			printf("involontariamente!\n\n");
		}
	}

	return 0;
}

