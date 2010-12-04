/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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

/* accessShortCuts.rex   Rexx program using the OLEObject.  */

  shell = .OleObject~new("Shell.Application")

  ssfPROGRAMS = 2
  folder = shell~NameSpace(ssfPROGRAMS)
  folderItem = folder~ParseName("Internet Explorer.lnk")

  -- The shell link is the short cut object.  The properties of the short cut
  -- are those printed out.
  shellLink = folderItem~GetLink
  say 'shellLink:' shellLink 'name:' folderItem~name
  say '  working directory:' shellLink~workingDirectory
  say '  path:             ' shellLink~path
  say '  target:           ' shellLink~target~name
  say '  arguments:        ' shellLink~arguments
  say '  description:      ' shellLink~description
  say '  hot key:          ' shellLink~hotKey
  say '  show command:     ' shellLink~showCommand
  say

  folder = shell~nameSpace(directory())
  folderItem = folder~parseName("Right Command Prompt.lnk")
  shortCut = folderItem~getLink
  say 'ShellLink:' shortCut 'name:' folderItem~name
  say '  working directory:' shortCut~workingDirectory
  say '  path:             ' shortCut~path
  say '  target:           ' shortCut~target~name
  say '  arguments:        ' shortCut~arguments
  say '  description:      ' shortCut~description
  say '  hot key:          ' shortCut~hotKey
  say '  show command:     ' shortCut~showCommand
  say

  ssfCOMMONPROGRAMS = 23
  folder = shell~nameSpace(ssfCOMMONPROGRAMS)
  folderItems = folder~items

  ooRexxFolder = .nil
  do item over folderItems
    say "item:" item~name
    if item~name = "Open Object Rexx" then do
      ooRexxFolder = item
      leave
    end
  end
  say

  if ooRexxFolder == .nil then do
    say "Failed to get the ooRexx folder in the Start Menu, have to quit."
    return
  end

  if ooRexxFolder~isFolder then do
    ooRexxFolder = ooRexxFolder~getFolder
  end
  else do
    say "Got something for the ooRexx folder, but it is not a folder, have to quit"
    return
  end

  folderItem = ooRexxFolder~parseName("Try Rexx (GUI).lnk")
  shortCut = folderItem~getLink
  say 'ShellLink:' shortCut 'name:' folderItem~name
  say '  working directory:' shortCut~workingDirectory
  say '  path:             ' shortCut~path
  say '  target:           ' shortCut~target~name
  say '  arguments:        ' shortCut~arguments
  say '  description:      ' shortCut~description
  say '  hot key:          ' shortCut~hotKey
  say '  show command:     ' shortCut~showCommand
  say
