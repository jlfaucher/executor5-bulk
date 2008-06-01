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

/** SimpleMonitor.rex
 * A simple program using the NoteActivatedWindows external function.
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

  -- Load the external function.
  if RxFuncQuery("NoteActivatedWindows") \== 0 then do
    ret = RxFuncAdd("NoteActivatedWindows", "activatedHook", "NoteActivatedWindows")
  end

  -- Catch Ctrl-c (or other errors) to remove the global hook.
  signal on any name removeHook

  -- Start the dialog.  If it does not initialize correctly, drop the external
  -- function and quit.
  dlg = .SimpleMonitor~new
  if dlg~initCode <> 0 then do
    say "Error creating dialog.  initCode:" dlg~initCode
    ret = RxFuncDrop("NoteActivatedWindows")
    return 99
  end

  -- The monitor dialog will not show on the screen.  It will run until Ctrl-C
  -- is used to halt it.
  dlg~Execute('HIDE INACTIVE')

  -- If an error was detected by the dialog in using the external function, it
  -- will quit.  The application code will contain the error code.  We check
  -- for that here.  But, most likely this program will end with Ctrl-C and we
  -- won't be here.  We will be in removeHook.
  ret = dlg~applicationCode
  j = displayError(ret)

  -- Drop all the external functions.
  dlg~DeInstall
  ret = RxFuncDrop("NoteActivatedWindows")

  return ret

removeHook:
  -- If an unexpected error happens, we try to do our best to uninstall the
  -- hook, (because it is system global.)  We call the uninstall function of
  -- NoteActivatedWindows() and then print out the same message that would
  -- happen if we did not trap any errors.

  -- Most likely this is just trapping the Ctrl-C used to end the program.
  code = rc
  name = condition('C')
  line = sigl
  cObj = condition('O')
  if code \== "RC" then extra = 'error:' code name
  else extra = name

  say 'Caught an exception ('extra') on line:' line
  say 'Cleaning up ... IF available, more error information follows:'
  say '(If error is on line 70, then it was just the ctrl-c trap.)'

  dlg~DeInstall
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

/* class: SimpleMonitor- - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  This the dialog class that receives notifications each time the activation has
  changed to a different, top-level, unowned, window.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'SimpleMonitor' public subclass UserDialog inherit AdvancedControls MessageExtensions

::method init
  self~init:super

  -- Size, etc of this dialog does not matter because it remains invicisble.
  ret = self~CreateCenter(200, 100, 'Simple Monitor')

  self~applicationCode = 0

::method initDialog

  -- This is the whole point.  We connect the onWindowActivted method in this
  -- dialog to the WM_USER_WINDOWACTIVATED message.  This message is posted to
  -- use each time a new window becomes the top activated window.
  ret = self~AddUserMsg(onWindowActivated, .WM_USER_WINDOWACTIVATED, 0xFFFFFFFF, 0, 0, 0, 0)

  -- We call the external function and tell it to install the hook.  We *have*
  -- to send it a valid window handle.  We send it ours.  The handle is not
  -- valid until at least initDialog(), it is not valid earlier in the life
  -- cycle of this dialog.
  self~applicationCode = NoteActivatedWindows(self~dlgHandle, 'I')

  -- If not a good return we quit.
  if self~applicationCode <> 0 then return self~cancel:super


::method onWindowActivated
  use arg hwnd, isFullScreen

  startTime = time('L')

  -- When the activated window changes to no window activated, the Windows API
  -- sends both parameters as 0.  In ooDialog, when it recieves both parameters
  -- as 0, it does not send any args to the method invoked. (This method.)
  -- Therefore, when and only when arg 1 is ommitted, there is no activated
  -- window.  You can see this when the user clicks on the Start Menu or the
  -- Task Bar.
  if arg(1, 'O') then do
    title = "<none>"
    handle = "<N/A>"
  end
  else do
    title = self~windowTitle(hwnd)
    handle = self~formatAsHex(hwnd)
  end

  say 'New window activated.'
  say title
  say "hwnd:" handle
  say "time:" startTime
  say

::method formatAsHex private
  use strict arg number
  return "0x" || number~d2x~right(8, "0")

::attribute applicationCode get
::attribute applicationCode set private

/* Simple function to display an error return code. */
::routine displayError
  use strict arg errCode

  if errCode == .SUCCESS_RC then do
    msg = ""
  end
  else do
    msg = "Error installing the activated window monitor." || .endOfLine  || -
          "  Return code:" errCode || .endOfLine
  end

  select
    when errCode == .SUCCESS_RC then
      msg = "Success.  Application completed without error."

    when errCode == .ERROR_BAD_HWND_RC then
      msg = msg || "  The dialog window handle is not recognized."

    when errCode == .ERROR_HOOK_ALREADY_INSTALLED_RC then
      msg = msg || "  The monitor can not be installed because it is still active," || .endOfLine || -
                   "  or it was not previosly removed correctly."

    when errCode == .ERROR_BAD_ARGS_RC then
      msg = msg || "  The arguments to NoteActivatedWindows() are not correct."

    when errCode > 0 then
      msg = msg || "  The return code is a system error code.  Look up its meaning" || .endOfLine || -
                   "  in the Windows documentation."

    otherwise
      msg = msg || "  Unrecognized error code."
  end
  -- End select

  say msg
return 0
