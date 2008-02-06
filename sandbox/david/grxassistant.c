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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantNew                                    */
/* Description: Create a new assistant window                                 */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantNew(const char * Name,
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget       *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_assistant_new();

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantSetCurrentPage                         */
/* Description: Set the current page                                          */
/* Rexx Args:   Pointer to the widget                                         */
/*              Page number (1-based)                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantSetCurrentPage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint pagenum;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &pagenum);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_assistant_set_current_page(GTK_ASSISTANT(myWidget), pagenum - 1);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantGetCurrentPage                         */
/* Description: Get the current page                                          */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantGetCurrentPage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint pagenum;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        pagenum = gtk_assistant_get_current_page(GTK_ASSISTANT(myWidget));
    }

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pagenum + 1);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantGetNthPage                             */
/* Description: Get the nth page                                              */
/* Rexx Args:   Pointer to the widget                                         */
/*              Page number                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantGetNthPage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *pageWidget;
    gint pagenum;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &pagenum);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        pageWidget = gtk_assistant_get_nth_page(GTK_ASSISTANT(myWidget), pagenum - 1);
    }

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", pageWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantPrependPage                            */
/* Description: Prepend a page to the assistant                               */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantPrependPage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;
    gint pagenum = 0;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        pagenum = gtk_assistant_prepend_page(GTK_ASSISTANT(myWidget), page);
    }

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pagenum + 1);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantAppendPage                             */
/* Description: Append a page to the assistant                                */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantAppendPage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;
    gint pagenum = 0;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        pagenum = gtk_assistant_append_page(GTK_ASSISTANT(myWidget), page);
    }

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pagenum + 1);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantInsertPage                             */
/* Description: Insert a page to the assistant                                */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*              Page number (1-based)                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantInsertPage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;
    gint pagenum, newpagenum;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);
    sscanf(Argv[2].strptr, "%d", &pagenum);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        newpagenum = gtk_assistant_insert_page(GTK_ASSISTANT(myWidget), page, pagenum - 1);
    }

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", newpagenum + 1);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantSetPageType                            */
/* Description: Set the page type                                             */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*              Page number (1-based)                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantSetPageType(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;
    GtkAssistantPageType type;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);
    if(strcmp(Argv[2].strptr,"GTK_ASSISTANT_PAGE_CONTENT") == 0)
        type = GTK_ASSISTANT_PAGE_CONTENT;
    else if(strcmp(Argv[2].strptr,"GTK_ASSISTANT_PAGE_INTRO") == 0)
        type = GTK_ASSISTANT_PAGE_INTRO;
    else if(strcmp(Argv[2].strptr,"GTK_ASSISTANT_PAGE_CONFIRM") == 0)
        type = GTK_ASSISTANT_PAGE_CONFIRM;
    else if(strcmp(Argv[2].strptr,"GTK_ASSISTANT_PAGE_SUMMARY") == 0)
        type = GTK_ASSISTANT_PAGE_SUMMARY;
    else if(strcmp(Argv[2].strptr,"GTK_ASSISTANT_PAGE_PROGRESS") == 0)
        type = GTK_ASSISTANT_PAGE_PROGRESS;
    else 
        sscanf(Argv[2].strptr, "%d", &type);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_assistant_set_page_type(GTK_ASSISTANT(myWidget), page, type);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantSetPageTitle                           */
/* Description: Set the page title                                            */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*              Title                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantSetPageTitle(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, 2, Argv)) { // don't check arg 3
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_assistant_set_page_title(GTK_ASSISTANT(myWidget), page,
                                     Argv[2].strptr);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantSetPageComplete                        */
/* Description: Set the page complete flag                                    */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*              Flag                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantSetPageComplete(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);
    sscanf(Argv[2].strptr, "%d", &flag);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_assistant_set_page_complete(GTK_ASSISTANT(myWidget), page, flag);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantSetPageHeaderImage                     */
/* Description: Set the page header imageg                                    */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*              Image file                                                    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantSetPageHeaderImage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;
    GdkPixbuf *image;
    GError *error = NULL;              // we will just ignore this error

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);
    image = gdk_pixbuf_new_from_file(Argv[2].strptr, &error);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_assistant_set_page_header_image(GTK_ASSISTANT(myWidget), page, image);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantSetPageSideImage                       */
/* Description: Set the page side imageg                                      */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the page                                           */
/*              Image file                                                    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantSetPageSideImage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *page;
    GdkPixbuf *image;
    GError *error = NULL;              // we will just ignore this error

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &page);
    image = gdk_pixbuf_new_from_file(Argv[2].strptr, &error);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_assistant_set_page_side_image(GTK_ASSISTANT(myWidget), page, image);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAssistantConnectSignal                          */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAssistantConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "apply") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "apply",
                             G_CALLBACK(signal_func_1), "signal_apply");
        }
        else if (strcmp(Argv[1].strptr, "cancel") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "cancel",
                             G_CALLBACK(signal_func_1), "signal_cancel");
        }
        else if (strcmp(Argv[1].strptr, "close") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "close",
                             G_CALLBACK(signal_func_1), "signal_close");
        }
        else if (strcmp(Argv[1].strptr, "prepare") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "prepareset-focus",
                             G_CALLBACK(signal_func_2), "signal_prepare");
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

