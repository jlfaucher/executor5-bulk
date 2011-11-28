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

/** createNewStore.rex
 *
 * Creates a new Store in Outlook.  A 'store' is a collection of folders and
 * items in Outlook, such as mail items, contact items, task items, etc..  The
 * entire collection of folders and stores are stored in a physical file on the
 * local file system.
 *
 * The physical file uses the .pst extension.  The file name and full path can
 * be specified on the command line, i.e.:
 *
 *     createNewStore 'C:\outlook3.pst'
 *
 * Or,  for the purposes of this example program, a default file name will be
 * used.  Which is:
 *
 *     'C:\outlook1_demo_mm.pst'
 *
 * The default name can be changed by editing the constant name in 'outlook.frm'
 * Look for the 'CN' class in outlook.frm and changed the value of STORE_PATH.
 *
 * Note, on Vista or later, you would probably need to run this program with
 * elevated privileges to write to C:\.
 */
use arg fileName

  if arg() == 0 then do
    fileName = CN~STORE_PATH
  end

  outLook = createOleObject("Outlook.Application", .true)
  if outLook == .nil then do
    say 'OutLook does not appear to be installed on this computer.'
    say 'The createNewStore.rex example program requires Outlook.'
    return 9
  end

  ret = createNewStore(outLook, fileName)

  if ret == 0 then say 'Created new OutLook store in:' fileName
  else say 'Failed to create new store ('fileName').'

::requires '..\outlook.frm'
