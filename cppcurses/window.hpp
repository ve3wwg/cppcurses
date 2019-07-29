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

enum class Key {
	Break,		// Break key (unreliable)
	Sreset,		// Soft (partial) reset (unreliable)
	Reset,		// Reset or hard reset (unreliable)
	Down,		// down-arrow key
	Up,		// up-arrow key
	Left,		// left-arrow key
	Right,		// right-arrow key
	Home,		// home key
	Backspace,	// backspace key
	F0,		// Function keys.  Space for 64
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	Dl,		// delete-line key
	Il,		// insert-line key
	Dc,		// delete-character key
	Ic,		// insert-character key
	Eic,		// sent by rmir or smir in insert mode
	Clear,		// clear-screen or erase key
	Eos,		// clear-to-end-of-screen key
	Eol,		// clear-to-end-of-line key
	Sf,		// scroll-forward key
	Sr,		// scroll-backward key
	Npage,		// next-page key
	Ppage,		// previous-page key
	Stab,		// set-tab key
	Ctab,		// clear-tab key
	Catab,		// clear-all-tabs key
	Enter,		// enter/send key
	Print,		// print key
	Ll,		// lower-left key (home down)
	A1,		// upper left of keypad
	A3,		// upper right of keypad
	B2,		// center of keypad
	C1,		// lower left of keypad
	C3,		// lower right of keypad
	Btab,		// back-tab key
	Beg,		// begin key
	Cancel,		// cancel key
	Close,		// close key
	Command,	// command key
	Copy,		// copy key
	Create,		// create key
	End,		// end key
	Exit,		// exit key
	Find,		// find key
	Help,		// help key
	Mark,		// mark key
	Message,	// message key
	Move,		// move key
	Next,		// next key
	Open,		// open key
	Options,	// options key
	Previous,	// previous key
	Redo,		// redo key
	Reference,	// reference key
	Refresh,	// refresh key
	Replace,	// replace key
	Restart,	// restart key
	Resume,		// resume key
	Save,		// save key
	SBeg,		// shifted begin key
	SCancel,	// shifted cancel key
	SCommand,	// shifted command key
	SCopy,		// shifted copy key
	SCreate,	// shifted create key
	SDc,		// shifted delete-character key
	SDl,		// shifted delete-line key
	Select,		// select key
	SEnd,		// shifted end key
	SEol,		// shifted clear-to-end-of-line key
	SExit,		// shifted exit key
	SFind,		// shifted find key
	SHelp,		// shifted help key
	SHome,		// shifted home key
	SIc,		// shifted insert-character key
	SLeft,		// shifted left-arrow key
	SMessage,	// shifted message key
	SMove,		// shifted move key
	SNext,		// shifted next key
	SOptions,	// shifted options key
	SPrevious,	// shifted previous key
	SPrint,		// shifted print key
	SRedo,		// shifted redo key
	SReplace,	// shifted replace key
	SRight,		// shifted right-arrow key
	SRsume,		// shifted resume key
	SSave,		// shifted save key
	SSuspend,	// shifted suspend key
	SUndo,		// shifted undo key
	Suspend,	// suspend key
	Undo,		// undo key
	Mouse,		// Mouse event has occurred
	Resize,		// Terminal resize event
	Event,		// We were interrupted by an event
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
	void wposition();					// Internal

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

	Window& cursor(bool on);

	int getch();
	int readch(unsigned ms=100);

	Window& yield();

	static bool is_supported(Key key);
};

#endif // WINDOW_HPP

// End window.hpp
