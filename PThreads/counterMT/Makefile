CC=gcc
LFLAGS=-pthread

# Nota: versioni precedenti di PThreads utilizzavano
# un flag differente ("-lpthread").
#LFLAGS=-lpthread

all: counterMT

counterMT: counterMT.c
	$(CC) $(LFLAGS) -o counterMT counterMT.c

clean:
	rm -f counterMT

