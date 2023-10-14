#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>

int main () {

    key_t chiave = ftok(".", 'a');

    int msgid = msgget(chiave, IPC_CREAT | 0644);

    if(msgid < 0) {
        perror("errore msgget");
        exit(1);
    }


    key_t chiave_risposte = ftok(".", 'b');

    int id_risposte = msgget(chiave_risposte, IPC_CREAT | 0644);

    if(id_risposte < 0) {
        perror("errore msgget risposte");
        exit(1);
    }



    pid_t pid;

    pid = fork();

    if(pid > 0) {
        // PADRE
    }
    else if(pid == 0) {
        // FIGLIO
        execl("./server", "server", NULL);
    }
    else {
        perror("errore fork");
        exit(1);
    }


    for(int i = 0; i<2; i++) {
        pid = fork();

        if(pid > 0) {
            // PADRE
        }
        else if (pid == 0) {
            // FIGLIO
            execl("./client", "client", NULL);
        }
        else {
            perror("errore fork");
            exit(1);
        }
    }


    wait(NULL);
    wait(NULL);
    wait(NULL);

    msgctl(msgid, IPC_RMID, NULL);
    msgctl(id_risposte, IPC_RMID, NULL);

}