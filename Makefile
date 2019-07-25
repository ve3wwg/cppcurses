.PHONY:	all clean clobber install

TARGETS = main

all:	$(TARGETS)

install: all

OBJS	= main.o cppcurses.o window.o
#XOBJS	= system.x1o dir.o

LDFLAGS = -lncurses -lpanel

main: $(OBJS)
	$(CXX) -o main $(OBJS) $(LDFLAGS)

clean:	
	rm -f *.o *.x1o a.out core core.*

clobber: clean
	@rm -f .errs.t
	rm -f $(TARGETS)

-include Makefile.incl

Makefile.deps: Makefile
	$(CXX) $(CXXFLAGS) -MM 	$(OBJ:.o=.cpp) >Makefile.deps

# End Makefile
