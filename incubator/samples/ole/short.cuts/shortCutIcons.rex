/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2011 Rexx Language Association. All rights reserved.    */
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

/** shortCutIcons.rex   Rexx program using the OLEObject.
 *
 * Displays the short cut icon information for all the short cuts found on the
 * desk top.
 *
 * Icons are contained in the resource section of an executable file (a .exe or
 * a .dll file.)  Any number of icons can be bound to the resource section.  The
 * individual icon is accessed by its index.
 */

  shell = .OleObject~new("Shell.Application")

  -- Get the desk top folder
  ssfDESKTOP = 0
  folder = shell~NameSpace(ssfDesktop)

  -- Find all the short cuts in the desk top by examining each item in the desk
  -- top.
  shortCuts = .array~new
  items = folder~items
  do item over folder~items
    -- If the folder item is a link, save it.
    if item~isLink then shortCuts~append(item)
  end

  -- Get the icon information for each short cut we found.
  do shortCut over shortCuts
    -- Need to get the shell link object.
    shellLink = shortCut~getLink

    -- The getIconLocation() method of a shell link object gets the file that
    -- contains the icon and the icon index in that file.  The return from the
    -- method is the icon index.  The icon file is returned in the argument to
    -- the function.  We use an oleVariant to capture the returned icon file.
    path = .oleVariant~new("", , "OUT")
    index = shellLink~getIconLocation(path)
    iconFile = path~!varValue_

    -- Now display the icon information.  Note this: if the short cut was
    -- created without assigning an icon, the Shell will use as a default the
    -- icon at index 0 of the short cut target.  In this case, the icon file
    -- returned by getIconLocation() will be the empty string.
    if iconFile == "" then iconFile = shellLink~path

    say "Found short cut:" shortCut~name
    say '  File containing icon:' iconFile
    say '  At index:            ' index
    say
  end

  return 0

