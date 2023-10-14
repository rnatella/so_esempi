#include "prodcons.h"

#include <stdio.h>

void produzione(struct ProdCons * p, int valore) {
    pthread_mutex_lock(&p->mutex);

    while(p->buffer_pieno) {

        pthread_cond_wait(&p->cv_produttori, &p->mutex);
    }

    printf("[PRODUZIONE] valore = %d\n", valore);
    p->buffer = valore;

    p->buffer_pieno = 1;
    p->buffer_vuoto = 0;

    pthread_cond_signal(&p->cv_consumatori);

    pthread_mutex_unlock(&p->mutex);
}


int consumazione(struct ProdCons *p) {
    pthread_mutex_lock(&p->mutex);

    while(p->buffer_vuoto) {
        pthread_cond_wait(&p->cv_consumatori, &p->mutex);
    }

    int valore = p->buffer;

    printf("[CONSUMAZIONE] valore = %d\n", valore);

    p->buffer_pieno = 0;
    p->buffer_vuoto = 1;

    pthread_cond_signal(&p->cv_produttori);

    pthread_mutex_unlock(&p->mutex);
}