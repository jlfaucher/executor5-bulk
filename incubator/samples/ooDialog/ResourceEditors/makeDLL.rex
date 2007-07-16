/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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

-- Compile a resource script file to a resource only DLL (a binary resource.)
-- Assumes, the Microsoft tools are in the path.
-- First arg is the resource script file name, without the extension.  The
-- second arg is optional and can be used to rename the output DLL file.
use arg cmdLine

  if arg(1, 'O') then return doHelp()

  parse var cmdLine inFile outFile .
  if outFile == "" then outFile = inFile
  inFile = inFile'.rc'

  if \ haveTools() then return doHelp()

  'rc -r -fo'outFile'.res' inFile
  'link' outFile'.res /NOENTRY /DLL /MACHINE:X86 /OUT:'outFile'.dll'

return 0

-- Simplistic check for rc.exe to see if the tools are accessible.
::routine haveTools
  'rc /? > nul 2>&1'
  rcRet = RC
  if rcRet <> 0 then do
    say "rx.exe or link.exe (or both) do not appear to be in the path."
    return .false
  end

return .true

::routine doHelp
  say
  say "makeDLL: compile a resource only DLL using Microsoft's tools."
  say 'rc.exe and link.exe must be in the path.'
  say
  say 'Syntax: makeDll rcFile [outFile]'
  say 'where rcFile is the input file, leave off the .rc extension.'
  say 'outFile is optional, used to rename the output file.'
  say
  say 'Examples:'
  say 'makeDll OpenWatcom'
  say '  Compiles OpenWatcom.rc producing OpenWatcom.dll.'
  say
  say 'makeDll OpenWatcom OpenWatcomMS'
  say '  Compiles OpenWatcom.rc producing OpenWatcomMS.dll.'

  return 0
