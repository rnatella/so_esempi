all: loop loop-calloc

loop: loop.c
	gcc -m32 loop.c -o loop

loop-calloc: loop-calloc.c
	gcc loop-calloc.c -o loop-calloc

clean:
	rm -f loop
	rm -f loop-calloc
