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

static void signal_func_2(GtkStatusbar *window,
                          guint arg1,
                          gchar *arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name,
                          context->WholeNumberToObject((wholenumber_t)arg1),
                          context->NewStringFromAsciiz(arg2));
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a status bar.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxStatusbarNew,           // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_statusbar_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get_context_id
 *
 * Get the context id for the status bar.
 *
 * @param desc    The context description
 *
 * @return        Context id
 **/
RexxMethod2(int,                       // Return type
            GrxStatusbarGetContextId,  // Object_method name
            CSTRING, desc,             // Description
            CSELF, self)               // GTK self
{
    return gtk_statusbar_get_context_id(GTK_STATUSBAR(self), desc);
}

/**
 * Method:  push
 *
 * Push the message onto the status bar..
 *
 * @param cid     The context id
 *
 * @param text    The message text
 *
 * @return        Message id
 **/
RexxMethod3(int,                       // Return type
            GrxStatusbarPush,          // Object_method name
            int, cid,                  // Context id
            CSTRING, text,             // Message text
            CSELF, self)               // GTK self
{
    return gtk_statusbar_push(GTK_STATUSBAR(self), cid, text);
}

/**
 * Method:  pop
 *
 * Pop a message from the status bar..
 *
 * @param cid     The context id
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxStatusbarPop,           // Object_method name
            int, cid,                  // Context id
            CSELF, self)               // GTK self
{
    gtk_statusbar_pop(GTK_STATUSBAR(self), cid);

    return 0;
}

/**
 * Method:  remove
 *
 * Remove a message from the status bar..
 *
 * @param cid     The context id
 *
 * @param mid     The message id
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxStatusbarRemove,        // Object_method name
            int, cid,                  // Context id
            int, mid,                  // Message id
            CSELF, self)               // GTK self
{
    gtk_statusbar_remove(GTK_STATUSBAR(self), cid, mid);

    return 0;
}

/**
 * Method:  set/get_has_resize_grip
 *
 * Set/get a resize grip to the status bar.
 *
 * @param flag    The boolean flag
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxStatusbarGetSetHasResizeGrip, // Object_method name
            OPTIONAL_logical_t, flag,  // Boolean flag
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(self), flag);
    }
    else {
        return gtk_statusbar_get_has_resize_grip(GTK_STATUSBAR(self));
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
            GrxStatusbarSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "text_popped") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_text_popped";
        g_signal_connect(G_OBJECT(self), "text-popped",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "text_pushed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_text_pushed";
        g_signal_connect(G_OBJECT(self), "text-pushed",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

