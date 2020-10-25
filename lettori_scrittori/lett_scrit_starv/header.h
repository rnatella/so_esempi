			/*----------HEADER FILE---------*/


#define DIM 16
#define NUM_VOLTE 3
#define SYNCH 0
#define MUTEXS 1
#define MUTEXL 2
#define MUTEX  3

typedef long msg;

            
typedef struct {
 	  	 int numlettori;
 	  	 int numscrittori;
 	  	 msg messaggio;
} Buffer;



void Wait_Sem(int, int );   
void Signal_Sem (int, int );
void InizioLettura (int, Buffer*);
void FineLettura(int, Buffer*);
void InizioScrittura(int, Buffer*);
void FineScrittura(int, Buffer*);
void Lettore(int, Buffer*);
void Scrittore(int, Buffer*);
