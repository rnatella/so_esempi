#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS	5

void *PrintHello(void *threadid)
{
   printf("\n%d: Hello World!\n", (int)threadid);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	pthread_t threads[NUM_THREADS];
	int rc;

	for(int i=0; i<NUM_THREADS; i++){

		printf("Creating thread %d\n", i);

		rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);

		if (rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL);
}

