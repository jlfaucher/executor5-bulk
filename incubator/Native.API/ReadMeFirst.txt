

                                Native.API
                                ==========


The 'Native.API' subdirectory in the incubator contains "stuff" related to
the C++ native API introduced in ooRexx 4.0.0, not stuff related to the
older string-based native API used prior to ooRexx 4.0.0

The older API is still available, it is just not the focus here.

                       Summary of Directory Content
                       ============================


platform/windows/template
-------------------------

This directory contains a set of template files that can be used to start a
new external library package for Windows.  Copy and rename the template
files to reflect the new package and it will immediately build a working
external library package.

platform/windows/WindowRexx

This directory contains a small external library package for the .WindowRexx
class. Currently, ooDialog 4.2.0 (not released yet.) has a .Window class
with methods pertaining to any Windows window.  The winsystem package has a
.WindowObject also with methods pertaining to any Windows window.

.WindowRexx contains fixes for some .WindowObject methods that do not work,
and / or are missing from .Window.  The methods will eventually be merged
into the .Window class in ooDialog.  In the mean time, this package can be
used by Rexx programmers that need a fixed sendKeyPress() method, or as an
example of creating an external library package.
