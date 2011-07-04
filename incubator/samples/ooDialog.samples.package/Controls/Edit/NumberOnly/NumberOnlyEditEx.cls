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
 *  The NumberOnlyEditEx class extends the .Edit class to provide an edit
 *  control which restricts entry to decimal only or signed decimal only
 *  numbers.
 *
 *  The NumberOnlyEditEx.cls file is meant to be included in any ooDialog
 *  program where this extension to the edit control is desired.
 */

  .Edit~inherit(.NumberOnlyEditEx, .EditControl)

::requires 'ooDialog.cls'
::requires 'winsystm.cls'

::class 'NumberOnlyEditEx' public mixinclass object

::method initDecimalOnly
  expose decimalPlaces signOk initialized clipBoard
  use strict arg decimalPlaces = 2, plusMinus = .false

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

  -- isValidDecimal() checks that the resulting text after inserting the pasted
  -- text is valid.
  if \ self~isValidDecimal(pastedText, text, cursorPos) then do
    reply .false
    self~pasteBalloon(pastedText, text, cursorPos)
    return
  end

  return .true


/**
 * This method produces the proper ballon message text when a decimal point has
 * been typed, but is not allowed at the current position.
 */
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
  expose decimalPlaces signOk

  text = self~getText
  decimalPos = text~pos('.')
  cursorPos = self~getCaretPos

  if .debug then do
    say 'decimalPos:' decimalPos 'cursorPos:' cursorPos 'length:' text~length
  end

  msg = .MT~ONLY_NUMBER

  select
    when cursorPos == 1 then do
      if self~hasSign(text) then msg = .MT~HAS_SIGN
      else if decimalPlaces == 0, signOk then msg  = .MT~ONLY_WHOLE_SIGNED_NUMBER
      else if decimalPlaces == 0, \signOk then msg  = .MT~ONLY_WHOLE_NUMBER
      else if decimalPos == 0, signOk then msg  = .MT~ONLY_SIGNED_DECIMAL_NUMBER
      else if decimalPos == 0, \signOk then msg  = .MT~ONLY_DECIMAL_NUMBER
      else if signOk then msg = .MT~ONLY_SIGNED_NUMBER
      else msg = .MT~ONLY_NUMBER
    end
    when decimalPlaces == 0 then do
      msg = .MT~ONLY_WHOLE_NUMBER
    end
    when decimalPos == 0 then do
      msg = .MT~ONLY_DECIMAL_NUMBER
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

  self~showBalloon(.MT~CHAR_TITLE, msg, "ERROR")


::method pasteBalloon private
  expose decimalPlaces signOk
  use strict arg pastedText, text, cursorPos

  if signOk then do
    if decimalPlaces == 0 then msg = .MT~ONLY_PASTE_WHOLE_SIGNED
    else msg = .MT~ONLY_PASTE_A_SIGNED  decimalPlaces .MT~ONLY_PASTE_B
  end
  else do
    if decimalPlaces == 0 then msg = .MT~ONLY_PASTE_WHOLE
    else msg = .MT~ONLY_PASTE_A decimalPlaces .MT~ONLY_PASTE_B
  end

  resultText = text~substr(1, cursorPos - 1) || pastedText || text~substr(cursorPos); say 'result:' resultText

  -- Balloon text has to be less than 1023 characters.  If we don't exceed that
  -- length, we will show the incorrect text to the user.  Otherwise we just
  -- show the short message.  Note that new line characters can be used to
  -- format the balloon text.
  len = msg~length + resultText~length + .MT~PASTED_A~length + .MT~PASTED_B~length + 8

  if len < 1023 then do
    msg ||= .endOfLine~copies(2) || .MT~PASTED_A     ||    -
            .endOfLine~copies(2) || '"'resultText'"' ||    -
            .endOfLine~copies(2) || .MT~PASTED_B
  end

  self~showBalloon(.MT~PASTE_TITLE, msg, "ERROR")


::method decimalPlacesUsedMsg private
  expose decimalPlaces

  if decimalPlaces == 1 then return .MT~ONLY_1_DECIMAL
  else return .MT~ONLY_DECIMALS_A decimalPlaces .MT~ONLY_DECIMALS_B


/** isValidDecimal()
 *
 * Checks that the resulting text after the pasted text is inserted into text at
 * the current position, results in a valid number using the current
 * restrictions.  Decimal places and sign allowed or not.
 */
::method isValidDecimal private
  expose decimalPlaces signOk
  use strict arg pasteText, text, cursorPos

  resultText = text~substr(1, cursorPos - 1) || pasteText || text~substr(cursorPos); say 'resultText:' resultText

  -- Can't have any type of space character, tab, new line, etc..
  if resultText \== resultText~space(0) then return .false

  -- Eliminate obvious problems with decimals.
  countDots = resultText~countStr(".")
  if countDots > 1 then return .false
  if decimalPlaces == 0, countDots > 0 then return .false

  -- Eliminate any problems with the sign character.
  if signOk then do
    if resultText~pos('-') > 1 | resultText~pos('+') > 1 then return .false
  end
  else do
    if resultText~pos('-') <> 0 | resultText~pos('+') <> 0 then return .false
  end


  decimalPos = resultText~pos('.')
  if decimalPos > 0 then do
    if resultText~length - decimalPos > decimalPlaces then return .false

    -- Remove the decimal char so we can test for all digits.
    resultText = resultText~changeStr('.', '')
  end

  -- Remove the sign character if it exists.
  if self~hasSign(resultText) then resultText = resultText~substr(2)

  -- Exponential notation can not be allowed.
  if resultText~caselessPos('E') <> 0 then return .false

  if \ resultText~datatype('W') then return .false

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
::constant ONLY_SIGNED_DECIMAL_NUMBER  "You can only type a number, the sign, or the decimal point here."
::constant ONLY_WHOLE_NUMBER           "You can only type a number here.  Only whole numbers are allowed."
::constant ONLY_WHOLE_SIGNED_NUMBER    "You can only type a number or the sign here.  Only whole numbers are allowed."

::constant HAS_SIGN             "You can not type here.  The sign character is already in place."
::constant ONLY_1_DECIMAL       "You can not type here.  Only 1 decimal place is allowed."
::constant ONLY_DECIMALS_A      "You can not type here.  Only"
::constant ONLY_DECIMALS_B      "decimal places are allowed."

::constant ONLY_PASTE_WHOLE        "You can only paste text here that produces a whole number."
::constant ONLY_PASTE_WHOLE_SIGNED "You can only paste text here that produces a signed whole number."
::constant ONLY_PASTE_A_SIGNED     "You can only paste text here that produces a signed decimal number with"
::constant ONLY_PASTE_A            "You can only paste text here that produces a decimal number with"
::constant ONLY_PASTE_B            "or less decimal places."
::constant PASTED_A                "Resulting text of:"
::constant PASTED_B                "is not valid."
