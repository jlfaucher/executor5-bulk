/**
 * Shows how prevent the user from entering invalid
 * characters in an edit control.  In this case the
 * user is resctricted to entering a decimal number
 * with no more than 2 decimal places.
 */

  -- Temp debugging
  .local~debug = .true

  .Edit~inherit(.NumberOnlyEditEx, .EditControl)

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

::requires ooDialog.cls
::requires winsystm.cls


::class 'MPGDlg' subclass ResDialog

::method initDialog
  expose editStartMiles editEndMiles editGas editCost stMPG stPM resultFont

  editStartMiles = self~newEdit(IDC_EDIT_START)
  editStartMiles~initDecimalOnly(1, .true)  -- temp to allow sign
  editStartMiles~connectKeyEvent(onChar)

  editEndMiles = self~newEdit(IDC_EDIT_END)
  editEndMiles~initDecimalOnly(0, .true)    -- temp to allow sign, no deicmals
  editEndMiles~connectKeyEvent(onChar)

  editGas = self~newEdit(IDC_EDIT_GAS)
  editGas~initDecimalOnly(3)
  editGas~connectKeyEvent(onChar)

  editCost = self~newEdit(IDC_EDIT_COST)
  editCost~initDecimalOnly(2)
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

::class 'NumberOnlyEditEx' public mixinclass object

::method initDecimalOnly
  expose decimalPlaces signOk initialized clipBoard
  use strict arg decimalPlaces, plusMinus = .false

  if plusMinus~isA(.String), plusMinus~datatype('O') then signOk = plusMinus
  else signOk = .false

  initialized = .true
  clipBoard = .WindowsClipboard~new

::method onChar unguarded
  expose decimalPlaces initialized signOk
  use arg char, isShift, isCtrl, isAlt, misc, control

  if .debug then do
    say "In edit control resource ID" self~ID 'decimalPlaces:' decimalPlaces 'initialized?' initialized
    say 'Got char:' char 'virtual key:' .VK~key2name(char) 'shft?' isShift 'ctrl?' isCtrl 'alt?' isAlt 'misc:' misc 'control' control
  end

  if \ initialized then return .true

  if misc~pos("extended") <> 0, self~isExtendedKey(char) then return .true

  if char == 8 then return .true
  if signOk, (char == 43 | char == 45) then return self~checkSign
  if char == 46 then return self~checkDecimal
  if char >= 48, char <= 57 then return self~checkDigit

  if isCtrl, \isAlt, \isShift then return self~checkControlKey(char)

  reply .false
  self~charBalloon


::method checkDigit private
  expose decimalPlaces signOk

  text = self~getText
  decimalPos = text~pos('.')
  cursorPos = self~getCaretPos

  if signOk, cursorPos == 1 then do
    c = text~left(1)

    if c == '-' | c == '+' then do
      reply .false
      self~showBalloon(.MT~NUM_TITLE, .MT~HAS_SIGN, "ERROR")
      return
    end
    return .true
  end

  if decimalPos == 0 then return .true

  if cursorPos <= decimalPos then return .true

  if text~length < (decimalPos + decimalPlaces) then return .true

  reply .false

  msg = self~decimalPlacesUsedMsg
  self~showBalloon(.MT~NUM_TITLE, msg, "ERROR")


/** checkSign()
 *
 * Checks that the sign character being typed is allowed at the current cursor
 * position.  Note that this method is only invoked when the sign character is
 * allowed.
 */
::method checkSign private
  expose decimalPlaces

  text = self~getText
  cursorPos = self~getCaretPos

  if cursorPos == 1 then do
    if self~hasSign(text) then do
      reply .false
      self~showBalloon(.MT~SIGN_TITLE, .MT~HAS_SIGN, "ERROR")
      return
    end

    return .true
  end

  reply .false

  decimalPos = text~pos('.')
  if decimalPos <> 0 | decimalPlaces == 0 then msg = .MT~ONLY_NUMBER
  else msg = .MT~ONLY_DECIMAL_NUMBER

  self~showBalloon(.MT~SIGN_TITLE, msg, "ERROR")


/** checkDecimal()
 *
 * Checks that the decimal point being typed is allowed at the current cursor
 * position.
 */
::method checkDecimal private
  expose decimalPlaces signOk

  text = self~getText
  decimalPos = text~pos('.')
  cursorPos = self~getCaretPos

  if decimalPlaces == 0 then do
    reply .false
    self~decimalPointBalloon(decimalPos, cursorPos, text)
    return
  end

  if decimalPos <> 0 then do
    reply .false
    self~decimalPointBalloon(decimalPos, cursorPos, text)
    return
  end

  if cursorPos == 1, signOk, self~hasSign(text) then do
    reply .false
    self~decimalPointBalloon(decimalPos, cursorPos, text)
    return
  end

  if text~length < cursorPos + decimalPlaces then return .true

  reply .false
  self~decimalPointBalloon(decimalPos, cursorPos, text)


::method checkControlKey unguarded private
  expose clipBoard decimalPlaces
  use strict arg char

  if char == 3 | char == 24 then return .true  -- Ctrl-X or Ctrl-C
  if char \== 22 then return .true             -- Not Ctrl-V

  -- If there is no text data available we don't care, pasting it will not do
  -- any harm.
  if \ clipBoard~isDataAvailable then return .true

  pastedText = clipBoard~paste

  -- We have text to paste, need to check is the text acceptable,
  -- and is it acceptable for where it will be placed.
  text = self~getText
  len  = text~length
  decimalPos = text~pos('.')
  cursorPos = self~getCaretPos

  -- If the pasted text is not a valid decimal number, or a valid signed decimal
  -- number if the sign is allowed, we don't need to check any further.
  if \ self~isValidDecimal(pasteText, text, cursorPos) then do
    reply .false
    self~pasteBalloon(pastedText)
    return
  end

  if pastedText~datatype('W') == 1 then do
    if decimalPos == 0 | cursorPos <= decimalPos then return .true
    if len - decimalPos + pastedText~length <= decimalPlaces then return .true

    reply .false
    self~pasteBalloon(pastedText)
    return
  end

  reply .false
  self~pasteBalloon(pastedText)


::method decimalPointBalloon private
  expose decimalPlaces signOk
  use strict arg decimalPos, cursorPos, text

  if .debug then do
    say 'decimalPos:' decimalPos 'cursorPos:' cursorPos 'text:' text 'sign allowed:' signOk
  end

  select
    when cursorPos == 1 then do
      if self~hasSign(text) then msg = .MT~HAS_SIGN
      else if decimalPlaces == 0, signOk then msg  = .MT~ONLY_WHOLE_SIGNED_NUMBER
      else if signOk then msg = .MT~ONLY_SIGNED_NUMBER
      else msg = .MT~ONLY_NUMBER
    end
    when decimalPlaces == 0 then do
      msg = .MT~ONLY_WHOLE_NUMBER
    end
    when cursorPos <= decimalPos then do
      msg = .MT~ONLY_NUMBER
    end
    when (decimalPos + decimalPlaces) > text~length then do
      msg = .MT~ONLY_NUMBER
    end
    otherwise do
      msg = self~decimalPlacesUsedMsg
    end
  end
  -- End select

  self~showBalloon(.MT~DEC_TITLE, msg, "ERROR")


::method charBalloon private
  expose decimalPlaces

  text = self~getText
  decimalPos = text~pos('.')
  cursorPos = self~getCaretPos

  if .debug then do
    say 'decimalPos:' decimalPos 'cursorPos:' cursorPos 'length:' text~length
  end

  msg = .MT~ONLY_NUMBER

  if decimalPos > 0, cursorPos > decimalPos, (decimalPos + decimalPlaces) <= text~length  then
    msg = self~decimalPlacesUsedMsg

  self~showBalloon(.MT~CHAR_TITLE, msg, "ERROR")


::method pasteBalloon private
  expose decimalPlaces
  use strict arg pastedText

  msg = .MT~ONLY_PASTE_A decimalPlaces .MT~ONLY_PASTE_B

  -- Balloon text has to be less than 1023 characters.  If we don't exceed that
  -- length, we will show the incorrect text to the user.  Otherwise we just
  -- show the short message.  Note that new line characters can be used to
  -- format the balloon text.
  len = msg~length + pastedText~length + .MT~PASTED_A~length + .MT~PASTED_B~length + 8

  if len < 1023 then do
    msg ||= .endOfLine~copies(2) || .MT~PASTED_A     ||    -
            .endOfLine~copies(2) || '"'pastedText'"' ||    -
            .endOfLine~copies(2) || .MT~PASTED_B
  end

  self~showBalloon(.MT~PASTE_TITLE, msg, "ERROR")


::method decimalPlacesUsedMsg private
  expose decimalPlaces

  if decimalPlaces == 1 then return .MT~ONLY_1_DECIMAL
  else return .MT~ONLY_DECIMALS_A decimalPlaces .MT~ONLY_DECIMALS_B


/** isValidDecimal()
 *
 * Checks that the pasted text, when pasted into text at the current position,
 * results in a valid decimal number.  Also checks the sign, if signed is okay.
 */
::method isValidDecimal private
  expose decimalPlaces signOk
  use strict arg pasteTex, text, cursorPos

  -- Can't have any type of space character, tab, new line, etc..
  if pasteText \== pasteText~space(0) then return .false

  pasteDecimals = pasteText~countStr(".")
  textDecimals   = text~countStr(".")

  -- Can't have more than 1 decimal point.
  if pasteDecimals + textDecimals > 1 then return .false

  -- Eliminate any problems with the sign character.
  if signOk then do
    if pasteText~pos('-') > 1 | pasteText~pos('+') > 1 then return .false

    pasteHasSign = self~hasSign(pasteText)
    textHasSign  = self~hasSign(text)

    if pasteHasSign, textHasSign then return .false
    if pasteHasSign, cursorPos \== 1 then return .false
    if textHasSign, cursorPos == 1 then return .false
  end
  else do
    if pasteText~pos('-') <> 0 | pasteText~pos('+') <> 0 then return .false
  end

  if pasteDecimals > 0 then do
    if decimalPlaces == 0 then return .false

    -- Count how many digits will be to the right of the decimal point when the
    -- text is pasted at the cursor position.
    textFollowing = text~length - cursorPos + 1
    pasteFollowing = pasteText~length - pasteText~pos('.')
    if (textFollowing + pasteFollowing) > decimalPlaces then return .false
  end

  if textDecimals > 0 then do
    decimalPos = text~pos(".")
    --if (cursorPos > decimalPos), 22  MM
  end

  return .true


::method isExtendedKey private
  use strict arg char

  if char >= 33, char <= 40 then return .true
  if char == 45 | char == 46 | char == 8 then return .true
  return .false


::method hasSign private
  use strict arg text

  c = text~left(1)
  if c == '-' | c == '+' then return .true
  return .false


::method getCaretPos private
  return self~selection~startChar


::class 'MT' private
::constant CHAR_TITLE           "Unacceptable Character"
::constant DEC_TITLE            "Unacceptable Decimal Point"
::constant NUM_TITLE            "Unacceptable Number"
::constant SIGN_TITLE           "Unacceptable Sign Character"
::constant PASTE_TITLE          "Unacceptable Pasted Text"

::constant ONLY_NUMBER                 "You can only type a number here."
::constant ONLY_SIGNED_NUMBER          "You can only type a number or the sign here."
::constant ONLY_DECIMAL_NUMBER         "You can only type a number or the decimal point here."
::constant ONLY_WHOLE_NUMBER           "You can only type a number here.  Only whole numbers are allowed."
::constant ONLY_WHOLE_SIGNED_NUMBER    "You can only type a number or the sign here.  Only whole numbers are allowed."

::constant HAS_SIGN             "You can not type here.  The sign character is already in place."
::constant SIGN_ONLY_FIRST             "You can not type here.  The sign character is already in place."
::constant ONLY_1_DECIMAL       "You can not type here.  Only 1 decimal place is allowed."
::constant ONLY_DECIMALS_A      "You can not type here.  Only"
::constant ONLY_DECIMALS_B      "decimal places are allowed."

::constant ONLY_PASTE_A         "You can only paste text here that produces a decimal number with"
::constant ONLY_PASTE_B         "or less decimal places."
::constant PASTED_A             "Pasted text of:"
::constant PASTED_B             "is not valid."
