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

/**  workWithContacts.rex
 *
 * Short example of accessing the contact
 * information in an OutLook contancts list.
 *
 * Lists the names of all the contacts.
 */

  -- Get the OutLook application object and its
  -- MAPI namespace object.
  outLook = .oleObject~new("Outlook.Application")
  nameSpace = outLook~getNamespace("MAPI")

  -- Get the olFolderContacts constant and use it
  --  to get the Contacts folder object.
  olFCID = outLook~getConstant(olFolderContacts)
  contactsFolder = nameSpace~getDefaultFolder(olFCID)

  -- Now get all the 'items' in the folder.
  items = contactsFolder~items

  -- 'items' is an oleObject that is a proxy for
  --  the COM object that is all the items in
  --  the contacts folder.  That it is an
  --  oleObject is demonstrated here
  say 'Got the contacts items object:' items
  say

  -- The methods of the COM object can be invoked
  -- on the oleObject.  One of the methods of the
  -- COM items object is getFirst() as shown here:
  contact = items~getFirst
  say 'First contact in my contacts:' contact~fullName
  say

  -- However, the oleObject class allows you to
  -- treat most COM objects that are collections as
  -- an ooRexx array.  This is not always true, but
  -- most often this works:
  say 'List my meager contacts.'
  do contact over items
    say ' Contact:' contact~fullName
  end
  say

  -- To further demonstrate this, an ooRexx
  -- array has the items() method to get the
  -- count of items in the array.  But this
  -- does not work.  If you uncomment the
  -- next line, you will get an error:

  --count = items~items

  -- However, the COM object has the count()
  -- method to get the number of items in the
  -- collection.  This does work:
  count = items~count
  say 'The number of contacts is:' count
  say

  -- One more example of treating the COM
  -- object 'items' as an array.  This does
  -- work:
  say 'The third contact is :' items[3]~fullName

