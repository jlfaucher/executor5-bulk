#----------------------------------------------------------------------------
#
# Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.
#
# This program and the accompanying materials are made available under
# the terms of the Common Public License v1.0 which accompanies this
# distribution. A copy is also available at the following address:
# http://www.ibm.com/developerworks/oss/CPLv1.0.htm
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
#----------------------------------------------------------------------------


# This is a part of the ooRexx Windows build

!include $(top_srcdir)\oorexx.ver

#
# Set your Windows compiler here.
# Microsoft Visual C++ 7.0 with service packs is required!
#
VCPP7 =
#VCPP7_64 =

#
# If you want debugging activated do it here
#
#DEBUG = 1

#
# Set up stuff based on the compiler.
#
!IFDEF VCPP_64
# TODO: set the compiler into 64-bit mode here
VCPP_64_FLAG =
OOREXX_BIT_SIZE = 64
!ELSE
VCPP_64_FLAG =
OOREXX_BIT_SIZE = 32
!ENDIF


#
# Now set everything else.
#
CC = cl
GLOBALCFLAGS = /DORX_VER=$(MAJOR) /DORX_REL=$(MINOR) /DORX_MOD=$(MOD_LVL) \
               $(VCPP_64_FLAG) /GX /Wp64

!IF "$(DEBUG)" == "1"
CFLAGS = /c $(GLOBALCFLAGS) $(INCLUDES) /Zi /Od
LFLAGS = /DEBUG
!ELSE
CFLAGS = /c $(GLOBALCFLAGS) $(INCLUDES)
LFLAGS =
!ENDIF

LD = link
LIBUTIL = lib

!IF "$(OOREXX_BIT_SIZE)" == "64"
# TODO: set the 64-bit libraries here
WINLIBS =
!ELSE
WINLIBS = user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib uuid.lib \
          ole32.lib oleaut32.lib shell32.lib kernel32.lib delayimp.lib wsock32.lib
!ENDIF

COMMONHEADERS = $(top_srcdir)\api\InternalAPI.hpp \
                $(top_srcdir)\api\oorexx.h \
                $(top_srcdir)\api\oorexxapi.h \
                $(top_srcdir)\api\oorexxerrors.h \
                $(top_srcdir)\api\rexx.h \
                $(top_srcdir)\api\rexxapidefs.h \
                $(top_srcdir)\api\platform\windows\rexxapitypes.h \
                $(top_srcdir)\api\platform\windows\rexxplatformapis.h \
                $(top_srcdir)\kernel\Rxstring.hpp \
                $(top_srcdir)\kernel\SynchronizedBlock.hpp \
                $(top_srcdir)\kernel\SystemException.hpp

OOREXXLIBS = $(top_srcdir)\kernel\rexx.lib

OOREXXINC =  /I$(top_srcdir)\api \
             /I$(top_srcdir)\api\platform\windows \
             /I$(top_srcdir)\shared

CLEAN = *.obj *.exe *.dll *.lib *.res *.exp *.pdb






