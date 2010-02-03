                          OrxnCurses Class Library
			  ========================


The OrxnCurses class library provides an interface to the nCurses library available on most
*nix distributions. It is also available to Windows users through the PDCurses library
which you can obtain freely on the Internet.

OrxnCurses provides an object oriented approach to the Curses function library. There are two
major classes provided: Window and Pad.

The Window class provides all the functionality for both the main stdscr and any other windows
you wish to create on that stdscr. The first window created by the class library will ALWAYS
be the stdscr device. Subsequent new windows are standard Curses windows.

The Pad class provides an interface to Curses pads. Pads are slightly different from a
Curses window and the Pad and Window classes contain code to differentiate between them.


IMPORTANT NOTE!!!!
------------------

The OrxnCurses class library provides a mechanism to control whether or not indexes to rows,
columns, etc. are 0- or 1-based. By default all indexes are 1-based. If you wish to change
that you can invoke the SetBase class method as follows.

      .window~SetBase(0)             -- changes all indexes to 0-based
      .wondow~SetBase(1)             -- changes all indexes to 1-based (default)
      base = .window~SetBase()	     -- returns the current indexing model (0 or 1)
      
The change to the indexing model is immediate and global for the user's process. But we do
not recommend that you change it in the middle of program code. It is best to set it at the
top of your code and leave it unchanged after that.

Exceptions: these are values, not indexes (always 0-based):
   attributes
   color numbers
   soft label numbers
   soft label format numbers


Provided Test Programs
----------------------

There are a number of test programs provided. They are all derrived from the book
"Programmer's Guide to nCurses" by Dab Gookin, Wiley Publishing, Inc., ISBN 978-0-470-10759-1.
All programs use 1-based indexes except the programs with an "a" suffix e.g. test2-4a.rex.

There are still a few minor problems with a few of the test scripts. I am still working
through them to resolve the issues.


Happy Programming!
W. David Ashley
February, 2010

