/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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
/* Authors;                                                                   */
/*       W. David Ashley <dashley@us.ibm.com>                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the linux headers and our local stuff */
#include "rexxgtk.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a table.
 *
 * @param rows    Table rows
 *
 * @param cols    Table columns
 *
 * @param homog   Table homegeneous boolean
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxTableNew,               // Object_method name
            int, rows,                 // Table rows
            int, cols,                 // Table columns
            logical_t, homogeneous,    // Homogeneous boolean
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_table_new(rows, cols, homogeneous);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  attach
 *
 * Attach a widget to a table.
 *
 * @param rxwidget Widget to be added
 *
 * @param left    Left position
 *
 * @param right   Right position
 *
 * @param top     Top position
 *
 * @param bottom  Bottom position
 *
 * @param xopt    X options
 *
 * @param yopt    Y optiions
 *
 * @param xpad    X padding
 *
 * @param ypad    Y padding
 *
 * @return        Zero.
 **/
RexxMethod10(int,                       // Return type
            GrxTableAttach,            // Object_method name
            RexxObjectPtr, rxWidget,   // Widget to be added
            int, left,                 // Left position
            int, right,                // Right position
            int, top,                  // Top position
            int, bottom,               // Bottom position
            int, xoptions,             // X options        
            int, yoptions,             // Y options
            int, xpad,                 // X padding
            int, ypad,                 // Y padding
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxWidget, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *myWidget = (GtkWidget *)context->ObjectToCSelf(rxWidget);

    gtk_table_attach(GTK_TABLE(self), myWidget, left, right, top, bottom,
                     (GtkAttachOptions)xoptions, (GtkAttachOptions)yoptions,
                     xpad, ypad);         

    return 0;
}

/**
 * Method:  set_row_spacings
 *
 * Set the table row spacing.
 *
 * @param spacing Row spacing
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTableSetRowSpacings,    // Object_method name
            int, spacing,              // Row spacing
            CSELF, self)               // GTK self
{
    gtk_table_set_row_spacings(GTK_TABLE(self), spacing);

    return 0;
}

/**
 * Method:  set_col_spacings
 *
 * Set the table column spacing.
 *
 * @param spacing Column spacing
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTableSetColSpacings,    // Object_method name
            int, spacing,              // Column spacing
            CSELF, self)               // GTK self
{
    gtk_table_set_col_spacings(GTK_TABLE(self), spacing);

    return 0;
}

