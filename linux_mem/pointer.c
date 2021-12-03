#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {

	while(1) {

		char input[20];

		printf("Inserisci un indirizzo in base 16: ");
		scanf("%s", input);

		errno = 0;

		char * address = (char*)strtol(input, NULL, 16);

		if(errno != 0) {
			printf("Errore, input non valido\n");
			continue;
		}

		printf("Il contenuto della memoria all'indirizzo %p è: %x\n", address, *address);

	}
}
