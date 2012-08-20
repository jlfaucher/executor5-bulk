/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2012 Rexx Language Association. All rights reserved.    */
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

-- This Rexx program can be used to compile a resource script file to a resource
-- only DLL (a binary resource.)
--
-- It assumes the Microsoft tools are in the path.
--
-- Syntax:
--
--   rexx makeDll.rex inputName <outputName>
--
-- Where:
--
-- inputName is required and is the name of the resource script file.
--
-- outputName is optional and is the name of the DLL file.  if outputName is
-- omitted, the the name of the DLL file will be constructed from the name of
-- the resource file.
--
-- Examples:
--
-- Compile ApplicationIcon.rc to ApplicationIcon.dll:
--
--   rexx makeDll.rex ApplicationIcon.rc
--
-- Compile resources.dlg to pplication.dll:
--
--   rexx makeDll.rex resources.dlg Application.dll
--
use arg cmdLine

  if arg(1, 'O') then return doHelp()

  parse var cmdLine inFile outFile .

  baseName = getBaseName(inFile)

  if outFile == "" then do
    outFile = baseName'.dll'
  end

  if \ haveTools() then return doHelp()

  'rc -r -fo' || baseName'.res' inFile
  'link' baseName'.res /NOENTRY /DLL /MACHINE:X86 /OUT:'outFile

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

::routine getBaseName
  use strict arg inFile

  ext = FileSpec('E', inFile)

  if ext == "" then return infile
  else return inFile~left(inFile~lastPos(ext) - 2)


::routine doHelp
  say
  say "makeDLL: compile a resource only DLL using Microsoft's tools."
  say 'rc.exe and link.exe must be in the path.'
  say
  say 'Syntax: rexx makeDll.rex rcFile [outFile]'
  say 'where rcFile is the input file, the resource script.'
  say 'outFile is optional, use it to rename the output file.'
  say
  say 'Examples:'
  say 'makeDll OpenWatcom.rc'
  say '  Compiles OpenWatcom.rc producing OpenWatcom.dll.'
  say
  say 'makeDll OpenWatcom.dlg OpenWatcomMS.dll'
  say '  Compiles OpenWatcom.dlg producing OpenWatcomMS.dll.'

  return 0
