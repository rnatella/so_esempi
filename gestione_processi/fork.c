#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int variabile = 123;

int main() {

	pid_t pid;	// Il tipo "pid_t" è un typedef ad un tipo intero
	
	pid = fork(); 
		
	if (pid > 0) {

		printf("\nSono il processo padre, con PID=%d, e Parent PID=%d\n", getpid(), getppid());

		printf("Padre: imposto la variable a 0\n");

		variabile = 0;
		
		sleep(10);
	}
	else if (pid == 0) {

		sleep(5);
			
	 	printf("\nSono il processo figlio, con PID=%d, e Parent PID=%d\n", getpid(), getppid());

		printf("Figlio: il valore della variabile è %d\n", variabile);

	}

	else if (pid == -1) {

		perror("Generazione del processo fallita");
		return -1;
	}

	return 0;
}


