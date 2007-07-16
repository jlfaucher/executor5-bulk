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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  File:    xnTemplate.rex                           Author:      Mark Miesfeld
                                                    Creation date:  06/27/2007
  Company: N/A
  Project: ooDialog Examples                        Last update:    07/14/2007
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Category:
    Command Line

  Syntax:
    C:\>rexx xnTemplate.rex

  Purpose:
    Display the XN Resource Editor template dialog, which is simply an empty
    dialog with an ok button.

    ooDialog users can start with the xnTemplate.dll file and use the XN
    resource editor to build their own dialog.

    If the XN Resource Editor starts with a compiled resource in a DLL, it can
    save the changes back to a DLL.  However, if a new dialog resource is
    created in the editor, it can only save the dialog to a .res file.  This
    file would then need to be linked into a DLL in order for ooDialog to be
    able to use.

    Therefore, a good approach to using the XN Resource Editor, is to start with
    an already compiled resource DLL, open the DLL in XN, use the visual editor
    to change the existing dialog to whatever is desired, then save the DLL
    under another name.

    By always starting a new dialog using the template, ooDialog users would
    be able to create their new dialogs using just the XN Resource Editor,
    without any additional tools such as a linker.  The steps are as follows:

    1.) Open the xnTemplate.dll file in the XN editor.

    2.) Use 'Save as' to save it to a DLL under another file name.

    3.) Edit the dialog.

    4.) Save it as a DLL file under the new name.

  Notes:
    XN Resource Editor is a free, powerful, fully featured resource editor and
    PE module explorer for Windows 98, Windows 2000 and Windows XP.

    The XN website:

    http://www.wilsonc.demon.co.uk/delphi.htm

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
use arg cmdLine

  dlg = .ResDialog~new("xnTemplate.dll", 100)
  if dlg~initCode == 0 then do
    dlg~execute( "SHOWTOP" )
    dlg~deInstall
  end

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "oodwin32.cls"

/* - - - End Of File: xnTemplate.rex- - - - - - - - - - - - - - - - - - - - - */
