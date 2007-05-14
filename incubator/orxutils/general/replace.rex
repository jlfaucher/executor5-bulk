#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Replace text in all files in a subdirectory tree.             */
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


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* This script operates on all files in the current subdirectory and          */
/* recursively on all subdirectories. It replace the text string found in     */
/* the srchtext array with the corresponding strings found in the reptext     */
/* array. The script ignores everything on the command line. The search       */
/* strings and the replacement strings are specified at the top of the        */
/* script file.                                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* this is the target search text */
srchtext = .array~new
srchtext[1] = '/* Copyright (c) 2005 Rexx Language Association. All rights reserved.         */'
srchtext[2] = 'Copyright (c) 2005 Rexx Language Association. All rights reserved.'

/* this is the replacement text */
reptext = .array~new
reptext[1] = '/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */'
reptext[2] = 'Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.'



filespec = '*.*'
retc = sysfiletree(filespec, 'files.', 'FSO')
if retc <> 0 then do
   say 'Error: Not enough memory.'
   return -1
   end
if files.0 = 0 then return 0

do i = 1 to files.0
   strm = .stream~new(files.i)
   retc = strm~open('read')
   flines = strm~arrayin
   strm~close
   txtrep = .false
   do j = 1 to srchtext~items
      do k = 1 to flines~items
         x = flines[k]~pos(srchtext[j])
         if x > 0 then do
            flines[k] = changestr(srchtext[j], flines[k], reptext[j])
            txtrep = .true
            end
         end
      end
   if txtrep = .true then do
      say files.i
      strm = .stream~new(files.i)
      retc = strm~open('write replace')
      strm~arrayout(flines)
      strm~close
      end
   end

return

