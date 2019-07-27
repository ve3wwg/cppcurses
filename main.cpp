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

#include <memory>

#include "cppcurses.hpp"

CppCurses curses;

int
main(int argc,char **argv) {
	Window *w = curses.open();	// Main window
	assert(w);

	w->colour(Colour::White,Colour::Red);
	w->mvprintf(8,10,"Is this in colour? ");
	w->bgclear();
	w->readch();

	w->printf("Hello World!\n");
	w->colour(Colour::Yellow,Colour::Black).attr_on("B");
	w->mvprintf(10,10,"[10,10] ");
	w->attr_on("R").addch('X').attr_off("R");

	w->fg(Colour::Red).addstr("(Red)").bg(Colour::Yellow).addstr("(YellowBg)");
	w->bg(Colour::Black);
	w->readch();
	
	w->colour(Colour::Red,Colour::Blue).attr_off("R");
	w->move(12,10).addgrstr("L--T--R");
	w->move(13,10).addgrstr("|  |  |");
	w->move(14,10).addgrstr("t--+--u");
	w->move(15,10).addgrstr("|  |  |");
	w->move(16,10).addgrstr("l--B--r");
	w->readch();

	{
		Window *w2 = w->border_window(12,12,8,40);
		std::unique_ptr<Window> up_w2(w2);

		w2->mvprintf(0,0,"+ Origin of the window..");
		w2->mvprintf(1,0,"+ (1,0) In the window..");
		w2->mvprintf(6,38,".");
		w2->readch();

		w2->move_window(15,15);
		w2->readch();
	}
	w->readch();
	{
		Window *w3 = w->new_window(15,15,8,50);
		std::unique_ptr<Window> up_w3(w3);

		w3->mvprintf(0,0,"* Origin of w3..");
		w3->mvprintf(1,1," * one over..");
		w3->readch();
	}

	w->mvprintf(8,5,"Pause..");
	w->readch();

	w->erase();
	w->attr_on("U");
	w->mvprintf(12,10,"Almost done..");
	w->attr_off("N");
	w->readch();

	curses.close();	

	return 0;
}

// End main.cpp
