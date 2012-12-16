/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2011 Rexx Language Association. All rights reserved.    */
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
 * This example shows how to use the NumberOnlyEditEx.cls file to extend the
 * edit control.  The extension allows the programmer to restrict the text input
 * in an edit control to decimal numbers (signed or not) only.
 *
 * The edit control itself has a style that restricts input to numbers only, but
 * that style does not allow decimal points (i.e. 436.54) or a sign (i.e. -76)
 * The extended number only function provided by NumberOnlyEditEx.cls does allow
 * signed decimal numbers.
 *
 * This example dialog allows the user to input starting and ending mileage,
 * gallons of gas used, and the price of a gallon of gas.  It then calculates
 * miles per gallon and cost per mile.
 */

  if .DlgUtil~comCtl32Version < 6.0 then do
    msg = 'This MPG calculator program uses features'      || .endOfLine ||           -
          'only avaliable on Windows XP and later,'        || .endOfLine ||           -
          'with Common Controls Library version 6.0'       || .endOfLine ||           -
          'loaded.'                                        || .endOfLine~copies(2) || -
          'The version of the Common Controls Library'     || .endOfLine           || -
          'currently loaded is: ' .DlgUtil~comCtl32Version || .endOfLine~copies(2) || -
          'The program will have to terminate.'

    ret = MessageDialog(msg, 0, "Common Controls Library Version Error", , "WARNING", "TOPMOST")
    return 99
  end

  .application~useGlobalConstDir("O", 'CalcMPG.h')

  dlg = .MPGDlg~new('CalcMPG.dll', IDD_MPG_DLG)
  if dlg~initCode <> 0 then do
    say "Error initializing dialog.  Aborting"
    return 99
  end

  dlg~execute("SHOWTOP")
  return 0

-- Must require the number only file.
::requires 'ooDialog.cls'
::requires 'NumberOnlyEditEx.cls'


::class 'MPGDlg' subclass ResDialog

-- For each edit control that we want to restrict input to, we need to call the
-- initDecimalOnly() method.  If the method is not called, there is no change to
-- the behaviour of the edit control.
--
-- initDecimalOnly() takes 2 arguments.
--
-- The first argument is the number of decimal places to allow.  The argument is
-- optional with the default being 2.  The decimal places can be set to 0 if
-- desired.
--
-- The second argument is whether the sign character is allowed or not.  The
-- default is false.
--
-- The second requirement is that connectCharEvent() must be invoked for the
-- edit control.  The same event handling method can be used for each connection
-- and the easiest way to do this is to name the event handler method 'onChar'
-- However, the programmer can choose to code this however she may want.  The
-- requirement is that the chacter event message and its arguments be forwarded
-- on to the 'onChar' method of the edit control.
::method initDialog
  expose editStartMiles editEndMiles editGas editCost stMPG stPM resultFont

  -- We want 1 decimal place for the starting and ending mileage.
  editStartMiles = self~newEdit(IDC_EDIT_START)
  editStartMiles~initDecimalOnly(1)
  editStartMiles~connectCharEvent(onChar)

  editEndMiles = self~newEdit(IDC_EDIT_END)
  editEndMiles~initDecimalOnly(1)
  editEndMiles~connectCharEvent(onChar)

  -- We want  decimal places for the gallons of gas, and also the price.
  editGas = self~newEdit(IDC_EDIT_GAS)
  editGas~initDecimalOnly(3)
  editGas~connectCharEvent(onChar)

  editCost = self~newEdit(IDC_EDIT_COST)
  editCost~initDecimalOnly(3)
  editCost~connectCharEvent(onChar)

  stMPG = self~newStatic(IDC_ST_MPG)
  stPM  = self~newStatic(IDC_ST_PERMILE)

  resultFont = self~createFontEx("Verdana", 20)

  stMPG~setFont(resultFont)
  stPM~setFont(resultFont)

  self~connectButtonEvent(IDC_PB_CALCULATE, "CLICKED", onCalc)


/** onChar()
 * This is the event handler for the character event.  The args to the handler
 * are:
 *
 *   use arg char, isShift, isCtrl, isAlt, misc, control
 *
 * where 'control' is the dialog control (edit control) that had the focus, and
 * had the character event.  The character event and its arguments *must* be
 * forwarded to the edit control's 'onChar' method.  Naming the event handler
 * onChar in the Rexx dialog object, simply makes forwarding simplier.  We just
 * forward to the object passed as the 6th arg and the NumberOnlyEditEx
 * extension control takes care of all the details.
 */
::method onChar unguarded
  forward to (arg(6))


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  The rest of the methods for this dialog are standard methods used to implement
  the functionaliyt of the dialog.  In this case calculating MPG from the user
  input.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

::method leaving
  expose resultFont
  self~deleteFont(resultFont)


::method onCalc unguarded
  expose editStartMiles editEndMiles editGas editCost stMPG stPM

  sMiles = editStartMiles~getText
  eMiles = editEndMiles~getText
  gas    = editGas~getText
  cost   = editCost~getText

  msg = "To calculate MPG, all fields must contain data."

  if sMiles == "" then return self~entryFieldErr(editStartMiles, msg)
  if eMiles == "" then return self~entryFieldErr(editEndMiles, msg)
  if gas    == "" then return self~entryFieldErr(editGas, msg)
  if cost   == "" then return self~entryFieldErr(editCost, msg)

  msg = "The ending milage must be greater than the starting milage."
  if sMiles >= eMiles then return self~entryFieldErr(editEndMiles, msg)

  msg = "The gasoline used can not be 0."
  if gas == 0 then return self~entryFieldErr(editGas, msg)

  msg = "The cost of the gasoline can not be 0."
  if cost = 0 then return self~entryFieldErr(editCost, msg)

  miles = eMiles - sMiles
  mpg = miles / gas
  cpm = (gas * cost) / miles

  mpg = mpg~format( , 3) || "mpg"
  cpm = '$' || cpm~format( , 2) || "/mile"

  stMPG~setText(mpg)
  stPM~setText(cpm)

::method entryFieldErr private
  use strict arg edit, msg

  title = "Data Entry Error"
  ret = MessageDialog(msg, self~dlgHandle, title, "WARNING")
  edit~assignFocus
  return 0

