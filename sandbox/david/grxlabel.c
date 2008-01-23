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
    g_snprintf(buffer, sizeof(buffer), "%p %s", window, data);
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
    g_snprintf(buffer, sizeof(buffer), "%p %s %p", window, data, widget);
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_3(GtkWidget *window,
                          GtkMovementStep arg1,
                          gint arg2,
                          gboolean arg3,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s %p %d %d %d", window, data, arg1, arg2, arg3);
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
/* Rexx External Function: GrxLabelNew                                        */
/* Description: Create a new label                                            */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxLabelNew(const char * Name,
                            const size_t Argc, const RXSTRING Argv[],
                            const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (Argc > 1) {
        return RXFUNC_BADCALL;
    }

	if(Argc)
       myWidget = gtk_label_new(Argv[0].strptr);
    else
       myWidget = gtk_label_new("\0");

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxLabelSetText                                    */
/* Description: Set the text in the label                                     */
/* Rexx Args:   Pointer to the container widget                               */
/*              Text                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxLabelSetText(const char * Name,
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_label_set_text(GTK_LABEL(myWidget), Argv[1].strptr); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxLabelSetJustify                                 */
/* Description: Set the justification for the text in the label               */
/* Rexx Args:   Pointer to the container widget                               */
/*              Justification                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxLabelSetJustify(const char * Name,
                                   const size_t Argc, const RXSTRING Argv[],
                                   const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkJustification jtype;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc,Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    if (strcmp(Argv[1].strptr, "GTK_JUSTIFY_LEFT") == 0) {
        jtype = GTK_JUSTIFY_LEFT;
    }
    else if (strcmp(Argv[1].strptr, "GTK_JUSTIFY_RIGHT") == 0) {
        jtype = GTK_JUSTIFY_RIGHT;
    }
    else if (strcmp(Argv[1].strptr, "GTK_JUSTIFY_CENTER") == 0) {
        jtype = GTK_JUSTIFY_CENTER;
    }
    else if (strcmp(Argv[1].strptr, "GTK_JUSTIFY_FILL") == 0) {
        jtype = GTK_JUSTIFY_FILL;
    }
    else {
        sscanf(Argv[1].strptr, "%d", &jtype);
    }

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_label_set_justify(GTK_LABEL(myWidget), jtype); 
        switch (jtype) {
        case GTK_JUSTIFY_LEFT:
            gtk_misc_set_alignment(GTK_MISC(myWidget), 0, 0);
            break;
        case GTK_JUSTIFY_RIGHT:
            gtk_misc_set_alignment(GTK_MISC(myWidget), 1, 0);
            break;
        case GTK_JUSTIFY_CENTER:
            gtk_misc_set_alignment(GTK_MISC(myWidget), 0.5, 0.5);
            break;
        default:
            break;
        }
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxLabelSetSelectable                              */
/* Description: Set the selectable flag                                       */
/* Rexx Args:   Pointer to the container widget                               */
/*              1 or 0                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxLabelSetSelectable(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc,Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    gtk_label_set_selectable(GTK_LABEL(myWidget), flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxLabelConnectSignal                              */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxLabelConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "copy_clipboard") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "copy-clipboard",
                             G_CALLBACK(signal_func_1), "signal_copy_clipboard");
        }
        else if (strcmp(Argv[1].strptr, "move_cursor") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "move-cursor",
                             G_CALLBACK(signal_func_3), "signal_move_cursor");
        }
        else if (strcmp(Argv[1].strptr, "populate_popup") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "populate-popup",
                             G_CALLBACK(signal_func_2), "signal_populate_popup");
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

