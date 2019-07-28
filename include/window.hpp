//////////////////////////////////////////////////////////////////////
// window.hpp -- C++ NCurses Class Library - Window
// Date: Mon Jul 22 21:50:51 2019   (C) Warren W. Gay ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdint.h>
#include "cppcurses.hpp"
#include <string>

class CppCurses;

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

class Window {
public:
	typedef short colpair_t;	// Local definition of colour pair
	typedef uint32_t wattr_t;	// Local definition of attr_t
protected:
	friend CppCurses;

	CppCurses	*main = nullptr;
	void		*win = nullptr;
	void		*sub = nullptr;
	void		*panel = nullptr;
	bool		mainf = false;
	wattr_t		attr = 0;
	colpair_t	colour_pair = 0;

	static void init_maps(bool colour);

	Window(CppCurses *main,void *win);
	Window(Window *parent,short y,short x,short nlines,short ncols);

	static colpair_t to_colour(Colour bg,Colour fg);	// Colour pair
	static void do_update();

public:	Window();
	~Window();

	Window& bgclear();
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

	Window& colour(Colour fg,Colour bg);
	Window& fg(Colour fg);
	Window& bg(Colour bg);
	Window& attr_on(const char *attrs);
	Window& attr_off(const char *attrs);
	Window& attr_set(const char *attrs,colpair_t = 0);

	Window *new_window(short y,short x,short nlines=0,short ncols=0);
	Window *border_window(short y,short x,short nlines=0,short ncols=0);
	Window& subwindow(short y,short x,short nlines=0,short ncols=0);
	Window& hide();
	Window& show();
	Window& top();
	Window& bottom();
	Window& move_window(short starty,short startx);

	Window& get_yx(int& y,int& x);
	Window& orig(int& y,int& x);
	Window& size(int& y,int& x);

	Window& sub_yx(int& y,int& x);
	Window& sub_size(int& y,int& x);

	int getch();
	int readch(unsigned ms=100);

	Window& yield();
};

#endif // WINDOW_HPP

// End window.hpp
