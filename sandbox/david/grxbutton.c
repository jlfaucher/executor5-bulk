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
/* Rexx External Function: GrxButtonNew                                       */
/* Description: Create a push button.                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonNew(char * Name, size_t Argc, RXSTRING Argv[],
                             char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_button_new();
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonSetLabel                                  */
/* Description: Set the label of the button.                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*              Label                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonSetLabel(char * Name, size_t Argc, RXSTRING Argv[],
                                 char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_button_set_label(GTK_BUTTON(myWidget), Argv[1].strptr);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonGetLabel                                  */
/* Description: Get the label of the button.                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonGetLabel(char * Name, size_t Argc, RXSTRING Argv[],
                                 char * Queuename, PRXSTRING Retstr)
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
        label = gtk_button_get_label(GTK_BUTTON(myWidget));
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
/* Description: Set the button to use mnemonics                               */
/* Rexx Args:   Pointer to the widget                                         */
/*              Flag                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonUseUnderline(char * Name, size_t Argc, RXSTRING Argv[],
                                      char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    flag = atoi(Argv[1].strptr);

    gtk_button_set_use_underline(GTK_BUTTON(myWidget), flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxCheckButtonNew                                  */
/* Description: Create a check button.                                        */
/* Rexx Args:   Label                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxCheckButtonNew(char * Name, size_t Argc, RXSTRING Argv[],
                                 char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (Argc > 1) {
        return RXFUNC_BADCALL;
    }

    if (Argc == 1 && RXVALIDSTRING(Argv[0])) {
       myWidget = gtk_check_button_new_with_label(Argv[0].strptr);
    }
    else {
        myWidget = gtk_check_button_new();
		
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxCheckButtonGetState                             */
/* Description: Get the checked state of the button                           */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxCheckButtonGetState(char * Name, size_t Argc, RXSTRING Argv[],
                                      char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean checked = FALSE;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(myWidget));
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", (int)checked);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxCheckButtonSetState                             */
/* Description: Set the checked state of the button                           */
/* Rexx Args:   Pointer to the widget                                         */
/*              New state                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxCheckButtonSetState(char * Name, size_t Argc, RXSTRING Argv[],
                                      char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean checked = FALSE;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &checked);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(myWidget), checked);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRadioButtonNew                                  */
/* Description: Create a radio button.                                        */
/* Rexx Args:   Group list                                                    */
/*              Label (optional)                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRadioButtonNew(char * Name, size_t Argc, RXSTRING Argv[],
                                 char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GSList *head;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);

    if (Argc == 2 && RXVALIDSTRING(Argv[1])) {
        myWidget = gtk_radio_button_new_with_label(head, Argv[1].strptr);
    }
    else {
        myWidget = gtk_radio_button_new(head);
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRadioButtonGroup                                */
/* Description: Get the radio button group.                                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRadioButtonGroup(char * Name, size_t Argc, RXSTRING Argv[],
                                   char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GSList *head = NULL;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        head = gtk_radio_button_group(GTK_RADIO_BUTTON(myWidget));
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonConnectSignal                             */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "pressed") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "pressed",
                             G_CALLBACK(signal_func_1), "signal_pressed");
        }
        else if (strcmp(Argv[1].strptr, "released") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "released",
                             G_CALLBACK(signal_func_1), "signal_released");
        }
        else if (strcmp(Argv[1].strptr, "clicked") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "clicked",
                             G_CALLBACK(signal_func_1), "signal_clicked");
        }
        else if (strcmp(Argv[1].strptr, "enter") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "enter",
                             G_CALLBACK(signal_func_1), "signal_enter");
        }
        else if (strcmp(Argv[1].strptr, "leave") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "leave",
                             G_CALLBACK(signal_func_1), "signal_leave");
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

