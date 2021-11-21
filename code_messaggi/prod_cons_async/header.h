#ifndef __HEADER
#define __HEADER

#define MESSAGGIO 1

typedef struct {
	long tipo;
	char mess[40];
} Messaggio;

void Produttore(int queue);
void Consumatore(int queue);

void SendAsincr(int queue, char * text);
void Receive(int queue, char * text);
void printMsgInfo(int queue);

#endif

