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
  File:    simpleRC.rex                             Author:      Mark Miesfeld
                                                    Creation date:  07/14/2007
  Company: N/A
  Project: ooDialog Examples                        Last update:    07/14/2007

  Example ooDialog program that uses a dialog defined in a resource script.
  This is a simplified version of ResourceEditors.rex and represents a more
  typical approach to dialogs.  The dialog is defined in a knoww file and much
  of the complexity of ResourceEditors.rex is removed.

  See the file: README.first for detailed information concerning this sample
  package.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  -- Start in the parent directory so we don't have to worry about relative
  -- file names.
  discard = directory( "..\" )

  -- This program uses symbolic IDs for the resource IDs.  These are defined in
  -- resource.h.  Passing the name of this file when the dialog object is
  -- instantiated is what allows this to work.
  dlg = .RcDlg~new( "ResEdit.rc", IDD_DIALOG1, , "resource.h" )

  -- Check that the dialog initialized okay.
  if dlg~initCode <> 0 then do
    say "Error starting dialog.  initCode:" dlg~initCode
    return dlg~initCode
  end

  -- Run the dialog.
  dlg~execute( "SHOWTOP", IDI_DLG_ALOHA )

  -- Uninstall the ooDialog C functions.
  dlg~deInstall

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "oodwin32.cls"

::class RcDlg subclass RcDialog inherit AdvancedControls MessageExtensions

-- The initDialog() method is a method of the PlainBaseDialog.
--
-- It is called by the PlainBaseDialog, and all subclasses, after the underlying
-- Windows dialog has been created successfully, but before the Windows dialog
-- is painted on the screen.  This makes it the proper place for all
-- initialization that requires the underlying dialog control to have been
-- created by the OS.
--
-- Things like setting the focus to a dialog control, inserting items into a
-- list-view or combobox control, all require that the underlying object has
-- already been created.  The connect mehods *could* be done in the init method,
-- but that requires an unnecessary over-ride of the init method.  I prefer to
-- put all the initialization in one place, when possible.
::method initDialog
  expose picked

  -- These connect methods map a message sent by the underlying control to a
  -- method in this class.  The first argument, the resource ID, is used to
  -- select the underlying control, the second argument, CLICKED in this case,
  -- says which type of message, and the third argument names the method to be
  -- called when / if the control sends the message.
  --
  -- In this case, we connect all the radio buttons to the same method.
  self~connectButtonNotify( IDC_RB_REGINALD, "CLICKED", rbClicked )
  self~connectButtonNotify( IDC_RB_REGINA, "CLICKED", rbClicked )
  self~connectButtonNotify( IDC_RB_OOREXX, "CLICKED", rbClicked )

  -- picked is an internal (exposed) variable used to track which radio button
  -- is checked.  When the dialog is first made visible, none of the radio
  -- buttons are checked.
  picked = "None"

  -- Here we set a 'cue' in the edit control.  This gives the user a hint as to
  -- what the expected input into the edit control is.  This feature is only
  -- available on Windows XP and later OSes.  If the feature is not supported
  -- on the OS this program is running on, the return code will be -4.  In a
  -- more sophisticated program, we could check the return and maybe take some
  -- appropriate action.  Here it is just ignored.
  --
  -- Technically we should check that we can actually get the edit control
  -- object.  If getEditControl returns .nil, this will blow up.
  ret = self~getEditControl( IDC_EDIT_NAME )~setCue( " Type your first and last name" )

return 0


-- The rbClicked() method is called when ever one of the radio buttons is
-- checked.  This can happen both when the user clicks the button with the
-- mouse, or when the user presses the space bar when a radio button has the
-- focus.  (So the method name is not totally accurate.)
--
-- The method is called because the connectButtonNotify calls in initDialog
-- connected this method to the radio buttons.
::method rbClicked
  expose picked
  use arg ctrlID, hwnd

  -- The first argument passed to this method contains the resource ID of the
  -- dialog control that generated the message in the 2 least significant bytes.
  -- We use a binary and to pick off those bytes.  (In reality, the 2 most
  -- significant bytes are going to be 0.)
  --
  -- The dialog control ID is then used to determine which radio button was
  -- checked.  Because the style of the radio buttons AUTORADIOBUTTON, the OS
  -- manages the check state of the 3 buttons.  When the user checks one button,
  -- the OS automatically unchecks the other buttons.  Because of this, only
  -- 1 radio button can be checked at a time.  We use this fact to make things
  -- easy, if a button is checked, then none of the other buttons can be
  -- checked.
  --
  -- The resource ID is returned by the Windows API, so it is going to be a
  -- numeric ID.  We use the ConstDir to match the numeric ID with one of our
  -- known symbolic IDs.
  rbID = binaryAnd( ctrlID, "0x0000FFFF" )

  select
    when rbID == self~constDir[IDC_RB_REGINALD] then picked = "Reginald"
    when rbID == self~constDir[IDC_RB_REGINA] then picked = "Regina"
    when rbID == self~constDir[IDC_RB_OOREXX] then picked = "ooRexx"
    otherwise picked = "None"  -- Should never happen.
  end
  -- End select


-- The validate() method is a method of the PlainBaseDialog.  That makes it a
-- method of all ooDialog dialogs, because all ooDialog dialogs are subclasses
-- of PlainBaseDialog
--
-- Validate is used to control whether the dialog should close, or not, when the
-- user clicks the OK button.
--
-- If this method returns 1 then the dialog will close, if 0 is returned the
-- dialog will not close.  If not over-ridden, the method always returns 1.
--
-- Note that validate is only called when the user pushes the OK button.  If the
-- user cancels the dialog, validate is not called.  If the user pushes the
-- cancel button, hits the esc key, or closes the dialog using the system
-- menu - these actions are all considered to be the user canceling.
--
-- Here, validate is over-ridden to check that the user "correctly" filled out
-- the dialog.
::method validate
  expose picked

  -- Get the text in the edit control and use a simplistic algorithm to
  -- validate: remove leading and trailing spaces and separate each word by 1
  -- space, make uppercase.  Then check that the text is 2 words exactly,
  -- pretending that this is a valid check for first and last name.
  --
  -- Next, have a little fun.  Check that Rick, Jeff, or Mark H are not trying
  -- to stuff the ballot box.
  name = self~getEntryLine( IDC_EDIT_NAME )~space( 1 )~translate
  eol = .endOfLine
  tab = "09"x

  msg = ""
  tail = "." || eol  ||  "No fair stuffing the ballot box."

  select
    when name~words <> 2 then
      msg = "You must enter both your first and last names,"  -
            || eol || "and only those two names."

    when name == "JEFF GLATT" & picked == "Reginald" then
      msg = "Jeff you have already voted for" picked || tail

    when name == "MARK HESSLING" & picked == "Regina" then
      msg = "Mark you have already voted for" picked || tail

    when name == "RICK MCGUIRE" & picked == "ooRexx" then
      msg = "Rick you have already voted for" picked || tail

    otherwise nop
  end
  -- End select

  -- If msg is not the empty string, then the data validation failed.  Put up
  -- a message box telling the user what is wrong.  By returning 0, we prevent
  -- the dialog from closing.
  if msg <> "" then do
    ret = InfoMessage( msg )
    return 0
  end

  -- Validation passed.  Put up a message box with the current ballot 'results.'
  countReginald = time( "H" )
  countRegina   = time( "M" )
  countOORexx   = time( "S" )
  select
    when picked = "Reginald" then countReginald += 1
    when picked = "Regina" then countRegina += 1
    when picked = "ooRexx" then countOORexx += 1
  end

  msg = "Interpreter Usage Count for" date( ) || eol || eol || -
        "Reginald:" || tab || tab || countReginald   || eol || -
        "Regina:"   || tab || tab || countRegina     || eol || -
        "ooRexx:"   || tab || tab || countOORexx

  ret = InfoMessage( msg )
return 1

/* - - - End Of File: simpleRC.rex- - - - - - - - - - - - - - - - - - - - - - */
