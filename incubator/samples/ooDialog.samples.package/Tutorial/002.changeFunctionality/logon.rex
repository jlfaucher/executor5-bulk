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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  File:    logon.rex                                Creation date:  06/27/2011

  Project: ooDialog Tutorial                        Last update:    06/27/2011
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Category:
    ooDialog samples

  Syntax:
    logon.rex

  Purpose:
    Take the original logonUser.rex and enhance / extend the program slightly to
    show how to use the ooDialog framework to produce application specific
    programs.

  Tutorial:
    As mentioned in 001.intro\logonUser.rex, software frameworks contain
    generic functionality that can be changed by user code to create an
    application specific program.

    In 002.changeFunctionality, the tutorial will begin to show how user code
    changes the generic function provided by the ooDialog framework to create
    an application specific program.

    In reality, all the following parts of the tutorial are about that same
    thing: writing user code to produce application specific programs.

    How the user code changes the functionality of the ooDialog framework breaks
    down into several fairly easy to grasp techniques, as follows.

    1.) User code can provide different argument values to the methods of the
        class objects in the program.  The different values change the behavior
        and or the appearence of the on-screen dialogs.

        I.e., the createCenter() method has arguments that specify the width and
        height of the dialog. The programmer can make the dialog bigger or
        smaller by using different values for the width and height.

    2.) The programmer uses the dialog template to construct a dialog that looks
        exactly as desired by adding whatever dialog controls, to the dialog,
        that the application needs to provide its specific behavior.  For
        eample, edit controls can be added to the dialog to allow the user to
        input specific information.

    3.) The ooDialog framework provides different methods for doing the same
        basic thing.  The programmer can implement application specific behavior
        by choosing the correct method.  For example the programmer can choose
        between the execute(), popup(), executeAsync(), and popupAsChild()
        methods to run the dialog.  The choice of methods here controls the
        behavior of the underlying Windows dialog.

    4.) The oodDialog framework provides default implmentations for many
        methods.  The programmer can over-ride these methods to change the
        default behavior to application specific behavior.  For instance, the
        default behavior of the ok() method is to simple close the dialog.  The
        programmer can over-ride the ok() to, for instance, not close the dialog
        if the user has not entered the correct information.

    5.) The programmer can add her own methods to the dialog subclass.  These
        methods are used to provide application specific funtionality.

    The example programs in this section and sub-sections of the tutorial start
    with the 001.intro\logonUser.rex example and use the techniques listed above
    to change the functionality provided by the ooDialog framework to an
    application specific functionality.

    The 001.intro\logonUser.rex dialog seems rather cramped and the position of
    the okay and cancel buttons is not ideal.  This program changes that using
    some of the techniques listed above.

    Note that as the tutorial moves along comments for areas that have been
    explained previously will be repeated some times, but eventually those
    comments are dropped from the programs.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  -- Numbers, such as (1.), in the comments refer back to the basic techniques
  -- in user code, listed above, for changing the framework functionality to
  -- create an application specific program.

  dlg = .LogonDialog~new

  -- Dynamically create the dialog template.  Here we use technique (1.) above
  -- to create a dialog that is bigger and uses a different font than the
  -- default.

  dlg~createCenter(165, 45, "Logon", "VISIBLE NOMENU SYSTEMMODAL", , 'Tahoma', 14)

  ret = dlg~execute

  if ret == 1 then say "The user is granted access to the system."
  else if ret == 2 then say "The user canceled, does not want to log on."
  else say "An unexpected error happened."

return ret
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

::requires "ooDialog.cls"

-- Subclass the UserDialog, a class provided by the ooDialog framework.
::class 'LogonDialog' public subclass UserDialog

-- The defineDialog() method is invoked automatically by the ooDialog framework.
-- Within that method, the programmer finishes dynamically creating the dialog
-- template, using methods provided by the UserDialog class.

::method defineDialog

  -- Dynamically create the controls in the dialog template.

  -- Use technique (1.) to supply different arguments for the positions of the
  -- label.  Use a margin of 10.

  self~createStaticText( , 10, 10, 85, 10, , "Do you want to log on?")

  -- The createOkCancelRightBottom() method does not place the buttons in a
  -- way suitable for this application.  So use technique (3.) above and choose
  -- an alternate method to create the Ok and Cancel buttons.  Here we create
  -- each button individually, making the Ok button the defualt button, and
  -- placing the buttons exactly where we want.

  self~createPushButton(IDOK, 45, 25, 50, 14, "DEFAULT", "&Ok")
  self~createPushButton(IDOK, 105, 25, 50, 14, , "&Cancel")

/* - - - End Of File: logonUser.rex - - - - - - - - - - - - - - - - - - - - - */
