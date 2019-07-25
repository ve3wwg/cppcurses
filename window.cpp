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
#include <map>
#include <array>

static const std::map<char,attr_t> attr_map({
	{ 'N',	A_NORMAL },		// Normal display (no highlight)
	{ 'S',	A_STANDOUT },		// Best highlighting mode of the terminal.
	{ 'U',	A_UNDERLINE },		// Underlining
	{ 'R',	A_REVERSE },		// Reverse video
	{ 'b',	A_BLINK },		// Blinking
	{ 'D',	A_DIM },		// Half bright
	{ 'B',	A_BOLD },		// Extra bright or bold
	{ 'A',	A_ALTCHARSET },		// Alternate character set
});

static std::map<char,chtype> graph_map;	// This must be initialized after initscr()

static const std::array<short,8> colour_map({
	COLOR_BLACK,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE
});

static std::map<short/*colorno*/,short/*pair*/> pair_map;
static std::map<short/*pair*/,short/*colourno*/> rev_pair;

static inline short
curs_colorno(short bg,short fg) {
	return (bg << 3) | fg;
}

static inline short
curs_background(Window::colpair_t pair) {
	short colourno = rev_pair.at(pair);
	return (colourno >> 3) & 7;
}

static inline short
curs_foreground(Window::colpair_t pair) {
	short colourno = rev_pair.at(pair);
	return colourno & 7;
}

//////////////////////////////////////////////////////////////////////
// Static method to initialize graph_map
//////////////////////////////////////////////////////////////////////

void
Window::init_maps(bool colour) {
	short pairno = 0;

	graph_map['L'] = ACS_ULCORNER;
	graph_map['l'] = ACS_LLCORNER;
	graph_map['R'] = ACS_URCORNER;
	graph_map['r'] = ACS_LRCORNER;
	graph_map['t'] = ACS_LTEE;
	graph_map['u'] = ACS_RTEE;
	graph_map['T'] = ACS_TTEE;
	graph_map['B'] = ACS_BTEE;
	graph_map['-'] = ACS_HLINE;
	graph_map['|'] = ACS_VLINE;
	graph_map['+'] = ACS_PLUS;

	if ( colour ) {
		for ( short bg = 0; bg < 8; ++bg ) {
			for ( short fg= 0; fg < 8; ++fg ) {
				short colour_no = curs_colorno(fg,bg);
				short pair = pairno++;
				init_pair(pair,colour_map[fg],colour_map[bg]);
				pair_map[colour_no] = pair;
				rev_pair[pair] = colour_no;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Return the colour pair for { fg,bg }
//////////////////////////////////////////////////////////////////////

Window::colpair_t
Window::to_colour(Colour fg,Colour bg) {
	short colour_no = curs_colorno(short(fg),short(bg));

	auto it = pair_map.find(colour_no);
	assert(it != pair_map.end());
	return colpair_t(it->second);
}

//////////////////////////////////////////////////////////////////////
// Convert attribute string to curses attributes
//////////////////////////////////////////////////////////////////////

static attr_t
to_attrs(const char *attrstr) {
	attr_t attrs = 0;
	char ch;

	if ( !attrstr || !*attrstr )
		return A_NORMAL;

	while ( (ch = *attrstr++) != 0 ) {
		auto it = attr_map.find(ch);
		if ( it != attr_map.end() )
			attrs |= it->second;
	}
	return attrs;
}

static inline void
curs_wattr_on(void *win,attr_t a) {
	wattr_on((WINDOW*)win,a,nullptr);
}

static inline void
curs_wattron(void *win,Window::colpair_t colour) {
	wattron((WINDOW*)win,COLOR_PAIR(colour));
}

static inline void
curs_wattr_off(void *win,attr_t a) {
	wattr_off((WINDOW*)win,a,nullptr);
}

static inline void
curs_wattr_set(void *win,attr_t a,int pair) {
	wattr_set((WINDOW*)win,a,pair,nullptr);
}

static inline void
curs_waddch(void *win,chtype ch) {
	waddch((WINDOW*)win,ch);
}

static inline void
curs_waddstr(void *win,const char *str) {
	waddstr((WINDOW*)win,str);
}

#undef attr_on
#undef wattr_on
#undef attr_off
#undef wattr_off
#undef attr_set
#undef wattr_set
#undef addch
#undef waddch
#undef addstr
#undef waddstr

Window::Window(CppCurses *main,void *win) : win(win) {
	mainf = true;	

	cbreak();		// Disable line buffering
	noecho();
	keypad(stdscr,TRUE);	// Recognize keys
	main->init_colours();
}

Window::~Window() {
	if ( mainf ) {
		main->fini();
		mainf = false;
	} else	{
		delwin((WINDOW*)win);
		win = nullptr;
	}
}

Window&
Window::addch(int ch) {
	curs_waddch(win,ch);
	return *this;
}

Window&
Window::addstr(const char *str) {
	curs_waddstr(win,str);
	return *this;
}

Window&
Window::addstr(const std::string& str) {
	curs_waddstr(win,str.c_str());
	return *this;
}

Window&
Window::addgrstr(const char *str) {
	char ch;

	while ( (ch = *str++) != 0 ) {
		auto it = graph_map.find(ch);
		if ( it != graph_map.end() ) {
			chtype c = it->second;
			curs_waddch(win,c);
		} else	curs_waddch(win,ch);
	}
	return *this;
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
Window::move(int y,int x) {
	wmove((WINDOW *)win,y,x);
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

Window&
Window::attr_on(const char *attrs) {
	attr_t a = to_attrs(attrs);

	curs_wattr_on(win,a);
	return *this;
}

Window&
Window::attr_off(const char *attrs) {
	attr_t a = to_attrs(attrs);

	curs_wattr_off(win,a);
	return *this;
}

Window&
Window::attr_set(const char *attrs,colpair_t pair) {
	attr_t a = to_attrs(attrs);

	curs_wattr_set(win,a,pair);
	return *this;
}

Window&
Window::colour(Colour fg,Colour bg) {
	colpair_t colour_pair = Window::to_colour(fg,bg);

	curs_wattron(win,colour_pair);
	return *this;
}

Window&
Window::fg(Colour fg) {
	Colour bg(Colour(curs_background(colour_pair)));;
	colpair_t colour_pair = Window::to_colour(fg,bg);

	curs_wattron(win,colour_pair);
	return *this;
}

Window&
Window::bg(Colour bg) {
	Colour fg(Colour(curs_foreground(colour_pair)));;
	colpair_t colour_pair = Window::to_colour(fg,bg);

	curs_wattron(win,colour_pair);
	return *this;
}

// End window.cpp
