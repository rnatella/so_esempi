#ifndef _HEADER_
#define _HEADER_

#define MESSAGGIO 1

typedef struct {
	long tipo;
	char mess[40];
} Messaggio;

void Receive (int queue, char * text);
void SendAsincr (int queue, char * text);

void Mittente(int queue);
void Ricevente(int queue);

#endif
