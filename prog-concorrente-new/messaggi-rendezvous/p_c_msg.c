
/*********PRODUTTORE-CONSUMATORE MEDIANTE SCAMBIO DI MESSAGGI******/
/*Il programma gestisce la comunicazione tra due processi, un processo
  produttore ed un processo consumatore,in un modello a memoria locale.
  Lo scambio avviene mediante protocollo sincrono realizzato a partire
  da primitive asincrone e con una comunicazione indiretta.Il program
  ma fa uso di tre mailbox:
  MB1-->SCAMBIO MSG DI SERVIZIO
  MB2-->SCAMBIO MSG DI SERVIZIO
  MB3-->SCAMBIO MSG DA TRASMETTERE

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

	int k;
	int queue;
	pid_t pid;

	// creazione della coda principale (dati prodotti/consumati)
	queue = msgget(IPC_PRIVATE,IPC_CREAT|0664);

	// creazione delle code di supporto alla send sincrona (RTS/OTS)
	initServiceQueues();

	pid = fork();		// primo figlio (produttore)

	if (pid == 0) {

		printf("Sono il produttore. Il mio pid %d \n",getpid());

		Produttore(queue);

		exit(0);
	}


	pid = fork();		// secondo figlio (consumatore)

	if (pid==0) {

		printf("Sono il figlio consumatore. Il mio pid %d \n",getpid());

		Consumatore(queue);

		exit(0);
	}

	// attesa di terminazione
	for (k=0; k<2;k++){
		wait(NULL);
	}

	// deallocazione code
	msgctl(queue,IPC_RMID,0);
	removeServiceQueues();

	return 0;
}

void Produttore(int queue) {

	char messaggio[] = "www.unina.it";

	// invio messaggio
	printf("Il produttore chiama la send sincrona...\n");
	SendSincr(queue, messaggio);

	printf("MESSAGGIO INVIATO: <%s>\n", messaggio);
}

void Consumatore(int queue) {

	char messaggio[40];

	// simula un consumatore ancora occupato
	sleep(1);
	printf("Il consumatore è occupato...\n");
	sleep(4);
	printf("Il consumatore è pronto...\n");

	// ricezione messaggio
	ReceiveBloc(queue, messaggio);

	printf("MESSAGGIO RICEVUTO: <%s>\n", messaggio);
}
