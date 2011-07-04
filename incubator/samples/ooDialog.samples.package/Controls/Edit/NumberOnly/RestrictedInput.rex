/**
 * Shows how prevent the user from entering invalid
 * characters in an edit control.  In this case the
 * user is resctricted to entering a decimal number
 * with no more than 2 decimal places.
 */

  -- Temp debugging
  .local~debug = .true

  resourceDLL = "restrictedInput32.dll"

  if .OS~isWOW64 then nop
  else if .OS~is64bit then resourceDLL = "restrictedInput64.dll"

  dlg = .MPGDlg~new(resourceDLL, IDD_MPG_DLG, , "restrictedInput.h")
  if dlg~initCode <> 0 then do
    say "Error initializing dialog.  Aborting"
    return 99
  end

  dlg~execute("SHOWTOP")
  return 0

::requires 'ooDialog.cls'
::requires 'NumberOnlyEditEx.cls'
::requires 'winsystm.cls'


::class 'MPGDlg' subclass ResDialog

::method initDialog
  expose editStartMiles editEndMiles editGas editCost stMPG stPM resultFont

  editStartMiles = self~newEdit(IDC_EDIT_START)
  editStartMiles~initDecimalOnly(0, .true)  -- temp to allow sign, no deicmals, should be 1
  editStartMiles~connectKeyEvent(onChar)

  editEndMiles = self~newEdit(IDC_EDIT_END)
  editEndMiles~initDecimalOnly(1, .true)    -- temp to allow sign, should be 1
  editEndMiles~connectKeyEvent(onChar)

  editGas = self~newEdit(IDC_EDIT_GAS)
  editGas~initDecimalOnly(2, .true)         -- temp to allow sign, should be 3
  editGas~connectKeyEvent(onChar)

  editCost = self~newEdit(IDC_EDIT_COST)
  editCost~initDecimalOnly(3)               -- temp to allow sign, should be 2
  editCost~connectKeyEvent(onChar)

  stMPG = self~newStatic(IDC_ST_MPG)
  stPM  = self~newStatic(IDC_ST_PERMILE)

  resultFont = self~createFontEx("Verdana", 20)

  stMPG~setFont(resultFont)
  stPM~setFont(resultFont)

  self~connectButtonEvent(IDC_PB_CALCULATE, "CLICKED", onCalc)


/** onChar()
 * This is the handler for the key event.  The args to the handler are:
 *
 *   use arg char, isShift, isCtrl, isAlt, misc, control
 *
 * where 'control' is the dialog control that had the focus, and had the key
 * event.  We just forward to that control and let the control handle the event
 * itself.
 *
 */
::method onChar unguarded
  forward to (arg(6))


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

