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
  File:    logon_a_uc.rex                           Creation date:  06/22/2011

  Project: ooDialog Tutorial                        Last update:    06/22/2011
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Uncommented version of logon_a.rex
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dlg = .LogonDialog~new

  if dlg~initCode == 0 then do
    dlg~createCenter(110, 55, "Logon", "VISIBLE NOMENU SYSTEMMODAL")
    dlg~execute

    accessLevel = dlg~access
    user = dlg~user
    accessData = display(accessLevel, user)
  end
  else do
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

::class 'LogonDialog' public subclass UserDialog

::method defineDialog

  self~createStaticText( , 5,  5, 35, 10, , "User ID:")
  self~createStaticText( , 5, 20, 35, 10, , "Password:")

  self~createEdit(101, 43,  5, 62, 10, "AUTOSCROLLH")
  self~createEdit(102, 43, 20, 62, 10, "AUTOSCROLLH PASSWORD")

  self~createPushButton(1, 35, 35, 30, 15, "DEFAULT", "Ok")
  self~createPushButton(2, 75, 35, 30, 15,          , "Cancel")

  self~setUp

::method cancel
  expose accessLevel userID

  accessLevel = 0
  userID = ""

  self~cancel:super

::method ok
  expose userID accessLevel

  userID = self~newEdit(101)~getText~strip
  password = self~newEdit(102)~getText~strip

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


::method access
  expose accessLevel
  return accessLevel

::method user
  expose userID
  return userID

::method setUp private
  expose userID accessLevel attempts

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
