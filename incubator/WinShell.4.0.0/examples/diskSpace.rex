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

/* Simple example of the WinShell~queryDiskSpace() method */
use arg cmdLine

  shell = .WinShell~new

  if arg(1, 'O') then do
    say 'Querying disk space using hard coded parameters.  You may'
    say 'need to edit some of these parameters for your system.'
    say '(In particular the "E:\" drive parameter.)'
    say

    paths = .array~new
    paths[1] = "C:\"
    paths[2] = "C:"
    paths[3] = "C:\Documents and Settings"   -- All good paths to get info on
                                             -- drive C:

    -- Need an E: drive for this.
    paths[4] = "E:\"

    -- UNC names also work, but you will need to use an UNC that is valid on
    -- your system.  This should work on any system, (but might not.)
    server = getServerName()
    paths[5] = "\\"server"\C$"

    -- Some not valid paths
    paths[6] = "\\"server"\bogus"
    paths[7] = "ZZ:\"
    paths[8] = "C:\my.name\my.dir"
    paths[9] = ""

    do path over paths
      numbers = shell~queryDiskSpace(path)
      z = print(numbers, path)
    end
  end
  else do
    say 'Querying disk space using parameters from command line'
    say
    do i = 1 to cmdLine~words
      path = cmdLine~word(i)
      numbers = shell~queryDiskSpace(path)
      z = print(numbers, path)
    end
  end

  -- And finally a regular syntax error.  Don't supply the required path
  say 'Demonstrate a syntax error using queryDiskSpace (called with no arg.)'
  say
  numbers = shell~queryDiskSpace
  z = print(numbers)


::requires 'WinShell.cls'

::routine print
  use strict arg numbers, path = ""

  say 'Disk space for' path
  if numbers~error <> 0 then do
    say 'Using' path 'for the path name results in error.'
    say '  Windows system error number:' numbers~error
  end
  else do
    say '  Disk size: ' numbers~total
    say '  Free space:' numbers~free
    say '  Available: ' numbers~userFree
  end
  say
  return 0

::routine getServerName
  'hostname | rxqueue'
  pull server
  return server

