/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2022 Rexx Language Association. All rights reserved.         */
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
/* Name: GETDOCREV.REX                                                        */
/* Type: Object REXX Script                                                   */
/*                                                                            */
/* Get the SVN revision level and revision date of a document. Expects one
    argument - the name of the document specified as the directory name of the
    source under docs/trunk. E.g. the Rexx Reference book is rexxref.

   Normally run as part of the DOCPREP.REX program when preprocessing the
    entity file for some documents, it can also be run from the command line.
    An optional second parameter can be supplied to specify the path to the
    working copy of the documents (docpath).  This value can also be retrieved
    from an environment variable named DOCPATH.  If this is not specified, the
    SVN revision level of the document in the repository will be determined.

   The return value (or output to the console if being run from the command
    line) is a two word string of the revision level and the revision date.
*/
    parse arg doc .
    parse source . how .
    _ = .file~separator
    svnURL = "https://svn.code.sf.net/p/oorexx/code-0/docs/trunk/"
    if how = "COMMAND" then
        docpath = arg(1)~word(2)
    else
        docpath = arg(2)
    if docpath = "" then
        docpath = value('docpath', , 'ENVIRONMENT') --is a working copy defined?
    -- use working copy if available
    docpath = docpath~strip("T", _)     -- remove trailing "slash" if there
    where = (docpath = "")~?(svnURL, docpath||_)
    svncmd = "svn log" where || doc "-l 1"
    -- use the svn log command to get the most recent change information for
    --  the document
    address path svncmd with output stem log.
    -- the second line of the output starts with the revision number and also
    --  contains the revision date; e.g. r12347
    parse var log.2 rev . . . rdate .
    if how = "COMMAND" then
        say "The" doc "document is at revision" rev rdate"."
    else
        return rev rdate
