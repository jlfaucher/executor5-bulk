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


  shell = .WinShell~new

  title = "ooRexx Windows Shell for ooDialog Programmers"
  app = "Vail Miesfeld's Icon Display"
  other = "Icons created by Vail Miesfeld."

  if shell~about(title, app, other) then
    say "About dialog created successfully."
  else
    say "Failed to create the About dialog."

  iconDll = .Path~searchAndQualify("vIcons.dll")
  iconDetails = shell~selectIcon(iconDll)

  if iconDetails == .nil then do
    say 'The user did not select an icon.'
  end
  else do
    say 'The user made an icon selection:'
    say '  The full path to the icon file is:' iconDetails[2]
    say '  The selected icon is at index:    ' iconDetails[1]

    -- Extract the big and small icons using the selection.  The two icons are
    -- returned in an array, with the big icon at index 1 and the small icon at
    -- index 2
    icons = shell~extractDefaultIcons(iconDetails[2], iconDetails[1])
    say 'Return from extractDefaultIcons():' icons
    if icons~isA(.array) then do
      say 'Big icon handle:  ' icons[1]
      say 'Small icon handle:' icons[2]

      msgTitle = "About Dialog Example - Information -"

      -- Now put up the about dialog adding the big icon.
      msg = "Showing the About dialog again, adding" || '0d0a'x || "a custom icon (the big icon.)"
      j = RxMessageBox(msg, msgTitle)

      shell~about(title, app, other, , icons[1])

      -- Now again with the small icon.
      msg = "Showing the About dialog again, adding" || '0d0a'x || "a custom icon (the small icon.)"
      j = RxMessageBox(msg, msgTitle)

      shell~about(title, app, other, , icons[2])

      -- The icon handles need to be released:
      ret = shell~releaseIcon(icons[1])
      say 'Released big icon, return:  ' ret
      ret = shell~releaseIcon(icons[2])
      say 'Released small icon, return:' ret
    end
  end

  return


::requires 'WinShell.cls'
