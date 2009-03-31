#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2008-2009 Rexx Language Association. All rights reserved.    */
#/*                                                                            */
#/* This program and the accompanying materials are made available under       */
#/* the terms of the Common Public License v1.0 which accompanies this         */
#/* distribution. A copy is also available at the following address:           */
#/* http://www.oorexx.org/license.html                                         */
#/*                                                                            */
#/* Redistribution and use in source and binary forms, with or                 */
#/* without modification, are permitted provided that the following            */
#/* conditions are met:                                                        */
#/*                                                                            */
#/* Redistributions of source code must retain the above copyright             */
#/* notice, this list of conditions and the following disclaimer.              */
#/* Redistributions in binary form must reproduce the above copyright          */
#/* notice, this list of conditions and the following disclaimer in            */
#/* the documentation and/or other materials provided with the distribution.   */
#/*                                                                            */
#/* Neither the name of Rexx Language Association nor the names                */
#/* of its contributors may be used to endorse or promote products             */
#/* derived from this software without specific prior written permission.      */
#/*                                                                            */
#/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
#/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
#/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
#/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
#/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
#/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
#/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
#/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
#/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
#/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
#/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
#/*                                                                            */
#/*----------------------------------------------------------------------------*/


# This is a Visual C++, nMake compatible make file for the logon ooDialog
# example.
#
# On Windows, the interpreter and ooDialog are linked with a manifest file
# specifying the version for the Common Controls library.  When using the native
# APIs in your C / C++ programs to start the intepreter, in order to execute an
# ooDialog program, your executable program must also be linked with a manifest
# file that specifies the same version of the Common Controls library.
#
# This make file provides an example of how to link in a manifest file.


# The compiler needs to be able to find the ooRexx native API headers and
# libraries. If REXX_HOME is set on the system, the correct directory will be
# automatically added to the LIB and INCLUDE environment variables by this make
# file.
#
# Otherwise, either uncomment the next line and define the correct REXX_HOME, or
# be sure the LIB and INCLUDE environment variables allow the compiler to find
# the ooRexx native API headers and libraries.
#
# On a build system for ooRexx, you can set REXX_HOME to point to Win32Dbg or
# Win32Rel, as appropriate, in your build directory.  For this to work, you
# will have to have built the interpreter.

#REXX_HOME = C:\work.ooRexx\wc\main\Win32Dbg   # For example

# Define RELEASE on the command line, or here, to build a non-debug version.  By
# default debug versions are built.  I.e. nMake RELEASE=1
#RELEASE = 1

# Define CPU=X64 on the nMake command line, or here, to build for 64-bit.  By
# default the build is for 32-bit.  I.e.: nMake CPU=X64 RELEASE=1  We only need
# to know the CPU in order to pick the right manifest file.
CPU = X64

# What we are going to build
all: rxStart.exe dbAccess.dll logon.dll

!IF DEFINED(REXX_HOME)
INCLUDE = $(INCLUDE);$(REXX_HOME)\api
LIB = $(LIB);$(REXX_HOME)\api
!ENDIF

!IFNDEF CPU
CPU = X86
!ENDIF

# Pick which manifest file to use.
!IF "$(CPU)" == "X64"
M_FILE = "rexx64.exe.manifest"
MACHINE = /MACHINE:X64
!ELSE
M_FILE = "rexx32.exe.manifest"
MACHINE = /MACHINE:X86
!ENDIF

REXX_LIBS = rexx.lib rexxapi.lib

WARNINGFLAGS = /W3 /Wp64 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE

DEF_FILE = /def:dbAccess.def

!IF DEFINED(RELEASE)
# Set the compile and link flags for a release build
OOREXX_CFLAGS = /nologo /EHsc /O2 /Gr /Gs /FAcs /MT $(WARNINGFLAGS) /c
EXE_LFLAGS = /nologo /SUBSYSTEM:Console $(MACHINE) $(REXX_LIBS) user32.lib comdlg32.lib gdi32.lib kernel32.lib
DLL_LFLAGS = /nologo /SUBSYSTEM:Windows $(MACHINE) $(REXX_LIBS) $(DEF_FILE) /DLL
!ELSE # release not defined
# Set the compile and link flags for a debug build
OOREXX_CFLAGS =  /nologo /EHsc /Zi /Od /Gr /MTd $(WARNINGFLAGS) /c
EXE_LFLAGS = /nologo /DEBUG -debugtype:cv /SUBSYSTEM:Console $(MACHINE) $(REXX_LIBS) user32.lib comdlg32.lib gdi32.lib kernel32.lib
DLL_LFLAGS = /nologo /DEBUG -debugtype:cv /SUBSYSTEM:Windows $(MACHINE) $(REXX_LIBS) $(DEF_FILE) /DLL
!ENDIF

rcflags_common=/DWIN32 /DMANIFEST_FILE=$(M_FILE)

rxStart.exe: rxStart.obj rxStart.res
    link $(EXE_LFLAGS) rxStart.res rxStart.obj -out:$(@B).exe

rxStart.obj: rxStart.cpp logon.h
  cl $(OOREXX_CFLAGS) rxStart.cpp

rxStart.res: rxStart.rc
  rc $(rcflags_common) -r -fo$(@B).res $(@B).rc

dbAccess.dll: dbAccess.obj
    link $(DLL_LFLAGS) dbAccess.obj -out:$(@B).dll

dbAccess.obj: dbAccess.cpp dbAccess.h logon.h
  cl $(OOREXX_CFLAGS) dbAccess.cpp

logon.dll: logon.res
  link logon.res /NOENTRY /DLL $(MACHINE) /OUT:logon.dll

logon.res: logon.rc logon.h
  rc /r /fo$(@B).res $(@B).rc

mostlyclean:
	del *.obj *.ilk *.lib *.exp  *.cod *.res 1>nul 2>&1

clean:
  attrib -h *.suo 1>nul 2>&1
	del *.exe *.dll *.obj *.ilk *.pdb *.lib *.exp *.suo *.cod *.res *.sln 1>nul 2>&1

