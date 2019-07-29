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
#include <stdint.h>
#include <assert.h>

#include <cppcurses/cppcurses.hpp>

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

static std::map<Key,uint32_t> rkeymap;
static std::map<uint32_t,Key> keymap({
#ifdef KEY_BREAK
	{ KEY_BREAK,	Key::Break }, 
#endif
#ifdef KEY_SRESET
	{ KEY_SRESET,	Key::Sreset }, 
#endif
#ifdef KEY_RESET
	{ KEY_RESET,	Key::Reset }, 
#endif
#ifdef KEY_DOWN
	{ KEY_DOWN,	Key::Down }, 
#endif
#ifdef KEY_UP
	{ KEY_UP,	Key::Up }, 
#endif
#ifdef KEY_LEFT
	{ KEY_LEFT,	Key::Left }, 
#endif
#ifdef KEY_RIGHT
	{ KEY_RIGHT,	Key::Right }, 
#endif
#ifdef KEY_HOME
	{ KEY_HOME,	Key::Home }, 
#endif
#ifdef KEY_BACKSPACE
	{ KEY_BACKSPACE, Key::Backspace }, 
#endif
#ifdef KEY_F0
	{ KEY_F0,	Key::F0 }, 
#endif
#ifdef KEY_F
	{ KEY_F(1),	Key::F1 }, 
	{ KEY_F(2),	Key::F2 }, 
	{ KEY_F(3),	Key::F3 }, 
	{ KEY_F(4),	Key::F4 }, 
	{ KEY_F(5),	Key::F5 }, 
	{ KEY_F(6),	Key::F6 }, 
	{ KEY_F(7),	Key::F7 }, 
	{ KEY_F(8),	Key::F8 }, 
	{ KEY_F(9),	Key::F9 }, 
	{ KEY_F(10),	Key::F10 }, 
	{ KEY_F(11),	Key::F11 }, 
	{ KEY_F(12),	Key::F12 }, 
#endif
#ifdef KEY_DL
	{ KEY_DL,	Key::Dl }, 
#endif
#ifdef KEY_IL
	{ KEY_IL,	Key::Il }, 
#endif
#ifdef KEY_DC
	{ KEY_DC,	Key::Dc }, 
#endif
#ifdef KEY_IC
	{ KEY_IC,	Key::Ic }, 
#endif
#ifdef KEY_EIC
	{ KEY_EIC,	Key::Eic }, 
#endif
#ifdef KEY_CLEAR
	{ KEY_CLEAR,	Key::Clear }, 
#endif
#ifdef KEY_EOS
	{ KEY_EOS,	Key::Eos }, 
#endif
#ifdef KEY_EOL
	{ KEY_EOL,	Key::Eol }, 
#endif
#ifdef KEY_SF
	{ KEY_SF,	Key::Sf }, 
#endif
#ifdef KEY_SR
	{ KEY_SR,	Key::Sr }, 
#endif
#ifdef KEY_NPAGE
	{ KEY_NPAGE,	Key::Npage }, 
#endif
#ifdef KEY_PPAGE
	{ KEY_PPAGE,	Key::Ppage }, 
#endif
#ifdef KEY_STAB
	{ KEY_STAB,	Key::Stab }, 
#endif
#ifdef KEY_CTAB
	{ KEY_CTAB,	Key::Ctab }, 
#endif
#ifdef KEY_CATAB
	{ KEY_CATAB,	Key::Catab }, 
#endif
#ifdef KEY_ENTER
	{ KEY_ENTER,	Key::Enter }, 
#endif
#ifdef KEY_PRINT
	{ KEY_PRINT,	Key::Print }, 
#endif
#ifdef KEY_LL
	{ KEY_LL,	Key::Ll }, 
#endif
#ifdef KEY_A1
	{ KEY_A1,	Key::A1 }, 
#endif
#ifdef KEY_A3
	{ KEY_A3,	Key::A3 }, 
#endif
#ifdef KEY_B2
	{ KEY_B2,	Key::B2 }, 
#endif
#ifdef KEY_C1
	{ KEY_C1,	Key::C1 }, 
#endif
#ifdef KEY_C3
	{ KEY_C3,	Key::C3 }, 
#endif
#ifdef KEY_BTAB
	{ KEY_BTAB,	Key::Btab }, 
#endif
#ifdef KEY_BEG
	{ KEY_BEG,	Key::Beg }, 
#endif
#ifdef KEY_CANCEL
	{ KEY_CANCEL,	Key::Cancel }, 
#endif
#ifdef KEY_CLOSE
	{ KEY_CLOSE,	Key::Close }, 
#endif
#ifdef KEY_COMMAND
	{ KEY_COMMAND,	Key::Command }, 
#endif
#ifdef KEY_COPY
	{ KEY_COPY,	Key::Copy }, 
#endif
#ifdef KEY_CREATE
	{ KEY_CREATE,	Key::Create }, 
#endif
#ifdef KEY_END
	{ KEY_END,	Key::End }, 
#endif
#ifdef KEY_EXIT
	{ KEY_EXIT,	Key::Exit }, 
#endif
#ifdef KEY_FIND
	{ KEY_FIND,	Key::Find }, 
#endif
#ifdef KEY_HELP
	{ KEY_HELP,	Key::Help }, 
#endif
#ifdef KEY_MARK
	{ KEY_MARK,	Key::Mark }, 
#endif
#ifdef KEY_MESSAGE
	{ KEY_MESSAGE,	Key::Message }, 
#endif
#ifdef KEY_MOVE
	{ KEY_MOVE,	Key::Move }, 
#endif
#ifdef KEY_NEXT
	{ KEY_NEXT,	Key::Next }, 
#endif
#ifdef KEY_OPEN
	{ KEY_OPEN,	Key::Open }, 
#endif
#ifdef KEY_OPTIONS
	{ KEY_OPTIONS,	Key::Options }, 
#endif
#ifdef KEY_PREVIOUS
	{ KEY_PREVIOUS,	Key::Previous }, 
#endif
#ifdef KEY_REDO
	{ KEY_REDO,	Key::Redo }, 
#endif
#ifdef KEY_REFERENCE
	{ KEY_REFERENCE, Key::Reference }, 
#endif
#ifdef KEY_REFRESH
	{ KEY_REFRESH,	Key::Refresh }, 
#endif
#ifdef KEY_REPLACE
	{ KEY_REPLACE,	Key::Replace }, 
#endif
#ifdef KEY_RESTART
	{ KEY_RESTART,	Key::Restart }, 
#endif
#ifdef KEY_RESUME
	{ KEY_RESUME,	Key::Resume }, 
#endif
#ifdef KEY_SAVE
	{ KEY_SAVE,	Key::Save }, 
#endif
#ifdef KEY_SBEG
	{ KEY_SBEG,	Key::SBeg }, 
#endif
#ifdef KEY_SCANCEL
	{ KEY_SCANCEL,	Key::SCancel }, 
#endif
#ifdef KEY_SCOMMAND
	{ KEY_SCOMMAND,	Key::SCommand }, 
#endif
#ifdef KEY_SCOPY
	{ KEY_SCOPY,	Key::SCopy }, 
#endif
#ifdef KEY_SCREATE
	{ KEY_SCREATE,	Key::SCreate }, 
#endif
#ifdef KEY_SDC
	{ KEY_SDC,	Key::SDc }, 
#endif
#ifdef KEY_SDL
	{ KEY_SDL,	Key::SDl }, 
#endif
#ifdef KEY_SELECT
	{ KEY_SELECT,	Key::Select }, 
#endif
#ifdef KEY_SEND
	{ KEY_SEND,	Key::SEnd }, 
#endif
#ifdef KEY_SEOL
	{ KEY_SEOL,	Key::SEol }, 
#endif
#ifdef KEY_SEXIT
	{ KEY_SEXIT,	Key::SExit }, 
#endif
#ifdef KEY_SFIND
	{ KEY_SFIND,	Key::SFind }, 
#endif
#ifdef KEY_SHELP
	{ KEY_SHELP,	Key::SHelp }, 
#endif
#ifdef KEY_SHOME
	{ KEY_SHOME,	Key::SHome }, 
#endif
#ifdef KEY_SIC
	{ KEY_SIC,	Key::SIc }, 
#endif
#ifdef KEY_SLEFT
	{ KEY_SLEFT,	Key::SLeft }, 
#endif
#ifdef KEY_SMESSAGE
	{ KEY_SMESSAGE,	Key::SMessage }, 
#endif
#ifdef KEY_SMOVE
	{ KEY_SMOVE,	Key::SMove }, 
#endif
#ifdef KEY_SNEXT
	{ KEY_SNEXT,	Key::SNext }, 
#endif
#ifdef KEY_SOPTIONS
	{ KEY_SOPTIONS,	Key::SOptions }, 
#endif
#ifdef KEY_SPREVIOUS
	{ KEY_SPREVIOUS, Key::SPrevious }, 
#endif
#ifdef KEY_SPRINT
	{ KEY_SPRINT,	Key::SPrint }, 
#endif
#ifdef KEY_SREDO
	{ KEY_SREDO,	Key::SRedo }, 
#endif
#ifdef KEY_SREPLACE
	{ KEY_SREPLACE,	Key::SReplace }, 
#endif
#ifdef KEY_SRIGHT
	{ KEY_SRIGHT,	Key::SRight }, 
#endif
#ifdef KEY_SRSUME
	{ KEY_SRSUME,	Key::SRsume }, 
#endif
#ifdef KEY_SSAVE
	{ KEY_SSAVE,	Key::SSave }, 
#endif
#ifdef KEY_SSUSPEND
	{ KEY_SSUSPEND,	Key::SSuspend }, 
#endif
#ifdef KEY_SUNDO
	{ KEY_SUNDO,	Key::SUndo }, 
#endif
#ifdef KEY_SUSPEND
	{ KEY_SUSPEND,	Key::Suspend }, 
#endif
#ifdef KEY_UNDO
	{ KEY_UNDO,	Key::Undo }, 
#endif
#ifdef KEY_MOUSE
	{ KEY_MOUSE,	Key::Mouse }, 
#endif
#ifdef KEY_RESIZE
	{ KEY_RESIZE,	Key::Resize }, 
#endif
#ifdef KEY_EVENT
	{ KEY_EVENT,	Key::Event }, 
#endif
});

static inline void
curs_visibility(void *win,int flag) {

	if ( flag < 0 )
		flag = 0;
	else if ( flag > 2 )
		flag = 2;

	curs_set(flag);
}

static inline void
curs_leaveok(void *win,bool flag) {
	leaveok((WINDOW*)win,flag);
}

static inline void
curs_move(void *win,int y,int x) {
	wmove((WINDOW*)win,y,x);
}

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
	int ikey = wgetch(stdscr);

	auto it = keymap.find(ikey);
	if ( it == keymap.end() )
		return ikey;		// Return ASCII
	return int(it->second);		// Return Key::*
}

#undef wgetch
#undef getch
#undef curs_set

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

	// Populate the reverse key map
	for ( auto& pair : keymap )
		rkeymap[pair.second] = pair.first;
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
	PANEL *p = panel_above(nullptr);	// Bottom panel is returned

	while ( p ) {
		Window *w = (Window*)panel_userptr(p);
		w->wposition();
		p = panel_above(p);		// Next panel up
	}

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
	curs_leaveok(w->win,false);
	curs_move(w->win,0,0);
	return w;
}

Window&
Window::subwindow(short y,short x,short nlines,short ncols) {
	if ( sub )
		delwin((WINDOW*)sub);
	sub = derwin((WINDOW*)win,nlines,ncols,y,x);
	curs_wattr_set(sub,attr,colour_pair);
	wbkgd((WINDOW*)sub,attr);
	curs_leaveok(sub,false);
	curs_move(sub,0,0);
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
		curs_leaveok(w->sub,false);
		curs_move(w->sub,0,0);
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

void
Window::wposition() {
	int ry, rx, y, x;
	
	if ( !sub )
		return;		// No subwindow to fuss about
	
	curs_getparyx(sub,ry,rx);
	curs_getyx(sub,y,x);
	curs_move(win,ry+y,rx+x);
}

Window&
Window::cursor(bool on) {
	WINDOW *w = sub ? (WINDOW*)sub : (WINDOW*)win;

	curs_visibility(w,on?1:0);
	return *this;
}

bool
Window::is_supported(Key key) {

	auto it = rkeymap.find(key);
	return it != rkeymap.end();
}

// End window.cpp
