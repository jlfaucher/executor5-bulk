/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2012 Rexx Language Association. All rights reserved.    */
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
 *  Helper program to produce a list of ooDialog example files only in 4.1.0
 *  and only in 4.2.0.
 *
 *  Does the same thing for 3.2.0 and 4.1.0.
 *
 *  Requires a working ooRexx and the GnuWin32 tools, which and grep.  These
 *  tools are available from the GnuWin32 project on SourceForge.
 */

 -- Be sure we have grep.
'which grep > \nul 2>&1'
if rc <> 0 then do
  say "No which or no grep found on system, aborting."
  return 9
end

curDir = directory("..\..\..\samples\windows")

-- The differences between 4.1.0 and 4.2.0
f1 = "oodialog.4.1.0"
f2 = "oodialog.4.2.0"
toFile = "> ..\..\platform\windows\install\onlyIn410.files 2>&1"

'diff -rq -x .svn' f1 f2 '| grep "Only in" | grep "4\.1\.0"' toFile

toFile = "> ..\..\platform\windows\install\onlyIn420.files 2>&1"
'diff -rq -x .svn' f1 f2 '| grep "Only in" | grep "4\.2\.0"' toFile


-- The differences between 3.2.0 and 4.1.0
f1 = "oodialog.3.2.0"
f2 = "oodialog.4.1.0"
toFile = "> ..\..\platform\windows\install\onlyIn320_410.files 2>&1"

'diff -rq -x .svn' f1 f2 '| grep "Only in" | grep "3\.2\.0"' toFile

toFile = "> ..\..\platform\windows\install\onlyIn410_320.files 2>&1"
'diff -rq -x .svn' f1 f2 '| grep "Only in" | grep "4\.1\.0"' toFile

j = directory(curDir)
