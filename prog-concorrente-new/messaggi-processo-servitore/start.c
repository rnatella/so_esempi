#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {

    key_t chiave_richieste = ftok(".", 'a');

    int coda_richieste = msgget(chiave_richieste, IPC_CREAT|0664);

    if(coda_richieste < 0) {
        perror("Errore msgget");
        exit(1);
    }


    key_t chiave_risposte = ftok(".", 'b');

    int coda_risposte = msgget(chiave_risposte, IPC_CREAT|0664);

    if(coda_risposte < 0) {
        perror("Errore msgget");
        exit(1);
    }


    pid_t pid_server = fork();

    if(pid_server == 0) {

        printf("Avvio processo server\n");

        execl("./server", "server", NULL);

        perror("Errore exec server");
        exit(1);
    }


    pid_t pid_client = fork();

    if(pid_client == 0) {

        printf("Avvio processo client\n");

        execl("./client", "client", NULL);

        perror("Errore exec client");
        exit(1);
    }



    for(int i=0; i<2; i++) {
        wait(NULL);
    }


    msgctl(coda_richieste, IPC_RMID, 0);
    msgctl(coda_risposte, IPC_RMID, 0);

    return 0;
}

