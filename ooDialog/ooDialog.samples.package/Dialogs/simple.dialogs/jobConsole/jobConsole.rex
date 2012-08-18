/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
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


/* Example job processing console */

  dlg = .JobConsole~new("guiConsole.rc", IDD_GUI_CONSOLE,  -
                        , 'guiConsole.h')

  if dlg~initCode = 0 then do
    dlg~Execute("SHOWTOP")
  end
  else do
    say 'Error initalizing dialog.  Aborting'
  end

  dlg~deinstall

return 0
-- End of entry point.

::requires "oodWin32.cls"

::class 'JobConsole' subclass RcDialog inherit             -
                     AdvancedControls MessageExtensions

::method initDialog
  expose outList pbStart pbStop lineCounter                -
         processorRunning processorAlarm maxLines jobThings

  -- This array is just to have some things to put into
  -- the output console
  jobThings = .array~new
  jobThings[1] = "Update database"
  jobThings[2] = "Query database One"
  jobThings[3] = "Query database Two"
  jobThings[4] = "Sync database"
  jobThings[5] = "Backup database"

  outList = self~getListControl(IDC_LV_OUTPUT)

  outList~insertColumn(0, 'Date', 35,'LEFT')
  outList~insertColumn(1, 'Time', 35,'LEFT')
  outList~insertColumn(2, 'Activity', 250,'LEFT')

  lineCounter = 0
  maxLines = 200

  self~connectButton(IDC_PB_START, onStart)
  self~connectButton(IDC_PB_STOP, onStop)

  pbStart = self~getButtonControl(IDC_PB_START)
  pbStop = self~getButtonControl(IDC_PB_STOP)
  pbStop~disable

  processorRunning = .false
  processorAlarm = .nil

  self~addPausedLine


::method onStart
  expose pbStart pbStop processorRunning

  pbStop~enable
  pbStart~disable
  pbStop~style = "DEFPUSHBUTTON"

  processorRunning = .true
  self~process


::method onStop
  expose pbStart pbStop processorRunning processorAlarm

  pbStart~enable
  pbStop~disable
  pbStart~style = "DEFPUSHBUTTON"

  self~stopProcessing
  self~addPausedLine

::method ok

  self~stopProcessing
  return self~ok:super

::method cancel

  self~stopProcessing
  return self~cancel:super


::method stopProcessing private
  expose processorRunning processorAlarm

  processorRunning = .false
  if processorAlarm \== .nil then do
    processorAlarm~cancel
    processorAlarm = .nil
  end

::method addPausedLine private

  line = 'Hello. Job Processor is ready, but paused.'
  self~addOutputLine(line)


::method addOutputLine private
  expose outList lineCounter maxLines
  use strict arg line

  outList~addRow( , , date('e'), time(), line)
  lineCounter += 1

  if lineCounter > maxlines then do
    outList~delete(0)
    lineCounter -= 1
  end

  if lineCounter > 15 then do
    outList~ensureVisible(lineCounter - 1)
  end

::method process unguarded
  expose processorRunning processorAlarm jobThings

  reply
  if \ processorRunning then return

  do i = 1 to jobThings~items while processorRunning
    self~addOutputLine(jobThings[i])
    if \ processorRunning then return
    j = SysSleep(1)
  end

  -- If the processor should still be running, create a
  -- new alarm to go off in 5 seconds.  The alarm will
  -- then invoke this method again.
  if processorRunning then do
    processMsg = .Message~new(self, process)
    processorAlarm = .Alarm~new(5, processMsg)
  end
