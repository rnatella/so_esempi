#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

	while(1){
		pid_t pid = fork();
		if(pid < 0) {
			perror("Fork fallita");
			exit(1);
		}
	}
}
