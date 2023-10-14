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

    srand(getpid());

    struct mymessage msg;

    msg.type = TIPO;
    msg.x = rand() % 10;
    msg.y = rand() % 10;

    printf("CLIENT: Invio messaggio tipo = %ld, x = %d, y = %d\n", msg.type, msg.x, msg.y);


    int result = msgsnd(msgid, &msg, sizeof(struct mymessage) - sizeof(long), 0);

    if(result < 0) {
        perror("errore send");
        exit(1);
    }



}


