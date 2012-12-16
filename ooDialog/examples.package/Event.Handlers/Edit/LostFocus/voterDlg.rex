/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2012 Rexx Language Association. All rights reserved.    */
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
 *  voterDlg.rex
 *
 *  This example program demonstrates a LOSTFOCUS event handler for the Edit
 *  control.
 *
 *  This dialog depends on the resource IDs for all the radio buttons being in
 *  order and sequential.  Typically this won't be the case when a resource
 *  editor manages the resource IDs.  However, it is a simple matter to edit the
 *  file containing the resource IDs after the design of the dialog is finished.
 *  That is what was done here.
 *
 */

   .application~setDefaults('O', 'voterDlg.h', .false)

   dlg = .VoterDlg~new("voterDlg.rc", IDD_VOTE)
   if dlg~initCode <> 0 then do
     say 'Dialog initialization failed.  Init code:' dlg~initCode
     say 'Aborting.'
     return dlg~initCode
   end

   ret = dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

   if ret == dlg~IDOK then say 'Rexx interpreter picked:' dlg~picked
   else say 'The user canceled without picking an interpreter.'

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "ooDialog.cls"

::class 'VoterDlg' subclass RcDialog

::attribute picked

/** initDialog()
 *
 * This method is always invoked automatically by the ooDialog framework at the
 * proper time to do any initialization that requires the underylying Windows
 * dialog to exist.
 *
 * Things like setting a radio button to the checked state, setting the focus to
 * a control, etc., can only be done when the underlying Windows controls exist.
 *
 * Other things done here, like connecting events and setting the values of
 * exposed varibles, could be done in the init() or defineDialog() methods.
 * But, for this dialog we do not need to over-ride those methods, so it is just
 * as good to do those things here.
 */
::method initDialog
  expose wiEdit wiText wiRB reginaRB

  self~picked = ""

  -- Instaniate a new RadioButton object that represents the Regina radiog
  -- button.  Assign the initial focus to that button and set its check state to
  -- checked.
  reginaRB = self~newRadioButton(IDC_RB_REGINA)
  reginaRB~assignFocus
  reginaRB~check

  -- Initially there is not a write-in, so disable that readio button.
  wiRB = self~newRadioButton(IDC_RB_WRITE_IN)
  wiRB~disable

  -- Set the write in text variable to the empty string and set a hint in the
  -- edit box that lets the user know what the edit box is for.
  wiText = ''
  wiEdit = self~newEdit(IDC_EDIT_WRITE_IN)
  wiEdit~setCue("Enter a write in canidate if desired", .true)

  -- Connect the got and lost focus events for the edit control to methods in
  -- this dialog.
  self~connectEditEvent(IDC_EDIT_WRITE_IN, GOTFOCUS, "onFocus")
  self~connectEditEvent(IDC_EDIT_WRITE_IN, LOSTFOCUS, "onLostFocus")


/** onFocus()
 *
 * The event handler for the GOTFOCUS event.  Invoked when the edit control
 * gains the focus.  We save the current text of the edit control, so that when
 * the edit control loses the focus, we can see if the user changed anything.
 */
::method onFocus unguarded
  expose wiText wiEdit

  -- Record what the text in the edit control is when the control gains the
  -- focus.
  wiText = wiEdit~getText
  return 0


/** onLostFocus()
 *
 * The event handler for the LOSTFOCUS event.  Invoked when the edit control
 * loses the focus.
 */
::method onLostFocus unguarded
  expose wiText wiEdit wiRB reginaRB

  -- Check if the text has changed from what it was when the edit control
  -- gained the focus.  If it hasn't don't do anything.
  text = wiEdit~getText

  if text \== wiText then do
    wiText = text

    -- Check if the new text is the empty string
    if wiText == '' then do
      -- Remove the lable from the write in radio button
      wiRB~setText('')

      -- If the write in radion button is currently checked, uncheck it and
      -- check the Regina radio button.
      if wiRB~checked then do
        reginaRB~check
        wiRB~uncheck
      end

      -- No write in candiate so disable that radio button.
      wiRB~disable
    end
    else do
      -- Set the new lable for the write in radio button, enable it, and check
      -- it.  checkInGroup() ensures that the write in radio button is checked,
      -- and all the other buttons are unchecked.
      wiRB~setText(wiText)
      wiRB~enable
      .RadioButton~checkInGroup(self, IDC_RB_REGINA, IDC_RB_REXXIMC, wiRB~id)
    end
  end

  return 0

::method cancel unguarded

  self~picked = ""
  return self~cancel:super

::method ok unguarded

  -- We purposively edited the symbolic resource IDs so that they would be
  -- sequential and in order from IDC_RB_REGINA to IDC_RB_REXXIMC

  idFirst = .constDir[IDC_RB_REGINA]
  idLast  = .constDir[IDC_RB_REXXIMC]

  do i = idFirst to idLast
    rb = self~newRadioButton(i)
    if rb~checked then do
      self~picked = rb~getText
      leave
    end
  end

  return self~ok:super
