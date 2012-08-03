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
 * This is a simple dialog showing how to use a list box and how to determine
 * what the user selects in the list box.  It does this by presenting a simple
 * quiz.
 *
 * The user selects the answer to the question posed at the top of the dialog.
 * Currently only a single question is asked and answered.  However, it would be
 * very simple to extend this program to asked multiple questions and to keep
 * track of the correct answers.
 */

  -- Use the .constDir only for symbolic IDs and turn automatic data detection
  -- off.
  .application~setDefaults('O', , .false)

  dlg = .PopQuizDlg~new("popQuiz.rc", IDD_POPQUIZ, , "popQuiz.h" )
  dlg~execute("SHOWTOP", IDI_DLG_OOREXX)


return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "ooDialog.cls"

::class 'PopQuizDlg' subclass RcDialog

::method init
    expose questions answers correct

    forward class (super) continue

    self~constructQuiz


::method initDialog
    expose questions answers lb

    -- Set the dialog box title to today's date.
    dt = .DateTime~new
    title = "Pop Quiz" dt~month~right(2, 0)'-'dt~day~right(2, 0)'-'dt~year
    self~title = title

    lb = self~newListBox(IDC_LB_ANSWERS)
    st = self~newStatic(IDC_ST_QUESTION)

    st~setText(questions[1])

    -- Holds the width in pixels of the longest answer.
    max = 0

    -- When the OS first creates the list box, its internal width is set to the
    -- width of the dialog control. If any of the strings are wider than the
    -- internal width, the will be truncated, even if the list box has the
    -- horizontal scroll option.  The scroll bars are only shown if the internal
    -- width is greater than the width of the dialog control.  Here, as we add
    -- each string to the list box, we calculate its width and determine the
    -- widest string.  Then the internal width of the list box is set to that
    -- value.

    ans = answers[1]
    do a over ans
      lb~add(a)
      size = lb~getTextSizePX(a)
      if size~width > max then max = size~width
    end

    -- Unfortunately, the OS does not calculate the size of a tab character, so
    -- we guess by using spaces.  In addition, we need a little bit for the
    -- margin of the list box.
    tabSize = lb~getTextSizePX("          ")
    margin  = 4
    lb~setWidthPX(max + margin + tabSize~width)


::method ok unguarded
    expose lb correct

    index = lb~selectedIndex

    title   = "Pop Quiz Results"
    buttons = "OK"

    if index == 0 then do
        icon = "EXCLAMATION"
        msg  = "You did not select an answer:"   || .endOfLine~copies(2) || -
               "You have FAILED the Pop Quiz."
    end
    else if index == correct[1] then do
        icon = "INFORMATION"
        msg  = "Answer" index "is correct:"      || .endOfLine~copies(2) || -
               "You have passed the Pop Quiz."
    end
    else do
        icon = "INFORMATION"
        msg  = "Answer" index "is NOT correct:"  || .endOfLine~copies(2) || -
               "You have FAILED the Pop Quiz."
    end

    ret = MessageDialog(msg, self~hwnd, title, buttons, icon)
    return self~ok:super


::method cancel unguarded
    expose lb

    title = "Are You Sure You Want to Cancel?"
    msg = "If you cancel and do not finish"        || .endOfLine || -
          "the Pop Quiz, you will receive an"      || .endOfLine || -
          "F for this quiz."                       || .endOfLine~copies(2) || -
          "If you finish the quiz and get all"     || .endOfLine || -
          "the answers wrong, you will receive"    || .endOfLine || -
          "a D for this quiz."                     || .endOfLine~copies(2) || -
          "Are you sure you want to cancel?"

    buttons = "YESNO"
    icon    = "WARNING"

    ret = MessageDialog(msg, self~hwnd, title, buttons, icon)

    if ret == self~IDYES then do
        -- Mark grade as an F
        return self~cancel:super
    end
    else do
        lb~deselectIndex
    end

    return 0


::method constructQuiz private
    expose questions answers correct

    -- Even though we only have 1 question, we set up an array of answers,
    -- questions and the correct answer.  This allows us to easily extend the
    -- 'Pop Quiz' to any number of questions.

    -- We put a tab character in the question to line things up nicely.
    tab = '09'x

    q1 = "Question:" || .endOfLine~copies(2) || "In what year was America discovered?"
    questions = .array~of(q1)

    a1 = .array~new(5)
    a1[1] = "A.)"tab"America was discovered by the Norseman in the 10th century."
    a1[2] = "B.)"tab"Paleo-Indians discovered America 12,000 years ago by crossing the Beringia Land Bridge."
    a1[3] = "C.)"tab"1492 when Columbus stumbled upon it."
    a1[4] = "D.)"tab"America is an imprecise term making the question unanswerable."
    a1[5] = "E.)"tab"None of the above."
    answers = .array~of(a1)

    correct = .array~of(4)
