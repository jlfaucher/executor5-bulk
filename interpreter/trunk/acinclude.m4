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


dnl
dnl Set sizes for C/C++ data types
dnl
AC_DEFUN([OOREXX_SET_SIZES],
    [
    OOREXX_SSFILE=testsize
    OOREXX_SSSOURCE="$OOREXX_SSFILE.c"
    echo "#include <stdlib.h>" > $OOREXX_SSSOURCE
    echo "#include <stdio.h>" >> $OOREXX_SSSOURCE
    echo "int main() {" >> $OOREXX_SSSOURCE
    # add more here if you need them
    # the lots of slashes are needed to do the processing below right
    echo "printf(\"unsigned char %d\\\\n\",sizeof(unsigned char));" >> $OOREXX_SSSOURCE
    echo "printf(\"unsigned short %d\\\\n\",sizeof(unsigned short));" >> $OOREXX_SSSOURCE
    echo "printf(\"unsigned int %d\\\\n\",sizeof(unsigned int));" >> $OOREXX_SSSOURCE
    echo "printf(\"char %d\\\\n\",sizeof(char));" >> $OOREXX_SSSOURCE
    echo "printf(\"short %d\\\\n\",sizeof(short));" >> $OOREXX_SSSOURCE
    echo "printf(\"int %d\\\\n\",sizeof(int));" >> $OOREXX_SSSOURCE
    echo "printf(\"float %d\\\\n\",sizeof(float));" >> $OOREXX_SSSOURCE
    echo "printf(\"double %d\\\\n\",sizeof(double));" >> $OOREXX_SSSOURCE
    echo "printf(\"void * %d\\\\n\",sizeof(void *));" >> $OOREXX_SSSOURCE
    echo "return 0;" >> $OOREXX_SSSOURCE
    echo "}" >> $OOREXX_SSSOURCE

    $CC -o $OOREXX_SSFILE $OOREXX_SSSOURCE

    OOREXX_TYPE_SIZES=`./$OOREXX_SSFILE`
    # feel free to define more logic here if we need it

    OOREXX_SIZEOF_UCHAR=`echo -e $OOREXX_TYPE_SIZES | grep "^unsigned char" | awk '{print $[3]}'`
    OOREXX_SIZEOF_USHORT=`echo -e $OOREXX_TYPE_SIZES | grep "^unsigned short" | awk '{print $[3]}'`
    OOREXX_SIZEOF_UINT=`echo -e $OOREXX_TYPE_SIZES | grep "^unsigned int" | awk '{print $[3]}'`
    OOREXX_SIZEOF_CHAR=`echo -e $OOREXX_TYPE_SIZES | grep "^char" | awk '{print $[2]}'`
    OOREXX_SIZEOF_SHORT=`echo -e $OOREXX_TYPE_SIZES | grep "^short" | awk '{print $[2]}'`
    OOREXX_SIZEOF_INT=`echo -e $OOREXX_TYPE_SIZES | grep "^int" | awk '{print $[2]}'`
    OOREXX_SIZEOF_FLOAT=`echo -e $OOREXX_TYPE_SIZES | grep "^float" | awk '{print $[2]}'`
    OOREXX_SIZEOF_DOUBLE=`echo -e $OOREXX_TYPE_SIZES | grep "^double" | awk '{print $[2]}'`
    OOREXX_SIZEOF_POINTER=`echo -e $OOREXX_TYPE_SIZES | grep "^void *" | awk '{print $[3]}'`
    rm -f $OOREXX_SSFILE $OOREXX_SSSOURCE
    ])


dnl
dnl Check to see if FILE struct contains _cnt member
dnl
AC_DEFUN([ORX_FILE__CNT],
    [
    AC_MSG_CHECKING(if FILE struct contains _cnt member)
    AC_TRY_COMPILE([#include <stdio.h>],
    [FILE *fp; fp->_cnt = 0;],
      orx_result=yes; AC_DEFINE(HAVE_FILE__CNT, 1, Define to 1 if FILE struct contains _cnt member), orx_result=no )
    AC_MSG_RESULT($orx_result)
    ])


dnl
dnl Check to see if FILE struct contains _IO_read_ptr member
dnl
AC_DEFUN([ORX_FILE__IO_READ_PTR],
    [
    AC_MSG_CHECKING(if FILE struct contains _IO_read_ptr member)
    AC_TRY_COMPILE([#include <stdio.h>],
    [FILE *fp; fp->_IO_read_ptr = 0;],
      orx_result=yes; AC_DEFINE(HAVE_FILE__IO_READ_PTR, 1, Define to 1 if FILE struct contains _IO_read_ptr member), orx_result=no )
    AC_MSG_RESULT($orx_result)
    ])


dnl
dnl Check to see if union semun is (incorrectly) defined in sys/sem.h
dnl
AC_DEFUN([ORX_SEMUN_DEFINED],
    [
    AC_MSG_CHECKING(if union semun is incorrectly defined)
    AC_TRY_COMPILE([
    #include <sys/types.h>
    #ifdef HAVE_SYS_SEM_H
    #include <sys/sem.h>
    #endif
    ],
    [union semun semopts],
      orx_result=yes; AC_DEFINE(HAVE_UNION_SEMUN, 1, Define to 1 if union semun is defined in sys/sem.h), orx_result=no )
    AC_MSG_RESULT($orx_result)
    ])


dnl
dnl Check which values are valid for pthread_mutexattr_settype() arg 4
dnl
AC_DEFUN([ORX_PTHREAD_MUTEXATTR_SETTYPE_ARG2],
    [
    AC_MSG_CHECKING(which values are valid for pthread_mutexattr_settype arg 2 are valid)
    AC_TRY_COMPILE([
    #ifdef HAVE_PTHREAD_H
    #include <pthread.h>
    #endif
    #ifdef HAVE_SYS_SEM_H
    #include <sys/sem.h>
    #endif
    ],
    [
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
    ],
      orx_result=yes; AC_DEFINE(HAVE_PTHREAD_MUTEX_RECURSIVE_NP, 1, Define to 1 if PTHREAD_MUTEX_RECURSIVE_NP is a valid value), orx_result=no )
    AC_TRY_COMPILE([
    #ifdef HAVE_PTHREAD_H
    #include <pthread.h>
    #endif
    #ifdef HAVE_SYS_SEM_H
    #include <sys/sem.h>
    #endif
    ],
    [
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
    ],
      orx_result=yes; AC_DEFINE(HAVE_PTHREAD_MUTEX_RECURSIVE, 1, Define to 1 if PTHREAD_MUTEX_RECURSIVE is a valid value), orx_result=no )
    AC_TRY_COMPILE([
    #ifdef HAVE_PTHREAD_H
    #include <pthread.h>
    #endif
    #ifdef HAVE_SYS_SEM_H
    #include <sys/sem.h>
    #endif
    ],
    [
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
    ],
      orx_result=yes; AC_DEFINE(HAVE_PTHREAD_MUTEX_ERRORCHECK, 1, Define to 1 if PTHREAD_MUTEX_ERRORCHECK is a valid value), orx_result=no )
    AC_MSG_RESULT(found)
    ])

