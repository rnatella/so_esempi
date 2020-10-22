#include <stdio.h>
#include <unistd.h>

int main(void)
{
 	pid_t pid, ppid;

 	pid = getpid();
 	printf("Sono il processo pid = %d\n", pid);
 
	ppid = getppid();
 	printf("Il mio processo genitore ha pid = %d\n", ppid);

 	return 0;
}
