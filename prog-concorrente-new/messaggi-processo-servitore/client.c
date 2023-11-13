#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

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



    messaggio richiesta;
    messaggio risposta;

    sleep(2);


    // Accensione caldaia

    richiesta.tipo = ACCENDI;

    printf("\n");
    printf("[CLIENT] Invio richiesta di accensione\n");

    SendAsincr(coda_richieste, &richiesta);



    sleep(2);

    // Lettura della temperatura

    richiesta.tipo = LEGGI;

    printf("\n");
    printf("[CLIENT] Invio richiesta di lettura\n");

    SendAsincr(coda_richieste, &richiesta);

    printf("[CLIENT] In attesa di risposta\n");

    Receive(coda_risposte, &risposta);

    printf("[CLIENT] Ricevuto lettura: %d °C\n", risposta.valore);



    sleep(2);

    // Spegnimento caldaia

    richiesta.tipo = SPEGNI;

    printf("\n");
    printf("[CLIENT] Invio richiesta di spegnimento\n");

    SendAsincr(coda_richieste, &richiesta);



    sleep(2);

    // Fine della simulazione

    richiesta.tipo = TERMINA;

    printf("\n");
    printf("[CLIENT] Invio richiesta di terminazione\n");

    SendAsincr(coda_richieste, &richiesta);



    printf("[CLIENT] Terminazione\n");

    return 0;
}

