#!/usr/local/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Simple clone of the Unix touch command.                       */
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/* Author: W. David Ashley                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


call RxFuncAdd 'SysLoadFuncs', 'rexxutil', 'SysLoadFuncs'
call SysLoadFuncs

/* parse the command line arguments into a .array similar to the way C does */
cmdLine = arg(1)
cmdArgs = .array~new
i = 1
do while cmdLine~length() > 0
   if cmdLine~left(1) = '"' then do
      parse var cmdLine '"' argument '"' cmdLine
      end
   else do
      parse var cmdLine argument cmdLine
      end
   cmdArgs[i] = argument
   i = i + 1
   end

/* set up the default command line arguments */
path = ''
srch_option = 'FO'

/* process the the command line arguments */
if cmdArgs~items < 1 | cmdArgs~items > 2 then do
   say 'Error: Invalid command line.'
   return 12
   end
do x = 1 to cmdArgs~items
   select
      when cmdArgs[x] = '-r' & x = 1 then ,
       srch_option = 'FOS'
      when cmdArgs[x] = '-h' | cmdArgs[x] = '--help' then do
         say 'Invocation: touch.rex [-r] [-h | --help] path'
         say '  -r      recurse subdirectories'
         say '  -h      display this message'
         say '  --help  display this message'
         say '  path    path/filespec to touch'
         return 0
         end
      otherwise path = cmdArgs[x]
      end
   end
if length(path) = 0 then do
   return
   end

/* get the time in a format SysSetFileDateTime can recognise */
sdate = date('S') time()
parse var sdate sdate stime
sdate = substr(sdate, 1, 4) || '-' || substr(sdate, 5, 2) || '-' || ,
        substr(sdate, 7, 2)

/* get the directory listing */
say path
call SysFileTree path, 'files', srch_option
say 'Number of files =' files.0

/* touch the files */
do i = 1 to files.0
   call SysSetFileDateTime files.i, sdate, stime
   end

return

