/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
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


::class texttemplate public

::attribute tlines private        -- array of lines from the template source
::attribute tfilename             -- template source file name

::method init
expose tfilename tlines
use strict arg tfilename
tfile = .stream~new(tfilename)
-- read in the lines from the source template file
retc = tfile~open('read')
if retc <> 'READY:' then do
   tline = .nil
   return
   end
tlines = tfile~arrayin()
tfile~close()
return

::method transform
expose tlines
use strict arg dir, ofilename
if \dir~isA(.directory) then raise syntax 93.948 array('DIRECTORY')
ofile = .stream~new(ofilename)
retc = ofile~open('write replace')
if tlines <> .nil then do line over tlines
   newline = line
   p = newline~pos('$(')
   do while p > 0
      e = newline~pos(')', p)
      vname = newline~substr(p + 2, e - p - 2)
      str = dir[vname]                            -- try the name as is
      if str = .nil then str = dir[vname~upper()] -- try the uppercase name
      if str = .nil then str = dir[vname~lower()] -- try the lowercase name
      if str = .nil then str = ''                 -- if not found then use a zero-length str
      newline = newline~changestr('$('vname')', str)
      p = newline~pos('$(')
      end
   ofile~lineout(newline)
   end
ofile~close()
return

