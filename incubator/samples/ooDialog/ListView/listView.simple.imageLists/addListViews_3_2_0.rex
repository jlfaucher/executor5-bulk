/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
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

/* Requires ooRexx 3.2.0 or later. */

  dlg = .ListViews~new("iconList.rc", IDD_DIALOG, , "iconList.h")
  if dlg~initCode == 0 then do
    dlg~Execute("SHOWTOP", IDI_DLG_OOREXX)
    dlg~Deinstall
  end

-- End of entry point.

::requires "OODWIN32.CLS"

::class 'ListViews' subclass RcDialog inherit AdvancedControls MessageExtensions

::method initDialog
  expose list

  self~getRadioControl(IDC_RB_ICON)~check
  self~connectButtonNotify(IDC_RB_REPORT, "CLICKED", onReport)
  self~connectButtonNotify(IDC_RB_LIST, "CLICKED", onList)
  self~connectButtonNotify(IDC_RB_ICON, "CLICKED", onIcon)
  self~connectButtonNotify(IDC_RB_SMALL_ICON, "CLICKED", onSmallIcon)

  list = self~getListControl(IDC_LV_VIEWS)
  list~setSmallImages("iconList_16.bmp")
  list~setImages("iconList_32.bmp")

  self~populateList(list)

::method onReport
  expose list
  list~replaceStyle("LIST ICON SMALLICON", "REPORT")

::method onList
  expose list
  list~replaceStyle("REPORT ICON SMALLICON", "LIST")

::method onIcon
  expose list
  list~replaceStyle("LIST REPORT SMALLICON", "ICON")

::method onSmallIcon
  expose list
  list~replaceStyle("LIST ICON REPORT", "SMALLICON")

::method populateList private
  use strict arg list

  list~insertColumn(0, "Title", 70)
  list~insertColumn(1, "Name", 35)
  list~insertColumn(2, "Last", 50)
  list~insertColumn(3, "e-mail", 70)

  list~addRow(0, 6, "Business Manager",   "Tom",    "Saywer",   "ts@google.com" )
  list~addRow(1, 1, "Software Developer", "Sam",    "Frank",    "boo@gmail.com")
  list~addRow(2, 7, "Accountant",         "Cienna", "Smith",    "cs@yahoo.com")
  list~addRow(3, 5, "Lawyer",             "Mary",   "Tyler",    "fkan@qualcom.com")
  list~addRow(4, 2, "Doctor",             "Sue",    "Acer",     "sa@sharp.org")
  list~addRow(5, 3, "Clerk",              "Harry",  "Houdini",  "HH@magic.net")
  list~addRow(6, 4, "Nurse",              "Mike",   "Thompson", "mike@microsoft.com")

  list~addExtendedStyle("FULLROWSELECT UNDERLINEHOT ONECLICKACTIVATE")

