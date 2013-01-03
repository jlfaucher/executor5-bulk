/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2013 Rexx Language Association. All rights reserved.    */
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

::requires 'ooDialog.cls'

::class 'ProgressDialog' public subclass UserDialog

::attribute captionText
::attribute msgText
::attribute rangeMax
::attribute rangeMin
::attribute incrStep
::attribute worker unguarded private

::method init

  self~init:super

  self~setDefaults


::method setDefaults private
  expose interruptible width height

  self~captionText = "Processing ..."
  self~msgText = "Background processing is taking place.  Please be patient."

  self~rangeMax = 100
  self~rangeMin = 0
  self~incrStep = 1

  self~worker = .nil

  width  = 350
  height = 150

::method begin unguarded
  expose width height

  reply 0

  if self~createCenter(width, height, self~captionText) then do
    self~popup('ShowTop')
  end
  else do
    self~initCode = 1
    j = infoDialog("Failed to create underyling dialog.")
  end

::method setInterruptible unguarded
  use strict arg w

  signal on syntax

  if \ w~isA(.Interruptible) then do
    raise syntax 93.948 array ("1", "Interruptible")
  end

  self~worker = w
  return

syntax:
  c = condition('O')
  if c~traceBack \== .nil then c~traceBack~remove(c~traceback~first)
  raise propagate

::method updateStatus unguarded
  expose statusBar
  use strict arg text

  statusBar~setText("Status:" text)


::method increase unguarded
  expose pb
  use strict arg incr = (self~incrStep)
  pb~step(incr)

::method complete unguarded
  expose pb
  use strict arg
  pb~setPos(self~rangeMax)

::method endNow unguarded
  return self~ok:super


::method defineDialog

  self~createStaticText(101, 5, 5, self~sizeX - 10, 50, , self~msgText)
  self~createStaticText(102, 5,  self~sizeY - 40, self~sizeX - 10, 15, , "" )
  self~createProgressBar(100, 5, self~sizeY - 20, self~sizeX - 10, 10, 'Smooth')


::method initDialog
  expose pb statusBar

  pb = self~newProgressBar(100)
  pb~setRange(self~rangeMin, self~rangeMax)
  pb~setStep(1)

  self~newStatic(101)~setText(self~msgText)

  statusBar = self~newStatic(102)
  statusBar~setText("Status: ")

-- Don't let the user close the dialog, unless the work can be stopped.

::method cancel unguarded
  expose worker
  say 'worker:' worker
  if worker <> .nil then do
    if worker~interruptible then do
      worker~interrupt
      return
    end
  end

  self~giveStatus


::method ok unguarded
  expose worker
  say 'worker:' worker
  if worker <> .nil then do
    if worker~interruptible then do
      worker~interrupt
      return
    end
  end

  self~giveStatus


::method giveStatus unguarded
  expose pb

  reply 0

  r = pb~getFullRange
  percentDone = trunc((pb~getPos / (r~max - r~min)) * 100)

  msg = "Processing is only" percentDone"% done " || .endOfLine ||             -
        "and can not be canceled at this time.  " || .endOfLine ||             -
        "(Try canceling later.)"                  || .endOfLine~copies(2) ||   -
        "Please be patient."

  j = infoDialog(msg)


::class 'Interruptible' public mixinclass Object
::method interrupt     unguarded abstract
::method interruptible unguarded abstract

