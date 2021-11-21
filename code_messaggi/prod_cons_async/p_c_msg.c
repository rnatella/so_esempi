
/*********PRODUTTORE-CONSUMATORE MEDIANTE SCAMBIO DI MESSAGGI******/
/*Il programma gestisce la comunicazione tra due processi, modello asincrono
  viene inviato un burst di messaggi e stampato lo stato della coda
  Header file:header.h
  Programma chiamante:p_c_msg.c
  Modulo delle procedure:procedure.c
  Direttive per la compilazione dei moduli:Makefile
*/

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

	int queue;
	pid_t pid;

	// creazione coda di comunicazione
	queue = msgget(IPC_PRIVATE, IPC_CREAT|0664);


	pid = fork();		// primo figlio (produttore)

	if (pid==0) {

		printf("sono il produttore. Il mio pid %d \n",getpid());

		Produttore(queue);

		exit(0);
	}

	pid = fork();		// secondo figlio (consumatore)
	if (pid==0){

		printf("sono il figlio consumatore. Il mio pid %d \n",getpid());

		Consumatore(queue);

		exit(0);
	}

	// attesa di terminazione
	for (int k=0; k<2;k++){
		wait(NULL);
	}

	// deallocazione code
	msgctl(queue,IPC_RMID,0);

	return 0;
}

void Produttore(int queue) {

	char messaggio[40];

	for(int i = 0; i < 10; i++){

		sprintf(messaggio, "stringa %d", i);

		usleep(100);

		SendAsincr(queue, messaggio);

		printf("MESSAGGIO INVIATO: <%s>\n", messaggio);
	}

}

void Consumatore(int queue) {

	char messaggio[40];

	sleep(1);

	for(int i = 0; i < 10; i++){

		Receive(queue, messaggio);

		printf("MESSAGGIO RICEVUTO: <%s>\n", messaggio);

		//printMsgInfo(queue);
	}

}


