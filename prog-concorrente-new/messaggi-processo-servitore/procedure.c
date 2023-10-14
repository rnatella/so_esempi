#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#include "header.h"

void SendAsincr(int queue, messaggio * m) {

	// invio messaggio
	msgsnd(queue, (void*)m, sizeof(messaggio)-sizeof(long), 0);
}

void Receive(int queue, messaggio * m) {

	// ricezione messaggio
	msgrcv(queue, (void *)m, sizeof(messaggio)-sizeof(long), 0, 0);
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

