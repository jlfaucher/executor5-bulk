/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2011 Rexx Language Association. All rights reserved.    */
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

/** createShortCut.rex
 *
 * Example of how to create a short cut using OLEObject.
 *
 * Surprising, I do not see any way to create a short cut using the Shell
 * object: shell = .OleObject~new("Shell.Application").  Fortunately, we can get
 * the WScript.Shell object which does have a createShortCut() method.
 */

  -- Get the WScript Shell object.
  wshShell = .OleObject~new("WScript.Shell")

  -- We will create the short cut on the desk top.
  deskTopLocation = wshShell~specialFolders("DeskTop")

  -- Create a short cut object using the file name of the short cut we want.
  -- Note that this does not, yet, create an actual file.  The short cut is in
  -- memory only.
  shortCut = wshShell~createShortcut(deskTopLocation || "\An ooDialog Samples Short Cut.lnk")

  -- We need to locate the ooRexx installation first.
  ooRexxInstallDir = getOORexxInstallation(wshShell)
  if ooRexxInstallDir == "" then do
    say "Could not locate the directory ooRexx is installed in.  Will have to quit."
    return 9
  end

  -- Assign the short cut properties.
  shortCut~targetPath = ooRexxInstallDir || "\rexxhide.exe"
  shortCut~arguments = ooRexxInstallDir || "\samples\oodialog\sample.rex"
  shortCut~windowStyle = 1
  shortCut~hotkey = "CTRL+SHIFT+F"
  shortCut~iconLocation = ooRexxInstallDir || "\rexx.exe, 0"
  shortCut~description = "Test ooDialog Sample Shortcut"
  shortCut~workingDirectory = ooRexxInstallDir || "\samples\oodialog"

  -- Saving the short cut object is what actually creates short cut file.
  shortCut~Save

  return 0


 ::routine getOORexxInstallation
   use strict arg shell

   wshSysEnv = shell~environment("SYSTEM")
   return wshSysEnv~item("REXX_HOME")

::requires '..\OleUtils.frm'
