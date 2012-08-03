/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2011 Rexx Language Association. All rights reserved.    */
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

/**
 * This is a simple dialog shows how to use the FileNameDialog() routine to get
 * the user's selection of files to open.
 *
 * The FileNameDialog() routine puts up the standard Windows Open dialog and
 * returns the file or list of files that the user picks.
 *
 * The Open dialog can be configured to allow single selection or multi-
 * selection.  This example configures the Open dialog for multi-selection.
 *
 * If the user cancels the Open dialog, FileNameDialog() returns 0.  When the
 * user selects one file, FileNameDialog() returns the complete path name of the
 * selected file.  When the user selcts more than 1 file, FileNameDialog returns
 * a string containing the directory the selected files are in and the file
 * names of all the selected files.  Each piece of data is separated by a single
 * character.
 *
 * By default the separator character is a space character.  This does not work
 * if the user selected files with spaces in their names, or files that are in
 * directories with spaces in their names.  Because of this, it is always better
 * to specify a separator character.  In this program we specify the '^'
 * character.  We also use the makeArray() method of the String class to put the
 * individual selected files into an array.
 */

    -- Get a direcotry to start in.  The Open dialog will be configured to start
    -- in this directory.
    ooRexxHome = value("REXX_HOME", , 'ENVIRONMENT' )

    if ooRexxHome~length == 0 then do
        path = 'C:\Program Files\ooRexx\'
    end
    else do
        path = ooRexxHome || '\'
    end

    -- Create a list of file types, including the extension for the file type.
    -- The Open dialog will be configured to use this list in the 'Files of
    -- Type' list box.  Each file type consists of a description and an
    -- extension.  The description and extension must be separated with the
    -- hexidecimal number 0.  Note this is not the character '0', it is the
    -- number 0.  Each individual file type must also be separated with 0x, and
    -- the entire list must have 0x added to the end.  To make this easy to do,
    -- we assign '0'x to a variable and then construct the list of file types.
    delim = '0'x
    filemask = 'Class Files (*.cls)'delim || -
               '*.cls'delim               || -
               'All Files (*.*)'delim     || -
               '*.*'delim

    -- This will be a multi-selection Open dialog, so we specify our own
    -- character to separate multiple files if the user selects more than one
    -- file.
    sepChar = '^'

    -- Give the Open dialog a title and then show the Open dialog
    title = 'FileNameDialog Example'
    ret = FileNameDialog(path, , filemask,   -
          'LOAD', title, , 'MULTI', sepChar)

    -- Now, examine the return to see what the user did.  If the user canceled,
    -- the return is 0.
    if ret == 0 then do
        say "The user canceled the dialog."
    end
    else do
        files = ret~makeArray(sepChar)
        if files~items == 1 then do
            say "File selected:" files[1]
        end
        else do
            say 'Files located in directory:' files[1]
            say '  Items:' files~items
            say '  Selected files:'
            do i = 2 to files~items
                say '   ' files[i]
            end
        end
    end

::requires "ooDialog.cls"
