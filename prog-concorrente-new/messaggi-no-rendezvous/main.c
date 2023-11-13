#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>

#include "header.h"


int main(){

	int k;
	int queue;
	pid_t pid;

	// creazione della coda
	queue = msgget(IPC_PRIVATE,IPC_CREAT|0664);


	pid = fork();		// primo figlio (ricevente)

	if (pid == 0) {

		printf("Sono il processo mittente. Il mio pid è %d \n", getpid());

		Mittente(queue);

		exit(0);
	}


	pid = fork();		// secondo figlio (ricevente)

	if (pid==0) {

		printf("Sono il processo ricevente. Il mio pid è %d \n", getpid());

		Ricevente(queue);

		exit(0);
	}

	// attesa di terminazione
	for (k=0; k<2;k++){
		wait(NULL);
	}

	// deallocazione code
	msgctl(queue,IPC_RMID,0);

	return 0;
}

void Mittente(int queue) {

	char messaggio[] = "www.unina.it";

	// invio messaggio
	printf("Il mittente chiama la send asincrona...\n");
	SendAsincr(queue, messaggio);

	printf("MESSAGGIO INVIATO: <%s>\n", messaggio);
}

void Ricevente(int queue) {

	char messaggio[40];

	// simula un ricevente ancora occupato
	sleep(1);
	printf("Il ricevente è occupato...\n");
	sleep(4);
	printf("Il ricevente è pronto...\n");

	// ricezione messaggio
	Receive(queue, messaggio);

	printf("MESSAGGIO RICEVUTO: <%s>\n", messaggio);
}
