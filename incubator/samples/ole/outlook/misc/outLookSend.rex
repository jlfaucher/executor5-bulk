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

/**
 * A very simple example of sending an e-mail through OutLook.
 *
 * To run the program you will first need to edit the recipient variable below
 * and set it to a valid e-mail address.  Presumably your e-mail address.
 *
 * The program can be run any number of times.
 *
 * This program may cause an OutLook warning message box to appear.  Pleas read
 * the outlook read me first file in the same directory as the outlook.frm file,
 * the ReadMe.first file.
 *
 * There is an interesting error that arises from this line:
 *
 *   emailItem~recipients~add(recipient)
 *
 * which will show:
 *
 *   Error 92.901:  An unknown OLE error occurred (HRESULT=80004004).
 *
 * I struggled to find a solution to the error and finally came up with the
 * solution here.  That is to get the In Box folder.  The In Box folder itself
 * is not needed in this program.
 */

  -- Change EDIT to a valid e-mail address.  Like tom@gmail.com
  recipient = 'EDIT'
  recipient = 'bmiesfeld@san.rr.com'

  if recipient == 'EDIT' then do
    say 'You must edit the outLookSend.rex file so it has a valid'
    say 'e-mail address in the "recipient" variable'
    return 99
  end

  if \ isOORexx4OrLater() then do
   say "The OLE sample programs require ooRexx 4.0.0 or later.  Aborting"
   return 99
  end

  outLook = createOleObject("Outlook.Application", .true)
  if outLook == .nil then do
    say 'OutLook does not appear to be installed on this computer.'
    say 'The saveDraftOutlook.rex example program requires Outlook.'
    return 9
  end

  -- This is the simplest solution to the OLE error: Get the In Box folder.
  -- Note that the In Box folder is not needed for this program.  You can
  -- comment out the next 2 lines to see the  error.
  nameSpace = outLook~getNamespace("MAPI")
	inBox = nameSpace~getDefaultFolder(outLook~getConstant(olFolderInbox))

  -- Create an e-mail item, save it, and then send it.
  emailItem = outLook~createItem(outLook~getConstant(olMailItem))
  emailItem~subject = "NEW Test of ooRexx OLEObject outlook send mail"
  emailItem~recipients~add(recipient)
  emailItem~body = "This is only a test.  A Rexx Test"

  emailItem~save
  emailItem~send


::requires '..\outlook.frm'
