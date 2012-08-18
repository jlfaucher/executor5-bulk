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

    -- Set global defaults for this application:
    --    O               use Only the global .constDir for symbolic IDs
    --    typingTutor.h   Constant symbols are defined in this file
    --    .false          Turn autodetection off
    .application~setDefaults('O', 'typingTutor.h', .false)

    -- Choice of phrases to copytype
    lines = .array~of('The quick brown fox jumps over the lazy dog',
                     ,'The cat sat on the mat',
                     ,'She sells seashells by the seashore')

    -- Pick a pharase to start with
    phrase = lines[random(1,lines~items)]

    -- Create a RcDialog object
    --    typingTutor.rc     The dialog template is in this file.
    --    IDD_TYPING_TUTOR   The symbolic ID of the dialog
    dlg = .TypeTutor~new('typingTutor.rc', IDD_TYPING_TUTOR)

    -- Let the dialog know what phrase to use
    dlg~phrase = phrase

    -- Start the underlying Windows dialog
    dlg~execute('ShowTop')

return 0

::requires 'ooDialog.cls'   -- Require the ooDialog framework

::class TypeTutor subclass RcDialog

::attribute phrase

::method initDialog
    expose editControl phrase

    -- Set the phrase being used
    self~newStatic(IDC_ST_PHRASE)~setText(phrase)

    -- As we are going to be referencing this control
    -- every time a key is typed it makes sense to create
    -- the ooRexx object just once and expose it to our
    -- event handler method
    editControl = self~newEdit(IDC_EDIT)

    -- Connect the onEditUpdate() method to the edit control's
    -- UPDATE event.  The update event happens each time the
    -- text in the edit control changes
    self~connectEditEvent(IDC_EDIT, 'UPDATE', 'onEditUpdate')

    -- Set the initial focus to the edit control
    editControl~assignFocus


-- This method is invoked every time the contents of the edit box are changed
::method onEditUpdate
    expose editControl phrase

    -- Get the current text in the edit control
    typed = editControl~getText

    -- Check for a a typing mistake
    if \ phrase~abbrev(typed) then do
       -- Let the user know she has made a mistake
       call beep 600, 200

       -- Remove the eroneous letter
       editControl~setText(typed~left(typed~length - 1))

       -- Put the cursor in the correct place
       editControl~select(typed~length, typed~length)
     end

