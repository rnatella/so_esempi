#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_mysyscall 335

long mysyscall(pid_t pid, char * buffer, size_t buf_size) {

        return  syscall(__NR_mysyscall, pid, buffer, buf_size);
}

int main() {

        long err;
        char buf_nomeprocesso[16];

	pid_t pid;
	printf("Inserisci il PID di un processo: ");
	scanf("%d", &pid);

        printf("Invocazione mysyscall(%d, buffer, buf_size)\n", pid);
        err = mysyscall( pid, buf_nomeprocesso, sizeof(buf_nomeprocesso) );

        if(err) {
                perror("mysyscall fallita");
                return -1;
        }

        printf("Risultato mysyscall: %s\n", buf_nomeprocesso);

        return 0;
}
