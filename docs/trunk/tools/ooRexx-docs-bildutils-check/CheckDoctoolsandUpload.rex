#!/usr/bin/env rexx
/*
# -----------------------------------------------------------------------------
#
# Copyright (c) 2023-2024, Rexx Language Association. All rights reserved.
#
# This program and the accompanying materials are made available under
# the terms of the Common Public License v1.0 which accompanies this
# distribution. A copy is also available at the following address:
# http://www.oorexx.org/license.html
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither the name of Rexx Language Association nor the names
# of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ------------------------------------------------------------------------------
#
# Name: CheckDoctoolsandUpload.rex
# Type: ooRexx script to check readme.txt in the docs/trunk/tools folder and upload
# it to sourceforge if/when it is amended
#
# Sourceforge downloadpath sfRoot and uploadpath sftoolsfolder should be 
# provided by the caller, the fallbacks are
# sfRoot = 'svn.code.sf.net/p/oorexx/code-0/docs/trunk/tools/'
# sftoolsfolder = '/home/frs/project/oorexx/oorexx-docs-bildutils'
#
# ------------------------------------------------------------------------------
*/

trace o

/* Here is where we find the docbuild tools on sourceforge		*/
/* Should be given as input to the script, fallback if not		*/
  if arg() < 1 then sfRoot = 'svn.code.sf.net/p/oorexx/code-0/docs/trunk/tools/'
  else sfRoot = arg(1)~strip

/* Here is where we upload the built documentation on sourceforge	*/
/* Should be given as input to the script, fallback if not		*/
  if arg() < 2 then    sftoolsfolder = '/home/frs/project/oorexx/oorexx-docs-bildutils'
  else sftoolsfolder = arg(2)~strip

/* These are the credentials for sourceforge upload, 
   User must have RWED rights on sourceforge and have submitted his
   ssh credentials to sf to make the scp command work without pw	*/
   sfuser      = 'perolovjonsson'
   sfIP        = 'frs.sourceforge.net:'

/* toolspath is the local copy of sfRoot				*/
  res = SysFileTree('*.*', 'file', 'FO')
  toolspath = 'tools'				-- local copy

/* The revision file will be created if not existing			*/
  revisionsFile    = 'ToolsRevisions.txt'
  oldrevisionsFile = 'ToolsRevisions.old'

/* These are the unix command used with full path			*/
  svncmd  = '/usr/local/bin/svn'
  bashcmd = '/bin/bash'
  scpcmd  = '/usr/bin/scp'

-- download all tools
  If SysFileExists(toolspath) then address path svncmd 'update' toolspath
  else address path svncmd 'co svn://' || sfRoot .

  outArray  = .array~new
  copyArray = .array~new

-- Get the SVN revision level of readme.txt  LOCALLY
-- if revision file does not exist -> create a dummy file first time with revision 1 (r1)
-- BEWARE OF CASE on readme.txt
  if SysFileExists(revisionsFile) = .false then
  do
    outarray~append('readme.txt r1')	
    q = .stream~new(revisionsFile)
    res = q~open('write replace')
    res = q~arrayout(outArray)
    q~close
  end
  else
  do
    say 'revision file exists'
    say revisionsFile
  end

-- resuse array
  outArray~empty

  q = .stream~new(revisionsFile)
  q~open('read')
  myArray = q~arrayIn()
  q~close

  say myArray~items 'items in revisionfile'

-- This will find the revision locally
-- The second line of the output starts with the revision number; e.g. r12347
  do items over myArray
    say 'items=' items
    address '' svncmd 'log' toolspath || .file~separator || items~word(1) '-l 1' with output stem log.
    parse var log.2 rev .

    if items~word(2) = rev then outArray~append(items~word(1) rev)
    else
    do
      outArray~append(items~word(1) rev)
      copyArray~append(items~word(1))
    end
  end

  res = sysFileCopy(revisionsFile,oldrevisionsFile)
  q   = .stream~new(revisionsFile)
  res = q~open('write replace')
  res = q~arrayout(outArray)
  q~close

  if copyArray~items > 0 then
  do
    do items over copyArray
      say 'item to copy' items
      address path scpcmd toolspath || .File~separator || items sfuser || '@' || sfIP || sftoolsfolder
    end
  end
  else say 'no change since the last time'

exit
