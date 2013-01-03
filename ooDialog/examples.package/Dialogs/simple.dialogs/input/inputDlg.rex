/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2013 Rexx Language Association. All rights reserved.    */
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
 * This is a simple dialog showing how to collect some information from a user
 * and return it.
 *
 * The information entered by the user is validated, to a small degree.  This
 * simple program merely checks that the user entered something for each entry
 * field.  It does not try to validate that the IP address is in the correct
 * form.
 */

  -- Use the .constDir only, and add some sybmolic IDs to it for the edit
  -- controls in our dialog
  .application~setDefaults('O', , .false)
  .constDir[IDC_EDIT_IP] = 100
  .constDir[IDC_EDIT_ID] = 105
  .constDir[IDC_EDIT_PW] = 110

  dlg = .InputDlg~new
  ret = dlg~execute("SHOWTOP")

  -- When the execute() method returns 1, it means the user closed the dialog
  -- with the 'Ok' button.  If 2 is returned, it means the user canceled the
  -- dialog.
  if ret == 1 then do
    say 'IP address:' dlg~ipAddr
    say 'TSO ID:    ' dlg~tsoID
    say 'Password:  ' dlg~passWord
  end
  else do
    say 'The user canceled the dialog.'
  end

::requires "ooDialog.cls"

::class 'InputDlg' subclass UserDialog

-- These attributes will be used to return the
-- users input.
::attribute ipAddr
::attribute tsoID
::attribute passWord

::method init
  forward class (super) continue

  self~ipAddr   = ""
  self~tsoID    = ""
  self~passWord = ""

  self~create(30, 30, 147, 100, "Enter Log On Details", "CENTER")


/** defineDialog()
 *
 * This method is invoked automatically by ooDialog.  Use it to lay out your
 * dialog controls.
 */
::method defineDialog

  self~createStaticText(IDC_STATIC, 10, 12, 45, 14, , "IP Address:")
  self~createEdit(IDC_EDIT_IP, 58, 10, 80, 14, , "AUTOSCROLLH")

  self~createStaticText(IDC_STATIC, 10, 35, 45, 14, , "TSO User ID:")
  self~createEdit(IDC_EDIT_ID, 58, 33, 80, 14, , "AUTOSCROLLH")

  self~createStaticText(IDC_STATIC, 10, 53, 45, 14, , "Password:")
  self~createEdit(IDC_EDIT_PW, 58, 51, 80, 14, "AUTOSCROLLH PASSWORD")

  self~createPushButton(IDOK, 32, 79, 50, 14, "DEFAULT", "Ok")
  self~createPushButton(IDCANCEL, 87, 79, 50, 14, , "Cancel")


/* validate()
 *
 * This method is invoked automatically by ooDialog.  Use it to validate the
 * users input.  Very simple validation done here.
 */
::method validate

  -- Get the text entered by the user in all the edit controls.
  ip = self~newEdit(IDC_EDIT_IP)~getText
  id = self~newEdit(IDC_EDIT_ID)~getText
  pw = self~newEdit(IDC_EDIT_PW)~getText

  -- Simple check: did the user enter something in all the fields?  If need,
  -- show a message to the user explaining why the dialog is not closing and
  -- return .false.  Returning .false tells the ooDialog framework to not close.
  if ip == '' | id == '' | pw == '' then do
    title = 'Data Entry Error'
    msg   = 'You must fill out all information.'
    ret = messageDialog(msg, self~hwnd, title, 'OK', 'WARNING')
    return .false
  end

  -- Okay, we have some text in all the fields.  Set our attributes to the text
  -- of the appropriate edit control and return .true.  Returning .true tells
  -- the ooDialog framework to continue with closing the underlying dialog.
  self~ipAddr   = ip
  self~tsoID    = id
  self~passWord = pw
  return .true
