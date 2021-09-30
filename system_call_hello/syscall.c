#include <unistd.h>

/*

  Prerequisiti:

  $ sudo apt-get -y install gcc-multilib g++-multilib

  Compilare con:

  $ gcc -m32 -O0 -Wall syscall.c -o syscall

*/

int main(){

        ssize_t result;

        char hello[] = "Hello world\n";
        size_t len = sizeof(hello);

        asm volatile (
                "mov $4,%%eax;"	// EAX = 4 (__NR_write)
                "mov $1,%%ebx;"	// EBX = fd = 1
                "mov %1,%%ecx;"	// ECX = %1 = hello
                "mov %2,%%edx;"	// EDX = %2 = len
                "int $0x80;"

                : "=g" (result)
                : "g" (hello), "g" (len)    // %1 = hello, %2 = len
                :"memory", "eax", "ebx", "ecx", "edx"
        );

        return 0;
}

