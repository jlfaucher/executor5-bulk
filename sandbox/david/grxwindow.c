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
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s", window, data);
#else
    snprintf(buffer, sizeof(buffer), "%p %s", window, data);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_2(GtkWidget *window,
                          GtkWidget *widget,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %p", window, data, widget);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %p", window, data, widget);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWindowNew                                       */
/* Description: Create a new toplevel window                                  */
/* Rexx Args:   Window type                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWindowNew(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget       *myWidget;
    GtkWindowType   type;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    if(strcmp(Argv[0].strptr,"GTK_WINDOW_TOPLEVEL") == 0)
        type = GTK_WINDOW_TOPLEVEL;
    else if(strcmp(Argv[0].strptr,"GTK_WINDOW_POPUP") == 0)
        type = GTK_WINDOW_POPUP;
    else 
        sscanf(Argv[0].strptr, "%d", &type);

    myWidget = gtk_window_new(type);

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWindowSetTitle                                  */
/* Description: Set the title of the Window                                   */
/* Rexx Args:   Pointer to the widget                                         */
/*              Title                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWindowSetTitle(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    // Note that we do not validate the title argument as it can be a
    // zero-length string
    if (Argc != 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_window_set_title(GTK_WINDOW(myWidget), Argv[1].strptr);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWindowStart                                     */
/* Description: Start the background main loop returns GThread object         */
/* Rexx Args:   Pointer to the window widget                                  */
/*----------------------------------------------------------------------------*/

static void dlgClose( GtkWidget *dialog, gpointer data )
{
	if(gtk_main_level()) // Quit main loop if it exists.
       gtk_main_quit();
}

static gpointer DialogThread(gpointer dialog)
{
	gtk_signal_connect(GTK_OBJECT(dialog), "destroy", GTK_SIGNAL_FUNC(dlgClose), 0);

    gdk_threads_enter();
	gtk_main();
    gdk_threads_leave();

    return (gpointer) 0;
}

APIRET APIENTRY GrxWindowStart(const char * Name,
                               const size_t Argc, const RXSTRING Argv[],
                               const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GThread   *thd          = 0;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        /* Start the dialog thread */
        thd =  g_thread_create( DialogThread, myWidget, 1, NULL);
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", thd);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWindowConnectSignal                             */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWindowConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "activate_default") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "destroy",
                             G_CALLBACK(signal_func_1), "signal_activate_default");
        }
        else if (strcmp(Argv[1].strptr, "activate_focus") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "activate-focus",
                             G_CALLBACK(signal_func_1), "signal_activate_focus");
        }
        else if (strcmp(Argv[1].strptr, "keys_changed") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "keys-changed",
                             G_CALLBACK(signal_func_1), "signal_keys_changed");
        }
        else if (strcmp(Argv[1].strptr, "set_focus") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "set-focus",
                             G_CALLBACK(signal_func_2), "signal_set_focus");
        }
        else {
            return RXFUNC_BADCALL;
        }
    }
    else {
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

