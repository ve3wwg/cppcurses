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

The main window may also be fetched any time with:

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

Grotty Example Program
----------------------

The grotty test program main.cpp tests some minimal functionality. Type

    make main (or make example)

and run it:

    ./main

Just press a key to advance beyond the current program state.
Source code is in main.cpp.
