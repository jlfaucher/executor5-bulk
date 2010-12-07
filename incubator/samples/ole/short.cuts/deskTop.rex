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
 */

  shell = .OleObject~new("Shell.Application")

  ssfPROGRAMS = 2
  programsFolder = shell~NameSpace(ssfPROGRAMS)
  programsFolder~newFolder("My Own Group")

  -- The parseName method returns the FolderItem object in a folder for the
  -- specified name.  If the folder contains an item with the name.  We use
  -- the method to get the folder item we just created.
  groupFolderItem = programsFolder~parseName("My Own Group")

  -- But, a FolderItem object is not a Folder object.  To get the folder itself
  -- you need to use the 'getFolder' method of the FolderItem.
  groupFolder = groupFolderItem~getFolder

  z = showFolder(groupFolder, .true)

  j = SysSleep(3)
  say 'Going to close folder'
  j = closeFolder(groupFolder, shell)

  say 'Going to sleep 3 seconds then reopen the folder.'
  j = SysSleep(3)
  z = showFolder(groupFolder)

  say 'Going to sleep 3 seconds then close and delete the folder.'
  j = SysSleep(3)
  z = closeFolder(groupFolder, shell)
  z = deleteItem(programsFolder, "My Own Group")


-- Shows (opens in an explorer window) a folder.
::routine showFolder
  use strict arg folder, maximized = .false

  openVerb = .nil

  -- We need the FolderItem object of the folder.  We can get this through
  -- the 'self' method.
  folderItem = folder~self

  verbs = folderItem~verbs
  do verb over verbs
    say verb~name
    if verb~name~caselessPos("Open") \= 0 then do
      openVerb = verb
      leave
    end
  end

  if openVerb <> .nil then do
    openVerb~doIt
    if maximized then do
      shell = .OleObject~new("Shell.Application")
      return maximizeFolder(folder, shell)
    end

    return 0
  end
  else do
    say "Some error happened in showFolder()"
    return 1
  end

::routine maximizeFolder
  use strict arg folder, shell

  window = .nil
  windows = shell~windows

  do w over windows
    if w~fullName~caselessPos("explorer.exe") <> 0 then do
      if w~locationName = folder~title then do
        say 'w max fullName:    ' w~fullName
        say 'w max locationName:' w~locationName
        window = w
        leave
      end
    end
  end

  if window <> .nil then do
    --j = displayKnownMethods(w, .true)

    window~top = 0
    window~left = 0
    window~width = 1288
    window~height = 972
    /*hwnd = window~hwnd
    say 'hwnd:' hwnd
    windowObject = .WindowObject~new(hwnd)
    ret = windowObject~maximize
    say 'maximize ret:' ret*/
    --windowObject~minimize
    --sysMenu = windowObject~systemMenu
    --sysMenu~processItem(sysMenu~idOf(4))
    --window~fullScreen = .true
    pull
    return 0
  end

  say "Could not find open folder:" folder~title
  return 1


-- Closes the explorer window showing the specified folder.
::routine closeFolder
  use strict arg folder, shell

  groupWindow = .nil
  windows = shell~windows

  do w over windows
    if w~fullName~caselessPos("explorer.exe") <> 0 then do
      if w~locationName = folder~title then do
        say 'w fullName:    ' w~fullName
        say 'w locationName:' w~locationName
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
    fso~deleteFolder(folderItem~path, .true)
    return 0
  end

  say "Could not find item:" itemName 'in folder:' folder~title
  return 1


::requires 'OleUtils.rex'
::requires 'winsystm.cls'
