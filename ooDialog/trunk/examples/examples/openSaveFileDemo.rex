/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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

/** openSaveFileDemo.rex
 *
 *  This application shows several examples of using the Common Item Dialog.
 *
 * The Common Item Dialog (formerly known as the Common File Dialog) is used in
 * two variations: the Open dialog and the Save dialog. These two dialogs share
 * most of their functionality, but each have some unique methods.
 *
 * The common item dialog implementation found in Windows Vista and later
 * provides several advantages over the implementation provided in earlier
 * versions of Windows.
 *
 *  ooDialog provides complete support for all features of the Common Item
 *  Dialog.
 *
 *  This example will only run on Vista or later.
 *
 *  Both the .OpenFileDialog and the .SaveFileDialog are subclasses of the
 *  .CommonItemDialog class.  The Rexx programmer can not instantiate a
 *  .CommonItemDialog object.  Instead the programmer picks the type of file
 *  dialog he wants, a save file or an open file dialog and instantitates that
 *  class.  Almost all of the methods for these classes come from the
 *  CommonItemDialog class and are documented in the reference manual under the
 *  CommonItemDialog section.
 *
 *  Some of the features of the Common Item Dialog are a little advanced.  How-
 *  ever, the basic every day usage is simple.  This example puts up an opening
 *  dialog that allows the user to choose from a number of different usage
 *  patterns and then shows the Common Item Dialog picked.
 *
 *  One of the features of the Common Item Dialog is that it allows its state to
 *  be saved on a per instance basis  in addition to the per process basis.
 *  This is done by generating a GUID and assigning it to the dialog before it
 *  is configured.  Then, for each dialog with the same GUID, the operating
 *  system saves its state separately.
 *
 *  To make use of this feature, the programmer would generate a single GUID and
 *  then assign the same GUID each time the file dialog was shown in the
 *  application.
 *
 *  This example makes use of that feature by assigning a different GUID for
 *  each of the XX dialogs that can be displayed from the opening dialog.
 *
 *  This simple program can be used to generate a GUID.  Run the program and
 *  then copy and paste the output on the command line in to your program.
 *
 * - - - - - - - - - Cut begin - - - - - - - - - - - - - - - - - - - - - - - -
 *  /* genGUID.rex */
 *
 *  guid = .DlgUtil~getGUID
 *  say guid
 *  return 0
 *
 *  ::requires 'ooDialog.cls'
 * - - - - - - - - - Cut end - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 */
    if \ .application~requiredOS('Vista', 'openSaveFileDemo.rex') then return 99

    -- Set up the symbolic IDs and then put up our example dialog.
    .application~setDefaults('O', 'resources\osfDialogs.h', .false)

    dlg = .CommonSaveDialog~new('resources\osfDialogs.rc', IDD_SIMPLE_OSF_DIALOGS)
    dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

    return 0

::requires "ooDialog.cls"

::class 'CommonSaveDialog' subclass RcDialog

-- Do not copy these GUIDs into your own code.  Always generate your own GUID.
::constant GUID_OPEN          '021b0a3d-cb16-4def-ab42-7fc259fd84d9'
::constant GUID_SAVE          'beff4495-fa60-468b-90c3-1dcb6fde0e78'
::constant GUID_OPEN_CUSTOM   '745c45cb-3464-46fc-96cf-8628ad4bf63b'
::constant GUID_SAVE_DEFAULT  'fe2c5305-d7f9-43cd-9d7b-9c3d4d1b79be'
::constant GUID_OPEN_FOLDER   'ce5ad183-b42f-4f37-a5d6-78d1cffea2e6'


/** initDialog()
 *
 * Simple standard init dialog method.  We save references to our commonly used
 * controls, and connect the button click event for the push button.
 *
 * We also use this method to find the installed directory of ooRexx.
 */
::method initDialog
		expose rbOpen rbSave rbOpenFolder rbSaveDefault rbOpenCustom edit rexx_home

    rbOpen        = self~newRadioButton(IDC_RB_OPEN)~~check
    rbSave        = self~newRadioButton(IDC_RB_SAVE)
    rbOpenFolder  = self~newRadioButton(IDC_RB_OPEN_FOLDER)
    rbSaveDefault = self~newRadioButton(IDC_RB_SAVE_DEFAULT)
    rbOpenCusom   = self~newRadioButton(IDC_RB_OPEN_CUSTOM)

    edit = self~newEdit(IDC_EDIT_OSF)

    self~connectButtonEvent(IDC_PB_SHOW_OSF, 'CLICKED', onShowOSFDialog)

    rexx_home = value('REXX_HOME', , 'ENVIRONMENT')
    if rexx_home == '' then rexx_home = 'C:\Program Files'


/** onShowOSFDialog()
 *
 * When the user clicks on the 'Show' push button, we put up the example Common
 * Item Dialog specified by the user through the radio buttons.
 *
 * Each Common Item Dialog is displayed using a separate, private, method.  To
 * look at the example code for some type of dialog, for instance the simple
 * open file dialog, examine the associated method.
 */
::method onShowOSFDialog unguarded
    expose rbOpen rbSave rbOpenFolder rbSaveDefault rbOpenCustom

    select
      when rbOpen~checked then return self~openExample
      when rbSave~checked then return self~saveExample
      when rbOpenFolder~checked then return self~openFolderExample
      when rbSaveDefault~checked then return self~saveDefaultExample
      when rbOpendCustom~checked then return self~openCustomExample
      otherwise return 0  -- Can not really happen
    end
    -- End select


::method openExample unguarded private
    expose edit

    -- Setting the client GUID has the operating system preserve the state for
    -- this specific open file dialog.
    ofd = .OpenFileDialog~new
    ret = ofd~setClientGuid(self~GUID_OPEN)

    -- The default flags for the Open File Dialog contain the file must exsit
    -- flag.  This prevents the user from typing in their own file name; the
    -- user can only save to an exsiting file name.
    --
    -- For certain applications, this behaviour may be desired.  But for this
    -- example, we want the user to be able to type in any file name, so we
    -- remove that flag:
    ofd~options = ofd~options~delWord(ofd~options~wordPos('FILEMUSTEXIST'), 1)

    -- Very simple, we are all set, show the dialog and get the user's response:
    ret = ofd~show(self)

    if ret == ofd~canceled then text = 'The user canceled the opne'
    else text = 'Open file:' ofd~getResult

    -- The proper use of both the .SaveFileDialog and the .OpenFileDialog is to
    -- instantiate the object, configure it, show it, and then release it.  The
    -- release() method is essential to ensuring the COM resources are properly
    -- cleaned up.
    --
    -- If release() is not called, the ooDialog framework will *attempt* to do
    -- the clean up in an uninit() method.  However, 1.) there is *no* guarentee
    -- that the interpreter will invoke the uninit() method.  2.) There is *no*
    -- guarentee the uninit() will be run on this thread.  The COM resources can
    -- *not* be cleaned up if uninit() is run on another thread than this one.
    --
    -- The only way to guarentee that the COM resources are cleaned up properly
    -- is for the programmer to invoke the release() method.
    ofd~release

    -- Have the edit box display the result.
    self~showResult(text)
    edit~setText(text)

    return 0


::method saveExample unguarded private
    expose edit

    -- Setting the client GUID has the operating system preserve the state for
    -- this specific save file dialog.
    sfd = .SaveFileDialog~new
    ret = sfd~setClientGuid(self~GUID_SAVE)

    -- We are all set, show the dialog and get the user's response:
    ret = sfd~show(self)

    if ret == sfd~canceled then text = 'The user canceled the save'
    else text = 'Save to file:' sfd~getResult

    -- The proper use of both the .SaveFileDialog and the .OpenFileDialog is to
    -- instantiate the object, configure it, show it, and then release it.  The
    -- release() method is essential to ensuring the COM resources are properly
    -- cleaned up.
    --
    -- If release() is not called, the ooDialog framework will *attempt* to do
    -- the clean up in an uninit() method.  However, 1.) there is *no* guarentee
    -- that the interpreter will invoke the uninit() method.  2.) There is *no*
    -- guarentee the uninit() will be run on this thread.  The COM resources can
    -- *not* be cleaned up if uninit() is run on another thread than this one.
    --
    -- The only way to guarentee that the COM resources are cleaned up properly
    -- is for the programmer to invoke the release() method.
    sfd~release

    -- Have the edit box display the result.
    edit~setText(text)

    return 0


::method openFolderExample unguarded private


    -- Have the edit box display the result.
    return self~showResult(text)


::method saveDefaultExample unguarded private


    -- Have the edit box display the result.
    return self~showResult(text)


::method openCustonExample unguarded private




::method showResult unguarded private
    expose edit
    use strict arg text

    edit~setText(text)
    return 0
