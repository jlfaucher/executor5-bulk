#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2020 Rexx Language Association. All rights reserved.         */
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
/* Name: DOC2HTML.REX                                                         */
/* Type: Object REXX Script                                                   */
/*                                                                            */
-- Transform an ooRexx Docbook XML file to chunked HTML files using a modified
--  Docbook stylesheet
    if arg(1) \= '' then    -- need to run docprep first
        call docprep arg(1)
    -- get the properties file and verify docpath is set
    props = .doc.props
    docpath = props~getProperty('docpath', '')
    if docpath = '' then do
        say 'You must specify the path to the ooRexx docs.'
        say 'Please run the DOCPATH command first.'
        exit
    end
    -- verify whichdoc is set
    whichdoc = props~getProperty('whichdoc', '')
    if whichdoc = '' then do
        say 'You must specify the name of the ooRexx document to be built.'
        say 'Please run the DOCPREP command first.'
        exit
    end
    -- the *.tmp files are now created only if needed in DOCPREP
    -- get current date and time (needed later for HTML files check)
    start_time = .dateTime~new
    whichOS = props~getProperty('OS_type')~left(1)~upper
    env = 'ENVIRONMENT'
    select
        when whichOS = 'W' then do  -- Windows needs this, has no default value
            dtd_cat = '.\DocBook_Files\catalog.xml'
            xsl_cat = '.\DocBook_Files\docbook-xsl-nons-1.79.2\catalog.xml'
            call value 'xml_catalog_files', dtd_cat xsl_cat, env
        end
        when whichOS = 'D' then do  -- Mac (Darwin)
            xml_cat = props~getProperty('XMLcatalog')
            if xml_cat <> '' then
                call value 'XML_CATALOG_FILES', xml_cat, env
        end
        otherwise                   -- Linux has a default value
            nop
    end
    _ = props~getProperty('dir_sep')    -- either \ or /
    work_folder = props~getProperty('work_folder')
    indoc = work_folder||_||whichdoc'.xml'
    -- get the property named 'HTML_folders' and verify it exists
    HTML_folders = props~getProperty('HTML_folders')
    if \.file~new(HTML_folders)~exists then do
        say 'Unable to find' HTML_folders'.'
        exit
    end
    base_dir = HTML_folders||_||whichdoc||_ -- the folder for this document
    theDir = .file~new(base_dir)
    if \theDir~exists then
        theDir~makeDir
    x_opt1 = '--stringparam base.dir' base_dir
    x_opt2 = '--stringparam package ooRexx_Documentation-Open_Object_Rexx-5.0-en-US-0-0'
    x_opt3 = '--stringparam prod.url https://www.oorexx.org/'
    x_opt4 = '--stringparam xsltproc.version 1.1.26'
    x_opts = '--xinclude' x_opt1 x_opt2 x_opt3 x_opt4 props~getProperty('xslt_opts')
    say time() 'Transforming' indoc 'into' base_dir
    say 'This may take up to several minutes for the largest documents!'
    'xsltproc' x_opts 'html.xsl' indoc
    xcpyf = docpath||_'ooRexx'_'en-US'_     -- common source folder
    xcpyt = base_dir'Common_Content'_
    say 'Updating the files from the CSS folder if required.'
    call do_rxcopy xcpyf'css', xcpyt'css'
    say 'Updating the files from the common images folder if required.'
    call do_rxcopy xcpyf'images', xcpyt'images'
    inpath = docpath||_||whichdoc||_'en-US'_    -- actual source folder
    if .file~new(inpath'images')~exists then do
        say 'Updating the files from the' whichdoc 'images folder if required.'
        call do_rxcopy inpath'images', base_dir'images'
    end
    -- check that the (new) HTML files have been created
    theFiles = .file~new(base_dir)~listFiles
    fileCount = 0
    do aFile over theFiles
        if aFile~isDirectory then
            iterate
        created_time = aFile~lastmodified   -- a DateTime object
        if start_time~compareTo(created_time) = -1 then -- aFile is new
            fileCount += 1
        else                                -- must be a left over file
            aFile~delete
    end
    if fileCount = 0 then
        fileCount = 'No'
    else do
        if props~getLogical('zip_HTML') then
            call HTML2zip base_dir, whichdoc
    end
    say time() fileCount 'HTML files were created'

::requires doc_props.rex

::routine do_rxcopy
    parse arg from_dir, to_dir
    lst = .array~new
    call rxcopy .file~new(from_dir), .file~new(to_dir), lst
    ctr = lst~items
    ? = (ctr = 1)~?('', 's')
    say ctr 'file'? 'copied.'

::routine rxcopy
    use arg srceObj, destObj, fn_list=(.array~new)
    -- expects srce and dest to be file objects AND dest to be a directory
    --  if srce is a single file, dest must already exist
    --  fn_list is an optional array (or other ordered collection) that will
    --  receive the names of the files that were copied
    dir_sep = .RexxInfo~directorySeparator
    select
        when srceObj~isFile then do -- copying a single file
            -- do we need to copy it?
            srceDate = srceObj~lastModified
            destFile = destObj~absolutePath || dir_sep || srceObj~name
            destDate = .file~new(destFile)~lastModified
            -- above may be .nil if destFile doesn't exist
            select
                when destDate == .nil then do
                --  say 'No destination file'
                    cpy? = .true
                end
                when srceDate > destDate then do
                --  say 'Newer source file'
                    cpy? = .true
                end
                otherwise
                --  say 'Destination file is current'
                    cpy? = .false
            end
            if cpy? then do
            --  say 'Need to copy the file'
                inFile = .stream~new(srceObj)
                inData = inFile~charIn(, inFile~chars)
                inFile~close
            --  say 'Srce data read'
                outFile = .stream~new(destFile)~~open(write replace)
                outFile~~charOut(inData)~close
            --  say 'Dest data written'
                fn_list~append(infile~qualify)
            end
        end
        when srceObj~isDirectory then do    -- copying a directory
            if \destObj~exists then
                destObj~makeDirs
            srceList = srceObj~listFiles    -- array of file objects
            do anObj over srceList
                if anObj~isDirectory then do
                    destName = destObj~absolutePath
                    theDest = .file~new(destName || dir_sep || anObj~name)
                end
                else
                    theDest = destObj
                call rxcopy anObj, theDest, fn_list
            end
        end
        otherwise   -- neither file nor directory; probably typo
            say 'Unknown source object:' srceObj~absolutePath'; check spelling'
    end
    return
