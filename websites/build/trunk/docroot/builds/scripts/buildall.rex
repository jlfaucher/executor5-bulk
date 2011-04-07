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


-- This script should be invoked as follows:
--
-- buildall.rex [location [machine [email]]]
--
-- where
--
-- location - (optional) the build type - trunk or branch
-- machine  - (optional) the target virtual machine
-- email    - (optional) The email address for the build report


-- initialization
cmdline = arg(1)~strip()
parse var cmdline location specific_machine email .
if location = 'branch' then do
   cmdargs = 'http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/branches/4.1.0/trunk/'
   cmdargs = cmdargs '/pub/www/build/docroot/builds/interpreter-main'
   cmdargs = cmdargs email
   end
else do  -- trunk
   cmdargs = 'http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/'
   cmdargs = cmdargs '/pub/www/build/docroot/builds/interpreter-main'
   cmdargs = cmdargs email
   end
cmdargs = cmdargs~strip()
machines = './orxbuildmachines.txt'

-- get the contentst of the input file and create the build array
buildmachines = .buildmachines~new(machines)

-- do the build(s)
if specific_machine <> '' then do
   buildmachines~build_one(specific_machine, cmdargs)
   end
else do  -- do all the machines
   buildmachines~build_all(cmdargs)
   end
return


::requires 'buildmachines.cls'

