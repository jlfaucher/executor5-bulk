/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
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
 *  The augmentedResize.rex example has the same initial dialog layout as the
 *  basicResize.rex example.  However, to give a better appearance to the dialog
 *  as it is enlarged or shrunk, the precise way each dialog control is to be
 *  changed is defined in the program code.
 *
 *  All code that defines the sizing of the resizable dialog has to be defined
 *  in the defineSizing() method.  This method is invoked automatically by the
 *  ooDialog framework.  The default implementation of the method does nothing.
 *  The method is meant to be over-ridden by the programmer to change the
 *  default sizing parameters of the ResizingAdmin class.  The method is invoked
 *  before the underlying dialog is created.  The programmer can not inovke any
 *  method that requires the underlying dialog to exist from the defineSizing()
 *  method.
 */

    --  Use the global .constDir, only, for symbolic IDs, load the symbols from
    --  the basicResize.h file.
    .application~setDefaults('O', 'basicResize.h', .false)

    dlg = .ResizableDlg~new("basicResize.rc", IDD_RESIZABLE)
    dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "ooDialog.cls"

-- Inherit the ResizingAdmin class to make resizing available.
::class 'ResizableDlg' subclass RcDialog inherit ResizingAdmin

/** defineSizing()
 *
 * This method is invoked automatically by the ooDialog framework.  A default
 * implmentation is provided.  That implmentation does not do anything.  To
 * change how the default implmentation works, the program must over-ride the
 * defineSizing() method and invoke ResizingAdmin methods to change the
 * defaults.
 *
 * Each dialog control must be referenced by its resource ID.  The ID can be
 * numeric or symbolic.  This is no different than any other use of ooDialog.
 *
 * The basic premise of the resizing admin is that each side, left, top, etc.,
 * of every control window can be "pinned" to the side of another window.  By
 * default each side is pinned to a side of the dialog window itself.  But, any
 * side of a dialog control window can be pinned to a side of any other dialog
 * control in the dialog.
 *
 * There are several different "types" of pins.  The programmer defines the
 * "sizing" of any dialog control by defining how each "edge" of the control
 * should be sized.  The definition consists of: the id of the control, which
 * side of the control is to be pined, which side of another window the control
 * edge is pinned to, the type of the pin, and the ID of the other window.  For
 * instance:
 *
 * The left side of a button control could be "pinned" to the left side of the
 * dialog window using a 'proportional' pin.
 *
 *   or
 *
 * The right side of a list-view control could be "pinned" the left side of an
 * edit control using a 'fixed' pin.
 *
 * The control windows are identified by the resource ID of the control.  The
 * dialog window is identified by a special constant provided by the
 * ResizingAdmin class, the IDC_DEFAULT_PINTO_WINDOW constant.
 *
 * The edges (sides) of windows are identified by the keywords: LEFT, TOP,
 * RIGHT, and BOTTOM.  In addition there are two other edge keywords: XCENTER
 * and YCENTER.
 *
 * XCENTER specifies that the edge is pinned to the horizontal center of the
 * pinned to edge.  YCENTER specifies that the edge is pinned to the vertical
 * center of the pinned to edge.
 *
 * The types of pins are identified by the kewords: PROPORTIONAL, STATIONARY,
 * MYTOP, and MYLEFT.
 *
 * PROPORTIONAL :  The distance between the edge and the pinned to edge grows,
 *                 (or shrinks,) proportionally to the size of the pinned to
 *                 window.
 *
 * STATIONARY   :  The distance between the edge and the pinned to edge remains
 *                 equal to what it was when the dialog is initially created.
 *
 * MYTOP        :  A special keyword, can only be used for the bottom edge of a control.
 *
 */
::method defineSizing

    ID_DLG = self~IDC_DEFAULT_PINTO_WINDOW

    self~controlSizing(IDC_LV_MAIN,                                          -
                       .array~of('STATIONARY', 'LEFT',   ID_DLG),            -
                       .array~of('STATIONARY', 'TOP',    ID_DLG),            -
                       .array~of('STATIONARY', 'RIGHT',  ID_DLG),            -
                       .array~of('STATIONARY', 'BOTTOM', ID_DLG)             -
                      )

    self~controlSizing(IDC_PB_TEST,                                          -
                       .array~of('STATIONARY', 'RIGHT', IDC_LV_MAIN),        -
                       .array~of('STATIONARY', 'TOP'),                       -
                       .array~of('MYLEFT',     'LEFT'),                      -
                       .array~of('MYTOP',      'TOP')                        -
                      )

    self~controlSizing(IDC_GB_TEST,                                          -
                       .array~of('STATIONARY', 'LEFT'),                      -
                       .array~of('STATIONARY', 'BOTTOM', IDC_LV_MAIN),       -
                       .array~of('STATIONARY', 'RIGHT'),                     -
                       .array~of('MYTOP',      'TOP')                        -
                      )

    self~controlSizing(IDC_EDIT_LABELS,                                      -
                       .array~of('PROPORTIONAL', 'LEFT',  IDC_GB_TEST),      -
                       .array~of('STATIONARY',   'TOP',   IDC_GB_TEST),      -
                       .array~of('STATIONARY',   'RIGHT', IDC_GB_TEST),      -
                       .array~of('MYTOP',        'TOP')                      -
                       )

    self~controlSizing(IDC_ST_LABELS,                                        -
                       .array~of('STATIONARY', 'LEFT', IDC_EDIT_LABELS),     -
                       .array~of('STATIONARY', 'TOP',  IDC_GB_TEST),         -
                       .array~of('MYLEFT',     'LEFT'),                      -
                       .array~of('MYTOP',      'TOP')                        -
                      )

    self~controlSizing(IDC_ST_NAMES,                                         -
                       .array~of('STATIONARY', 'LEFT', IDC_GB_TEST),         -
                       .array~of('STATIONARY', 'TOP',  IDC_GB_TEST),         -
                       .array~of('MYLEFT',     'LEFT'),                      -
                       .array~of('MYTOP',      'TOP')                        -
                      )

    self~controlSizing(IDC_EDIT_NAMES,                                       -
                       .array~of('STATIONARY', 'LEFT', IDC_ST_NAMES),        -
                       .array~of('STATIONARY', 'TOP',  IDC_GB_TEST),         -
                       .array~of('STATIONARY', 'LEFT', IDC_ST_LABELS),       -
                       .array~of('MYTOP',      'TOP')                        -
                       )

    self~controlSizing(IDC_ST_PLACES,                                        -
                       .array~of('STATIONARY', 'LEFT', IDC_GB_TEST),         -
                       .array~of('STATIONARY', 'TOP',  IDC_GB_TEST),         -
                       .array~of('MYLEFT',     'LEFT'),                      -
                       .array~of('MYTOP',      'TOP')                        -
                      )

    self~controlSizing(IDC_EDIT_PLACES,                                      -
                       .array~of('STATIONARY', 'LEFT', IDC_ST_PLACES),       -
                       .array~of('STATIONARY', 'TOP',  IDC_GB_TEST),         -
                       .array~of('STATIONARY', 'LEFT', IDC_EDIT_NAMES),      -
                       .array~of('MYTOP',      'TOP')                        -
                       )

    self~controlSizing(IDOK,                                                 -
                       .array~of('STATIONARY', 'RIGHT'),                     -
                       .array~of('STATIONARY', 'BOTTOM'),                    -
                       .array~of('MYLEFT',     'LEFT'),                      -
                       .array~of('MYTOP',      'TOP')                        -
                      )

    self~controlSizing(IDCANCEL,                                             -
                       .array~of('STATIONARY', 'RIGHT'),                     -
                       .array~of('STATIONARY', 'BOTTOM'),                    -
                       .array~of('MYLEFT',     'LEFT'),                      -
                       .array~of('MYTOP',      'TOP')                        -
                      )


  return 0


::method initDialog

  list = self~newListView(IDC_LV_MAIN)
  list~setView('REPORT')

  columnNames = .array~of("Line", "Number")
  list~addExtendedStyle("FULLROWSELECT GRIDLINES CHECKBOXES HEADERDRAGDROP")

  list~InsertColumn(0, "Row (Column 1)", 95)
  list~InsertColumn(1, "Column 2", 95)
  list~InsertColumn(2, "Column 3", 95)

  do i = 1 to 200
    list~addRow(i, , "Line" i, "Column" 2, "Column" 3)
  end



