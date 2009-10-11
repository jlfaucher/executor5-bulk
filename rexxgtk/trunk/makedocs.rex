#!/usr/bin/rexx
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
/* Authors;                                                                   */
/*       W. David Ashley <dashley@us.ibm.com>                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


-- get the list of files
call sysfiletree './*.cpp', 'files.', 'FO'
say files.0
if files.0 = 0 then do
   say 'No files to document.'
   return
   end

do i = 1 to files.0
   say 'Processing file' files.i
   -- get the output file name
   parse var files.i outfile '.' .
   outfile = outfile || '.' || 'xml'
   -- open the input and output files
   istream = .stream~new(files.i)
   retc = istream~open('read')
   ilines = istream~arrayin()
   ostream = .stream~new(outfile)
   retc = ostream~open('write replace')
   -- parse the input file and produce the documentation
   state = 0
   do iline over ilines
      if iline~strip() = '/**' & state = 0 then do 
         state = 1
         meth = .rexxgtkmethod~new()
         end
      if iline~strip() = '**/' & state = 1 then do
         state = 0
         call printdocs meth, ostream
         end
      if state = 1 then do
         -- we found a documentation stanza
         if iline~strip() = '*' then iterate
         else if iline~pos('* Method:') > 1 then do
            parse var iline '*' 'Method:' name
            meth~name = name~strip()
            end
         else if iline~pos('* Function:') > 1 then do
            parse var iline '*' 'Function:' name
            meth~name = name~strip()
            end
         else if iline~pos('* Routine:') > 1 then do
            parse var iline '*' 'Routine:' name
            meth~name = name~strip()
            end
         else if iline~pos('@param') > 1 then do
            parse var iline '*' '@param' name desc
            meth~arg_name[meth~arg_name~items + 1] = name
            meth~arg_desc[meth~arg_desc~items + 1] = desc
            end
         else if iline~pos('@return') > 1 then do
            parse var iline '*' '@return' type
            meth~returns = type~strip()
            end
         else do
            parse var iline '*' desc
            meth~description = meth~description desc
            end
         end
      end
   -- close the input and output files
   istream~close()
   ostream~close()
   end

return


::routine printdocs
   use strict arg meth, ostream
   say '   Method:' meth~name
   ostream~lineout('')
   ostream~lineout('<section id="'meth~name'">')
   ostream~lineout('<title>'meth~name'</title>')
   ostream~lineout('<indexterm><primary>'meth~name'</primary></indexterm>')
   ostream~lineout('<para>'meth~description'</para>')
   ostream~lineout('<para><emphasis role="bold">Arguments:</emphasis></para>')
   if meth~arg_name~items() = 0 then do
      ostream~lineout('<para>None.</para>')
      end
   else do
      ostream~lineout('<variablelist>')
      do i = 1 to meth~arg_name~items()
         ostream~lineout('   <varlistentry>')
         ostream~lineout('      <term>'meth~arg_name[i]'</term>')
         ostream~lineout('      <listitem><para>'meth~arg_desc[i]'</para></listentry>')
         ostream~lineout('   </varlistentry>')
         end
      ostream~lineout('</variablelist>')
      end
   ostream~lineout('<para><emphasis role="bold">Returns:</emphasis></para>')
   ostream~lineout('<para>'meth~returns'</para>')
   ostream~lineout('</section>')
   return


::class rexxgtkmethod
::attribute name
::attribute description
::attribute returns
::attribute arg_name
::attribute arg_desc
::method init
   expose name description returns arg_name arg_desc
   use strict arg
   name = ''
   description = ''
   returns = ''
   arg_name = .array~new()
   arg_desc = .array~new()
   return

