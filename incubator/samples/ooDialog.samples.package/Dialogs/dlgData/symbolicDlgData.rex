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
 * This example program is an almost exact copy of the dlgData.rex program.  The
 * only difference is that sybmolic IDs are used for the dialog data stem
 * indexes instead of numeric IDs.  The two programs work exactly the same.
 *
 * There is one caveat however when using symbolic IDs.  Each symbolic ID must
 * have an unique numeric ID.  If different symbolic IDs have the same numeric
 * value, when the user closes the dialog with Ok, the ooDialog framework will
 * not be able tell which symbolic ID is assigned to duplicate numeric ID.  This
 * will make the outcome non-deterministic.
 *
 * It takes very little effort to ensure that all resource IDs are unique.  If
 * the programmer is going to combine using a dialog data stem and symbolic IDs
 * then she should ensure each resource ID has an unique numeric value.
 */

  dlgData.IDC_RB_MARRIED = 1
  dlgData.IDC_RB_RETIRED = 1
  dlgData.IDC_RB_MALE = 1

  dlg = .CensusDlg~new("dlgData.rc", IDD_CENSUS, dlgData., "dlgData.h" )

  if dlg~execute("SHOWTOP", IDI_DLG_OOREXX) == 1 then do
    l1 = 'Name:' dlgData.IDC_EDIT_LNAME',' dlgData.IDC_EDIT_FNAME':' || .endOfLine~copies(2)

    l2 = '09'x || 'Marital Status:' || '09'x
    select
      when dlgData.IDC_RB_MARRIED  == 1 then l2 ||= 'married'  || .endOfLine
      when dlgData.IDC_RB_SINGLE   == 1 then l2 ||= 'single'   || .endOfLine
      when dlgData.IDC_RB_WIDOWED  == 1 then l2 ||= 'widowed'  || .endOfLine
      when dlgData.IDC_RB_DIVORCED == 1 then l2 ||= 'divorced' || .endOfLine
    end
    -- End select

    l3 = '09'x || 'Work Force Status:' || '09'x
    select
      when dlgData.IDC_RB_RETIRED    == 1 then l3 ||= 'employed'   || .endOfLine
      when dlgData.IDC_RB_RETIRED    == 1 then l3 ||= 'retired'    || .endOfLine
      when dlgData.IDC_RB_STUDENT    == 1 then l3 ||= 'student'    || .endOfLine
      when dlgData.IDC_RB_UNEMPLOYED == 1 then l3 ||= 'unemployed' || .endOfLine
    end
    -- End select

    l4 = '09'x || 'Gender:' || '09'x~copies(2)
    if dlgData.IDC_RB_MALE == 1 then l4 ||= 'male' || .endOfLine~copies(2)
    else l4 ||= 'female' || .endOfLine~copies(2)

    l5 = ""
    if dlgData.IDC_CK_US_CITIZEN == 1 then l5 ||= 'Citizen, '
    else l5 ||= 'Non-citizen, '

    if dlgData.IDC_CK_OVER65 == 1 then l5 ||= 'elderly, '
    else l5 ||= 'youthful, '

    if dlgData.IDC_CK_FELON == 1 then l5 ||= 'felon, '
    else l5 ||= 'law-abiding, '

    if dlgData.IDC_CK_DISABLED == 1 then l5 ||= 'disabled, '
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

  ret = MessageDialog(msg, 0, title, 'OK', icon, 'SETFOREGROUND')

  if icon == 'ERROR' then return 99

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "ooDialog.cls"

::class 'CensusDlg' subclass RcDialog

