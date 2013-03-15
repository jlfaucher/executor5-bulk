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

/**
 *
 *
 *
 *
 *
 *
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

      -- Set up the symbolic IDs and then put up our example dialog.
      .application~setDefaults('O', 'resources\saveFileWithFilter.h', .false)

      dlg = .CommonSaveDialog~new('resources\saveFileWithFilter.rc', IDD_SAVE_FILE)
      dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

      return 0

::requires "ooDialog.cls"

::class 'CommonSaveDialog' subclass RcDialog

::constant GUID   'a25b89e4-db67-4423-ae66-a4836bb76024'


/** initDialog()
 *
 * Simple standard init dialog method.  We also use it to find the installed
 * directory of ooRxx and set up the inheritance of the
 * CommonDialogCustomizations mixin class.
 */
::method initDialog
		expose ckExe ckDll ckCls ckNone edit rexx_home

    ckExe  = self~newCheckBox(IDC_CK_EXE)~~check
    ckDll  = self~newCheckBox(IDC_CK_DLL)
    ckCls  = self~newCheckBox(IDC_CK_CLS)
    ckNone = self~newCheckBox(IDC_CK_NONE)

    edit = self~newEdit(IDC_EDIT)

    self~connectButtonEvent(IDC_CK_NONE, 'CLICKED', onCheckNone)
    self~connectButtonEvent(IDC_PB_SHOW, 'CLICKED', onSave)

    rexx_home = value('REXX_HOME', , 'ENVIRONMENT')
    if rexx_home == '' then rexx_home = 'C:\Program Files'

    .CommonItemDialog~inherit(.CommonDialogCustomizations)



/** onSave()
 *
 * When the user clicks on the 'Browse' push button, we put up the browse for
 * folder dialog and then report the results in the edit control.
 *
 * If the 'Use opitonal arguments' radio button is checked we use all the
 * possible optional arugments.  Otherwise we just put up the browse for folder
 * dialog 'plain.'  I.e., using all defaults.
 */
::method onSave unguarded
    expose edit rexx_home

    sfd = .SaveFileDialog~new
    ret = sfd~setClientGuid(self~GUID)

    filter = self~getCurrentFilter

    eventHandler = .CDevents~new
    eventHandler~filter = filter
    sfd~advise(eventHandler)

    filterObj = .SIFilter~new
    filterObj~filter = filter

    sfd~setFilter(filterObj)
    sfd~setFolder(rexx_home)

    ret = sfd~show(self)

    text = 'The user canceled the save'
    if ret \== sfd~canceled then do
        text = 'Save to file:' sfd~getResult
    end

    sfd~release

    edit~setText(text)


::method onCheckNone unguarded
		expose ckExe ckDll ckCls ckNone

    if ckNone~checked then do
        ckExe~~uncheck~disable
        ckDll~~uncheck~disable
        ckCls~~uncheck~disable
    end
    else do
        ckExe~enable
        ckDll~enable
        ckCls~enable
    end


::method getCurrentFilter unguarded private
		expose ckExe ckDll ckCls ckNone

    if ckNone~checked then do
        f = 'None'
    end
    else do
        f = ''
        if ckExe~checked then f = '.exe'
        if ckDll~checked then f ||= ' .dll'
        if ckCls~checked then f ||= ' .cls'
    end
    if f == '' then f = 'None'

    return f


::class 'Helper' mixinclass object
::method getExtension
    use strict arg file

    pos = file~lastpos('.')
    if pos > 0 then return file~substr(pos)
    else return ''


::class 'SIFilter' subclass ShellItemFilter inherit Helper

::attribute filter unguarded

::method includeItem
    use arg sfd, hwnd, item

    filStr = self~filter
    if filStr == 'None' then return self~S_OK

    ext = self~getExtension(item)
    if filStr~caselessWordPos(ext) <> 0 then return self~S_FALSE

    return self~S_OK


::class 'CDEvents' subclass CommonDialogEvents inherit Helper

::attribute filter unguarded


/** onFileOk()
 *
 *  This is the event handler Ok push button, which is labeled Open, or Save
 *  depending on the type of file dialog.  Or it could be a custom label.
 *
 *  It is invoked when the user has pushed Save (for this dialog) but before
 *  the dialog is closed.  It gives us a chance to veto the close.  If S_OK is
 *  returned the dialog close.  If S_FALSE is returned the dialog does not
 *  close.
 *
 *  Obviously, if the dialog does not close when the user presses the button, it
 *  can be disconcerting.  So, you should always put up some explanation if why
 *  the dialog does not close.
 *
 *  Here we examine the choosen file and see if it has one of the extensions not
 *  allowed.  If so we refuse the file.
 *
 *  cfd -> .CommonFileDialog  This is the Rexx Common File Dialog
 */
::method onFileOk unguarded
  use arg cfd, hwnd

  if self~filter == 'None' then return sefl~S_OK

  file = cfd~getResult
  if file \== .nil then do
      ext = self~getExtension(file)

      if self~filter~caselessWordPos(ext) <> 0 then do
          extList = self~nameExtensions(self~filter)
          title   = 'ooRexx Save File Selection Error'
          msg     = 'File:' file 'is not acceptable.' || .endOfLine~copies(2) || -
                    'The file to save to must not have' extList
          j = MessageDialog(msg, hwnd, title, 'OK', 'ERROR')
          return self~S_FALSE
      end
  end

  return self~S_OK


/** nameExtensions()
 *
 * Just a simple helper method to take the filter string: '.exe .dll' and turn
 * it into a phrase for the message box.  I.e.:
 *
 * '.cls'  -> an extension of .cls
 *
 * '.exe .cls' -> an extension of .exe, or .cls
 *
 * etc., etc..
 */
::method nameExtensions unguarded private
    use strict arg exts

    count = exts~words
    str = 'an extension of '

    if count == 1 then do
      str ||= exts
    end
    else do i = 1 to count
        if i == count then str ||= 'or' exts~word(i)
        else str ||= exts~word(i) || ', '
    end
    return str || '.'
