/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

/** A Sample program showing how to use the .SimpleFolderBrowse class in an
 *  ooDialog program.
 */

  -- Create a simple dialog.  The code to actually use .SimpleFolderBrowse is
  -- in the dialog code.

  say 'version   ' .Sh~version
  say 'from local' .WinShell_version

  dlg = .SimpleDlg~new
  if dlg~initCode <> 0 then do
      say "Error creating dialog.  initCode:" dlg~initCode
      return 99
  end

  dlg~Execute('ShowTop')
  dlg~DeInstall

exit

::requires 'oodwin32.cls'
::requires 'WinShell.cls'

::class SimpleDlg subclass userdialog -
                   inherit AdvancedControls MessageExtensions

::method Init
  expose browser counter csidls

  self~init:super
  rc = self~CreateCenter(190, 160, 'Using the WinShell SimpleFolderBrowse Class')
  if rc == 0 then self~initCode = -1

  -- Instantiate a SimpleFolderBrowse object.  This does not need to be done
  -- here, it could just be done when it is actually needed.

  -- The default title and banner can be changed to whatever the programmer
  -- wants.  The 'title' is the tile of the dialog.  The 'banner' is the text
  -- displayed above the tree control in the dialog.
  title  = "Simple Dialog Sample - Browse for Folder Application"
  banner = "Open Object Rexx allows you to choose the folder you need"
  browser = .SimpleFolderBrowse~new(title, banner)

  -- The default root of the tree control can also be changed.  The 'root' is
  -- the topmost folder in the tree control.  The user can not 'browse' above
  -- that point.  This will use the CSIDL_XXXX constants to name the root.
  -- CSIDL_PERSONAL is the 'My Documents' folder.
  browser~root = "CSIDL_PERSONAL"

  -- This sample will cycle through a series of different root folders.  They
  -- are set up in this array.
  counter = 0
  csidls = .array~new
  csidls[1] = "CSIDL_APPDATA"
  csidls[2] = "CSIDL_COMMON_PROGRAMS"
  csidls[3] = "CSIDL_CONNECTIONS"
  csidls[4] = "CSIDL_COOKIES"
  csidls[5] = "CSIDL_DESKTOP"

::method DefineDialog

  instructions = 'When the Browse button is clicked a Browse for Folder dialog '  || -
                 'will be displayed.  After closing the dialog, the result '      || -
                 'appears in the static text box below.  (Long path names will '  || -
                 'be wrapped.)'
  self~addGroupBox(5, 10, 180, 50, 'Instructions:', , 150)
  self~addText(10, 20, 170, 35, instructions, , 155)

  self~addGroupBox(5, 70, 180, 60, 'Result:', , 160)
  self~addText(10, 80, 170, 40, "<N/A>", , 165)

  self~AddButton(300, 5, 140, 75, 15,'Browse', 'onBrowse', "DEFAULT")
  self~AddButton(IDOK, 135, 140, 50, 15, 'Done')

::method InitDialog
  expose staticControl
  staticControl = self~getEditControl(165)

::method onBrowse
  expose staticControl browser counter csidls

  if counter > 0 then do
    if counter > 5 then do
      counter = 0
      browser~root = .nil
    end
    else do
      browser~root = csidls[counter]
    end
  end

  path = browser~getFolder(self~dlgHandle)
  if path == '' then path = "User canceled"
  else if path == .nil then path = "User picked a virtual folder"

  staticControl~setTitle(path)
  counter += 1

::method ok
   return self~ok:super
