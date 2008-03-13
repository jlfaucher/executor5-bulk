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
/* Rexx External Function: GrxTextViewNew                                     */
/* Description: Create a new text view window                                 */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewNew(const char * Name,
                               const size_t Argc, const RXSTRING Argv[],
                               const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget       *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_text_view_new();

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetBuffer                               */
/* Description: Get the text buffer                                           */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetBuffer(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView     *myWidget;
    GtkTextBuffer   *myBuffer;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    myBuffer = gtk_text_view_get_buffer(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myBuffer);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetWrapMode                             */
/* Description: Set the wrap mode                                             */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Wrap mode                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetWrapMode(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView     *myWidget;
    GtkWrapMode     wrap;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &wrap);

    gtk_text_view_set_wrap_mode(myWidget, wrap);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetWrapMode                             */
/* Description: Get the wrap mode                                             */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetWrapMode(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView     *myWidget;
    GtkWrapMode     wrap;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    wrap = gtk_text_view_get_wrap_mode(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", wrap);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetJustification                        */
/* Description: Set the justification                                         */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Justification mode                                            */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetJustification(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    GtkJustification just;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &just);

    gtk_text_view_set_justification(myWidget, just);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetJustification                        */
/* Description: Get the justification                                         */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetJustification(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    GtkJustification just;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    just = gtk_text_view_get_justification(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", just);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetEditable                             */
/* Description: Set the editable flag                                         */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Editable flag                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetEditable(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gboolean         flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    gtk_text_view_set_editable(myWidget, flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetEditable                             */
/* Description: Get the editable flag                                         */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetEditabel(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gboolean         flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    flag = gtk_text_view_get_editable(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", flag);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetCursorVisible                        */
/* Description: Set the cursor visible flag                                   */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Cursor visible flaf                                           */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetCursorVisible(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gboolean         flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    gtk_text_view_set_cursor_visible(myWidget, flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetCursorVosible                        */
/* Description: Get the cursor visible flag                                   */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetCursorVisible(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gboolean         flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    flag = gtk_text_view_get_cursor_visible(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", flag);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetPixelsAboveLines                     */
/* Description: Set number of pixels above lines                              */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Number of pixels                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetPixelsAboveLines(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &pixels);

    gtk_text_view_set_pixels_above_lines(myWidget, pixels);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetPixelsAboveLines                     */
/* Description: Get the number of pixels above lines                          */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetPixelsAboveLines(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    pixels = gtk_text_view_get_pixels_above_lines(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pixels);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetPixelsBelowLines                     */
/* Description: Set number of pixels below lines                              */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Number of pixels                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetPixelsBelowLines(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &pixels);

    gtk_text_view_set_pixels_below_lines(myWidget, pixels);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetPixelsBelowLines                     */
/* Description: Get the number of pixels above lines                          */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetPixelsBelowLines(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    pixels = gtk_text_view_get_pixels_below_lines(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pixels);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetPixelsInsideWrAp                     */
/* Description: Set number of pixels inside wrap                              */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Number of pixels                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetPixelsInsideWrap(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &pixels);

    gtk_text_view_set_pixels_inside_wrap(myWidget, pixels);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetPixelsInsideWrap                     */
/* Description: Get the number of pixels above lines                          */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetPixelsInsideWrap(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    pixels = gtk_text_view_get_pixels_inside_wrap(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pixels);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetLeftMargin                           */
/* Description: Set number of pixels for the left margin                      */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Number of pixels                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetLeftMargin(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &pixels);

    gtk_text_view_set_left_margin(myWidget, pixels);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetLeftMargin                           */
/* Description: Get the number of pixels for the left margin                  */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetLeftMargin(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    pixels = gtk_text_view_get_left_margin(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pixels);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewSetRightMargin                          */
/* Description: Set number of pixels for the right margin                     */
/* Rexx Args:   Pointer to the text view widget                               */
/*          :   Number of pixels                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewSetRightMargin(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &pixels);

    gtk_text_view_set_right_margin(myWidget, pixels);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewGetRightMargin                          */
/* Description: Get the number of pixels for the right margin                 */
/* Rexx Args:   Pointer to the text view widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewGetRightMargin(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkTextView      *myWidget;
    gint             pixels;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    pixels = gtk_text_view_get_right_margin(myWidget);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", pixels);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTextViewConnectSignal                           */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTextViewConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "backspace") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "backspace",
                             G_CALLBACK(signal_func_1), "signal_backspace");
        }
        else if (strcmp(Argv[1].strptr, "copy_clipboard") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "copy-clipboard",
                             G_CALLBACK(signal_func_1), "signal_copy_clipboard");
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

