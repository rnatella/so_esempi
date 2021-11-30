#include <stdio.h>
#include <stdlib.h>

#define MEGABYTE 1024*1024

int main(int argc, char * argv[])
{
	void* block=NULL;
	int count=0;
        
        while(1)
	{
		block = (void *) malloc(MEGABYTE);
		if (!(block)) break;
		// il blocco rimane inutilizzato
		printf("Currently allocating %d MB\n", ++count);
	}
	printf("Done\n");
	getchar();
        exit(0);
}
