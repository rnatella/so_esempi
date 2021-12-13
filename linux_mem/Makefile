all: loop loop-calloc chunk

loop: loop.c
	gcc -m32 loop.c -o loop

loop-calloc: loop-calloc.c
	gcc loop-calloc.c -o loop-calloc

chunk: chunk.c
	gcc chunk.c -o chunk

clean:
	rm -f loop
	rm -f loop-calloc
	rm -f chunk
