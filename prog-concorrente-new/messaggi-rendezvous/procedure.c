#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "header.h"

static int queue_rts; // "static" previene la loro visibilità al di fuori del modulo
static int queue_ots;

// inizializzazione code di servizio
void initServiceQueues(){

	queue_rts = msgget(IPC_PRIVATE,IPC_CREAT|0664);
	queue_ots = msgget(IPC_PRIVATE,IPC_CREAT|0664);
}

// rimozione code di servizio
void removeServiceQueues(){

	msgctl(queue_rts,IPC_RMID,0);
	msgctl(queue_ots,IPC_RMID,0);
}

// Send Sincrona
void SendSincr (int queue, char * text){

	Messaggio m, m_rts, m_ots;

	// costruzione messaggio RTS
	m_rts.tipo=REQUEST_TO_SEND;
	strcpy(m_rts.mess,"Richiesta di invio");

	// invio messaggio RTS
	msgsnd(queue_rts, &m_rts, sizeof(Messaggio)-sizeof(long), 0);

	// ricezione OTS
	msgrcv(queue_ots, &m_ots, sizeof(Messaggio)-sizeof(long), OK_TO_SEND, 0);


	// costruzione del messaggio da trasmettere
	m.tipo=MESSAGGIO;
	strcpy(m.mess,text);

	// invio messaggio
	msgsnd(queue, &m, sizeof(Messaggio)-sizeof(long), 0);
}

// Receive Bloccante
void ReceiveBloc (int queue, char * text){

	Messaggio m, m_rts, m_ots;

	// attende RTS
	msgrcv(queue_rts, &m_rts, sizeof(Messaggio)-sizeof(long), REQUEST_TO_SEND, 0);

	// costruzione messaggio OTS
	m_ots.tipo=OK_TO_SEND;
	strcpy(m_ots.mess,"Pronto all'invio");

	// invio messaggio OTS
	msgsnd(queue_ots, &m_ots, sizeof(Messaggio)-sizeof(long), 0);


	// ricezione messaggio
	msgrcv(queue, &m, sizeof(Messaggio)-sizeof(long), 0, 0);

	// restituzione della stringa al chiamante
	strcpy(text, m.mess);
}

