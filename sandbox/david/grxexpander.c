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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxExpanderNew                                     */
/* Description: Create a expander.                                            */
/* Rexx Args:   Label                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxExpanderNew(const char * Name,
                               const size_t Argc, const RXSTRING Argv[],
                               const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_expander_new(Argv[0].strptr);
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxExpanderSetExpanded                             */
/* Description: Set the expanded state.                                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              State                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxExpanderSetExpanded(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean state;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &state);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_expander_set_expanded(GTK_EXPANDER(myWidget), state);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxExpanderSetLabel                                */
/* Description: Get the label of the expander.                                */
/* Rexx Args:   Pointer to the widget                                         */
/*              Label                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxExpanderSetLabel(const char * Name,
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
        gtk_expander_set_label(GTK_EXPANDER(myWidget), Argv[1].strptr);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxExpanderGetLabel                                */
/* Description: Get the label of the expander.                                */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxExpanderGetLabel(const char * Name,
                                    const size_t Argc, const RXSTRING Argv[],
                                    const char * Queuename, PRXSTRING Retstr)
{
    const gchar *label = NULL;
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        label = gtk_expander_get_label(GTK_EXPANDER(myWidget));
    }

    /* Set up the REXX return code */
    if (label != NULL) {
        if (strlen(label) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory(strlen(label) + 1);
        }
        strcpy(Retstr->strptr, label);
    }
    else {
        strcpy(Retstr->strptr, "\0");
    }
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonUseUnderline                              */
/* Description: Set the expander to use mnemonics                             */
/* Rexx Args:   Pointer to the widget                                         */
/*              Flag                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxExpanderUseUnderline(const char * Name,
                                        const size_t Argc, const RXSTRING Argv[],
                                        const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    gtk_expander_set_use_underline(GTK_EXPANDER(myWidget), flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxExpanderSetSpacing                              */
/* Description: Set the spacing of the expander.                              */
/* Rexx Args:   Pointer to the widget                                         */
/*              Spacing                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxExpanderSetSpacing(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint spacing;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &spacing);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_expander_set_spacing(GTK_EXPANDER(myWidget), spacing);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxExpanderConnectSignal                           */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxExpanderConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "activate") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "activate",
                             G_CALLBACK(signal_func_1), "signal_activate");
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

