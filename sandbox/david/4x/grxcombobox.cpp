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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a combobox.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxComboBoxNew,            // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    myWidget = gtk_combo_box_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a combobox.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxComboBoxNewText,        // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    myWidget = gtk_combo_box_new_text();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  append_text
 *
 * Append text to a combobox.
 *
 * @param text    The text to append
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxComboBoxAppendText,     // Object_method name
            CSTRING, text)             // Text to append
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkComboBox *myWidget = (GtkComboBox *)context->PointerValue(rxptr);

    gtk_combo_box_append_text(myWidget, text);

    return 0;
}

/**
 * Method:  get_active_text
 *
 * Get the active text.
 *
 * @return        Text.
 **/
RexxMethod0(CSTRING,                   // Return type
            GrxComboBoxGetActiveText)  // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkComboBox *myWidget = (GtkComboBox *)context->PointerValue(rxptr);

    return gtk_combo_box_get_active_text(myWidget);
}

/**
 * Method:  set_active
 *
 * Set the active entry.
 *
 * @param active  The entry to set active.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxComboBoxSetActive,      // Object_method name
            int, active)               // Entry number
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkComboBox *myWidget = (GtkComboBox *)context->PointerValue(rxptr);

    gtk_combo_box_set_active(myWidget, active);

    return 0;
}

/**
 * Method:  get_active
 *
 * Get the active entry.
 *
 * @return        Entry number
 **/
RexxMethod0(int,                       // Return type
            GrxComboBoxGetActive)      // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkComboBox *myWidget = (GtkComboBox *)context->PointerValue(rxptr);

    return gtk_combo_box_get_active(myWidget);
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
            GrxComboBoxSignalConnect,  // Object_method name
            CSTRING, name,             // Image file name
            ARGLIST, args)             // The whole argument list as an array
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(name, "changed") == 0) {
            cblock = (cbcb *)malloc(sizeof(cbcb));
            cblock->instance = context->threadContext->instance;
            cblock->signal_name = "signal_changed";
            g_signal_connect(G_OBJECT(myWidget), "changed",
                             G_CALLBACK(signal_func_0), cblock);
            return context->True();
        }
        else if (strcmp(name, "move_active") == 0) {
            cblock = (cbcb *)malloc(sizeof(cbcb));
            cblock->instance = context->threadContext->instance;
            cblock->signal_name = "signal_move_active";
            g_signal_connect(G_OBJECT(myWidget), "move-active",
                             G_CALLBACK(signal_func_1), cblock);
            return context->True();
        }
        else if (strcmp(name, "popdown") == 0) {
            cblock = (cbcb *)malloc(sizeof(cbcb));
            cblock->instance = context->threadContext->instance;
            cblock->signal_name = "signal_popdown";
            g_signal_connect(G_OBJECT(myWidget), "popdown",
                             G_CALLBACK(signal_func_0), cblock);
            return context->True();
        }
        else if (strcmp(name, "popup") == 0) {
            cblock = (cbcb *)malloc(sizeof(cbcb));
            cblock->instance = context->threadContext->instance;
            cblock->signal_name = "signal_popup";
            g_signal_connect(G_OBJECT(myWidget), "popup",
                             G_CALLBACK(signal_func_0), cblock);
            return context->True();
        }
        else {
            RexxObjectPtr parent = context->GetSuper();
            return context->SendMessage0(parent, name);
        }
    }
    return context->SendSuperMessage("signal_connect", args);
}

