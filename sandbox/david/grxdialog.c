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
                          gint arg1,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %d", window, data);
#else
    snprintf(buffer, sizeof(buffer), "%p %s", window, data, arg1);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


static gint str2resp(const char * str)
{
    gint resptype;

    if (!strcmp(str, "GTK_RESPONSE_NONE"))
        return GTK_RESPONSE_NONE;
    else if (!strcmp(str, "GTK_RESPONSE_REJECT"))
        return GTK_RESPONSE_REJECT;
    else if (!strcmp(str, "GTK_RESPONSE_ACCEPT"))
        return GTK_RESPONSE_ACCEPT;
    else if (!strcmp(str, "GTK_RESPONSE_DELETE_EVENT"))
        return GTK_RESPONSE_DELETE_EVENT;
    else if (!strcmp(str, "GTK_RESPONSE_OK"))
        return GTK_RESPONSE_OK;
    else if (!strcmp(str, "GTK_RESPONSE_CANCEL"))
        return GTK_RESPONSE_CANCEL;
    else if (!strcmp(str, "GTK_RESPONSE_CLOSE"))
        return GTK_RESPONSE_CLOSE;
    else if (!strcmp(str, "GTK_RESPONSE_YES"))
        return GTK_RESPONSE_YES;
    else if (!strcmp(str, "GTK_RESPONSE_NO"))
        return GTK_RESPONSE_NO;
    else if (!strcmp(str, "GTK_RESPONSE_APPLY"))
        return GTK_RESPONSE_APPLY;
    else if (!strcmp(str, "GTK_RESPONSE_HELP"))
        return GTK_RESPONSE_HELP;
    else
        sscanf(str, "%d", &resptype);
    return resptype;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogNew                                       */
/* Description: Create a new dialog                                           */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogNew(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_dialog_new();

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p %p", myWidget, GTK_DIALOG(myWidget)->vbox);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogNewWithButtons                            */
/* Description: Create a new dialog with buttons                              */
/* Rexx Args:   Title                                                         */
/*              Parent window                                                 */
/*              Flags                                                         */
/*              Button text                                                   */
/*              Response id                                                   */
/*              ...                                                           */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogNewWithButtons(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *myParent;
    GtkDialogFlags flags;
    gint resptype;
    size_t i = 5;

    /* Check for valid arguments */
    if (Argc < 5) {
        return RXFUNC_BADCALL;
    }
    if (GrxCheckArgs(5, 5, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[1].strptr, "%p", &myParent);
    if (!strcmp(Argv[2].strptr, "GTK_DIALOG_MODAL"))
        flags = GTK_DIALOG_MODAL;
    else if (!strcmp(Argv[2].strptr, "GTK_DIALOG_DESTROY_WITH_PARENT"))
        flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    else if (!strcmp(Argv[2].strptr, "GTK_DIALOG_NO_SEPARATOR"))
        flags = GTK_DIALOG_NO_SEPARATOR;
    else
        sscanf(Argv[2].strptr, "%d", &flags);
    resptype = str2resp(Argv[4].strptr);

    myWidget = gtk_dialog_new_with_buttons(Argv[0].strptr,
                                           GTK_WINDOW(myParent),
                                           flags,
                                           Argv[3].strptr,
                                           resptype,
                                           NULL);

    // add the rest of the buttons
    while (i + 1 < Argc) {
        if (!RXVALIDSTRING(Argv[i]) || !RXVALIDSTRING(Argv[i + 1]))
            return RXFUNC_BADCALL;
        resptype = str2resp(Argv[i + 1].strptr);
        gtk_dialog_add_button(GTK_DIALOG(myWidget), Argv[i].strptr, resptype);
        i += 2;
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p %p", myWidget, GTK_DIALOG(myWidget)->vbox);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogAddButton                                 */
/* Description: Add a button to a dialog                                      */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Button text                                                   */
/*              Response id                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogAddButton(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint resptype;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    resptype = str2resp(Argv[2].strptr);

    // add the button
    gtk_dialog_add_button(GTK_DIALOG(myWidget), Argv[1].strptr, resptype);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogSetHasSeparator                           */
/* Description: Toggle the seperator                                          */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Flag                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogSetHasSeparator(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    // add the button
    gtk_dialog_set_has_separator(GTK_DIALOG(myWidget), flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogRun                                       */
/* Description: Run the dialog                                                */
/* Rexx Args:   Pointer to the dialog                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogRun(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint resp;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    // add the button
    resp = gtk_dialog_run(GTK_DIALOG(myWidget));

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", resp);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogConnectSignal                             */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "close") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "close",
                             G_CALLBACK(signal_func_1), "signal_close");
        }
        else if (strcmp(Argv[1].strptr, "response") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "response",
                             G_CALLBACK(signal_func_2), "signal_response");
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

