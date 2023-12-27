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
/* Name: FO2PDF.REX                                                           */
/* Type: Object REXX Script                                                   */
/*                                                                            */
-- Create a PDF from an XSL-FO file using Apache FOP
    -- get the properties collection and verify docpath is set
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
    fop_opts = props~getProperty('fop_opts', '')
    if fop_opts = '' then   -- use the JAVA default
        fop_opts = .nil
    call value 'FOP_OPTS', fop_opts, 'ENVIRONMENT'
    -- get the property named 'fo_files' and verify the folder exists
    fo_files = props~getProperty('fo_files')
    if \.file~new(fo_files)~exists then do
        say 'Unable to find' fo_files'.'
        exit
    end
    -- verify the input file exists
    _ = props~getProperty('dir_sep')    -- either \ or /
    in_file = fo_files||_||whichdoc'.fo'
    if \.file~new(in_file)~exists then do
        say 'Unable to find' in_file'.'
        say 'Make sure the DOC2FO command has run successfully.'
        exit
    end
    -- get the property named 'log_files' and verify the folder exists
    log_files = props~getProperty('log_files')
    if \.file~new(log_files)~exists then do
        say 'Unable to find' log_files'.'
        exit
    end
    log_file = log_files||_||whichdoc'.log'
    -- get the property named 'PDF_files' and verify the folder exists
    PDF_files = props~getProperty('PDF_files')
    if \.file~new(PDF_files)~exists then do
        say 'Unable to find' PDF_files'.'
        exit
    end
    out_file = PDF_files||_||whichdoc'.pdf'
    -- check for the fop.cfg file and create it if necessary
    work_folder = props~getProperty('work_folder')
    fopcfg = work_folder||_'Common_Content'_'fop.cfg'
    if \.file~new(fopcfg)~exists then do    -- need to create a new fop.cfg file
        -- read in the fop.cfg template
        fopcfg_in = .stream~new('fop.cfg')
        fopcfg_src = fopcfg_in~arrayin
        fopcfg_in~close
        .stream~new(fopcfg)~~arrayout(fopcfg_src)~close -- and write it out
    end
    fop = 'fop' -- fop is installed on non-Windows systems
    foplvl = ''
    Jenkins_fop = '/usr/local/bin/fop'
    if _ = '\' then do  -- Windows
        -- determine the (most recent) version of FOP available
        address cmd 'dir fop-* /ad /b /o-d' with output stem fopvers.
        if fopvers.0 > 0 then do
            fop = fopvers.1'\fop\fop.bat'   -- first item is newest/only version
            foplvl = fopvers.1~substr(5)    -- numeric part; e.g. 2.8
        end
        else do
            say 'Unable to find Apache FOP.'
            say 'Make sure the SETUP command has run successfully.'
            exit
        end
    end
    else select
        when SysSearchPath('PATH', fop) <> '' then
            nop
        when .file~new(Jenkins_fop)~exists then
            fop = Jenkins_fop   -- fop on Jenkins
        otherwise
            say 'Unable to find Apache FOP.'
            exit
    end
    say time() 'Creating' out_file 'from' in_file 'using Apache FOP' foplvl
    address "" fop '-c' fopcfg in_file out_file '2>'log_file with input normal
    -- check that a (new) PDF has been created
    thePDF = .file~new(out_file)
    msg = 'FOP processing was unsuccessful'
    if thePDF~exists then do
        created_time = thePDF~lastmodified -- a DateTime object
        now = .dateTime~new -- current date and time
        if (now - created_time)~totalSeconds < 2 then
            msg = 'PDF created'
    end
    say time() msg

::requires doc_props.rex
