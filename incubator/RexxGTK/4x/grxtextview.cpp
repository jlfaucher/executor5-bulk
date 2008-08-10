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

static void signal_func_0(GtkWidget *window,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static void signal_func_1(GtkWidget *window,
                          char *arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxarg1 = context->NewStringFromAsciiz(arg1);
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxarg1);
    context->DetachThread();
    return;
}

static void signal_func_1a(GtkWidget *window,
                          GtkWidget *widget,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxObjectPtr rxwidget = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxwidget);
    context->DetachThread();
    return;
}

static void signal_func_1b(GtkWidget *window,
                          bool arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxarg1 = context->NumberToObject((wholenumber_t)arg1);
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxarg1);
    context->DetachThread();
    return;
}

static void signal_func_2(GtkWidget *window,
                          int arg1,
                          int arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxarg1 = context->NumberToObject((wholenumber_t)arg1);
    RexxObjectPtr rxarg2 = context->NumberToObject((wholenumber_t)arg2);
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, rxarg1, rxarg2);
    context->DetachThread();
    return;
}
static void signal_func_2a(GtkWidget *window,
                          int arg1,
                          bool arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxarg1 = context->NumberToObject((wholenumber_t)arg1);
    RexxObjectPtr rxarg2 = context->NumberToObject((wholenumber_t)arg2);
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, rxarg1, rxarg2);
    context->DetachThread();
    return;
}

static void signal_func_3(GtkWidget *window,
                          int arg1,
                          int arg2,
                          bool arg3,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxArrayObject arr;

    cblock->instance->AttachThread(&context);
    arr = context->NewArray(1);
    RexxObjectPtr rxstep = context->NumberToObject((wholenumber_t)arg1);;
    context->ArrayPut(arr, rxstep, 1);
    RexxObjectPtr rxarg2 = context->NumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, rxarg2, 2);
    RexxObjectPtr rxarg3 = context->NumberToObject((wholenumber_t)arg3);;
    context->ArrayPut(arr, rxarg3, 3);
    context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an textview.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewNew,            // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_text_view_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myWidget));
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get_buffer
 *
 * Get the associated text buffer.
 *
 * @return        Text buffer object
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxTextViewGetBuffer,      // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);
    GtkTextBuffer   *myBuffer;

    myBuffer = gtk_text_view_get_buffer(myWidget);
    return context->SendMessage1(self, "create_textbuffer", context->NewPointer(myBuffer));
}

/**
 * Method:  set_wrap_mode
 *
 * Set the wrap mode.
 *
 * @param mode    The wrap mode
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetWrapMode,    // Object_method name
            int, mode,                 // Wrap mode
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_wrap_mode(myWidget, (GtkWrapMode)mode);

    return 0;
}

/**
 * Method:  get_wrap_mode
 *
 * Get the wrap mode.
 *
 * @return        Wrap mode
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewGetWrapMode,    // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return (int)gtk_text_view_get_wrap_mode(myWidget);
}

/**
 * Method:  set_justification
 *
 * Set the justification.
 *
 * @param just    The justification
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetJustification, // Object_method name
            int, just,                 // Justification
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_justification(myWidget, (GtkJustification)just);

    return 0;
}

/**
 * Method:  get_justification.
 *
 * Get the justification.
 *
 * @return        Wrap mode
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewGetJustification, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return (int)gtk_text_view_get_justification(myWidget);
}

/**
 * Method:  set_editable
 *
 * Set editable.
 *
 * @param flag    The editable boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetEditable,    // Object_method name
            logical_t, flag,           // Editable boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_editable(myWidget, flag);

    return 0;
}

/**
 * Method:  get_editable
 *
 * Get the editable boolean.
 *
 * @return        Editable boolean
 **/
RexxMethod1(logical_t,                 // Return type
            GrxTextViewGetEditable,    // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return gtk_text_view_get_editable(myWidget);
}

/**
 * Method:  set_cursor_visible
 *
 * Set the visibility of the cursor.
 *
 * @param flag    The cursor visible boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetCursorVisible, // Object_method name
            logical_t, flag,           // Cursor visible boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_cursor_visible(myWidget, flag);

    return 0;
}

/**
 * Method:  get_cursor_visible
 *
 * Get the cursor visibility boolean.
 *
 * @return        Visible boolean
 **/
RexxMethod1(logical_t,                 // Return type
            GrxTextViewGetCursorVisible, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return gtk_text_view_get_cursor_visible(myWidget);
}

/**
 * Method:  set_pixels_above_lines
 *
 * Set the number of pixels above lines.
 *
 * @param pixels  The number of pixels
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetPixelsAboveLines, // Object_method name
            int, pixels,               // Number of pixels
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_pixels_above_lines(myWidget, pixels);

    return 0;
}

/**
 * Method:  get_pixels_above_lines
 *
 * Get the number of pixels above lines.
 *
 * @return        Number of pixels
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewGetPixelsAboveLines, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return gtk_text_view_get_pixels_above_lines(myWidget);
}

/**
 * Method:  set_pixels_below_lines
 *
 * Set the number of pixels below lines.
 *
 * @param pixels  The number of pixels
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetPixelsBelowLines, // Object_method name
            int, pixels,               // Number of pixels
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_pixels_below_lines(myWidget, pixels);

    return 0;
}

/**
 * Method:  get_pixels_below_lines
 *
 * Get the number of pixels below lines.
 *
 * @return        Number of pixels
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewGetPixelsBelowLines, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return gtk_text_view_get_pixels_below_lines(myWidget);
}

/**
 * Method:  set_pixels_inside_wrap
 *
 * Set the number of pixels inside wrap.
 *
 * @param pixels  The number of pixels
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetPixelsInsideWrap, // Object_method name
            int, pixels,               // Number of pixels
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_pixels_inside_wrap(myWidget, pixels);

    return 0;
}

/**
 * Method:  get_pixels_inside_wrap
 *
 * Get the number of pixels inside wrap.
 *
 * @return        Number of pixels
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewGetPixelsInsideWrap, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return gtk_text_view_get_pixels_inside_wrap(myWidget);
}

/**
 * Method:  set_left_margin
 *
 * Set the number of pixels for the left margin.
 *
 * @param pixels  The number of pixels
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetLeftMargin,  // Object_method name
            int, pixels,               // Number of pixels
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_left_margin(myWidget, pixels);

    return 0;
}

/**
 * Method:  get_left_margin
 *
 * Get the number of pixels for the left margin.
 *
 * @return        Left margin
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewGetLeftMargin,  // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return gtk_text_view_get_left_margin(myWidget);
}

/**
 * Method:  set_right_margin
 *
 * Set the number of pixels for the right margin.
 *
 * @param pixels  The number of pixels
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextViewSetRightMargin, // Object_method name
            int, pixels,               // Number of pixels
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    gtk_text_view_set_right_margin(myWidget, pixels);

    return 0;
}

/**
 * Method:  get_right_margin
 *
 * Get the number of pixels for the right margin.
 *
 * @return        Right margin
 **/
RexxMethod1(int,                       // Return type
            GrxTextViewGetRightMargin, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);

    return gtk_text_view_get_right_margin(myWidget);
}

/**
 * Method:  add_child_at_offset
 *
 * Add a child widget at the specified offset.
 *
 * @param rxchild The child widget
 *
 * @param offset  The offset
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxTextViewAddChildAtOffset, // Object_method name
            RexxObjectPtr, rxchild,    // Child widget
            int, offset,               // Offset
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkTextView *myWidget = (GtkTextView *)context->PointerValue(rxptr);
    RexxPointerObject addptr = (RexxPointerObject)context->SendMessage0(rxchild, "POINTER");
    GtkWidget *myChild = (GtkWidget *)context->PointerValue(addptr);
    GtkTextBuffer    *myBuffer;
    GtkTextIter      iter;
    GtkTextChildAnchor *anchor;

    myBuffer = gtk_text_view_get_buffer(myWidget);
    gtk_text_buffer_get_iter_at_offset(myBuffer, &iter, offset);
    anchor = gtk_text_buffer_create_child_anchor(myBuffer, &iter);
    gtk_text_view_add_child_at_anchor(myWidget, myChild, anchor);

    return 0;
}


/**
 * Method:  signal_connect
 *
 * Connect a signal to an ooRexx method.
 *
 * @param name    The signal name
 *
 * @return        Zero
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxTextViewSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "backspace") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_backspace";
        g_signal_connect(G_OBJECT(myWidget), "backspace",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "copy_clipboard") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_copy_clipboard";
        g_signal_connect(G_OBJECT(myWidget), "copy-clipboard",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "cut_clipboard") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_cut_clipboard";
        g_signal_connect(G_OBJECT(myWidget), "cut-clipboard",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "delete_from_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_delete_from_cursor";
        g_signal_connect(G_OBJECT(myWidget), "delete-from-cursor",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "insert_at_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_insert_at_cursor";
        g_signal_connect(G_OBJECT(myWidget), "insert-at-cursor",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_cursor";
        g_signal_connect(G_OBJECT(myWidget), "move-cursor",
                         G_CALLBACK(signal_func_3), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_viewport") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_viewport";
        g_signal_connect(G_OBJECT(myWidget), "move-viewport",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "page_horizontally") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_page_horizontally";
        g_signal_connect(G_OBJECT(myWidget), "page-horizontally",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    else if (strcmp(name, "paste_clipboard") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_paste_clipboard";
        g_signal_connect(G_OBJECT(myWidget), "paste-clipboard",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "populate_popup") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_populate_popup";
        g_signal_connect(G_OBJECT(myWidget), "populate-popup",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "select_all") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_select_all";
        g_signal_connect(G_OBJECT(myWidget), "select-all",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "set_anchor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_anchor";
        g_signal_connect(G_OBJECT(myWidget), "set-anchor",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "set_scroll_adjustments") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_scroll_adjustments";
        g_signal_connect(G_OBJECT(myWidget), "set-scroll-adjustments",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_cursor_visible") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_cursor_visible";
        g_signal_connect(G_OBJECT(myWidget), "toggle_cursor_visible",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_overwrite") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_overwrite";
        g_signal_connect(G_OBJECT(myWidget), "toggle_overwrite",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
}

