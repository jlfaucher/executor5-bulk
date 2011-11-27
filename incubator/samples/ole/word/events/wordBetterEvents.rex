/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2008 Rexx Language Association. All rights reserved.    */
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

/* Simple demonstration of connecting events without using a subclass of
 * OLEObject.  The implications of this, which may not be at first apparent, are
 * that events can now be connected to OLEObjects that are created indirectly.
 * This used to be impossible to do.
 *
 * In addition, OLEObjects can now be connected and disconnected from event
 * notifications throughout the life cycle of the OLEObject.  This can be done
 * repeatedly, as often as desired.
 *
 * These new methods are added to OLEObject to support this functionality:
 *
 *   addEventMethod(methodName, methodObj)
 *   removeEventMethod(methodName)
 *   isConnectable
 *   isConnected
 *   connectEvents
 *   disconnectEvents
 *   removeEventHandler
 *
 *   getKnownEvents() is now enhanced.
 *
 * addEventMethod()
 *   used to add an event method to an existing OLEObject.
 *
 * removeEventMethod()
 *   used to remove an event method that was previously added to an existing
 *   OLEObject.
 *
 * isConnectable()
 *   used to determine if the OLEObject object supports connections, or not.  It
 *   connections are not supported, trying to connect event methods will not
 *   succeed.
 * Returns true or false.
 *
 * isConnected() \
 *   used to determine if the OLEObject object is currently connected, is
 *   currently set up to receive event notifications.
 * Returns true or false.
 *
 * connectEvents()
 *   used to tell the OLEObject to make the event connection.  This can be done
 *   at any time in the life cycle of an OLEObject object.  Prior to this, event
 *   connections could *only* be done at the time of object instantiation.
 * Returns true if no errors are detected, and false if errors prevent the
 * connection from being made.
 *
 * disconnectEvents()
 *   used to tell the OLEObject to drop the event connection.  Likewise, this
 *   can be done at any time in the life cycle of an OLEObject object. Prior to
 *   this, event connections could never be dropped.
 * Returns true if no errors are detected, and false if errors prevent the
 * connection from being dropped.
 *
 * removeEventHandler()
 *   disconnects (if connected) the OLEObject from event notifications and also
 *   destroys the underlying object that takes care of receiving event
 *   notifications and routing the notifications to the ooRexx object (the
 *   OLEObject.)  After this handler is destroyed, connectEvents() and
 *   getKnownEvents() will still work.  They will then first recreate an event
 *   handler and then proceed as normal.
 * Returns true if no errors are detected, and false if errors are detected.
 *
 * getKnownEvents()
 *   This existing method is now enhanced to always return the event methods for
 *   a connectable object.  Prior to this, only OLEObjects created directly, and
 *   created with the 'event' flag (WITHEVENTS or NOEVENTS) would return any
 *   known events.  This little known fact was never really documented
 *   correctly.  So, if the user did not create the OLEObject correctly, .nil
 *   would be returned for objects that actually would support event
 *   connections.
 */

  /* Before we do anything, test if MS Word is installed on this computer.  Bail
   * out if it is not installed.
   */
  say
  say 'Checking for Microsoft Word ...'
  if \ msWordIsInstalled() then do
    say 'This program demonstrates using OLEObject with Microsoft Word.'
    say 'However, it looks like Word is not installled.'
    say '  Aborting.'
    say
    return 99
  end
  say '                            ... okay.'
  say

  /* Get a Word application object.  Note that is object is not created with
   * events, and that OLeObject is not subclassed.
   */
  wordApp = .OLEObject~new("Word.Application")

  say "Automating the Word application used by" wordApp~UserName;
  say

  /* Create DocumentBeforeClose() and DocumentBeforeSave() method objects. */
  mArray = .array~new
  mArray[1] = 'use arg doc, cancel'
  mArray[2] = '  say "Received DocumentBeforeClose event."'
  mArray[3] = '  say "  Event has" arg() "parameters."'
  mArray[4] = '  say "    doc:     " pp(doc)'
  mArray[5] = '  say "    cancel:  " pp(cancel)'
  mArray[6] = '  say'
  mArray[7] = '  say "The document closing is named:" doc~name'
  mArray[8] = '  say'

  mClose = .Method~new("DocumentBeforeClose", mArray)

  mArray = .array~new
  mArray[1] = 'use arg doc, saveAsUI, cancel'
  mArray[2] = '  say "Received DocumentBeforeSave event."'
  mArray[3] = '  say "  Event has" arg() "parameters."'
  mArray[4] = '  say "    doc:     " pp(doc)'
  mArray[5] = '  say "    saveAsUI:" pp(saveAsUI)'
  mArray[6] = '  say "    cancel:  " pp(cancel)'
  mArray[7] = '  say'

  mSave = .Method~new("DocumentBeforeSave", mArray)

  /* Add the 2 event methods to the Word application object, the OLEObject
   * instance.
   */
  wordApp~addEventMethod("DocumentBeforeClose", mClose)
  wordApp~addEventMethod("DocumentBeforeSave", mSave)

  /* Set Word visible to see that a document is actually opened. */
  wordApp~visible = .true

  /* Create a new document.  The Document COM object does have an event
   * interface.  But, the only way to create a Document object is through
   * another Word object.  The programmer can not create a Document object
   * directly.  Formally, this meant it was impossible to use events with this
   * object.  (Something that VBScript and WScript have no problem doing.)
   */
  document = wordApp~documents~Add

  /* Use the isConnectable method to determine if the object supports
   * connections.
   */
  if document~isConnectable then do
    say 'Is the Document object a connectable object?' document~isConnectable

    /* Create a method for the OLEEvent_Close event. From the Word documentation
     * and experimentation, it is known that this event has no arguments.
     */
    mArray = .array~new
    mArray[1] = 'say "Received the OLEEvent_Close event."'
    mArray[2] = 'say "  Event has" arg() "parameters."'
    mArray[3] = 'say'

    mClose = .Method~new("OLEEvent_Close", mArray)

    /* Now add this method to the document object. */
    document~addEventMethod("OLEEvent_Close", mClose)

    /* Now tell the object to make an events connection.  The method returns
     * true on success, otherwise false.
     */
    if document~connectEvents then do
      j = printEvents(document, "document")
    end
    else do
      say "  The event connection failed.  Event methods will not be invoked."
      say
    end
  end

  say 'Is the Word Application object a connectable object?' wordApp~isConnectable

  /* Note that here, we print the Word events, even though the object was
   * created without events.  This never used to work.  Now, if the object
   * supports events, but was created without events, this function will get the
   * known events, if the object supports events.
   */
  j = printEvents(wordApp, "wordApp")

  /* The event connection is still not made however.  The user needs to
   * explictly ask for the connection.
   */
  wordApp~connectEvents

  /* Get the environment variable TEMP and create a filename to save the
   * document as.
   */
  fileName = VALUE( "TEMP",,ENVIRONMENT) || "\" || "OLERexx"

  /* Use "save as..." and then close the document. */
  document~saveAs(fileName)
  document~close

  /* Hide Word for the next tests so it is not so distracting. */
  wordApp~visible = .false

  /* Test the disconnectEvents method. */
  wordApp~disconnectEvents

  /* Test the removeEventMthod method */
  wordApp~removeEventMethod("DocumentBeforeClose")
  wordApp~removeEventMethod("DocumentBeforeSave")

  say 'Is the wordApp object connected to event notifications?' wordApp~isConnected
  say

  /* Now if we create a new document, save and close it, we should not get the
   * event notifications.
   */
  document2 = wordApp~documents~Add
  fileName = VALUE( "TEMP",,ENVIRONMENT) || "\" || "OLE-doc2"
  document2~saveAs(fileName)
  document2~close

  /* Create a new, different, DocumentBeforeClose() method object. */
  mArray = .array~new
  mArray[1] = 'use arg doc, cancel'
  mArray[2] = '  say "In DocumentBeforeClose()"'
  mArray[3] = '  say "The closing document is named:" doc~name'
  mArray[4] = '  say'

  say 'Reconnecting the wordApp object to event notifications.'

  /* Add the method to the workApp object and reconnect it to events. */
  mClose = .Method~new("DocumentBeforeClose", mArray)
  wordApp~addEventMethod("DocumentBeforeClose", mClose)
  wordApp~connectEvents

  say 'Is the wordApp object connected to event notifications?' wordApp~isConnected
  say

  /* Create a 3rd document and see if we get the new, different, method
   * executing.
   */
  document3 = wordApp~documents~Add
  fileName = VALUE( "TEMP",,ENVIRONMENT) || "\" || "TheThirdTesting"
  document3~saveAs(fileName)
  document3~close

  /* Test the removeEventHandler method.  This method not only disconnects the
   * OLEObject from events, but it also releases (frees, deletes) the underlying
   * event handler object.
   */
  wordApp~removeEventHandler
  say 'Removed the event handler.'
  say 'Is the wordApp object connected to event notifications?' wordApp~isConnected
  say 'Is the wordApp object a connectable object?            ' wordApp~isConnectable
  say

  /* Sometimes, it seems as through Word quits so quickly, it does not post the
   * last events.   Sleep just a little bit.
   */
  j = SysSleep( 1 )
  wordApp~quit
  j = SysSleep( 1 )


::routine pp
  return "[" || arg(1)~string || "]"

::routine printEvents
  use strict arg obj, name

  evt. = obj~getKnownEvents
  if evt. <> .nil then do
    say '  The' name 'object has these event methods:'
    do i = 1 to evt.0
      say '    Event name:' evt.i.!NAME
    end
  end
  else do
    say '  The' name 'object has no event methods.'
  end
  say
  return ""

::routine msWordIsInstalled

  signal on syntax name returnFalse

  wordApp = .OLEObject~new("Word.Application")
  wordApp~quit
  drop wordApp
  j = SysSleep(.2)
  return .true

returnFalse:
  return .false
