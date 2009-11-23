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

/** A simple program demonstrating how to: Extract icon images from a file
 *  containing icons (in this case a DLL.)  Use the extracted icons to set the
 *  icons in a List-view control.
 */

  dlg = .IconDlg~new( , "otherImages.h")
  if dlg~initCode = 0 then do
    dlg~create(30, 30, 370, 230, "Icon Display")
    dlg~Execute("SHOWTOP")
    dlg~Deinstall
  end

-- End of entry point.

::requires "OODWIN32.CLS"
::requires 'WinShell.cls'

::class IconDlg subclass UserDialog inherit AdvancedControls MessageExtensions

::method initAutoDetection
  self~noAutoDetection

::method defineDialog
  expose vIconNames

  -- Do not use SHAREIMAGES and this way the List-view will destroy the current
  -- image list when it is destroyed.
  self~addListControl(IDC_LIST, , 10, 10, 350, 185, "ICON SINGLESEL");

  self~addRadioButton(IDC_RB_VICONS, , 10, 200, 55, 10, "vIcons.dll", "GROUP")
  self~addRadioButton(IDC_RB_SHELL32, , 10, 215, 55, 10, "shell32.dll")

  self~addButton(IDC_PB_CLOSE, 330, 205, 30, 15, "Close", close, "DEFAULT")

  self~connectButtonNotify(IDC_RB_VICONS, "CLICKED", onVIconsClick)
  self~connectButtonNotify(IDC_RB_SHELL32, "CLICKED", onShell32Click)

  vIconNames = .array~of("3D", "Fabrick", "Froggy", "Hipmatise", "Nightfall", -
                         "Sharky", "The Thing", "Woola", "Ygy", "Torpedo")

::method initDialog
  expose list shell

  list  = self~getListControl(IDC_LIST)
  shell = .WinShell~new

  self~getRadioControl(IDC_RB_VICONS)~check
  self~onVIconsClick

::method onVIconsClick
  expose list vIconNames

  imageList = self~getImageList("vIcons.dll")
  if imageList == .nil then return

  self~setImages(imageList)
  count = imageList~getImageCount

  -- Remove any existing items.
  list~deleteAll

  do i = 0 to (count - 1)
    list~add(vIconNames[i+1], i)
  end

::method onShell32Click
  expose list

  imageList = self~getImageList("Shell32.dll")
  if imageList == .nil then return

  self~setImages(imageList)
  count = imageList~getImageCount

  -- Remove any existing items.
  list~deleteAll

  -- Now add the items.
  do i = 0 to (count - 1)
    list~add(i, i)
  end


::method setImages private
  expose list
  use strict arg imageList

  -- The user, (the ooRexx programmer,) is responsible for destroying the old
  -- image list.
  oldIL = imageList~setListViewImages(list~hwnd, LVSIL_NORMAL)
  say 'set image list old image list:' oldIL
  if oldIL <> 0 then .ImageList~destroy(oldIL)


::method getImageList private
  expose shell
  use strict arg file

  imageList = .nil

  iconDll = .Path~searchAndQualify(file)
  icons = shell~extractDefaultIcons(iconDll)

  if icons \== .nil then do
    largeIcons = icons[1]
    imageList = .ImageList~new("ICON", largeIcons, 32, 32)
  end

  return imageList


::method close
  return self~ok:super


