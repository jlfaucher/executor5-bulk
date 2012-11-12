/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2012 Rexx Language Association. All rights reserved.    */
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
 * ToolTip control example.
 *
 */

  .application~setDefaults('O', , .false)

  .constDir[IDC_TT_MAIN ]     = 500
  .constDir[IDC_TT_TEST1]     = 501
  .constDir[IDC_TT_TEST2]     = 502
  .constDir[IDC_TT_TEST3]     = 503
  .constDir[IDTOOL_DLG_RECT1] = 774
  .constDir[IDTOOL_DLG_RECT2] = 775
  .constDir[IDTOOL_DLG_RECT3] = 776
  .constDir[IDTOOL_DLG_RECT4] = 777
  .constDir[IDC_PB_TEST]      = 600


  dlg = .ToolTipDialog~new
  dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

::requires "ooDialog.cls"

::class 'ToolTipDialog' subclass UserDialog

::method init
  forward class (super) continue

  self~create(30, 30, 257, 123, "ToolTip Testing Dialog", "CENTER")

::method defineDialog

  self~createPushButton(IDC_PB_TEST, 10, 99, 50, 14, , "Test", onTest)
  self~createPushButton(IDOK, 142, 99, 50, 14, "DEFAULT", "Ok")
  self~createPushButton(IDCANCEL, 197, 99, 50, 14, , "Cancel")


::method initDialog unguarded
  expose tt ttOk clRect1 clRect2 clRect3 clRect4

  count = 0

  ttOk   = self~newToolTip(IDC_TT_Test1, 'BALLOON')
  ttEsc  = self~newToolTip(IDC_TT_Test2)
  ttTest = self~newToolTip(IDC_TT_TEST3)
  tt     = self~newToolTip(IDC_TT_MAIN)

  clRect = self~clientRect
  hMidpoint = trunc((clRect~right - clRect~left) / 2) + clRect~left
  vMidpoint = trunc((clRect~bottom - clRect~top) / 2) + clRect~top

  clRect1 = .Rect~new(clRect~left, clRect~top, hMidpoint, vMidpoint)
  clRect2 = .Rect~new(hMidpoint + 1, clRect~top, clRect~right, vMidpoint + 1)
  clRect3 = .Rect~new(clRect~left, vMidpoint + 1, hMidpoint + 1, clRect~bottom)
  clRect4 = .Rect~new(hMidpoint + 1, vMidpoint + 1, clRect~right, clRect~bottom)

  ret = tt~addToolRect(self, IDTOOL_DLG_RECT1, clRect1, 'Over main dialog, top left quadrant', 'TRANSPARENT')
  ret = tt~addToolRect(self, IDTOOL_DLG_RECT2, clRect2, 'Over main dialog, top right quadrant', 'TRANSPARENT')
  ret = tt~addToolRect(self, IDTOOL_DLG_RECT3, clRect3, 'Over main dialog, bottom left quadrant', 'TRANSPARENT')
  ret = tt~addToolRect(self, IDTOOL_DLG_RECT4, clRect4, 'Over main dialog, bottom right quadrant', 'TRANSPARENT')

  self~connectToolTipEvent(IDC_TT_MAIN, 'SHOW', onShow)

  text = 'Push the'.endOfLine'Ok button'.endOfLine'to end this'.endOfLine'foolishness'
  ttOk~addTool(self~newPushButton(IDOK), text)

  margins = 2 * 4
  s = self~getTextSizePX('foolishness')
  ttOk~setMaxTipWidth(s~width + margins)

  ttEsc~addTool(self~newPushButton(IDCANCEL), 'Press Cancel to end the dialog')
  ttTest~addTool(self~newPushButton(IDC_PB_TEST), 'Press Test to execute a ToolTip test')

::method onShow unguarded
  expose clRect1 clRect2 clRect3 clRect4
  use arg toolID, toolTip

  select
    when toolID == .constDir[IDTOOL_DLG_RECT1] then pos = .Point~new(clRect1~left, clRect1~top)
    when toolID == .constDir[IDTOOL_DLG_RECT2] then pos = .Point~new(clRect2~left, clRect2~top)
    when toolID == .constDir[IDTOOL_DLG_RECT3] then pos = .Point~new(clRect3~left, clRect3~top)
    when toolID == .constDir[IDTOOL_DLG_RECT4] then pos = .Point~new(clRect4~left, clRect4~top)
    otherwise pos = .Point~new
  end
  -- End select

  self~client2screen(pos)
  toolTip~setWindowPos(TOPMOST, pos, .Size~new(0, 0), "NOACTIVATE NOSIZE NOZORDER")

  return .true

::method onTest unguarded
    expose tt ttOk

    do i = 1 to tt~getToolCount
      toolInfo = tt~enumTools(i)
      say 'Tool info  hwnd:    ' toolInfo~rexxHwnd
      say 'Tool info  id:      ' toolInfo~rexxID
      say 'Tool info  text:    ' toolInfo~text
      say 'Tool info  flags:   ' toolInfo~flags
      say 'Tool info  rect:    ' toolInfo~rect
      say 'Tool info  userData:' toolInfo~userData
      say 'Tool info  resource:' toolInfo~resource
      say
    end

    return 0

