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
 * This example program focuses on the use of the dialog data stem argument to
 * the new() method of the various dialog classes.
 *
 * The dialog data stem use is interwoven with the data autodetection and data
 * attributes mechanism in ooDialog.  Please read the "Data Attribute Methods"
 * section in the "Dialog Object" chapter from the ooDialog reference if you are
 * unfamiliar with the concepts of data autodetection and data attributes.
 *
 * The basic idea is to set the 'data' of the dialog controls using the values
 * from the dialog data stem passed as the third argument to the new() method.
 * For each dialog control, set an index in the stem to the resource ID of the
 * control.  The 'data' of the control will be set to the value at that index.
 * If there is no index in the stem for any control, the 'data' of that control
 * will be set to the empty string, or 0, depending on which type of control it
 * is.
 *
 * On return, if the user closed the dialog with 'Ok', the stem will have an
 * index for every control in the dialog with a positive resource ID.  The value
 * for those indexes with be the 'data' of the control at the time it was
 * closed.  If the user cancels, the dialog data stem has no meaning.
 */

  dlgData.1005 = 1
  dlgData.1016 = 1
  dlgData.1022 = 1

  dlg = .CensusDlg~new("dlgData.rc", IDD_CENSUS, dlgData., "dlgData.h" )

  if dlg~execute("SHOWTOP", IDI_DLG_OOREXX) == 1 then do
    l1 = 'Name:' dlgData.1003',' dlgData.1002':' || .endOfLine~copies(2)

    l2 = '09'x || 'Marital Status:' || '09'x
    select
      when dlgData.1005 == 1 then l2 ||= 'married'  || .endOfLine
      when dlgData.1009 == 1 then l2 ||= 'single'   || .endOfLine
      when dlgData.1010 == 1 then l2 ||= 'widowed'  || .endOfLine
      when dlgData.1011 == 1 then l2 ||= 'divorced' || .endOfLine
    end
    -- End select

    l3 = '09'x || 'Work Force Status:' || '09'x
    select
      when dlgData.1016 == 1 then l3 ||= 'employed'   || .endOfLine
      when dlgData.1017 == 1 then l3 ||= 'retired'    || .endOfLine
      when dlgData.1018 == 1 then l3 ||= 'student'    || .endOfLine
      when dlgData.1019 == 1 then l3 ||= 'unemployed' || .endOfLine
    end
    -- End select

    l4 = '09'x || 'Gender:' || '09'x~copies(2)
    if dlgData.1022 == 1 then l4 ||= 'male' || .endOfLine~copies(2)
    else l4 ||= 'female' || .endOfLine~copies(2)

    l5 = ""
    if dlgData.1026 == 1 then l5 ||= 'Citizen, '
    else l5 ||= 'Non-citizen, '

    if dlgData.1027 == 1 then l5 ||= 'elderly, '
    else l5 ||= 'youthful, '

    if dlgData.1030 == 1 then l5 ||= 'felon, '
    else l5 ||= 'law-abiding, '

    if dlgData.1031 == 1 then l5 ||= 'disabled, '
    else l5 ||= 'able-bodied, '

    l5 ||= 'compliant.'  -- Filled out form as required.

    msg = l1 || l2 || l3 || l4 || l5
    title = 'Form 59-339A Completed.  Results:'
    icon = 'INFORMATION'
  end
  else do
    msg = 'The user canceled and did not fill out form 59-339A.'
    title = 'Form 59-339A Incomplete'
    icon =  'ERROR'
  end

  ret = MessageDialog(msg, 0, title, 'OK', icon, 'TOPMOST')

  if icon == 'ERROR' then return 99

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "ooDialog.cls"

::class 'CensusDlg' subclass RcDialog

