/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2014 - 2014 Rexx Language Association. All rights reserved.  */
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
 *  Provides an infor message dialog that can be positioned anywhere on the
 *  screen, rather than centered, and whose font can be customized.
 *
 *  Public class:    .InfoMessage
 *  Public routine:  InfoMessage()
 */

-- End of entry point.
::requires "ooDialog.cls"

/**
 * Routine InfoMessage()
 *
 * Supplies the .InfoMessage class as a public routine.
 */
::routine InfoMessage public
  use strict arg msg, title = "Information Dialog", x = (-1), y = (-1), fontSize = .nil, fontName = .nil

  return .InfoMessage~new(msg, title, x, y, fontSize, fontName)~execute

/**
 * Class InfoMessage
 *
 * This class provides the programmer with a self-contained dialog that provides
 * some information displayed to the user.  Rather than have the dialog centered
 * on the screen, the programmer can position the dialog where ever she would
 * like.  In addition, the font size of the dialog can be specified providing a
 * way to create a more-readable dialog.
 *
 * Syntax:
 *
 *   .InfoMessage~new(information, <title>, <x>, <y>, <fontSize>, <fontName>)~execute
 *
 * Syntax example:
 *
 *   ret = .InfoMessage~new("Permanently delete ALL files?", "File Purge", 18, "Arial Bold" 'N')~execute
 *
 * Options:
 *   information       REQUIRED
 *     A string that will be displayed as the 'information' for this dialog
 *
 *   title          OPTIONAL
 *     The title for the dialog. If not specified the title will be "Ask Dialog"
 *
 *   x              OPTIONAL
 *      The x position for the top left corner of the dialog on the screen.  If
 *      this argument is omitted, or -1 then the dialog is centered on the
 *      screen.
 *
 *   y              OPTIONAL
 *      The y position for the top left corner of the dialog on the screen.  If
 *      this argument is omitted, or -1 then the dialog is centered on the
 *      screen.
 *
 *   fontSize       OPTIONAL
 *     The font size to use for the dialog. If omitted, or .nil, then the
 *     default dialog font size is used.
 *
 *   fontName       OPTIONAL
 *     The font to use for the dialog.  Normally the default dialog font would
 *     be used.  This argument changes the font to that specified.  Omit this
 *     argument, or specify .nil, to use the default dialog font.
 *
 *   Returns:
 *     0, always.
 *
 *   Remarks:
 *     Both the x and y arguments are restricted to a range of -1 through 8,000.
 *     If either argument is omitted, or if either argument is -1, then the
 *     dialog is centered on the screen.
 *
 */
::class 'InfoMessage' public subclass UserDialog

::constant CX_BTN     58
::constant CY_BTN     16
::constant TOP_MARGIN      17
::constant LEFT_MARGIN     17
::constant RIGHT_MARGIN    6
::constant BOTTOM_MARGIN   6

::method init
  expose msg title position fontName fontSize
  use strict arg msg, title = "Information Dialog", x = (-1), y = (-1), fontSize = .nil, fontName = .nil

  -- Check for proper args.
  if arg(1, 'O') then raise syntax 93.903 array (1)

  -- If the user specified x, then check it is a number between -1 and 8000
  if arg(2, 'E') then do
    errMsg = 'Argument 2 "x" must be a whole number between -1 and 8000, found:' x
    if \ x~isInstanceOf(.string) then raise syntax 93.900 array (errMsg)
    if \ x~datatype('W') then raise syntax 93.900 array (errMsg)
    if x < -1 | x > 8000 then raise syntax 93.900 array (errMsg)
  end

  -- If the user specified y, then check it is a number between -1 and 8000
  if arg(3, 'E') then do
    errMsg = 'Argument 3 "y" must be a whole number between -1 and 8000, found:' y
    if \ y~isInstanceOf(.string) then raise syntax 93.900 array (errMsg)
    if \ y~datatype('W') then raise syntax 93.900 array (errMsg)
    if y < -1 | y > 8000 then raise syntax 93.900 array (errMsg)
  end

  -- If fontSize is .nil, then that arg was omitted, otherwise we check that it is valid
  if fontSize <> .nil then do
    errMsg = 'Argument 5 "fontSize" must be a whole number between 8 and 100, found:' fontSize
    if \ fontSize~isInstanceOf(.string) then raise syntax 93.900 array (errMsg)
    if \ fontSize~datatype('W') then raise syntax 93.900 array (errMsg)
    if fontSize < 8 | fontSize > 100 then raise syntax 93.900 array (errMsg)
  end

  -- A similar check for fontName
  if fontName <> .nil then do
    errMsg = 'Argument 6 "fontName" must be the string name of a font, found:' fontName
    if \ fontName~isInstanceOf(.string) then raise syntax 93.900 array (errMsg)
  end

  -- Have the superclass initialize.
  self~init:super
  if self~initCode <> 0 then
    raise syntax 98.900 array ("The superclass did not initialize correctly")

  if x == -1 | y == -1 then position = .nil
  else position = .Point~new(x, y)

  -- To correctly calculate sizes in dialog units, they must be calculated using
  -- the actual font that the dialog will be using.  If the user specified a
  -- font or size, we set them here, so that the sizing calculations will be
  -- correct.
  if fontSize <> .nil then self~fontSize = fontSize
  if fontName <> .nil then self~fontName = fontName

  -- Set symbolic IDs for the dialog controls.
  self~constDir["IDC_ST_ICON"] = 100
  self~constDir["IDC_ST_MSG"]  = 102

return 0

/**
 * execute()
 *
 * Determine the layout of the dialog based on the message and the font size
 * supplied  by the programmer.  Create the dialog and show it to the user.
 * Then return the user's response to the programmer.
 */
::method execute
  expose msg title positionTable infoIcon maxTextWidth lineHeight lineCount interGap iconSize position

  -- Position table will hold the co-ordinates needed to create the dialog
  -- controls
  positionTable = .table~new

  self~calcSizes

  -- Get the operating system's stock question icon.
  infoIcon = .Image~getImage(.Image~toID(IDI_ASTERISK), .Image~toID(IMAGE_ICON))

  -- Position the icon, other postions are based on the icon's position.  The
  -- static control will be resized to the actual size of the icon when it is
  -- created. We have already calculated that size in dialog units
  positionTable[IDC_ST_ICON] = .DlgPosition~new(self~LEFT_MARGIN, self~TOP_MARGIN, iconSize~width, iconSize~height)

  -- The over-all size of the dialog is based on the width and height of the
  -- message. But, for aesthetics, we want the message width to be at least as
  -- wide as the Ok button plus a little.  If the message width is less than
  -- this minimum, increase the message width.
  minX = self~CX_BTN + 5
  if lineCount == 1, maxTextWidth < minX then maxTextWidth = minX

  -- Add the position and size values to the table for the static text box.
  p = positionTable[IDC_ST_ICON]
  if lineCount <= 2 then do
    h = lineCount * lineHeight
    y = p~y + trunc((p~cy - h) / 2)
  end
  else do
    y = p~y
  end
  positionTable[IDC_ST_MSG] = .DlgPosition~new(p~x + p~cx + 5, y, maxTextWidth, lineCount * lineHeight)

  -- Calculate where the interGap starts.  It is the bottom of the static text,
  -- or the bottom of the static icon control, whichever is lower.
  bottomY = positionTable[IDC_ST_ICON]~y + positionTable[IDC_ST_ICON]~cy
  if bottomY <  positionTable[IDC_ST_MSG]~y + positionTable[IDC_ST_MSG]~cy then do
    bottomY = positionTable[IDC_ST_MSG]~y + positionTable[IDC_ST_MSG]~cy
  end

  -- This will be the top of the button:
  topY = bottomY + interGap

  p = positionTable[IDC_ST_MSG]
  dlgCX = p~x + p~cx + self~RIGHT_MARGIN
  dlgCY = topY + self~CY_BTN + self~BOTTOM_MARGIN

  -- If the message is very narrow, the dialog is too small.  We at least want
  -- the button to the left of the icon.
  dlgMinCX = self~LEFT_MARGIN + self~RIGHT_MARGIN + iconSize~width + self~CX_BTN + 5
  if dlgMinCX > dlgCX then dlgCX = dlgMinCX

  buttonX = dlgCX - self~RIGHT_MARGIN - self~CX_BTN

  -- Ok button
  positionTable[IDOK] = .DlgPosition~new(buttonX, topY, self~CX_BTN, self~CY_BTN)

  -- Now, define the dialog.  If the user specified a postion we use that,
  -- otherwise we center the dialog.
  if position == .nil then self~createCenter(dlgCX, dlgCY, title, "")
  else self~create(position~x, position~y, dlgCX, dlgCY, title, "")

  -- The dialog is now completely defined.  execute will create the underlying
  -- Windows dialog and run until the user closes the Windows dialog.  When the
  -- user closes the Windows dialog, we will return from the execute method.
  self~execute:super("SHOWTOP")

return 0

/**
 * defineDialog
 *
 * Define all the controls for this dialog using the values stored in the
 * position table.
 */
::method defineDialog
  expose msg positionTable

  pos = positionTable[IDC_ST_ICON]
  self~createStaticImage(IDC_ST_ICON, pos~x, pos~y, pos~cx, pos~cy, "NOTIFY ICON SIZEIMAGE")

  -- Static text message:
  pos = positionTable[IDC_ST_MSG]
  self~createStaticText(IDC_ST_MSG, pos~x, pos~y, pos~cx, pos~cy, "LEFT", msg)

  pos = positionTable[IDOK]
  self~createPushButton(IDOK, pos~x, pos~y, pos~cx, pos~cy, 'DEFAULT', "Ok")


/**
 * calcSizes
 *
 * This methods calculates some sizes to help us layout the dialog.  The size of
 * the dialog is dependent on the text message.  If the programmer uses a very
 * long string, this gets tricky.  We don't want the dialog to be wider than the
 * screen.  The static text control will automatically wrap the text if it is
 * wider than the control width, but we need to make the control high enough.
 *
 * Looking at the Windows message box, it appears that the OS keeps its width
 * no more that 1/3 of the screen, so we will arbitrarily set the maximum width
 * of the dialog to 1/3 of the screen width.
 *
 * To further complicate this, we need to lay out the dialog in dialog units,
 * but much of what we know is in pixels.
 */
::method calcSizes private
  expose msg maxTextWidth lineHeight lineCount interGap iconSize

  -- First determine the width and height of the message, in dialog units, at
  -- the font size used by the dialog.
  size = self~getTextSizeDU(msg)
  msgWidth = size~width
  lineHeight = size~height

  -- Calculate size of the icon.  The static control will be resized to the
  -- actual size of the icon when it is created.  We can get the *pixel* size of
  -- the icon from the system metrics.  But, we need the size in dialog units.
  SM_CXICON = 11
  SM_CYICON = 12
  cx = .DlgUtil~getSystemMetrics(SM_CXICON)
  cy = .DlgUtil~getSystemMetrics(SM_CYICON)

  iconSize = .Size~new(cx, cy)
  self~pixel2dlgUnit(iconSize)

  -- Get the max width of the dialog, use that to determine the max width of our
  -- static text control.  Again we need to convert pixels to dialog units
  maxDlgSize = .Size~new(trunc(.SM~cxScreen / 3), .SM~cyScreen)
  self~pixel2dlgUnit(maxDlgSize)
  maxTextWidth = maxDlgSize~width - (self~LEFT_MARGIN + self~RIGHT_MARGIN + iconSize~width + 5)

  -- interGap is the space between the bottom of the static text control, or
  -- icon control, and the top of the buttons.  We make this 3 * line height
  interGap = 3 * lineHeight

  -- lineCount is the quess as to how many lines we need for the message to fit
  -- within the max width we have available. Because of word wrap, a simple
  -- division can not be accurate.
  if msgWidth < maxTextWidth then do
    lineCount = 1
    maxTextWidth = msgWidth
  end
  else do
    lineCount = trunc(msgWidth / maxTextWidth)
  end

  -- The longer the message, the more likely that word wrap will produce more
  -- lines.
  select
    when lineCount > 20 then lineCount += 5
    when lineCount > 15 then lineCount += 4
    when lineCount > 10 then lineCount += 3
    when lineCount > 6 then  lineCount += 2
    when lineCount > 0 then  lineCount += 1
    otherwise nop
  end
  -- End select


/**
 * initDialog()
 *
 * If the programmer requested that the No button be the default, we change the
 * focus here to be that button.
 */
::method initDialog
  expose infoIcon position

  st = self~newStatic(IDC_ST_ICON)
  st~~setImage(infoIcon)

  if position <> .nil then self~moveTo(position)
  self~ensureVisible


/**
 * Class DlgPosition
 *
 * A simple utility class to hold position / size co-ordinantes.
 */
::class 'DlgPosition'
::method x attribute
::method y attribute
::method cx attribute
::method cy attribute
::method init
  use arg x = 0, y = 0, cx = 0, cy = 0
  self~x = x
  self~y = y
  self~cx = cx
  self~cy = cy
