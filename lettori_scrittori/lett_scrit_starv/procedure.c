			/*------IMPLEMENTAZIONE DELLE PROCEDURE--------*/

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"



void Wait_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}

void Signal_Sem (int id_sem,int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}

//Procedure di inizio e fine lettura

void InizioLettura(int sem,Buffer*buf){
	Wait_Sem(sem,MUTEXL);
        buf->numlettori=buf->numlettori+1;
        
	if (buf->numlettori==1) //se si tratta del primo lettore blocca gli scrittori      	
		Wait_Sem(sem,SYNCH);
        Signal_Sem(sem,MUTEXL);

    }

void FineLettura(int sem, Buffer*buf){
        Wait_Sem(sem,MUTEXL);
        buf->numlettori=buf->numlettori-1;

        if (buf->numlettori==0)
     		Signal_Sem(sem,SYNCH);
        Signal_Sem(sem,MUTEXL);
}



//Procedure di inizio e fine scrittura

void InizioScrittura(int sem,Buffer*buf){
	Wait_Sem(sem,MUTEXS);
        buf->numscrittori=buf->numscrittori+1;
        
	if (buf->numscrittori==1) //se si tratta del primo lettore blocca gli scrittori      	
		Wait_Sem(sem,SYNCH);
        Signal_Sem(sem,MUTEXS);
	Wait_Sem(sem,MUTEX);

    }

void FineScrittura(int sem, Buffer*buf){
	Signal_Sem(sem,MUTEX);
        Wait_Sem(sem,MUTEXS);
        buf->numscrittori=buf->numscrittori-1;

        if (buf->numscrittori==0)
     		Signal_Sem(sem,SYNCH);
        Signal_Sem(sem,MUTEXS);
}



void Scrittore(int sem, Buffer *buf){

	InizioScrittura(sem,buf);
	struct timeval t1;
    	struct timezone t2;
 	gettimeofday(&t1,&t2);    //valore diverso ad ogni produzione
    	buf->messaggio =t1.tv_usec;
	sleep(1);
    	printf ("Valore scritto: <%ld> \n", buf->messaggio);
	FineScrittura(sem,buf);
}

void Lettore (int sem, Buffer* buf) {

	InizioLettura(sem,buf);

	/*********Lettura********/
	sleep(1); // per simulare un ritardo di lettura
        printf("Valore letto=<%ld>, numero lettori=%d \n",buf->messaggio,buf->numlettori);
	FineLettura(sem,buf);
}


