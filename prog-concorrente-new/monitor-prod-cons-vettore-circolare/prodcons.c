#include "prodcons.h"

#include <stdio.h>

void Produzione(MonitorProdCons * p, int valore) {

    enter_monitor(&(p->m));

    // controllare se il buffer è *vuoto*
    if(p->contatore == DIM) {
        wait_condition(&(p->m), CV_PROD);
    }

    p->buffer[p->coda] = valore;
    p->coda = (p->coda + 1) % DIM;
    p->contatore++;

    printf("[PRODUTTORE] ho prodotto %d\n", valore);



    signal_condition(&p->m, CV_CONS);

    leave_monitor(&(p->m));
}

int Consumazione(MonitorProdCons * p) {

    int valore;

    enter_monitor(&(p->m));

    // consumatore si blocca se buffer è *vuoto*
    if(p->contatore == 0) {
        wait_condition(&(p->m), CV_CONS);
    }

    valore = p->buffer[p->testa];
    p->testa = (p->testa + 1) % DIM;
    p->contatore--;

    printf("[CONSUMATORE] ho consumato %d\n", valore);


    signal_condition(&p->m, CV_PROD);

    leave_monitor(&(p->m));

    return valore;
}
