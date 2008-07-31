/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2008 Rexx Language Association. All rights reserved.    */
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
 * Example of getShortPath() / getLongPath().  Note that the underlying Windows
 * APIs that these methods use only work if the path already exists.  If you
 * just make up a path name, you just get that path name back.
 */

  -- Examples using relative paths
  path  = "..\examples\short.path.data\activatedWindows.frm"
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  path  = "..\examples\short.path.data\activatedHook.file"
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  -- Example using just a file name
  file  = "ShortPathDataFile.bin"
  short = .Path~getShortPath(file)
  long  = .Path~getLongPath(short)
  say 'File name:      ' file
  say 'Short file name:' short
  say 'Long from short:' long
  say

  -- Examples using a full path name.  Will get some known directories using a
  -- WinShell method.
  shell = .WinShell~new
  path  = shell~pathFromCSIDL("CSIDL_HISTORY")
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  path  = shell~pathFromCSIDL("CSIDL_PROGRAMS")
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  -- Examples using a made up path name.
  path  = "C:\My Bogus Diretory\myBogus.file"
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  file  = "someTotallyNonexistentFile.ImadeUp"
  short = .Path~getShortPath(file)
  long  = .Path~getLongPath(short)
  say 'File name:      ' file
  say 'Short file name:' short
  say 'Long from short:' long
  say

  return 0

::requires 'WinShell.cls'
