.PHONY:	all clean clobber install

all:	libcppcurses.a

install: all

OBJS	= cppcurses.o window.o

LDFLAGS = -L. -lcppcurses -lpanel -lncurses

libcppcurses.a: $(OBJS)
	@rm -f libcppcurses.a	
	$(AR) r libcppcurses.a $(OBJS)

install: libcppcurses.a
	$(MKDIR) -p $(PREFIX)/include/cppcurses
	$(MKDIR) -p $(PREFIX)/lib
	$(INSTALL) cppcurses/cppcurses.hpp $(PREFIX)/include/cppcurses/cppcurses.hpp
	$(INSTALL) cppcurses/window.hpp $(PREFIX)/include/cppcurses/window.hpp
	$(INSTALL) libcppcurses.a $(PREFIX)/lib

uninstall:
	rm -fr $(PREFIX)/cppcurses/cppcurses $(PREFIX)/lib/libcppcurses.a

example: main

main: 	libcppcurses.a main.o
	$(CXX) -o main main.o $(LDFLAGS)

clean:	
	rm -f *.o *.x1o a.out core core.*

clobber: clean
	@rm -f .errs.t
	rm -f $(TARGETS)

include Makefile.incl

# End Makefile
