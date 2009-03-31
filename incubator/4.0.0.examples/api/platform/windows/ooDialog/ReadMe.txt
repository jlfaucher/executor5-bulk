
                         ooDialog Logon Example
                         ======================

The primary purpose of this example is to show how to execute ooDialog
programs directly from a compiled C / C++ program using the ooRexx
object-orientated native API introduced in ooRexx 4.0.0.  There is a
"gotcha" when executing ooDialog programs through the API.

On Windows, ooRexx uses a manifest to specify the version of the Common
Controls Library.  When starting an ooDialog program from a compiled C
or C++ program, the compiled program must be linked with a manifest that
specifies the same version of the Common Controls Library as ooDialog,
or ooDialog will not initialize.

To run the example, simply use rxStart with no arguments.  Hint: user
passwords are the same as the user IDs.

This example is relatively simple and provides examples of the following
items:

1.) The Makefile file in the example shows how to link in a manifest to
a complied executable, in this case rxStart.exe.

2.) rxStart.cpp shows how to create an instance of the ooRexx
interpreter, use it to run a Rexx program, and understand the result of
executing the Rexx program.

3.) dbAccess.cpp and the Makefile show how to build a library (.DLL) of
external routines and methods that can be used in Rexx programs to
extend the ooRexx interpreter.

4.) logon.rex shows the correct syntax to use the external methods and
routines in a Rexx program.

5.) The Makefile file, logon.rc, and logon.rex show how to use a
compiled resource only DLL to provide the dialog template for an
ooDialog dialog.  The dialog itself was designed, in March 2009, using a
WYSIWYG (What You See Is What You Get) resource editor to create
logon.rc.  This is yet another attempt to dispel the myth that IBM's
Resource Workshop is needed to use ooDialog with normal Windows
resources.

dbAccess contains only one external routine and two external methods.
They are intended to simulate routines and methods that might link to a
backend database.  In this case there is no real database, just a
simulation.  The methods are attached to the ooDialog dialog to show how
a dialog could access a database directly from within the dialog class.
The routine is used to show how a series of routines could be created
providing database access.
