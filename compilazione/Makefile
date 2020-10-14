programma: mylib.o main.o
	gcc mylib.o main.o -o programma

mylib.o: mylib.c mylib.h 
	gcc -c  mylib.c  -o mylib.o

main.o: main.c mylib.h
	gcc -c main.c  -o main.o

clean:
	rm -f *.o
	rm -f programma
