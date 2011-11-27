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
 */

  -- Change EDIT to a valid e-mail address.  Like tom@gmail.com
  recipient = 'EDIT'
  recipient = 'miesfeld@gmail.com'

  if recipient == 'EDIT' then do
    say 'You must edit the outLookSend.rex file so it has a valid'
    say 'e-mail address in the "recipient" variable'
    return 99
  end

  outLook = .oleObject~new("Outlook.Application")

  didActivate = .false

  explorer = outLook~activeExplorer
  if explorer == .nil then do
    nameSpace = outLook~getNamespace("MAPI")
  	inBox = nameSpace~getDefaultFolder(outLook~getConstant(olFolderInbox))
    explorer = inBox~getExplorer
    explorer~activate
    explorer~windowState = explorer~getConstant(olMinimized)
    didActivate = .true
  end

  id = outLook~getConstant(olMailItem)

  emailItem = outLook~createItem(id)

  emailItem~body = "This is only a test.  A Rexx Test"
  emailItem~subject = "NEW Test of ooRexx OLEObject outlook send mail"
  emailItem~Recipients~add(recipient)

  emailItem~save
  emailItem~send

  if didActivate then do
    z = SysSleep(3)
    explorer~close
  end

  outLook~quit

