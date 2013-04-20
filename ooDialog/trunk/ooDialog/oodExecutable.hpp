/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/**
 * oodExecutable.hpp
 *
 * This file contains defines and other data used by both ooDialog.com and
 * ooDialog.exe, along with the .rc file for those 2 executables.
 *
 */

#ifndef oodExeuctable_Included
#define oodExeuctable_Included

#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

#define IDD_OODIALOG_DISPLAY                    102
#define IDC_ST_DISPLAY_ICON                     1001
#define IDC_EDIT                                1003
#define IDC_ST_SHORT_MSG                        1004
#define IDI_APP_ICON                            200

#ifndef RC_INVOKED

static char *long_syntax_text =
"The ooDialog executable is a stand alone interface to run ooDialog programs, "
"or to optionally provide other ooDialog services and information.\r\n\r\n"
"Syntax:\r\n\r\n"
"    ooDialog [option flags] [programName] [arg1 arg2 ... argN]\r\n\r\n"
"If no arguments are given, this help is shown.  All option flags start with "
"the '-' character.  The first argument that does not start with the '-' char"
"acter is taken to be the name of an ooRexx program to be executed.  All argu"
"ments following programName are passed as arguments to programName.  When "
"programName is executed, it is executed with no console window.  This is simi"
"lar to rexxHide.\r\n\r\n"
"option flags:\r\n"
"  -h        Show the short help.\r\n"
"  -H        Show this, longer, help.\r\n"
"  --help    When run from a console window, show the long help.\r\n"
"  -s        Display the ooDialog Setup dialog.\r\n"
"  -v        Print the short version string.\r\n"
"  -V        Print the long version string.\r\n"
"  --version When run from a console window, print the long version string.\r\n\r\n"
"programName:\r\n"
"  The first argument that does not begin with '-' is taken as the name"
"  of a Rexxx program to be executed.  The program is executed without"
"  creating a console window."
"\r\n";

static char *short_syntax_text =
"Syntax:\r\n\r\n"
"    ooDialog [option flags] [programName] [arg1 arg2 ... argN]\r\n\r\n";

#endif /* not defined RC_COMPILER_INVOKED*/

#endif /* defined oodExeuctable_Included */
