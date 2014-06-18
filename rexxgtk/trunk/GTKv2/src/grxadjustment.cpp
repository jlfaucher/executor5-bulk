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

static void signal_func_0(GtkWidget *window,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init (6 args)
 *
 * Create an adjustment.
 *
 * @param value   The value
 *
 * @param upper   The upper limit
 *
 * @param lower   The lower limit
 *
 * @param step    The step increment
 *
 * @param page    The page increment
 *
 * @param pagesz  The page size
 *
 * @return        Zero.
 **/
RexxMethod7(int,                       // Return type
            GrxAdjustmentNew,          // Object_method name
            double, value,             // Value
            double, upper,             // Upper limit
            double, lower,             // Lower limit
            double, step,              // Step increment
            double, page,              // Page increment
            double, pagesz,            // Page size
            OSELF, self)               // Self
{
    GtkAdjustment   *adj;

    adj = (GtkAdjustment *)gtk_adjustment_new(value, upper, lower, step,
                                              page, pagesz);

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(adj));
    g_object_set_data(G_OBJECT(adj), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init (1 arg)
 *
 * Create an adjustment fro an existing widget.
 *
 * @param rxptr   The adjustment pointer
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAdjustmentNewFromPtr,   // Object_method name
            POINTER, rxptr,            // Widget pointer
            OSELF, self)               // Self
{
    GtkAdjustment *adj = (GtkAdjustment *)rxptr;

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(adj));
    g_object_set_data(G_OBJECT(adj), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_value
 *
 * Set the current value.
 *
 * @param value   The new value
 *
 * @return        Zero
 **/
RexxMethod2(double,                    // Return type
            GrxAdjustmentGetSetValue,  // Object_method name
            OPTIONAL_double, value,    // New value
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_adjustment_set_value(GTK_ADJUSTMENT(self), value);
    }
    else {
        return gtk_adjustment_get_value(GTK_ADJUSTMENT(self));
    }

    return 0.0;
}

/**
 * Method:  clamp_page
 *
 * Clamp the page.
 *
 * @param lower   The lower limit
 *
 * @param upper   The upper limit
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxAdjustmentClampPage,    // Object_method name
            double, lower,             // Lower limit
            double, upper,             // Upper limit
            CSELF, self)               // GTK self
{
    gtk_adjustment_clamp_page(GTK_ADJUSTMENT(self), lower, upper);

    return 0;
}

/**
 * Method:  signal_connect
 *
 * Connect a signal to an ooRexx method.
 *
 * @param name    The signal name
 *
 * @return        Zero
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxAdjustmentSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_changed";
        g_signal_connect(G_OBJECT(self), "changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "value_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_value_changed";
        g_signal_connect(G_OBJECT(self), "value-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->False();
}

