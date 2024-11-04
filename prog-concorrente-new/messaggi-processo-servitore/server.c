#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"


/************ LA RISORSA ************/
/* privata, allocata nella area     */
/* dati globali del processo server */
/* (è accessibile solo dal server)  */
/************************************/

#define SPENTA 0
#define ACCESA 1

struct risorsa {

    int stato;

};

struct risorsa caldaia = { SPENTA };



int leggi_temperatura(struct risorsa * caldaia);

int main() {

    key_t chiave_richieste = ftok(".", 'a');

    int coda_richieste = msgget(chiave_richieste, IPC_CREAT|0664);

    if(coda_richieste < 0) {
        perror("Errore msgget");
        exit(1);
    }


    key_t chiave_risposte = ftok(".", 'b');

    int coda_risposte = msgget(chiave_risposte, IPC_CREAT|0664);

    if(coda_risposte < 0) {
        perror("Errore msgget");
        exit(1);
    }


    /* Il server itera indefinitamente */

    srand(getpid());

    while(1) {

        messaggio richiesta;


        // Ricezione bloccante
        // (il server attende richieste)

        printf("[SERVER] In attesa di richieste...\n");

        Receive(coda_richieste, &richiesta);



        // La risposta del server varia in base
        // al tipo di richiesta che è stata ricevuta

        if(richiesta.tipo == ACCENDI) {

            printf("[SERVER] Accensione caldaia\n");

            caldaia.stato = ACCESA;

        } else if(richiesta.tipo == SPEGNI) {

            printf("[SERVER] Spegnimento caldaia\n");

            caldaia.stato = SPENTA;

        } else if(richiesta.tipo == LEGGI) {

            messaggio risposta;

            risposta.tipo = TEMPERATURA;
            risposta.valore = leggi_temperatura(&caldaia);

            printf("[SERVER] Lettura temperatura: %d °C\n", risposta.valore);

            SendAsincr(coda_risposte, &risposta);

        } else if(richiesta.tipo == TERMINA) {

            printf("[SERVER] Terminazione\n");

            exit(0);
        }

    }


    return 0;
}


int leggi_temperatura(struct risorsa * caldaia) {

    int temp;

    if(caldaia->stato == SPENTA) {

        temp = 16 + (rand() % 3);

    } else { // ACCESA

        temp = 23 + (rand() % 2);
    }

    return temp;
}

