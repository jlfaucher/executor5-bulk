/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2009 Rexx Language Association. All rights reserved.    */
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
 * This Rexx program provides a graphical 'log on' dialog where the user
 * supplies their user ID and password.  The returm from the program indicates
 * whether the user is allowed access to the system being logged into.
 *
 * It uses an external library, dbAccess.dll, that provides database function.
 * In the example the database access is just simulated, but shows how access to
 * a real backend database could be done.
 *
 * The intent of the example is that this Rexx program be started through
 * rxStart.exe, however it runs stand alone just as well.
 *
 * Among other things, it shows how a resource editor can be used to create the
 * dialog template (logon.rc,) which is then compiled into a resource only .DLL,
 * (logon.dll,) and used with an ooDialog program.
 */

  -- Open the database, return on error.  open() is an external routine,
  -- implemented in dbAccess.cpp.  Note how open is defined in the ::routine
  -- directive in this file just below the ::requires 'dbAccess' LIBRARY
  -- directive.

  ret = open()
  if ret <> 0 then return ret

  -- Create a new LogonDialog object and execute it.  The dialog template is
  -- contained in the logon.dll library, with a symbolic ID of IDD_LOGON_DLG,
  -- and the symbolic IDs used in this program are defined in logon.h

  dlg = .LogonDialog~new("logon.dll", IDD_LOGON_DLG, , "logon.h" )
  dlg~execute

return dlg~status
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

-- Requires for ooDialog
::requires "oodWin32.cls"

-- Requires for the exeternal library.  This causes the intepreter to
-- automatically load the dbAccess.dll library and register all the external
-- routines and methods in the library.  It does this through the
-- RexxGetPackage() function using the dbAccess_package_entry struct.  That
-- struct in turn contains pointers to the dbAccess_functions[] and
-- dbAccess_methods[] arrays.  The arrays contain the actual entry points for
-- each of the external routines and external methods.  See the end of
-- dbAccess.cpp for the details of implementing this in the C++ code.
::requires 'dbAccess' LIBRARY

-- This is the definition for the Rexx routine open().  It basically says the
-- code for the open() routine defined in an 'external' 'LIBRARY'.  The library
-- name is dbAccess (with the appropriate extension for the current operating
-- system.)  The entry point to the code is named db_open.  See dbAcess.cpp for
-- the actual implementation of the routine.
::routine open external "LIBRARY dbAccess db_open"

::class 'LogonDialog' public subclass ResDialog inherit AdvancedControls

::constant MAX_LOGON_ATTEMPTS  3

::attribute status get
::attribute status set private

::attribute attempts private

-- The next to lines define the verifyUser() and verifyPassword() methods for
-- the Logon class.  They are defined as private methods.  This is exactly the
-- same as the definition for the external routine above.  It the code that
-- implements these methods is in an 'external' 'LIBRARY' name dbAccess (.dll
-- since we are operating on Windows.)  The entry point for the verifyUser()
-- method is named ld_verifyUser in the library.  And, the entry point for the
-- verifyPassword() method is name ld_verifyPassword.
--
-- Note that the duplication of the Rexx method name in the C++ function name
-- is just a personal idiosyncrasy.  The two C++ functions could just as easily
-- be call abcd() and wxyz().
::method verifyUser private external 'LIBRARY dbAccess ld_verifyUser'
::method verifyPassword private external 'LIBRARY dbAccess ld_verifyPassword'

-- The rest of the code is just a standard ooDialog dialog implementation.
-- Maybe interest to someone learning ooDialog, but not relevant to this
-- example, so it is not commented from here on down.

::method initDialog
  expose userEditControl passwordEditControl

  self~status = self~constDir[USER_DENIED_ACCESS_RC]
  self~attempts = 0
  userEditControl = self~getEditControl(IDC_EDIT_USER_ID)
  passwordEditControl = self~getEditControl(IDC_EDIT_PASSWORD)

::method ok
  expose userEditControl passwordEditControl

  user = userEditControl~getText~strip
  if \ self~verifyUser(user) then return self~invalidUser(user)

  password = passwordEditControl~getText~strip
  if \ self~verifyPassword(user, password) then return self~invalidPassword

  self~status = self~constDir[USER_GRANTED_ACCESS_RC]
  return self~ok:super

::method cancel
  self~status = self~constDir[USER_CANCELED_RC]
  return self~cancel:super

::method invalidUser private
  use strict arg user

  msg = "'"user"' is not a valid user ID for this system."
  return self~failed(msg)

::method invalidPassword private

  msg = "Your user ID and password do not match."
  return self~failed(msg)

::method failed private
  expose attempts userEditControl
  use strict arg msg

  attempts += 1
  EOL = .endOfLine
  msg = msg || EOL || EOL

  if attempts >= self~MAX_LOGON_ATTEMPTS then do
    self~status = self~constDir[USER_DENIED_ACCESS_RC]

    msg = msg || "Access denied.  Your account is locked out after" attempts || EOL || -
          "failed logon attemps.  Contact the system administrator" || EOL || -
          "to unlock your account."

    j = errorDialog(msg)
    self~status = self~constDir[USER_DENIED_ACCESS_RC]
    return self~cancel:super
  end

  msg = msg || "Please enter a valid user ID and password."
  j = errorDialog(msg)
  userEditControl~assignFocus
  return 0

/* - - - End Of File: logon.rex - - - - - - - - - - - - - - - - - - - - - - - */
