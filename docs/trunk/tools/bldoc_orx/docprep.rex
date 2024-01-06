#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2020-2023 Rexx Language Association. All rights reserved.    */
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
/* Name: DOCPREP.REX                                                          */
/* Type: Object REXX Script                                                   */
/*                                                                            */
    -- determine the document to be built and resolve Publican dependencies
    -- get the properties collection and verify docpath is set
    props = .doc.props
    docpath = props~getProperty('docpath', '')
    if docpath = '' then do
        say 'You must specify the path to the ooRexx docs.'
        say 'Please run the DOCPATH command first.'
        exit
    end
    info? = props~getLogical('verbose') -- amount of information displayed
    -- check that the document name has been specified
    if arg(1, O) then do
        say 'You must specify the name of the ooRexx document to be built.'
        exit
    end
    parse arg whichdoc
    -- verify that whichdoc is a valid document name
    _ = props~getProperty('dir_sep')    -- either \ or /
    srcdir = docpath||_||whichdoc||_'en-US' -- directory containing the source
    doc_source = .file~new(srcdir)
    if \doc_source~exists then do
        say 'Unable to find' srcdir'; please check the spelling.'
        exit
    end
    say time() '- Preparing' whichdoc 'source files.'

/* Instead of creating/updating a sub-folder for the Common_Content in the
    working copy of the document - docpath||_||whichdoc, replicate that
    structure in 'work_folder'. Edit each .xml file in the whichdoc directory,
    replacing the path for any *.tmp files with '' and saving the result in
    'work_folder'.  (Xsltproc will then use the files in 'work_folder' rather
    than those in docpath||_||whichdoc.)  Can also update the .ent file at the
    same time.  And any references to *.tmp files will be noted and then the
    files created.  */

    -- if needed, create/update the Common_Content subfolder
    wf_name = props~getProperty('work_folder')  -- path to the work folder
    wfDir = .file~new(wf_name)~~makeDirs        -- file object for work_folder
    cc_srce = docpath||_'oorexx'_'en-US'        -- path to Common_Content source
    cc_dest = wf_name||_'Common_Content'        -- path to Common_Content dest.
    fileList = .array~new                       -- for list of files copied

    destDir = .file~new(cc_dest)~~makeDir
    do aSrce over .file~new(cc_srce)~listFiles
        call rxcopy aSrce, destDir, fileList, info?, .true
    end
    if info? then say fileList~toString

    -- then we need to clean up any existing files in 'work_folder'
    do oldFile over wfDir~listFiles
        if oldFile~isDirectory then do
            if oldFile~compareTo(destDir) <> 0 then do  -- keep Common_Content
                call delTree oldFile, info?             -- empty the folder
                oldFile~delete                          -- delete the folder
            end
        end
        else
            oldFile~delete                              -- delete the file
    end
    p4tmp = '../../../' -- path for *.tmp files
    chgs = 0            -- lines changed in a file
    cfiles = 0          -- files changed
    totcLines = 0       -- total lines changed
    need_tmp? = .false  -- set to true if we find a reference to *.tmp
    fileList~empty      -- clear the list of files copied
    do aFile over doc_source~listFiles
        if aFile~isDirectory then do
        --  destDir = .file~new(wf_name)
            call rxcopy aFile, wfDir, fileList, info?
            if info? then say fileList~toString
        end
        else do
            src = .stream~new(aFile)
            theLines = src~arrayin
            src~close
            ext = aFile~extension~upper
            select
                when ext = 'XML' then do
                    -- process the source file, resolving p4tmp
                    chgs = 0
                    do j = 1 to theLines~items
                        aLine = theLines[j]
                        select
                        /*  when aLine~caselessPos(cc) > 0 then do
                                theLines[j] = aLine~caselessChangeStr(cc, rep)
                                chgs += 1
                            /*  say 'change in' aFile~name 'at line' j
                                say 'new line is: ['theLines[j]']'  */
                            end --*/
                            when aLine~pos(p4tmp) > 0 then do
                                theLines[j] = aLine~changeStr(p4tmp, '')
                                chgs += 1
                                need_tmp? = .true
                            /*  say 'change in' aFile~name 'at line' j
                                say 'new line is: ['theLines[j]']'  */
                            end
                            otherwise   -- no changes needed to line
                        end
                    end
                    if chgs > 0 then do
                        cfiles += 1
                        totcLines += chgs
                    end
                end
                when ext = 'ENT' then do
                    -- process the entities file, updating the VERSION and EDITION
                    -- determine if this document doesn't need these changes
                    if props~getLogical(whichdoc~lower, .true) then do
                        parse value getdocrev(whichdoc, docpath) with rev rdate
                        do j = 1 to theLines~items
                            aLine = theLines[j]
                            parse var aLine w1 w2 w3 .
                            if w2 = 'VERSION', w1 = '<!ENTITY' then do  -- VERSION
                                if w3~countstr('.') > 2 then do -- has revision lvl
                                    -- remove the revision level
                                    parse var w3 v1 '.' v2 '.' v3 '.' .
                                    w3 = v1'.'v2'.'v3'">'
                                    theLines[j] = w1 w2 w3
                                end
                            end
                            if w2 = 'EDITION', w1 = '<!ENTITY' then do  -- EDITION
                                if (w3~left(3) = '"0.') | ,   -- old format
                                   (w3~pos('(revision') > 0) then do
                                    w3 = '"'rdate '(revision' rev~substr(2)')">'
                                    theLines[j] = w1 w2 w3
                                end
                                needle = '(last revised on'
                                if aLine~subword(3)~pos(needle) > 0 then do
                                    -- newer format:
-- <!ENTITY EDITION "2023.01.01 (last revised on 20221024 with r12526)">
                                    words = aLine~makearray(' ')
                                    words[3] = '"'date('I', , , '.')
                                    words[7] = rdate
                                    words[9] = rev')">'
                                    theLines[j] = words~makestring(, ' ')
                                end
                            end
                        end
                    end
                end
                otherwise   -- must be an unknown file type
            end -- select
            newSource = .file~new(wf_name||_||aFile~name)
            .stream~new(newSource)~~arrayOut(theLines)~close
        end
    end
    if info? then do
        say 'The total number of files modified:' cfiles
        say 'The total number of lines modified:' totcLines
    end
    else
        say 'Source files have been modified.'
    if need_tmp? then do    -- create the *.tmp files in 'work_folder'
        svnrev_tmp = wf_name||_'svnrev.tmp'
        address path 'svnversion' srcdir with output stream (svnrev_tmp)
        parse value date() with d . y
        theLine = date(W)', 'date(M) d', 'y
        datepub_tmp = wf_name||_'datepub.tmp'
        .stream~new(datepub_tmp)~~lineout(theLine)~close
        say 'Temporary files have been created.'
    end

    if whichdoc~right(1) = _ then
        whichdoc = whichdoc~strip(T, _) -- delete any trailing dir. separator
    -- set whichdoc in the properties collection
    props~setProperty('whichdoc', whichdoc)
    call save_props                     -- save the updated properties
    say time() whichdoc 'source files are ready.'

::requires doc_props.rex
::requires delTree.rex

::routine rxcopy
-- routine to implement the "deep copy" of the XCOPY command
    use arg srceObj, destObj, fn_list=(.array~new), info?=.false, chkDate?=.false
    -- expects srce and dest to be file objects AND dest to be a directory
    --  fn_list is an optional array (or other ordered collection) that will
    --  receive the names of the files that were copied
    --  info? controls the display of messages and chkDate? if true causes only
    --  newer files to be copied
    dir_sep = .doc.props~getProperty('dir_sep')
    select
        when srceObj~isFile then do -- copying a single file
            -- ensure dest directory exists
            destObj~makeDirs
            -- do we need to copy it?
            destFile = destObj~absolutePath || dir_sep || srceObj~name
            if chkDate? then do
                srceDate = srceObj~lastModified
                destDate = .file~new(destFile)~lastModified
                -- above may be .nil if destFile doesn't exist
                select
                    when destDate == .nil then do
                        if info? then say 'No destination file'
                        cpy? = .true
                    end
                    when srceDate > destDate then do
                        if info? then say 'Newer source file'
                        cpy? = .true
                    end
                    otherwise
                        if info? then say 'Destination file is current'
                        cpy? = .false
                end
            end
            else
                cpy? = .true
            if cpy? then do
                if info? then say 'Need to copy the file:' srceObj~name
                inFile = .stream~new(srceObj)
                inData = inFile~charIn(, inFile~chars)
                inFile~close
                if info? then say 'Srce data read'
                outFile = .stream~new(destFile)~~open(write replace)
                outFile~~charOut(inData)~close
                if info? then say 'Dest data written'
                fn_list~append(infile~qualify)
            end
        end
        when srceObj~isDirectory then do    -- copying a directory
            newDestName = destObj~absolutePath || dir_sep || srceObj~name
            newDestObj = .file~new(newDestName)~~makeDirs
            srceList = srceObj~listFiles    -- array of file objects
            do anObj over srceList
                call rxcopy anObj, newDestObj, fn_list, info?, chkDate?
            end
        end
        otherwise   -- neither file nor directory; probably typo
            say 'Unknown source object:' srceObj~absolutePath'; check spelling'
    end
    return
