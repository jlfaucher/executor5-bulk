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
                          GtkWidget *widget,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxObjectPtr rxwidget = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxwidget);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Methods                                                             */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a new Window.
 *
 * @param type    The window type.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxWindowNew,              // Object_method name
            int, type,                 // Window type
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_window_new((GtkWindowType)type);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get_title
 *
 * Get the title for a window.
 *
 * @return        Title string
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxWindowGetTitle,         // Object_method name
            CSELF, self)               // GTK self
{
    const char * retc = gtk_window_get_title(GTK_WINDOW(self));
    if (retc == NULL) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    return (RexxObjectPtr)context->NewStringFromAsciiz(retc);
}

/**
 * Method:  set_title
 *
 * Set the title for a window.
 *
 * @param title   The window title string.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxWindowSetTitle,         // Object_method name
            CSTRING, title,            // Window title
            CSELF, self)               // GTK self
{
    gtk_window_set_title(GTK_WINDOW(self), title);

    return 0;
}

/**
 * Method:  set_modal
 *
 * Set the modal flag for the window.
 *
 * @param modal   The modal flag
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxWindowSetModal,         // Object_method name
            logical_t, modal,          // Window modal flag
            CSELF, self)               // GTK self
{
    gtk_window_set_modal(GTK_WINDOW(self), modal);

    return 0;
}

/**
 * Method:  get_modal
 *
 * Get the modal flag for the window.
 *
 * @return        Modal boolean
 **/
RexxMethod1(logical_t,                 // Return type
            GrxWindowGetModal,         // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_window_get_modal(GTK_WINDOW(self));
}

/**
 * Method:  add_accel_group
 *
 * Add an accelerator group to the window,
 *
 * @param group   The accel group.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                        // Return type
            GrxWindowAddAccelGroup,     // Object_method name
            RexxObjectPtr, accelgrp,    // Accelerator group
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(accelgrp, "GtkAccelGroup")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkAccelGroup"));
        return 0;
    }
    GtkAccelGroup *accelgrpWidget = (GtkAccelGroup *)context->ObjectToCSelf(accelgrp);

    gtk_window_add_accel_group(GTK_WINDOW(self), accelgrpWidget);

    return 0;
}

/**
 * Method:  signal_connect
 *
 * Connect a signal to an ooRexx method.
 *
 * @param name    The signal name
 *
 * @return        Zero.
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxWindowSignalConnect,    // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "activate_default") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_scroll_adjustments";
        g_signal_connect(G_OBJECT(self), "set-scroll-adjustments",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "activate_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate_focus";
        g_signal_connect(G_OBJECT(self), "activate-focus",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "keys_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_keys_changed";
        g_signal_connect(G_OBJECT(self), "keys-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "set_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_focus";
        g_signal_connect(G_OBJECT(self), "set-focus",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

