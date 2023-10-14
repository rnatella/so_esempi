#ifndef _HEADER_
#define _HEADER_

#include "monitor_hoare.h"

#define CV_PROD 0
#define CV_CONS 1

typedef struct {

    int buffer;

    int buffer_pieno;
    int buffer_vuoto; // opzionale, è l'opposto di buffer_pieno

    Monitor m;

} MonitorProdCons;

void Produzione(MonitorProdCons *, int valore);
int Consumazione(MonitorProdCons *);

#endif