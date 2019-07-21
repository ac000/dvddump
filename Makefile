dvddump: dvddump.c
	gcc -Wall -Wextra -g -O2 -std=c99 -o dvddump dvddump.c -ldvdread

clean:
	rm -f dvddump
