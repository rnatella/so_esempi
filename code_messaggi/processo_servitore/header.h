#ifndef _HEADER_
#define _HEADER_

#define ACCENDI 1
#define SPEGNI 2
#define LEGGI 3
#define TERMINA 4
#define TEMPERATURA 5

typedef struct {

    long tipo;
    int valore;

} messaggio;

void SendAsincr(int coda, messaggio * m);
void Receive(int coda, messaggio * m);

#endif

