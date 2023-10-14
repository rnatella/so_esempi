#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SPAZIO_DISP 0
#define MSG_DISP 1





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


void Produttore(int * p, int id_sem) {

    Wait_Sem(id_sem, SPAZIO_DISP);

    int valore = rand() % 10;

    printf("PRODUTTORE: ho prodotto %d\n", valore);
    *p = valore;

    Signal_Sem(id_sem, MSG_DISP);

}

void Consumatore(int * p, int id_sem) {

    Wait_Sem(id_sem, MSG_DISP);

    int valore = *p;
    printf("CONSUMATORE: ho consumato %d\n", valore);

    Signal_Sem(id_sem, SPAZIO_DISP);
}


int main() {

    key_t chiave_sem = IPC_PRIVATE;

    int id_sem = semget(chiave_sem, 2, IPC_CREAT | 0644);

    if(id_sem < 0) {
        perror("Errore semget");
        exit(1);
    }

    semctl(id_sem, SPAZIO_DISP, SETVAL, 1);
    semctl(id_sem, MSG_DISP, SETVAL, 0);


    key_t chiave_mem = IPC_PRIVATE;

    int id_mem = shmget(chiave_mem, sizeof(int), IPC_CREAT | 0644);

    if(id_mem < 0) {
        perror("errore shmget");
        exit(1);
    }

    int * p = shmat(id_mem, NULL, 0);


    srand(time(NULL));

    pid_t pid;

    // CREAZIONE PRODUTTORE
    pid = fork();


    if(pid > 0) {
        // CODICE PADRE
    }
    else if(pid == 0) {
        // CODICE FIGLIO
        for(int i=0; i<3; i++) {
            Produttore(p, id_sem);
        }
        exit(0);
    }
    else {
        perror("errore fork");
        exit(1);
    }


    // CREAZIONE CONSUMATORE
    pid = fork();


    if(pid > 0) {
        // CODICE PADRE
    }
    else if(pid == 0) {
        // CODICE FIGLIO
        for(int i=0; i<3; i++) {
            Consumatore(p, id_sem);
        }
        exit(0);
    }
    else {
        perror("errore fork");
        exit(1);
    }

    for(int i=0; i<2; i++) {
        wait(NULL);
    }


    semctl(id_sem, 0, IPC_RMID);
    shmctl(id_mem, IPC_RMID, NULL);


}
