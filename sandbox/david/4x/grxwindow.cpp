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

    cblock->context->SendMessage0(GrxDBFindObject(widget), ((cbcb *)data)->signal_name);
    return;
}

static void signal_func_1(GtkWidget *window,
                          GtkWidget *widget,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;

    cblock->context->SendMessage1(GrxDBFindObject(widget), ((cbcb *)data)->signal_name,
                                  GrxDBFindObject(widget));
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
 */
RexxMethod1(int,                       // Return type
            GrxWindowNew,              // Object_method name
            int, type)                 // Window type
{
    GtkWidget       *myWidget;

    myWidget = gtk_window_new(type);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/**
 * Method:  set_title
 *
 * Set the title for a window.
 *
 * @param title   The title
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxWindowSetTitle,         // Object_method name
            CSTRING, title)            // Window title
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_window_set_title(GTK_WINDOW(myWidget), title);

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
 */
RexxMethod1(int,                       // Return type
            GrxWindowSetTitle,         // Object_method name
            bool, modal)               // Window modal flag
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_window_set_modal(GTK_WINDOW(myWidget), modal);

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
 */
RexxMethod1(int,                       // Return type
            GrxWindowSignalConnect,    // Object_method name
            CSTRING, name)             // Signal name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(Argv[1].strptr, "activate_default") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->context = context->threadContext;
        cblock->signal_name = "signal_set_scroll_adjustments";
        g_signal_connect(G_OBJECT(myWidget), "set-scroll-adjustments",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else if (strcmp(Argv[1].strptr, "activate_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->context = context->threadContext;
        cblock->signal_name = "signal_activate_focus";
        g_signal_connect(G_OBJECT(myWidget), "activate-focus",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else if (strcmp(Argv[1].strptr, "keys_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->context = context->threadContext;
        cblock->signal_name = "signal_keys_changed";
        g_signal_connect(G_OBJECT(myWidget), "keys-changed",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else if (strcmp(Argv[1].strptr, "set_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->context = context->threadContext;
        cblock->signal_name = "signal_set_focus";
        g_signal_connect(G_OBJECT(myWidget), "set-focus",
                         G_CALLBACK(signal_func_1), cblock);
    }
    else {
        RexxObjectPtr parent = context->GetSuper();
        context->SendMessage0(parent, name);
    }

    return 0;
}

