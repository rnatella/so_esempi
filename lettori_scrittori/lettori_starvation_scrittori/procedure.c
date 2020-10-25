			/*-----IMPLEMENTAZIONE DELLE PROCEDURE-------*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


/*******PROCEDURE PER IL BLOCCO E LO SBLOCCO DEI SEMAFORI******/


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


/*********PROCEDURE DI LETTURA E SCRITTURA*********/

void InizioLettura(int sem,Buffer*buf){
	Wait_Sem(sem,MUTEX);
        buf->numlettori=buf->numlettori+1;
        
	if (buf->numlettori==1) //se si tratta del primo lettore blocca gli scrittori      	
		Wait_Sem(sem,SYNCH);
        Signal_Sem(sem,MUTEX);

    }

void FineLettura(int sem, Buffer*buf){
        Wait_Sem(sem,MUTEX);
        buf->numlettori=buf->numlettori-1;

        if (buf->numlettori==0)
     		Signal_Sem(sem,SYNCH);
        Signal_Sem(sem,MUTEX);
}


void InizioScrittura(int sem){
	Wait_Sem(sem,SYNCH);
}


void FineScrittura (int sem){
	Signal_Sem(sem,SYNCH);
}


void Scrittore(int sem, Buffer* buf) {

	       InizioScrittura(sem);

	       /*********Scrittura********/
               struct timeval t1;
               struct timezone t2;
               gettimeofday(&t1,&t2); //per avere un valore diverso ad ogni produzione
               msg val =t1.tv_usec;
               buf->messaggio = val;
	       sleep(1);
	       printf("Valore scritto=<%ld> \n", buf->messaggio);
	       FineScrittura(sem);
}


void Lettore (int sem, Buffer* buf) {

		InizioLettura(sem,buf);

		/*********Lettura********/
		sleep(1); // per simulare un ritardo di lettura
                printf("Valore letto=<%ld>, numero lettori=%d \n",buf->messaggio,buf->numlettori);
		FineLettura(sem,buf);
}

