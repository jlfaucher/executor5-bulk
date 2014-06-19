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
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an handle box.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxHandleBoxNew,           // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_handle_box_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_shadow_type
 *
 * Set the handle box shadow type.
 *
 * @param type    The shadow type
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxHandleBoxSetShadowType, // Object_method name
            int, type,                 // Shadow type
            CSELF, self)               // GTK self
{
    gtk_handle_box_set_shadow_type(GTK_HANDLE_BOX(self), (GtkShadowType)type);

    return 0;
}

/**
 * Method:  set_position
 *
 * Set the handle box position type.
 *
 * @param type    The position type
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxHandleBoxSetPosition,   // Object_method name
            int, type,                 // Position type
            CSELF, self)               // GTK self
{
    gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(self), (GtkPositionType)type);

    return 0;
}

/**
 * Method:  set_snap_edge
 *
 * Set the handle box snap edge type.
 *
 * @param type    The snap edge type
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxHandleBoxSetSnapEdge,   // Object_method name
            int, type,                 // Snap edge type
            CSELF, self)               // GTK self
{
    gtk_handle_box_set_snap_edge(GTK_HANDLE_BOX(self), (GtkPositionType)type);

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
            GrxHandleBoxSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "child_attached") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_child_attached";
        g_signal_connect(G_OBJECT(self), "child-attached",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "child_detached") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_child_detached";
        g_signal_connect(G_OBJECT(self), "child-detached",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

