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
    RexxObjectPtr scrolltype = context->NumberToObject((wholenumber_t)arg1);;
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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  set_digits
 *
 * Set the scale digits.
 *
 * @param digits  The number of digits
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxScaleSetDigits,         // Object_method name
            int, digits)               // Digits
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkScale *myWidget = (GtkScale *)context->PointerValue(rxptr);

    gtk_scale_set_digits(myWidget, digits);

    return 0;
}

/**
 * Method:  get_digits
 *
 * Get the scale digits.
 *
 * @return        Digits
 **/
RexxMethod0(int,                       // Return type
            GrxScaleGetDigits)         // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkScale *myWidget = (GtkScale *)context->PointerValue(rxptr);

    return gtk_scale_get_digits(myWidget);
}

/**
 * Method:  set_value_pos
 *
 * Set the scale value position type.
 *
 * @param values  The value
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxScaleSetValuePos,       // Object_method name
            int, type)                 // Position type
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkScale *myWidget = (GtkScale *)context->PointerValue(rxptr);

    gtk_scale_set_value_pos(myWidget, (GtkPositionType)type);

    return 0;
}

/**
 * Method:  get_value_pos
 *
 * Get the scale value position type.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxScaleGetValuePos)       // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkScale *myWidget = (GtkScale *)context->PointerValue(rxptr);

    return gtk_scale_get_value_pos(myWidget);
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
            OSELF, self,               // Self
            double, value,             // Initial value
            double, lower,             // Lower limit
            double, upper,             // Upper limit
            double, step,              // Step increment
            double, page,              // Page increment
            double, pagesz)            // Page size
{
    GtkWidget *myWidget;
    GtkAdjustment *adj;

    adj = (GtkAdjustment *) gtk_adjustment_new(value, lower, upper, step, page, pagesz);
    myWidget = gtk_hscale_new(adj);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
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
            OSELF, self,               // Self
            double, min,               // Minimum
            double, max,               // Maximum
            double, step)              // Step increment
{
    GtkWidget *myWidget;

    myWidget = gtk_hscale_new_with_range(min, max, step);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
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
            OSELF, self,               // Self
            double, value,             // Initial value
            double, lower,             // Lower limit
            double, upper,             // Upper limit
            double, step,              // Step increment
            double, page,              // Page increment
            double, pagesz)            // Page size
{
    GtkWidget *myWidget;
    GtkAdjustment *adj;

    adj = (GtkAdjustment *) gtk_adjustment_new(value, lower, upper, step, page, pagesz);
    myWidget = gtk_vscale_new(adj);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
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
            GrxVScaleNewWithRange,     // Object_method name
            OSELF, self,               // Self
            double, min,               // Minimum
            double, max,               // Maximum
            double, step)              // Step increment
{
    GtkWidget *myWidget;

    myWidget = gtk_vscale_new_with_range(min, max, step);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
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
RexxMethod2(RexxObjectPtr,             // Return type
            GrxRangeSignalConnect,     // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args)             // The whole argument list as an array
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "adjust_bounds") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_adjust_bounds";
        g_signal_connect(G_OBJECT(myWidget), "adjust-bounds",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_slider") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_slider";
        g_signal_connect(G_OBJECT(myWidget), "move-slider",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "value_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_value_Changed";
        g_signal_connect(G_OBJECT(myWidget), "value-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
}

