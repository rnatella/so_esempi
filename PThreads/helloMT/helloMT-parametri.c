#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS	5

struct dati {
	int valore;
};

struct uscita {
	int valore;
};


void *PrintHello(void *p)
{

   struct dati * d = p;

   printf("\n%d: Hello World!\n", d->valore);

   free(d);


   struct uscita * x = malloc(sizeof(struct uscita));

   x->valore = 100;


   pthread_exit(x);
}

int main(int argc, char *argv[])
{

	pthread_t threads[NUM_THREADS];
	int rc;

	for(int i=0; i<NUM_THREADS; i++){

		printf("Creating thread %d\n", i);

		struct dati * d = malloc(sizeof(struct dati));

		d->valore = i;

		rc = pthread_create(&threads[i], NULL, PrintHello, d);

		if (rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}


	for(int i=0; i<NUM_THREADS; i++) {

		struct uscita * x;

		pthread_join(threads[i], (void*)&x);

		printf("Terminazione thread\n");

		printf("Ho ricevuto %d\n", x->valore);

		free(x);

	}

	pthread_exit(NULL);
}

