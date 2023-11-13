#ifndef _HEADER_
#define _HEADER_

#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2
#define MESSAGGIO 3

typedef struct {
	long tipo;
	char mess[40];
} Messaggio;

void ReceiveBloc (int queue, char * text);
void SendSincr (int queue, char * text);
void initServiceQueues();
void removeServiceQueues();

void Mittente(int queue);
void Ricevente(int queue);

#endif
