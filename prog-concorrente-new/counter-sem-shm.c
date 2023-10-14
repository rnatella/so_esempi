#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


void Wait_Sem (int id_sem, int numsem)     {
	struct sembuf sem_buf;

	sem_buf.sem_num = numsem;
	sem_buf.sem_op = -1;		// Decrementa di -1
	sem_buf.sem_flg = 0;

	semop(id_sem, &sem_buf, 1);	//semaforo rosso
}


void Signal_Sem (int id_sem, int numsem)     {
	struct sembuf sem_buf;

	sem_buf.sem_num = numsem;
	sem_buf.sem_op = 1;		// Incrementa di +1
	sem_buf.sem_flg = 0;

	semop(id_sem, &sem_buf, 1);   //semaforo verde
}


int main()
{

    key_t chiave = IPC_PRIVATE;

    int id = shmget(chiave, sizeof(int), IPC_CREAT | 0644);

    if(id < 0) {
        perror("ERRORE SHMGET");
        exit(1);
    }

    int * p = shmat(id, NULL, 0);

    *p = 0;




    key_t chiave_sem = IPC_PRIVATE;

    int id_sem = semget(chiave_sem, 1, IPC_CREAT | 0664);

    if(id_sem < 0) {
        perror("ERRORE SEMGET");
        exit(1);
    }


    semctl(id_sem, 0, SETVAL, 1);






    pid_t pid;
    pid = fork();

    if(pid > 0) {
        // CODICE PROCESSO PADRE

    }
    else if(pid == 0) {

        // CODICE PROCESSO FIGLIO

        for(int i=0; i<100000; i++) {

            // INIZIO SEZIONE CRITICA
            Wait_Sem(id_sem, 0);
            *p = *p + 1;
            Signal_Sem(id_sem, 0);
            // FINE SEZIONE CRITICA
        }

        exit(0);

    }
    else {  // pid < 0

        // ERRORE
        perror("ERRORE FORK");
        exit(1);
    }


    pid = fork();

    if(pid > 0) {
        // CODICE PROCESSO PADRE

    }
    else if(pid == 0) {

        // CODICE PROCESSO FIGLIO

        for(int i=0; i<100000; i++) {

            // INIZIO SEZIONE CRITICA
            Wait_Sem(id_sem, 0);
            *p = *p + 1;
            Signal_Sem(id_sem, 0);
            // FINE SEZIONE CRITICA
        }

        exit(0);

    }
    else {  // pid < 0

        // ERRORE
        perror("ERRORE FORK");
        exit(1);
    }


    wait(NULL);
    wait(NULL);


    printf("COUNTER = %d\n", *p);


    shmctl(id, IPC_RMID, NULL);
    semctl(id_sem, 0, IPC_RMID);


}






