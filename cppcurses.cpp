//////////////////////////////////////////////////////////////////////
// cppcurses.cpp -- C++ NCurses Class Library
// Date: Mon Jul 22 21:52:57 2019   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sched.h>

#include "cppcurses.hpp"

#include <ncurses.h>

CppCurses::CppCurses() {
}

CppCurses::~CppCurses() {

	if ( mainw ) {
		delete mainw;
		mainw = nullptr;
	}
}

void
CppCurses::fini() {
	endwin();
}

bool
CppCurses::open() {

	if ( !openf ) {
		mainw = new Window(this,initscr());
		openf = true;
	}
	return true;
}

bool
CppCurses::close() {

	if ( mainw ) {
		delete mainw;
		mainw = nullptr;
		return true;
	}
	return false;
}

#undef getch

int
CppCurses::getch() {
	int ch;

	refresh();
	ch = ::wgetch(stdscr);
	if ( ch == ERR )
		return -1;
	return ch;
}

int
CppCurses::readch(unsigned ms) {
	int ch;

	this->refresh();
	while ( (ch = getch()) == -1 )
		usleep(ms);
	return ch;
}

CppCurses&
CppCurses::yield() {
	::sched_yield();
	return *this;
}

void
CppCurses::init_colours() {

	assert(!coloursf);
	if ( has_colors() ) {
		start_color();
		Window::init_maps(true);
	}
}

CppCurses&
CppCurses::refresh() {

	Window::do_update();
	return *this;
}

Window *
CppCurses::new_window(short y,short x,short nlines,short ncols) {
	return mainw->new_window(y,x,nlines,ncols);
}

Window *
CppCurses::border_window(short y,short x,short nlines,short ncols) {
	return mainw->border_window(y,x,nlines,ncols);
}

// End cppcurses.cpp
