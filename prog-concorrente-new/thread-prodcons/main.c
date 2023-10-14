#include <pthread.h>
#include <stdlib.h>

#include "prodcons.h"

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 2

void * main_produttore(void *);
void * main_consumatore(void *);

int main() {

    pthread_t produttori[NUM_PRODUTTORI];
    pthread_t consumatori[NUM_CONSUMATORI];

    struct ProdCons * p = malloc(sizeof(struct ProdCons));

    p->buffer_pieno = 0;
    p->buffer_vuoto = 1;

    pthread_mutex_init(&p->mutex, NULL);
    pthread_cond_init(&p->cv_consumatori, NULL);
    pthread_cond_init(&p->cv_produttori, NULL);

    for(int i=0; i<NUM_PRODUTTORI; i++) {
        pthread_create(&produttori[i], NULL, main_produttore, p);
    }

    for(int i=0; i<NUM_CONSUMATORI; i++) {
        pthread_create(&consumatori[i], NULL, main_consumatore, p);
    }

    for(int i=0; i<NUM_PRODUTTORI; i++) {
        pthread_join(produttori[i], NULL);
    }

    for(int i=0; i<NUM_CONSUMATORI; i++) {
        pthread_join(consumatori[i], NULL);
    }

}

void * main_produttore(void * x) {

    struct ProdCons * p = x;

    for(int i=0; i<4; i++) {

        int valore = rand() % 10;
        produzione(p, valore);
    }

    pthread_exit(NULL);
}

void * main_consumatore(void * x) {

    struct ProdCons * p = x;

    for(int i=0; i<4; i++) {

        int valore = consumazione(p);
    }

    pthread_exit(NULL);
}