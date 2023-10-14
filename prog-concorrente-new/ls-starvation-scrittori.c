#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MUTEXL 0
#define SYNCH 1

#define NUM_LETTORI 5
#define NUM_OPERAZIONI_LETTURA 3

#define NUM_SCRITTORI 2
#define NUM_OPERAZIONI_SCRITTURA 3


struct lettscritt {

    int num_lettori;
    int val;
};



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


void Inizio_Lettura(struct lettscritt * p, int id_sem) {

    Wait_Sem(id_sem, MUTEXL);

    p->num_lettori++;

    if (p->num_lettori == 1) {
        Wait_Sem(id_sem, SYNCH);
    }

    Signal_Sem(id_sem, MUTEXL);
}

void Fine_Lettura(struct lettscritt * p, int id_sem) {
    
        Wait_Sem(id_sem, MUTEXL);
    
        p->num_lettori--;
    
        if (p->num_lettori == 0) {
            Signal_Sem(id_sem, SYNCH);
        }
    
        Signal_Sem(id_sem, MUTEXL);
}

void Inizio_Scrittura(struct lettscritt * p, int id_sem) {

    Wait_Sem(id_sem, SYNCH);
}

void Fine_Scrittura(struct lettscritt * p, int id_sem) {

    Signal_Sem(id_sem, SYNCH);
}



void Lettura(struct lettscritt * p, int id_sem) {

    Inizio_Lettura(p, id_sem);

    // operazione di lettura
    sleep(1);
    int val = p->val;
    printf("[LETTORE %d] Ho letto: %d\n", getpid(), val);

    Fine_Lettura(p, id_sem);
}

void Scrittura(struct lettscritt * p, int id_sem) {

    Inizio_Scrittura(p, id_sem);

    // operazione di scrittura
    sleep(1);
    int val = rand() % 10;
    p->val = val;
    printf("[SCRITTORE %d] Ho scritto: %d\n", getpid(), val);

    Fine_Scrittura(p, id_sem);
}


int main() {

    int shm_key = IPC_PRIVATE;

    int shm_id = shmget(shm_key, sizeof(struct lettscritt), IPC_CREAT|0644);

    if(shm_id < 0) {
        perror("Errore shmget");
        exit(1);
    }


    int sem_key = IPC_PRIVATE;

    int sem_id = semget(sem_key, 2, IPC_CREAT|0644);

    if(sem_id < 0) {
        perror("Errore semget");
        exit(1);
    }


    semctl(sem_id, MUTEXL, SETVAL, 1);
    semctl(sem_id, SYNCH, SETVAL, 1);


    struct lettscritt * p = shmat(shm_id, NULL, 0);

    if(p == (void *) -1) {
        perror("Errore shmat");
        exit(1);
    }


    p->num_lettori = 0;
    p->val = 0;


    int pid;

    for(int i=0; i<NUM_LETTORI; i++) {
            
        pid = fork();

        if(pid == 0) {
            for(int j=0; j<NUM_OPERAZIONI_LETTURA; j++) {
                Lettura(p, sem_id);
                sleep(1);
            }
            exit(0);
        }
    }


    for(int i=0; i<NUM_SCRITTORI; i++) {
            
        pid = fork();

        if(pid == 0) {

            srand(getpid());

            for(int j=0; j<NUM_OPERAZIONI_SCRITTURA; j++) {
                Scrittura(p, sem_id);
                sleep(1);
            }

            exit(0);
        }
    }


    for(int i=0; i<NUM_LETTORI+NUM_SCRITTORI; i++) {
        wait(NULL);
    }


    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);


    return 0;
}