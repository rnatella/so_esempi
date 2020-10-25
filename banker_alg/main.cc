#include <iostream>
#include <unistd.h>

#include "banker.h"

#define NUM_RES 3

#define NUM_PROC 3

#define PROC1 0
#define PROC2 1
#define PROC3 2

#define RES1 0
#define RES2 1
#define RES3 2


void * code_p1(void *);
void * code_p2(void *);
void * code_p3(void *);

int main() {
	int claim[NUM_RES][NUM_PROC] =
		{
			{ 70, 70, 50 },
			{ 1, 1, 0 },
			{ 0, 0, 1 }
		};

	int resources[NUM_RES] = { 100, 1, 1 };

	BankerAlgorithm<NUM_RES, NUM_PROC> * banker = new BankerAlgorithm<NUM_RES, NUM_PROC>( resources, claim );



	pthread_t thread[NUM_PROC];

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&thread[0], &attr, code_p1, (void *)banker);  
	pthread_create(&thread[1], &attr, code_p2, (void *)banker);  
	pthread_create(&thread[2], &attr, code_p3, (void *)banker);  



	// wait for thread termination

	int i;
	for(i=0; i<NUM_PROC; i++) {
		pthread_join(thread[i], NULL);
	}

	std::cout << std::endl << "Simulation correctly terminated" << std::endl;
}


void * code_p1(void * b) {
	BankerAlgorithm<NUM_RES, NUM_PROC> * banker = (BankerAlgorithm<NUM_RES, NUM_PROC> *)b;

	banker->allocateResource(PROC1, RES1, 40);
	sleep(5);

	banker->allocateResource(PROC1, RES1, 30);
	banker->allocateResource(PROC1, RES2, 1);
	sleep(5);

	// release resources
	banker->terminateProcess(PROC1);
    
    pthread_exit(NULL);
}


void * code_p2(void * b) {
	BankerAlgorithm<NUM_RES, NUM_PROC> * banker = (BankerAlgorithm<NUM_RES, NUM_PROC> *)b;

	sleep(1);

	banker->allocateResource(PROC2, RES1, 40);
	sleep(5);

	banker->allocateResource(PROC2, RES1, 30);
	banker->allocateResource(PROC2, RES2, 1);
	sleep(5);

	// release resources
	banker->terminateProcess(PROC2);
    
    pthread_exit(NULL);
}


void * code_p3(void * b) {
	BankerAlgorithm<NUM_RES, NUM_PROC> * banker = (BankerAlgorithm<NUM_RES, NUM_PROC> *)b;

	sleep(2);

	banker->allocateResource(PROC3, RES1, 10);
	sleep(4);

	banker->allocateResource(PROC3, RES1, 40);
	banker->allocateResource(PROC3, RES3, 1);
	sleep(4);

	// release resources
	banker->terminateProcess(PROC3);
    
    pthread_exit(NULL);
}

