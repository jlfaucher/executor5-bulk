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
 * Method:  add1
 *
 * Add a widget to a paned.
 *
 * @param addWidget The widget to add.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxPanedAdd1,              // Object_method name
            RexxObjectPtr, rxadd,      // Widget to add
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxadd, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *addWidget = (GtkWidget *)context->ObjectToCSelf(rxadd);

    gtk_paned_add1(GTK_PANED(self), addWidget); 

    return 0;
}

/**
 * Method:  add2
 *
 * Add a widget to a paned.
 *
 * @param addWidget The widget to add.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxPanedAdd2,              // Object_method name
            RexxObjectPtr, rxadd,      // Widget to add
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxadd, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *addWidget = (GtkWidget *)context->ObjectToCSelf(rxadd);

    gtk_paned_add2(GTK_PANED(self), addWidget); 

    return 0;
}

/**
 * Method:  pack1
 *
 * Pack a widget to a paned.
 *
 * @param addWidget The widget to add.
 *
 * @return        Zero
 **/
RexxMethod4(int,                       // Return type
            GrxPanedPack1,             // Object_method name
            RexxObjectPtr, rxpack,     // Widget to add
            logical_t, resize,         // Resize flag
            logical_t, shrink,         // Shrink flag
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxpack, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *packWidget = (GtkWidget *)context->ObjectToCSelf(rxpack);

    gtk_paned_pack1(GTK_PANED(self), packWidget, resize, shrink); 

    return 0;
}

/**
 * Method:  pack2
 *
 * Pack a widget to a paned.
 *
 * @param addWidget The widget to add.
 *
 * @return        Zero
 **/
RexxMethod4(int,                       // Return type
            GrxPanedPack2,             // Object_method name
            RexxObjectPtr, rxpack,     // Widget to add
            logical_t, resize,         // Resize flag
            logical_t, shrink,         // Shrink flag
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxpack, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *packWidget = (GtkWidget *)context->ObjectToCSelf(rxpack);

    gtk_paned_pack2(GTK_PANED(self), packWidget, resize, shrink); 

    return 0;
}

/**
 * Method:  init
 *
 * Initialize a VPand object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxVPanedNew,              // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_vpaned_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Initialize a HPand object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxHPanedNew,              // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_hpaned_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

