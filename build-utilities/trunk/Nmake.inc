#------------------------------------------------------------------------------*
#
# Copyright (c) 2007 Rexx Language Association. All rights reserved.
#
# This program and the accompanying materials are made available under
# the terms of the Common Public License v1.0 which accompanies this
# distribution. A copy is also available at the following address:
# http://www.oorexx.org/license.html
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither the name of Rexx Language Association nor the names
# of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#------------------------------------------------------------------------------*

# This is a part of the Windows ooRexx build utilities.  (Just a template for
# now, what there is to make is very simple.)

#
# Set your Windows compiler here by uncommenting the proper macro.
#
# MSVCVer >= 8.0 is preferred going forward.  However, build utilities needed
# for a certain version of ooRexx will have the same compilier restrictions as
# that version of ooRexx.  There is no guarentee that all utilities will build
# with all supported versions of the compiler.
#
# (At this point everything compiles with MSVC 6.0 and on.  There is no need to
# set the compiler.)
#
# MSVC6 =
# MSVC7 =
# MSVC7_64 =
# MSVC8 =
# MSVC8_64 =


WINDIR    = $(TOPDIR)\platform\windows
SRCDIR    = $(WINDIR)
BINDIR    = $(WINDIR)\bin
OBJDIR    = $(WINDIR)\obj
ASMDIR    = $(WINDIR)\asm_src
PLUGINDIR = $(WINDIR)\ooRexxProcess

# To build the NSIS plugin, ooRexxProcess, you must set NSIS_HOME_DIR to point
# to your installation of NSIS.  Do not include the trailing '\'.
NSIS_HOME_DIR = C:\Tools\misc.dev.tools\Nsis

# Default ooRexx top level build directory.  Change this to point to the correct
# directory, or define on the command line to Nmake.
ooRexx_SRCDIR = ..\main

# CC, CPP, CXX are predefined for MSVC, but nothing is predefined for the linker
# or lib utility, etc..
LD = link /nologo

COMMONCFLAGS = /nologo /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE

CFLAGS = /c $(COMMONCFLAGS) /Fo$(OBJDIR)\ /W3

