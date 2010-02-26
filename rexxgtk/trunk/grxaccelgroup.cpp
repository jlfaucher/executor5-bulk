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

static gboolean signal_func_3a(GtkAccelGroup *accelgrp,
                           GObject *arg1,
                           guint arg2,
                           GdkModifierType arg3,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(accelgrp), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg1), "OORXOBJECT");
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg3);;
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}

static void signal_func_3b(GtkAccelGroup *accelgrp,
                           guint arg1,
                           GdkModifierType arg2,
                           GClosure *arg3,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(accelgrp), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, tempobj, 2);
    tempobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg3), "OORXOBJECT");
    context->ArrayPut(arr, tempobj, 3);
    context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an accelerator group.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxAccelGroupNew,          // Object_method name
            OSELF, self)               // Self
{
    GtkAccelGroup *accelgrp;

    accelgrp = gtk_accel_group_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(accelgrp));
    g_object_set_data(G_OBJECT(accelgrp), "OORXOBJECT", self);

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
            GrxAccelGroupSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "accel_activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_accel_activate";
        g_signal_connect(G_OBJECT(self), "accel-activate",
                         G_CALLBACK(signal_func_3a), cblock);
        return context->True();
    }
    else if (strcmp(name, "accel_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_accel_changed";
        g_signal_connect(G_OBJECT(self), "accel-changed",
                         G_CALLBACK(signal_func_3b), cblock);
        return context->True();
    }
    return context->False();
}

