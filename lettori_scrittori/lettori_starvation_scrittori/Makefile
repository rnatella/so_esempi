
all:  lettore_scrittore

lettore_scrittore: lett_starv_scritt.o procedure.o
	gcc -o start lett_starv_scritt.o procedure.o

lett_starv_scritt.o:lett_starv_scritt.c header.h
	gcc -c lett_starv_scritt.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o 
	rm -f ./start
	rm *~
