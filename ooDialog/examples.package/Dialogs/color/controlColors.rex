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
 * A relatively simple dialog that allows the user to change the background
 * color of the dialog and the color of the control text.
 *
 * The focus of this example is these methods:
 *
 *   backgroundColor()
 *   backgroundSysColor()
 *   setControlColor()
 *   controlColor()    -- Not shown, but works similarly to setControlColor()
 *
 * Unlike some of the examples in this package, this example does not have a lot
 * of comment.  Please read the ooDialog reference manual for any details that
 * are unclear.
 *
 * One item to note though is this:  When the color is changed for the dialog
 * controls, all the controls have their color set.  But, for buttons, the text
 * color does not change.  And for the combo box, the text color of the selected
 * item does not change.  This behaviour is dictated by Windows, there is
 * nothing that ooDialog can do about that.
 *
 * This example requires ooDialog 4.2.1 or later to execute.
 */

  if .DlgUtil~version('L') < '4.2.1' then return abort()

  .application~setDefaults('O', 'controlColors.h', .false)

  dlg = .ColorExplorer~new("controlColors.rc", IDD_COLOR_EXPLORER)
  dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "ooDialog.cls"

::class 'ColorExplorer' subclass RcDialog

-- Constants for some of the palette index colors.
::constant DARKBLUE       4
::constant LIGHTBLUE      9
::constant BLUE          16
::constant DARKYELLOW     3
::constant YELLOW        15

-- Constants to signal which color the controls should be set to.
::constant CTL_BLACK             1
::constant CTL_AWESOMERED        2
::constant CTL_STRIKINGYELLOW    3
::constant CTL_GREATGREEN        4
::constant CTL_DARKYELLOW        5
::constant CTL_YELLOW            6

::method init
  expose stateArray clrCoolBlue clrPalePurple clrAwesomeRed clrStrikingYellow clrGreatGreen clrBlack

  forward class (super) continue

  self~initControlArray

  clrBlack          = .Image~colorRef(0, 0, 0)
  clrCoolBlue       = .Image~colorRef(171, 214, 245)
  clrPalePurple     = .Image~colorRef(245, 188, 241)
  clrAwesomeRed     = .Image~colorRef(161, 6, 17)
  clrStrikingYellow = .Image~colorRef(215, 247, 5)
  clrGreatGreen     = .Image~colorRef(5, 161, 10)

  self~connectButtonEvent(IDC_PB_REFRESH, 'CLICKED', onRefresh)

  stateArray = .array~of('California', 'Washington', 'Oregon', 'Nevada', 'Arizona')


::method initDialog
  expose rbDlgNormal rbCtlBlack cbStates stateArray

  self~setDlgRbObjs
  self~setCtlRbObjs

  cbStates = self~newComboBox(IDC_CB_STATES)
  do state over stateArray
    cbStates~add(state)
  end
  cbStates~selectIndex(1)

  rbDlgNormal~check
  rbCtlBlack~check


::method onRefresh unguarded

  self~setDlgColor
  self~setCtlColor

  self~redrawClient(.true)

  return 0


::method setDlgColor private unguarded
  expose rbDlgNormal rbDlgCoolBlue rbDlgPalePurple rbDlgDarkBlue rbDlgLightBlue rbDlgBlue clrCoolBlue clrPalePurple

  select
    when rbDlgNormal~checked then     self~backgroundSysColor('3DFACE')
    when rbDlgCoolBlue~checked then   self~backgroundColor(clrCoolBlue, .true)
    when rbDlgPalePurple~checked then self~backgroundColor(clrPalePurple, .true)
    when rbDlgDarkBlue~checked then   self~backgroundColor(self~DARKBLUE)
    when rbDlgLightBlue~checked then  self~backgroundColor(self~LIGHTBLUE)
    when rbDlgBlue~checked then       self~backgroundColor(self~BLUE)
    otherwise nop
  end
  -- End select


::method setCtlColor private unguarded
  expose rbCtlBlack rbCtlAwesomeRed rbCtlStrikingYellow rbCtlGreatGreen rbCtlDarkYellow rbCtlYellow

  select
    when rbCtlBlack~checked then           self~setControlColors(self~CTL_BLACK)
    when rbCtlAwesomeRed~checked then      self~setControlColors(self~CTL_AWESOMERED)
    when rbCtlStrikingYellow~checked then  self~setControlColors(self~CTL_STRIKINGYELLOW)
    when rbCtlGreatGreen~checked then      self~setControlColors(self~CTL_GREATGREEN)
    when rbCtlDarkYellow~checked then      self~setControlColors(self~CTL_DARKYELLOW)
    when rbCtlYellow~checked then          self~setControlColors(self~CTL_YELLOW)
    otherwise nop
  end
  -- End select


::method setControlColors private
  expose ctls clrAwesomeRed clrStrikingYellow clrGreatGreen clrBlack
  use strict arg color

  select
    when color == self~CTL_DARKYELLOW then do
      do ctlID over ctls
        self~setControlColor(ctlID, , self~DARKYELLOW)
      end
    end

    when color == self~CTL_YELLOW then do
      do ctlID over ctls
        self~setControlColor(ctlID, , self~YELLOW)
      end
    end

    when color == self~CTL_BLACK then do
      do ctlID over ctls
        self~setControlColor(ctlID, , clrBlack, .true)
      end
    end
    when color == self~CTL_AWESOMERED then do
      do ctlID over ctls
        self~setControlColor(ctlID, , clrAwesomeRed, .true)
      end
    end

    when color == self~CTL_STRIKINGYELLOW then do
      do ctlID over ctls
        self~setControlColor(ctlID, , clrStrikingYellow, .true)
      end
    end

    when color == self~CTL_GREATGREEN then do
      do ctlID over ctls
        self~setControlColor(ctlID, , clrGreatGreen, .true)
      end
    end

    otherwise do
      nop
    end
  end
  -- End select


::method setDlgRbObjs private
  expose rbDlgNormal rbDlgCoolBlue rbDlgPalePurple rbDlgDarkBlue rbDlgLightBlue rbDlgBlue -

  rbDlgNormal     = self~newRadioButton(IDC_RB_DLG_NORMAL)
  rbDlgCoolBlue   = self~newRadioButton(IDC_RB_DLG_COOLBLUE)
  rbDlgPalePurple = self~newRadioButton(IDC_RB_DLG_PALEPURPLE)
  rbDlgDarkBlue   = self~newRadioButton(IDC_RB_DLG_DARKBLUE)
  rbDlgLightBlue  = self~newRadioButton(IDC_RB_DLG_LIGHTBLUE)
  rbDlgBlue       = self~newRadioButton(IDC_RB_DLG_BLUE)


::method setCtlRbObjs private
  expose rbCtlBlack rbCtlAwesomeRed rbCtlStrikingYellow rbCtlGreatGreen rbCtlDarkYellow rbCtlYellow

  rbCtlBlack          = self~newRadioButton(IDC_RB_CTL_BLACK)
  rbCtlAwesomeRed     = self~newRadioButton(IDC_RB_CTL_AWESOMERED)
  rbCtlStrikingYellow = self~newRadioButton(IDC_RB_CTL_STRIKINGYELLOW)
  rbCtlGreatGreen     = self~newRadioButton(IDC_RB_CTL_GREATGREEN)
  rbCtlDarkYellow     = self~newRadioButton(IDC_RB_CTL_DARKYELLOW)
  rbCtlYellow         = self~newRadioButton(IDC_RB_CTL_YELLOW)


::method initControlArray private
  expose ctls

  ctls = .array~new

  ctls[ 1] = IDC_ST_EDIT_LABEL
  ctls[ 2] = IDC_ST_TYPICAL
  ctls[ 3] = IDC_ST_CB_LABEL
  ctls[ 4] = IDC_EDIT
  ctls[ 5] = IDC_GB_DIALOG
  ctls[ 6] = IDC_RB_DLG_NORMAL
  ctls[ 7] = IDC_RB_DLG_COOLBLUE
  ctls[ 8] = IDC_RB_DLG_PALEPURPLE
  ctls[ 9] = IDC_RB_DLG_BLUE
  ctls[10] = IDC_RB_DLG_DARKBLUE
  ctls[11] = IDC_RB_DLG_LIGHTBLUE
  ctls[12] = IDC_PB_REFRESH
  ctls[13] = IDC_CB_STATES
  ctls[14] = IDC_GB_CONTROL
  ctls[15] = IDC_RB_CTL_BLACK
  ctls[16] = IDC_RB_CTL_AWESOMERED
  ctls[17] = IDC_RB_CTL_STRIKINGYELLOW
  ctls[18] = IDC_RB_CTL_GREATGREEN
  ctls[19] = IDC_RB_CTL_YELLOW
  ctls[20] = IDC_RB_CTL_DARKYELLOW
  ctls[21] = IDOK
  ctls[22] = IDCANCEL


::routine abort
  use strict arg

  msg = 'The controlColors.rex example requires'            || .endOfLine           || -
        'ooDialog 4.2.1 or later to execute.'               || .endOfLine           || -
        'Current ooDialog level is:' .DlgUtil~version('L')  || .endOfLine~copies(2) || -
        'The program will terminate.'

  title = 'ooDialog - Incompatible Version Error'
  icon  = 'WARNING'
  bttn  = 'OK'

  ret = MessageDialog(msg, 0, title, bttn, icon)
  return 99
