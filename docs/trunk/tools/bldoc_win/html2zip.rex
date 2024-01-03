/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2020-2024, Rexx Language Association. All rights reserved.         */
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
-- Zip all the HTML, CSS and graphics files for the book
    if arg() = 2 then   -- called from CHK4HTML; base_dir has trailing \
        parse arg base_dir, whichdoc
    else do
        -- get the value of the env. var. whichdoc
        whichdoc = value('whichdoc', , 'ENVIRONMENT')
        if whichdoc = '' then do
            say 'Unable to determine the document to zip.'
            say 'Be sure DOCPREP and/or DOC2HTML has been run.'
            exit
        end
--  need the base dir which depends on %HTML_folders%; it might not be defined
        dir_name = value('HTML_folders', , 'ENVIRONMENT')
        if dir_name = '' then
            dir_name = 'HTML_folders'   -- use the default
        base_dir = dir_name'\'whichdoc'\'
    end
-- allow the destination folder to be specified via the env. var. HTML_zipfiles
    zip_dir = value('HTML_zipfiles', , 'ENVIRONMENT')
    if zip_dir = '' then
        zip_dir = 'HTML_zipfiles'       -- use the default
    zip_name = whichdoc'.zip'
    say 'Creating' zip_name 'in' zip_dir
    zip_fullName = zip_dir'\'zip_name
    .file~new(zip_fullName)~delete  -- delete the old copy if it exists
    -- You may modify the following command to use a different zip program
    address path 'powershell Compress-Archive' base_dir'*' zip_fullName
