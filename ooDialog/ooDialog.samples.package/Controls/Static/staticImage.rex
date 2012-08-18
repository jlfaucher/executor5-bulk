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

/** staticImage.rex
 *
 * A simple dialog showing how to use static image controls and the NOTIFY style
 * for a static control.
 *
 * The NOTIFY style allows you to connect a few simple events for a static
 * control.  Without that style, there are no event notifications from a static
 * control.
 *
 * When running the program, double click on the icon image and the image will
 * change.
 */

  .application~setDefaults("O", , .false)
  .constDir[IDC_ICON_QUESTION] = 100
  .constDir[IDC_ST_QUESTION]   = 101

  dlg = .SimpleDialog~new
  dlg~execute("SHOWTOP")

return 0

::requires "ooDialog.cls"

::class 'SimpleDialog' public subclass UserDialog

::method init

  forward class (super) continue

  self~createCenter(186, 95, "Simple Question Dialog")
  return 0

::method defineDialog
  expose someIcon questionIcon isQuestion

  self~createStaticImage(IDC_ICON_QUESTION, 14, 17, 20, 20, "NOTIFY ICON SIZEIMAGE")
  self~createStaticText(IDC_ST_QUESTION, 59, 17, 122, 20)
  self~createPushButton(IDOK, 126, 74, 50, 14, "DEFAULT", 'Ok')

  self~connectStaticEvent(IDC_ICON_QUESTION, "DBLCLK", onDoubleClick)

  someIcon     = .Image~getImage("shaveIce.ico", .Image~toID(IMAGE_ICON))
  questionIcon = .Image~getImage(.Image~toID(IDI_QUESTION), .Image~toID(IMAGE_ICON))
  isQuestion   = .false

::method initDialog
  expose iconImage iconText

  iconImage = self~getStaticControl(IDC_ICON_QUESTION)
  iconText  = self~getStaticControl(IDC_ST_QUESTION)

  self~swapIcons

::method onDoubleClick unguarded
  expose iconImage questionIcon someIcon

  self~swapIcons

::method swapIcons private unguarded
  expose iconImage questionIcon someIcon isQuestion iconText

  if isQuestion then do
    iconImage~setImage(questionIcon)
    iconText~setText("What is your question?")
    isQuestion = .false
  end
  else do
    iconImage~setImage(someIcon)
    iconText~setText("This is where the question goes.")
    isQuestion = .true
  end

