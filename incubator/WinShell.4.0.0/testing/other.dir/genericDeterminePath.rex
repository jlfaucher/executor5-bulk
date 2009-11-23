/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

/** There are a number of System folders that are always available, but where
 *  the user may have changed the actual location.  For instance, from a post to
 *  RexxLA:
 *
 *    My users tend to have their “My Documents ” folder in another location
 *    as the default one.
 *
 *    Because I need to know where that is I wrote the following script ...
 *
 *  This program shows how to use the WinShell pathFromCSIDL method to find the
 *  correct path on any machine.
 */

  shell = .WinShell~new

  desktopPath = shell~pathFromCSIDL("CSIDL_COMMON_DESKTOPDIRECTORY")
  say 'Path to Common Desktop:' desktopPath
  say
  startUpPath = shell~pathFromCSIDL("CSIDL_COMMON_STARTUP")
  say 'Path to Common Startup:' startUpPath
  say
  startUpPath = shell~pathFromCSIDL("CSIDL_STARTUP")
  say 'Path to User Startup:  ' startUpPath
  say

  -- Some of the CSIDL_XXX constants refer to virtual folders that have no path
  -- on the file system.  If you query for one of those you will get back .nil.
  recycleBinPath = shell~pathFromCSIDL("CSIDL_BITBUCKET")
  say 'Path to Recycle Bin:' recycleBinPath
  say

  -- Now for the topic of the My Documents folder.  You would think you would
  -- use: CSIDL_MYDOCUMENTS, but, that is a virtual folder.  So you need to use
  -- CSIDL_PERSONAL

  path = shell~pathFromCSIDL("CSIDL_PERSONAL")
  say 'Path to User My Documents:' path
  say

  -- Example of using CSIDL_MYDOCUMENTS
  say 'The following is an example of using CSIDL_MYDOCUMENTS'
  say
  path = shell~pathFromCSIDL("CSIDL_MYDOCUMENTS")
  say 'Path to User My Documents:' path
  say

  -- Not all systems have all the special folders created.  You can use the
  -- optional second arg to tell the shell to try and create the folder if it
  -- does not exist.
  path = shell~pathFromCSIDL("CSIDL_COMMON_FAVORITES", .true)
  say 'Path to Common Favorites: ' path
  say

::requires 'WinShell.cls'
