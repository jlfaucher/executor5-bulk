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
/* Rexx External Function: GrxTextBufferNew                                   */
/* Description: Create a new text buffer                                      */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferNew(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;
    GtkTextTagTable     *myTable;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myTable);

    myBuffer = gtk_text_buffer_new(myTable);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myBuffer);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferSetText                               */
/* Description: Set the text in the buffer                                    */
/* Rexx Args:   Pointer to the buffer                                         */
/*              Text                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferSetText(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myBuffer);

    gtk_text_buffer_set_text(myBuffer, Argv[1].strptr, -1);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferInsertText                            */
/* Description: Insert text at the current mark                               */
/* Rexx Args:   Pointer to the buffer                                         */
/*              Text                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferInsertText(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;
    GtkTextMark         *mark;
    GtkTextIter         iter;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myBuffer);

    mark = gtk_text_buffer_get_insert(myBuffer);
    gtk_text_buffer_get_iter_at_mark(myBuffer, &iter, mark);
    gtk_text_buffer_insert(myBuffer, &iter, Argv[1].strptr, -1);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferGetText                               */
/* Description: Get text at the current mark                                  */
/* Rexx Args:   Pointer to the buffer                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferGetText(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;
    GtkTextIter         start, end;
    gchar               *text;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myBuffer);

    gtk_text_buffer_get_selection_bounds(myBuffer, &start, &end);
    text = gtk_text_buffer_get_text(myBuffer, &start, &end, FALSE);

    /* Set up the REXX return code */
    if (strlen(text) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(text) + 1);
    }
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", text);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferCutClipboard                          */
/* Description: Cut text to the clipboard                                     */
/* Rexx Args:   Pointer to the buffer                                         */
/*              Clipboard                                                     */
/*              Editable flag                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferCutClipboard(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;
    GtkClipboard        *cb;
    gboolean            flag; 

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myBuffer);
    sscanf(Argv[1].strptr, "%p", &cb);
    sscanf(Argv[2].strptr, "%p", &flag);

    gtk_text_buffer_cut_clipboard(myBuffer, cb, flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferCopyClipboard                         */
/* Description: Copy text to the clipboard                                    */
/* Rexx Args:   Pointer to the buffer                                         */
/*              Clipboard                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferCopyClipboard(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;
    GtkClipboard        *cb;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myBuffer);
    sscanf(Argv[1].strptr, "%p", &cb);

    gtk_text_buffer_copy_clipboard(myBuffer, cb);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferPasteClipboard                        */
/* Description: Paste text from the clipboard                                 */
/* Rexx Args:   Pointer to the buffer                                         */
/*              Clipboard                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferPasteClipboard(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;
    GtkClipboard        *cb;
    gboolean            flag; 

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myBuffer);
    sscanf(Argv[1].strptr, "%p", &cb);
    sscanf(Argv[2].strptr, "%p", &flag);

    gtk_text_buffer_paste_clipboard(myBuffer, cb, NULL, flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferForwardSearch                         */
/* Description: Search the buffer                                             */
/* Rexx Args:   Pointer to the buffer                                         */
/*              Search Text                                                   */
/*              Start search flag                                             */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferForwardSearch(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer       *myBuffer;
    gboolean            flag, found;
    GtkTextIter         start, begin, end, *retiter;
    GtkTextMark         *mark;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%p", &myBuffer);
    sscanf(Argv[2].strptr, "%p", &flag);

    if (flag) {
        /* start search from the start of the buffer */
        gtk_text_buffer_get_start_iter(myBuffer, &start);
    }
    else {
        /* start search from the current position */
        mark = gtk_text_buffer_get_insert(myBuffer);
        gtk_text_buffer_get_iter_at_mark(myBuffer, &start, mark);
    }
    found = gtk_text_iter_forward_search(&start, Argv[1].strptr,
                                         GTK_TEXT_SEARCH_TEXT_ONLY , &begin,
                                         &end, NULL);

    /* Set up the REXX return code */
    if (found) {
        retiter = gtk_text_iter_copy(&begin);
        g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", retiter);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferForwardSearchNext                     */
/* Description: Search the buffer                                             */
/* Rexx Args:   Search Text                                                   */
/*              Pointer to Iter                                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferForwardSearchNext(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    gboolean            found;
    GtkTextIter         *start, begin, end, *retiter;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[1].strptr, "%p", &start);

    gtk_text_iter_forward_char(start);
    found = gtk_text_iter_forward_search(start, Argv[0].strptr,
                                         GTK_TEXT_SEARCH_TEXT_ONLY , &begin,
                                         &end, NULL);
    gtk_text_iter_free(start);

    /* Set up the REXX return code */
    if (found) {
        retiter = gtk_text_iter_copy(&begin);
        g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", retiter);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferCreateTag                             */
/* Description: Create a tag for the buffer                                   */
/* Rexx Args:   Pointer to the text buffer                                    */
/*          :   Tag name                                                      */
/*          :   Property name                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferCreateTag(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer *myWidget;     
    gchar         *prop;

    /* Check for valid arguments */
    if (Argc < 3) {
        return RXFUNC_BADCALL;
    }
    if (GrxCheckArgs(3, 3, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_text_buffer_create_tag(myWidget, Argv[1].strptr, Argv[2].strptr, NULL);
    for (size_t i = 2; i < Argc; i++) {
        g_object_set(G_OBJECT(myWidget), Argv[1].strptr, Argv[i].strptr, NULL
    }


    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(myWidget),
                                   hpolicy, vpolicy);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextBufferonnectSignal                          */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextBufferConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkTextBuffer *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "apply_tag") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "apply-tag",
                             G_CALLBACK(signal_func_1), "signal_apply_tag");
        }
        else if (strcmp(Argv[1].strptr, "begin_user_action") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "begin-user-action",
                             G_CALLBACK(signal_func_1), "signal_begin_user_action");
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

