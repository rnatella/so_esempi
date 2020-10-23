#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void SIGhandler(int sig) {
	printf("Ricevuto SIGUSR1, esco gentilmente\n");
	exit(0);
}

void main(void) {
	signal(SIGUSR1, SIGhandler);
	while(1) {}
}

