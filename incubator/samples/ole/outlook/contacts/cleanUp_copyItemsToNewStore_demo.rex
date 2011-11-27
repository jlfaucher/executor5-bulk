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

/** cleanUp_copyItems_etc_demo.rex
 *
 * This program cleans up the Outlook stuff created
 * by the 'copyItemsToNewStore.rex' demo program.
 * It requires a single arg that can be one of:
 *
 *   all folder items
 *
 * all    - Deletes the entire Outlook store created
 *           by the demo.  Which is by default:
 *            'C:\outlook1_demo_mm.pst'
 *
 * folder - Removes just the created folder, which by default is:
 *          'Demo Contacts Folder MM' in the
 *          Outlook store.
 *
 * items  - Removes all the copied Contact items from the created folder.  Which
 *          by default is: 'Demo Contacts Folder MM'
 *
 * Prints out some help if the single argument requirement is not met.
 */
use arg what

  if arg() == 0 then return printHelp()

  validArgs = .set~of('all', 'folder', 'items')
  if \ validArgs~hasIndex(what) then return printHelp()

  outLook = .oleObject~new("Outlook.Application")

  storePath =  .CN~STORE_PATH
  folderName = .CN~CONTACTS_FOLDER

  store = getStore(outLook, storePath)
  if store == .nil then do
    say 'Could not find store ('storePath').  Aborting'
    return 99
  end

  select
    when what == 'all' then do
      z = deleteStoreByPath(outLook, storePath)
    end

    when what == 'folder' then do
      z = removeFolderFromStore(store, folderName)
    end

    when what == 'items' then do
      z = emptyFolder(store, folderName)
    end

    otherwise return printHelp()
  end
  -- End select

  return 0

::requires 'outlook.frm'

::routine printHelp

  say
  say "This program cleans up the Outlook stuffed created by the"
  say "'copyItemsToNewStore.rex' demo program.  It requires a single"
  say "arg that must be exactly one of: all folder items"
  say ""
  say "all    - Deletes the entire Outlook store created by the demo."
  say "           Which is:  C:\outlook1.pst"
  say ""
  say "folder - Removes just the created folder 'Contacts Folder' in the"
  say "         Outlook store."
  say ""
  say "items  - Removes all the copied Contact items from the created"
  say "         'Contacts Folder'."
  say ""

  return 45
