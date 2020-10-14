# Compilazione

Per compilare i singoli moduli ("mylib" e il programma utente):

```
gcc -c main.c -o main.o

gcc -c mylib.c -o mylib.o
```

Per effettuare il link dei moduli (eseguibile "programma"):

```
gcc  mylib.o  main.o  -o programma
```


# Analisi del codice oggetto

I seguenti comandi forniscono informazioni sul codice oggetto:

```
objdump -d main.o
nm main.o
```

Per aggiungere informazioni di debugging ai file oggetto:
```
gcc -c -g main.c -o main.o
objdump -d -S main.o
```


# Makefile

Per compilare il programma tramite make:
```
make -f Makefile programma
```

È possibile omettere il nome del file con le regole (se si chiama esattamente `Makefile`), e il nome della regola (di default, esegue la prima regola, ossia in questo caso `programma`)
```
make
```

Per rimuovere tutti i file oggetto, si utilizzi la regola `clean`
```
make clean
```

Il file `Makefile-avanzato` è una versione alternativa, che fa uso di variabili sia definite di default (`$@`,`$^`,`$<`) sia definite dal programmatore (`CC`, `CFLAGS`, etc.).
```
make -f Makefile-avanzato
```


