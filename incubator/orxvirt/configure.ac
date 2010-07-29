dnl
dnl Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.
dnl
dnl This program and the accompanying materials are made available under
dnl the terms of the Common Public License v1.0 which accompanies this
dnl distribution. A copy is also available at the following address:
dnl http://www.oorexx.org/license.html
dnl
dnl Redistribution and use in source and binary forms, with or
dnl without modification, are permitted provided that the following
dnl conditions are met:
dnl
dnl Redistributions of source code must retain the above copyright
dnl notice, this list of conditions and the following disclaimer.
dnl Redistributions in binary form must reproduce the above copyright
dnl notice, this list of conditions and the following disclaimer in
dnl the documentation and/or other materials provided with the distribution.
dnl
dnl Neither the name of Rexx Language Association nor the names
dnl of its contributors may be used to endorse or promote products
dnl derived from this software without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
dnl LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
dnl FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
dnl OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
dnl TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
dnl OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
dnl OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
dnl NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
dnl SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
dnl

dnl Process this file with autoconf to produce a configure script

AC_PREREQ([2.61])
AC_INIT([orxvirt], [4.1.0])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

AC_CHECK_HEADERS([oorexxapi.h \
                  virterror.h \
                  libvirt.h])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile
                 src/Makefile])

AC_OUTPUT

