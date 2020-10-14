# Progetto di esempio

Questo semplice progetto include un modulo (`vettore.cpp`) che usa la libreria `iostream` per popolare il contenuto di un vettore, e un modulo (`alg_ord.cpp`) che implementa alcuni algoritmi di ordinamento.

Per compilare il progetto senza l'uso dei meccanismi di libreria, è sufficiente il comando:
```
make
```


# Libreria statica

Per creare una libreria statica (contenente i moduli `vettore.cpp` e `alg_ord.cpp`), utilizzare il comando:
```
make -f MakefileStaticLib
```


# Libreria dinamica

Per creare una libreria dinamica (contenente i moduli `vettore.cpp` e `alg_ord.cpp`), utilizzare il comando:
```
make -f MakefileDynamicLib
```

Prima di eseguire il programma, è necessario configurare la shell per indicare il percorso della libreria, con il comando:
```
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
```
