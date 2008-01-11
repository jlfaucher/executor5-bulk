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
#include <errno.h>
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
/* Rexx External Function: GrxWidgetShow                                      */
/* Description: Show the widget                                               */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetShow(const char * Name,
                              const size_t Argc, const RXSTRING Argv[],
                              const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_widget_show(myWidget);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetShowAll                                   */
/* Description: Show the widget and all children                              */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetShowAll(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_widget_show_all(myWidget);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetHide                                      */
/* Description: Hide the widget                                               */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetHide(const char * Name,
                              const size_t Argc, const RXSTRING Argv[],
                              const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_widget_hide(myWidget);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetHideAll                                   */
/* Description: Hide a widget and all child widgets                           */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetHideAll(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_widget_hide_all(myWidget);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetSetSize                                   */
/* Description: Set the width and height of the widget                        */
/* Rexx Args:   Pointer to the widget                                         */
/*              Width                                                         */
/*              Height                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetSetSize(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint width, height;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &width);
    sscanf(Argv[2].strptr, "%d", &height);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_widget_set_size_request(myWidget, width, height);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetDestroy                                   */
/* Description: Destroy widget                                                */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetDestroy(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if(myWidget && GTK_IS_OBJECT(myWidget))
    {
       if(GTK_IS_WIDGET(myWidget))
          gtk_widget_destroy(myWidget);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetGrabFocus                                 */
/* Description: Set the keyboard focus to this widget                         */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetGrabFocus(const char * Name,
                                   const size_t Argc, const RXSTRING Argv[],
                                   const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_widget_grab_focus(myWidget);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetSetFont                                   */
/* Description: Set the font the widget uses                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*              Font name                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetSetFont(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkStyle *style;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

	/* Set current widget font */
    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        style = gtk_style_copy(gtk_widget_get_style(myWidget));
        style->font_desc = pango_font_description_from_string(Argv[1].strptr);
        gtk_widget_set_style(myWidget, style);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetGetFont                                   */
/* Description: Get the font the widget uses                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*              Font name                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetGetFont(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkStyle *style = NULL;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        style = gtk_widget_get_style(myWidget);
    }

    /* Set up the REXX return code */
    if (style != NULL) {
        sprintf(Retstr->strptr, "%s",
                pango_font_description_to_string(style->font_desc));
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetGetAccessible                             */
/* Description: Get the pointer to the accessibility object                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetGetAccessible(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    AtkObject *access = NULL;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    /* get the accessibility pointer */
    access = gtk_widget_get_accessible(myWidget);

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", access);
    Retstr->strlength = strlen(Retstr->strptr);


    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetGetName                                   */
/* Description: Get the name the widget uses                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetGetName(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    const gchar *name;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    name = gtk_widget_get_name(myWidget);

    /* Set up the REXX return code */
    strcpy(Retstr->strptr, name);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetSetName                                   */
/* Description: Set the name the widget uses                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*              New name                                                      */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetSetName(const char * Name,
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

    if (strlen(Argv[1].strptr) > RXAUTOBUFLEN - 1) {
            *(Argv[1].strptr + RXAUTOBUFLEN) = '\0';
    }
    gtk_widget_set_name(myWidget, Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetSetSensitive                              */
/* Description: Enable/disable a widget                                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Sensitive flag                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetSetSensitive(const char * Name,
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

    gtk_widget_set_sensitive(myWidget, flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetConnectSignal                             */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "destroy") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "destroy",
                             G_CALLBACK(signal_func_1), "signal_destroy");
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


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetGetTopLevel                               */
/* Description: Get the top level parent window of a widget                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWidgetGetTopLevel(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkWidget *parentWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        parentWidget = gtk_widget_get_toplevel(myWidget);
    }
    else {
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", parentWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}

