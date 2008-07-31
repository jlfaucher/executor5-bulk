/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

/** Demonstrates some of the ways to use the WinShell methods that relate to
 *  the recycle bin.
 */

  say 'As part of this sample a recycle bin on one of you drives will be'
  say 'emptied without confirmation. (The recycle bin on drive D:, if you'
  say 'have a drive D:, if not on drive C:.)'
  say
  say 'If you do not want this to happen, type no.'
  pull answer
  if answer~left(1)~upper == 'N' then return 0

  shell = .WinShell~new

  -- See if we have more than 1 drive to work with.
  localDisks = getDisks()

  haveD = .false
  do disk over localDisks
    if disk~caselessCompare("D:\") == 0 then haveD = .true

    numbers = shell~queryRecycleBin(disk)
    say "Recycle bin on" disk
    if numbers~error == 0 then do
      say "  items:" numbers~objects
      say "  bytes:" numbers~bytes
    end
    else do
      say "Error:" numbers~error "Error code:" numbers~error~word(2)
    end
  say
  end

  numbers = shell~queryRecycleBin
  say "All Recycle bins:"
  if numbers~error == 0 then do
    say "  items:" numbers~objects
    say "  bytes:" numbers~bytes
  end
  else do
    say "Error:" numbers~error "Error code:" numbers~error~word(2)
  end
  say

  if haveD then disk = 'D:\'
  else disk = 'C:\'

  say 'Going to empty recycle bin on C:\ with confirmation'
  ret = shell~emptyRecycleBin('C:\', .true)
  say 'emptyRecycleBin returns:' ret
  say

  if \ haveD then do
    say 'No disk to use for emptyRecycleBin without confirmation'
    say 'Edit this program to see that function.'
  end
  else do
    say 'Going to empty recycle bin on D:\ with no confirmation'
    ret = shell~emptyRecycleBin('D:\')
    say 'emptyRecycleBin returns:' ret
    say
  end

  numbers = shell~queryRecycleBin
  say "Size of all recycle bins now:"
  if numbers~error == 0 then do
    say "  items:" numbers~objects
    say "  bytes:" numbers~bytes
  end
  else do
    say "Error:" numbers~error "Error code:" numbers~error~word(2)
  end

return 0

::requires 'WinShell.cls'

::routine getDisks
  disks = .array~new

  drives = SysDriveMap( , "LOCAL")
  do i = 1 to drives~words
    d = drives~word(i) || '\'
    disks~append(d)
  end

  return disks
