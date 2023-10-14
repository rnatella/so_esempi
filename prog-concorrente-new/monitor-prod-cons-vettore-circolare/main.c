#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "prodcons.h"

int main() {

    key_t chiave_shm = ftok("./prodcons", 'z');

    int id_shm = shmget(chiave_shm, sizeof(MonitorProdCons), IPC_CREAT|0644);

    if(id_shm < 0) {
        perror("Errore shmget");
        exit(1);
    }


    MonitorProdCons * p = shmat(id_shm, NULL, 0);


    init_monitor(&p->m, 2);

    p->testa = 0;
    p->coda = 0;
    p->contatore = 0;


    // 2 PRODUTTORI

    for(int i=0; i<2; i++) {


        pid_t pid = fork();

        if(pid > 0) {
                //PADRE

        } else if(pid == 0) {
                //FIGLIO

                srand(getpid());

                for(int j=0; j<5; j++) {

                    int valore = rand() % 10;

                    Produzione(p, valore);

                }

                exit(0);
        }
        else {
            perror("errore fork");
            exit(1);
        }
    }


    // questa sleep() ritarda l'avvio dei consumatori,
    // permette ai produttori di accumulare più valori
    // nel vettore di buffer
    sleep(5);

    // 2 CONSUMATORI

    for(int i=0; i<2; i++) {

        pid_t pid = fork();

        if(pid > 0) {

            // PADRE
        }
        else if(pid == 0) {

            // FIGLIO

            for(int j=0; j<5; j++) {

                int valore = Consumazione(p);

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

    remove_monitor(&p->m);

    shmctl(id_shm, IPC_RMID, NULL);

}