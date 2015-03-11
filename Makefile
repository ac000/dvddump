dvddump: dvddump.c
	gcc -Wall -g -O2 -std=c99 -o dvddump dvddump.c -ldvdread

clean:
	rm -f dvddump
