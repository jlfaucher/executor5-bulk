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

static void signal_func_2(GtkWidget *window,
                          GtkDirectionType arg1,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s %d", window, data, arg1);
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


static void signal_func_3(GtkWidget *window,
                          GtkScrollType arg1,
                          gboolean arg2,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s %d %d", window, data, arg1, arg2);
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
/* Rexx External Function: GrxScrolledWindowNew                               */
/* Description: Create a new scrolled window                                  */
/* Rexx Args:   Horizontal adjustment                                         */
/*              Vertical adjustment                                           */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScrolledWindowNew(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget     *myWidget;     
    GtkAdjustment *hadj, *vadj;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &hadj);
    sscanf(Argv[1].strptr, "%p", &vadj);

    myWidget = gtk_scrolled_window_new(hadj, vadj);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScrolledWindowGetHAdjustment                    */
/* Description: Get the horizontal adjustment                                 */
/* Rexx Args:   Pointer to the scrolled window                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScrolledWindowGetHAdjustment(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget     *myWidget;     
    GtkAdjustment   *adj;     

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    adj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(myWidget));

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", adj);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScrolledWindowGetVAdjustment                    */
/* Description: Get the vertical adjustment                                   */
/* Rexx Args:   Pointer to the scrolled window                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScrolledWindowGetVAdjustment(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget     *myWidget;     
    GtkAdjustment   *adj;     

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(myWidget));

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", adj);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScrolledWindowSetPolicy                         */
/* Description: Set the policy of the scrolled window                         */
/* Rexx Args:   Pointer to the scrolled window                                */
/*          :   Hscroll bar policy                                            */
/*          :   Vscroll bar policy                                            */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScrolledWindowSetPolicy(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget     *myWidget;     
    GtkPolicyType hpolicy, vpolicy;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &hpolicy);
    sscanf(Argv[2].strptr, "%d", &vpolicy);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(myWidget),
                                   hpolicy, vpolicy);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScrolledWindowAddWithViewport                   */
/* Description: Add a child without native scrolling support                  */
/* Rexx Args:   Pointer to the scrolled window                                */
/*          :   Pointer to the child widget                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScrolledWindowAddWithViewport(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget     *myWidget, *myChild;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &myChild);

    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(myWidget),
                                          myChild);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScrolledWindowConnectSignal                     */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScrolledWindowConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "move-focus-out") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "move-focus-out",
                             G_CALLBACK(signal_func_2), "signal_move_focus_out");
        }
        else if (strcmp(Argv[1].strptr, "scroll-child") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "scroll-child",
                             G_CALLBACK(signal_func_3), "signal_scroll_child");
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

