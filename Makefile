CC = g++
CFLAGS = -std=c++0x -Wall

bspwmtobar: bspwmtobar.o
	$(CC) $(CFLAGS) bspwmtobar.o -o bspwmtobar

bspwmtobar.o: bspwmtobar.cpp
	$(CC) $(CFLAGS) -c bspwmtobar.cpp

install: bspwmtobar
	install -D -m 755  bspwmtobar /usr/bin/bspwmtobar

uninstall:
	rm -f /usr/bin/bspwmtobar

clean:
	rm -f bspwmtobar *.o *~
