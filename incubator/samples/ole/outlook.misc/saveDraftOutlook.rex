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


  outLook = .oleObject~new("Outlook.Application")
  say 'Version:' outLook~version

  nameSpace = outLook~GetNamespace("MAPI")
	inBox = nameSpace~getDefaultFolder(outLook~getConstant(olFolderInbox))
	newFolder = inBox~folders~add("Saved Mail", outLook~getConstant(olFolderDrafts))

  emailItem = outLook~createItem(outLook~getConstant(olMailItem))
  emailItem~subject = "Speeches"
  emailItem~save

  emailItem~recipients~add("miesfeld@gmail.com")
  savedCopy = emailItem~copy
  savedCopy~move(newFolder)
  savedCopy~body = "This is a copy"
  savedCopy~save


  /*
Set myNameSpace = outLook~GetNamespace("MAPI")
	Set myFolder = myNameSpace.GetDefaultFolder(olFolderInbox)
	Set myNewFolder = myFolder.Folders.Add("Saved Mail", olFolderDrafts)
	Set myItem = myolApp.CreateItem(olMailItem)
	myItem.Subject = "Speeches"
	Set myCopiedItem = myItem.Copy
	myCopiedItem.Move myNewFolder
  */

