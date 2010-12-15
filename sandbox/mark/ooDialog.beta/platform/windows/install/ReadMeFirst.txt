
                              Switch ooDialog
                              ===============

Overview
--------

Switch ooDialog is an application that transparently switches between
two versions of ooDialog in an ooRexx installation.  The switch is easy,
fast, and complete.

The first Switch ooDialog package to become available switches between
ooDialog 4.2.0 (now in beta) and ooDialog 4.1.0 (the version of ooDialog
distributed with ooRexx 4.1.0.)

Switch ooDialog is installed using the typical, standard, Windows
installation program.  Once installed, each time the switchOODialog
application is executed, it completely replaces the current installed
version of ooDialog with its opposite version.

The Switch ooDialog 420 <<-->> 410 package installs ooDialog 4.2.0
during the initial set up of Switch ooDialog.  The user can then switch
back to ooDialog 4.1.0 at any time by executing switchOODialog.exe.
This can be done from the command line, through a menu item in the Start
Menu, or by double clicking the executable in Explorer.
switchOODialog.exe acts like a toggle, switching the installed ooDialog
between the two versions of ooDialog each time it is executed.


ooDialog 4.2.0 (beta)
---------------------

The primary purpose of Switch ooDialog, at this time, is to solicit help
from ooDialog users in moving ooDialog 4.2.0 to a release.

Please read the ooDialog.4.2.0.ReadMe.txt file for important information
concerning ooDialog 4.2.0 and the effort to get 4.2.0 released.

Switch ooDialog allows the user to try / test ooDialog 4.2.0 (beta) and
easily switch back to ooDialog 4.1.0 (the released version of ooDialog
that comes with ooRexx 4.1.0.)


Installation
------------

Switch ooDialog requires that a version of ooRexx, 4.1.0 or higher, is
installed prior to installing Switch ooDialog.  Switch ooDialog has a 32
bit and a 64 bit version.  The bitness of Switch ooDialog must match
that of the installed ooRexx.

Download the appropriate Switch ooDialog installation program from
SourceForge into a convenient directory.  Double click the installer in
Explorer or execute the installer from the command line.

There are no options or preferences for the user to select.  The install
detects the presence of ooRexx and automatically installs itself in the
ooRexx installation directory.  The current version of ooDialog is
switched to 4.2.0 (beta) before the installation program ends.

If there is no correctly installed ooRexx on the system, the Switch
ooDialog installation will notify the user and quit.  If an existing
version of Switch ooDialog is detected, that version is first
uninstalled.  This is done automatically and silently.


Usage
-----

* On Windows Vista or Windows 7, switchOODialog.exe must be executed
with elevated privileges.

During the installation of Switch ooDialog, several menu items are
placed in the Start Menu, within the ooRexx program group.  If for some
reason the ooRexx program group can not be located, Switch ooDialog
creates its own program group.  The menu items are:

Switch ooDialog: This item runs switchOODialog.exe to switch the active
version of ooDialog to the opposite of the current version.  If ooDialog
4.2.0 (beta) is active, ooDialog 4.1.0 becomes active.  If ooDialog
4.1.0 is active, then ooDialog 4.2.0 (beta) becomes active.

Switch ooDialog Info: This item displays information about Switch
ooDialog and is the same as executing switchOODialog from the command
line with the /I parameter.  See below for command line usage.  The
current active version of ooDialog is part of the information displayed.

Uninstall Switch ooDialog: This item uninstalls Switch ooDialog and is
the same as using the Add / Remove Programs applet in the Control Panel.

switchOODialog.exe can of course be run from the command line.

When executed with no parameters, the active version of ooDialog is
switched.

Valid arguments are:

/I       Display Switch ooDialog information.  The current active
         ooDialog is part of the information, as is the syntax for
         command line execution.

/?       Same as /I

/F=key   Force the active version of ooDialog to 'key'.  key must be 410
         or 420.

/V       Switch the active version of ooDialog "visibly."  Normally
         Switch ooDialog runs silently, the user sees nothing.  In this
         mode a window showing the switch activity is displayed.  This
         mode is primarily used by the developers for debugging
         purposes.

* On Windows Vista or Windows 7 when running switchOODialog from a
command prompt window, always open the window using the "Run as
administrator" right click context menu item.

NOTE:

The active version of ooDialog can not be switched properly if an
ooDialog program is running, or if the ooDialog reference book is open.
Switch ooDialog detects this problem during execution, notifies the
user, and quits without making any changes.


Removing Switch ooDialog
------------------------

Switch ooDialog can be uninstalled at any time using the normal Windows
methods.  An uninstall menu item is placed within the ooRexx program
group in the Start Menu.  Therefore, Switch ooDialog can be uninstalled
through the Add / Remove Programs applet in the Control Panel, by using
the menu item in the Start Menu, or by executing the uninstall program,
(uninstall_switchOODialog.exe,) in the ooRexx installation directory.

During the uninstall, the installed version of ooDialog is switched back
to 4.1.0.

If the user is going to uninstall ooRexx, Switch ooDialog should be
uninstalled first.


Known limitations and problems
------------------------------

* The active version of ooDialog can not be changed if an ooDialog
program is running or if the ooDialog reference book is open.  The user
must close all ooDialog programs and close the ooDialog reference
document prior to running switchOODialog.

* Switch ooDialog must be executed using elevated privileges on Windows
Vista and Windows 7.

* The ooRexx installation program is unaware of Switch ooDialog.  If
ooRexx is uninstalled before uninstalling Switch ooDialog it will no
longer be possible to uninstall Switch ooDialog cleanly.  This is not a
big problem, the user will need to delete some files manually.  The fix
is to uninstall Switch ooDialog prior to uninstalling ooRexx.

