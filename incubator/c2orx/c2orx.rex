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


if arg() > 1 then do
   -- get the input from the arguments
   use strict arg prefix, argstr, incl_cself = .false, incl_oself - .false
   myfunc = .orxfunc~new(prefix)
   myfunc~incl_cself = incl_cself
   myfunc~incl_oself = incl_oself
   end
else do
   -- get the input from the user
   answer = ''
   do while answer = ''
      say 'What ooRexx function prefix do you want? (must be non-blank)'
      answer = linein()
      end
   myfunc = .orxfunc~new(answer)
   answer = ''
   do while answer = ''
      say 'Enter the C function prototye:'
      answer = linein()
      end
   argstr = answer
   say 'Add a CSELF argument? (no | yes)'
   answer = linein()~upper()
   if answer~substr(1, 1) = 'Y' then myfunc~incl_cself = .true
   say 'Add a OSELF argument? (no | yes)'
   answer = linein()~upper()
   if answer~substr(1, 1) = 'Y' then myfunc~incl_oself = .true
   end

-- get the C function prefix and argument list
parse var argstr prefix '(' arglist ')' .
myfunc~org_retv = prefix
prefix = convertarg(prefix, 0)
parse var prefix myfunc~retv ',' myfunc~name .

-- parse out each argument of the C function
numargs = 0
parse var arglist funcarg ',' arglist
do while funcarg <> ''
   numargs += 1
   myfunc~org_args~append(funcarg)
   myfunc~args~append(convertarg(funcarg, numargs))
   parse var arglist funcarg ',' arglist
   end

-- now create the new ooRexx function
funclines = .array~new()
if myfunc~incl_oself then numargs += 1
if myfunc~incl_cself then numargs += 1
funclines~append('RexxRoutine'numargs'(')
funclines~append('    'myfunc~prefix || myfunc~name',' myfunc~retv',')
do farg over myfunc~args
   funclines~append('    'farg',')
   end
if myfunc~incl_oself then funclines~append('    OSELF, oself,')
if myfunc~incl_cself then funclines~append('    CSELF, cself,')
funclines[funclines~items] = funclines[funclines~items]~substr(1, funclines[funclines~items]~length - 1) || ')'
funclines~append('{')
if myfunc~org_retv~pos('void') then funclines~append('    'myfunc~retv 'retv = 0;')
else funclines~append('    'myfunc~retv 'retv;')
funclines~append('')
list = ''
do argname over myfunc~args
   if list~length > 0 then list = list','
   parse var argname . ',' argname
   list = list argname~strip
   end
if myfunc~org_retv~pos('void') then do
   funclines~append('    'myfunc~name'('list~strip');')
   end
else do
   funclines~append('    retv =' myfunc~name'('list~strip');')
   end
funclines~append('    return retv;')
funclines~append('}')

-- output the new ooRexx function
do line over funclines
   say line
   end
return


::class orxfunc public

::attribute prefix
::attribute name
::attribute retv
::attribute org_retv
::attribute args
::attribute org_args
::attribute incl_oself
::attribute incl_cself

::method init
use strict arg self~prefix
self~name = ''
self~retv = ''
self~org_retv = ''
self~args = .array~new()
self~org_args = .array~new()
self~incl_oself = .false
self~incl_cself = .false
return


::routine convertarg public
use strict arg argstr, argnum
-- find the base type
select
   when argstr~pos('void') > 0 then do
      if argstr~pos('*') > 0 then orxtype = 'POINTER'
      else orxtype = 'int32_t'
      end
   when argstr~pos('char') > 0 then do
      if argstr~pos('*') > 0 then orxtype = 'CSTRING'
      else if argstr~pos('[') > 0 then orxtype = 'CSTRING'
      else do
         if argstr~pos('unsigned') > 0 then orxtype = 'uint8_t'
         else orxtype = 'int8_t'
         end
      end
   when argstr~pos('short') > 0 then do
      if argstr~pos('*') > 0 then orxtype = 'POINTER'
      else if argstr~pos('[') > 0 then orxtype = 'POINTER'
      else do
         if argstr~pos('unsigned') > 0 then orxtype = 'uint16_t'
         else orxtype = 'int16_t'
         end
      end
   when argstr~pos('int') > 0 then do
      if argstr~pos('*') > 0 then orxtype = 'POINTER'
      else if argstr~pos('[') > 0 then orxtype = 'POINTER'
      else do
         if argstr~pos('unsigned') > 0 then orxtype = 'uint32_t'
         else orxtype = 'int32_t'
         end
      end
   when argstr~pos('long long') > 0 then do
      if argstr~pos('*') > 0 then orxtype = 'POINTER'
      else if argstr~pos('[') > 0 then orxtype = 'POINTER'
      else do
         if argstr~pos('unsigned') > 0 then orxtype = 'uint64_t'
         else orxtype = 'int64_t'
         end
      end
   when argstr~pos('long') > 0 then do
      parse source os .
      if argstr~pos('*') > 0 then orxtype = 'POINTER'
      else if argstr~pos('[') > 0 then orxtype = 'POINTER'
      else if os~pos('Windows') > 0 then do
         if argstr~pos('unsigned') > 0 then orxtype = 'uint64_t'
         else orxtype = 'int64_t'
         end
      else do
         if argstr~pos('unsigned') > 0 then orxtype = 'uint32_t'
         else orxtype = 'int32_t'
         end
      end
   when argstr~pos('float') > 0 then do
      if argstr~pos('*') > 0 then orxtype = 'POINTER'
      else if argstr~pos('[') > 0 then orxtype = 'POINTER'
      else orxtype = 'float'
      end
   when argstr~pos('double') > 0 then do
      if argstr~pos('*') > 0 then orxtype = 'POINTER'
      else if argstr~pos('[') > 0 then orxtype = 'POINTER'
      else orxtype = 'double'
      end
   otherwise orxtype = 'RexxObjectPtr'
   end
-- find the arg name
argname = argstr~word(argstr~words())
do while argname~substr(1, 1) = '*'
   argname = argname~substr(2)
   end
i = argname~pos('[')
if i > 0 then argname = argname~substr(1, i - 1)
select
   when argname = 'char'   then argname = 'arg'argnum
   when argname = 'short'  then argname = 'arg'argnum
   when argname = 'int'    then argname = 'arg'argnum
   when argname = 'long'   then argname = 'arg'argnum
   when argname = 'float'  then argname = 'arg'argnum
   when argname = 'double' then argname = 'arg'argnum
   when argname = 'void'   then argname = ''
   when argname = ''       then argname = 'arg'argnum
   otherwise nop
   end
-- return th new Rexx argument string
return orxtype',' argname

