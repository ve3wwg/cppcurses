//////////////////////////////////////////////////////////////////////
// main.cpp -- Test
// Date: Mon Jul 22 22:18:38 2019   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "cppcurses.hpp"

CppCurses curses;

int
main(int argc,char **argv) {
	bool okf;

	okf = curses.open();
	assert(okf);

	Window *w = curses.get_window();

	w->printf("Hello World!\n");
	w->mvprintf(10,10,"[10,10] X ");
	w->refresh();

	sleep(5);
	okf = curses.close();	

	return 0;
}

// End main.cpp
