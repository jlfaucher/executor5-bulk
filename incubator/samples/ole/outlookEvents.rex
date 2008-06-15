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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  File:    outlookEvents.rex                        Author:      Mark Miesfeld
                                                    Creation date:  09/22/2006
  Organization: RexxLA
  Project:      ooRexx Examples                     Last update:    06/15/2008
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Category:
    OLE Object, ooDialog

  Syntax:
    rexx outlookEvents.rex arg1 [string2]

  Purpose:
    Demonstrates some techniques for working with events when using .OLEObject.
    The example works with Outlook, but the techniques can be applied to working
    with other COM objects.

    The application searches the user's Inbox using two different search
    strings.  The resulting items are then listed, with date and subject.  As
    each item is listed, the user can choose to view the whole e-mail, or not.
    For each item listed, the user can move it to the folder of her choice,
    delete it, or do nothing.

  Assumes:
    Nothing, but the example is of little use if Outlook is not installed.

  Input:
    arg1     REQUIRED
      If arg1 is /p then the user is prompted for the two search strings.
      Otherwise, this is the first string (substring) to search for in the
      subject field of e-mails in the user's Outlook Inbox.

    string2  OPTIONAL
      Second string (substring) to search for in the subject field of e-mails
      in the user's Outlook Inbox.  If not given the string 're' is used.

  Returns:
    0, if the OutLook Application object was created, 99 otherwise.

  Notes:
    This program demonstrates how to use the following OLEObject methods:
      connectEvents()
      addEventMethod()
      isConnected()
      isConnectable()

    The program is relatively complex.  In order to focus on the COM event
    handling code, functions that do not involve event handling are placed at
    the very bottom of the file.

    The functions and classes that do most of the stuff related to OLEObject and
    events are towards the top of the file.

  Changes:
    Updated from a basic Outlook example to a move complicated example that
    actually does something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
use arg cmdLine

  if arg(1, 'O') then return displaySyntax()

  srchStrings = doSetup( arg( ), cmdLine )

  if \ srchStrings~isA(.array) then
    return srchStrings

  say "Creating Outlook OLEObject object."
  outLook = createOutlookObject()

  if outLook == .nil then return 99

  say 'Is the outLook object connectable?' outLook~isConnectable
  retCode = 99

  if outLook~isConnected then do

    destinationFolder = getMoveToFolder(outLook)

    searchObjectsArray = startSearch(outLook, srchStrings)

    retCode = waitForEvents(outLook, destinationFolder, searchObjectsArray)

  end

return retCode
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires 'oodWin32.cls'

/* This is the 'open' event method implemenation for the MailItem object.  It is
 * used to produce the method object used in the .OLEObject~addEventMethod().
 *
 * Note that this method is not attached to a Class object.  It is what is
 * termed a 'floating' method and is placed in the .methods builtin ooRexx
 * object.  (Please look at the "Directives" and the "Rexx Runtime Objects"
 * chapters in the ooRexx reference for more information.  Specifically at the
 * ::method directive and the .methods object sections.)
 *
 * Note also that the method here does not have the name of exactly 'open'  This
 * is to show that the method name in the .methods directly does not have to
 * match the method name used in assigning the method to an OLEObject.  This
 * would be important in programs that worked with events for multiple COM
 * objects where 2 or more of the COM objects had events with the same name.
 *
 * To see how this works, look at the getOpenEvent() of the .MethodProducer
 * class and at the workWithSearchResults() routine.
 */
::method openEventMailItem unguarded
  use arg cancel
  say 'Received the open event'
  say '  Event has' arg() 'arguments'
  say "    cancel:" pp(arg(1))
  say
  dlg = .MailItemDlg~new("outlookEvents.rc", IDD_LIST_EMAIL_DLG, , 'outlookEvents.h')
  dlg~setDateSubject(self~receivedTime, self~subject)
  ret = dlg~execute("SHOWTOP")

  if ret == 1 then return .false

  .local~mailItemIsDone = .true
  return .true


/* This class produces the event method objects on demand.
 *
 * You do not need this extra class of course.  The code that creates the method
 * objects could just be put directly where it is used.
 *
 * For this example program, I choose to put it in a class by itself so that
 * the reader could focus just on one specific thing at a time.  And so I could
 * comment this without distracting from the program flow.
 *
 * Obviously, the code to implement an event method has to come from somewhere.
 * This class shows 3 different ways to produce that code:
 *
 *  1.) Put the lines of code in an array and use that array to create the
 *      .Method object.  This is okay, if the method is simple.  It gets awkward
 *      when the implementation gets complicated, or has to be rewritten.
 *
 *  2.) Put the lines of code in a separate file.  This works well and is simple
 *      to work with.  It has this drawback, the code in the file can not use
 *      routines defined in the mail program file.  For instance, the code in
 *      the outlookEvents.src can not use the pp() method defined in this file.
 *
 *  3.) Put the method implementation in a 'floating' method.  See the
 *      openEventMailItem() method in this file for more comment on this.  This
 *      is probably the easiest to use way of implementing the event methods and
 *      has the advantage of being able to refere to other routines and classes
 *      defined in this file.
 */
::class 'MethodProducer'

::method init
  expose openEvent closeEvent folderAddEvent
  openEvent = .nil
  closeEvent = .nil
  folderAddEvent = .nil

/* The getCloseEvent() method returns a .Method object with the implementation
 * of the close() event method.
 *
 * The code for the method is simply put in an array, line for line.
 */
::method getCloseEvent
  expose closeEvent

  if closeEvent == .nil then do
    mA = .array~new
    mA[1] = "use arg cancel"
    mA[2] = "  say 'Received the close event'"
    mA[3] = "  say '  Event has' arg() 'arguments'"
    mA[4] = '  say "    cancel:" pp(cancel)'
    mA[5] = '  say'
    mA[6] = ".local~mailItemIsDone = .true"

    closeEvent = .Method~new("close", mA)
    closeEvent~setUnguarded
  end

  return closeEvent

/* The getFolderAddEvent() method returns a .Method object with the
 * implementation of the folderAdd() method.
 *
 * The code for the method is put in a separate file which is then loaded by the
 * .Method~newFile class method.
 */
::method getFolderAddEvent
  expose folderAddEvent

  if folderAddEvent == .nil then do
    folderAddEvent = .Method~newFile("outlookEvents.src")
    folderAddEvent~setUnguarded
  end

  return folderAddEvent

/* The getOpenEvent() method returns a .Method object with the implementation of
 * the open() event method.
 *
 * The code is in this file, in the openEventMailItem() method.  The object is
 * simply retrieved from the .methods directory, one of the ooRexx builtin
 * objects.
 */
::method getOpenEvent
  expose openEvent

  if openEvent == .nil then do
    openEvent = .methods~openEventMailItem
  end

  return openEvent

/* This is the main outlook application class.  It subclasses OLEObject to add
 * the AdvancedSearchComplete() event method.  This is the easiest (and probably
 * easiest to understand) way to work with events.  However, it can only be used
 * with OLEObjects that are created directly by the programmer using a ProgID.
 *
 * In this program, this way of using events can not be done with either the
 * MailItem object or the Folders object.  Neither of these objects can be
 * created directly by the programmer using a ProgID.
 */
::class 'OutlookApplication' subclass OLEObject

/* The new method of this subclass creates the proxy object for the Outlook
 * application COM object by invoking the superclass (OLEObject) using the
 * ProgID and the 'withevents' argument.  The withevents keyword causes an
 * event connection with the COM object to be initialized with no further action
 * on the programmer's part.
 *
 * If outlook is not installed, an error happens.  We just trap the error and
 * then return .nil instead of the new OLEObject object.
 */
::method new class

  signal on any name returnNil
  forward class (super) array ("Outlook.Application", "WITHEVENTS") continue

  obj = result
  return obj

returnNil:
  return .nil

  -- Attribute: event1Fired
  --   Set to true when the AdvancedSearchComplete event is received and the
  --   search tag is Search.One.
::method event1Fired attribute unguarded

  -- Attribute: event1Fired
  --   Set to true when the AdvancedSearchComplete event is received and the
  --   search tag is Search.Two.
::method event2Fired attribute unguarded

::method init

  forward class (super) continue
  self~event1Fired = .false
  self~event2Fired = .false

/* This is the implementation of the advancedSearchComplete event method.  Note
 * how it is simply a method of the outlook application subclass.
 */
::method AdvancedSearchComplete UNGUARDED
  use arg searchObject

  say "Received the AdvancedSearchComplete event."
  say "  Argument searchObject:" searchObject
  say "  Search object tag:    " searchObject~Tag
  say "  Search object filter: " searchObject~Filter
  say "  Object is connectable:" searchObject~isConnectable
  say

  if searchObject~Tag == "Search.One" then
    self~event1Fired = .true
  else if searchObject~Tag == "Search.Two" then
    self~event2Fired = .true
  else
    say 'Unexpected search complete.  Search object tag is unknown!'


/* This routine starts the advanced searches. */
::routine startSearch
  use strict arg outLook, srchStrings

  searchObjs = .array~new

  searchStr1 = "urn:schemas:mailheader:subject LIKE '%"srchStrings[ 1 ]"%'"
  searchStr2 = "urn:schemas:mailheader:subject LIKE '%"srchStrings[ 2 ]"%'"
  searchSrc = "Inbox"

  say "Going to start 2 concurrent advanced searches."
  searchObj1 = outLook~AdvancedSearch( searchSrc, searchStr1, .true, "Search.One" )
  searchObj2 = outLook~AdvancedSearch( searchSrc, searchStr2, .true, "Search.Two" )

  searchObjs[1] = searchObj1
  searchObjs[2] = searchObj2

  return searchObjs

/* This routine allows the user to pick a folder to optionally move mail items
 * to.  The actual work is done in the PickFolderDlg.
 */
::routine getMoveToFolder
  use strict arg outLook

  dlg = .PickFolderDlg~new(outLook, "outlookEvents.rc", IDD_PICKFOLDER_DLG, 'outlookEvents.h')
  dlg~execute("SHOWTOP")
  return dlg~folder

/* This routine is called after the AdvancedSeachComplete event happens.  Most
 * of the actual work of the program flows from this function.
 */
::routine processMsgs
  use arg searchObj, number, producer, destinationFolder

  results = searchObj~results

  say "Got search results for search" number", count of messages:" results~Count

  if \ .mailItemEventDisplayIsDone & results~count > 0 then do
    item1 = results~getFirst
    if item1~isConnectable then do
      j = showMailItemEvents(item1)
    end
    .local~mailItemEventDisplayIsDone = .true
  end

  j = workWithSearchResults(results, producer, destinationFolder)

  return ""

/* Prints out the MailItems event methods. */
::routine showMailItemEvents
  use strict arg mailItem

  do 3; say; end
  say 'Mail item is connectable?' mailItem~isConnectable
  j = printEvents(mailItem, "Mail item")
  do 3; say; end
  return 0

/* This routine is where the processing of all the mail items actually takes
 * place.  In terms of using events with non-top-level OLEObject objects, this
 * is the routine to exammine.  It is called with a search's results object.
 * The results object contains all the mail item objects that the search found.
 */
::routine workWithSearchResults
  use strict arg results, producer, destinationFolder

  do 3; say; end

  /* Note that we start with the last index and work down to the first index in
   * the results object.  The act of moving a mail item to a different folder
   * will change the index valuse in the results object.
   *
   * Say for instance a results object contains 5 items.  If the user moves the
   * item at index 2 and 3 to another folder, then a COM error will be generated
   * if index 4 or 5 is accessed.  Starting at the last index and working down
   * prevents that error.
   */
  do i = results~count to 1 by -1
    mailItem = results~item(i)

    say "Message" i":"
    say "  Message received time:" mailItem~receivedTime
    say "  Message subject:      " mailItem~subject~left( 60 )
    say

    if mailItem~connectEvents then do

      .local~mailItemIsDone = .false
      .local~mailItemAbort = .false

      mailItem~addEventMethod("open", producer~getOpenEvent)
      mailItem~addEventMethod("close", producer~getCloseEvent)

      /* Simply calling mailItem~display is *usually* enough to show the mail
       * item.  The mail item window is always created.  99% of the time the
       * window is set to the foreground, but, ever so often it is not set to
       * the foreground.  So, instead of just calling mailItem~display, we get
       * the inspector for the mail item and use the activate method.  This
       * guarentees that the display window pops up to the forfront. (Actually,
       * it doesn't quarentee it, sometimes it still does not come to the
       * front.)
       */
      --mailItem~display

      inspector = mailItem~getInspector
      inspector~activate

      do while \ .mailItemIsDone
        j = SysSleep(1)
      end
    end

    if .mailItemAbort then leave

    if destinationFolder \== .nil then do
      dlg = .MoveDialog~new(mailItem, destinationFolder, -
                            "outlookEvents.rc", IDD_MOVE_DLG, 'outlookEvents.h')
      dlg~execute("SHOWTOP")
    end
  end

  do 3; say; end
  return ""

/* This dialog class allows the user to pick a mail folder to move mail items
 * to.  The user can either pick an exisiting folder or create a new one.
 */
::class 'PickFolderDlg' subclass RcDialog inherit AdvancedControls MessageExtensions

::method init
  expose outLook folder

  outLook = arg(1)
  folder = .nil

  arry = .array~new(4)
  arry[1] = arg(2)
  arry[2] = arg(3)
  arry[4] = arg(4)

  forward class (super) arguments (arry) continue

::method initAutoDetection
  self~noAutoDetection

::method initDialog
  expose outLook editCtrl lvCtrl mBoxFolders

  lvCtrl = self~getListControl(IDC_LV_FOLDERS)
  editCtrl = self~getEditControl(IDC_EDIT_FOLDER)

  self~getRadioControl(IDC_RB_EXISTING)~check
  self~connectButtonNotify(IDC_RB_EXISTING, "CLICKED", onClick)
  self~connectButtonNotify(IDC_RB_CREATE, "CLICKED", onClick)

  mBoxFolders = self~getMailBoxFolders(outLook)

  lvCtrl~InsertColumn(0, "Mailbox Folder", 105)
  do fldr over mBoxFolders~allIndexes
    lvCtrl~addRow( , , fldr)
  end
  lvCtrl~select(0)

::method folder
  expose folder
  return folder

::method getMailBoxFolders private
  expose theParentFolder
  use strict arg outLook

  theParentFolder = .nil

  -- A nice enhancement would be to use a tree control instead of a list-view
  -- control and produce a tree view of all the folders.

  tble = .table~new
  mailItemType = outLook~getConstant(olMailItem)
  theParentFolder = .nil

  -- This looks for the 'Mailbox - .. ' folder.  Which is usually the defautl
  -- folder in a business installation of Outlook.
  nameSpace = outLook~getNameSpace('MAPI')
  folders = nameSpace~folders
  do i = 1 to folders~count
    say folders~item(i)~name
    if folders~item(i)~name~caselessPos("Mailbox") <> 0 then do
      theParentFolder = folders~item(i)
      leave
    end
  end

  -- However, the above method doesn't always find a folder.  If we didn't find
  -- a folder, then this method should get a folder for sure.  (In fact, we
  -- probably should just use this method and skip the above, but it is an
  -- example program.)
  if theParentFolder == .nil then do
    defaultInboxID = outLook~getConstant(olFolderInbox)
    f = nameSpace~getDefaultFolder(defaultInboxID)
    theParentFolder = f~parent
  end

  -- Make sure we found something, then iterate over all the folders, adding
  -- each folder to the returned table, indexed by its name..
  if theParentFolder \== .nil then do
    mBoxFolders = theParentFolder~folders
    mBoxCount = mBoxFolders~count

    do i = 1 to mBoxCount
      item = mBoxFolders~item(i)
      if item~defaultItemType == mailItemType then do
        tble[item~name] = item
      end
    end
  end

  return tble

::method onClick
  expose lvCtrl editCtrl
  use arg id

  id = self~loWord(id)
  if id == self~constDir[IDC_RB_EXISTING] then do
    lvCtrl~enable
    editCtrl~disable
  end
  else do
    lvCtrl~disable
    editCtrl~enable
    editCtrl~assignFocus
  end

::method ok
  expose outLook editCtrl lvCtrl mBoxFolders folder

  if self~getRadioControl(IDC_RB_EXISTING)~isChecked == "CHECKED" then do
    name = lvCtrl~itemText(lvCtrl~selected)
    folder = mBoxFolders[name]
  end
  else do
    name = editCtrl~getText~strip
    if name == "" then do
      msg = "To create a new folder you must" || '0d0a'x || -
            "a name."                         || '0d0a0d0a'x || -
            "(Use cancel if you do not want"  || '0d0a'x || -
            "to use a move to folder.)"
      j = InfoDialog(msg)
      return
    end

    folder = self~createFolderFromName(outLook, name)

    if folder == .nil then do
      msg = "Some error trying to add the new"  || '0d0a'x || -
            "folder."                           || '0d0a0d0a'x || -
            "For this example, try just using"  || '0d0a'x || -
            "an existing folder."
      j = InfoDialog(msg)
      return
    end
  end

  return self~ok:super

::method createFolderFromName private
  expose theParentFolder
  use strict arg outLook, name

  if theParentFolder == .nil then return .nil

  folderType = outLook~getConstant(olFolderInbox)

  folders = theParentFolder~folders

  if folders~isConnectable then do
    say "The Folders collection supports events"
    do 3; say; end
    j = printEvents(folders, "Folders collection")
    do 3; say; end

    producer = .MethodProducer~new

    -- Add the folderAdd() event method to our OLEObject and then connect events
    -- to that object.  See the .MethodProducer class in this file for details
    -- on how to implement event methods to be used with addEventMethod().

                        --  Method name   Method object
    folders~addEventMethod("folderAdd",   producer~getFolderAddEvent)
    folders~connectEvents

    .local~folderAddEventIsDone = .false
    newFolder = folders~add(name, folderType)

    do while \ .folderAddEventIsDone
      j = SysSleep(1)
    end
  end
  else do
    say "The Folders collections does not support events"
    newFolder = folders~add(name, folderType)
  end

  return newFolder

::method cancel
  expose folder
  folder = .nil  -- Just insurance, folder is already .nil.
  self~cancel:super

::class 'MoveDialog' subclass RcDialog inherit AdvancedControls MessageExtensions

::method init
  expose mailItem destinationFolder

  mailItem = arg(1)
  destinationFolder = arg(2)

  arry = .array~new(4)
  arry[1] = arg(3)
  arry[2] = arg(4)
  arry[4] = arg(5)

  forward class (super) arguments (arry) continue

::method initAutoDetection
  self~noAutoDetection

::method initDialog
  expose mailItem destinationFolder

  self~getRadioControl(IDC_RB_NOTHING)~check

  self~getStaticControl(IDC_ST_MD_RECEIVED)~setText(mailItem~receivedTime)
  self~getStaticControl(IDC_ST_MD_SUBJECT)~setText(mailItem~subject)
  self~getStaticControl(IDC_ST_FOLDER_NAME)~setText(destinationFolder~name)

::method ok
  expose mailItem destinationFolder

  select
    when self~getRadioControl(IDC_RB_NOTHING)~isChecked == "CHECKED" then nop

    when self~getRadioControl(IDC_RB_MOVETO)~isChecked == "CHECKED" then do
      mailItem~move(destinationFolder)
    end

    when self~getRadioControl(IDC_RB_DELETE)~isChecked == "CHECKED" then do
      -- Normally, I hate programs that prompt 'Are you sure?'  But, since this
      -- is an example running on someone else's computer, we'll prompt.

      msg = "Are you sure you want to delete" || '0d0a'x || -
            "this mail item?"                 || '0d0a0d0a'x || -
            "(You can always retrieve it"     || '0d0a'x || -
            "from the Deleted Items folder.)"
      if AskDialog(msg) == 1 then mailItem~delete
    end

    otherwise nop  -- Hmmm, this should never happen.
  end
  -- End select

  return self~ok:super

::method getRB private
  use strict arg id


::routine waitForEvents
  use strict arg outLook, destinationFolder, searchObjs

  say "Searches are started."

  if waitForEitherEvent(outLook) <> 0 then return 99

  -- Get the search objects from the passed in array.
  searchObj1 = searchObjs[1]
  searchObj2 = searchObjs[2]

  -- Set up a .local variable to control how many times a mail item's event
  -- methods are displayed.
  .local~mailItemEventDisplayIsDone = .false

  if outLook~event1Fired then finished = 1
  else if outLook~event2Fired then finished = 2
  else finished = 0

  methodProducer = .MethodProducer~new
  retCode = 0

  select
    when finished == 1 then do
      j = processMsgs(searchObj1, 'one', methodProducer, destinationFolder)

      if waitForEventTwo(outLook) == 0 then do
        j = processMsgs(searchObj2, 'two', methodProducer, destinationFolder)
      end
      else do
        say "Unexpected error, quitting."
        retCode = 99
      end
    end

    when finished == 2 then do
      j = processMsgs(searchObj2, 'two', methodProducer, destinationFolder)

      if waitForEventOne(outLook) == 0 then do
        j = processMsgs(searchObj1, 'one', methodProducer, destinationFolder)
      end
      else do
        say "Unexpected error, quitting."
        retCode = 99
      end
    end

    otherwise do
      say "Unexpected error, quitting."
      retCode = 99
    end
  end
  -- End select

  return retCode

::routine waitForEitherEvent
  use strict arg outLook

  out = 'Searching'

  do i = 1 to 3
    say out
    out = out '.'
    j = SysSleep( .01 )
  end

  timeout = 20 -- After 20 seconds, maybe we should give up.

  do while (\ outLook~event1Fired & \ outLook~event2Fired) & timeout > 0
    say out
    out = out '.'
    j = SysSleep( 1 )
    timeout -= 1
  end

  if timeout < 1 then return 99

  return 0


::routine waitForEventTwo
  use strict arg outLook
  out = 'Searching'
  timeout = 20

  do while (\ outLook~event2Fired) & timeout > 0
    say out
    out = out '.'
    j = SysSleep( 1 )
    timeout -= 1
  end

  if timeout < 1 then return 99

  return 0

::routine waitForEventOne
  use strict arg outLook
  out = 'Searching'
  timeout = 20

  do while (\ outLook~event1Fired) & timeout > 0
    say out
    out = out '.'
    j = SysSleep( 1 )
    timeout -= 1
  end

  if timeout < 1 then return 99

  return 0

/* This routine is used to print out the event methods of an OLEObject. */
::routine printEvents
  use strict arg obj, name

  evt. = obj~getKnownEvents
  if evt. <> .nil then do
    say 'The' name 'object has these event methods:'
    do i = 1 to evt.0
      say 'Event method name:' evt.i.!NAME
    end
  end
  else do
    say 'The' name 'object has no event methods.'
  end
  say
  return ""

/* Simple 'pretty print' routine borrowed from Rony Flatscher */
::routine pp public
  return "[" || arg(1)~string || "]"


/* This routine is used to trap the error if Outlook is not installled. */
::routine createOutlookObject

  obj = .OutlookApplication~new

  if obj == .nil then do
    msg = "The Outlook object could not be created." || '0d0a'x || -
          "Maybe you do not have Outlook installed?"
    j = infoDialog(msg)
  end

  return obj


/* This Dialog prompts the user for the search strings. */
::class 'PromptDialog' public subclass RcDialog inherit AdvancedControls

::method init
  arry = .array~new(4)
  arry[1] = arg(1)
  arry[2] = arg(2)
  arry[4] = arg(3)

  forward class (super) arguments (arry) continue

::method setArray
  expose srchStrings
  use strict arg srch
  srchStrings = srch

::method initAutoDetection
  self~noAutoDetection

::method initDialog
  expose srchStrings

  self~getEditControl(IDC_EDIT_SEARCH1)~setText(srchStrings[1])
  self~getEditControl(IDC_EDIT_SEARCH2)~setText(srchStrings[2])

::method ok
  expose srchStrings

  text1 = self~getEditControl(IDC_EDIT_SEARCH1)~getText~strip
  text2 = self~getEditControl(IDC_EDIT_SEARCH2)~getText~strip

  if text1 == "" | text2 == "" then do
    msg = "To run the example program you"   || '0d0a'x || -
          "must to supply 2 search strings." || '0d0a0d0a'x || -
          "Do you want to quit the program"  || '0d0a'x || -
          "completely?"
    if AskDialog(msg) == 1 then return self~cancel:super

    return -- Keep the prompt dialog running.
  end

  srchStrings[1] = text1
  srchStrings[2] = text2

  return self~ok:super

::class 'MailItemDlg' subclass RcDialog inherit AdvancedControls MessageExtensions

::method setDateSubject
  expose mailTime mailSubject
  use strict arg time subject
  mailTime = time
  mailSubject = subject

::method initDialog
  expose mailTime mailSubject

  iconControl = self~getStaticControl(IDC_ICON_QUESTION)
  if iconControl <> .nil then do
    hIcon = iconControl~loadSysIcon("QUESTION")
    ret = iconControl~setIcon(hIcon)
  end

  self~getStaticControl(IDC_ST_RECEIVED_TEXT)~setText(mailTime)
  self~getStaticControl(IDC_ST_SUBJECT_TEXT)~setText(mailSubject)
  self~connectButton(IDABORT, abort)

::method abort
  .local~mailItemAbort = .true
  return self~cancel:super


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  The routines below this point are used to implement the mechanics of doing the
  initial set up for this program.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* doSetup( argCount, cmdLine )- - - - - - - - - - - - - - - - - - - - - - - -*\

  Does the routine set up for this program, parses the command line, etc.

  For this sample program, the set up is put in a routine to allow the reader
  to focus on the .OLEObject stuff.

  Input:
    argCount REQUIRED
      The count of command line arguments.  This will be 0 if no command line
      arguments are passed and 1 if they are.

    cmdLine  OPTIONAL
      Only used if argCount is not 0.  The command line to this program.

  Returns:
    An array of search strings for the program to use, or the number -1 if the
    user asked for help on the command line.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine doSetup
  use arg argCount, cmdLine

  -- Support common 'help' parameters, /?, -h, etc..
  if isHelpRequest( cmdLine~word( 1 ) ) then
    return displaySyntax( )

  -- Set up the default search strings.
  srchStrings = .array~of( 're:', 'Status' )

  -- See if the user wants to be prompted for the search strings.
  if cmdLine~word(1)~upper == '/P' then
    return promptUser(srchStrings)

  if \ parseCmdLine( cmdLine, srchStrings ) then
    return displaySyntax( )

return srchStrings
-- End doSetup( argCount, cmdLine )

/* This routine puts up a dialog box where the user can enter the search
 *strings.
 */
::routine promptUser
  use strict arg srchStrings

  dlg = .PromptDialog~new("outlookEvents.rc", IDD_PROMPT_DLG, 'outlookEvents.h')
  dlg~setArray(srchStrings)

  ret = dlg~execute
  if ret == 1 then return srchStrings
  return .false

/* isHelpRequest( token )- - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  Checks if token is one of the common things passed on the command line to a
  program to ask for help.

  Input:
    token REQUIRED
      The token to check.

  Returns:
    True if token looks like a "help" arg, otherwis false.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine isHelpRequest
  use arg token

  helpArgs = .set~of( "?", "/?", "-?", "/H", "-H", "/HELP", "-HELP", "--HELP")

  if helpArgs~hasindex( token ) then
    return .true

return .false
-- End isHelpRequest( token )

/* displaySyntax( )- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  Displays some simple syntax help for this program.

  Input:
    None.

  Returns:
    -1, always.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine displaySyntax

  say
  say 'Syntax: rexx outlookEvents.rex arg1 [string2]'
  say
  say 'A simple example program demonstrating how to use events with .OLEObject.'
  say
  say '  The Outlook Inbox is searched, using two asynchronous searchs, for all'
  say '  messages containing 2 sub-strings.'
  say
  say '  If arg1 == /p (case insensitve) then the user is prompted for the 2'
  say '  search strings.  Otherwise arg1 is taken to be search string 1.'
  say
  say '  string2 is optional, the default is: "RE:"'
  say
  say '  Sub-strings containing spaces must be enclosed in single qoutes'
  say
  say 'Examples:'
  say
  say '  rexx outlookEvents.rex /p'
  say
  say '  rexx outlookEvents.rex RISCWatch'
  say
  say "  rexx outlookEvents.rex 'meeting notes' RISCWatch"
  say

return -1
-- End displaySyntax( )

/* parseCmdLine( cmdLine, strings )- - - - - - - - - - - - - - - - - - - - - -*\

  Parses the command line into the two strings to be used when searching the
  users Inbox.

  Single quotes are used to denote a search string with spaces.  There is no
  attempt to enforce a rigorous check of the command line.  So command lines
  like the following:

    one two 'three four'

  will produce search strings of:

    string 1: "one two"
    string 2: "three four"

  A command line of:

    'one two' three four 'five six' seven

  will produce search strings of:

    string 1: "one two"
    string 2: "three four 'five six' seven"

  Input:
    cmdLine REQUIRED
      The command line to parse.

    strings REQUIRED
      An array to return the results in.  This array is assumed to have the
      default search strings already in place.

  Returns:
    True, if no errors were detected, otherwise false.  Only an unmatched single
    quote is considered an error.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine parseCmdLine
  use arg cmdLine, strings

  quoteCount =  cmdLine~countstr( "'" )

  select
    when quoteCount == 0 then do i = 1 to cmdLine~words until i > 2
      strings[ i ] = cmdLine~word( i )
    end

    when datatype( quoteCount / 2, 'W' ) then do
      -- There is at least 1 set of single quotes.
      parse var cmdLine leading "'" token "'" trailing

      select
        when leading == '' & trailing == '' then
          strings[ 1 ] = token~strip( 'B', "'" )

        when leading == '' then do
          -- If trailing is enclosed in single quotes, strip them.
          strings[ 1 ] = token~strip( 'B', "'" )
          strings[ 2 ] = trailing~strip( 'B', "'" )
        end

        when trailing == '' then do
          strings[ 1 ] = leading
          strings[ 2 ] = token~strip( 'B', "'" )
        end

        otherwise do
          -- Too much on the command line, ignore trailing.
          strings[ 1 ] = leading
          strings[ 2 ] = token~strip( 'B', "'" )
        end
      end
      -- End select
    end

    otherwise do
      -- Unmatched single quote.
      say
      say "This is a *simple* sample program."
      say "Unmatched single quotes are not allowed in the search string(s)."
      say
      return .false
    end
  end
  -- End select

return .true
-- End parseCmdLine( cmdLine, strings )

/* - - - End Of File: outlookEvents.rex - - - - - - - - - - - - - - - - - - - */
