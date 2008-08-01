#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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

# This make file is currently set to build outside of the ooRexx source tree.
# It is my intent to add this package to the Windows extensions shipped with
# ooRexx.  If that is done this make file needs to be reworked.

# The easiest way to build is to use the api directory in an installed version
# of ooRexx 4.0.0.  But, if that is not practical, then you need to build using
# the ooRexx source tree, and you need to have rexx.lib and rexxapi.lib built.
#
# If you need to build using the ooRexx source tree then either define these
# variables in your environment, or uncomment them here and set them to the
# correct paths.  Otherwise, just ensure that REXX_HOME is set properly.
#
# OOREXX_SRC_DRIVE=your.drive
# OOREXX_SRC_DIR=\some\directory\on\your\system

!IF DEFINED(OOREXX_SRC_DRIVE) && DEFINED(OOREXX_SRC_DIR)
SRC_ROOT = $(OOREXX_SRC_DRIVE)$(OOREXX_SRC_DIR)
SRC_API = $(SRC_ROOT)\api

!IF "$(DBG)" == "1"
REXX_LIBS = $(SRC_ROOT)\Win32Dbg\rexx.lib $(SRC_ROOT)\Win32Dbg\rexxapi.lib
!ELSE
REXX_LIBS = $(SRC_ROOT)\Win32Rel\rexx.lib $(SRC_ROOT)\Win32Rel\rexxapi.lib
!ENDIF

REXX_INCLUDES = /I$(SRC_API) /I$(SRC_API)\platform\windows

!ELSE IF DEFINED(REXX_HOME)
REXX_HOME = "$(REXX_HOME)"
REXX_LIBS = $(REXX_HOME)\rexx.lib $(REXX_HOME)\rexxapi.lib
REXX_INCLUDES = /I$(REXX_HOME)
!ELSE
!ERROR REXX_HOME or OOREXX_SRC_DRIVE needs to be set, either in environment or in this make file
!ENDIF

all: ..\bin\WinShell.dll ..\bin\WinShell.cls

LIBS = user32.lib $(REXX_LIBS)

WINSHELL_LIBS = $(LIBS) shell32.lib ole32.lib shlwapi.lib comctl32.lib

INCLUDES = $(REXX_INCLUDES)

!IF "$(VCPP8)" == "1"
CFLAGS_COMMON = /nologo /c /EHsc /MT /W3 /Wp64
!ELSE
CFLAGS_COMMON = /nologo /c /GX /MT /W3
!ENDIF

CFLAGS_DEFINES = /DWIN32 /D_CRT_SECURE_NO_DEPRECATE

!IF "$(DBG)" == "1"
CFLAGS_DEBUG = /Zi /Od -D_DEBUG /DEBUGTYPE:CV
!ELSE
CFLAGS_OPTIMIZE = /Ox
!ENDIF

CPPFLAGS = $(CFLAGS_OPTIMIZE) $(CFLAGS_COMMON) $(CFLAGS_DEFINES) $(CFLAGS_DEBUG) $(INCLUDES)

LFLAGS_COMMON = /nologo /machine:X86 /SUBSYSTEM:Windows /DLL

!IF "$(DBG)" == "1"
LFLAGS_DEBUG = /MAP /NOLOGO /PROFILE /DEBUG -debugtype:cv
!ENDIF

LFLAGS = $(LFLAGS_COMMON) $(LFLAGS_DEBUG)

WinShell.dll: WinShell.obj WinShell.def WinShell.res
	link WinShell.obj WinShell.res $(LFLAGS) $(WINSHELL_LIBS) /def:WinShell.def /OUT:$(@R).dll /implib:$(@R).lib /PDB:$(@R).pdb

WinShell.obj: WinShell.cpp WinShell.hpp
	cl $(CPPFLAGS) /TpWinShell.cpp

WinShell.res: WinShell.rc WinShellResources.h
  rc -r -foWinShell.res WinShell.rc

..\bin\WinShell.dll : WinShell.dll
  copy WinShell.dll ..\bin\WinShell.dll

..\bin\WinShell.cls : WinShell.cls
  copy WinShell.cls ..\bin\WinShell.cls

clean:
	@del *.dll *.obj *.res *.pdb *.lib *.exp *.map 1>nul 2>&1

