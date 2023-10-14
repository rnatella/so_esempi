#ifndef _HEADER_
#define _HEADER_

#include "monitor_hoare.h"

#define CV_PROD 0
#define CV_CONS 1

#define DIM 5

typedef struct {

    int buffer[DIM];

    int testa;
    int coda;
    int contatore;

    Monitor m;

} MonitorProdCons;

void Produzione(MonitorProdCons *, int valore);
int Consumazione(MonitorProdCons *);

#endif