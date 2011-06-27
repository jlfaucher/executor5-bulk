/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2011 Rexx Language Association. All rights reserved.    */
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
  File:    logon_a.rex                              Creation date:  05/20/2008

  Project: ooDialog Tutorial                        Last update:    06/22/2011
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Category:
    ooDialog samples

  Syntax:
    logon_a

  Purpose:
    Provide a simple introduction to ooDialog.

    This dialog is part of a fictious accounting program.  The dialog presents
    a screen where the user would provide their user ID and password.

    A few things are done differently than they would be in a real program to
    faciliate running this from a command prompt as a stand alone example.
    Those places should be relatively easy to spot.

  Notes:
    In its most basic sense, ooDialog is a framework that allows the ooRexx
    programmer access to a portion of the underlying graphical user interface
    provided by the Windows operating system.  It does this through the use of a
    number of class objects that represent a dialog box and dialog box controls.

    In the graphical user interface of the Windows OS, everything is a window.
    Most software applications consist of a main window and possibly several
    document windows.  In a main window the software application usually is
    responsible for drawing everything that takes place and handling every user
    generated event.

    A dialog box is a specialized type of window that is constructed using any
    number of pre-defined control windows.  Unlike an application with a main
    window, with a dialog box the programmer normally does none of the drawing
    and very little of the handling of user events.  Instead the dialog box and
    dialog controls handle all the drawing and most of the user events
    internally.

    Originally dialog boxes were conceived of as a means of getting input from
    the user to be used in the main application window.  However, since dialog
    boxes require less work for the programmer, it wasn't long before entire
    apllications were written using only dialog boxes.

    ooDialog exposes the functionality of dialog boxes and dialog controls.  It
    does *not* expose the functionality of main windows.

    All dialogs in the Windows operating system are created by supplying a
    dialog template to the operating system (OS).  The OS then creates the
    dialog seen on the screen from the template.  A dialog template defines the
    size, position, and style of the dialog, along with the size, position, and
    style of the dialog controls within the dialog.

    The ooDialog framework provides 3 basic ways for the programmer to provide
    the dialog template to the OS in the form of 3 base dialog classes.  The
    programmer decides which method he wants to use to provide the dialog
    template and then creates a dialog by subclassing one of the 3 base dialog
    classes.

    The 3 base classes are: UseDialog, RcDialog, and ResDialog.

    With the UserDialog, the programmer defines the dialog template dynamically
    using methods of the UserDialog class.

    With the RcDialog, the programmer provides the dialog template in the form
    of a resource script file.  Resource script files are usually created with
    a resource editor.

    With the ResDialog, the programmer provides the dialog template in a
    compiled binary form contained within a resource-only DLL.

    This introductory program uses a UserDialog subclass to provide the dialog
    template.  The dialog template is created by using methods supplied by the
    UserDialog class.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  -- NOTE that all routines and methods not defined in this file are provided
  -- by the ooDialog framework and are documented in the oodialog.pdf reference
  -- book.

  -- Instantiate a new Rexx dialog object.  The dialog presents a simple log on
  -- screen to the user, with a field to enter the user name and a field to
  -- enter the password.

  dlg = .LogonDialog~new

  -- The initCode attribute of the dialog object is used to signal any error in
  -- the instantiation of the object.  The init code is 0 if there were no
  -- errors detected and not 0 if there were errors.  It is very unlikely that
  -- an error will occur, but it does no harm to check.

  if dlg~initCode == 0 then do

    -- The create() and createCenter() methods start the definition of the
    -- dialog template for a UserDialog subclass.  One or the other must be
    -- called first, before the creation of the dialog controls.  The methods
    -- defines the size and position of the dialog, the title of the dialog,
    -- and the style of the dialog.
    --
    -- The create() or createCenter() methods automatically invoke the
    -- defineDialog().  Within the defineDialog() method, the programmer then
    -- creates the dialog controls that populate the dialog.  When the create()
    -- or createCenter() method returns, the dialog template has been completely
    -- finished.

    dlg~createCenter(110, 55, "Logon", "VISIBLE NOMENU SYSTEMMODAL")

    -- Now that the dialog template is complete, we are ready to pass the
    -- template on to the OS, show the dialog, and run the dialog.  The
    -- execute() method does just that.  It passes the template to the OS, the
    -- OS creates the undelying dialog, shows the dialog on the screen, and runs
    -- the dialog.  The dialog continues running until the user closes it.
    --
    -- When the execute() method returns, the underlying Windows dialog has been
    -- closed by the user.  This could be seconds, minutes, or even days after
    -- we invoke the execute() method.

    dlg~execute

    -- Now that the dialog has been closed, we get the results from the dialog.

    accessLevel = dlg~access
    user = dlg~user

    -- After the logon attempt, the data would usually be passed on to the main
    -- program.  Here it is just displayed to the console, using a convenience
    -- function, dislplay() defined at the end of this program.

    accessData = display(accessLevel, user)
  end
  else do
    -- This is the case that the initCode was not 0, we inform the user

    j = infoDialog("System error.  The logon dialog could" || '0d0a'x || -
                   "not be created.  Contact your system"  || '0d0a'x || -
                   "administrator.")
    accessData = .nil
  end

return accessData
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "oodialog.cls"

-- This is the basis of our log on dialog.  We subclass the UserDialog and then
-- code the methods that will define how the dialog behavies.
::class 'LogonDialog' public subclass UserDialog

/** defineDialog()
 *
 * The defineDialog() method is provided by the ooDialog framework, it is
 * invoked at the proper time by the framework, that is, after the dialog
 * template has been started and now needs the addition of the dialog controls.
 *
 * The default implementation of this method does nothing.  The method is meant
 * to be over-ridden by the programmer.  Within the method, the programmer
 * creates the dialog controls that will be present in the dialog.
 */
::method defineDialog

  -- Create 2 static controls.  These controls are the labels for the edit
  -- controls.

  self~createStaticText( , 5,  5, 35, 10, , "User ID:")
  self~createStaticText( , 5, 20, 35, 10, , "Password:")

  -- Create 2 edit controls.  These edit controls are where the user enters her
  -- user ID and password.

  self~createEdit(101, 43,  5, 62, 10, "AUTOSCROLLH")
  self~createEdit(102, 43, 20, 62, 10, "AUTOSCROLLH PASSWORD")

  -- Create the Ok and Cancel push buttons.

  self~createPushButton(1, 35, 35, 30, 15, "DEFAULT", "Ok")
  self~createPushButton(2, 75, 35, 30, 15,          , "Cancel")

  -- setUp() is a private method of this dialog that we use to initialize
  -- things.
  self~setUp

/** cancel()
 *
 * The cancel() method is provided by the ooDialog framework and is invoked
 * by the framework automatically at the correct time.  That is, when the user
 * has canceled the dialog.
 *
 * The method is meant to be over-ridden by the programmer, if needed.
 *
 * The programmer can prevent the dialog from closing by simply not invoking the
 * superclass cancel() method.
 *
 * To allow the dialog to close, invoke the superclass cancel() method.  Always
 * close a dialog by invoking either the superclass ok() or cancel() methods.
 * This ensures that the dialog execution is processed correctly.
 *
 * We over-ride the method simply to ensure that the accessLevel and userID
 * attributes are set to null values.
 */
::method cancel
  expose accessLevel userID

  accessLevel = 0
  userID = ""

  self~cancel:super

/** okd()
 *
 * The okd() method is provided by the ooDialog framework and is invoked by the
 * framework automatically at the correct time.  That is, when the user has
 * closed the dialog with okay.
 *
 * The method is meant to be over-ridden by the programmer, if needed.
 *
 * The programmer can prevent the dialog from closing by simply not invoking the
 * superclass ok() method.
 *
 * To allow the dialog to close, continue by invoking the superclass ok()
 * method.  Always close a dialog by invoking either the superclass ok() or
 * cancel() methods.  This ensures that the dialog execution is processed
 * correctly.
 *
 * Most often the ok() method is over-ridden to validate the user input.  Here
 * we over-ride the method to be sure the user entered a user ID and password,
 * and then to check that the ID and password are correct.
 */
::method ok
  expose userID accessLevel

  -- We get the text in both edit controls.  This is done by getting a new edit
  -- control object and asking it for its text.

  userID = self~newEdit(101)~getText~strip
  password = self~newEdit(102)~getText~strip

  /*
   * The above is a short hand version of the method invocations.  This is a
   * clearer form that does the same thing:
   *
   * editControl = self~newEdit(101)
   * text = editControl~getText
   * userID = text~strip
   */

  -- The following is just standard programming to verify the user and password.
  -- To prevent the dialog from closing, return 0.  When everything checks out,
  -- continue with the dialog closing by invoking the superclass ok() method.

  if userID == "" | password == "" then do
    j = infoDialog("You must enter both a user ID and a password")
    return 0
  end

  accessLevel = self~certify(userID, password)

  if accessLevel == 0 then do
    j = infoDialog("The user ID or password is not recognized")
    return 0
  end

  select
    when accessLevel == -1 then do
      j = infoDialog("Four failed log on attempts in a row." || '0d0a0d0a'x || -
                      "This session is terminated.")
    end

    when accessLevel == -2 then do
      j = infoDialog("Three failed log on attempts for" userID"." || -
                     '0d0a0d0a'x || -
                     "The" userID "account is locked.")
    end

    otherwise
      nop
  end
  -- End select

  self~ok:super


/** access()
 * Returns the access level granted for the log on attempt.
 */
::method access
  expose accessLevel
  return accessLevel

/** user()
 * Returns the user ID for the log on attempt.
 */
::method user
  expose userID
  return userID

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  The methods below are used to give this example the feel of a real
  application.  It is not necessary to understand them in order to understand
  how the application works.  They should be treated simply as a black box
  function - provided some input, you get back some output.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/** setUp()
 * Initializes the housekeeping instance variables.  This could be done in the
 * init() method of the dialog class, but this simple dialog does not need an
 * init method.  The variables are set here to keep the relevant ooDialog
 * methods as uncluttered as possible and to show that the init() method is not
 * needed.
 */
::method setUp private
  expose userID accessLevel attempts

  -- The exposed instance variables are used for housekeeping.  3 failed
  -- attempts to log on with the same user ID locks out the user's account.
  userID = ""
  accessLevel = 0
  attempts = 0


::method certify private
  expose users recorder attempts
  use strict arg id, password

  attempts += 1
  id = id~upper

  if \ users~isA(.directory) then users = self~createUserDirectory

  if users[id] == password then return id~length

  if \ recorder~isA(.table) then recorder = .table~new

  if recorder~hasIndex(id) then do
    recorder[id] += 1
    if recorder[id] > 2 then return -2
  end
  else do
    recorder[id] = 1
  end

  if attempts > 3 then return -1

  return 0


::method createUserDirectory private
  d = .directory~new

  d~rick = 'java'
  d~markh = 'tree'
  d~david = 'ibm'
  d~lee = 'safe'
  d~miesfeld = 'ppclab'
  d~jon = 'england'

  return d

/** display()
 * Prints out some facts about the log on attempt to the console.
 */
::routine display
  use strict arg level, user

  select
    when level == -2 then
      say "Failed logon.  The" user "account is now locked out."

    when level == -1 then
      say "Failed logon.  The session was forcibly terminated."

    when level == 0 & user == "" then
      say "The user canceled the logon."

    when level == 0 then
      say "The user" user "is not authorized to access this system."

    otherwise
      say "The user" user "is granted access at level" level
  end
  -- End select

  return level user

/* - - - End Of File: logon_a.rex - - - - - - - - - - - - - - - - - - - - - - */
