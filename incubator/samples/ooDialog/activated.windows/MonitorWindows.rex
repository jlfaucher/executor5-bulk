/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

/** MonitorWindows.rex
 * An example program using the NoteActivatedWindows external function.
 */

  -- We expect to be executed through rexxhide.  If passed an argument on the
  -- command line, it is taken to be the name of a file to use as an output log.
  -- The default output stream is replaced with this log file.  Then, any say
  -- statements will go to the log file, rather than be lost.  This is really
  -- only of consequence if there is an unexpected error.
  if arg(1, 'E') then do
    name = date('S') || '_' || time('L') || '_' || arg(1)
    name = name~translate("_", ":")
    fsObj = .stream~new(name)
    fsObj~open('BOTH REPLACE SHAREREAD NOBUFFER')
    .local~setentry("output", .monitor~new(fsObj))
  end

  -- We try to enforce that only one instance of the Monitor be running at any
  -- one time.
  if monitorIsRunning() then do
    say 'Monitor is already running.  Going to abort.'
    return -2
  end

  -- Load the external function.
  if RxFuncQuery("NoteActivatedWindows") \== 0 then do
    ret = RxFuncAdd("NoteActivatedWindows", "activatedHook", "NoteActivatedWindows")
  end

  signal on any name removeHook

  dlg = .ActivatedWindows~new('Dialogs.dll', 'IDD_ACTIVATED', , 'resource.h')
  if dlg~initCode <> 0 then do
    say "Error creating dialog.  initCode:" dlg~initCode
    ret = RxFuncDrop("NoteActivatedWindows")
    return 99
  end

  dlg~Execute('HIDE INACTIVE')
  dlg~DeInstall
  ret = RxFuncDrop("NoteActivatedWindows")

  return 0

removeHook:
  -- If an unexpected error happens, we try to do our best to uninstall the
  -- hook, (because it is system global.)  We call the uninstall function of
  -- NoteActivatedWindows() and then print out the same message that would
  -- happen if we did not trap any errors.
  code = rc
  name = condition('C')
  line = sigl
  cObj = condition('O')
  if code \== "RC" then extra = 'error:' code name
  else extra = name

  say 'Caught an exception ('extra') on line:' line
  say 'Cleaning up ... IF available, more error information follows:'

  if RxFuncQuery("NoteActivatedWindows") == 0 then do
    j = NoteActivatedWindows(0, 'U')
    j = RxFuncDrop("NoteActivatedWindows")
  end

  if cObj \== .nil then do
    say
    if cObj~traceBack~isA(.list) then do line over cObj~traceBack
      say line
    end
    if cObj~rc \== .nil, cObj~program \== .nill, cObj~code \== .nil, cObj~message \== .nil then do
      say 'Error' cObj~rc 'running' cObj~program 'line' cObj~position":" cObj~errorText
      say 'Error' cObj~code cObj~message
    end
  end

  return cObj~rc
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires 'oodwin32.cls'
::requires 'activatedWindows.frm'


/* class: ActivatedWindows - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  The dialog class that receives notifications that the activation has changed
  to a different, top-level, unowned, window.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ActivatedWindows' public subclass ResDialog -
                           inherit AdvancedControls MessageExtensions DataOutputter

::method init
  expose windowTable startDate startTime prefs

  forward class (super) continue
  if result <> 0 then return result

  windowTable = .table~new
  startDate = date('S')
  startTime = time('L')

  prefs = self~getProperties

::method getProperties private

  p = .ComparableProperties~load(.PROP_FILE)
  if p~isEmpty then do
    p['isHidden']           = .MONITOR_HIDDEN
    p['rotateMonitorLog']   = .MONITOR_LOG_ROTATE
    p['monitorLogWithApp']  = .MONITOR_LOG_IN_DIR
    p['alwaysLog']          = .MONITOR_LOG_ALWAYS
    p['appendToMonitorLog'] = .MONITOR_LOG_APPEND
    p['monitorLogDir']      = .MONITOR_LOG_DIR
    p['monitorLogName']     = .MONITOR_LOG_NAME

    p['sortOrder']         = .REPORT_SORT
    p['sortAscending']     = .REPORT_ASCENDING
    p['reportLogWithApp']  = .REPORT_LOG_IN_DIR
    p['rotateReportLog']   = .REPORT_LOG_ROTATE
    p['appendToReportLog'] = .REPORT_LOG_APPEND
    p['alwaysPrompt']      = .REPORT_LOG_PROMPT
    p['reportLogDir']      = .REPORT_LOG_DIR
    p['reportLogName']     = .REPORT_LOG_NAME

    p['printFile'] = .PRINT_FILE

    dir = .ActivatedWindows~getApplicationDir
    if p['monitorLogWithApp'] then p['monitorLogDir'] = dir
    if p['reportLogWithApp'] then p['reportLogDir'] = dir

    p~save(.PROP_FILE)
  end
  return p

::method initDialog
  expose listControl lastActive currentRow startDate startTime

  self~connectButton(IDC_PB_REPORT, onReport)
  self~connectButton(IDC_PB_HIDE, onHide)
  self~connectButton(IDC_PB_PREFERENCES, onPreferences)
  self~connectButton(IDOK, onClose)

  msg = "Started Monitoring Windows on" date('N', startDate, 'S') 'at' time('C', startTime, 'L')
  self~getStaticControl(IDC_STARTED_DESC)~title = msg

  listControl = self~getListControl(IDC_LIST)
  lastActive = 0
  currentRow = -1

  listControl~InsertColumn(0, "Title", 210)
  listControl~InsertColumn(1, "Handle", 40)
  listControl~InsertColumn(2, "Start Time", 50)
  listControl~InsertColumn(3, "ElapsedTime", 50)

  listControl~addExtendedStyle("FULLROWSELECT GRIDLINES HEADERDRAGDROP LABELTIP DOUBLEBUFFER")

  ret = self~AddUserMsg(onWindowActivated, .WM_USER_WINDOWACTIVATED, 0xFFFFFFFF, 0, 0, 0, 0)

  ret = self~AddUserMsg(onMonitorShow, .WM_USER_MONITORSHOW, 0xFFFFFFFF, 0, 0, 0, 0)
  ret = self~AddUserMsg(onReceiveCmd, .WM_USER_MONITOR_CMD, 0xFFFFFFFF, 0, 0, 0, 0)

  ret = NoteActivatedWindows(self~dlgHandle, 'I')
  if ret <> 0 then self~displayError(ret)

  self~postHandle

::method onWindowActivated
  expose listControl lastActive currentRow startSpan windowTable
  use arg hwnd, isFullScreen

  now = .TimeSpan~new(time('F'))

  -- When the activated window changes to no window activated, the Windows API
  -- sends both parameters as 0.  In ooDialog, when it recieves both parameters
  -- as 0, it does not send any args to the method invoked. (This method.)
  -- Therefore, when and only when arg 1 is ommitted, there is no activated
  -- window.  You can see this when the user clicks on the Start Menu or the
  -- Task Bar.
  if arg(1, 'O') then do
    title = "<none>"
    handle = "<N/A>"
    hwnd = 0
  end
  else do
    title = self~windowTitle(hwnd)
    handle = formatAsHex(hwnd)
  end

  -- When the activated window is change by the user clicking on an icon on the
  -- task bar, it seems as though the system sends two messages.  One for the
  -- window losing the activation and then one for the window gainging the
  -- activation.  This does not happen when the activation is changes by the
  -- user clicking on the window, or using the Alt-Tab task switcher.  So, some
  -- code to take this into account.
  if hwnd == lastActive then return

  endSpan = now

  if currentRow <> -1 then do
    duration = endSpan - startSpan
    listControl~setItemText(currentRow, 3, duration~string)
    windowTable[formatAsHex(lastActive)]~addTo(duration)
  end
  startTime = time('L')
  currentRow = listControl~addRow(, , title, handle, startTime)
  listControl~ensureVisible(currentRow)

  wndID = formatAsHex(hwnd)
  if windowTable[wndID] == .nil then
    windowTable[wndID] = .Accumulator~new(handle, title, startTime)

  startSpan = now
  lastActive = hwnd

::method onClose
  self~removeHook
  self~shutDown
  return self~ok:super

::method cancel
  self~removeHook
  return self~cancel:super

::method onReport
  expose windowTable prefs

  id = self~constDir[IDD_STATS]
  report = .StatsWindow~new('Dialogs.dll', id, 'resource.h', windowTable~copy, self, prefs)
  self~show("HIDE")
  report~popupAsChild(self, "SHOWTOP")

::method onPreferences
  expose prefs

  id = self~constDir[IDD_PREFERENCES]
  prefDlg = .PreferenceWindow~new('Dialogs.dll', id, 'resource.h', prefs)
  self~show("HIDE")
  prefDlg~execute("SHOWTOP")

::method onMonitorShow
  use arg param1, param2
  self~show("SHOWTOP")

::method onHide
  self~show("HIDE")

::method onReceiveCmd
  use arg cmd, cmdOption

  -- Not many implemented.
  select
    when cmd == .MONITOR_CLOSE then return self~onClose
    when cmd == .MONITOR_REPORT then self~onReport
    when cmd == .MONITOR_SAVE then self~doSave(cmdOption)

    otherwise nop
  end
  -- End select

::method getCurrentStats
  expose windowTable
  return windowTable~copy

::method doSave
  expose windowTable
  use arg which

  data = windowTable~allItems
  sorter = .AccumulatorSorter~new("start", .true)
  data~sortWith(sorter)

  ret = self~writeStatsToFile(data, .DEFAULT_SAVE_FILE, .false)


::method shutDown private
  self~removeHook
  self~emptyHwndQueue

::method postHandle private

  if openQueue(.MONITOR_HWND_QUEUE_NAME) then self~emptyHwndQueue

  hwnd = self~dlgHandle
  pid  = SysQueryProcess("PID")

  queue hwnd pid
  queue hwnd pid

::method emptyHwndQueue private
  -- Assumes the queue is set already.
  do queued()
    pull .
  end

::method removeHook private
  ret = NoteActivatedWindows(self~hwnd, 'U')

::method displayError private
  expose listControl
  use strict arg errCode

  listControl~addRow(, , "Monitor not installled")
  listControl~disable

  msg = "Error installing the activated window monitor." || .endOfLine  || -
        "  Return code:" errCode || .endOfLine
  select
    when errCode == .ERROR_BAD_HWND_RC then
      msg = msg || "  The dialog window handle is not recognized."

    when errCode == .ERROR_HOOK_ALREADY_INSTALLED_RC then
      msg = msg || "  The monitor can not be installed because it was not previosly removed correctly."

    when errCode == .ERROR_BAD_ARGS_RC then
      msg = msg || "  The arguments to NoteActivatedWindows() are not correct."

    when errCode > 0 then
      msg = msg || "  The return code is a system error code.  Look up its meaning" || .endOfLine || -
                   "  in the Windows documentation."

    otherwise
      msg = msg || "  Unrecognized error code."
  end
  -- End select

  j = errorDialog(msg)

::attribute startTime get
::attribute startTime set private

::attribute startDate get
::attribute startDate set private

::method init class
  expose dir
  parse source . . fileSpec
  dir = fileSpec('D', fileSpec) || fileSpec('P', fileSpec)
  dir = dir~strip('T', '\')

::method getApplicationDir class
  expose dir
  return dir


::class "DataOutputter" public mixinclass Object

::method writeStatsToFile
  use strict arg stats, fileName, forPrinter = .true

  fsObj = .stream~new(fileName)
  fsObj~open("WRITE REPLACE")
  if fsObj~state \== "READY" then return -1

  fsObj~lineout("")
  fsObj~lineout("Application Times"~center(80)~strip('T'))
  fsObj~lineout("")
  fsObj~lineout("  Title"~left(32) " Start"~left(14) " Handle"~left(12) " Elapsed"~left(14))
  fsObj~lineout("-"~copies(76))
  do stat over stats
    line = self~formatForPrint(stat)
    fsObj~lineout(line)
  end
  fsObj~close

  return 0

::method formatForPrint private
  use arg stat
  in = "  "
  t = stat~title~left(27)
  if stat~title~length > 27 then t = t || "..."
  else t = t || "   "

  line = in || t || in || stat~startTime~left(13) || in || -
         stat~handle~left(11) || in || stat~duration~string~left(13)
  return line


::class "PreferenceWindow" public subclass ResDialog inherit AdvancedControls MessageExtensions

::method init
  expose prefs appDir
  use strict arg dll, id, hFile, prefs

  resArgs = .array~new(4)
  resArgs[1] = dll
  resArgs[2] = id
  resArgs[4] = hFile

  forward class (super) arguments (resArgs) continue
  if result <> 0 then return result

  appDir = .ActivatedWindows~getApplicationDir

  self~noAutoDetection
  return 0


::method initDialog
  expose prefs editFolder_M editFolder_R editLog_M editLog_R pbBrowse_M pbBrowse_R  -
  chkHidden chkRotate chkLogInDir rbContinous rbAppend rbAscending                  -
  chkLogInDir_R chkRotate_R chkAppend chkPrompt rbTitle rbStart rbDuration pbApply

  pbApply = self~getButtonControl(IDC_PB_APPLY)
  self~updateApplyButton(.false)
  self~connectButton(IDC_PB_APPLY, onApply)

  chkHidden   = self~getCheckControl(IDC_CHECK_HIDE)
  chkRotate   = self~getCheckControl(IDC_CHECK_ROTATE)
  chkLogInDir = self~getCheckControl(IDC_CHECK_LOG_IN_FOLDER)
  if prefs['isHidden'] then chkHidden~check
  if prefs['rotateMonitorLog'] then chkRotate~check
  if prefs['monitorLogWithApp'] then chkLogInDir~check

  rbContinous = self~getRadioControl(IDC_RB_CONTINOUS)
  if prefs['alwaysLog'] then rbContinous~check
  else self~getRadioControl(IDC_RB_ONSAVE)~check

  rbAppend = self~getRadioControl(IDC_RB_APPEND)
  if prefs['appendToMonitorLog'] then rbAppend~check
  else self~getRadioControl(IDC_RB_OVERWRITE)~check

  rbTitle    = self~getRadioControl(IDC_RB_TITLE_R)
  rbStart    = self~getRadioControl(IDC_RB_START_R)
  rbDuration = self~getRadioControl(IDC_RB_DURATION_R)
  order      = prefs['sortOrder']
  select
    when order == 'title' then rbTitle~check
    when order == 'start' then rbStart~check
    when order == 'duration' then rbDuration~check
    otherwise self~getRadioControl(IDC_RB_HWND_R)~check
  end
  -- End select

  rbAscending = self~getRadioControl(IDC_RB_ASCENDING_R)
  if prefs['sortAscending'] then rbAscending~check
  else self~getRadioControl(IDC_RB_DESCENDING_R)~check

  chkLogInDir_R = self~getCheckControl(IDC_CHECK_LOG_IN_FOLDER_R)
  chkRotate_R   = self~getCheckControl(IDC_CHECK_ROTATE_R)
  chkAppend     = self~getRadioControl(IDC_CHECK_APPEND_R)
  chkPrompt     = self~getRadioControl(IDC_CHECK_PROMPT_R)
  if prefs['reportLogWithApp'] then chkLogInDir_R~check
  if prefs['rotateReportLog'] then chkRotate_R~check
  if prefs['appendToReportLog'] then chkAppend~check
  if prefs['alwaysPrompt'] then chkPrompt~check

  editFolder_M = self~getEditControl(IDC_FOLDER_EDIT)
  editFolder_R = self~getEditControl(IDC_FOLDER_EDIT_R)

  editLog_M = self~getEditControl(IDC_LOG_EDIT)
  editLog_R = self~getEditControl(IDC_LOG_EDIT_R)

  pbBrowse_M = self~getButtonControl(IDC_PB_FOLDER_BROWSE)
  pbBrowse_R = self~getButtonControl(IDC_PB_FOLDER_BROWSE_R)

  editFolder_M~setText(prefs['monitorLogDir'])
  editFolder_R~setText(prefs['reportLogDir'])
  editLog_M~setText(prefs['monitorLogName'])
  editLog_R~setText(prefs['reportLogName'])

  if prefs['monitorLogWithApp'] then self~disableFolderControls(IDC_CHECK_LOG_IN_FOLDER)
  if prefs['reportLogWithApp'] then self~disableFolderControls(IDC_CHECK_LOG_IN_FOLDER_R)

  self~connectEditNotify(IDC_FOLDER_EDIT,   "CHANGE", onEditTextChange)
  self~connectEditNotify(IDC_FOLDER_EDIT_R, "CHANGE", onEditTextChange)
  self~connectEditNotify(IDC_LOG_EDIT,      "CHANGE", onEditTextChange)
  self~connectEditNotify(IDC_LOG_EDIT_R,    "CHANGE", onEditTextChange)

  self~connectButton(IDC_PB_FOLDER_BROWSE,   onBrowse)
  self~connectButton(IDC_PB_LOG_BROWSE,      onBrowse)
  self~connectButton(IDC_PB_FOLDER_BROWSE_R, onBrowse)
  self~connectButton(IDC_PB_LOG_BROWSE_R,    onBrowse)

  self~connectButtonNotify(IDC_CHECK_LOG_IN_FOLDER,   "CLICKED", onChangeFolder)
  self~connectButtonNotify(IDC_CHECK_LOG_IN_FOLDER_R, "CLICKED", onChangeFolder)

  self~connectButtonNotify(IDC_CHECK_HIDE,      "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_CHECK_ROTATE,    "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_CONTINOUS,    "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_ONSAVE,       "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_APPEND,       "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_OVERWRITE,    "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_START_R,      "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_DURATION_R,   "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_TITLE_R,      "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_HWND_R,       "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_ASCENDING_R,  "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_RB_DESCENDING_R, "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_CHECK_ROTATE_R,  "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_CHECK_APPEND_R,  "CLICKED", onButtonClicked)
  self~connectButtonNotify(IDC_CHECK_PROMPT_R,  "CLICKED", onButtonClicked)

::method onApply
  self~maybeUpdatePrefs
  self~maybeUpdateApply

::method onBrowse
  use arg id, hwnd

  symbol = self~getSymbolicID(id)
  if symbol == IDC_PB_FOLDER_BROWSE | symbol == IDC_PB_FOLDER_BROWSE_R then self~browseForFolder(symbol)
  else self~browseForFile(symbol)

::method browseForFolder private
  expose prefs editFolder_M editFolder_F
  use arg id

  -- If the directory in the edit box is a good directory, we will start there.
  -- Otherwise we start with the directory set in the properites object.
  if id == IDC_PB_FOLDER_BROWSE then do
    if SysIsFileDirectory(editFolder_M~getText) then dir = editFolder_M~getText
    else dir = prefs['monitorLogDir']
    if dir~right(1) \== '\' then dir = dir || '\'

    newDir = GetFileNameWindow(dir, self~dlgHandle, , , "Select Log Directory", "")
    say 'Got newDir:' newDir
  end

::method browseForFile


::method onButtonClicked
  self~maybeUpdateApply

::method onEditTextChange
  self~maybeUpdateApply

::method onChangeFolder
  use arg id, hwnd
  symbol = self~getSymbolicID(id)
  disable = (self~getCheckControl(id)~isChecked == "CHECKED")
  self~disableFolderControls(symbol, disable)
  self~maybeUpdateApply

::method ok
  if self~maybeUpdatePrefs == IDCANCEL then do
    msg = "Preferences were not saved." || .endOfLine~copies(2) || -
          "Continue to quit?"
    if yesNoMessage(msg) \== 1 then return
  end
  self~ok:super

::method maybeUpdatePrefs private
  expose prefs

  newPrefs = self~getCurrentPrefs
  if \ prefs~isEqualTo(newPrefs) then do
    if \ self~dirsAreGood(newPrefs) then return IDCANCEL
    prefs~copyFrom(newPrefs)
    prefs~save(.PROP_FILE)
    return IDOK
  end

  return IDOK

::method dirsAreGood private
  use arg newPrefs

  if \ self~checkDir(newPrefs['monitorLogDir'], "monitor") then return .false

  return self~checkDir(newPrefs['reportLogDir'], "report")

::method checkDir private
  use arg dir, qualifier

  if \ SysIsFileDirectory(dir) then do
    msg = "The directory for the" qualifier "log file does not exist." || .endOfLine~copies(2) || -
          "Do you want to create it?"
    if yesNoMessage(msg) \== 1 then return .false

    ret = SysMkDir(mFolder)
    if ret \== 0 then do
      msg = "The directory could not be created." || .endOfLine~copies(2) || -
            "Return code:" ret
      j = ErrorMessage(msg)
      return .false
    end
  end
  return .true

::method maybeUpdateApply private
  expose prefs

  enable = \ prefs~isEqualTo(self~getCurrentPrefs)
  self~updateApplyButton(enable)

::method getSymbolicID private
  use arg id
  return self~constDir~index(id)

::method disableFolderControls private
  expose editFolder_M editFolder_R pbBrowse_M pbBrowse_R appDir prefs
  use arg index, disable = .true

  if index == IDC_CHECK_LOG_IN_FOLDER then do
    edit = editFolder_M
    pb   = pbBrowse_M
    text = prefs['monitorLogDir']
  end
  else do
    edit = editFolder_R
    pb   = pbBrowse_R
    text = prefs['reportLogDir']
  end

  if disable then do
    edit~setText(appDir)
    edit~setReadOnly(.true)
    pb~disable
  end
  else do
    edit~setText(text)
    edit~setReadOnly(.false)
    pb~enable
  end

::method getCurrentPrefs private
  expose editFolder_M editFolder_R editLog_M editLog_R pbBrowse_M pbBrowse_R -
  chkHidden chkRotate chkLogInDir rbContinous rbAppend rbAscending           -
  chkLogInDir_R chkRotate_R chkAppend chkPrompt

  p = .ComparableProperties~new

  p['isHidden']           = (chkHidden~isChecked == "CHECKED")
  p['rotateMonitorLog']   = (chkRotate~isChecked == "CHECKED")
  p['monitorLogWithApp']  = (chkLogInDir~isChecked == "CHECKED")
  p['alwaysLog']          = (rbContinous~isChecked == "CHECKED")
  p['appendToMonitorLog'] = (rbAppend~isChecked == "CHECKED")
  p['monitorLogDir']      = editFolder_M~getText~strip~strip('T', '\')
  p['monitorLogName']     = editLog_M~getText~strip

  p['sortOrder']         = self~getSortOrder
  p['sortAscending']     = (rbAscending~isChecked == "CHECKED")
  p['reportLogWithApp']  = (chkLogInDir_R~isChecked == "CHECKED")
  p['rotateReportLog']   = (chkRotate_R~isChecked == "CHECKED")
  p['appendToReportLog'] = (chkAppend~isChecked == "CHECKED")
  p['alwaysPrompt']      = (chkPrompt~isChecked == "CHECKED")
  p['reportLogDir']      = editFolder_R~getText~strip~strip('T', '\')
  p['reportLogName']     = editLog_R~getText~strip

  p['printFile'] = .PRINT_FILE

  return p

::method updateApplyButton private
  expose pbApply
  use arg enable = .true
  if enable then pbApply~enable
  else pbApply~disable


::method getSortOrder private
  expose rbTitle rbStart rbDuration
  select
    when rbTitle~isChecked == "CHECKED" then return 'title'
    when rbStart~isChecked == "CHECKED" then return 'start'
    when rbDuration~isChecked == "CHECKED" then return 'duration'
    otherwise return 'id'
  end
  -- End select


::class "StatsWindow" public subclass ResDialog inherit AdvancedControls MessageExtensions DataOutputter

::attribute logFile private

::method init
  expose stats parent prefs
  use strict arg dll, id, hFile, windowTable, parent, prefs

  resArgs = .array~new(4)
  resArgs[1] = dll
  resArgs[2] = id
  resArgs[4] = hFile

  forward class (super) arguments (resArgs) continue
  if result <> 0 then return result

  self~noAutoDetection
  stats = windowTable~allItems

  sorter = .AccumulatorSorter~new(prefs['sortOrder'], prefs['sortAscending'])
  stats~sortWith(sorter)

  self~logFile = .stream~new(prefs['reportLogDir'] || '\' || prefs['reportLogName'])

  return 0

::method initDialog
  expose listControl stats rbStart rbTitle rbDuration rbHwnd rbAscending rbDescending chkPrompt prefs

  listControl = self~getListControl(IDC_LIST_STATS)

  listControl~InsertColumn(0, "Title", 230)
  listControl~InsertColumn(1, "Handle", 40)
  listControl~InsertColumn(2, "Start Time", 50)
  listControl~InsertColumn(3, "ElapsedTime", 50)

  listControl~addExtendedStyle("FULLROWSELECT GRIDLINES HEADERDRAGDROP LABELTIP DOUBLEBUFFER")

  self~connectButton(IDC_PB_REFRESH, onRefresh)
  self~connectButton(IDC_PB_SAVE, onSave)
  self~connectButton(IDC_PB_PRINT, onPrint)
  self~connectButton(IDOK, onDone)

  rbStart      = self~getRadioControl(IDC_RB_START)
  rbTitle      = self~getRadioControl(IDC_RB_TITLE)
  rbDuration   = self~getRadioControl(IDC_RB_DURATION)
  rbHwnd       = self~getRadioControl(IDC_RB_HWND)
  rbAscending  = self~getRadioControl(IDC_RB_ASCENDING)
  rbDescending = self~getRadioControl(IDC_RB_DESCENDING)
  chkPrompt    = self~getCheckControl(IDC_CHECK_NEW)

  if prefs['sortAscending'] then rbAscending~check
  else rbDescending~check

  order = prefs['sortOrder']
  select
    when order == 'title' then rbTitle~check
    when order == 'start' then rbStart~check
    when order == 'duration' then rbDuration~check
    otherwise rbHwnd~check
  end
  -- End select


  self~connectButtonNotify(IDC_RB_ASCENDING,  "CLICKED", onChangeSort)
  self~connectButtonNotify(IDC_RB_DESCENDING, "CLICKED", onChangeSort)
  self~connectButtonNotify(IDC_RB_START,      "CLICKED", onChangeSort)
  self~connectButtonNotify(IDC_RB_DURATION,   "CLICKED", onChangeSort)
  self~connectButtonNotify(IDC_RB_HWND,       "CLICKED", onChangeSort)
  self~connectButtonNotify(IDC_RB_TITLE,      "CLICKED", onChangeSort)

  -- Stats is aleady sorted.
  do stat over stats
    listControl~addRow(, , stat~title, stat~handle, stat~startTime, stat~duration~string)
  end

  if prefs['alwaysPrompt'] then chkPrompt~check
  self~getStaticControl(IDC_TXT_FILE)~title = self~logFile~qualify

::method onSave
  expose stats
  fileName = self~getSaveFileName

  sorter = .AccumulatorSorter~new(self~getSortField, self~isAscending)
  stats~sortWith(sorter)

  self~writeStatsToFile(stats, fileName, .false)

::method onRefresh
  expose stats parent

  wTable = parent~getCurrentStats
  stats = wTable~allItems

  -- Use the onChangeSort method to reload the list-view control. (The sort
  -- order is not really changed but this will update all the items.)
  self~onChangeSort

::method onPrint
  expose stats

  fileName = .PRINT_FILE

  sorter = .AccumulatorSorter~new(self~getSortField, self~isAscending)
  stats~sortWith(sorter)

  ret = self~writeStatsToFile(stats, fileName, .true)
  if ret \== 0 then do
    j = errorDialog("Error formatting the data to a file for printing." || .endOfLine || -
                    "The file could not be written, printing can not proceed.")
    return
  end

  -- Should trap OLE errors.
  fso = .oleObject~new("Scripting.FileSystemObject")
  file = fso~GetFile(fileName)
  path = fso~GetParentFolderName(file)

  shell = .oleObject~new("Shell.Application")
  folder = shell~Namespace(path)
  item = folder~ParseName(fileName)
  /* Could check that print is available.
  do verb over item~verbs
    say verb~name
  end
  */
  item~InvokeVerbEx("Print")

::method getSaveFileName private
  expose prefs

  -- TODO this is still all wrong need to check if use has chaged things!!!
  return self~logFile~qualify

::method onChangeSort
  self~sort(self~getSortField, self~isAscending)

::method sort private
  expose stats listcontrol
  use arg field, order

  sorter = .AccumulatorSorter~new(field, order)
  stats~sortWith(sorter)

  listControl~deleteAll
  do stat over stats
    listControl~addRow(, , stat~title, stat~handle, stat~startTime, stat~duration~string)
  end

::method getSortField private
  expose rbStart rbTitle rbDuration rbHwnd
  select
    when rbStart~isChecked == "CHECKED" then return "start"
    when rbTitle~isChecked == "CHECKED" then return "title"
    when rbDuration~isChecked == "CHECKED" then return "duration"
    otherwise return "id"
  end
  -- End select

::method isAscending private
  expose rbAscending
  return (rbAscending~isChecked == "CHECKED")

::method onDone
  self~ok:super


::class "Accumulator" inherit Comparable

::attribute id get
::attribute id set private

::attribute handle get
::attribute handle set private

::attribute title get
::attribute title set private

::attribute startTime get
::attribute startTime set private

::method init
  expose id duration handle title startTime
  use strict arg handle, title, startTime

  if handle = "<N/A>" then id = formatAsHex(0)
  else id = handle

  n = .TimeSpan~new(time('F'))
  duration = n - n

::method addTo
  expose duration
  use strict arg increment
  duration += increment

::method duration
  expose duration
  return duration

::method compareTo
  expose id
  use strict arg other
  return id~compareTo(other~id)

::class "AccumulatorSorter" inherit Comparator

::method init
  expose field ascending
  use strict arg field, ascending = .true

::method compare
  expose field
  use arg left, right
  select
    when field~caselessCompare("title") == 0 then return self~compareTitle(left, right)
    when field~caselessCompare("start") == 0 then return self~compareStart(left, right)
    when field~caselessCompare("duration") == 0 then return self~compareDuration(left, right)
    otherwise return self~compareID(left, right)
  end
  -- End select

::method compareStart
  expose ascending
  use arg left, right
  ret = left~startTime~compareTo(right~startTime)
  if \ ascending then ret = -ret
  return ret


::method compareTitle
  expose ascending
  use arg left, right
  ret = left~title~compareTo(right~title)
  if \ ascending then ret = -ret
  return ret


::method compareDuration
  expose ascending
  use arg left, right
  ret = left~duration~compareTo(right~duration)
  if \ ascending then ret = -ret
  return ret


::method compareID
  expose ascending
  use arg left, right
  ret = left~id~compareTo(right~id)
  if \ ascending then ret = -ret
  return ret


