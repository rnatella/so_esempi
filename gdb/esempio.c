#include <stdio.h>
#include "string.h"

int tester(int* c, int k) {
	printf("x[%d] = %d\n", k, c[k]);
}

int main(int argc, char* argv[]) {
	int i, j, k;
	int x[1000];

	for(i = 0; i < 10000; ++i){
		x[i] = i;
	}

	printf("Enter integer in 0..9999: ");
	scanf("%d", k);

	tester(x, k);
}

