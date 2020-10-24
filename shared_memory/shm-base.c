#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

int main() {

  key_t chiave = ftok(".",'A');

  // crea una memoria condivisa in cui contenere un valore intero
  // il numero di bytes è pari a sizeof(int)
  int id_shm = shmget(chiave, sizeof(int), IPC_CREAT|0664);

  if(id_shm<0){
    perror("errore shmget"); exit(1);
  }


  // si accede alla variabile in memoria condivisa tramite puntatore
  // (in questo caso puntatore a intero "int *")
  int * p = shmat(id_shm, NULL, 0);

  if(p==(void *)-1) {
    perror("errore shmat");
  }


  key_t pid = fork();

  // sia il processo padre sia il figlio ottengono una copia
  // del puntatore a memoria condivisa "p"

  if(pid==0) {

    // processo figlio

    printf("FIGLIO: scrivo 123...\n");
    *p = 123;

    sleep(3);

    exit(0);

 } else if(pid>0) {

    // processo padre

    wait(NULL);

    printf("PADRE: leggo il valore: %d\n", *p);

    shmctl(id_shm, IPC_RMID, 0);

 }

}

