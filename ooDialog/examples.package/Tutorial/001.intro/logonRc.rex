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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  File:    logonRc.rex                              Creation date:  06/27/2011

  Project: ooDialog Tutorial                        Last update:    06/27/2011
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Category:
    ooDialog samples

  Syntax:
    logonRc

  Purpose:
    Show a very simple version of a RcDialog subclass dialog to demonstrate the
    very basics of creating and executing a dialog using ooDialog.

  Tutorial:
    This example is a variation of the logonUser.rex example that uses a
    RcDialog subclass.

    Please read the 'Tutorial:' section in logonUser.rex, first, for the full
    version of these comments.

    All ooDialog programs are coded using the following simple, basic steps:

    1.) Subclass one of the ooDialog provided dialog classes.

    2.) Name, or dynamically create, the dialog template for the application
        specific dialog.

    3.) Execute the dialog (run the dialog.)

    4.) Collect the user's response to the dialog and act on that response.

    There are 3 main types of ooDialog supplied dialog classes.  These 3 types
    differ only in how the dialog template is supplied.

    * UserDialog class

      The dialog template is created dynamically by the programmer using the
      methods of the UserDialog class.

    * RcDialog class

      The dialog template is supplied by a resource script (.rc file.)  The
      programmer specifies the name of the resource script.

    * ResDialog classe

      The dialog template is supplied by a pre-compiled template attached to a
      resource only DLL.  The DLL name is specified by the programmer.

    This example (logonRc.rex) uses a subclass of a RcDialog to create an
    application specific program, a log on program.  Also in this directory are
    the logonUser.rex and logonRes.rex examples.  They use a subclass of a
    UserDialog and a ResDialog, respectively, to produce identical application
    specific programs.

    Note that there is very little code needed in any of the examples to produce
    a working dialog.  Most of the basic functionality is provided by ooDialog.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  -- Numbers, such as (1.), in the comments refer back to the basic steps in
  -- coding an ooDialog listed in the Tutorial: section above.


  -- Instantiate an object of our RcDialog (1.) subclass.  The subclass
  -- definition is below.  The programmer provides the name of the resource
  -- script (logonRc.rc) that contains the dialog template.  If there is only
  -- one template in the resource script, that is all the programmer need
  -- supply.

  dlg = .LogonDialog~new('logonRc.rc')

  -- Execute (3.) the dialog.  The execute() method passes the dialog template
  -- to the operarting system, runs the dialog, and does not return until the
  -- user has closed the dialog.

  ret = dlg~execute

  -- Collect (4.) the user's response and act on it.  The execute() method
  -- returns 1 if the user closed the dialog with 'okay' and returns 2 if the
  -- user canceled the dialog.

  if ret == 1 then say "The user is granted access to the system."
  else if ret == 2 then say "The user canceled, does not want to log on."
  else say "An unexpected error happened."

return ret
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

-- All ooDialog programs must 'require' the ooDialog.cls file.
::requires "ooDialog.cls"

-- (1.) Subclass the RcDialog, a class provided by the ooDialog framework.  For
-- a RcDialog class, this is all the subclass needs to define.  All other
-- functionality is provided by the ooDialog framework.
::class 'LogonDialog' public subclass RcDialog

/* - - - End Of File: logonRc.rex - - - - - - - - - - - - - - - - - - - - - - */
