#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Convert a text file into a Rexx variable assignment.          */
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
/* This script opens the file specified on the command line and converts the  */
/* contents of the file to a Rexx assignment statement. The assignment        */
/* statement is then sent to stdout.                                          */
/*                                                                            */
/* You should invoke the script like this:                                    */
/*                                                                            */
/*    text2rexx.rex varname filename                                          */
/*                                                                            */
/* The varname is the name of a Rexx variable that will be the target of the  */
/* assignment statement. Filename is the name of the file to convert.         */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* get the input arguments and open files */
parse arg varname ifilename .
if ifilename = '' | varname = '' then do
   say 'Error: Invalid input arguments.'
   return
   end
ifile = .stream~new(ifilename)
if ifile~open('read') <> 'READY:' then do
   say 'Error: Cannot open' ifilename'.'
   return
   end

/* read in the input file */
ilines = ifile~makearray('LINES')
if ilines~items() = 0 then do
   say 'Error:' ifilename 'is empty.'
   return
   end

/* now go through the file and convert it */
say varname '= ,'
do i = 1 to ilines~items()
   ilines[i] = ilines[i]~changestr('"', '""')
   if i = ilines~items() then say '"' || ilines[i] || '"'
   else say '"' || ilines[i] || '" ,'
   end

/* cleanup */
ifile~close()
return

