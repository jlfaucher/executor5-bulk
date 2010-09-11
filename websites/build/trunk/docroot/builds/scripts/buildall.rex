#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Build ooRexx on all build machines.                           */
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
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


-- initialization
machines = 'orxbuildmachines.txt'

-- get the contentst of the input file and create the build array
arr = file_arrayin(machines)
buildarr = .array~new()
do line over arr
   if line~strip() = '' then iterate
   if line~strip()~substr(1, 1) = '#' then iterate
   if line~strip()~substr(1, 2) = '--' then iterate
   parse var line osname addr userid cmd virt_flag .
   buildarr~append(.buildmachine~new(osname, addr, userid, cmd, virt_flag))
   end

-- do all the virtual builds
do machine over buildarr
   machine~do_build()
   end
return


::requires 'kvmdomain.cls'


/*----------------------------------------------------------------------------*/
/* Class: buildmachine                                                        */
/*----------------------------------------------------------------------------*/

::class buildmachine public

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
if virt_flag = .true then do
   -- start the domain
   domain = .kvmdomain~new(osname)
   retc = domain~startup()
   if retc <> 0 then return
   call SysSleep 90  -- allow some time for the domain to fully start
   end
say 'ssh' userid'@'addr '"'cmd'"'
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

