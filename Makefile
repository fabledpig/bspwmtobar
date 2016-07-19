CXX = clang++
CXXFLAGS = -std=c++11 -Wall

bspwmtobar: bspwmtobar.o
	$(CXX) $(CXXFLAGS) bspwmtobar.o -o bspwmtobar

bspwmtobar.o: bspwmtobar.cpp
	$(CXX) $(CXXFLAGS) -c bspwmtobar.cpp

install: bspwmtobar
	install -D -m 755  bspwmtobar /usr/bin/bspwmtobar

uninstall:
	rm -f /usr/bin/bspwmtobar

clean:
	rm -f bspwmtobar *.o *~
