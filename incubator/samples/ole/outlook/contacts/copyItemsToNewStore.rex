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

/** copyItemsToNewStore.rex
 *
 * Demonstration of using the oleObject
 * to work with Outlook.
 *
 * Requires 'outlook.frm' which contains
 * useful routines.
 *
 * What this program does is create a new Store, add a Contacts folder to the
 * new Store, and then populate the Contacts folder with copies of the contact
 * items in the default address book.
 *
 * Unique names are used for the new Store and Folder so you should be able to
 * run this program without interfering with your OutLook.  You can look up the
 * unique names in the 'outlook.frm' file, in the 'CN' class, in the STORE_PATH
 * and CONTACTS_FOLDER constants.
 *
 * You can use the cleanUp_copyItems_etc_demo.rex program to remove all the
 * added stuff from your OutLook.  However, you can not programmaticaly remove
 * the physical backing file for the new Store.  Once you have cleaned up the
 * added stuff, if desired, delete the backing file using normal operating
 * system commands.
 *
 * You can run this program more than once with out doing any clean up.  The new
 * contacts file will just end up containing more sets of duplicated contact
 * items.  The clean up program will delete all copies, so how many sets of
 * copies does not make any difference.
 */


  outLook = .oleObject~new("Outlook.Application")

  -- Unique names for the added store and the
  -- added contacts folder.  This allows you
  -- to use the demo programs without interfering
  -- with your Outlook stuff.
  storePath  = .CN~STORE_PATH
  folderName = .CN~CONTACTS_FOLDER

  -- Get the store if it was already created, otherwise create it.
  store = getStore(outLook, storePath)
  if store == .nil then do
    ret = createNewStore(outLook, storePath)
    if ret <> 0 then do
      say "Error trying to access store:" storePath
      say "Have to abort example program."
      return 99
    end

    store = getStore(outLook, storePath)
    if store == .nil then do
      say "Error trying to access store:" storePath
      say "Have to abort example program."
      return 99
    end
  end

  -- Get the new contacts folder, creating it if it does not already exist.
  contactsFolder = getFolderFromStore(store, folderName)
  if contactsFolder == .nil then do
    type = outLook~getConstant(olFolderContacts)
    contactsFolder = addFolderToStore(store, folderName, type)
  end

  -- Get the default Contacts folder.
  defContacts = getDefaultContacts(outLook)

  say 'Contacts folder: ' contactsFolder~name
  say 'Default contacts:' defContacts~name

  -- Duplicate each item in the default contacts list and then move the
  -- duplicate contact to the new folder.  In effect, a copy to operation.
  items = defContacts~items
  do item over items
    dup = item~copy
    dup~move(contactsFolder)
  end

::requires 'outlook.frm'
