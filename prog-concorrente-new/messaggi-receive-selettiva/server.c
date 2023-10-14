#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "header.h"

int main() {

    key_t chiave = ftok(".", 'a');

    int msgid = msgget(chiave, 0);

    if(msgid < 0) {
        perror("errore msgget");
        exit(1);
    }


    key_t chiave_risposte = ftok(".", 'b');

    int id_risposte = msgget(chiave_risposte, 0);

    if(id_risposte < 0) {
        perror("errore msgget risposte");
        exit(1);
    }


    for(int i=0; i<2; i++) {

        struct mymessage msg;

        int risultato = msgrcv(msgid, &msg, sizeof(struct mymessage) - sizeof(long), 0, 0);

        if(risultato < 0) {
            perror("errore receive");
            exit(1);
        }

        printf("SERVER: Ricevo messaggio tipo = %ld, x = %d, y = %d, PID = %d\n", msg.type, msg.x, msg.y, msg.pid);


        int somma = msg.x + msg.y;

        struct risposta r;

        r.somma = somma;
        r.type = msg.pid;

        printf("SERVER: Invio risposta tipo = %ld, somma = %d\n", r.type, r.somma);

        risultato = msgsnd(id_risposte, &r, sizeof(struct risposta) - sizeof(long), 0);

        if(risultato < 0) {
            perror("errore send risposta");
            exit(1);
        }
    }
    
}