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
	w->mvprintf(10,10,"[10,10] ");
	w->attr_on("R").addch('X').attr_off("R");
	w->refresh();
	curses.readch();
	
	w->addstr(" Oink!");
	w->addstr(std::string(" A key was pressed."));
	curses.readch();

	w->erase();
	w->attr_on("U");
	w->mvprintf(12,10,"Almost done..");
	w->attr_off("N");
	w->refresh();

	curses.readch();

	okf = curses.close();	

	return 0;
}

// End main.cpp
