#!/usr/bin/rexx
/*
  SVN Revision: $Rev$
  Change Date:  $Date$
*/
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010 - 2010 Rexx Language Association. All rights reserved.  */
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
 * This Rexx program is used to set uniform svn properties on the test files
 * that make up the test suite.
 *
 * Currently, it needs to be run from the root directory of the test suite and
 * only on Windows (because of a few directory slashes.)  Feel free to enhance
 * it to remove those restrictions.
 *
 * Early on it was decided to have all test files use Unix (LF) line endings.
 * This way the test suite can be zipped up, unzipped on any system, and it will
 * execute.  Because of the #! line, if the files have Windows (0x0D0A) line
 * endings, the files will not excute on Unix-like systems.
 *
 * It was also decided early on to use the svn keywords: Date and Rev with the
 * test suite files.
 *
 */

  j = SysFileTree("ooRexx\*.testGroup", files., "FOS")

  do i = 1 to files.0
    j = doSvn(files.i)
  end

  extra = .array~of("building.frm", "worker.rex", "ooTest.frm", "testOORexx.rex")
  do file over extra
    j = doSvn(file)
  end

  j = SysFileTree("ooRexx\*.rex", files., "FOS")
  do i = 1 to files.0
    j = doSvn(files.i)
  end

  j = SysFileTree("ooRexx\*.cls", files., "FOS")
  do i = 1 to files.0
    j = doSvn(files.i)
  end

::routine doSvn
  use arg file

  'svn propset svn:executable ON' file
  'svn propset svn:eol-style LF' file
  'svn propset svn:keywords "Date Rev"' file
  say 'Did:' file

  return 0
