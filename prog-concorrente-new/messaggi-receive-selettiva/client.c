#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "header.h"

int main () {

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


    srand(getpid());

    struct mymessage msg;

    msg.type = TIPO;
    msg.x = rand() % 10;
    msg.y = rand() % 10;
    msg.pid = getpid();

    printf("CLIENT: Invio messaggio tipo = %ld, x = %d, y = %d, PID = %d\n", msg.type, msg.x, msg.y, msg.pid);


    int result = msgsnd(msgid, &msg, sizeof(struct mymessage) - sizeof(long), 0);

    if(result < 0) {
        perror("errore send");
        exit(1);
    }


    struct risposta r;

    result = msgrcv(id_risposte, &r, sizeof(struct risposta) - sizeof(long), getpid(), 0);

    printf("CLIENT: Ricevuto risposta tipo = %ld, somma = %d\n", r.type, r.somma);




}


