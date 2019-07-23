//////////////////////////////////////////////////////////////////////
// window.cpp -- C++ NCurses Class Library -- Window
// Date: Mon Jul 22 22:01:51 2019   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "cppcurses.hpp"

#include <curses.h>

Window::Window(CppCurses *main,void *win) : win(win) {
	mainf = true;	
}

Window::~Window() {
	if ( mainf ) {
		main->fini();
		mainf = false;
	}
}

size_t
Window::printf(const char *format,...) {
	va_list ap;
	size_t n;

	va_start(ap,format);
	n = vwprintw((WINDOW*)win,format,ap);
	va_end(ap);
	return n;
}

size_t
Window::mvprintf(int y,int x,const char *format,...) {
	va_list ap;
	size_t n;

	wmove((WINDOW*)win,y,x);
	va_start(ap,format);
	n = vwprintw((WINDOW*)win,format,ap);
	va_end(ap);
	return n;
}

Window&
Window::refresh() {

	assert(win);
	wrefresh((WINDOW*)win);
	return *this;
}

Window&
Window::erase() {
	werase((WINDOW*)win);
	return *this;
}

Window&
Window::clear() {
	wclear((WINDOW*)win);
	return *this;
}

// End window.cpp
