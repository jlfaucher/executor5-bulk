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

/** activatedWindows.frm
 * Contains common definitions and routines used by MonitorWindows and
 * ManageMonitor.
 */

  -- These numbers are defined in the C code for the external function library,
  -- activatedHook.dll.  We put them into the .local directory and use symbolic
  -- ids for readability.
  .local~WM_USER_WINDOWACTIVATED         = 0x0501
  .local~SUCCESS_RC                      =  0
  .local~ERROR_BAD_HWND_RC               = -1
  .local~ERROR_HOOK_ALREADY_INSTALLED_RC = -2
  .local~ERROR_HOOK_NOT_INSTALLED_RC     = -3
  .local~ERROR_BAD_ARGS_RC               = -4

  -- Windows API window messages.
  .local~WM_QUERYOPEN                    = 0x0013

  -- These queues are ooRexx queues used to pass string data back and forth
  -- between the Monitor and the Manager.
  .local~MONITOR_HWND_QUEUE_NAME = "MONITOR_WINDOW_HANDLE_QUEUE"

  -- These user window messages are posted between the Monitor and Manager using
  -- the Windows API.  In a sense, they allow an ooDialog object to invoke a
  -- method on another ooDialog object in a different process space.
  .local~WM_USER_MONITORSHOW             = 0x0502

  -- This user message is sent to the monitor window as a command.  Parameter 1
  -- of the window message is sent as the code for what command to execute.
  -- Some commands take a second paramter to qualify the command.
  .local~WM_USER_MONITOR_CMD             = 0x0503
  .local~MONITOR_HIDE     = 1
  .local~MONITOR_SAVE     = 2
    .local~SAVE_LOG         = 21
    .local~SAVE_STATS       = 22
  .local~MONITOR_REPORT   = 3
  .local~MONITOR_PRINT    = 4
    .local~PRINT_LOG         = 41
    .local~PRINT_STATS       = 42
  .local~MONITOR_STATUS   = 5
  .local~MONITOR_CLOSE    = 6

  -- The default option values when no properties file exists.
  .local~PROP_FILE = "monitor.props"

  .local~MONITOR_HIDDEN = .true
  .local~MONITOR_LOG_ROTATE = .false
  .local~MONITOR_LOG_IN_DIR = .true
  .local~MONITOR_LOG_ALWAYS = .false
  .local~MONITOR_LOG_APPEND = .false
  .local~MONITOR_LOG_DIR = ""
  .local~MONITOR_LOG_NAME = "activated_windows.log"

  .local~REPORT_SORT = "id"  -- Can be id start title duration
  .local~REPORT_ASCENDING = .true
  .local~REPORT_LOG_IN_DIR = .true
  .local~REPORT_LOG_ROTATE = .false
  .local~REPORT_LOG_APPEND = .false
  .local~REPORT_LOG_PROMPT = .false
  .local~REPORT_LOG_DIR = ""
  .local~REPORT_LOG_NAME = "application_stats.log"

  .local~DEFAULT_SAVE_FILE  = "FIX_ME_FIX_ME"
  .local~PRINT_FILE = "Application_Usage_Trends.txt"


-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/** openQueue()
 * Sets the current external data queue to that named.  The queue is created if
 * necessary.
 *
 * @param   qName  REQUIRED  The queue name
 * @return  Returns true if the named queue already existed, otherwise false.
 */
::routine openQueue public
  use strict arg qName

  didExist = .false
  name = RxQueue("Create", qName)
  if name \== qName then do
    -- The queue already exists.  Delete the newly created queue.
    didExist = .true
    ret = RxQueue("Delete", name)
  end
  old = RxQueue("Set", qName)

return didExist

/** monitorIsRunning()
 * Returns true if the Monitor is running, otherwise false.  This function takes
 * some extra steps to ensure the Monitor is actually running.  When the Monitor
 * starts it puts its dialog handle and process ID in a named external queue and
 * empties the queue when it quits.  If the queue is not empty, the Monitor
 * should be running.  However, it is possible that something unexpected
 * happened and the queue was not emptied properly.
 */
::routine monitorIsRunning public

  info = readMonitorRunningQueue()
  if info == .nil then return .false

  parse var info hwnd pid
  hwndIsGood = .false
  pidIsGood  = .false
  loadedFunc = .false

  if RxFuncQuery("SendWinMsg") \== 0 then do
    loadedFunc = .true
    ret = RxFuncAdd("SendWinMsg", "oodialog", "SendWinMsg")
  end

  ret = SendWinMsg("ANY", hwnd, .WM_QUERYOPEN, 0, 0)
  if ret == 1 then hwndIsGood = .true

  wmi = .OLEObject~getObject("WinMgmts:")

  do p over wmi~instancesOf("win32_process")
    if p~processid == pid, p~name~caselessAbbrev('rexx') then do
      pidIsGood = .true
      leave
    end
  end

  if loadedFunc then ret = RxFuncDrop("SendWinMsg")

return hwndIsGood & pidIsGood

/** readMonitorRunningQueue()
 * Reads and returns the information from the special Monitor is running queue.
 * .nil is returned if the queue is empty.  Otherwise a string is returned in
 * the form:  hwnd pid.
 */
::routine readMonitorRunningQueue public

  oldQ = RxQueue('G')
  line = .nil

  j = openQueue(.MONITOR_HWND_QUEUE_NAME)

  -- The queue should always contain two entries, or be empty.  Each entry
  -- consists of the Monitor dialog handle and its process ID.  The entries are
  -- placed in the queue by the Monitor as soon as its dialog window is created,
  -- and removed when it closes.  If the queue is not empty, after reading an
  -- entry, the entry is put back on the queue so that the queue always remains
  -- in the state that the Monitor has put it.

  if queued() > 0 then do
    pull line
    queue line
  end

  j = RxQueue('S', oldQ)

return line

/** readMonitorHandle()
 * Reads and returns the MonitorWindows dialog handle.  .nil is returned if the
 * Monitor has not posted its handle.  (Which would imply that the Monitor is
 * not running.)
 */
::routine readMonitorHandle public

  line = readMonitorRunningQueue()
  if line == .nil then hwnd = .nil
  else parse var line hwnd .

return hwnd

/** readMonitorPID()
 * Reads and returns the MonitorWindows process ID.  .nil is returned if the
 * Monitor has not posted its process ID.  (Which would imply that the Monitor
 * is not running.)
 */
::routine readMonitorPID public

  line = readMonitorRunningQueue()
  if line == .nil then pid = .nil
  else parse var line hwnd pid

return pid

/**formatAsHex()
 * Returns a decimal number formatted as a 32-bit hexidecimal string.  Leading
 * zeros are uses so that the string is a constant width of 10 characters.  The
 * string always starts with '0x'
 */
::routine formatAsHex public
  use strict arg number
  return "0x" || number~d2x~right(8, "0")


::class 'ComparableProperties' public subclass Properties

::method load class

  forward class (super) continue
  other = result

  p = self~new
  p~copyFrom(other)
  return p

::method isEqualTo
  use strict arg other
  if \ other~isA(.Properties) then return .false

  itr = self~supplier
  do while itr~available
    if other[itr~index] \== itr~item then return .false
    itr~next
  end

  return .true

::method copyFrom
  use strict arg other

  if \ other~isA(.Properties) then
    raise syntax 93.948 array("1 other", "Properites")

  itr = other~supplier
  do while itr~available
    self[itr~index] = itr~item
    itr~next
  end

::method print
  indices = self~allIndexes
  indices~sort
  do i over indices
    say i"="self[i]
  end
