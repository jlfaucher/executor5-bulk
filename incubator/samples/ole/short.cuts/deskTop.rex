/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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

/** deskTop.rex
 *
 * Demonstrates using the OleObject class to perform the same functionality as
 * the WindowsProgramManager class.
 *
 * In particular, this example replicates the WindowsProgramManager desktop.rex
 * example without using the WindowsProgramManager class.
 */

  say "Creating program group and items"

  shell = .OleObject~new("Shell.Application")

  ssfPROGRAMS = 2
  programsFolder = shell~NameSpace(ssfPROGRAMS)
  programsFolder~newFolder("My Own Group")

  -- The parseName method returns the FolderItem object in a folder for the
  -- specified name, if the folder contains an item with the name.  We use
  -- the method to get the folder item we just created.
  groupFolderItem = programsFolder~parseName("My Own Group")

  -- But, a FolderItem object is not a Folder object.  To get the folder itself
  -- you need to use the 'getFolder' method of the FolderItem.
  groupFolder = groupFolderItem~getFolder

  z = createNotePadSC(groupFolderItem)
  z = createCalcSC(groupFolderItem)
  z = createWriteSC(groupFolderItem)

  say "Displaying program group.  In addition, you can go to:"
  say "  Start Menu -> Programs and see the program group: My Own Group"
  z = showFolder(groupFolder)

  z = SysSleep(3)
  say "Hit enter to continue ..."
  pull

  say 'Going to close program group'
  j = closeFolder(groupFolder, shell)

  say 'Going to sleep 3 seconds then reopen the program group.'
  z = SysSleep(3)
  z = showFolder(groupFolder)

  say "Hit enter to delete the My own write short cut ..."
  pull
  say "It may take several seconds for the short cut to actually disappear ..."

  z = deleteItem(groupFolder, "My own write.lnk")
  z = showFolder(groupFolder)
  z = SysSleep(3)

  say 'Hit enter to close and delete the program group.'
  pull
  z = closeFolder(groupFolder, shell)
  z = deleteItem(programsFolder, "My Own Group")

  return 0

-- Shows (opens in an explorer window) a folder.
::routine showFolder
  use strict arg folder

  openVerb = .nil

  -- We need the FolderItem object of the folder.  We can get this through
  -- the 'self' method.
  folderItem = folder~self

  verbs = folderItem~verbs
  do verb over verbs
    if verb~name~caselessPos("Open") \= 0 then do
      openVerb = verb
    end
  end

  if openVerb <> .nil then do
    openVerb~doIt
    return 0
  end
  else do
    say "Some error happened in showFolder()"
    return 1
  end


-- Closes the explorer window showing the specified folder.
::routine closeFolder
  use strict arg folder, shell

  groupWindow = .nil
  windows = shell~windows

  do w over windows
    if w~fullName~caselessPos("explorer.exe") <> 0 then do
      if w~locationName = folder~title then do
        groupWindow = w
        leave
      end
    end
  end

  if groupWindow <> .nil then do
    groupWindow~quit
    return 0
  end

  say "Could not find open folder:" folder~title
  return 1

-- Deletes an item in a folder using the item's name.  If the item is a folder,
-- then the folder and all its contents are deleted.
::routine deleteItem
  use strict arg folder, itemName

  folderItem = folder~parseName(itemName)

  if folderItem <> .nil then do
    fso = .OleObject~new("Scripting.FileSystemObject")

    if folderItem~isFolder then do
      fso~deleteFolder(folderItem~path, .true)
    end
    else do
      fso~deleteFile(folderItem~path, .true)
    end

    return 0
  end

  say "Could not find item:" itemName 'in folder:' folder~title
  return 1


::routine createNotePadSC
  use strict arg folderItem

  -- We need a WScript Shell object to create short cuts.
  wshShell = .OleObject~new("WScript.Shell")

  -- Create a short cut object using the file name of the short cut we want.
  -- Note that this does not, yet, create an actual file.  The short cut is in
  -- memory only.
  shortCut = wshShell~createShortcut(folderItem~path || "\My own notepad.lnk")

  -- Assign the short cut properties.
  shortCut~targetPath = "notepad.exe"
  shortCut~iconLocation = "NOTEPAD.EXE, 0"

  -- Saving the short cut object is what actually creates short cut file.
  shortCut~Save

  return 0

::routine createCalcSC
  use strict arg folderitem

  -- We need a WScript Shell object to create short cuts.
  wshShell = .OleObject~new("WScript.Shell")

  -- Create a short cut object using the file name of the short cut we want.
  -- Note that this does not, yet, create an actual file.  The short cut is in
  -- memory only.
  shortCut = wshShell~createShortcut(folderItem~path || "\My own calculator.lnk")

  -- Assign the short cut properties.
  shortCut~targetPath = "calc.exe"
  --shortCut~iconLocation = "NOTEPAD.EXE, 0"

  -- Saving the short cut object is what actually creates short cut file.
  shortCut~Save

  return 0

::routine createWriteSC
  use strict arg folderitem

  -- We need a WScript Shell object to create short cuts.
  wshShell = .OleObject~new("WScript.Shell")

  -- Create a short cut object using the file name of the short cut we want.
  -- Note that this does not, yet, create an actual file.  The short cut is in
  -- memory only.
  shortCut = wshShell~createShortcut(folderItem~path || "\My own write.lnk")

  -- Assign the short cut properties.
  shortCut~targetPath = "write.exe"
  shortCut~workingDirectory = "C:\"

  -- Saving the short cut object is what actually creates short cut file.
  shortCut~Save

  return 0
