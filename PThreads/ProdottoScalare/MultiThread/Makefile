CC=gcc
LFLAGS=-pthread

all: prodottoScalareMT

prodottoScalareMT: prodottoScalareMT.c
	$(CC) $(LFLAGS) -o prodottoScalareMT prodottoScalareMT.c

clean:
	rm -rf *.o
	rm -rf prodottoScalareMT
