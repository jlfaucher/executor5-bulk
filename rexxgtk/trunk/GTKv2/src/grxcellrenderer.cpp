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

static void signal_func_0(GtkCellRenderer *renderer,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(renderer), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static void signal_func_1(GtkCellRenderer *renderer,
                          gchar *arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(renderer), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr tempobj1 = context->NewStringFromAsciiz(arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, tempobj1);
    context->DetachThread();
    return;
}

static void signal_func_2(GtkCellRenderer *renderer,
                          gchar *arg1,
                          gchar *arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(renderer), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr tempobj1 = context->NewStringFromAsciiz(arg2);;
    RexxObjectPtr tempobj2 = context->NewStringFromAsciiz(arg2);;
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, tempobj1, tempobj2);
    context->DetachThread();
    return;
}

static void signal_func_2a(GtkCellRenderer *renderer,
                           GtkCellEditable *arg1,
                           gchar *arg2,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(renderer), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr tempobj1 = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg1), "OORXOBJECT");
    RexxObjectPtr tempobj2 = context->NewStringFromAsciiz(arg2);;
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, tempobj1, tempobj2);
    context->DetachThread();
    return;
}

static void signal_func_4(GtkCellRenderer *renderer,
                          gchar *arg1,
                          gint arg2,
                          gint arg3,
                          guint arg4,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(renderer), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = context->NewStringFromAsciiz(arg1);
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg3);;
    context->ArrayPut(arr, tempobj, 3);
    tempobj = context->StringSizeToObject((size_t)arg4);;
    context->ArrayPut(arr, tempobj, 4);
    context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  set_fixed_size
 *
 * Sets a fixed size for the renderer object.
 *
 * @param width   The width
 *
 * @param height  The height
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxCellRendererSetFixedSize, // Object_method name
            int, width,                // Width
            int, height,               // Height
            CSELF, self)               // GTK self
{
    gtk_cell_renderer_set_fixed_size(GTK_CELL_RENDERER(self), width, height);

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
            GrxCellRendererSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "editing_canceled") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_editing_canceled";
        g_signal_connect(G_OBJECT(self), "editing-canceled",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "editing_started") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_editing_started";
        g_signal_connect(G_OBJECT(self), "editing-started",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    return context->False();
}

/**
 * Method:  init
 *
 * Create a cell renderer text object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxCellRendererTextNew,    // Object_method name
            OSELF, self)               // Self
{
    GtkCellRenderer *myRenderer = gtk_cell_renderer_text_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myRenderer));
    g_object_set_data(G_OBJECT(myRenderer), "OORXOBJECT", self);

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
            GrxCellRendererTextSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "edited") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_edited";
        g_signal_connect(G_OBJECT(self), "edited",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

/**
 * Method:  init
 *
 * Create a cell renderer pixbuf object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxCellRendererPixbufNew,  // Object_method name
            OSELF, self)               // Self
{
    GtkCellRenderer *myRenderer = gtk_cell_renderer_pixbuf_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myRenderer));
    g_object_set_data(G_OBJECT(myRenderer), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a cell renderer progress object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxCellRendererProgressNew, // Object_method name
            OSELF, self)               // Self
{
    GtkCellRenderer *myRenderer = gtk_cell_renderer_progress_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myRenderer));
    g_object_set_data(G_OBJECT(myRenderer), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a cell renderer toggle object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxCellRendererToggleNew,  // Object_method name
            OSELF, self)               // Self
{
    GtkCellRenderer *myRenderer = gtk_cell_renderer_toggle_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myRenderer));
    g_object_set_data(G_OBJECT(myRenderer), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_radio
 *
 * Set/get the toggle a radio button.
 *
 * @param flag    The radio boolean
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxCellRendererToggleGetSetRadio, // Object_method name
            OPTIONAL_logical_t, flag,  // Radio boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_cell_renderer_toggle_set_radio(GTK_CELL_RENDERER_TOGGLE(self), flag);
    }
    else {
        return gtk_cell_renderer_toggle_get_radio(GTK_CELL_RENDERER_TOGGLE(self));
    }

    return 0;
}

/**
 * Method:  set/get_active
 *
 * Set/get the toggle active.
 *
 * @param flag    The active boolean
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxCellRendererToggleGetSetActive, // Object_method name
            OPTIONAL_logical_t, flag,  // Radio boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_cell_renderer_toggle_set_active(GTK_CELL_RENDERER_TOGGLE(self), flag);
    }
    else {
        return gtk_cell_renderer_toggle_get_active(GTK_CELL_RENDERER_TOGGLE(self));
    }

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
            GrxCellRendererToggleSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "toggled") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggled";
        g_signal_connect(G_OBJECT(self), "toggled",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

/**
 * Method:  init
 *
 * Create a cell renderer accel object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxCellRendererAccelNew,   // Object_method name
            OSELF, self)               // Self
{
    GtkCellRenderer *myRenderer = gtk_cell_renderer_accel_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myRenderer));
    g_object_set_data(G_OBJECT(myRenderer), "OORXOBJECT", self);

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
            GrxCellRendererAccelSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "accel_cleared") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_accel_cleared";
        g_signal_connect(G_OBJECT(self), "accel-cleared",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "accel_edited") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_accel_edited";
        g_signal_connect(G_OBJECT(self), "accel-edited",
                         G_CALLBACK(signal_func_4), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

/**
 * Method:  init
 *
 * Create a cell renderer combo object.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxCellRendererComboNew,   // Object_method name
            OSELF, self)               // Self
{
    GtkCellRenderer *myRenderer = gtk_cell_renderer_combo_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myRenderer));
    g_object_set_data(G_OBJECT(myRenderer), "OORXOBJECT", self);

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
            GrxCellRendererComboSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_changed";
        g_signal_connect(G_OBJECT(self), "changed",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

