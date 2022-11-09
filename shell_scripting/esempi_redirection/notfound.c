#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {

 int fd = open("/home/file_che_non_esiste.txt", O_RDONLY);

 if(fd < 0) {
    perror("Non riesco ad aprire il file");
    exit(1);
 }

 return 0;
}

