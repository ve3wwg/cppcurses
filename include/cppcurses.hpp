//////////////////////////////////////////////////////////////////////
// cppcurses.hpp -- C++ NCurses Class Library
// Date: Mon Jul 22 21:49:59 2019   (C) Warren W. Gay ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#ifndef CPPCURSES_HPP
#define CPPCURSES_HPP

#include "window.hpp"

class CppCurses {
	friend Window;
	
	Window		*mainw = nullptr;
	bool		openf = false;

	void fini();

public:	CppCurses();
	~CppCurses();
	bool open();
	bool close();

	Window *get_window() { return mainw; }

	int getch();
	int readch(unsigned ms=1000);
	CppCurses& yield();
};

#endif // CPPCURSES_HPP

// End cppcurses.hpp
