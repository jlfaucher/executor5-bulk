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

/*
  progressProgram.rex

  Originally from code written by Kees Wiegal.  Heavily rewritten.

  Demonstrates using a progress bar while work is done.  Needs ooDialog 4.2.0
  at least.

  The class ProgressDialog is intended to be the start of the fix for the
  undocumented ProgressIndicator in UtilityClasses.cls
*/

  -- Create the progress indicator dialog.
  progressDlg = .ProgressDialog~new

  -- Create the worker and start working.
  worker = .Worker~new(progressDlg)
  worker~startWorking

  say "The work" worker~getStatus

return 0


::requires 'ProgressDialog.cls'

::class 'Worker' inherit Interruptible

::method init
  expose progressDlg interruptible canceled status
  use strict arg progressDlg

  interruptible = .true
  canceled = .false
  status = 'was not started'

  progressDlg~setInterruptible(self)


::method startWorking
  expose progressDlg interruptible canceled status

  --reset timer
  call time('R')

  interruptible = .false
  status = "was started"

  progressDlg~setDlgFont('Comic Sans MS', 10)
  progressDlg~msgText = "Some other text".endOfLine"besides".endOfLine"the default"
  progressDlg~rangeMax = 200
  progressDlg~begin

  if progressDlg~initCode <> 0 then do
    status = "failed to start"
    return
  end

  -- Start of work, just a simulation

  do i = 1 to progressDlg~rangeMax - progressDlg~rangeMin
    call SysSleep 0.1

    --  Update the progress indicator's status with the processing time
    eMin = time('E') % 60; eMin = eMin~right(2,'0')
    eSec = (time('E')//60)~format(,0); eSec = eSec~right(2,'0')
    progressDlg~updateStatus('processing time:' eMin':'eSec)

    --  Have the progress bar step.
    progressDlg~increase

    -- Simulate periods when we can be halted, and when we can not
    select
      when i == 1 then interruptible = .true
      when i // 10 == 0  then interruptible = .false
      when i // 4 == 0 & \interruptible then interruptible = .true
      otherwise nop
    end

    if interruptible & canceled then do
      progressDlg~updateStatus('Canceled at:' eMin':'eSec)
      progressDlg~complete

      -- Give the user a little time to see results
      call SysSleep 1.5
      status = "was interrupted"
      leave
    end
  end

  -- End of simulated work

  if status \== "was interrupted" then status = "was finished successfully"

  -- End the progress indicator.
  progressDlg~endNow


::method interrupt unguarded
  expose interruptible canceled status

  if \ interruptible then return .false

  canceled = .true
  status = 'was canceled'
  return .true

::method interruptible unguarded
  expose interruptible
  return interruptible

::method getStatus
  expose status
  return status

