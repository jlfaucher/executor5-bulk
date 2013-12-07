                          Release Notes for 1.2.0
                          =======================

This is the binary release of the ooRexx build-utilities package for
Windows.  It contains executables and dynamic link libraries to assist in
the Windows build process of ooRexx.

All binaries have been rebuilt and the ooRexxProcess.dll has been added to
this release.  This DLL is a NSIS plugin that is required for the
packaging step of the Windows ooRexx build.  This is the only change from
the 1.1.0 release.

Two files are available, a regular .zip file and a self-extracting zip
file:

  ooRexx-build-utils-1.2.0-windows.zip
  ooRexx-build-utils-1.2.0-windows.exe

Nsis_longStrings.zip
====================

In addition, the file: Nsis_longStrings.zip is available here.  This is a
special build of Nsis that includes support for long strings.  It also
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

Installation
============

Unzip the .zip file or execute the self-extracting file in a directory in
the path.

Contents
========

Two executables are and one dynamic link library are included in this
release.

ooRexxProcess.dll
-----------------

ooRexxProcess is a NSIS plugin.  The Windows ooRexx installation package is
created using the NSIS system.  oorexx.nsi the NSIS script used to create
the installation package requires the ooRexxProcess plugin.

If you intend to build the Windows ooRexx installation package from
source, you must use this DLL because its functions are used in
oorexx.nsi.  To use the plugin, copy the DLL to the Plugins subdirectory
of your NSIS installation directory.

The plugin can be used in any NSIS script.  It has two functions,
findProcess and killProcess.

Usage (in NSIS script):

  ooRexxProcess::findProcess <processName>
  Pop <varName>

  ooRexxProcess::killProcess <processName>
  Pop <varName>

Returns a numeric value, 0 for success.

Example (in NSIS script):

  ooRexxProcess::killProcess "rxapi"
  Pop $0

  ; $0 now contains 0 for success, 1 if no rxapi process was found.

Return codes:

 0  Success.
 1  The specified process was not found.
 2  No process name was passed to the function.
 3  The process was found, everything worked, but termination failed.
 4  Creating the process list failed.
 5  The Process32First() API failed.
 6  The LookupPrivilegeValue() API failed.
 7  The OpenProcess() API, for our process, failed.
 8  The OpenProcessToken() API failed.
 9  The OpenProcess() API, for the specified process, failed.
10  The AdjuctTokenPrivileges() API failed.

stouch.exe
----------

stouch is a simple 'touch' program.  It updates the last access and
modification times of each fileName arg on the command line to the current
time.  Wildcards are accepted.  There are no options to specify the time
or date to use when touching the files.  The current date and time is
always used.

stouch is used by the touchheaders.bat file in the ooRexx source tree.

Examples:

E:\interpreter>stouch kernel\messages\RexxErrorCodes.h

E:\interpreter>stouch kernel\messages\*.h

Note that the 1.2.0 release of stouch is exactly the same as the 1.1.0
release.

killer.exe
----------

killer is a simple process killer that acts like kill.exe or taskkill.exe.
It kills the process specified on the command line.  The process is
specified by image name, (not by process ID.) It works on both 32-bit and
64-bit Windows.

killer is used in the Windows build process to kill rxapi.exe.

Examples:

E:\killer rxapi.exe

E:\killer rxapi

Note that the 1.2.0 release of killer is exactly the same as the 1.1.0
release.

Source
======

All source code for the build-utilities package is available through
anonymous check out from the ooRexx Subversion repository.  The command to
check out the build-utilities source is:

  svn co https://oorexx.svn.sourceforge.net/svnroot/oorexx/build-utilities/trunk build-utils

The last entry on the command line is the directory into which the files are
extracted.  The 'build-utils' directory name is arbitrary and can be any
name the user chooses.
