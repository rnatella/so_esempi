#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

#include "header.h"

void SendAsincr(int queue, char * text) {

	Messaggio m;

	// costruzione del messaggio da trasmettere
	m.tipo=MESSAGGIO;
	strcpy(m.mess,text);

	// invio messaggio
	msgsnd(queue,(void*)&m,sizeof(Messaggio)-sizeof(long),0);

}

void Receive(int queue, char * text) {

	Messaggio m;

	// ricezione messaggio
	msgrcv(queue, (void *) &m, sizeof(Messaggio)-sizeof(long), MESSAGGIO, 0);

	// restituisce la stringa del messaggio al chiamante
	strcpy(text, m.mess);
}

void printMsgInfo(int queue){

	struct msqid_ds mid;

	msgctl(queue,IPC_STAT,&mid);

	char *time_sender = ctime(&mid.msg_stime);
	char *time_receiver = ctime(&mid.msg_rtime);
	char *time_ctime = ctime(&mid.msg_ctime);

	printf("Time Sender: %sTime Receiver: %sTime Ctime: %s",time_sender,time_receiver,time_ctime);
	printf("Number of messages: %lu\n\n",mid.msg_qnum);
}

