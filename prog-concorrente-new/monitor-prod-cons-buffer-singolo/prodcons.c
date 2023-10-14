#include "prodcons.h"

#include <stdio.h>

void Produzione(MonitorProdCons * p, int valore) {

    enter_monitor(&(p->m));

    // controllare se il buffer è *vuoto*
    if(p->buffer_pieno == 1) {
        wait_condition(&(p->m), CV_PROD);
    }

    p->buffer = valore;

    printf("[PRODUTTORE] ho prodotto %d\n", valore);


    p->buffer_pieno = 1;
    p->buffer_vuoto = 0;

    signal_condition(&p->m, CV_CONS);

    leave_monitor(&(p->m));
}

int Consumazione(MonitorProdCons * p) {

    int valore;

    enter_monitor(&(p->m));

    // consumatore si blocca se buffer è *vuoto*
    if(p->buffer_vuoto == 1) {
        wait_condition(&(p->m), CV_CONS);
    }

    valore = p->buffer;

    printf("[CONSUMATORE] ho consumato %d\n", valore);


    p->buffer_pieno = 0;
    p->buffer_vuoto = 1;

    signal_condition(&p->m, CV_PROD);

    leave_monitor(&(p->m));

    return valore;
}
