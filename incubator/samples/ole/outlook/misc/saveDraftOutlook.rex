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
 * A simple demonstration of working with folders and mail items in OutLook.
 *
 * This program creates a new folder of the 'Drafts' type and places it under
 * the In Box folder.  It then creates a new e-mail and saves it.  This places
 * the saved e-mail in the default 'Drafts' folder.  Next the program creates a
 * copy of the saved e-mail and moves it to the newly created folder.  This copy
 * is then updated with some more information.  The original saved draft is
 * deleted from the defautl 'Drafts' folder.
 *
 * The program can be run any number of times.  To clean up, open up your
 * OutLook and manually delete the "Saved Mail MM" folder under your "In Box"
 * folder.
 *
 * This program may cause an OutLook warning message box to appear.  Pleas read
 * the outlook read me first file in the same directory as the outlook.frm file,
 * the ReadMe.first file.
 */

  -- Set the recipient variable to a valid e-mail address, presumably your own
  -- address:
  recipient = 'EDIT'
  recipient = 'miesfeld@gmail.com'

  if recipient == 'EDIT' then do
    say 'You must change the "recipient" variable in the first'
    say 'code line of the saveDraftOutlook.rex program to an e-mail'
    say 'address of your choice.'
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

  -- Get the Inbox folder.
  nameSpace = outLook~getNamespace("MAPI")
	inBox = nameSpace~getDefaultFolder(outLook~getConstant(olFolderInbox))

  -- If this is the first time this example is run, we will create a new folder
  -- within the Inbox folder.  But, if the program has already been run and the
  -- saved mail folder already exists, trying to create it will fail.  So, we
  -- first try to retrieve the folder, and create it when it does not exist.

  saveFolder = getFolderFromFolder(inBox, "Saved Mail MM")
  if saveFolder == .nil then do
    draftID = outLook~getConstant(olFolderDrafts)
    saveFolder = inBox~folders~add("Saved Mail MM", draftID)
  end

  -- Create an e-mail item with a subject and recipient, then save it.
  emailItem = outLook~createItem(outLook~getConstant(olMailItem))
  emailItem~subject = "Speeches Tonight MM"
  emailItem~recipients~add(recipient)
  emailItem~save

  -- The above save, saves the new e-mail item in the drafts folder.  Now we
  -- create a copy of the new e-mail and move the copy to our saved mail folder.
  savedCopy = emailItem~copy
  savedCopy~move(saveFolder)

  -- Clean up the drafts folder by deleting the original e-mail item.
  emailItem~delete

  -- Retrieve the saved e-mail from the folder we moved it to, and add to it.
  savedItems = saveFolder~items
  savedItem = .nil
  do item over savedItems
    if item~subject == "Speeches Tonight MM" then do
      savedItem = item
      leave
    end
  end

  -- Hmm ... didn't find the e-mail.  Probably not possible, but if it does we
  -- need to just quit.
  if savedItem == .nil then do
    say "Unexpected error; can not find the e-mail draft:"
    say '  Speeches Tonight MM'
    say 'Aborting.'
    return 99
  end

  -- In order to run this program any number of times, without cleaning up,
  -- we give each e-mail a successive revision number.  (In reality, we do not
  -- need to do this, Outlook will save any number of drafts with the same
  -- subject in our folder.  This just adds a little to the example and makes
  -- it easy to see that the program does something each time it is run.)
  revID = 0
  do item over savedItems
    parse value item~subject with s t m n
    if n \== '', n~datatype('W'), n > revID then revID = n
  end

  -- Add some stuff to our saved draft and then just quit.
  revID += 1
  savedItem~subject = 'Speeches Tonight MM' revID
  savedItem~body = "This is a copy.  It is copy number:" revID
  savedItem~save

  return 0

::requires '..\outlook.frm'

