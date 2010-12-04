




  shell = .OleObject~new("Shell.Application")

  ssfPROGRAMS = 2
  folder = shell~NameSpace(ssfPROGRAMS)
  folder~newFolder("My Own Group")

  z = deleteItem(folder, "My Own Group")
  z = deleteItem(folder, "My twn Group")
  return 0
  -- The parseName method returns the FolderItem object in a folder for the
  -- specified name.  If the folder contains an item with the name.  We use
  -- the method to get the folder item we just created.
  groupFolderItem = folder~parseName("My Own Group")

  -- But
  groupFolder = groupFolderItem~getFolder
  gfItemTest = groupFolder~self
  say groupFolderItem~name
  say groupFolder~title
  say groupFolderItem~path

  say 'gfItemTest path:' gfItemTest~path

  z = showFolder(groupFolder)



  j = SysSleep(3)
  say 'Going to close folder'
  j = closeFolder(groupFolder, shell)

  say 'Going to sleep 3 then delete.'
  j = SysSleep(3)

  fso = .OleObject~new("Scripting.FileSystemObject")
  fso~deleteFolder(groupFolderItem~path, .true)


::requires 'oleUtils.rex'

::routine showFolder
  use strict arg folder

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

  openVerb~doIt

  return 0

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
  else do
    say "Could not find open folder:" folder~title
    return 1
  end

-- Deletes an item in a folder using the item's name
::routine deleteItem
  use strict arg folder, itemName

  folderItem = folder~parseName(itemName)
  say "folder item:" folderItem
  return 0
