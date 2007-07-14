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
  File:    ListView.rex                             Author:      Mark Miesfeld
                                                    Creation date:  07/08/2007
  Company: N/A
  Project: ooDialog Examples                        Last update:    07/14/2007
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Category:
    Command Line

  Syntax:
    list-view [useRC]

  Purpose:
    Demonstrates (uses) new List-View features in ooDialog.

  Assumes:
    Rexx is ooRexx (Open Object Rexx.)  Requires an ooRexx build later than
    SVN revision: 590

  Input:
    useRC  OPTIONAL
      Will use the resource script file (*.rc) to define the dialog.  By default
      the binary resource file (*.dll) is used.

  Returns:
    0 if no error, non-zero if the dialog could not be created.

  Notes:
    This dialog was created using a visual resource editor, the ResEdit Resource
    Editor.  There is another sample ooDialog package that contains example code
    and documentation on how to use a number of common resource editors.  This
    package should be located in the ooRexx SVN repository, currently at:

    https://oorexx.svn.sourceforge.net/svnroot/oorexx/incubator/samples/ooDialog/ResourceEditors

    and could be checked out separately using this command:

    svn co https://oorexx.svn.sourceforge.net/svnroot/oorexx/incubator/samples/ooDialog/ResourceEditors <yourNameForDirectory>

    Note: The ResourceEditors package may not appear in SVN until 7/15/2007.

    Symbolic IDs are used exclusively in this program for the resource ID
    numbers of the dialog controls.  ResEdit manages the symbolic IDs and places
    them in the resource.h header file.  Passing the name of the header file
    when the ooDialog dialog is instantiated is what allows these IDs to be used
    in place of the numeric IDs.  In a program like this that uses a lot of
    resource IDs, symbolic IDs make the code much easier to read and maintain.

    This program will use either the resource script (ListViewEx.rc) file or the
    binary compiled resource (ListViewEx.dll) file for the definition of the
    dialog.  This is done simply to show that it is possible, (and also because
    this program was used to unit-test the code when the new list-view features
    were added to ooDialog.)  Normally you would write your application to use
    one or the other, not both.

  Changes:
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
use arg useRC

  -- This stem variable is used by ooDialog to set the initial state of the
  -- controls.  There is only a need to set radio buttons and checkboxes that
  -- are to start out as checked (1), by default they are set to unchecked (0).
  initData.IDC_EDIT_CHKBOX = ""
  initData.IDC_RB_CHK_SET = 1
  initData.IDC_CHECK_DOALL = 1
  initData.IDC_CHECK_CHECKBOXES = 1
  initData.IDC_CHECK_FULLROW = 1
  initData.IDC_CHECK_HEADERDRAGDROP = 1
  initData.IDC_CHECK_GRIDLINES = 1
  initData.IDC_CHECK_DOUBLEBUFFER = 1
  initData.IDC_CHECK_LABELTIP = 1
  initData.IDC_CHECK_INFOTIP = 1
  initData.IDC_CHECK_EVENTCHK = 1
  initData.IDC_CHECK_EVENTSEL = 1
  initData.IDC_CHECK_ONEXIT = 1

  if arg(1, 'O') then
    dlg = .ListViewExDLL~new("ListViewEx.dll", IDD_DLG, initData., "resource.h")
  else
    dlg = .ListViewExRC~new("ListViewEx.rc", IDD_DLG, initData., "resource.h")

  if dlg~initCode <> 0 then do
    say "Error creating dialog.  InitCode:" dlg~initCode
    return dlg~initCode
  end

  dlg~Execute("SHOWTOP")
  dlg~Deinstall

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "oodWin32.CLS"

-- The ListViewEx.dll version of the dialog.
::class ListViewExDLL subclass ResDialog inherit ListViewExBase

-- The ListViewEx.rc version of the dialog.
::class ListViewExRC subclass RcDialog inherit ListViewExBase

-- All the functionality of the dialog is implemented in this base, mixin,
-- class.
::class ListViewExBase public mixinclass BaseDialog                            -
                       inherit AdvancedControls MessageExtensions

-- initDialog()
--
-- This method is called after the underlying Windows dialog has been created,
-- but before it is shown on the screen.
--
-- It makes it the correct place to do any initializing that needs the actual
-- Windows control.  For instance, you can not insert a row into a list-view
-- control, until Windows has actually created the control.  You can
--
-- I also think it is the best place to connect methods to the control events.
-- You can do the connecting in the init method, but by doing it here, you keep
-- everything in one place and you do not need to unnecessarily over-ride the
-- init method.
::method initDialog
  expose sLC lb

  -- Note this:  In this example, I do not check what the return code is on most
  -- of the method calls.  However, if things were not working as I expected,
  -- that is the first thing I would check.

  -- Connect the push button events to methods of this class.
  ret = self~connectButton(IDC_PB_SETSTYLE, "onSetStyle")
  ret = self~connectButton(IDC_PB_GETSTYLE, "onGetStyle")
  ret = self~connectButton(IDC_PB_EXECUTE,  "onExecute")
  ret = self~connectButton(IDC_PB_CLEAR,    "onClear")

  -- Get the list-view with symbolic ID of IDC_LIST and save the reference to
  -- it in the exposed sLC variable.
  sLC = self~getListControl(IDC_LIST)

  -- Add 12 rows of 3 columns to the main list-view.
  sLC~InsertColumn(0, "Occupation", 80)
  sLC~InsertColumn(1, "Name", 50)
  sLC~InsertColumn(2, "Employer", 70)
  sLC~addRow(2, ,  "Business Manager", "Tom", "Qualcom")
  sLC~addRow(3, ,  "Software Developer and All Around Great Guy", "Sam", "AMCC")
  sLC~addRow(4, ,  "Accountant", "Cienna", "Bank of America")
  sLC~addRow(5, ,  "Waitress", "Adam", "The Hausenhof")
  sLC~addRow(6, ,  "Salesman", "Frank", "Lee White Toyota")
  sLC~addRow(7, ,  "Solider", "Nancy", "United State Marine Corp")
  sLC~addRow(8, ,  "Astronaut", "Vail", "United States Air Force")
  sLC~addRow(9, ,  "Receptionist", "Jayline", "Western Dental")
  sLC~addRow(10, , "Clerk", "Victor", "Circle K")
  sLC~addRow(11, , "Mechanic", "Woody", "Frank's Garage")
  sLC~addRow(12, , "Dentist", "Beth", "Western Dental")
  sLC~addRow(13, , "Tree Trimmer ", "Myra", "Eastern Garden Services")
  sLC~addRow(14, , "Pathologist", "Ned", "Mercy Hospital")

  -- Add these extended styles to the control.
  myStyles = "CHECKBOXES FULLROWSELECT HEADERDRAGDROP GRIDLINES"
  otherStyles = "DOUBLEBUFFER LABELTIP INFOTIP"
  sLC~addExtendedStyle(myStyles otherStyles)

  -- Get a reference to the ListBox control.  Text that could be sent to the
  -- console is put into this list box to display what is happening.  A sort of
  -- log of the execution.
  lb = self~getListBox(IDC_LB_EVENTS)

  -- If the horizontal extent is not set, the horziontal scroll bar will not
  -- scroll when long strings are inserted.
  --
  -- ooDialog does not provide a specific method to do this, (an oversight on
  -- the designer's part,)  But, there is a method provided to send any message
  -- to a dialog item.  This of course requires that the programmer understands
  -- the underlying Windows API.  The horizontal extent is set by sending the
  -- following message with a parameter of how wide to set the extent.  The
  -- second parameter must be 0.
  --
  -- Message: LB_SETHORIZONTALEXTENT == 0x00000194
  -- WPARAM:  Width in pixels -> 800
  -- LPARM:   Must be 0
  ret = self~sendMessageToItem(IDC_LB_EVENTS, "0x00000194", 800, 0)
  self~lbAdd("Send Message: LB_SETHORIZONTALEXTENT rc:" ret)

  -- The hover time is the amount of time, in milliseconds, that a mouse
  -- 'hovering' over an itme should elapse before an action is taken.  This
  -- effects tool tips, info tips, track select.  This changes the time from the
  -- default system time to 2 seconds.
  oldTime = sLC~setHoverTime(2000)
  self~lbAdd("Hover time was:   " oldTime)
  self~lbAdd("Hover time now is:" sLC~getHoverTime)

  -- Connect methods to some different list-view events.  Here all the events
  -- that can be displayed in the dialog are connected.  What is actually
  -- printed is controlled by check boxes in the dialog.  All the events are
  -- received, the program uses the check box state to filter what is displayed.

  ret = self~connectListViewNotify(IDC_LIST, "SelectChanged")
  ret = self~connectListViewNotify(IDC_LIST, "CheckboxChanged")
  ret = self~connectListViewNotify(IDC_LIST, "FocusChanged")
  ret = self~connectListViewNotify(IDC_LIST, "SelectFocus")
  ret = self~connectListViewNotify(IDC_LIST, "Click", "onMouseClick")

  self~getEditControl(IDC_EDIT_CHKBOX)~setCue(" Enter row numbers to act on")

-- End initDialog()


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  The next 5 methods are for the list-view notification events, that were
  'connected' through the connectListViewNotify method call in initDialog.

  Note the arguments that are received by the methods.  The programmer can rely
  on getting these arguments each time the method executes.

  Note also that for each method, the first argument is the resource ID of the
  dialog control that sent the notification message.  Because of this, if you
  had a dialog with more than one list-view control, you could connect each
  list-view to the same event method.  Then, when the method was called, you
  would check the id argument to determine which list-view control sent the
  message.  Something like the following code snippet:

  ::method onMouseClick
    use arg id, row, column, keyDown

    if id == self~constDir[IDC_LIST_DIRS] then do
      <do something specific for this control>
    end
    else if id == self~constDir[IDC_LIST_SERVERS] then do
      <do something with the servers list-view>
    end
    ...

  On the other hand, because the resource ID is used to filter the messages in
  the underlying C functions, you could also connect each list-view to separate
  event methods.  Something like this snippet:

  ...

  self~connectListViewNotify(IDC_LIST_DIRS, "Click", "onMouseClickDirs")
  self~connectListViewNotify(IDC_LIST_SERVERS, "Click", "onMouseClickServers")

  ...

  ::method onMouseClickDirs
    use arg id, row, column, keyDown

    <do something specific for the dirs list-view>
  ...

  ::method onMouseClickServers
    use arg id, row, column, keyDown

    <do something specific for the servers list-view>
  ...

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


-- onSelectFocus( id, row, status )
--
-- Called when the selection or focus state has changed.
--
-- Args:
--  id      The resource id of the dialog control (list-view control.)
--  row     The 0-based index of the row that changed.
--  status  What the new state is, some combination of SELECTED, UNSELECTED,
--          FOCUSED, or UNFOCUSED.
::method onSelectFocus
  expose chkSelFoc
  use arg id, row, status

  if \ chkSelFocus~isInstanceOf(.CheckControl) then
    chkSelFocus = self~getCheckControl(IDC_CHECK_EVENTSELFOC)

  if chkSelFocus~isChecked == "CHECKED" then
  	self~lbAdd("Row:" row~right(2) "select focus ->" status)

-- End onSelectFocus( id, row, status )


-- onCheckboxChanged( id, row, status )
--
-- Called when the an item's checkbox is check or unchecked.
--
-- Args:
--  id      The resource id of the dialog control (list-view control.)
--  row     The 0-based index of the row that changed.
--  status  If the checkbox is now checked or unchecked, will be either CHECKED
--          or UNCHECKED.
::method onCheckboxChanged
  expose chkCheckbox
  use arg id, row, status

  if \ chkCheckbox~isInstanceOf(.CheckControl) then
    chkCheckbox = self~getCheckControl(IDC_CHECK_EVENTCHK)

  if chkCheckbox~isChecked == "CHECKED" then
    self~lbAdd("Row:" row~right(2) "checkbox -> " status)

-- End onCheckboxChanged( id, row, status )


-- onSelectChanged( id, row, status )
--
-- Called when the an item is selected or unselected.
--
-- Args:
--  id      The resource id of the dialog control (list-view control.)
--  row     The 0-based index of the row that was selected or unselected.
--  status  If the row is selected or unselected, values could be: SELECTED
--          or UNSELECTED.
::method onSelectChanged
  expose chkSelect
  use arg id, row, status

  if \ chkSelect~isInstanceOf(.CheckControl) then
    chkSelect = self~getCheckControl(IDC_CHECK_EVENTSEL)

  if chkSelect~isChecked = "CHECKED" then
    self~lbAdd("Row:" row~right(2) "selection ->" status)

-- End onSelectChanged( id, row, status )


-- onFocusChanged( id, row, status )
--
-- Called when the an item gains or loses the focus
--
-- Args:
--  id      The resource id of the dialog control (list-view control.)
--  row     The 0-based index of the row that gained or lost focus.
--  status  If the row now has, or doesn't have the focus, values could be:
--          FOCUSED or UNFOCUSED.
::method onFocusChanged
  expose chkFocus
  use arg id, row, status

  if \ chkFocus~isInstanceOf(.CheckControl) then
    chkFocus = self~getCheckControl(IDC_CHECK_EVENTFOC)

  if chkFocus~isChecked = "CHECKED" then
    self~lbAdd("Row:" row~right(2) "focus ->" status)


-- onMouseClick( id, row, column, keyDown )
--
-- Called when a left mouse click happens on the list-view control.
--
-- Args:
--  id       The resource id of the dialog control (list-view control.)
--  row      The row number where the click occurred.
--  column   The column number where the click occurred.
--  keyDown  If the control, alt, or shift key was held donw during the mouse
--           click.  Values could be:  CONTROL, SHIFT, ALT, or NONE.
::method onMouseClick
  expose chkMouse
  use arg id, row, column, keyDown

  if \ chkMouse~isInstanceOf(.CheckControl) then
    chkMouse = self~getCheckControl(IDC_CHECK_EVENTMOUSE)

  if chkMouse~isChecked == "CHECKED" then
  	self~lbAdd("Mouse click -> Row:" row~right(2) "column:" column "key down:" -
               keyDown)

-- End onMouseClick( id row, column, keyDown )


-- onExecute()
--
-- Called when the user presses the Execute button in the Checkboxes group.
-- Carry out the action specified by the radio / checkbox buttons.
--
-- This method seems complicated because it has a lot in it, but it is really
-- just data validation.  Making sure the user has set the radio buttons and
-- the text in the edit control correctly.  If the edit control text is not
-- correct, a visual help is put up explaining what is wrong.  This 'balloon
-- tip' is a new feature of the Edit control, now available in ooDialog.
::method onExecute
  expose sLC rbCheck rbUncheck rbGet checkAll editChk rbIs

  if \ rbCheck~isInstanceOf(.RadioButton) then do
    rbCheck   = self~getRadioControl(IDC_RB_CHK_SET)
    rbUncheck = self~getRadioControl(IDC_RB_CHK_UN)
    rbGet     = self~getRadioControl(IDC_RB_CHK_GET)
    rbIs      = self~getCheckControl(IDC_RB_CHK_IS)
    checkAll  = self~getCheckControl(IDC_CHECK_DOALL)
    editChk   = self~getEditControl(IDC_EDIT_CHKBOX)
  end

  items = self~getEntryLine(IDC_EDIT_CHKBOX)

  all = checkAll~isChecked == "CHECKED"
  doGet = rbGet~isChecked == "CHECKED"
  doIs  = rbIs~isChecked  == "CHECKED"

  self~setEntryLine(IDC_EDIT_CHKBOX, "")

  select
    when rbCheck~isChecked == "CHECKED" & all then do
      ret = sLC~checkAll
      self~setEntryLine(IDC_EDIT_CHKBOX, "checkAll returns:" ret)
    end

    when rbCheck~isChecked == "CHECKED" then do
      if items~words == 0 then
        return self~doBalloonTip(editChk, 'check')

      msg = "returns:"
      do i = 1 to items~words
        n = items~word(i)
        if \ n~datatype('W') then
          return self~doBalloonTip(editChk, 'check', '"'n'"')

        ret = sLC~check(items~word(i))
        msg = msg ret
      end
      self~setEntryLine(IDC_EDIT_CHKBOX, msg)
    end
    -- End when rbCheck~isChecked == "CHECKED"

    when rbUncheck~isChecked == "CHECKED" & all then do
      ret = sLC~uncheckAll
      self~setEntryLine(IDC_EDIT_CHKBOX, "uncheckAll returns:" ret)
    end

    when rbUncheck~isChecked == "CHECKED" then do
      if items~words == 0 then
        return self~doBalloonTip(editChk, 'uncheck')

      msg = "returns:"
      do i = 1 to items~words
        n = items~word(i)
        if \ n~datatype('W') then
          return self~doBalloonTip(editChk, 'uncheck', '"'n'"')

        ret = sLC~uncheck(n)
        msg = msg ret
      end
      self~setEntryLine(IDC_EDIT_CHKBOX, msg)
    end
    -- End when rbUncheck~isChecked == "CHECKED"

    when (doGet | doIs) & all then do
      count = sLC~items
      msg = ''
      do i = 0 to (count -1)
        if doGet then ret = sLC~getCheck(i); else ret = sLC~isChecked(i)
        msg = msg "row:" i "->" ret
      end
      self~setEntryLine(IDC_EDIT_CHKBOX, msg)
    end

    when doGet | doIs then do
      if doGet then tipWord = 'get'; else tipWord = 'is'
      if items~words == 0 then
        return self~doBalloonTip(editChk, tipWord)

      msg = ""
      do i = 1 to items~words
        n = items~word(i)
        if \ n~datatype('W') then
          return self~doBalloonTip(editChk, tipWord, '"'n'"')

        if doGet then ret = sLC~getCheck(n); else ret = sLC~isChecked(n)
        msg = msg "row:" n "->" ret
      end
      self~setEntryLine(IDC_EDIT_CHKBOX, msg)
    end
    -- End when rbCheck~isChecked == "CHECKED"

    otherwise self~setEntryLine(IDC_EDIT_CHKBOX, "Not implemented")
  end
  -- End select

-- End onExecute()


-- doBalloonTip(ec, type, title)
--
-- Shows a 'tip' over the edit control that is used to manipulate the checkbox
-- feature of the list-view control.  The tip is displayed in the form of a
-- balloon, and is a common feature of the XP GUI.  The tip is shown when there
-- is an error in the  way the edit control is used to help the user understand
-- how the dialog works.
--
-- Args:
--   ec     The edit control where the balloon is shown.
--   type   Just a flag to help generate the right message.
--   title  An aid to getting the title of the cue correct.
::method doBalloonTip
  use arg ec, type, title

  if arg(3, 'E') then title = title "is not a number."
  else title = "No item numbers specified in the edit control."

  type = type~left(1)~translate

  cue = "Enter the item number(s) in this edit control whose"

  select
    when type == 'G' then cue = cue "checked state you want to get."
    when type == 'I' then cue = cue "checked state you want to query."
    when type == 'C' then cue = cue || " checkbox you want to check."
    when type == 'U' then cue = cue || " checkbox you want to uncheck."
    otherwise do
      title = "Internal Application Logic Error"
      cue = "The code in the doBallonTip method has encountered an error. "    -
            "Please contact the developer of this software." -- A little humor.
    end
  end
  -- End select

  ret = ec~showBalloon(title, cue)

return ""
-- End doBalloonTip(ec, type, title)


-- onSetStyle()
--
-- Called when the user pushes the Set Style push button.  For each style check
-- box that is checked, add that style.  For each check box not checked, remove
-- that style.
::method onSetStyle
  expose sLC

  -- Note that here I use a slightly different technique with the checkbox
  -- controls.  Instead of getting a reference to each control and saving it in
  -- an exposed variable, I re-acquire a reference every time the method
  -- executes.  This is for the purpose of example, normally in a program I
  -- would use one technique or the other throughout.

  removeStyle = ''
  addStyle    = ''

  -- Simply check the state of every checkbox and put the associated style in
  -- the appropriate variable.  Tedious, but straight forward.
  if self~getCheckControl(IDC_CHECK_CHECKBOXES)~isChecked = "CHECKED" then
    addStyle = addStyle "CHECKBOXES"
  else
    removeStyle = removeStyle "CHECKBOXES"

  if self~getCheckControl(IDC_CHECK_FULLROW)~isChecked = "CHECKED" then
    addStyle = addStyle "FULLROWSELECT"
  else
    removeStyle = removeStyle "FULLROWSELECT"

  if self~getCheckControl(IDC_CHECK_BORDER)~isChecked = "CHECKED" then
    addStyle = addStyle "BORDERSELECT"
  else
    removeStyle = removeStyle "BORDERSELECT"

  if self~getCheckControl(IDC_CHECK_GRIDLINES)~isChecked = "CHECKED" then
    addStyle = addStyle "GRIDLINES"
  else
    removeStyle = removeStyle "GRIDLINES"

  if self~getCheckControl(IDC_CHECK_UNDERLINEHOT)~isChecked = "CHECKED" then
    addStyle = addStyle "UNDERLINEHOT"
  else
    removeStyle = removeStyle "UNDERLINEHOT"

  if self~getCheckControl(IDC_CHECK_ONECLICK)~isChecked = "CHECKED" then
    addStyle = addStyle "ONECLICKACTIVATE"
  else
    removeStyle = removeStyle "ONECLICKACTIVATE"

  if self~getCheckControl(IDC_CHECK_FLATSB)~isChecked = "CHECKED" then
    addStyle = addStyle "FLATSB"
  else
    removeStyle = removeStyle "FLATSB"

  if self~getCheckControl(IDC_CHECK_INFOTIP)~isChecked = "CHECKED" then
    addStyle = addStyle "INFOTIP"
  else
    removeStyle = removeStyle "INFOTIP"

  if self~getCheckControl(IDC_CHECK_REGIONAL)~isChecked = "CHECKED" then
    addStyle = addStyle "REGIONAL"
  else
    removeStyle = removeStyle "REGIONAL"

  if self~getCheckControl(IDC_CHECK_SUBITEM)~isChecked = "CHECKED" then
    addStyle = addStyle "SUBITEMIMAGES"
  else
    removeStyle = removeStyle "SUBITEMIMAGES"

  if self~getCheckControl(IDC_CHECK_TRACK)~isChecked = "CHECKED" then
    addStyle = addStyle "TRACKSELECT"
  else
    removeStyle = removeStyle "TRACKSELECT"

  if self~getCheckControl(IDC_CHECK_LABELTIP)~isChecked = "CHECKED" then
    addStyle = addStyle "LABELTIP"
  else
    removeStyle = removeStyle "LABELTIP"

  if self~getCheckControl(IDC_CHECK_TWOCLICK)~isChecked = "CHECKED" then
    addStyle = addStyle "TWOCLICKACTIVATE"
  else
    removeStyle = removeStyle "TWOCLICKACTIVATE"

  if self~getCheckControl(IDC_CHECK_DOUBLEBUFFER)~isChecked = "CHECKED" then
    addStyle = addStyle "DOUBLEBUFFER"
  else
    removeStyle = removeStyle "DOUBLEBUFFER"

  if self~getCheckControl(IDC_CHECK_HEADERDRAGDROP)~isChecked = "CHECKED" then
    addStyle = addStyle "HEADERDRAGDROP"
  else
    removeStyle = removeStyle "HEADERDRAGDROP"

  if self~getCheckControl(IDC_CHECK_MULTIWORK)~isChecked = "CHECKED" then
    addStyle = addStyle "MULTIWORKAREAS"
  else
    removeStyle = removeStyle "MULTIWORKAREAS"

  if self~getCheckControl(IDC_CHECK_SIMPLE)~isChecked = "CHECKED" then
    addStyle = addStyle "SIMPLESELECT"
  else
    removeStyle = removeStyle "SIMPLESELECT"

  if self~getCheckControl(IDC_CHECK_UNDERLINECOLD)~isChecked = "CHECKED" then
    addStyle = addStyle "UNDERLINECOLD"
  else
    removeStyle = removeStyle "UNDERLINECOLD"

  -- Now change the styles.  While working with this sample, I noticed that the
  -- list-view control's appearance is sometimes a little ragged after changing
  -- the styles.  Forcing the control to repaint entirely helps eliminate that
  -- problem.
  sLC~replaceExtendedStyle(removeStyle, addStyle)
  ret = self~redrawWindow(sLC~hwnd)

-- End onSetStyle()


-- onGetStyle()
--
-- Called when the user pushes the Get Style push button.  Get the current
-- exteneded style of the list-view control and display it in the list box.
::method onGetStyle
  expose sLC lb

  -- Display the string and the numeric versions of the style.
  --
  -- Note that the Window styles value is a 32 bit number.  The default numeric
  -- digits of 9 is not large enough to hold this number for all Window styles.
  -- You need 11 digits.  Although in this specific case, the list-view
  -- extended styles, the actual values are small enough to fit with numeric
  -- digits 9.   However, in many cases the value will not fit, and it can be a
  -- subtle bug to understand.  So it is worth pointing this out.
  numeric digits 11
  self~lbAdd('Style:' sLC~getExtendedStyle)
  self~lbAdd('Style: 0x' || sLC~getExtendedStyleRaw~d2x~right(8, '0'))

-- End onGetStyle()


-- onClear()
--
-- Called when the user pushes the Clear Events button.  Delete all items from
-- the list box.
::method onClear
  expose lb
  lb~deleteAll

-- End onClear()


-- lbAdd( line )
--
-- Add a row to the list box.
--
-- Args:
--   line   The text of the row.
::method lbAdd
  expose lb
  use arg line

  -- Insert at the end of the items; automatically scroll as the list box gets
  -- filled.  Since the Windows ListBox control is not designed to handle a lot
  -- of items, before we reach 1000 items start deleting the oldest items.
  items = lb~items + 1
  lb~insert(items, line)
  if items > 10 then lb~makeFirstVisible(items - 8)
  if items > 998 then lb~delete(1)

-- End lbAdd( line )


-- ok()
--
-- Called when the user clicks the OK button.  This will display, or not, a list
-- of the selected rows at the time of exit.  A check box in the dialog controls
-- whether or not the selected rows are displayed.
::method ok
  expose sLC onExit

  if \ onExit~isInstanceOf(.CheckBox) then
    onExit = self~getCheckControl(IDC_CHECK_ONEXIT)

  -- If the user is tired of the last InfoDialog displaying each time they quit
  -- and unchecks the display selected items checkbox, then just exit.
  if onExit~isChecked \== "CHECKED" then do
    self~ok:super
    self~finished = 1
    return self~finished
  end

  -- The show selection on exit checkbox is checked, put up a message box
  -- showing what is selected at this point.
  eol = .endOfLine

  msg = "Is this the correct selection?" || eol || eol
  count = sLC~selectedItems
  if count > 0 then
    msg = msg "Selected:"
  else
    msg = "There are no items selected"

  if count > 0 then do
    item = sLC~selected
    selectedText = item
    do i = 2 to count
      item = sLC~nextSelected( item )
      selectedText = selectedText item
    end
    msg = msg selectedText || eol
  end

  ret = AskDialog(msg)
  junk = self~ok:super

-- End ok()

/* - - - End Of File: ListView.rex- - - - - - - - - - - - - - - - - - - - - - */
