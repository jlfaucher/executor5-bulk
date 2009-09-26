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

/* Test auto detect creation of attributes and the CONNECT buttons feature
 * when using a RcDialog.  Test a UserDialog with the same buttons.
 */

  dlg = .SimpleButtonsRc~new("resources\autoDetectAutoConnect.rc", IDD_DIALOG1, , -
                             "resources\autoDetectAutoConnect.h",                 -
                             "CENTER CONNECTBUTTONS CONNECTRADIOS CONNECTCHECKS")

  say "Test one RcDialog:" dlg
  say 'AutoDetect?' dlg~autoDetect
  say 'Blank push buttons *only* should not generate a "was clicked" message'
  if dlg~initCode = 0 then do
    dlg~Execute("SHOWTOP")
    dlg~Deinstall
  end
  else do
    say "Problem creating the rc dialog.  Init code:" dlg~initCode
    return 99
  end

  say 'The following attributes should all be set (if you clicked ok):'
  say 'dlg~IDC_RB_ONE:     ' (dlg~IDC_RB_ONE)
  say 'dlg~RadioTwo:       ' (dlg~RadioTwo)
  say 'dlg~IDC_RB_THREE:   ' (dlg~IDC_RB_THREE)
  say 'dlg~DATA160:        ' (dlg~DATA160)
  say 'dlg~IDC_CHECK_ONE:  ' (dlg~IDC_CHECK_ONE)
  say 'dlg~CheckTwo:       ' (dlg~CheckTwo)
  say 'dlg~IDC_CHECK_THREE:' (dlg~IDC_CHECK_THREE)
  say 'dlg~DATA180:        ' (dlg~DATA180)

  dlg = .SimpleButtonsUser~new( , "resources\autoDetectAutoConnect.h")

  say; say
  say "Test two UserDialog:" dlg
  say 'AutoDetect?' dlg~autoDetect
  say 'No button click should generate a "was clicked" message'
  if dlg~initCode = 0 then do
    dlg~Execute("SHOWTOP")
    dlg~Deinstall
  end
  else do
    say "Problem creating the user dialog.  Init code:" dlg~initCode
    return 99
  end

  say 'The following attributes should all be set (if you clicked ok):'
  say 'dlg~RadioOne:       ' (dlg~RadioOne)
  say 'dlg~RadioTwo:       ' (dlg~RadioTwo)
  say 'dlg~DATA1011:       ' (dlg~DATA1011)
  say 'dlg~DATA160:        ' (dlg~DATA160)
  say 'dlg~CheckOne:       ' (dlg~CheckOne)
  say 'dlg~CheckTwo:       ' (dlg~CheckTwo)
  say 'dlg~DATA1012:       ' (dlg~DATA1012)
  say 'dlg~DATA180:        ' (dlg~DATA180)

  return 0
-- End of entry point.

::requires "OODWIN32.CLS"

::class 'SimpleButtonsRc' subclass RcDialog inherit AdvancedControls MessageExtensions

-- This method gets connected for check boxes and radio buttons that have no text
::method idRadioOne
  say 'Radio button one was clicked.'
::method idRadioTwo
  say 'Radio button two was clicked.'
::method idCheckOne
  say 'Check box one was clicked.'
::method idCheckTwo
  say 'Check box two was clicked.'

::method id
  say 'Button (radio button or check box) with resource id:' arg(1) 'was clicked'

::method pushOne
  say 'The push one button was clicked.'

::method pushTwo
  say 'The push two button was clicked.'


::class 'SimpleButtonsUser' subclass UserDialog inherit AdvancedControls MessageExtensions

::method init
  forward class (super) continue

  self~createCenter(223, 138, "UserDialog Dialog", "VISIBLE NOTMODAL", , "MS Shell Dlg 2", 8)

::method defineDialog

    self~createGroupBox(IDC_GB_BUTTONS,    7,   7, 180, 81, "", "Push All Buttons")
    self~createRadioButton(IDC_RB_ONE,    18,  20,    ,   ,   ,  "Radio One")
    self~createRadioButton(150,           18,  35,  56, 10,   , "RadioTwo")
    self~createRadioButton(IDC_RB_THREE,  18,  50,  60, 10,   , "")
    self~createRadioButton(160,           18,  65,  58, 10,   , "")
    self~createCheckBox(IDC_CHECK_ONE,   115,  17,  54, 10,   , "Check One")
    self~createCheckBox(170,             115,  34,  54, 10,   , "Check Two")
    self~createCheckBox(IDC_CHECK_THREE, 115,  53,  54, 10,   , "")
    self~createCheckBox(180,             115,  73,  54, 10,   , "")

    self~createPushButton(IDC_PB_PUSH,       12, 117,  50, 14,          , "Push One")
    self~createPushButton(190,               12,  94,  50, 14,          , "Push Two", , )
    self~createPushButton(IDC_PB_PUSH_FOUR, 144,  94,  50, 14,          , "",         , )
    self~createPushButton(200,                77,  94, 50, 14,          , "",         , )
    self~createPushButton(IDOK,              108, 114, 50, 14, "DEFAULT", "OK",       , )
    self~createPushButton(IDCANCEL,          163, 114, 50, 14,          , "Cancel",   , )
