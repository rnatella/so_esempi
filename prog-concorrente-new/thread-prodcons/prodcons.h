#ifndef _HEADER_H
#define _HEADER_H

#include <pthread.h>

struct ProdCons {

    int buffer;

    int buffer_pieno;
    int buffer_vuoto;

    pthread_mutex_t mutex;
    pthread_cond_t cv_produttori;
    pthread_cond_t cv_consumatori;
};

void produzione(struct ProdCons *, int valore);
int consumazione(struct ProdCons *);

#endif