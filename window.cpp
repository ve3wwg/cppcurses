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
#include <sched.h>
#include <assert.h>

#include "cppcurses.hpp"

#include <curses.h>
#include <panel.h>

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

static inline void
curs_wmove(void *win,short y,short x) {
	wmove((WINDOW*)win,y,x);
}

static inline int
curs_getch() {
	return wgetch(stdscr);
}

#undef wgetch
#undef getch

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
	wattrset((WINDOW*)win,a);
	wcolor_set((WINDOW*)win,pair,nullptr);
}

static inline void
curs_waddch(void *win,chtype ch) {
	waddch((WINDOW*)win,ch);
}

static inline void
curs_waddstr(void *win,const char *str) {
	waddstr((WINDOW*)win,str);
}

static inline void
curs_wattr_get(void *win,Window::wattr_t& attrs,Window::colpair_t& pair) {
	attr_t a = 0;
	short p = 0;

	wattr_get((WINDOW*)win,&a,&p,nullptr);
	attrs = Window::wattr_t(a);
	pair = Window::colpair_t(p);
}

static inline void
curs_getyx(void *win,int& y,int& x) {
	getyx((WINDOW*)win,y,x);
}

static inline void
curs_getparyx(void *win,int& y,int& x) {
	getparyx((WINDOW*)win,y,x);
}

static inline void
curs_getbegyx(void *win,int& y,int& x) {
	getbegyx((WINDOW*)win,y,x);
}

static inline void
curs_getmaxyx(void *win,int& y,int& x) {
	getmaxyx((WINDOW*)win,y,x);
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
#undef getyx
#undef getparyx
#undef getbegyx
#undef getmaxyx

Window::Window(CppCurses *main,void *win) : main(main), win(win) {

	panel = new_panel((WINDOW*)win);
	set_panel_userptr((PANEL*)panel,this);
	mainf = true;	

	cbreak();		// Disable line buffering
	noecho();
	keypad(stdscr,TRUE);	// Recognize keys
	main->init_colours();
	curs_wattr_get(win,attr,colour_pair);
}

Window::~Window() {
	if ( mainf ) {
		main->fini();
		mainf = false;
	} else	{
		if ( sub ) {
			delwin((WINDOW*)sub);
			sub = nullptr;
		}
		del_panel((PANEL*)panel);
		delwin((WINDOW*)win);
		win = nullptr;
		this->do_update();
	}
}

Window&
Window::addch(int ch) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	curs_waddch(w,ch);
	return *this;
}

Window&
Window::addstr(const char *str) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	curs_waddstr(w,str);
	return *this;
}

Window&
Window::addstr(const std::string& str) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	curs_waddstr(w,str.c_str());
	return *this;
}

Window&
Window::addgrstr(const char *str) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	char ch;

	while ( (ch = *str++) != 0 ) {
		auto it = graph_map.find(ch);
		if ( it != graph_map.end() ) {
			chtype c = it->second;
			curs_waddch(w,c);
		} else	curs_waddch(w,ch);
	}
	return *this;
}

size_t
Window::printf(const char *format,...) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	va_list ap;
	size_t n;

	va_start(ap,format);
	n = vwprintw(w,format,ap);
	va_end(ap);
	return n;
}

size_t
Window::mvprintf(int y,int x,const char *format,...) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	va_list ap;
	size_t n;

	wmove(w,y,x);
	va_start(ap,format);
	n = vwprintw(w,format,ap);
	va_end(ap);
	return n;
}

Window&
Window::refresh() {

	assert(win);
	this->do_update();
	return *this;
}

Window&
Window::move(int y,int x) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	wmove(w,y,x);
	return *this;
}

Window&
Window::bgclear() {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	wbkgd((WINDOW*)w,attr_t(this->attr));
	return *this;
}

Window&
Window::erase() {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	werase(w);
	return *this;
}

Window&
Window::clear() {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	wclear(w);
	return *this;
}

Window&
Window::attr_on(const char *attrs) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	attr = wattr_t(to_attrs(attrs));

	curs_wattr_on(w,attr_t(attr));
	curs_wattr_get(win,attr,colour_pair);
	return *this;
}

Window&
Window::attr_off(const char *attrs) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	attr = wattr_t(to_attrs(attrs));

	curs_wattr_off(w,attr_t(attr));
	curs_wattr_get(win,attr,colour_pair);
	return *this;
}

Window&
Window::attr_set(const char *attrs,colpair_t pair) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	attr = wattr_t(to_attrs(attrs));

	curs_wattr_set(w,attr_t(attr),pair);
	curs_wattr_get(win,attr,colour_pair);
	return *this;
}

Window&
Window::colour(Colour fg,Colour bg) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	colour_pair = Window::to_colour(fg,bg);

	curs_wattron(w,colour_pair);
	curs_wattr_get(w,attr,colour_pair);
	return *this;
}

Window&
Window::fg(Colour fg) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	Colour bg(Colour(curs_background(colour_pair)));;
	colpair_t colour_pair = Window::to_colour(fg,bg);

	curs_wattron(w,colour_pair);
	curs_wattr_get(w,attr,colour_pair);
	return *this;
}

Window&
Window::bg(Colour bg) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;
	Colour fg(Colour(curs_foreground(colour_pair)));;
	colour_pair = Window::to_colour(fg,bg);

	curs_wattron(w,colour_pair);
	curs_wattr_get(w,attr,colour_pair);
	return *this;
}

Window *
Window::new_window(short y,short x,short nlines,short ncols) {
	Window *w = new Window(this,y,x,nlines,ncols);

	w->attr = this->attr;
	w->colour_pair = this->colour_pair;
	curs_wattr_set(w->win,w->attr,w->colour_pair);
	wbkgd((WINDOW*)w->win,w->attr);
	return w;
}

Window&
Window::subwindow(short y,short x,short nlines,short ncols) {
	if ( sub )
		delwin((WINDOW*)sub);
	sub = derwin((WINDOW*)win,nlines,ncols,y,x);
	curs_wattr_set(sub,attr,colour_pair);
	wbkgd((WINDOW*)sub,attr);
	return *this;
}

Window *
Window::border_window(short y,short x,short nlines,short ncols) {
	Window *w = new Window(this,y,x,nlines,ncols);

	if ( nlines > 2 && ncols > 2 ) {
		WINDOW *nw = (WINDOW*)w->win;

		curs_wmove(nw,0,0);

		w->attr = this->attr;
		w->colour_pair = this->colour_pair;
		curs_wattr_set(nw,w->attr,w->colour_pair);
		wbkgd((WINDOW*)nw,w->attr);

		curs_waddch(nw,ACS_ULCORNER);
		for ( short tx=1; tx<ncols-1; ++tx )
			curs_waddch(nw,ACS_HLINE);
		curs_waddch(nw,ACS_URCORNER);

		curs_wmove(nw,nlines-1,0);
		curs_waddch(nw,ACS_LLCORNER);
		for ( short tx=1; tx<ncols-1; ++tx )
			curs_waddch(nw,ACS_HLINE);
		curs_waddch(nw,ACS_LRCORNER);

		for ( short ty=1; ty<nlines-1; ++ty ) {
			curs_wmove(nw,ty,0);
			curs_waddch(nw,ACS_VLINE);
			curs_wmove(nw,ty,ncols-1);
			curs_waddch(nw,ACS_VLINE);
		}

		touchwin((WINDOW*)nw);
		w->sub = derwin(nw,nlines-2,ncols-2,1,1);
		curs_wattr_set(w->sub,w->attr,w->colour_pair);
	}
	return w;
}

Window::Window(Window *parent,short y,short x,short nlines,short ncols) : main(parent->main) {

	win = newwin(nlines,ncols,y,x);
	panel = new_panel((WINDOW *)win);
	set_panel_userptr((PANEL*)panel,this);
	top_panel((PANEL*)panel);
	this->erase();
}

void
Window::do_update() {
	PANEL *p = panel_above(nullptr);	// Start with bottom panel

	while ( p ) {
		Window *w = (Window*)panel_userptr(p);
		if ( w->sub ) {
			touchwin((WINDOW*)w->win);
		}
		p = panel_above((PANEL*)w->panel);
	}
	update_panels();
	doupdate();
}

Window&
Window::hide() {
	hide_panel((PANEL*)panel);
	return *this;
}

Window&
Window::show() {
	show_panel((PANEL*)panel);
	return *this;
}

Window&
Window::top() {
	top_panel((PANEL*)panel);
	return *this;
}

Window&
Window::bottom() {
	bottom_panel((PANEL*)panel);
	return *this;
}

Window&
Window::move_window(short starty,short startx) {
	move_panel((PANEL*)panel,starty,startx);
	return *this;
}

Window&
Window::get_yx(int& y,int& x) {
	curs_getyx((WINDOW*)win,y,x);
	return *this;
}

Window&
Window::sub_yx(int& y,int& x) {
	if ( sub )
		curs_getparyx(sub,y,x);
	else	y = x = 0;		// No subwindow
	return *this;
}

Window&
Window::sub_size(int& y,int& x) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	curs_getmaxyx(w,y,x);
	return *this;
}

Window&
Window::orig(int& y,int& x) {
	curs_getbegyx((WINDOW*)win,y,x);
	return *this;
}

Window&
Window::size(int& y,int& x) {
	curs_getmaxyx((WINDOW*)win,y,x);
	return *this;
}

int
Window::getch() {
	int ch;

	this->refresh();
	ch = curs_getch();
	if ( ch == ERR )
		return -1;
	return ch;
}

int
Window::readch(unsigned ms) {
	int ch;

	this->refresh();
	while ( (ch = curs_getch()) == -1 )
		usleep(ms);
	return ch;
}

Window&
Window::yield() {
	::sched_yield();
	return *this;
}

// End window.cpp
