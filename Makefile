.PHONY:	all clean clobber install

all:	libcppcurses.a

install: all

OBJS	= cppcurses.o window.o

LDFLAGS = -L. -lcppcurses -lpanel -lncurses

libcppcurses.a: $(OBJS)
	@rm -f libcppcurses.a	
	$(AR) r libcppcurses.a $(OBJS)

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
