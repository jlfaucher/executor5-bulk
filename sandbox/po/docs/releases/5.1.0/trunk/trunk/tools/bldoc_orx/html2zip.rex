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
/* Name: HTML2ZIP.REX                                                         */
/* Type: Object REXX Script                                                   */
/*                                                                            */
-- Zip all the HTML, CSS and graphics files for the document
    -- get the properties collection
    props = .doc.props
    _ = props~getProperty('dir_sep')    -- either \ or /
    if arg() = 2 then   -- called from DOC2HTML; base_dir has trailing (\ or /)
        parse arg base_dir, whichdoc
    else do
        -- verify whichdoc is set
        whichdoc = props~getProperty('whichdoc')
        if whichdoc = '' then do
            say 'Unable to determine the document to zip.'
            say 'Be sure DOCPREP and/or DOC2HTML has been run.'
            exit
        end
        --  need the base dir which depends on HTML_folders
        dir_name = props~getProperty('HTML_folders')
        base_dir = dir_name||_||whichdoc||_
    end
    -- get the destination folder name from the HTML_zipfiles property
    zip_dir = props~getProperty('HTML_zipfiles')
    zip_name = whichdoc'.zip'
    say 'Creating' zip_name 'in' zip_dir
    zip_fullName = zip_dir||_||zip_name
    .file~new(zip_fullName)~delete  -- delete the old copy if it exists
    -- To use a different zip program, modify the following command
    if _ = '\' then       -- Windows
        address path 'powershell Compress-Archive' base_dir'*' zip_fullName
    else
    --  address path 'zip -rq' zip_fullName base_dir'*'
        address path 'zip -rq' zip_fullName ". -i" base_dir'*'

::requires doc_props.rex
