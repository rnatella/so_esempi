#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>

#define SEGSIZE 100

void usage();
void writeshm(char *segptr, char *text);
void readshm(char *segptr);
void removeshm(int shmid);
void changemode(int shmid, char *mode) ;


int main(int argc, char *argv[])
{
        key_t key;
        int   shmid, cntr;
        char  *segptr;

        if(argc == 1) {
                usage();
                return 0;
        }

        /* Crea una chiave unica */
        key = ftok(".", 'S');

        /* Crea il segmento, restituendo errore se già esiste */
        if((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0664)) == -1) 
        {
                printf("Il segmento esiste già - apertura come client\n");

                /* Il segmento già esiste e viene aperto da client */
                if((shmid = shmget(key, SEGSIZE, 0)) == -1) 
                {
                        perror("shmget");
                        return -1;
                }
        }
        else
        {
                printf("Creazione di un nuovo segmento di memoria condivisa\n");
        }

        /* Collega il segmento creato al processo corrente */
        if((segptr = shmat(shmid, 0, 0)) == (void *) -1)
        {
                perror("shmat");
                return -1;
        }
        
        switch(tolower(argv[1][0]))
        {
                case 'w': writeshm(segptr, argv[2]);
                          break;
                case 'r': readshm(segptr);
                          break;
                case 'd': removeshm(shmid);
                          break;
                case 'm': changemode(shmid, argv[2]);
                          break;
                default: usage();

        }
	//sleep(5);
	return 0;
}

void writeshm(char *segptr, char *text)
{
        strcpy(segptr, text);
        printf("Scritto...\n");
}

void readshm(char *segptr)
{
        printf("Contenuto: %s\n", segptr);
}

void removeshm(int shmid)
{
        shmctl(shmid, IPC_RMID, 0);
        printf("Segmento marcato come da eliminare\n");
}

void changemode(int shmid, char *mode) 
{
        struct shmid_ds myshmds;

        /* Restituisce il descrittore */
        shmctl(shmid, IPC_STAT, &myshmds);

        /* Visualizza i vecchi permessi */
        printf("I vecchi peremessi erano: %o\n", myshmds.shm_perm.mode);

        /* Converte e carica i nuovi permessi */ 
        sscanf(mode, "%o", (unsigned int*) &myshmds.shm_perm.mode);

        /* Modifica i permessi */
        shmctl(shmid, IPC_SET, &myshmds);

        printf("I nuovi permessi sono : %o\n", myshmds.shm_perm.mode);
}

void usage()
{
        fprintf(stderr, "shmtool - Un utility per manipolare memorie condivise\n");
        fprintf(stderr, "\nUSO:  shmtool (w)rite <text>\n");
        fprintf(stderr, "                (r)ead\n");
        fprintf(stderr, "                (d)elete\n");
        fprintf(stderr, "                (m)ode change <octal mode>\n");
}

