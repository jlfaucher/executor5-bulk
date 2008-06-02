/* Some example code to use WinShell */

  -- You always need to create one of the Shell objects, even if you call one
  -- of the external functions directly.  Creating the first shell object is
  -- what sets up the proper house keeping.

  shell = .WinShell~new

  -- WinShell has a browse for folder method that allows programmer's to use a
  -- GUI dialog to have their users select a folder.  You can not do this with
  -- fileNameDialog.
  path = shell~browseForFolder
  select
    when path == "" then say 'The user canceled'
    when path == .nil then say 'The user selected a non-file system folder (a virtual folder.)'
    otherwise say 'Selected:' path
  end
  -- End select

  -- There is also a customizable browse for folder dialog.  This is represented
  -- by its own class.  Note that if you created a SimpleFolderBrowse object,
  -- you do not need to create a WinShell object.  The SimpleFolderBrowse is a
  -- Shell object.

  -- With no options, the SimpleFolderBrowse object uses all its defaults.
  browser = .SimpleFolderBrowse~new
  path = browser~getFolder
  select
    when path == "" then say 'The user canceled'
    when path == .nil then say 'The user selected a non-file system folder (a virtual folder.)'
    otherwise say 'Selected:' path
  end
  -- End select


  -- There are 4 optional arguments to new.  Title, banner, hint, and initial
  -- folder.  The programmer can set any of them to her choice.  Banner is the
  -- text that displays right above the tree control window.  Initial folder
  -- causes the dialog to open with that folder selected.  Hint is text that
  -- goes above the buttons on the bottom of the dialog and title is ... well
  -- the title.

  title = "ooRexxTry - Save Settings"
  banner = "Select the folder where your settings will be stored"
  hint = "ooRexxTry only needs a single folder to save its settings in"
  start = "C:\Documents and Settings\All Users\Application Data"
  browser = .SimpleFolderBrowse~new(title, banner, hint, start)
  path = browser~getFolder
  select
    when path == "" then say 'The user canceled'
    when path == .nil then say 'The user selected a non-file system folder (a virtual folder.)'
    otherwise say 'Selected:' path
  end
  -- End select

  -- If you use the special name of PATH for hint, then the full path of the
  -- currently selected folder is displayed in the hint area.
  title = "ooRexxTry - Restore Settings from Unknown folder"
  banner = "Select the folder where you saved your previous settings"
  hint = "PATH"
  start = "C:\Documents and Settings\All Users\Application Data"
  browser = .SimpleFolderBrowse~new(title, banner, hint, start)
  path = browser~getFolder
  select
    when path == "" then say 'The user canceled'
    when path == .nil then say 'The user selected a non-file system folder (a virtual folder.)'
    otherwise say 'Selected:' path
  end
  -- End select

  -- You could subclass and create you own getFolder convenience function that
  -- will return a default value.
  title = "ooRexxTry - Save Settings"
  banner = "Select the folder where your settings will be stored"
  hint = "If the folder you want does not exist you can always create it"
  start = "C:\Documents and Settings\All Users\Application Data"
  browser = .MySimpleFolderBrowse~new(title, banner, hint, start)

  defaultPath = "C:\Program Files"
  path = browser~getFolder(defaultPath)
  say 'The folder to save settings in is:' path



-- Require the WinShell.cls file to use the Shell objects.
::requires 'WinShell.cls'

-- A simple subclass to over-ride getFolder().
::class 'MySimpleFolderBrowse' public subclass SimpleFolderBrowse

-- If a folder is not returned by the browse dialog for any reason, the default
-- folder value is returned to the caller.
::method getFolder
  use arg default = "C:\Documents and Settings"

  path = self~getFolder:super
  if path == "" | path == .nil then path = default

  return path
