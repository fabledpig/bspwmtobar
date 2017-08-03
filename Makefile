CC = gcc
CFLAGS = -O3 -std=c99 -Wall

bspwmtobar: bspwmtobar.o desktop.o tile.o
	$(CC) $(CFLAGS) bspwmtobar.o desktop.o tile.o -o bspwmtobar

bspwmtobar.o: bspwmtobar.c
	$(CC) $(CFLAGS) -c bspwmtobar.c

desktop.o: desktop.c
	$(CC) $(CFLAGS) -c desktop.c

tile.o: tile.c
	$(CC) $(CFLAGS) -c tile.c

install: bspwmtobar
	install -D -m 755  bspwmtobar /usr/bin/bspwmtobar

uninstall:
	rm -f /usr/bin/bspwmtobar

clean:
	rm -f bspwmtobar *.o *~
