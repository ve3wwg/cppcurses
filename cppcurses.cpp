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

Window *
CppCurses::open() {

	if ( !openf ) {
		mainw = new Window(this,initscr());
		openf = true;
	}
	return mainw;
}

bool
CppCurses::close() {

	if ( mainw ) {
		delete mainw;
		mainw = nullptr;
	}
	return !mainw;
}

#undef getch

void
CppCurses::init_colours() {

	assert(!coloursf);
	if ( has_colors() ) {
		start_color();
		Window::init_maps(true);
	}
}

// End cppcurses.cpp
