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
/* Name: DOC_PROPS.REX                                                        */
/* Type: Object REXX Script                                                   */
/*                                                                            */
-- REQUIRES file for scripts to build ooRexx documents
    prop_fn = 'bldoc.env'   -- name of the properties file

    if .file~new(prop_fn)~exists then   -- load the saved properties
        props = .properties~load(prop_fn)
    else do                             -- need to create a new properties file
        props = .properties~new
        props~setProperty('prop_fn', prop_fn)   -- save the name of the file
        -- set all the default values
        OS_type = .RexxInfo~platform
        props~setProperty('OS_type', OS_type)
        props~setProperty('dir_sep', .file~separator)
        props~setProperty('docpath', '')
        props~setProperty('whichdoc', '')
        props~setProperty('work_folder', 'work_folder')
        props~setProperty('fo_files', 'fo_files')
        props~setProperty('log_files', 'log_files')
        props~setProperty('PDF_files', 'pdf_files')
        props~setProperty('HTML_folders','html_folders')
        props~setProperty('HTML_zipfiles','html_zipfiles')
        props~setProperty('xslt_opts', '')
        props~setProperty('fop_opts', '-Xmx1280M')
        props~setLogical('verbose', .false)
        props~setLogical('zip_HTML', .true)
        if OS_type~left(1) = 'D' then do    -- Darwin (MacOS)
            subpath = '/local/etc/xml/catalog'
            cat_loc1 = '/opt'subpath
            cat_loc2 = '/usr'subpath
            select
                when .file~new(cat_loc1)~exists then
                    XMLcat = cat_loc1
                when .file~new(cat_loc2)~exists then
                    XMLcat = cat_loc2
                otherwise
                    XMLcat = ''
            end
            props~setProperty('XMLcatalog', XMLcat)
        end
        props~save(prop_fn)         -- save it in the properties file
    end
    .local~doc.props = props        -- put the properties collection in .local

::routine save_props public
    props = .doc.props
    prop_fn = props~getProperty('prop_fn')
    props~save(prop_fn)
