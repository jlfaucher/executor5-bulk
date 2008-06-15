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


# If the REXX_HOME environment variable is not set, then set it here by
# uncommenting the variable and adding the path to your ooRexx installation.  If
# it is already set in the environment, then do nothing.
#
# REXX_HOME = SomePath.To.Your.Rexx

!IF DEFINED(REXX_HOME)
REXX_HOME = "$(REXX_HOME)"
!ELSE
!ERROR REXX_HOME needs to be set, either in environment or in this make file
!ENDIF

# The interpreter headers are needed to build.  Set this to point to the
# interpreter source tree either by defining the variables here, or in the
# environment.
#
# OOREXX_SRC_DRIVE=your.drive
# OOREXX_SRC_DIR=\some\directory\on\your\system

!IF DEFINED(OOREXX_SRC_DRIVE) && DEFINED(OOREXX_SRC_DIR)
SRC_ROOT = $(OOREXX_SRC_DRIVE)$(OOREXX_SRC_DIR)
SRC_KERNEL = $(SRC_ROOT)\kernel
!ELSE
!ERROR OOREXX_SRC_DRIVE and OOREXX_SRC_DIR variables need to be set. Read Makefile
!ENDIF

all: ..\bin\ShellUtils.dll ..\bin\WinShell.cls

LIBS = user32.lib $(REXX_HOME)\api\rexx.lib $(REXX_HOME)\api\rexxapi.lib

SHELL_LIBS = $(LIBS) shell32.lib ole32.lib shlwapi.lib comctl32.lib

INCLUDES = /I$(REXX_HOME)\api /I$(SRC_ROOT)\lib /I$(SRC_ROOT)\platform\windows \
 /I$(SRC_KERNEL)\runtime /I$(SRC_KERNEL)\platform\windows /I$(SRC_KERNEL)\messages \
 /I$(SRC_KERNEL)\classes /I$(SRC_KERNEL)\expression

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

ShellUtils.dll: ShellUtils.obj ShellUtils.def ShellUtils.res
	link ShellUtils.obj ShellUtils.res $(LFLAGS) $(SHELL_LIBS) /def:ShellUtils.def /OUT:$(@R).dll /implib:$(@R).lib /PDB:$(@R).pdb

ShellUtils.obj: ShellUtils.cpp ShellUtils.hpp
	cl $(CPPFLAGS) /TpShellUtils.cpp

ShellUtils.res: ShellUtils.rc ShellUtilsResources.h
  rc -r -foShellUtils.res ShellUtils.rc

..\bin\ShellUtils.dll : ShellUtils.dll
  copy ShellUtils.dll ..\bin\ShellUtils.dll

..\bin\WinShell.cls : WinShell.cls
  copy WinShell.cls ..\bin\WinShell.cls

clean:
	@del *.dll *.obj *.res *.pdb *.lib *.exp *.map 1>nul 2>&1

