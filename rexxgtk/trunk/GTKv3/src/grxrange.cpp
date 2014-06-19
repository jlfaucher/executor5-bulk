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

static void signal_func_1(GtkWidget *window,
                          GtkScrollType arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr scrolltype = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, scrolltype);
    context->DetachThread();
    return;
}

static void signal_func_1a(GtkWidget *window,
                           gdouble arg1,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->DoubleToObject(arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}

static void signal_func_2(GtkWidget *window,
                          GtkScrollType arg1,
                          gdouble arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    RexxObjectPtr arg2type = context->DoubleToObject(arg2);;
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, arg1type, arg2type);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  set/get_digits
 *
 * Set/get the scale digits.
 *
 * @param digits  The number of digits
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxScaleGetSetDigits,      // Object_method name
            OPTIONAL_int, digits,      // Digits
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_scale_set_digits(GTK_SCALE(self), digits);
    }
    else {
        return gtk_scale_get_digits(GTK_SCALE(self));
    }

    return 0;
}

/**
 * Method:  set/get_value_pos
 *
 * Set/get the scale value position type.
 *
 * @param values  The value
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxScaleGetSetValuePos,    // Object_method name
            OPTIONAL_int, type,        // Position type
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_scale_set_value_pos(GTK_SCALE(self), (GtkPositionType)type);
    }
    else {
        return gtk_scale_get_value_pos(GTK_SCALE(self));
    }

    return 0;
}

/**
 * Method:  init
 *
 * Create a new HScale.
 *
 * @param value   The initial value
 *
 * @param lower   The lower limit
 *
 * @param upper   The upper limit
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
            GrxHScaleNew,              // Object_method name
            double, value,             // Initial value
            double, lower,             // Lower limit
            double, upper,             // Upper limit
            double, step,              // Step increment
            double, page,              // Page increment
            double, pagesz,            // Page size
            OSELF, self)               // Self
{
    GtkWidget *myWidget;
    GtkAdjustment *adj = (GtkAdjustment *) gtk_adjustment_new(value, lower, upper, step, page, pagesz);
    myWidget = gtk_hscale_new(adj);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new HScale with a range.
 *
 * @param min     The minimum
 *
 * @param max     The maximum
 *
 * @param step    The step increment
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxHScaleNewWithRange,     // Object_method name
            double, min,               // Minimum
            double, max,               // Maximum
            double, step,              // Step increment
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_hscale_new_with_range(min, max, step);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new VScale.
 *
 * @param value   The initial value
 *
 * @param lower   The lower limit
 *
 * @param upper   The upper limit
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
            GrxVScaleNew,              // Object_method name
            double, value,             // Initial value
            double, lower,             // Lower limit
            double, upper,             // Upper limit
            double, step,              // Step increment
            double, page,              // Page increment
            double, pagesz,            // Page size
            OSELF, self)               // Self
{
    GtkWidget *myWidget;
    GtkAdjustment *adj = (GtkAdjustment *) gtk_adjustment_new(value, lower, upper, step, page, pagesz);
    myWidget = gtk_vscale_new(adj);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new VScale with a range.
 *
 * @param min     The minimum
 *
 * @param max     The maximum
 *
 * @param step    The step increment
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxVScaleNewWithRange,     // Object_method name
            double, min,               // Minimum
            double, max,               // Maximum
            double, step,              // Step increment
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_vscale_new_with_range(min, max, step);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

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
RexxMethod3(RexxObjectPtr,             // Return type
            GrxRangeSignalConnect,     // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "adjust_bounds") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_adjust_bounds";
        g_signal_connect(G_OBJECT(self), "adjust-bounds",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_slider") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_slider";
        g_signal_connect(G_OBJECT(self), "move-slider",
                         G_CALLBACK(signal_func_1), cblock);
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
    else if (strcmp(name, "change_value") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_change_value";
        g_signal_connect(G_OBJECT(self), "change-value",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

