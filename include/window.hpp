//////////////////////////////////////////////////////////////////////
// window.hpp -- C++ NCurses Class Library - Window
// Date: Mon Jul 22 21:50:51 2019   (C) Warren W. Gay ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "cppcurses.hpp"

class CppCurses;

class Window {
	friend CppCurses;

	CppCurses	*main = nullptr;
	void		*win = nullptr;
	bool		mainf = false;

	Window(CppCurses *main,void *win);

public:	Window();
	~Window();

	size_t printf(const char *format,...);
	void refresh();
};

#endif // WINDOW_HPP

// End window.hpp
