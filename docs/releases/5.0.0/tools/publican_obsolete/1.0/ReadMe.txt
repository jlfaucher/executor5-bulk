                          Release Notes for 1.0.0
                          =======================

windows-build-tools is a set of tools commonly used to work with the Windows
source code for ooRexx and to build ooRexx on Windows.  The tools are used
to check out the source code and to build the ooRexx interpreter and its
installation package for Windows.

This is provided as a convenience for people interested in working with the
ooRexx source code.  There is by no means a requirement that these specific
tools be used.  All the tools are freely available on the Internet and
interested parties are encouraged to gather their own tools.

Please read the windows-build.txt file in the top level of the ooRexx source
tree for information on how build ooRexx on Windows.  That information is
not included here.

CollabNetSubversion-client-1.5.5-1.win32.exe
--------------------------------------------

A Windows Subversion client.  A Subversion client is needed to check out the
source code.  This is a typical Windows installation package.

killer.exe
----------

A 32-bit process killer.  A process killer is used during the Windows build
of ooRexx.  Place killer.exe in a directory in the path.

Nsis_longStrings.zip
--------------------

NSIS is required to build the Windows installation package for ooRexx.  This
is a special build of NSIS that includes support for long strings.  It also
includes the plugins necessary to build the ooRexx Windows installation
package.

To use Nsis_longStrings.zip, simply unzip the file in the location of your
choice.  Typically, the resulting directory is added to the path for ease of
use.  Alternatively, makensis.exe can be invoked using the complete pathname
of the executable.

Nothing else needs to be done to use this version of NSIS to build the
Windows installation package for ooRexx.

Warning:  If you do not use a build of makensis that has support for long
strings, the resulting installer will warn the user not to use it.

xalan_xerces_binaries.zip
-------------------------

The Xalan and Xerces binaries are used during the build of ooRexx.  In
theory, Xalan and Xerces are not supposed to be needed during the build.
However in practice the build often fails if the binaries are not found.  It
is easier to simply have the binaries available than it is to debug the
build process.

Unzip the file in to a directory that is in the path, or unzip to a location
of your choice and ensure that directory is in the path during a build of
ooRexx on Windows.

