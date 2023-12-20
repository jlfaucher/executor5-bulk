#!/usr/bin/env rexx
/*
# -----------------------------------------------------------------------------
#
# Copyright (c) 2022-2023 Rexx Language Association. All rights reserved.
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
# Name: BuildandUploadDocs.rex
# Type: ooRexx script
# Script with 4 stages for building documentation for ooRexx
# 1:  If documentation source is existing -> svn update
# 1a: otherwise download souce from sourceforge
# 2: Compare each documents revision with a local property file
#    that is read and updated each time the script runs
# 3: Build only those documents that have changes
#    use existing build script
# 4: Upload the newly built documents to sourceforge
# ------------------------------------------------------------------------------
*/
/* Change 2022-12-17 to prepare for 5.0.0 Beta -> Release candidate*/
/* Change 2022-12-19 rexxgtk excluded from build and upload */
/* Change 2022-12-19 .zip changed to -html.zip -> for Release candidate */
/* Change needed also in doc2html.sh                    */
/* Change 2022-12-25 to upload to 5.1.0beta             */
/* Change 2023-01-02 add 3 zipfiles containing all pdf+html files; svn  */
/* downloadpath and Sourceforge uploadpath MUST be provided from caller */
/* changes to support using the bld_orx tools (ooRexx programs) rather than */
/* the shell scripts to build the documents; runs the shell scripts if all of */
/* ooRexx programs are not present  */

trace o

/* Here is where we find the documentation source on sourceforge    */
/* Should be given as input to the script, fallback if not      */
  if arg() < 1 then sfRoot = 'svn.code.sf.net/p/oorexx/code-0/docs/trunk/'
  else sfRoot = arg(1)~strip

/* Here is where we upload the built documentation on sourceforge   */
/* Should be given as input to the script, fallback if not      */
  if arg() < 2 then    sfdocfolder = '/home/frs/project/oorexx/oorexx-docs/5.1.0beta'
  else sfdocfolder = arg(2)~strip

/* These are the credentials for sourceforge upload,
   User must have RWED rights on sourceforge and have submitted his
   ssh credentials to sf to make the scp command work without pw    */
   sfuser      = 'perolovjonsson'
   sfIP        = 'frs.sourceforge.net:'

/* docpath is the local copy of sfRoot                  */
  res = SysFileTree('*.*', 'file', 'FO')
  cwd = filespec('path',file.1)     -- trailing slash for cwd
  docpath = cwd || 'oorexxdocSVN'   -- local copy

/* this is where the build process puts the newly built documents   */
  pdfpath   = cwd || 'pdf_files'
  htmlpath  = cwd || 'html_folders'
  zippath   = cwd || 'html_zipfiles'

/* The revision file will be created if not existing            */
  revisionsFile = cwd || 'DocRevisions.txt'
  oldrevisionsFile = cwd || 'DocRevisions.old'

/* These are all books that need to be checked              */
/* rexxgtk excluded from upload 2022-12-20              */
/* ooconsole excluded from upload 2023-01-01                */
/* to enable add this to list below and to DocRevisions.txt     */
   outArray = .array~of("oodguide r1", "oodialog r1", "oorexxbuild r1", "oosqlite r1", "ootest r1", "orxncurses r1", "readme r1", "rexxapi r1", "rexxextensions r1", "rexxpg r1", "rexxref r1", "rxftp r1", "rxmath r1", "rxsock r1", "unixextensions r1", "winextensions r1")

/* These are the unix command used with full path           */
  svncmd  = '/usr/local/bin/svn'
  bashcmd = '/bin/bash'
  scpcmd  = '/usr/bin/scp'
  zipcmd  = '/usr/local/bin/7zz a'

-- *1*  documentation download
  If SysFileExists(docpath) then address path svncmd 'update' docpath
  else address path svncmd 'co svn://' || sfRoot docpath

-- *2* Get the SVN revision level of all documents LOCALLY
  if SysFileExists(revisionsFile) = .false then
  do
    q = .stream~new(revisionsFile)
    res = q~open('write replace')
    res = q~arrayout(outArray)
    q~close
  end

  q = .stream~new(revisionsFile)
  q~open('read')
  myArray = q~arrayIn()
  q~close

  outArray~empty
  buildArray = .array~new

-- *2* This will find the revision locally
-- The second line of the output starts with the revision number; e.g. r12347
  do items over myArray
    address '' svncmd 'log' docpath || .file~separator || items~word(1) '-l 1' with output stem log.
    parse var log.2 rev .

    if items~word(2) = rev then outArray~append(items~word(1) rev)
    else
    do
      outArray~append(items~word(1) rev)
      buildArray~append(items~word(1))
    end
  end

  res = sysFileCopy(revisionsFile,oldrevisionsFile)
  q   = .stream~new(revisionsFile)
  res = q~open('write replace')
  res = q~arrayout(outArray)
  q~close

  if buildArray~items > 0 then
  do
-- *3* Build those docs that needs rebuilding:
-- *4* Upload the newly built docs to sourceforge
    if rexxReqs() then  -- Rexx programs are available to build documents
      do a_doc over buildArray
        call doc2pdf a_doc
        address '' scpcmd pdfpath || .file~separator || a_doc || '.pdf' sfuser || '@' || sfIP || sfdocfolder
        call doc2html   -- no argument means the same document
        zip_source = htmlpath || .file~separator || a_doc || .file~separator
        call sysfileDelete zip_source || '.DS_Store'
        address '' zipcmd zippath || .file~separator || a_doc || '-html.zip' zip_source'.'
        address '' scpcmd zippath || .file~separator || a_doc || '-html.zip' sfuser || '@' || sfIP || sfdocfolder
      end
    else                -- use the shell scripts
      do a_doc over buildArray
        address '' bashcmd 'doc2pdf.sh' a_doc
        address '' scpcmd pdfpath || .file~separator || a_doc || '.pdf' sfuser || '@' || sfIP || sfdocfolder
        address '' bashcmd 'doc2html.sh' a_doc
        address '' scpcmd zippath || .file~separator || a_doc || '-html.zip' sfuser || '@' || sfIP || sfdocfolder
      end

-- Zip the whole shebang and upload in three further files. Use syntax from ooRexx4.1.3
    allfiles = 'ooRexx-' || filespec('name',sfdocfolder) || '-all.zip'
    allpdf   = 'ooRexx-' || filespec('name',sfdocfolder) || '-pdf.zip'
    allhtml  = 'ooRexx-' || filespec('name',sfdocfolder) || '-html.zip'
-- delete old zipfiles first
    res = sysfileDelete(allfiles)
    res = sysfileDelete(allhtml)
    res = sysfileDelete(allpdf)
-- delete pestering .DS_Store files before zipping
    res = sysfileDelete(zippath || .file~separator || '.DS_Store')
    res = sysfileDelete(pdfpath || .file~separator || '.DS_Store')
-- zip all html folders
    address '' zipcmd  allhtml zippath || .file~separator || '*.zip'
    address '' zipcmd  allfiles zippath || .file~separator || '*.zip'
-- zip all pdf
    address '' zipcmd allpdf pdfpath || .file~separator || '*.pdf'
    address '' zipcmd allfiles pdfpath || .file~separator || '*.pdf'

-- Upload to SF pdf files newest should go on top
    address '' scpcmd allfiles sfuser || '@' || sfIP || sfdocfolder
    address '' scpcmd allhtml sfuser || '@' || sfIP || sfdocfolder
    address '' scpcmd allpdf sfuser || '@' || sfIP || sfdocfolder
  end

exit

rexxReqs: procedure expose docpath cwd
/* This function verifies that all the files needed by the ooRexx version of the
    documentation build tools are available and returns .true if so.  Otherwise
    it returns .false.  It also initializes the docpath property if docpath is
    not set and creates the fo_files and log_files folders. */
    theReqs = .resources[theReqs]   -- an array of file names
    OK? = .true
    do fn over theReqs while OK?
        OK? &= .file~new(fn)~exists
    end
    if ok? then do
        .context~package~loadPackage('doc_props.rex')
        props = .doc.props
        if props~getProperty('docpath') = '' then do    -- new props file
            props~setProperty('docpath', docpath)       -- set docpath
            props~setLogical('zip_HTML', .false)        -- don't use HTML2zip
            .file~new(cwd || 'fo_files')~makeDir        -- create the folder
            .file~new(cwd || 'log_files')~makeDir       -- create the folder
            call save_props     -- rewrite the properties file
        end
    end
    else
        say 'Unable to locate' -
            fn'; using the shell scrips to build the documents.'
    return OK?

::resource theReqs
delTree.rex
doc2fo.rex
doc2HTML.rex
doc2pdf.rex
docprep.rex
doc_props.rex
fo2pdf.rex
getdocrev.rex
::END
