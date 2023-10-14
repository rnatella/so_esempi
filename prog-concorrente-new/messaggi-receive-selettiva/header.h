#ifndef _HEADER_
#define _HEADER_

struct mymessage {
    long type;
    int x;
    int y;
    pid_t pid; // serve per le risposte
};

struct risposta {
    long type; // metteremo il pid del client destinatario
    int somma;
};

#define TIPO 1

#endif