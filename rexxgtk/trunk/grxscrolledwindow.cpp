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
                          GtkDirectionType arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}


static void signal_func_2(GtkWidget *window,
                          GtkScrollType arg1,
                          gboolean arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    RexxObjectPtr arg2type = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, arg1type, arg2type);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a scrolled window.
 *
 * @param rxhadj  The horizontal adjustment object
 *
 * @param rxvadj  The vertical adjustment object
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxScrolledWindowNew,      // Object_method name
            OPTIONAL_RexxObjectPtr, rxhadj, // The horizontal adjustment
            OPTIONAL_RexxObjectPtr, rxvadj, // The vertical adjustment
            OSELF, self)               // Self
{
    GtkAdjustment *hadj = NULL, *vadj = NULL;

    if (argumentExists(1)) {
        if (rxhadj != context->Nil()) {
            if (!context->IsOfType(rxhadj, "GtkAdjustment")) {
                context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                         context->WholeNumberToObject(1),
                                         context->NewStringFromAsciiz("GtkAdjustment"));
                return 0;
            }
            hadj = (GtkAdjustment *)context->ObjectToCSelf(rxhadj);
        }
    }
    if (argumentExists(2)) {
        if (rxvadj != context->Nil()) {
            if (!context->IsOfType(rxvadj, "GtkAdjustment")) {
                context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                         context->WholeNumberToObject(2),
                                         context->NewStringFromAsciiz("GtkAdjustment"));
                return 0;
            }
            vadj = (GtkAdjustment *)context->ObjectToCSelf(rxvadj);
        }
    }
    GtkWidget *myWidget = gtk_scrolled_window_new(hadj, vadj);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get_hadjustment
 *
 * Get the horizontal adjustment.
 *
 * @return        Horizontal adjustment object
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxScrolledWindowGetHAdjustment, // Object_method name
            OSELF, oself,              // Self
            CSELF, cself)              // GTK self
{
    GtkAdjustment *adj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(cself));
    RexxObjectPtr rxhadj = (RexxObjectPtr)g_object_get_data(G_OBJECT(adj), "OORXOBJECT");
    if (rxhadj == NULL) {
        rxhadj = context->SendMessage1((RexxObjectPtr)oself, "create_adj", context->NewPointer(adj));
    }
    return rxhadj;
}

/**
 * Method:  get_vadjustment
 *
 * Get the vertical adjustment.
 *
 * @return        Vertical adjustment object
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxScrolledWindowGetVAdjustment, // Object_method name
            OSELF, oself,              // Self
            CSELF, cself)              // GTK self
{
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(cself));
    RexxObjectPtr rxvadj = (RexxObjectPtr)g_object_get_data(G_OBJECT(adj), "OORXOBJECT");
    if (rxvadj == NULL) {
        rxvadj = context->SendMessage1((RexxObjectPtr)oself, "create_adj", context->NewPointer(adj));
    }
    return rxvadj;
}

/**
 * Method:  set_policy
 *
 * Set the adjustment policy
 *
 * @param hpolicy The horizontal policy
 *
 * @param vpolicy The vertical policy
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxScrolledWindowSetPolicy, // Object_method name
            int, hpolicy,              // Horizontal policy
            int, vpolicy,              // Vertical policy
            CSELF, self)               // GTK self
{
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self),
                                   (GtkPolicyType)hpolicy,
                                   (GtkPolicyType)vpolicy);

    return 0;
}

/**
 * Method:  add_with_viewport
 *
 * Add a child without native scrolling support.
 *
 * @param rxchild The child widget
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxScrolledWindowAddWithViewport, // Object_method name
            RexxObjectPtr, rxchild,    // Child widget
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxchild, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *myChild = (GtkWidget *)context->ObjectToCSelf(rxchild);

    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(self), myChild);

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
            GrxScrolledWindowSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "move_focus_out") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_focus_out";
        g_signal_connect(G_OBJECT(self), "move-focus-out",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "scroll_child") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_scroll_child";
        g_signal_connect(G_OBJECT(self), "scroll-child",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

