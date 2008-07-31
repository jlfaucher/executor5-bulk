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

/* Sample using the recent document methods */

  shell = .WinShell~new

  -- The underlying Windows API has no return code to indicate failure or
  -- success.  So, there is no way to tell if this works or not, except by
  -- looking at the recent docs.

  parse source . . fileSpec
  say 'Adding' fileSpec 'to Recent Documents.'
  say
  shell~addToRecentDocuments(fileSpec)

  say 'Check your recent docs for the addition, then'
  say 'hit enter to continue'
  pull a

  j = SysFileTree('*.rex', files., 'FOS')
  say 'Adding all *.rex files from this directory and down to Recent Docuents'
  say
  do i = 1 to files.0
    say '  adding:' files.i
    shell~addToRecentDocuments(files.i)
  end

  say
  say 'Check your recent docs for the additional files, then'
  say 'hit enter to continue'
  pull a

  say
  say 'The next example will clear all documents from the Recent Documents'
  say 'list.'
  say
  say 'If you do not want this to happen, type: no<enter>'
  say 'Otherwise just hit enter to continue'
  say
  pull answer
  if answer~left(1)~upper == 'N' then return 0

  shell~clearRecentDocuments
  say 'Your Recent Documents is now empty.'

::requires 'WinShell.cls'
