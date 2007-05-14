#!/usr/local/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Simple clone of the Unix head command.                        */
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
filename = ''
displaylines = 10

/* process the the command line arguments */
if cmdArgs~items < 1 | cmdArgs~items > 2 then do
   say 'Error: Invalid command line.'
   return 12
   end
do x = 1 to cmdArgs~items
   select
      when cmdArgs[x]~left(1) = '-' & x = 1 then ,
       displaylines = cmdArgs[x]~substr(2)
      otherwise filename = cmdArgs[x]
      end
   end

/* open the input file */
in = .stream~new(filename)
in~open('read')

/* read the file */
call on notready name EOF
eof = .false
do i = 1 to displaylines while eof = .false
   line = in~linein()
   say line
   end

/* close tho input file */
in~close()

return


EOF:
eof = .true
return

