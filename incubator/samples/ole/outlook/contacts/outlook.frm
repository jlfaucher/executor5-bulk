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

/** outlook.frm
 *
 * A collection of useful routines for working with Outlook through ooRexx's
 * oleObject class.  Meant to be used as a 'required' file.
 *
 * The documentation for each routine is in the comments preceding each routine.
 */


/** getDefaultContacts()
 *
 * Returns the default Contacts folder.
 *
 * @param  outLook  An OutLook application object.
 *
 * @return  The Outlook folder object that contains all the contact items in the
 *          default address book.
 */
::routine getDefaultContacts public
  use strict arg outLook

  nameSpace = outLook~getNamespace("MAPI")

  olFCID = outLook~getConstant(olFolderContacts)
  contactsFolder = nameSpace~getDefaultFolder(olFCID)
  return contactsFolder


/** printFolders()
 *
 * Prints to the console information about all the 'Stores' in the current
 * OutLook session, including all the Folders contained in each Store.
 *
 * @param  outLook  An OutLook application object.
 *
 * @return  0, always.
 */
::routine printFolders public
  use strict arg outLook
  stores = outLook~session~stores

  do store over stores
    say "Dispaly name:" store~displayName
    say "File path:   " store~filePath
    say "ID:          " store~storeID

    root = store~getRootFolder
    say "  Root folder path:" root~folderPath
    j = enumerateFolders(root, "  ")
    say
  end
  return 0


/** enumerateFolders()
 *
 * Prints to the console information about all the folders contained with in
 * the specified folder, recursively.
 *
 * @param  rFolder  An OutLook folder object.  All folders contained within this
 *                  folder are recursively enumerated.
 *
 * @return  0, always.
 *
 * @note    enumerateFolders() is not public by design.
 */

::routine enumerateFolders
  use strict arg rFolder, pad

  folders = rFolder~folders
  count = folders~count
  pad ||= "  "
  if count > 0 then do folder over folders
    say pad || folder~name
    say pad || folder~folderPath
    j = enumerateFolders(folder, pad)
  end
  else do
    say pad || 'End of folder branch'
  end
  return 0


/** createNewStore()
 *
 * Adds a new Store to OutLook.
 *
 * @param  outLook  An OutLook application object.
 *
 * @param fileName  The full path and file name of the backing file for the
 *                  Store.  If the path contains spaces, be sure to enclose the
 *                  entire path name in quotes.
 *
 * @return  0 for success, 99 if OutLook already contains a Store for the named
 *          backing file.
 */
::routine createNewStore public
  use strict arg outLook, fileName

  -- Check if the store already exists
  stores = outLook~session~stores
  do store over stores
    if fileName~caselessCompare(store~filePath) == 0 then do
      say 'An Outlook store for:'
      say '  ' fileName
      say 'already exists.'
      return 99
    end
  end

  nameSpace = outLook~getNamespace("MAPI")
  nameSpace~addStore(fileName)

  return 0


/** getStore()
 *
 * Finds and returns a Store in the current session of OutLook.  The Store is
 * located through the path name of its backing file.
 *
 * @param  outLook    An OutLook application object.
 *
 * @param  storePath  The full path and file name of the backing file for the
 *                    desired Store.
 *
 * @return The Store object if found, .nil if not found.
 */
::routine getStore public
  use strict arg outLook, storePath

  stores = outLook~session~stores
  do store over stores
    if storePath~caselessCompare(store~filePath) == 0 then do
      return store
    end
  end

  return .nil

/** getStoreIndex()
 *
 * Given the file path name of a Store, returns its
 * index in the Stores collection.
 *
 * @param  outLook    An OutLook application object.
 * @param  storePath  The path name of the Store.
 *
 * @returns  The index of the Store on success,
 *           otherwise 0.
 */
::routine getStoreIndex public
  use strict arg outLook, storePath

  stores = outLook~session~stores
  count = stores~count
  do i = 1 to count
    store = stores[i]
    if storePath~caselessCompare(store~filePath) == 0 then return i
  end

  return 0


/** deleteStoreByPath()
 *
 * Deltes a Store from the Stores collection by searching for the Store using
 * the file name of the store.
 *
 * @param  outLook    An OutLook application object
 * @param  storePath  The path / file name of the Store
 *
 * @return  True on success, false on error.
 *
 * @note  It is not possible to delete the physical file of the Store through
 *        the OutLook object.  In addition, the MAPI spooler keeps a file lock
 *        on the file which will not be released until the process (this process
 *        ends.)  Therefore it is simply not possible to programmaticaly delete
 *        the physical file in this method.
 */
::routine deleteStoreByPath public
  use strict arg outLook, storePath

  index = getStoreIndex(outLook, storePath)
  if index == 0 then do
    say "Could not find store file:" storePath
    return .false
  end

  store = outLook~session~stores[index]

  nameSpace = outLook~getNamespace("MAPI")
  nameSpace~removeStore(store~getRootFolder)
  z = SysSleep(1)

  if getStoreIndex(outLook, storePath) <> 0 then do
    say "Failed to delete Outlook store:" storePath
    return .false
  end
  else do
    say "Deleted Outlook store:" storePath
  end

  return .true


/** getFolderFromStore()
 *
 * Finds and returns a folder within a store.
 *
 * @param  store   The Store object to search for the folder.
 * @param  name    The name of the folder to search for.
 *
 * @return The folder if found, otherwise .nil.
 */
::routine getFolderFromStore public
  use strict arg store, name

  root = store~getRootFolder
  folders = root~folders

  do folder over folders
    if folder~name == name then return folder
  end

  return .nil


/** addFolderToStore()
 *
 * Adds a new folder to a Store.
 *
 * @param  store   The Store object within which to add the folder.
 * @param  name    The name for the folder being added.
 * @param  type    The Outlook type constant for the folder.
 *
 * @return The newly added folder object.
 *
 * @remarks  The 'type' argument must be one of the OutLook folder type
 *           constants, such olFolderDrafts, olFolderInbox, or olFolderContacts.
 */
::routine addFolderToStore public
  use strict arg store, name, type

  folders = store~getRootFolder~folders
  folder = folders~add(name, type)

  return folder


/** removeFolderFromStore()
 *
 * Removes a folder from the specified Store.
 *
 * @param  store   The Store object within which to remove the folder.
 * @param  name    The name for the folder being removed.
 *
 * @return  True on success, false on failure.
 */
::routine removeFolderFromStore public
  use strict arg store, name

  root = store~getRootFolder
  folders = root~folders

  do folder over folders
    if folder~name == name then do
      say 'Removing folder:' name
      folder~delete
      return .true
    end
  end

  return .false


/** emptyFolder()
 *
 * Removes all items from a Contacts folder.
 *
 * @param  store       The Store object containing the folder being emptied.
 * @param  folderName  The name for the folder being emptied.
 *
 * @return  True on success, false on error.
 *
 * @remarks  This function was implemented for working with OutLook contacts and
 *           as such has some assumptions that the folder being emptied is a
 *           Contacts folder.  I.e., the items in non-contacts folders probably
 *           do not have a 'fullName' property.
 *
 *           So, the function does a check that the folder is most likely a
 *           Contacts folder and aborts if it is not.
 */
::routine emptyFolder public
  use strict arg store, folderName

  folder = getFolderFromStore(store, folderName)
  if folder \== .nil then do
    olCIid = folder~getConstant(olContactItem)

    if folder~defaultItemType \== olCIid then do
      say 'Folder ('folder~name') does not appear to be a Contacts folder'
      say 'Aborting.'
      return .false
    end

    items = folder~items
    do while items~count > 0
      say 'Removing contact item:' items[1]~fullName
      items~remove(1)
    end
  end

  return .true

/** Class:: CN
 *
 * This is a simple class of constants.  It is used to produce what should be
 * unique names for a Store and a Folder within a Store.  The example programs
 * that use this framework use these constant names so that the example programs
 * will not interfer with a user's regular Stores and Folders.
 */
::class 'CN' public

::constant STORE_PATH           'C:\outlook1_demo_mm.pst'
::constant CONTACTS_FOLDER      'Demo Contacts Folder MM'
