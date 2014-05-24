/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2014-2014 Rexx Language Association. All rights reserved.    */
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
  con = .StdOutput~new

  inputCodePage = con~getCP
  outputCodePage = con~getOutputCP
  historyInfo = con~getHistoryInfo

  say 'Input codepage: ' inputCodePage
  say 'Output codepage:' outputCodePage

  say 'History commands:' historyInfo~commands
  say 'History buffers: ' historyInfo~buffers
  say 'History flag:    ' historyInfo~flag

  dMode = con~getDisplayMode
  say 'Display mode:' dMode 'errRC:' con~errRC

  if \ con~setHistoryInfo(3, 35, .false) then do
    say 'setHistoryInfo failed.  errRC:' con~errRC
  end
  d = con~getHistoryInfo
  say 'History commands:' d~commands
  say 'History buffers: ' d~buffers
  say 'History flag:    ' d~flag

  ret = con~setCP(437); say 'ret:' ret
  ret = con~setOutputCP(437); say 'ret:' ret

  say "Hello there"

  con~setCP(inputCodePage)
  con~setOutputCP(outputCodePage)

  if historyInfo~flag~caselessCompare('Duplicates') == 0 then flag = .true
  else flag = .false

  con~setHistoryInfo(historyInfo~commands, historyInfo~buffers, flag)
  say 'setHistoryInfo back to its original setting.  errRC:' con~errRC

  con~setHistoryInfo(historyInfo)
  say 'setHistoryInfo back to its original setting using a directory.  errRC:' con~errRC

  d = con~getHistoryInfo
  say 'History Settings Now:'
  say 'History commands:' d~commands
  say 'History buffers: ' d~buffers
  say 'History flag:    ' d~flag


::requires 'ooConsole.cls'
