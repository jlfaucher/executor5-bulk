/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2009-2012 Rexx Language Association. All rights reserved.    */
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

/**
 *  Example of a simple list-view that uses image lists to supply the icon for
 *  each list-view item.
 *
 *  List-views use both a 'normal' (32x32) icon and a 'small' (16x16) icon.  The
 *  normal icon is used in Icon view and the small icon is used for the
 *  SmallIcon view.  The icons are supplied to the list-view through image
 *  lists, a separate image list for the normal and small icons.
 *
 *  For this example, the icons for the list-view items are contained in 2
 *  bitmap files.  The image lists are created by loading the files.
 *
 *  List-view controls allow different views, report view, icon view, etc..  The
 *  example dialog shows a single list-view and 4 radio buttons.  The radio
 *  buttons allow the user to switch between 4 different view, list, report,
 *  icon and small icon.
 *
 *  The logic of the example is relatively simple.  Each radio button is
 *  connected to an event handler for the CLICKED event.  The event handler when
 *  invoked, switches the list-view to the view specified by the label of the
 *  radio button.
 *
 *  Requires ooDialog 4.2.0 or later.
 */

  .application~setDefaults('O', "iconList.h")

  dlg = .ListViews~new("iconList.rc", IDD_DIALOG)

  if dlg~initCode == 0 then dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

return 0
-- End of entry point.

::requires "ooDialog.cls"

::class 'ListViews' subclass RcDialog

::method initDialog
  expose list

  -- Start in Icon view, so set that radio button to checked.
  self~newRadioButton(IDC_RB_ICON)~check

  -- Connect the CLICKED event for each radio button to the proper event handler.
  self~connectButtonNotify(IDC_RB_REPORT, "CLICKED", onReport)
  self~connectButtonNotify(IDC_RB_LIST, "CLICKED", onList)
  self~connectButtonNotify(IDC_RB_ICON, "CLICKED", onIcon)
  self~connectButtonNotify(IDC_RB_SMALL_ICON, "CLICKED", onSmallIcon)

  -- Instantiate a new list-view object.
  list  = self~newListView(IDC_LV_VIEWS)

  -- Create the 2 images lists, one for the normal icons and one for the small
  -- icons.
  flags = .Image~toID(ILC_COLOR24)
  imageList = .ImageList~create(.Size~new(16), flags, 7, 0)

  imageList~add(.Image~getImage("iconList_16.bmp"))
  list~setImageList(imageList, .Image~toID(LVSIL_SMALL))

  imageList = .ImageList~create(.Size~new(32), flags, 7, 0)

  imageList~add(.Image~getImage("iconList_32.bmp"))
  list~setImageList(imageList, .Image~toID(LVSIL_NORMAL))

  -- Add the list-view items to the list-view.
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

  -- The first argument to addRow() is the index of the item.  The second
  -- argument is the index in the image list for the icon(s) of the item.  The
  -- single image list index is used for both the normal icon and the small
  -- icon.  So the 2 image lists need to keep the normal and small icons at the
  -- same index for each item.
  --
  -- Here, the icon indexes are assigned to the items in random order just to
  -- show that the icons in the image list do not need to be in the same order
  -- as the order of the items.
  list~addRow(0, 6, "Business Manager",   "Tom",    "Saywer",   "ts@google.com" )
  list~addRow(1, 1, "Software Developer", "Sam",    "Frank",    "boo@gmail.com")
  list~addRow(2, 0, "Accountant",         "Cienna", "Smith",    "cs@yahoo.com")
  list~addRow(3, 5, "Lawyer",             "Mary",   "Tyler",    "fkan@qualcom.com")
  list~addRow(4, 2, "Doctor",             "Sue",    "Acer",     "sa@sharp.org")
  list~addRow(5, 3, "Clerk",              "Harry",  "Houdini",  "HH@magic.net")
  list~addRow(6, 4, "Nurse",              "Mike",   "Thompson", "mike@microsoft.com")

  list~addExtendedStyle("FULLROWSELECT UNDERLINEHOT ONECLICKACTIVATE")

