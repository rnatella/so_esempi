all: start server client

start: start.c
	gcc -o start start.c

server: server.o procedure.o
	gcc -o server server.o procedure.o

client: client.o procedure.o
	gcc -o client client.o procedure.o

server.o: server.c header.h
	gcc -c -o server.o server.c

client.o: client.c header.h
	gcc -c -o client.o client.c

procedure.o: procedure.c header.h
	gcc -c -o procedure.o procedure.c

clean:
	rm -f start
	rm -f client
	rm -f server
	rm -f *.o
