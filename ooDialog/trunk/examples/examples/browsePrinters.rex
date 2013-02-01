/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
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

/**
 * A quick example of the SimpleFolderBrowse class.  This class only has one
 * method: getFolder() and it is a class method.
 *
 * It is very easy to use, you just invoke the getFolder() method and the folder
 * picked by the user is returned.  When the user picks a folder the fully
 * qualified path name is returned.  If the user cancels, the empty string is
 * returned.
 *
 * It is possible, depending on how the dialog is set up, for the user to pick a
 * virtual folder that has no file system path.  In that case, .nil is returned.
 *
 */

    -- Set up some symbolic IDs and then put up our dialog.
    .application~setDefaults('O', , .false)
    .constDir[IDC_PB_BROWSE]      = 100
    .constDir[IDC_ST_RESULTS]     = 101
    .constDir[IDC_EDIT]           = 102
    .constDir[IDC_RB_LONG_NAME] = 103
    .constDir[IDC_RB_SHORT_NAME]  = 104

    dlg = .PrintersDialog~new
    dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

    return 0

::requires "ooDialog.cls"

::class 'PrintersDialog' subclass UserDialog

::method init
  forward class (super) continue

  self~create(30, 30, 257, 123, "Browse For Printers Example", "CENTER")

::method defineDialog

    self~createStaticText(IDC_ST_RESULTS, 10, 10, 30, 11, , "Results:")
    self~createEdit(IDC_EDIT, 10, 24, 237, 11, 'AUTOSCROLLH')
    self~createRadioButton(IDC_RB_LONG_NAME, 10, 60, 90, 14, "AUTO", 'Long Display Name')
    self~createRadioButton(IDC_RB_SHORT_NAME, 10, 79, 90, 14, "AUTO", 'Short Display Name')
    self~createPushButton(IDC_PB_BROWSE, 10, 99, 65, 14, "DEFAULT", "Browse Printers", onBrowse)
    self~createPushButton(IDCANCEL, 197, 99, 50, 14, , "Done")


/** onBrowse()
 *
 * The event handler for the Browse Printers push button.  We configure a
 * BrowseForFolder object, display it, and report the user's actions in the edit
 * control
 */
::method onBrowse unguarded
  expose rbShort edit

    title  = 'Browse For a Printer'
    banner = 'Select the printer to use for this test.'
    hint   = 'This is only a demonstration, no printing will be done.'

    bff = .BrowseForFolder~new(title, banner, hint)
    bff~owner = self
    bff~root = 'CSIDL_PRINTERS'
    bff~options = 'BROWSEFORPRINTERS NONEWFOLDERBUTTON'

    pidl = bff~getItemIDList(.true)
    if pidl == .nil then do
        edit~setText('The user canceled.')
        return 0
    end

    if rbShort~checked then do
        name = bff~getDisplayName(pidl, 'NORMALDISPLAY')
    end
    else do
        name = bff~getDisplayName(pidl)
    end

    -- We are done with pidl, release it.
    bff~releaseItemIDList(pidl)

    -- Determine text for the edit control ...
    if name == .nil then do
        text = 'Unexpected result. ' .DlgUtil~errMsg(.systemErrorCode)
    end
    else do
        text = 'The user picked: ' name
    end

    -- ... and set it
    edit~setText(text)

    return 0


/** initDialog()
 *
 * Simple init dialog method.  We get references to the edit control and one of
 * the two radio buttons.  Since the radio buttons are auto and there are only 2
 * of them we only need one reference to be able to tell which is checked.
 */
::method initDialog
    expose rbShort edit

    edit = self~newEdit(IDC_EDIT)
    rbShort = self~newRadioButton(IDC_RB_SHORT_NAME)
    rbShort~check
