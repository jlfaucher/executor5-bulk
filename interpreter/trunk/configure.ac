dnl
dnl Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.
dnl
dnl This program and the accompanying materials are made available under
dnl the terms of the Common Public License v1.0 which accompanies this
dnl distribution. A copy is also available at the following address:
dnl http://www.ibm.com/developerworks/oss/CPLv1.0.htm
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

PACKAGE=ooRexx
OOREXX_VERSION_MAJOR=4
OOREXX_VERSION_MINOR=0
OOREXX_VERSION_REVISION=0
OOREXX_VERSION=$OOREXX_VERSION_MAJOR.$OOREXX_VERSION_MINOR.$OOREXX_VERSION_REVISION
OOREXX_LIB_VERSION=$OOREXX_VERSION_MAJOR:0:0

dnl Process this file with autoconf to produce a configure script
AC_INIT($PACKAGE, $OOREXX_VERSION)

AC_SUBST(OOREXX_VERSION_MAJOR)
AC_SUBST(OOREXX_VERSION_MINOR)
AC_SUBST(OOREXX_VERSION_REVISION)
AC_SUBST(OOREXX_VERSION)
AC_SUBST(OOREXX_LIB_VERSION)

dnl Set the default prefix for the installation directory. The setting here
dnl is designed to make us compliant with the Linux Standards Base.
case $prefix in
  NONE) prefix=/opt/ooRexx
esac
AC_SUBST(OOREXX_PREFIX,$prefix)

AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_CPP
AC_C_CONST
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

dnl Set the size of a pointer (in bytes)
dnl AC_MSG_CHECKING(pointer size)
OOREXX_SET_SIZES
AC_SUBST(OOREXX_SIZEOF_POINTER, $OOREXX_SIZEOF_POINTER)

dnl set the defaults to Linux
GENERIC_OS="-DLINUX"
GENERIC_OPSYS="-DOPSYS_LINUX"
GENERIC_DEFINES="-DNOOPT -DPTHREAD_KERNEL \
                 -DSHARED -D_POSIX_THREAD -D_REENTRANT -DHIGHTID  \
                 -D_GNU_SOURCE"

dnl Now we throw in a bunch of warnings. C and C++ need
dnl different sets here
GCC_WARNINGS="\
        -Wall \
        -Wno-unused \
        -funsigned-char \
        -Wmissing-prototypes \
        -Wmissing-declarations \
        -Wstrict-prototypes \
        -Wpointer-arith \
        -Wformat=2 \
        -Wformat-security \
        -Wformat-nonliteral \
        -Wno-format-y2k \
        -Wcast-qual \
        -Wcast-align \
        -Werror"

GCXX_WARNINGS="\
        -Wall \
        -Wno-unused \
        -funsigned-char \
        -Wpointer-arith \
        -Wcast-qual \
        -Wcast-align \
        -Wshadow \
        -Wwrite-strings \
        -Wredundant-decls \
        -Werror"

XLC_WARNINGS="\
        -qchars=unsigned \
        -qnodigraph"

XLCXX_WARNINGS="\
        -+ \
        -qchars=unsigned \
        -qnodigraph"

dnl Set platform and compiler specific switches
echo $CXX $target
case "$target" in
   *hp-hpux*)
      ;;
   *ibm-aix*)
      GENERIC_OS="-DAIX"
      GENERIC_OPSYS="-DOPSYS_AIX"
      GENERIC_DEFINES="$GENERIC_DEFINES -DOPSYS_AIX43"
      case "$CXX" in
         g++)
            CC_WARNINGS="-maix64 -mthreads $GCC_WARNINGS"
            CXX_WARNINGS="-maix64 -mthreads $GCXX_WARNINGS"
            ;;
         xlC_r)
            CC_WARNINGS="$XLC_WARNINGS"
            CXX_WARNINGS="$XLCXX_WARNINGS"
            ;;
         *)
dnl         AC_MSG_ERROR("Compiler $CXX is not supported on this platform")
            ;;
      esac
      ;;
   *solaris*)
      GENERIC_OPSYS="$GENERIC_OPSYS -DOPSYS_SUN"
      case "$CXX" in
         g++)
            CC_WARNINGS="-m32 $GCC_WARNINGS"
            CXX_WARNINGS="-m32 $GCXX_WARNINGS"
            ;;
         *)
dnl         AC_MSG_ERROR("Compiler $CXX is not supported on this platform")
            ;;
      esac
      ;;
   i*86-*linux*)
      CC_WARNINGS="-m32 $GCC_WARNINGS"
      CXX_WARNINGS="-m32 $GCXX_WARNINGS"
      ;;
   *86_64-*linux*)
      CC_WARNINGS="-m64 $GCC_WARNINGS"
      CXX_WARNINGS="-m64 $GCXX_WARNINGS"
      ;;
   *cygnus*)
      CC_WARNINGS="$GCC_WARNINGS"
      CXX_WARNINGS="$GCXX_WARNINGS"
      ;;
   *)
      ;;
esac

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h time.h sys/time.h unistd.h stdarg.h netinet/in.h pwd.h stropts.h])

dnl Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_VPRINTF
AC_HEADER_TIME

AC_CHECK_FUNCS([gettimeofday memset strdup getpwuid geteuid \
                nsleep IDtouser getpgrp])

dnl Special check for nanosleep
AC_CHECK_FUNC(nanosleep)
if test x"$ac_cv_func_nanosleep" = xno; then
  AC_CHECK_LIB(rt, nanosleep)
  if test x"$ac_cv_lib_rt_nanosleep" = xyes; then
    AC_DEFINE(HAVE_NANOSLEEP, 1, "Define to 1 if you have nanosleep function")
  fi
else
  AC_DEFINE(HAVE_NANOSLEEP, 1, "Define to 1 if you have nanosleep function")
fi

dnl Check for Xalan or xalan for xsl files
xsl=''
AC_PATH_PROGS([xsl], [Xalan xalan])
if test -z "$xsl"; then
   AC_MSG_WARN([cannot find xalan, Xalan])
else
   # $1="OUT" $2="XML" $3="XSL"
   case "$xsl" in
      *Xalan)
         xsl="$xsl -o \$1 \$2 \$3"
         ;;
      *xalan)
         xsl="$xsl -OUT \$1 -IN \$2 -XSL \$3"
         ;;
      *)
         xsl="echo Cannot transform XSL as Xalan not installed;exit 1"
         ;;
   esac
fi

dnl Checks for other helper programs
AC_PATH_PROGS(RPM, rpmbuild)

dnl remerge the warnings into the FLAGS
GENERAL_CXXFLAGS="\
              $GENERIC_DEFINES \
              $GENERIC_OS \
              $GENERIC_OPSYS"

GENERAL_CFLAGS="\
              $GENERIC_DEFINES \
              $GENERIC_OS \
              $GENERIC_OPSYS"
CXXFLAGS="$CFLAGS $CXX_WARNINGS $GENERAL_CXXFLAGS"
CFLAGS="$CFLAGS $CC_WARNINGS $GENERAL_CFLAGS"

OOREXX_INCLUDES="-I\$(top_srcdir)/api \
                 -I\$(top_srcdir)/api/platform/unix \
		 -I\$(top_srcdir)/kernel \
		 -I\$(top_srcdir)/kernel/messages \
		 -I\$(top_srcdir)/shared"
AC_SUBST(OOREXX_INCLUDES)

OOREXX_COMMONHEADERS="\$(top_srcdir)/api/InternalAPI.hpp \
              \$(top_srcdir)/api/oorexx.h \
              \$(top_srcdir)/api/oorexxapi.h \
              \$(top_srcdir)/api/oorexxerrors.h \
              \$(top_srcdir)/api/rexx.h \
              \$(top_srcdir)/api/rexxapidefs.h \
              \$(top_srcdir)/api/platform/unix/rexxapitypes.h \
              \$(top_srcdir)/kernel/Rxstring.hpp \
              \$(top_srcdir)/kernel/SynchronizedBlock.hpp \
              \$(top_srcdir)/kernel/SystemException.hpp"
AC_SUBST(OOREXX_COMMONHEADERS)

OOREXX_LIBS="-L\$(top_srcdir)/rexxapi \
            -L\$(top_srcdir)/kernel \
            -lpthread \
            -ldl"
AC_SUBST(OOREXX_LIBS)




AC_CONFIG_FILES([
        oorexx.spec \
        ooRexx.pc \
        Makefile \
        kernel/Makefile \
        kernel/messages/Makefile \
        rexxapi/Makefile \
        utilities/Makefile \
        utilities/rexx/Makefile \
        utilities/rexxc/Makefile \
        utilities/rexximg/Makefile \
        utilities/rxapi/Makefile \
        utilities/rxapi/platform/unix/rxapid.sh \
        utilities/rxqueue/Makefile \
        utilities/rxsubcom/Makefile \
	xsl.sh
	])

AC_OUTPUT
