/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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

if \ .local~hasEntry('WINSHELL_VERSION') then do
  .local~WinShell_version = .Sh~version

  .local~setentry("SH", .Sh)
  .local~setentry("WINSHELL", .WinShell)
  .local~setentry("IMAGELIST", .ImageList)
  .local~setentry("SIMPLEFOLDERBROWSE", .SimpleFolderBrowse)
  .local~setentry("SHELLFILEOP", .ShellFileOp)
  .local~setentry("PATH", .Path)

end
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* class: WinShell - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A representation of the Windows Shell with methods providing access to the
    Windows Shell functionality.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'WinShell'

::method init                      external "LIBRARY SHELLUTILS WinShell_init"
::method uninit                    external "LIBRARY SHELLUTILS WinShell_uninit"
::method browseForFolder           external "LIBRARY SHELLUTILS WinShell_browseForFolder"
::method pathFromCSIDL             external "LIBRARY SHELLUTILS WinShell_pathFromCSIDL"
::method pathFromItemID            external "LIBRARY SHELLUTILS WinShell_pathFromItemID"
::method getItemID                 external "LIBRARY SHELLUTILS WinShell_getItemID"
::method getItemIDFromPath         external "LIBRARY SHELLUTILS WinShell_getItemIDFromPath"
::method getItemIDFromCSIDL        external "LIBRARY SHELLUTILS WinShell_getItemIDFromCSIDL"
::method releaseItemID             external "LIBRARY SHELLUTILS WinShell_releaseItemID"
::method openFindFiles             external "LIBRARY SHELLUTILS WinShell_openFindFiles"
::method openFolder                external "LIBRARY SHELLUTILS WinShell_openFolder"
::method closeWindow               external "LIBRARY SHELLUTILS WinShell_closeWindow"
::method addToRecentDocuments      external "LIBRARY SHELLUTILS WinShell_addToRecentDocuments"
::method clearRecentDocuments      external "LIBRARY SHELLUTILS WinShell_clearRecentDocuments"
::method emptyRecycleBin           external "LIBRARY SHELLUTILS WinShell_emptyRecycleBin"
::method selectIcon                external "LIBRARY SHELLUTILS WinShell_selectIcon"
::method loadIcon                  external "LIBRARY SHELLUTILS WinShell_loadIcon"
::method getSharedIcon             external "LIBRARY SHELLUTILS WinShell_getSharedIcon"
::method extractDefaultIcons       external "LIBRARY SHELLUTILS WinShell_extractDefaultIcons"
::method releaseIcon               external "LIBRARY SHELLUTILS WinShell_releaseIcon"
::method about                     external "LIBRARY SHELLUTILS WinShell_about"

::method _queryDiskSpace  private  external "LIBRARY SHELLUTILS WinShell_queryDiskSpace_private"
::method _queryRecycleBin private  external "LIBRARY SHELLUTILS WinShell_queryRecycleBin_private"

-- Temp method used to test things.
::method test                      external "LIBRARY SHELLUTILS WinShell_test"


/** queryDiskSpace()
 *
 * Returns the total size of a disk and the the free space in bytes.  The free
 * space includes total number of bytes free and total number of bytes free for
 * the current user.  If disk quotas are in effect, the free bytes available for
 * the user many be different than the total free bytes.  Any valid path name,
 * including UNC path names, for the disk may be used.  (Including C:)
 *
 * The information is returned in a directory object with the indexes of:
 * error, total, free, and userFree.
 */
::method queryDiskSpace
  use strict arg path

  details = self~_queryDiskSpace(path)

  obj = .directory~new
  if details~abbrev("Err:") then do
    obj~error    = details~word(2)
    obj~total    = 0
    obj~free     = 0
    obj~userFree = 0
  end
  else do
    obj~error    = 0
    obj~total    = details~word(1)
    obj~free     = details~word(2)
    obj~userFree = details~word(3)
  end
return obj


/** queryRecycleBin()
 *
 * If path is omitted, queries info for all recycle bins.  When used, path is
 * restricted to a fully qualified path and then the query is for just the
 * recycle bin on that drive.  On error, the error code is in hexadecimal
 * format.  Error codes are COM error codes.
 *
 * The info is returned in a directory object with the indexes of: error, bytes,
 * and objects.
 *
 */
::method queryRecycleBin
  use strict arg path = ""

  details = self~_queryRecycleBin(path)

  obj = .directory~new
  if details~abbrev("Err:") then do
    obj~error   = details~word(2)
    obj~objects = 0
    obj~bytes   = 0
  end
  else do
    obj~error   = 0
    obj~objects = details~word(2)
    obj~bytes   = details~word(1)
  end
return obj


/* class: ImageList- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    An abstraction of the ImageList control.  Image lists are a collection of
    same sized images and methods to manipulate those images.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ImageList'

::method destroy class             external "LIBRARY SHELLUTILS ImageList_destroy_class"

::method init                      external "LIBRARY SHELLUTILS ImageList_init"
::method uninit                    external "LIBRARY SHELLUTILS ImageList_uninit"
::method getImageSize              external "LIBRARY SHELLUTILS ImageList_getImageSize"
::method getImageCount             external "LIBRARY SHELLUTILS ImageList_getImageCount"
::method getBkColor                external "LIBRARY SHELLUTILS ImageList_getBkColor"
::method setBkColor                external "LIBRARY SHELLUTILS ImageList_setBkColor"
::method setListViewImages         external "LIBRARY SHELLUTILS ImageList_setListViewImages"
::method release                   external "LIBRARY SHELLUTILS ImageList_release"

::attribute handle get


/* class: SimpleFolderBrowse - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A SimpleFolderBrowse is an abstraction of the Windows Browse for Folder
    common dialog.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'SimpleFolderBrowse'

::method init      external "LIBRARY SHELLUTILS SimpleFolderBrowse_init"
::method uninit    external "LIBRARY SHELLUTILS SimpleFolderBrowse_uninit"
::method getFolder external "LIBRARY SHELLUTILS SimpleFolderBrowse_getFolder"
::method getItemID external "LIBRARY SHELLUTILS SimpleFolderBrowse_getItemID"

::method _setRoot  external "LIBRARY SHELLUTILS SimpleFolderBrowse_setRoot_private"

::attribute title get
::attribute title set
  expose title
  use strict arg _title
  if \ _title~isA(.String) then
    raise syntax 88.900 array ("title must be set to a string or .nil; found:" _title)

  if _title == "" then title = .nil
  else title = _title

::attribute banner get
::attribute banner set
  expose banner
  use strict arg _banner
  if \ _banner~isA(.String) then
    raise syntax 88.900 array ("banner must be set to a string or .nil; found:" _banner)

  if _banner == "" then banner = .nil
  else banner = _banner

::attribute hint get
::attribute hint set
  expose hint
  use strict arg _hint
  if \ _hint~isA(.String) then
    raise syntax 88.900 array ("hint must be set to a string or .nil; found:" _hint)

  if _hint == "" then hint = .nil
  else hint = _hint

::attribute initialFolder get
::attribute initialFolder set
  expose initialFolder
  use strict arg _initialFolder
  if \ (_initialFolder~isA(.String) | _initialFolder == .nil) then
    raise syntax 88.900 array ("initialFolder must be set to a string or .nil; found:" _initialFolder)

  if _initialFolder == "" then initialFolder = .nil
  else initialFolder = _initialFolder

::attribute root get
::attribute root set
  use strict arg _root
  self~_setRoot(_root)


/* class: ShellFileOp- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Performs operations on files in the same manner as the Windows Shell.

    The file operations are: delete, move, copy, and rename.  These operations
    can all be undone.  This means that files deleted are put in the Recycle
    Bin and can be restored by the user.  In addition, the normal GUI dialogs
    will be displayed (if desired) for the file operations.  I.e., if a large
    copy operation is being done the progress dialog will display.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ShellFileOp'

::method init            external "LIBRARY SHELLUTILS ShellFileOp_init"
::method uninit          external "LIBRARY SHELLUTILS ShellFileOp_uninit"
::method delete          external "LIBRARY SHELLUTILS ShellFileOp_delete"
::method copy            external "LIBRARY SHELLUTILS ShellFileOp_copy"
::method move            external "LIBRARY SHELLUTILS ShellFileOp_move"
::method rename          external "LIBRARY SHELLUTILS ShellFileOp_rename"

-- The file lists.  The programmer can not set these directly.
::attribute fromList get
::attribute toList   get

-- The attributes corresponding to some of the FOF_XXXX flags.  Setting these
-- attributes gives the programmer a simple-to-understand way of setting the
-- most commonly used flags.

::attribute confirmation  get        -- if false use FOF_NOCONFIRMATION
::attribute confirmation  set
  expose confirmation
  use strict arg confirm

  if \ isBoolean(confirm) then
    raise syntax 88.916 array ("1 'confirmation'", "true or false", confirm)
  confirmation = confirm

::attribute confirmMkdir  get        -- if false use FOF_NOCONFIRMMKDIR
::attribute confirmMkdir  set
  expose confirmMkdir
  use strict arg confirm

  if \ isBoolean(confirm) then
    raise syntax 88.916 array ("1 'confirmMkdir'", "true or false", confirm)
  confirmMkDir = confirm

::attribute progressGUI get          -- if false use FOF_SILENT
::attribute progressGUI set
  expose progressGUI
  use strict arg showProgress

  if \ isBoolean(showProgress) then
    raise syntax 88.916 array ("1 'progressGUI'", "true or false", showProgress)
  progressGUI = showProgress

::attribute progressSimpleGUI get    -- if true use FOF_SIMPLEPROGRESS
::attribute progressSimpleGUI set
  expose progressSimpleGUI
  use strict arg useSimple

  if \ isBoolean(useSimple) then
    raise syntax 88.916 array ("1 'progressSimpleGUI'", "true or false", useSimple)
  progressSimpleGUI = useSimple

::attribute errorGUI get             -- if false use FOF_NOERRORUI
::attribute errorGUI set
  expose errorGUI
  use strict arg useErrorGUI

  if \ isBoolean(useErrorGUI) then
    raise syntax 88.916 array ("1 'errorGUI'", "true or false", useErrorGUI)
  errorGUI = useErrorGUI

::attribute recursive get            -- if false use FOF_NORECURSION
::attribute recursive set
  expose recursive
  use strict arg recurse

  if \ isBoolean(recurse) then
    raise syntax 88.916 array ("1 'recursive'", "true or false", recurse)
  recursive = recurse

::attribute permanent get            -- if false use FOF_ALLOWUNDO
::attribute permanent set
  expose permanent
  use strict arg noRecycleBin

  if \ isBoolean(noRecycleBin) then
    raise syntax 88.916 array ("1 'permanent'", "true or false", noRecycleBin)
  permanent = noRecycleBin

-- The programmer can not set this, it is set internally if the to list contains
-- more than 1 path name.
::attribute multiDestination get     -- if true use FOF_MULTIDESTFILES

-- If the programmer sets the FOF_XXXX flags directly, this is their value,
-- otherwise it is .nil.
::attribute FOFlags get
::method setFOFlags      external "LIBRARY SHELLUTILS ShellFileOp_setFOFlags"

-- The value of the owner window handle, if set by the programmer.  Othewise
-- .nil.
::attribute ownerWindow get
::method setOwnerWindow  external "LIBRARY SHELLUTILS ShellFileOp_setOwnerWnd"

-- After a file operation, set to .true if the user canceled any operation,
-- otherwise false.  Before any file operation has been done, it is .nil.
::attribute userCanceled get

-- .nil until (if) the programmer sets it.  This is *only* used if the owner
-- window is set and the FOF_XXXX flags include FOF_SIMPLEPROGRESS.
::attribute progressDlgTitle get
::attribute progressDlgTitle set
  expose progressDlgTitle
  use strict arg title
  if \ title~isA(.string) then
      raise syntax 88.914 array (1 "'progressDlgTitle'", "String")
  progressDlgTitle = title

-- Not a permanent method of this class.  It is / was used for testing purposes.
-- Do not use this method, its results are indeterminate for the programmer.
::method myTest
  use strict arg name
  return self~name(name)


/* class: Path - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Provides access to the Shell Lightweight path utilities.  All the methods of
    this class are class methods.

    This class does not follow the pattern of the other WinShell classes in that
    it does not need the external routines loaded, and does not load them.  You
    do not instantiate an object of this class but rather use the class methods
    as utility routines.  (At least that is the intent at this point.)

    The purpose of many of the methods in this class is to make repetitive
    chores in dealing with paths a little easier.  For instance, take the task
    of adding the backslash character to a path name.  It is easy enough to do
    in Rexx:

      if path~left(1) \== '\' then
        path = path || '\'

    With the Path class this just becomes slightly easier, it is not any
    ground-shaking functionality.  <grin>

      path = .Path~addBackSlash(path)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'Path'

::method makePretty       class external "LIBRARY SHELLUTILS Path_makePretty"
::method canonicalize     class external "LIBRARY SHELLUTILS Path_canonicalize"
::method compact          class external "LIBRARY SHELLUTILS Path_compact"
::method searchAndQualify class external "LIBRARY SHELLUTILS Path_searchAndQualify"
::method quoteSpaces      class external "LIBRARY SHELLUTILS Path_quoteSpaces"
::method unquoteSpaces    class external "LIBRARY SHELLUTILS Path_unquoteSpaces"
::method removeBackslash  class external "LIBRARY SHELLUTILS Path_removeBackslash"
::method addBackslash     class external "LIBRARY SHELLUTILS Path_addBackslash"
::method removeFileSpec   class external "LIBRARY SHELLUTILS Path_removeFileSpec"
::method exists           class external "LIBRARY SHELLUTILS Path_exists"
::method isDirectory      class external "LIBRARY SHELLUTILS Path_isDirectory"
::method isDirectoryEmpty class external "LIBRARY SHELLUTILS Path_isDirectoryEmpty"
::method isRoot           class external "LIBRARY SHELLUTILS Path_isRoot"
::method isFull           class external "LIBRARY SHELLUTILS Path_isFull"
::method isNetworkPath    class external "LIBRARY SHELLUTILS Path_isNetworkPath"
::method isUNC            class external "LIBRARY SHELLUTILS Path_isUNC"
::method isUNCServer      class external "LIBRARY SHELLUTILS Path_isUNCServer"
::method isUNCServerShare class external "LIBRARY SHELLUTILS Path_isUNCServerShare"
::method getShortPath     class external "LIBRARY SHELLUTILS Path_getShortPath"
::method getLongPath      class external "LIBRARY SHELLUTILS Path_getLongPath"

-- Some doc for the above methods.  Haven't written much of it.

/** getShortPath()
 *
 * Converts a long path name to its short path name equivalent.  Note that the
 * long path name has to be a valid path or file name.  When the conversion
 * fails, the original path name is returned.  You can use relative path names
 * and single file names:
 *
 *  "..\active.windows\activatedHook.dll"  works
 *  "activatedWindows.frm"                 works
 *
 */


/* class: Sh - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A class of simple utility routines.  Each routine is a class method.

    This class does not follow the pattern of the other WinShell classes in that
    it does not need the external routines loaded, and does not load them.  You
    do not instantiate an object of this class but rather use the class methods
    as utility routines.

    For example:

    if .SH~isXP then say 'The current operating system is Windows XP.'

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'Sh'

::method version              class external "LIBRARY SHELLUTILS Sh_version_class"
::method is64Bit              class external "LIBRARY SHELLUTILS Sh_is64Bit_class"
::method is32on64Bit          class external "LIBRARY SHELLUTILS Sh_is32on64Bit_class"
::method isW2K                class external "LIBRARY SHELLUTILS Sh_isW2K_class"
::method isAtLeastW2K         class external "LIBRARY SHELLUTILS Sh_isAtLeastW2K_class"
::method isXP                 class external "LIBRARY SHELLUTILS Sh_isXP_class"
::method isXP32               class external "LIBRARY SHELLUTILS Sh_isXP32_class"
::method isXP64               class external "LIBRARY SHELLUTILS Sh_isXP64_class"
::method isAtLeastXP          class external "LIBRARY SHELLUTILS Sh_isAtLeastXP_class"
::method isW2K3               class external "LIBRARY SHELLUTILS Sh_isW2K3_class"
::method isAtLeastW2K3        class external "LIBRARY SHELLUTILS Sh_isAtLeastW2K3_class"
::method isVista              class external "LIBRARY SHELLUTILS Sh_isVista_class"
::method isAtLeastVista       class external "LIBRARY SHELLUTILS Sh_isAtLeastVista_class"


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Some useful public routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/** isBoolean()
 *
 * Tests if an object is a boolean (.true or .false.)
 *
 * Note that this function will always return true or false so that it is always
 * safe to use in an logical expression.  If no argument is supplied (which of
 * course makes no sense) then false is returned.
 *
 * @param obj   The object to test.
 *
 * Returns true if the object is strictly true, otherwise false.
 */
::routine isBoolean public
  use arg obj
  if arg(1, 'E') then do
    if obj~isA(.string) then do
      if obj~datatype('O') then return .true
    end
  end
return .false
-- End isBoolean()

/** isHandleType()
 *
 * Tests if an object is a number that can be used for a Windows handle.
 *
 * This is a temporary solution until ooRexx 3.x with its pointer class is
 * available.  Windows handles are numbers (the same size as a pointer) and it
 * is common to represent them as hex numbers.  ooDialog however used decimal
 * numbers.  This function returns true if obj is a valid decimal or hexadecimal
 * number.
 *
 * @param obj   The object to test.
 *
 * @return Returns true if the object is a whole number in either base 10 or
 * base 16, otherwise false.
 */
::routine isHandleType public
  use arg obj
  if arg(1, 'E') then do
    if obj~isA(.string) then do
      if obj~datatype('W') | obj~datatype('X') then return .true
    end
  end
return .false
-- End isBoolean()
