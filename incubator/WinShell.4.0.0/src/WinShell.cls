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

-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires 'winshell' LIBRARY

/* class: Sh - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
     A class of utility methods.  All methods are class methods.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'Sh' public

::method init                 class external "LIBRARY WINSHELL Sh_init_class"
::method version              class external "LIBRARY WINSHELL Sh_version_class"
::method comctl32Version      class external "LIBRARY WINSHELL Sh_comctl32Version_class"
::method is64Bit              class external "LIBRARY WINSHELL Sh_is64Bit_class"
::method is32on64Bit          class external "LIBRARY WINSHELL Sh_is32on64Bit_class"
::method isW2K                class external "LIBRARY WINSHELL Sh_isW2K_class"
::method isAtLeastW2K         class external "LIBRARY WINSHELL Sh_isAtLeastW2K_class"
::method isXP                 class external "LIBRARY WINSHELL Sh_isXP_class"
::method isXP32               class external "LIBRARY WINSHELL Sh_isXP32_class"
::method isXP64               class external "LIBRARY WINSHELL Sh_isXP64_class"
::method isAtLeastXP          class external "LIBRARY WINSHELL Sh_isAtLeastXP_class"
::method isW2K3               class external "LIBRARY WINSHELL Sh_isW2K3_class"
::method isAtLeastW2K3        class external "LIBRARY WINSHELL Sh_isAtLeastW2K3_class"
::method isVista              class external "LIBRARY WINSHELL Sh_isVista_class"
::method isAtLeastVista       class external "LIBRARY WINSHELL Sh_isAtLeastVista_class"

/* class: Size - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Sizes have a width and a height on a coordinate system.  Which translates to
    a count of the x units (width) and a count of the y units (height.)

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'Size' public
::method init  external "LIBRARY winshell size_init"
::method cx    external "LIBRARY winshell size_cx"
::method 'cx=' external "LIBRARY winshell size_setCX"
::method cy    external "LIBRARY winshell size_cy"
::method 'cy=' external "LIBRARY winshell size_setCY"

/* class: WinShell - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A representation of the Windows Shell with methods providing access to the
    Windows Shell functionality.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'WinShell' public

::method init                      external "LIBRARY WINSHELL WinShell_init"
::method uninit                    external "LIBRARY WINSHELL WinShell_uninit"
::method browseForFolder           external "LIBRARY WINSHELL WinShell_browseForFolder"
::method pathFromCSIDL             external "LIBRARY WINSHELL WinShell_pathFromCSIDL"
::method pathFromItemID            external "LIBRARY WINSHELL WinShell_pathFromItemID"
::method getItemID                 external "LIBRARY WINSHELL WinShell_getItemID"
::method getItemIDFromPath         external "LIBRARY WINSHELL WinShell_getItemIDFromPath"
::method getItemIDFromCSIDL        external "LIBRARY WINSHELL WinShell_getItemIDFromCSIDL"
::method releaseItemID             external "LIBRARY WINSHELL WinShell_releaseItemID"
::method openFindFiles             external "LIBRARY WINSHELL WinShell_openFindFiles"
::method openFolder                external "LIBRARY WINSHELL WinShell_openFolder"
::method closeWindow               external "LIBRARY WINSHELL WinShell_closeWindow"
::method addToRecentDocuments      external "LIBRARY WINSHELL WinShell_addToRecentDocuments"
::method clearRecentDocuments      external "LIBRARY WINSHELL WinShell_clearRecentDocuments"
::method emptyRecycleBin           external "LIBRARY WINSHELL WinShell_emptyRecycleBin"
::method selectIcon                external "LIBRARY WINSHELL WinShell_selectIcon"
::method loadIcon                  external "LIBRARY WINSHELL WinShell_loadIcon"
::method getSharedIcon             external "LIBRARY WINSHELL WinShell_getSharedIcon"
::method extractDefaultIcons       external "LIBRARY WINSHELL WinShell_extractDefaultIcons"
::method releaseIcon               external "LIBRARY WINSHELL WinShell_releaseIcon"
::method about                     external "LIBRARY WINSHELL WinShell_about"
::method queryDiskSpace            external "LIBRARY WINSHELL WinShell_queryDiskSpace"


-- Temp method used to test things.
::method test                      external "LIBRARY WINSHELL WinShell_test"




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
::method queryRecycleBin external "LIBRARY WINSHELL WinShell_queryRecycleBin"
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
::class 'ImageList' public

::method destroy class             external "LIBRARY WINSHELL ImageList_destroy_class"

::method init                      external "LIBRARY WINSHELL ImageList_init"
::method uninit                    external "LIBRARY WINSHELL ImageList_uninit"
::method getImageSize              external "LIBRARY WINSHELL ImageList_getImageSize"
::method getImageCount             external "LIBRARY WINSHELL ImageList_getImageCount"
::method getBkColor                external "LIBRARY WINSHELL ImageList_getBkColor"
::method setBkColor                external "LIBRARY WINSHELL ImageList_setBkColor"
::method setListViewImages         external "LIBRARY WINSHELL ImageList_setListViewImages"
::method release                   external "LIBRARY WINSHELL ImageList_release"

::attribute handle get


/* class: SimpleFolderBrowse - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A SimpleFolderBrowse is an abstraction of the Windows Browse for Folder
    common dialog.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'SimpleFolderBrowse' public

::method init      external "LIBRARY WINSHELL SimpleFolderBrowse_init"
::method uninit    external "LIBRARY WINSHELL SimpleFolderBrowse_uninit"
::method getFolder external "LIBRARY WINSHELL SimpleFolderBrowse_getFolder"
::method getItemID external "LIBRARY WINSHELL SimpleFolderBrowse_getItemID"

::method _setRoot

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
  if \ (_banner~isA(.String) | _banner == .nil) then
    raise syntax 88.900 array ("banner must be set to a string or .nil; found:" _banner)

  if _banner == "" then banner = .nil
  else banner = _banner

::attribute hint get
::attribute hint set
  expose hint
  use strict arg _hint
  if \ (_hint~isA(.String) | _hint == .nil) then
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
::method 'root=' external "LIBRARY WINSHELL SimpleFolderBrowse_setRoot"


/* class: ShellFileOp- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Performs operations on files in the same manner as the Windows Shell.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ShellFileOp' public

::method init            external "LIBRARY WINSHELL ShellFileOp_init"
::method uninit          external "LIBRARY WINSHELL ShellFileOp_uninit"
::method delete          external "LIBRARY WINSHELL ShellFileOp_delete"
::method copy            external "LIBRARY WINSHELL ShellFileOp_copy"
::method move            external "LIBRARY WINSHELL ShellFileOp_move"
::method rename          external "LIBRARY WINSHELL ShellFileOp_rename"

-- The file lists.  The programmer can not set these directly.
::attribute fromList get
::attribute toList   get

/* TODO need to add these methods:  addSourceFiles, addDestinationFiles which
   will take a single file name or an array of file names.  Note that more than
   one file / directory destination name can only be used with FO_MOVE and
   FO_COPY.  FO_DELETE does not use a destionation obviously and more than one
   name can not be used with FO_RENAME.  For FO_RENAME you would use FO_MOVE.
*/

-- The attributes corresponding to some of the FOF_XXXX flags.  Setting these
-- attributes gives the programmer a simple-to-understand way of setting the
-- most commonly used flags.

::attribute confirmation  get        -- if false use FOF_NOCONFIRMATION
::method 'confirmation='  external "LIBRARY WINSHELL ShellFileOp_setConfirmation"

::attribute confirmMkdir  get        -- if false use FOF_NOCONFIRMMKDIR
::method 'confirmMkdir='  external "LIBRARY WINSHELL ShellFileOp_setConfirmMkDir"

::attribute progressGUI get          -- if false use FOF_SILENT
::method 'progressGUI=' external "LIBRARY WINSHELL ShellFileOp_setProgressGUI"

::attribute progressSimpleGUI get    -- if true use FOF_SIMPLEPROGRESS
::method 'progressSimpleGUI=' external "LIBRARY WINSHELL ShellFileOp_setProgressSimpleGUI"

::attribute errorGUI get             -- if false use FOF_NOERRORUI
::method 'errorGUI=' external "LIBRARY WINSHELL ShellFileOp_setErrorGUI"

::attribute recursive get            -- if false use FOF_NORECURSION
::method 'recursive=' external "LIBRARY WINSHELL ShellFileOp_setRecursive"

::attribute permanent get            -- if false use FOF_ALLOWUNDO
::method 'permanent='  external "LIBRARY WINSHELL ShellFileOp_setPermanent"

-- The programmer can not set this, it is set internally if the to list contains
-- more than 1 path name.
::attribute multiDestination get     -- if true use FOF_MULTIDESTFILES

-- If the programmer sets the FOF_XXXX flags directly, this is their value,
-- otherwise it is .nil.
::attribute FOFlags get
::method setFOFlags      external "LIBRARY WINSHELL ShellFileOp_setFOFlags"

-- The value of the owner window handle, if set by the programmer.  Othewise
-- .nil.
::attribute ownerWindow get
::method setOwnerWindow  external "LIBRARY WINSHELL ShellFileOp_setOwnerWnd"

-- .nil until (if) the programmer sets it.  This is *only* used if the owner
-- window is set and the FOF_XXXX flags include FOF_SIMPLEPROGRESS.
::attribute progressDlgTitle get
::method setProgressDlgTitle external "LIBRARY WINSHELL ShellFileOp_setProgressDlgTitle"

-- After a file operation, set to .true if the user canceled any operation,
-- otherwise false.  Before any file operation has been done, it is .nil.
::attribute userCanceled get

-- Not a permanent method of this class.  It is / was used for testing purposes.
-- Do not use this method, its results are indeterminate for the programmer.
::method myTest
  use strict arg name
  return self~name(name)


/* class: Path - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Provides access to the Shell Lightweight path utilities.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'Path' public

::method makePretty       class external "LIBRARY WINSHELL Path_makePretty"
::method canonicalize     class external "LIBRARY WINSHELL Path_canonicalize"
::method compact          class external "LIBRARY WINSHELL Path_compact"
::method searchAndQualify class external "LIBRARY WINSHELL Path_searchAndQualify"
::method quoteSpaces      class external "LIBRARY WINSHELL Path_quoteSpaces"
::method unquoteSpaces    class external "LIBRARY WINSHELL Path_unquoteSpaces"
::method removeBackslash  class external "LIBRARY WINSHELL Path_removeBackslash"
::method addBackslash     class external "LIBRARY WINSHELL Path_addBackslash"
::method removeFileSpec   class external "LIBRARY WINSHELL Path_removeFileSpec"
::method exists           class external "LIBRARY WINSHELL Path_exists"
::method isDirectory      class external "LIBRARY WINSHELL Path_isDirectory"
::method isDirectoryEmpty class external "LIBRARY WINSHELL Path_isDirectoryEmpty"
::method isRoot           class external "LIBRARY WINSHELL Path_isRoot"
::method isFull           class external "LIBRARY WINSHELL Path_isFull"
::method isNetworkPath    class external "LIBRARY WINSHELL Path_isNetworkPath"
::method isUNC            class external "LIBRARY WINSHELL Path_isUNC"
::method isUNCServer      class external "LIBRARY WINSHELL Path_isUNCServer"
::method isUNCServerShare class external "LIBRARY WINSHELL Path_isUNCServerShare"
::method getShortPath     class external "LIBRARY WINSHELL Path_getShortPath"
::method getLongPath      class external "LIBRARY WINSHELL Path_getLongPath"


