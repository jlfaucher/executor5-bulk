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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  File:    logon_a.rex                              Author:      Mark Miesfeld
                                                    Creation date:  05/20/2008

  Project: ooDialog Tutorial                        Last update:    06/01/2008
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
    Most software applications consist of a main window or possibly several
    document windows.  In a main window the software application usually is
    responsible for drawing everything that takes place and handling every user
    generated event.

    A dialog box is a specialized type of window that is constructed using any
    number of pre-defined control windows.  Unlike with a main application
    window, with a dialog box the programmer normally does none of the drawing
    and very little of the handling of user events.  Instead the dialog box and
    dialog controls handle all the drawing and most of the user events
    internally.

    Originally dialog boxes were conceived of as a means of getting input from
    the user to be used in the main application window.  However, since dialog
    boxes require less work for the programmer, it wasn't long before entire
    apllications were written using only dialog boxes.

    ooDialog exposes the functionality of dialog boxes and dialog controls.  It
    does not expose the functionality of main windows.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dlg = .LogonDialog~new

  if dlg~initCode == 0 then do
    dlg~createCenter(110, 55, "Logon", "VISIBLE NOMENU SYSTEMMODAL")
    dlg~execute

    accessLevel = dlg~access
    user = dlg~user

    -- After the logon attempt, the data would usually be passed on to the main
    -- program.  Here it is just displayed to the console, using a convenience
    -- function defined at the end of this program.
    accessData = display(accessLevel, user)
  end
  else do
    j = infoDialog("System error.  The logon dialog could" || '0d0a'x || -
                   "not be created.  Contact your system"  || '0d0a'x || -
                   "administrator.")
    accessData = .nil
  end

  -- The deInstall method unregisters all the ooDialog external functions.
  -- Normally that would only be done when the program is entirely finished.
  -- Since this is just an example and it is now finished, the deInstall method
  -- is invoked.  This is not really needed.
  dlg~deInstall

return accessData
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "oodialog.cls"

::class 'LogonDialog' public subclass UserDialog

::method defineDialog

  self~addText(5,  5, 35, 10, "User ID:")
  self~addText(5, 20, 35, 10, "Password:")

  self~addEntryLine(101, , 43,  5, 62, 10, "AUTOSCROLLH")
  self~addEntryLine(102, , 43, 20, 62, 10, "AUTOSCROLLH PASSWORD")

  self~addButton(2, 35, 35, 30, 15, "Cancel")
  self~addButton(1,     75, 35, 30, 15, "Ok", , "DEFAULT")

  self~setUp

::method cancel
  expose accessLevel userID

  accessLevel = 0
  userID = ""

  self~cancel:super

::method ok
  expose userID accessLevel

  userID = self~getEntryLine(101)~strip
  password = self~getEntryLine(102)~strip
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
