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

/* Improved SingleSelection dialog - the PickOneDlg class */

  .local~put(.PickOneDlg, 'PICKONEDLG')

-- End of entry point.
::requires "OODWin32.cls"

/**
 * Class PickOneDlg
 *
 * This class provides the programmer with a self-contained dialog that allows
 * a user to pick one option out of a number of mutually exclusive options.  The
 * dialog provides space for a message at the top of the dialog, a series of
 * auto radio buttons detailing the options, and the typical Ok / Cancel
 * buttons.
 *
 * The dialog is created and sized dynamically based on the number of radio
 * button labels specified.  The programmer is not required to input any
 * information related to size or position for the dialog.
 *
 * Which radio button the user picked is returned to the programmer.
 *
 * Syntax example:
 *
 *   answer = .PickOneDlg~new(msg, title, labels, check, minimum)~execute
 *
 * Options:
 *   msg      REQUIRED
 *     The message placed in the top panel of the dialog.
 *
 *   title    REQUIRED
 *     The title for the dialog.
 *
 *   labels   REQUIRED
 *     An array containing the labels for the radio buttons.  A radio button
 *     will be created for each index in the array using the value of the index
 *     as the label for the radio button.
 *
 *   check    OPTIONAL
 *     The index of the radio button that should be initially checked.  If
 *     omitted or out of range, the first radio button will be checked.
 *
 *   minimum  OPTIONAL
 *     A minimum width for the dialog can optionally be specified by the
 *     programmer.  This class determines the size of the dialog based on
 *     several factors, including the number and widths of the radio buttons.
 *     If the dialog is being created to tall and skinny, the programmer may
 *     want to specify a minimum width that is large enough to make the dialog
 *     more proportional.
 *
 * Returns:
 *  The one-based index of the selected radio button, or -1 if the user cancels
 *  the dialog.
 */
::class 'PickOneDlg' subclass UserDialog inherit AdvancedControls

::method init
  expose msg title radioButtons selected
  use arg msg, title, radioButtons, selected, minimumWidth

  -- Have the superclass initialize.
  self~init:super
  if self~initCode <> 0 then
    raise syntax 98.900 array ("The superclass did not initialize correctly")

  -- Check for proper args.
  if arg(1, 'O') then raise syntax 93.903 array (1)
  if arg(2, 'O') then raise syntax 93.903 array (2)
  if arg(3, 'O') then raise syntax 93.903 array (3)

  if \ radioButtons~isInstanceOf(.array) then
    raise syntax 93.948 array (3, "Array")

  -- Set an arbitrary limit of no more than 20 radio buttons.  The reality is
  -- that for more than 7 or 8 choices, the programmer should really pick a
  -- different interface.
  if radioButtons~items > 20 then
    raise syntax 93.900 array ("The number of radio button labels exceeds the maximum of 20")

  -- Set selected to the index of the radio button that should be checked when
  -- the dialog is first shown.
  if arg(4, 'O') then selected = 1
  else if \ selected~datatype('W') then selected = 1
  else if selected < 1 | selected > radioButtons~items then selected = 1

  -- Set symbolic IDs for the dialog controls.
  self~constDir["IDC_ST_MSG"] = 100
  self~constDir["IDC_GB_RADIOBUTTONS"] = 101
  self~constDir["IDC_RB_BASE"] = 110
  self~constDir["IDC_RB_LAST"] = 110 + radioButtons~items - 1

  -- Set the minimum width of the dialog.
  min = 120
  if arg(5, 'E') then do
    if minimumWidth~datatype('W') then do
      if minimumWidth > min then min = minimumWidth
    end
  end

  -- Put the minimum width value into the Constant Directory, (the constDir
  -- attribute.)
  --
  -- NOTE: You should NOT do this IFF you are using a data stem with symbolic
  -- IDs.  If you are using a data stem with symbolic IDs, you would want to be
  -- sure to only put unique resource IDs into the constDir.  Since this class
  -- is structured to not use the data stem and is not really suitable for sub-
  -- classing, this usage of the constDir causes no problems.
  self~constDir["DLG_MIN_CX"] = min

return 0

/**
 * execute()
 *
 * Determine the layout of the dialog and its controls based on the message amd
 * radio button labels supplied by the programmer.  Create the dialog and show
 * it to the user.  Then return the user's response to the programmer.
 */
::method execute
  expose msg title radioButtons selected

  -- First determine the maximum width of a radio button label, the width of
  -- the message, and the (maximum) height of a line of text.
  parse value GetStdTextSize(msg) with msgWidth textHeight .

  rbCX = 0
  do l over radioButtons
    parse value GetStdTextSize(l) with cx cy .
    if cx > rbCX then rbCX = cx
    if cy > textHeight then textHeigth = cy
  end

  -- Determine if we are going to use 2 columns and if so how many radio buttons
  -- will be in the first column.
  if radioButtons~items > 6 then
    twoColumn = .true
  else
    twoColumn = .false

  select
    when \ twoColumn then columnOneCount = radioButtons~items
    when radioButtons~items < 9  then columnOneCount = 4
    when radioButtons~items < 13 then columnOneCount = 6
    when radioButtons~items < 17 then columnOneCount = 8
    otherwise columnOneCount = 10
  end
  -- End select

  -- The following uses more variables than is necessary, but it makes it easier
  -- to keep track of what is what and to visualize the adjustments needed to
  -- calculate the absolute positions of the controls.

  -- Outside margin around the dialog controls.
  dlgMarginCXY = 10

  -- The width of the radio button circle.
  rbCircleWidth = 10

  -- Group boxes have space for the title text at the top of the group box.
  -- The top line cuts through the middle of this text.  The space is still
  -- there even if there is no text in the title.  This offset is used to
  -- calculate where the first radio button goes.
  gbTopOffsetCY = textHeight

  -- Have a margin around the inside of the group box.
  gbMargin = trunc(.5 * textHeight) + 1

  -- Spacer is the space between 'sections' of the dialog.  I.e. between the
  -- static text and the group box; between the group box and the butons.  Make
  -- it the height of one line of text.
  spacer = textHeight

  -- The width for a radio button is the width of the longest label plus the
  -- width of radio button circle.  The height of a radio button is the height
  -- of a line of text.
  rbCX = rbCX + rbCircleWidth
  rbCY = textHeight

  -- rbTotalCX is the total width for the radio buttons.  If there is only 1
  -- column, then rbTotalCX is equal to rbCX.
  if twoColumn then
    rbTotalCX = (2 * rbCX) + spacer
  else
    rbTotalCX = rbCX

  -- Enforce a minimum width for the dialog.
  if (rbTotalCX + (2 * dlgMarginCXY) + (2 * gbMargin)) < self~constDir~DLG_MIN_CX then do
    rbTotalCX = self~constDir~DLG_MIN_CX - (2 * dlgMarginCXY) - (2 * gbMargin)

    if twoColumn then
      rbCX = trunc((rbTotalCX - spacer) / 2) + 1
    else
      rbCX = rbTotalCX
  end

  -- This offset is from the top Y co-ordinate of 1 radio button to the top Y
  -- co-ordinate of the next.
  rbOffsetY = rbCY + gbMargin

  -- Calculate the height for all the radio buttons when they are spaced apart.
  rbRowsCY = rbOffsetY * columnOneCount

  -- The width of the group box is determined by the width of the widest radio
  -- button column(s).  Calculate this width, which then determines the width of
  -- the static message box.
  gbCX = rbTotalCX  +  2 * gbMargin

  -- In order to determine how high to make the static text control that
  -- displays the message, we need to know how many lines of text the message
  -- will need.  If the message width is less than the group box width, there is
  -- only 1 line of text needed.  Otherwise we need to calculate the number of
  -- lines.
  if msgWidth <= gbCX then do
    countMsgLines = 1
  end
  else do
    -- Divide the width of the message by the width of the group box.  This
    -- gives the approximate number of lines needed in the static text control.
    -- Round the number up, always.
    ratio = msgWidth / gbCX
    countMsgLines = trunc(ratio) + 1

    -- Windows will break the message on word boundaries.  Therefore dividing
    -- the message width by the width of the group box might not be sufficiently
    -- accurate.  There is no perfect solution.  If all words are short, then
    -- an extra line will probably not be needed.  On the other hand, if very
    -- long words are close to the word break point, more than one line may be
    -- needed.
    if (ratio - trunc(ratio)) > .5 then countMsgLines = countMsgLines + 1

    -- In addition, Windows will honor new line sequences in the text.  This
    -- will also cause more lines to be needed.  Again, there is no perfect
    -- solution.
    --c = msg~countstr('0d0a'x)
    countMsgLines = countMsgLines + trunc(msg~countstr('0d0a'x) * (7/8))
  end

  -- Put all the numbers that the defineDialog method will need into the
  -- Constant Directory, (the constDir attribute.)
  --
  -- NOTE: You should NOT do this IFF you are using a data stem with symbolic
  -- IDs.  If you are using a data stem with symbolic IDs, you would want to be
  -- sure to only put unique resource IDs into the constDir.  Since this class
  -- is structured to not use the data stem and is not really suitable for sub-
  -- classing, this usage of the constDir causes no problems.

  -- Co-ordinates of the the static text.
  self~constDir["ST_X"]  = dlgMarginCXY
  self~constDir["ST_Y"]  = dlgMarginCXY
  self~constDir["ST_CX"] = gbCX
  self~constDir["ST_CY"] = countMsgLines * textHeight

  -- Co-ordinates of the group box
  self~constDir["GB_X"]  = dlgMarginCXY
  self~constDir["GB_Y"]  = self~constDir~ST_Y + self~constDir~ST_CY + spacer
  self~constDir["GB_CX"] = gbCX
  self~constDir["GB_CY"] = rbRowsCY + (2 *gbMargin) + gbTopOffsetCY

  -- Radio buttons co-ordinates
  self~constDir["RB_X"]  = dlgMarginCXY + gbMargin
  self~constDir["RB_Y"]  = self~constDir~GB_Y + gbTopOffsetCY + gbMargin
  self~constDir["RB_CX"] = rbCX
  self~constDir["RB_CY"] = rbCY

  self~constDir["RB_OFFSET_Y"] = rbOffsetY

  if twoColumn then
    self~constDir["RB_COL2_X"] = self~constDir~RB_X + self~constDir~RB_CX + spacer
  else
    self~constDir["RB_COL2_X"] = 0

  self~constDir["RB_COL1_COUNT"] = columnOneCount

  -- Push buttons, Ok and Cancel.  These buttons have the same width and height.
  -- Their Y co-ordinate is the same, so we only need to differentiate their
  -- X co-ordinate.
  self~constDir["PB_Y"]        = self~constDir~GB_Y + self~constDir~GB_CY + spacer
  self~constDir["PB_CX"]       = 35
  self~constDir["PB_CY"]       = 15

  self~constDir["PB_OK_X"]     = self~constDir~GB_X + self~constDir~GB_CX - 35
  self~constDir["PB_CANCEL_X"] = self~constDir~PB_OK_X - spacer - 35

  -- Calculate the over-all size for the dialog.
  dlgX = (2 * dlgMarginCXY) + self~constDir~GB_CX
  dlgY = self~constDir~PB_Y + self~constDir~PB_CY + dlgMarginCXY

  -- Now, define the dialog.  The createCenter method will invoke the create
  -- method, which will in turn invoke the defineDialog method of this instance.
  self~createCenter(dlgX, dlgY, title, "NOT VISIBLE")

  -- The dialog is now completely defined.  execute will create the underlying
  -- Windows dialog and run until the user closes the Windows dialog.  When the
  -- user closes the Windows dialog, either the ok or the cancel method of this
  -- instance will be invoked and select will get set to the appropriate value.
  -- We will then return the value of select.
  self~execute:super("SHOWTOP")

return selected

/**
 * defineDialog
 *
 * Define all the controls for this dialog using the values stored in the
 * constDir.
 */
::method defineDialog
  expose msg radioButtons

  -- This uses some extra, unnecessary, variables to make it more readable.

  -- Static text message:
  x  = self~constDir~ST_X
  y  = self~constDir~ST_Y
  cx = self~constDir~ST_CX
  cy = self~constDir~ST_CY
  self~addText(x, y, cx, cy, msg, "", IDC_ST_MSG);

  -- Group box:
  x  = self~constDir~GB_X
  y  = self~constDir~GB_Y
  cx = self~constDir~GB_CX
  cy = self~constDir~GB_CY
  self~addGroupBox(x, y, cx, cy, "", "", IDC_GB_RADIOBUTTONS)

  -- Radio buttons:
  x      = self~constDir~RB_X
  startY = self~constDir~RB_Y
  cx     = self~constDir~RB_CX
  cy     = self~constDir~RB_CY
  offset = self~constDir~RB_OFFSET_Y
  do i = 1 to self~constDir~RB_COL1_COUNT
    y  = startY + ((i - 1) * offset)
    id = self~constDir~IDC_RB_BASE + i - 1
    self~addRadioButton(id, , x, y, cx, cy, radioButtons[i])
  end

  -- Do we have two columns?
  if self~constDir~RB_COL2_X <> 0 then do
    x = self~constDir~RB_COL2_X
    j = 0

    do i = i to radioButtons~items
      y  = startY + (j * offset)
      id = self~constDir~IDC_RB_BASE + i - 1
      self~addRadioButton(id, , x, y, cx, cy, radioButtons[i])
      j = j + 1
    end
  end

  -- OK and Cancel buttons:
  x  = self~constDir~PB_CANCEL_X
  y  = self~constDir~PB_Y
  cx = self~constDir~PB_CX
  cy = self~constDir~PB_CY
  self~addButton(IDCANCEL, x, y, cx, cy, "Cancel", ,"GROUP")

  x = self~constDir~PB_OK_X
  self~addButton(IDOK, x, y, cx, cy, "OK", "GROUP")

/**
 * initDialog()
 *
 * Check and set the focus to the initial radio button default.
 */
::method initDialog
  expose selected
  id = self~constDir~IDC_RB_BASE + selected - 1
  self~getRadioControl(id)~check
  self~focusItem(id)

/**
 * ok()
 *
 * The user pressed the OK button.  Set selected to the checked radio button.
 */
::method ok
  expose selected

  do i = self~constDir~IDC_RB_BASE to self~constDir~IDC_RB_LAST
    if self~getRadioControl(i)~isChecked == "CHECKED" then do
      selected = i - self~constDir~IDC_RB_BASE + 1
      leave
    end
  end

return self~ok:super

/**
 * cancel()
 *
 * The user canceled the dialog.  Set selected to -1.
 */
::method cancel
  expose selected
  selected = -1
return self~cancel:super
