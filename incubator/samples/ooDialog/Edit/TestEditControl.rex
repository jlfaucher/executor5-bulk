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

/* Sample Dialog useing some newish features of the EditControl in ooDialog. */

-- NOTE: This is a work in progress, it will be updated to make it a better
-- example for people new to ooDialog.  Still it may be of benefit to anyone
-- using ooDialog.
--
-- NOTE: This requires ooRexx 3.2.0 svn revision 573 or later to run.


-- NOTE: The TestEditControl dialog was created with ResEdit.  ResEdit REQUIRES
-- that the header file for the symbolic IDs be named "resource.h"  If you want
-- to use ResEdit to make changes to this dialog, you will need to:
--
--   Rename the file, TestEditControl.h, to resource.h
--
--   Change the reference in TestEditControl.rc from TestEditControl.h to
--   resource.h
--
--   Change the reference to TestEditControl.h in TestEditControl.rex to
--   resource.h


  .local~cueExplanation = "Unfortunately, you can not set a" || .endOfLine || -
                          "cue banner on a multiline edit"   || .endOfLine || -
                          "control."

  .local~upperCue  = " type here, all letters will be upper cased"
  .local~lowerCue  = " type here, all letters will be lower cased"
  .local~numberCue = " type here, only numbers will be allowed"
  .local~noneCue   = " type here, what you type is what you get"

  dlg = .ExtendedEdit~new( TestEditControl.dll, IDD_DIALOG1, ,                -
                           "TestEditControl.h" )
  if dlg~initCode == 0 then do
    dlg~Execute( "SHOWTOP", IDI_DLG_OOREXX )
    dlg~Deinstall
  end

-- End of entry point.

::requires "OODWin32.cls"

::class ExtendedEdit subclass ResDialog -
                     inherit AdvancedControls MessageExtensions

::method init
  forward class (super) continue
  if result <> 0 then return result
  self~noAutoDetection
  return 0

::method initDialog
  expose elQ staticTextQ dlgControls sids

  numeric digits 11
  sids = .table~new
  sids["ES_UPPERCASE" ] = "0x0008"
  sids["ES_LOWERCASE" ] = "0x0010"
  sids["ES_NUMBER"    ] = "0x2000"
  sids["ES_WANTRETURN"] = "0x1000"
  sids["ES_OEMCONVERT"] = "0x0400"
  sids["WS_DISABLED"  ] = "0x08000000"
  sids["WS_VISIBLE"   ] = "0x10000000"
  sids["WS_TABSTOP"   ] = "0x00010000"
  sids["WS_GROUP"     ] = "0x00020000"

  el1 = self~getEditControl( IDC_EDIT_FIRST )
  el2 = self~getEditControl( IDC_EDIT_SECOND )
  el3 = self~getEditControl( IDC_EDIT_THIRD )
  elQ = .queue~of( el1, el2, el3 )

  el1Static = self~getStaticControl( IDC_ST_STYLE_EL1 )
  el2Static = self~getStaticControl( IDC_ST_STYLE_EL2 )
  el3Static = self~getStaticControl( IDC_ST_STYLE_EL3 )
  staticTextQ = .queue~of( el1Static, el2Static, el3Static )

  el1~addStyle( "NUMBER")
  el2~addStyle( "UPPER WANTRETURN")
  el3~addStyle( "LOWER")
  el1~setCue( .numberCue )
  el3~setCue( .lowerCue )

  el1Static~setTitle( "Numbers")
  el2Static~setTitle( "Upper Case")
  el3Static~setTitle( "Lower Case")

  self~connectButton( IDC_PB_SETTABSTOP, setTabStops )
  self~connectButton( IDC_PB_STYLES, setStyles )
  self~connectButton( IDC_PB_TIP, showTip )
  self~connectButton( IDC_PB_SETCUE, setCue )

  self~connectButtonNotify( IDC_CHECK_DLG, "CLICKED", setTabstopSetType )
  self~connectButtonNotify( IDC_CHECK_CTRL, "CLICKED", setTabstopSetType )

  dlgControls = .DlgControls~new(self)

  dlgControls~get( IDC_RB_ALL )~check
  dlgControls~get( IDC_CHECK_DLG )~check

  dlgControls~get( IDC_CHECK_NO_EDITLINES )~group
  dlgControls~get( IDC_EDIT_FIRST )~group

  combo = dlgControls~get( IDC_COMBO_APPLYTO )
  combo~add( "EditLine 1 (EL-1)" )
  combo~add( "EditLine 2 (EL-2)" )
  combo~add( "EditLine 3 (EL-3)" )
  combo~selectIndex( 1 )
  ret = self~connectComboBoxNotify( IDC_COMBO_APPLYTO, "SELENDOK", onComboChange )

  dlgControls~tabStops( 'A' )
  self~onComboChange

::method getEditStyle private
  expose sids dlgControls
  use arg editControl

  numeric digits 11

  styleString = ""
  styleNumeric = editControl~getStyleRaw

  if BinaryAnd( styleNumeric, sids["ES_UPPERCASE"] ) <> 0 then
    styleString = styleString "UPPER"

  if BinaryAnd( styleNumeric, sids["ES_LOWERCASE"] ) <> 0 then
    styleString = styleString "LOWER"

  if BinaryAnd( styleNumeric, sids["ES_NUMBER"] ) <> 0 then
    styleString = styleString "NUMBER"

  if BinaryAnd( styleNumeric, sids["ES_WANTRETURN"] ) <> 0 then
    styleString = styleString "WANTRETURN"

  if BinaryAnd( styleNumeric, sids["ES_OEMCONVERT"] ) <> 0 then
    styleString = styleString "OEM"

  if BinaryAnd( styleNumeric, sids["WS_DISABLED"] ) <> 0 then
    styleString = styleString "DISABLED"

  if BinaryAnd( styleNumeric, sids["WS_VISIBLE"] ) <> 0 then
    styleString = styleString "VISIBLE"

  if BinaryAnd( styleNumeric, sids["WS_TABSTOP"] ) <> 0 then
    styleString = styleString "TABSTOP"

  if BinaryAnd( styleNumeric, sids["WS_GROUP"] ) <> 0 then
    styleString = styleString "GROUP"

return styleString~strip( "L" )

::method onComboChange
  expose elQ sids dlgControls

  numeric digits 11

  el = elQ~at( dlgControls~get( IDC_COMBO_APPLYTO )~selectedIndex )
  style = self~getEditStyle( el )

  select
    when style~pos( "UPPER" ) <> 0 then do
      dlgControls~get( IDC_RB_UPPER )~check
      dlgControls~get( IDC_RB_LOWER )~uncheck
      dlgControls~get( IDC_RB_NUMBER )~uncheck
      dlgControls~get( IDC_RB_NONE )~uncheck
    end
    when style~pos( "LOWER" ) <> 0 then do
      dlgControls~get( IDC_RB_LOWER )~check
      dlgControls~get( IDC_RB_UPPER )~uncheck
      dlgControls~get( IDC_RB_NUMBER )~uncheck
      dlgControls~get( IDC_RB_NONE )~uncheck
    end
    when style~pos( "NUMBER" ) <> 0 then do
      dlgControls~get( IDC_RB_NUMBER )~check
      dlgControls~get( IDC_RB_LOWER )~uncheck
      dlgControls~get( IDC_RB_UPPER )~uncheck
      dlgControls~get( IDC_RB_NONE )~uncheck
    end
    otherwise do
      dlgControls~get( IDC_RB_NONE )~check
      dlgControls~get( IDC_RB_UPPER )~uncheck
      dlgControls~get( IDC_RB_LOWER )~uncheck
      dlgControls~get( IDC_RB_NUMBER )~uncheck
    end
  end
  -- End select

  if style~pos( "WANTRETURN" ) <> 0 then
    dlgControls~get( IDC_CHECK_RETURN )~check
  else
    dlgControls~get( IDC_CHECK_RETURN )~unCheck

  if style~pos( "OEM" ) <> 0 then
    dlgControls~get( IDC_CHECK_OEM )~check
  else
    dlgControls~get( IDC_CHECK_OEM )~unCheck

  if style~pos( "DISABLED" ) <> 0 then
    dlgControls~get( IDC_CHECK_ENABLED )~unCheck
  else
    dlgControls~get( IDC_CHECK_ENABLED )~check

  if style~pos( "VISIBLE" ) <> 0 then
    dlgControls~get( IDC_CHECK_WSVISIBLE )~check
  else
    dlgControls~get( IDC_CHECK_WSVISIBLE )~unCheck

  if style~pos( "TABSTOP" ) <> 0 then
    dlgControls~get( IDC_CHECK_TABSTOP )~check
  else
    dlgControls~get( IDC_CHECK_TABSTOP )~unCheck

  if style~pos( "GROUP" ) <> 0 then
    dlgControls~get( IDC_CHECK_GROUP )~check
  else
    dlgControls~get( IDC_CHECK_GROUP )~unCheck


::method setTabStopSetType
  expose checkDlg checkCtrl dlgControls
  use arg controlID

  if checkDlg~class \== .CheckBox then do
    checkDlg  = dlgControls~get( IDC_CHECK_DLG )
    checkCtrl = dlgControls~get( IDC_CHECK_CTRL )
  end

  id = BinaryAnd( controlID, '0x0000FFFF' )
  if id == self~constDir[IDC_CHECK_DLG] then checkCtrl~unCheck
  else checkDlg~unCheck

::method setTabStops
  expose dlgControls

  select
    when dlgControls~get( IDC_RB_ALL )~isChecked == "CHECKED" then
      dlgControls~tabStops( 'A' )
    when dlgControls~get( IDC_RB_NOTABSTOPS )~isChecked == "CHECKED" then
      dlgControls~tabStops( 'N' )
    when dlgControls~get( IDC_RB_EVEN )~isChecked == "CHECKED" then
      dlgControls~tabStops( 'E' )
    when dlgControls~get( IDC_RB_ODD )~isChecked == "CHECKED" then
      dlgControls~tabStops( 'O' )
    otherwise
      nop  -- Should never happen.  Could indicate an error somehow.
  end
  -- End select

  self~focusItem( IDC_RB_ALL )

::method setStyles
  expose elQ staticTextQ sids dlgControls

  index = dlgControls~get( IDC_COMBO_APPLYTO )~selectedIndex
  el = elQ~at( index )
  removeStyle = ""
  addStyle = ""

  if dlgControls~get( IDC_CHECK_TABSTOP )~isChecked == "CHECKED" then
    addStyle = "TABSTOP"
  else
    removeStyle = "TABSTOP"

  if dlgControls~get( IDC_CHECK_GROUP )~isChecked == "CHECKED" then
    addStyle = addStyle "GROUP"
  else
    removeStyle = removeStyle "GROUP"

  if dlgControls~get( IDC_CHECK_OEM )~isChecked == "CHECKED" then
    addStyle = addStyle "OEM"
  else
    removeStyle = removeStyle "OEM"

  if dlgControls~get( IDC_CHECK_RETURN )~isChecked == "CHECKED" then
    addStyle = addStyle "WANTRETURN"
  else
    removeStyle = removeStyle "WANTRETURN"

  select
    when dlgControls~get( IDC_RB_UPPER )~isChecked == "CHECKED" then do
      addStyle = addStyle "UPPER"
      removeStyle = removeStyle "LOWER NUMBER"
      staticText = "Upper Case"
    end
    when dlgControls~get( IDC_RB_LOWER )~isChecked == "CHECKED" then do
      addStyle = addStyle "LOWER"
      removeStyle = removeStyle "UPPER NUMBER"
      staticText = "Lower Case"
    end
    when dlgControls~get( IDC_RB_NUMBER )~isChecked == "CHECKED" then do
      addStyle = addStyle "NUMBER"
      removeStyle = removeStyle "UPPER LOWER"
      staticText = "Numbers"
    end
    otherwise do
      removeStyle = removeStyle "UPPER LOWER NUMBER"
      staticText = "Any"
    end
  end
  -- End select

  if dlgControls~get( IDC_CHECK_WSVISIBLE )~isChecked == "CHECKED" then
    hide = .false
  else
    hide = .true

  if dlgControls~get( IDC_CHECK_ENABLED )~isChecked == "CHECKED" then
    enable = .true
  else
    enable = .false

  el~replaceStyle( removeStyle~strip( 'L' ), addStyle~strip( 'L' ))
  staticTextQ~at( index )~setTitle( staticText )
  if enable then el~enable; else el~disable
  if hide then el~hide; else el~display( "NORMAL" )


::method setCue
  expose elQ

  do el over elQ
    style = self~getEditStyle( el )
    select
      when style~pos( "UPPER" ) <> 0 then cue = .upperCue
      when style~pos( "LOWER" ) <> 0 then cue = .lowerCue
      when style~pos( "NUMBER" ) <> 0 then cue = .numberCue
      otherwise cue = .noneCue
    end
    -- End select

    el~setTitle( "" )
    ret = el~setCue( cue )
    if ret <> 0 then el~setTitle( .cueExplanation )
  end


::method showTip
  expose elQ

  el = elQ~pull
  el~showBalloon( "Using the Test Edit Control Sample Dialog",            -
                  "Use the Edit Styles radio buttons"    || .endOfLine || -
                  "and check boxes to choose the"        || .endOfLine || -
                  "desired styles.  Then push the"       || .endOfLine || -
                  "'Set Styles' button." )

  elQ~queue( el )

-- A class to keep track of all the controls in the dialg.  The class can handle
-- thinks like setting / unsetting the tab stops, querying the state of the
-- check boxes, etc..
::class 'DlgControls'

::method controls attribute

::method init
  expose parent
  use arg parent

  tmpA = .array~new
  tmpA[ 1] = IDC_RB_ALL
  tmpA[ 2] = IDC_RB_EVEN
  tmpA[ 3] = IDC_RB_ODD
  tmpA[ 4] = IDC_RB_NOTABSTOPS
  tmpA[ 5] = IDC_CHECK_NO_EDITLINES
  tmpA[ 6] = IDC_CHECK_NOBUTTONS
  tmpA[ 7] = IDC_CHECK_DLG
  tmpA[ 8] = IDC_CHECK_CTRL
  tmpA[ 9] = IDC_COMBO_APPLYTO
  tmpA[10] = IDC_RB_NUMBER
  tmpA[11] = IDC_RB_LOWER
  tmpA[12] = IDC_RB_UPPER
  tmpA[13] = IDC_RB_NONE
  tmpA[14] = IDC_CHECK_RETURN
  tmpA[15] = IDC_CHECK_WSVISIBLE
  tmpA[16] = IDC_CHECK_OEM
  tmpA[17] = IDC_CHECK_ENABLED
  tmpA[18] = IDC_CHECK_TABSTOP
  tmpA[19] = IDC_CHECK_GROUP
  tmpA[20] = IDC_EDIT_FIRST
  tmpA[21] = IDC_EDIT_SECOND
  tmpA[22] = IDC_EDIT_THIRD
  tmpA[23] = IDC_PB_SETTABSTOP
  tmpA[24] = IDC_PB_STYLES
  tmpA[25] = IDC_PB_TIP
  tmpA[26] = IDC_PB_SETCUE
  tmpA[27] = IDCANCEL
  tmpA[28] = IDOK

  self~controls = .table~new
  i = 1
  do id over tmpA
    c = .Cntrl~new(id)
    select
      when c~button then c~obj = parent~getButtonControl(id)
      when c~check then c~obj = parent~getCheckControl(id)
      when c~radio then c~obj = parent~getRadioControl(id)
      when c~combo then c~obj = parent~getComboBox(id)
      when c~edit then c~obj = parent~getEditControl(id)
      otherwise nop
    end
    -- End select

    if (i // 2) == 0 then c~even = .true; else c~odd = .true
    i += 1
    self~controls[id] = c
  end


::method get
  use arg id
return self~controls[id]~obj

::method tabStops
  expose parent
  use arg type

  if self~controls[IDC_CHECK_DLG]~obj~isChecked == "CHECKED" then
    useParent = .true
  else
    useParent = .false

  select
    when type == 'A' then do id over self~controls
      self~setTabStopStyle( id, useParent, self~setOrUnset( id ) )
    end
    -- End when type == 'A'

    when type == 'E' then do id over self~controls
      if self~controls[id]~odd then
        self~setTabStopStyle( id, useParent, .false )
      else
        self~setTabStopStyle( id, useParent, self~setOrUnset( id ) )
    end

    when type == 'O' then do id over self~controls
      if self~controls[id]~even then
        self~setTabStopStyle( id, useParent, .false )
      else
        self~setTabStopStyle( id, useParent, self~setOrUnset( id ) )
    end

    when type == 'N' then do id over self~controls
      self~setTabStopStyle( id, useParent, .false )
    end

    otherwise nop -- Should not happen, could indicate an internal error.
  end
  -- End select


::method setOrUnset private
  use arg id

  if self~controls[IDC_CHECK_NO_EDITLINES]~obj~isChecked == "CHECKED" then
    noEL = .true
  else
    noEL = .false

  if self~controls[IDC_CHECK_NOBUTTONS]~obj~isChecked == "CHECKED" then
    noButtons = .true
  else
    noButtons = .false

  select
    when noEl & self~controls[id]~edit then return .false
    when noButtons & self~controls[id]~button then return .false
    otherwise return .true
  end
  -- End select


::method setTabStopStyle private
  expose parent
  use arg id, useParent, set

  if useParent then parent~setTabStop( id, set )
  else self~controls[id]~obj~tabStop( set )

::class 'Cntrl'

::method button attribute
::method check attribute
::method radio attribute
::method edit attribute
::method combo attribute
::method odd attribute
::method even attribute
::method id attribute
::method obj attribute

::method init
  use arg id

  self~button = .false
  self~check = .false
  self~radio = .false
  self~combo = .false
  self~edit = .false
  self~odd = .false
  self~even = .false
  self~id = id
  self~obj = .nil
  select
    when id~pos( "_PB_" ) <> 0 then self~button = .true
    when id~pos( "_RB_" ) <> 0 then self~radio = .true
    when id~pos( "_CHECK_" ) <> 0 then self~check = .true
    when id~pos( "_COMBO_" ) <> 0 then self~combo = .true
    when id~pos( "_EDIT_" ) <> 0 then self~edit = .true
    otherwise self~button = .true
  end
  -- End select

