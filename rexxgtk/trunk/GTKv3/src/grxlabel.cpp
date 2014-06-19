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

static void signal_func_2(GtkWidget *window,
                          GtkMovementStep arg1,
                          gint arg2,
                          gboolean arg3,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxArrayObject arr = context->NewArray(1);

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxstep = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->ArrayPut(arr, rxstep, 1);
    RexxObjectPtr rxarg2 = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, rxarg2, 2);
    RexxObjectPtr rxarg3 = context->WholeNumberToObject((wholenumber_t)arg3);;
    context->ArrayPut(arr, rxarg3, 3);
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
 * Create an label.
 *
 * @param text    The text of the label (optional).
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxLabelNew,               // Object_method name
            OPTIONAL_CSTRING, text,    // Label text
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

	if(argumentExists(1))
       myWidget = gtk_label_new(text);
    else
       myWidget = gtk_label_new("\0");

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_label
 *
 * Set/get the text of the label.
 *
 * @param text    The text of the label.
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxLabelGetSetLabel,       // Object_method name
            OPTIONAL_CSTRING, text,    // Label text
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_label_set_label(GTK_LABEL(self), text);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_label_get_label(GTK_LABEL(self)));
    }

    return retc;
}

/**
 * Method:  set/get_justify
 *
 * Set/get the label justification
 *
 * @param jtype   The justification type
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxLabelGetSetJustify,     // Object_method name
            OPTIONAL_int, jtype,       // Justification type
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_label_set_justify(GTK_LABEL(self), (GtkJustification)jtype);
        switch (jtype) {
        case GTK_JUSTIFY_LEFT:
            gtk_misc_set_alignment(GTK_MISC(self), 0, 0);
            break;
        case GTK_JUSTIFY_RIGHT:
            gtk_misc_set_alignment(GTK_MISC(self), 1, 0);
            break;
        case GTK_JUSTIFY_CENTER:
            gtk_misc_set_alignment(GTK_MISC(self), 0.5, 0.5);
            break;
        default:
            break;
        }
    }
    else {
        return (int)gtk_label_get_justify(GTK_LABEL(self));
    }

    return 0;
}

/**
 * Method:  set_selectable
 *
 * Set the label selectable flag.
 *
 * @param flag    The selectable boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxLabelSetSelectable,     // Object_method name
            logical_t, flag,           // Selectable boolean
            CSELF, self)               // GTK self
{
    gtk_label_set_selectable(GTK_LABEL(self), flag);

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
            GrxLabelSignalConnect,     // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "copy_clipboard") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_copy_clipboard";
        g_signal_connect(G_OBJECT(self), "copy-clipboard",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_cursor";
        g_signal_connect(G_OBJECT(self), "move-cursor",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "populate_popup") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_populate_popup";
        g_signal_connect(G_OBJECT(self), "populate-popup",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

