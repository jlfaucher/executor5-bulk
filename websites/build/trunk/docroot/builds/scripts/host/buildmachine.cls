#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Build ooRexx on all build machines.                           */
/*                                                                            */
/* Copyright (c) 2010-2011 Rexx Language Association. All rights reserved.    */
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


::requires 'kvmdomain.cls'


/*----------------------------------------------------------------------------*/
/* Class: buildmachines                                                       */
/*----------------------------------------------------------------------------*/

::class buildmachines public

::attribute arr

::method init
use strict arg file
lines = file_arrayin(file)
self~arr = .array~new()
do line over lines
   if line~strip() = '' then iterate
   if line~strip()~substr(1, 1) = '#' then iterate
   if line~strip()~substr(1, 2) = '--' then iterate
   parse var line osname addr userid cmd virt_flag .
   self~arr~append(.buildmachine~new(osname, addr, userid, cmd, virt_flag))
   end
return

::method build_all
use strict arg cmdargs
do machine over self~arr  -- do all the machines
   machine~do_build(cmdargs)
   end
return

::method build_one
use strict arg specific_machine, cmdargs
if specific_machine <> '' then do
   -- find the machine and do the build
   do machine over buildarr
      if specific_machine = machine~osname() then do
         machine~do_build(cmdargs)
         leave
         end
      end
   else say specific_machine 'Error: not found.'
   end
else say 'Error: no machine specified.'
return

::method copy_all
use strict arg cmdargs
do machine over self~arr  -- do all the machines
   machine~do_copy(cmdargs)
   end
return

::method copy_one
use strict arg specific_machine, cmdargs
if specific_machine <> '' then do
   -- find the machine and do the build
   do machine over buildarr
      if specific_machine = machine~osname() then do
         machine~do_copy(cmdargs)
         leave
         end
      end
   else say specific_machine 'Error: not found.'
   end
else say 'Error: no machine specified.'
return


/*----------------------------------------------------------------------------*/
/* Class: buildmachine                                                        */
/*----------------------------------------------------------------------------*/

::class buildmachine

::attribute osname
::attribute addr
::attribute userid
::attribute cmd
::attribute virt_flag

::method init
expose osname addr userid cmd virt_flag
use strict arg osname, addr, userid, cmd, virt_flag = .true
return

::method do_build
expose osname addr userid cmd virt_flag
use strict arg cmdargs
if virt_flag = .true then do
   -- start the domain
   domain = .kvmdomain~new(osname)
   retc = domain~startup()
   if retc <> 0 then return
   call SysSleep 90  -- allow some time for the domain to fully start
   end
if cmdargs = '' then cmdline = cmd
else cmdline = cmd cmdargs
'ssh' userid'@'addr '"'cmdline'"'
if virt_flag = .true then do
   retc = domain~shutdown()
   call SysSleep 90  -- allow some time for the domain to fully stop
   end
return

::method do_copy
expose osname addr userid virt_flag
use strict arg file
if virt_flag = .true then do
   -- start the domain
   domain = .kvmdomain~new(osname)
   retc = domain~startup()
   if retc <> 0 then return
   call SysSleep 90  -- allow some time for the domain to fully start
   end
'scp' file userid'@'addr':/home/'userid
if virt_flag = .true then do
   retc = domain~shutdown()
   call SysSleep 90  -- allow some time for the domain to fully stop
   end
return


/*----------------------------------------------------------------------------*/
/* Routine: file_arrayin                                                      */
/*----------------------------------------------------------------------------*/

::routine file_arrayin
use strict arg file
strm = .stream~new(file)
strm~open('read')
arr = strm~arrayin()
strm~close()
return arr

