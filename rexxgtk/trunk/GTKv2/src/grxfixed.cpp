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

/*
 * Method:  init
 *
 * Create an fixed.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxFixedNew,               // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_fixed_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  put
 *
 * Attach a widget to the fixed.
 *
 * @param rxwidget The widget to attach
 *
 * @param xpos    The X position
 *
 * @param ypos    The Y position
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxFixedPut,               // Object_method name
            RexxObjectPtr, rxwidget,   // Page number
            int, xpos,                 // X position 
            int, ypos,                 // Y position
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxwidget, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *putWidget = (GtkWidget *)context->ObjectToCSelf(rxwidget);

    gtk_fixed_put(GTK_FIXED(self), putWidget, xpos, ypos);

    return 0;
}

/**
 * Method:  move
 *
 * Move a widget in the fixed.
 *
 * @param rxwidget The widget to move
 *
 * @param xpos    The X position
 *
 * @param ypos    The Y position
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxFixedMove,              // Object_method name
            RexxObjectPtr, rxwidget,   // Page number
            int, xpos,                 // X position 
            int, ypos,                 // Y position
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxwidget, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *moveWidget = (GtkWidget *)context->ObjectToCSelf(rxwidget);

    gtk_fixed_move(GTK_FIXED(self), moveWidget, xpos, ypos);

    return 0;
}

