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

/* pollActiveWindows.rex */

   dlg = .activeWindows~new
   if dlg~initCode <> 0 then do
       say "Error creating dialog.  initCode:" dlg~initCode
       return 99
   end

   dlg~Execute('ShowTop')
   dlg~DeInstall

exit

::requires 'oodwin32.cls'
::requires 'winsystm.cls'

::class activeWindows subclass userdialog inherit AdvancedControls MessageExtensions

::method Init
   self~init:super
   rc = self~CreateCenter(335, 280, 'Acitve Windows')
   if rc == 0 then self~initCode = -1

::method DefineDialog

  self~addListControl(100, , 10, 10, 315, 245, "REPORT FULLROWSEL");
  self~addButton(101, 300, 260, 25, 15, "Close", close, "DEFAULT")


::method InitDialog
  expose listControl keepGoing

  keepGoing = .true
  listControl = self~getListControl(100)

  listControl~InsertColumn(0, "Title", 195)
  listControl~InsertColumn(1, "Handle", 35)
  listControl~InsertColumn(2, "Time", 65)

  listControl~addExtendedStyle("FULLROWSELECT GRIDLINES CHECKBOXES HEADERDRAGDROP")
  self~watchForWindows


::method watchForWindows unguarded
  expose listControl keepGoing

  reply 0

  last = 0
  myself = self~hwnd

  do while keepGoing
    hwnd = self~foregroundWindow
    if hwnd \== last, hwnd \ == 0 then do
      last = hwnd
      title = self~windowTitle(hwnd)
      startTime = time('L')
      listControl~addRow(, , title, hwnd, startTime)
    end
      j = sleepMS(500)
  end


::method close
  expose keepGoing
  keepGoing = .false
  j = SysSleep(1)
  return self~ok:super

::method cancel
  expose keepGoing
  keepGoing = .false
  j = SysSleep(1)
  return self~cancel:super
