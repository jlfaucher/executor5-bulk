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
/* Name: DOCPATH.REX                                                          */
/* Type: Object REXX Script                                                   */
/*                                                                            */
    -- Get the path for the working copy of the ooRexx docs
    parse arg docpath
    if docpath = '' then do
        say 'You must specify the path to the ooRexx docs.'
        exit
    end
    /* Because the program is invoked as a command, there is only a single
        argument, regardless of the number of words.  So, docpath contains the
        entire path, unlike on Windows where double quotes are needed if the
        path contains blanks. However it may be enclosed in quotes so we remove
        them.   */
    docpath = docpath~strip(,"'")~strip(,'"')   -- remove either type of quotes
    -- verify that the path is valid
    if \.file~new(docpath)~exists then do
        say 'Unable to find' docpath'; please check the spelling.'
        exit
    end
    -- get the properties collection
    props = .doc.props
    _ = props~getProperty('dir_sep')
    docpath = docpath~strip(T, _)   -- remove any trailing directory separator
    if docpath~words > 1 then
        docpath = '"'docpath'"'
    -- set docpath in the properties collection
    props~setProperty('docpath', docpath)
    call save_props                 -- save the updated properties
    say 'The docpath is set to' docpath

-- need to delete the Common_Content subfolder if it exists as it probably is
--  for a different docpath
    ccfo = .file~new(props~getProperty('work_folder')||_'Common_Content')
    if ccfo~isDirectory then
        call delTree ccfo, props~getLogical('verbose')

::requires doc_props.rex
::requires delTree.rex
