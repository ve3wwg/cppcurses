//////////////////////////////////////////////////////////////////////
// window.hpp -- C++ NCurses Class Library - Window
// Date: Mon Jul 22 21:50:51 2019   (C) Warren W. Gay ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "cppcurses.hpp"

#include <string>

class CppCurses;

class Window {
	friend CppCurses;

	CppCurses	*main = nullptr;
	void		*win = nullptr;
	bool		mainf = false;

	static void init_maps();

	Window(CppCurses *main,void *win);

public:	typedef short colpair_t;

	enum class Colour {
		Black=0,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Yellow,
		White
	};

	Window();
	~Window();

	static colpair_t to_colour(Colour bg,Colour fg);	// Colour pair

	Window& erase();
	Window& clear();
	Window& refresh();
	
	Window& move(int y,int x);
	Window& addch(int ch);
	Window& addstr(const char *str);
	Window& addstr(const std::string& str);
	Window& addgrstr(const char *str);
	size_t printf(const char *format,...) __attribute((format(printf,2,3)));
	size_t mvprintf(int y,int x,const char *format,...) __attribute((format(printf,4,5)));

	Window& attr_on(const char *attrs);
	Window& attr_off(const char *attrs);
	Window& attr_set(const char *attrs);
};

#endif // WINDOW_HPP

// End window.hpp
