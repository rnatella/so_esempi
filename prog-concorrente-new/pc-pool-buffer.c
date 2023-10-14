/*

1) Non cambiano i semafori, non cambiano i processi

2) Rimuoviamo testa/coda

3) Aggiungiamo il vettore di stato

4) Nei produttori/consumatori, aggiungiamo il ciclo while

5) Nei produttori/consumatori, scriviamo sul vettore di stato

6) Inizializziamo il vettore di stato (tutti gli elementi sono LIBERI)

*/

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
#define MUTEX_P 2
#define MUTEX_C 3


#define LIBERO    0
#define OCCUPATO  1
#define INUSO     2


#define DIM 3

typedef struct {

    int stato[DIM];

    int buffer[DIM];

} mystruct;




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


void Produttore(mystruct * p, int id_sem) {

    int indice = 0;

    Wait_Sem(id_sem, SPAZIO_DISP);

    Wait_Sem(id_sem, MUTEX_P);

    while( indice < DIM && p->stato[indice] != LIBERO ) {
        indice++;
    }
    p->stato[indice] = INUSO;

    Signal_Sem(id_sem, MUTEX_P);

    printf("Sleep....\n");
    sleep(2);

    int valore = rand() % 10;

    p->buffer[indice] = valore;
    printf("PRODUTTORE: Ho prodotto %d\n", valore);

    p->stato[indice] = OCCUPATO;

    Signal_Sem(id_sem, MSG_DISP);

}

void Consumatore(mystruct * p, int id_sem) {

    int indice = 0;

    Wait_Sem(id_sem, MSG_DISP);
    Wait_Sem(id_sem, MUTEX_C);

    while( indice<DIM && p->stato[indice] != OCCUPATO) {
        indice++;
    }

    p->stato[indice] = INUSO;

    Signal_Sem(id_sem, MUTEX_C);

    int valore;

    valore = p->buffer[indice];

    p->stato[indice] = LIBERO;

    printf("CONSUMATORE: ho consumato %d\n", valore);


    Signal_Sem(id_sem, SPAZIO_DISP);
}


int main() {

    key_t chiave_sem = IPC_PRIVATE;

    int id_sem = semget(chiave_sem, 4, IPC_CREAT | 0644);

    if(id_sem < 0) {
        perror("Errore semget");
        exit(1);
    }

    semctl(id_sem, SPAZIO_DISP, SETVAL, DIM);
    semctl(id_sem, MSG_DISP, SETVAL, 0);
    semctl(id_sem, MUTEX_P, SETVAL, 1);
    semctl(id_sem, MUTEX_C, SETVAL, 1);


    key_t chiave_mem = IPC_PRIVATE;

    int id_mem = shmget(chiave_mem, sizeof(mystruct), IPC_CREAT | 0644);

    if(id_mem < 0) {
        perror("errore shmget");
        exit(1);
    }

    mystruct * p = shmat(id_mem, NULL, 0);

    
    for(int i=0; i<DIM; i++) {
        p->stato[i] = LIBERO;
    }



    pid_t pid;

    for(int k=0; k<2; k++) {

        // CREAZIONE PRODUTTORE
        pid = fork();


        if(pid > 0) {
            // CODICE PADRE
        }
        else if(pid == 0) {
            // CODICE FIGLIO

            srand(getpid());


            for(int i=0; i<5; i++) {
                Produttore(p, id_sem);
            }
            exit(0);
        }
        else {
            perror("errore fork");
            exit(1);
        }
    }

    for(int k=0; k<2; k++) {

        // CREAZIONE CONSUMATORE
        pid = fork();


        if(pid > 0) {
            // CODICE PADRE
        }
        else if(pid == 0) {
            // CODICE FIGLIO
            for(int i=0; i<5; i++) {
                Consumatore(p, id_sem);
            }
            exit(0);
        }
        else {
            perror("errore fork");
            exit(1);
        }
    }


    for(int i=0; i<4; i++) {
        wait(NULL);
    }


    semctl(id_sem, 0, IPC_RMID);
    shmctl(id_mem, IPC_RMID, NULL);


}
