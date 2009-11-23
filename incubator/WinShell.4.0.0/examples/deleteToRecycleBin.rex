/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
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

/** Demonstrates deleting files so that they are placed in the recycle bin,
 *  rather than being permanently deleted.
 *
 *  The ShellFileOp class provides an interface to the Windows SHFileOperation()
 *  function.  You can get more information on that API by doing a Google search
 *  of:  SHFileOperation MSDN
 */

  shell = .WinShell~new

  -- queryRecycleBin() fills in a .Directory object with the indexes of:
  -- objects, bytes, and error.  It returns .true on success, and .false for an
  -- error.  On error, the error index will be a COM error code, formatted as a
  -- string in hex format.  I.e,. something like: 0x80070005
  numbers = .directory~new
  say "Current size of all Recycle bins:"
  if shell~queryRecycleBin(numbers) then do
    say "  items:" numbers~objects
    say "  bytes:" numbers~bytes
  end
  else do
    say "Error:" numbers~error "Error code:" numbers~error~word(2)
  end
  say

  -- The safe.to.delete directory should be deleted at the end of this program.
  -- But, if it isn't, this prevents an error when running the program again.
  'rd /q /s safe.to.delete 1>nul 2>&1'

  -- Create some files to delete.
  ret = SysMkDir("safe.to.delete")
  if ret <> 0 then do
    say "Error creating temp directory.  Error:" ret SysGetErrortext(ret)
    say "Just going to abort."
    return ret
  end

  fileName = "tmpFile.Ok_to_delete"
  fsObj = .stream~new(fileName)
  do i = 1 to sourceline()
    fsObj~lineout(sourceline(i))
  end
  fsObj~close


  -- The file operation flags must include FOF_ALLOWUNDO or the delete operation
  -- will not place the deleted files in the recycle bin.  Actually, the default
  -- flags for a ShellFileOp object already include the allow undo flag.
  flags = "FOF_NOCONFIRMATION | FOF_ALLOWUNDO | FOF_NOCONFIRMMKDIR"

  -- Each ShellFileOp object contains a from list and a to list.  This will set
  -- the from list to fileName and the to list to safe.to.delete\testFileOne.
  -- The intent is to allow an array of names for the lists, but that is not
  -- implemented yet, so each arg has to be a single name.
  --
  -- The from and to args are optional for the new method.
  fo =  .ShellFileOp~new(fileName, "safe.to.delete\testFileOne", flags)

  -- Each of the operations, (copy, move, etc.,) also taks a from and two arg.
  -- So in this case, by using the from and to args in both the new() method and
  -- the copy method, the lists will have two items in them.  In effect fileName
  -- will be copied to testFileOne and then fileName is again copied to
  -- testFileTwo.
  if fo~copy(fileName, "safe.to.delete\testFileTwo") then say 'Copying to testFileOne and testFileTwo succeeded.'
  else say 'Copying to testFileOne and testFileTwo failed.'

  -- After each operation, the from and to lists are set back to empty.  This
  -- will then do another copy of fileName to testFileThree.
  if fo~copy(fileName, "safe.to.delete\testFileThree") then say 'Copying to testFileThree succeeded.'
  else say 'Copying to testFileThree failed.'

  -- Now delete the files, using the same object:
  prefix = "safe.to.delete\testFile"
  files = .array~of("One", "Two", "Three")

  do file over files
    if fo~delete(prefix || file) then say "Deleting" file "worked."
    else say "Deleting" file "had a problem."

    say "Size of all recycle bins now:"
    if shell~queryRecycleBin(numbers) then do
      say "  items:" numbers~objects
      say "  bytes:" numbers~bytes
    end
    else do
      say "Error:" numbers~error "Error code:" numbers~error~word(2)
    end
    say
  end

  -- Now move our original temporary source file for the copies to the safe.to.delete
  -- directory.  Then delete the whole directory.  (Which we could have done rather
  -- than deleting the files one by one.)  We need to specify the full name when we
  -- do the move operation.  Trying to do this: move fileName safe.to.delete fails,
  -- because the shell tries to move (rename) the file to the directory name, not
  -- into the directory.

  if fo~move(fileName, "safe.to.delete\" || fileName) then say "Moved" fileName "to the directory safe.to.delete"
  else say "Failed trying to move" fileName "to the directory safe.to.delete"

  if fo~delete("safe.to.delete") then say "Also deleted the directory safe.to.delete"
  else say "Tried to delete the directory safe.to.delete, but had an error."

  say "Size of all recycle bins now:"
  if shell~queryRecycleBin(numbers) then do
    say "  items:" numbers~objects
    say "  bytes:" numbers~bytes
  end
  else do
    say "Error:" numbers~error "Error code:" numbers~error~word(2)
  end
  say

return 0

::requires 'WinShell.cls'

