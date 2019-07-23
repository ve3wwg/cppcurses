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
Window::printw(const char *format,...) {
	va_list ap;

	va_start(ap,format);
	va_end(ap);
}

// End window.cpp
