
all:  lettore_scrittore

lettore_scrittore: lettore_scrittore.o procedure.o
	gcc -o lettorescrittore lettore_scrittore.o procedure.o

lettore_scrittore.o:lettore_scrittore.c header.h
	gcc -c lettore_scrittore.c

procedure.o:procedure.c header.h
	gcc -c procedure.c
clean :
	 rm -f *.o 
	 rm -f lettorescrittore
	 rm -f *~
