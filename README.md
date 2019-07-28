C++ Curses/NCurses Library
==========================

Usage:
------

Somewhere in your program, usually as a global, though it need not be global, define the singleton CppCurses:

    CppCurses curses;

This is not active until you open it thusly:

    Window *main_win = nullptr;

    if ( !(main_win = curses.open()) )
        abort();

The main window may also be fetched any time after curses is open, with:

    main_win = curses.main_window();

Curses can later be shutdown with:

    curses.close();

From this point on, your interactions should be with the returned Window objects. For example:

    Window *w = main_win->new_window(15,20,8,30);
    std::unique_ptr up_w(w);

    w->mvprintf(2,3,"This is {2,3} within w ");

The std::unique_ptr will delete the window when your code goes out of scope, to automatically delete the window.

To get a character without blocking use:

    int ch = w->getch();

The value -1 is returned if there were no characters pending. A lazy way to block until a character is received,
can be done thusly:

    int ch = w->readch();

See the header file include/window.hpp for the full Window API. It provides normal windows, border windows and the ability
to create subwindows within a window, for display safety.

Attributes:
-----------

Attributes are selected through characters, as shown in the following table

    Char    Curses Macro Used Internally
    'N',    A_NORMAL        // Normal display (no highlight)
    'S',    A_STANDOUT      // Best highlighting mode of the terminal.
    'U',    A_UNDERLINE     // Underlining
    'R',    A_REVERSE       // Reverse video
    'b',    A_BLINK         // Blinking
    'D',    A_DIM           // Half bright
    'B',    A_BOLD          // Extra bright or bold
    'A',    A_ALTCHARSET    // Alternate character set

This arrangement allows you to conveniently set several attributes at once:

    w->attr_on("RbB");      // Enable Reverse, Blink and Bold
    ...
    w->attr_off("bB");      // Disable Blink and Bold

Graphics Characters:
--------------------

A limited set of graphic characters are supported through the addgrstr() method. The following code draws a window with panes:

    w->move(12,10).addgrstr("L--T--R");
    w->move(13,10).addgrstr("|  |  |");
    w->move(14,10).addgrstr("t--+--u");
    w->move(15,10).addgrstr("|  |  |");
    w->move(16,10).addgrstr("l--B--r");

Chaining:
---------

Many of the Windows methods return a reference to "self", so that several operations can be chained. An example of this was shown above for setting graphics.

    w->move(16,10).addgrstr("l--B--r");

is equivalent to:

    w->move(16,10);
    w->addgrstr("l--B--r");

Build:
------

To build the library, do:

    $ make

This creates libcppcurses.a, which can be statically linked against.

To install set your PREFIX and make install:

    $ PREFIX=/usr/local make install

Grotty Example Program
----------------------

The grotty test program main.cpp tests some minimal functionality. Type

    make main (or make example)

and run it:

    ./main

Just press a key to advance beyond the current program state.
Source code is in main.cpp.
