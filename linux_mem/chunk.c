#include <stdio.h>
#include <stdlib.h>

/* Testato su Ubuntu 20.04 su processore x86-64 */

int main() {

    int size_input;
    int size_chunk;

    printf("Inserire il numero di byte da allocare:\n");
    scanf("%d", &size_input);

    void * p = malloc(size_input);

    // scrive un valore
    *(int*)p = 123;
    printf("p = %p, valore = %d\n", p, *(int*)p);


    // legge la dimensione del chunk dalla double-word
    // precedente al chunk
    // (nota: pone a 0 il bit meno significativo,
    // è da ignorare ("status"))

    size_chunk = *(int*)(p-8) & (0xFFFFFFFF - 1);

    printf("chunk size = %d\n", size_chunk);

    free(p);
}

