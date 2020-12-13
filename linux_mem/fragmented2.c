#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define PRIME1 130027
#define PRIME2 130043
#define SLACK  102407 

int main (int argc, const char *argv[])
{
	char temps[256];
	long int requested = 0;
	char *waste = NULL;
	char *waste2 = NULL;
	char *waste3 = NULL;
	char *waste4 = NULL;

	int counter=0;
	if (argc > 1)
	  {
		  strncpy (temps, argv[1], 255);
		  temps[255] = 0;
		  requested = strtol (temps, NULL, 10);
		  printf ("Requesting %lu bytes \n", requested);
	  }
	else requested=SLACK;
	printf("Processing.... \n");
	for(counter =0;; counter++)
	{
		waste = malloc(PRIME1);
		if (!(waste)) break;
		
		waste3 = malloc(PRIME2);
		if (!(waste3)) break;
	
		waste2 = malloc(requested+counter*2);
		if (!(waste2)) break;
		waste4 = malloc(requested+counter*3);
		if (!(waste4)) break;
		
		free(waste2);
		free(waste4);
	};
	printf("Done... \n");
	malloc_stats();
	getchar();
	return 0;
}
