/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
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
 *  Doc not done
 *
 *
 *  Note that you must add the FULLROWSELECT extended style to the list view in
 *  order to get the correct row number.  If the list view does not have this
 *  style, Windows only reports the row number if the text label in the left-
 *  most column is clicked on.
 *
 */

  dlg = .SimpleLV~new
  if dlg~initCode = 0 then do
    -- Add a symbolic resource ID for the list view.
    dlg~constDir[IDC_LISTVIEW] = 200
    dlg~constDir[IDC_CB]       = 201

    dlg~create(30, 30, 325, 200, "A Simple List View", "VISIBLE")
    dlg~execute("SHOWTOP")
  end

-- End of entry point.

::requires "ooDialog.cls"

::class 'SimpleLV' subclass UserDialog

::method defineDialog
  expose cbVisible

  cbVisible = .false

  self~createComboBox(IDC_CB, 10, 20, 40, 12, 'PARTIAL HIDDEN LIST')
  self~createListView(IDC_LISTVIEW, 10, 20, 305, 145, "REPORT SHOWSELALWAYS")
  self~createPushButton(IDOK, 280, 175, 35, 15, "DEFAULT", "Close")

  self~connectListViewEvent(IDC_LISTVIEW, "CLICK", onClick)
  self~connectListViewEvent(IDC_LISTVIEW, "BEGINSCROLL", onBeginScroll)
  self~connectListViewEvent(IDC_LISTVIEW, "ENDSCROLL", onBeginScroll)

::method initDialog
  expose list cb edit

  cb = self~newComboBox(IDC_CB)
  cb~add('One')
  cb~add('Two')
  cb~add('Three')

  list = self~newListView(IDC_LISTVIEW)

  cb~setParent(list)
  cb~isGrandChild

  list~addExtendedStyle("FULLROWSELECT GRIDLINES CHECKBOXES HEADERDRAGDROP")

  list~insertColumn(0, "Row (Column 1)", 75)
  list~insertColumn(1, "Column 2", 70)
  list~insertColumn(2, "Column 3", 70)

  do i = 1 to 200
    list~addRow(i, , "Line" i, "Line / Col ("i", 2)", "Line / Col ("i", 3)")
  end


::method onClick unguarded
  expose list cb cbVisible lastIdx lastCol
  use arg id, itemIndex, columnIndex, keyState

  if lastIdx == itemIndex & lastCol == columnIndex then do
    if columnIndex > 0 then do
        r = list~getSubitemRect(itemIndex, columnIndex, 'LABEL')
        ret = cb~setWindowPos(list~hwnd, r~left, r~top - 2, r~right - r~left, 4 * (r~bottom - r~top), "SHOWWINDOW NOZORDERCHANGE")
        cbVisible = .true
        cb~assignFocus
    end
    else do
        r = list~getItemRect(itemIndex, 'LABEL')
        ret = cb~setWindowPos(list~hwnd, r~left, r~top, r~right - r~left, 4 * (r~bottom - r~top), "SHOWWINDOW NOZORDERCHANGE")
        cbVisible = .true
        cb~assignFocus
    end
  end

  lastIdx = itemIndex; lastCol = columnIndex

  return 0


::method onBeginScroll unguarded
  expose cbVisible
  use arg ctrlID, dx, dy, lv, isBegin

  if cbVisible then self~hideComboBox
  return 0


::method onComboBoxGrandChildEvent unguarded
  expose list lastIdx lastCol
  use arg id, key, cbCtrl

  if key == 'enter' then do
    text = cbCtrl~selected~strip
    if text \== '' then list~setItemText(lastIdx, lastCol, text)
    self~hideComboBox
  end
  else if key == 'escape' then self~hideComboBox
  else if key == 'killfocus' then self~hideComboBox(.false)

  return 0


::method hideComboBox private unguarded
  expose cb cbVisible list
  use strict arg assignFocus = .true

  cb~hide
  cbVisible = .false
  cb~selectIndex

  if assignFocus then list~assignFocus


