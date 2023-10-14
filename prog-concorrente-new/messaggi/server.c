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


    struct mymessage msg;

    int risultato = msgrcv(msgid, &msg, sizeof(struct mymessage) - sizeof(long), 0, 0);

    if(risultato < 0) {
        perror("errore receive");
        exit(1);
    }

    printf("SERVER: Ricevo messaggio tipo = %ld, x = %d, y = %d\n", msg.type, msg.x, msg.y);

}