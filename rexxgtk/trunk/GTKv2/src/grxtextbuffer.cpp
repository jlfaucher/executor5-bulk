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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an textbuffer.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextBufferNew,          // Object_method name
            OPTIONAL_POINTER, rxptr,   // buffer pointer
            OSELF, self)               // Self
{
    GtkTextBuffer       *myBuffer;

    if (rxptr == NULL) {
        myBuffer = gtk_text_buffer_new(NULL);
    }
    else {
        myBuffer = (GtkTextBuffer *)rxptr;
    }

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myBuffer));
    g_object_set_data(G_OBJECT(myBuffer), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_text
 *
 * Set/get the textbuffer text.
 *
 * @param text    The text
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxTextBufferGetSetText,   // Object_method name
            OPTIONAL_CSTRING, text,    // Tagtable object
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(self), text, -1);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        GtkTextIter         start, end;

        gtk_text_buffer_get_selection_bounds(GTK_TEXT_BUFFER(self), &start, &end);
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_text_buffer_get_text(GTK_TEXT_BUFFER(self), &start, &end, FALSE));
    }

    return retc;
}

/**
 * Method:  insert_text
 *
 * Insert the text in the textbuffer.
 *
 * @param text    The text
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextBufferInsertText,   // Object_method name
            CSTRING, text,             // Tagtable object
            CSELF, self)               // GTK self
{
    GtkTextMark         *mark;
    GtkTextIter         iter;

    mark = gtk_text_buffer_get_insert(GTK_TEXT_BUFFER(self));
    gtk_text_buffer_get_iter_at_mark(GTK_TEXT_BUFFER(self), &iter, mark);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(self), &iter, text, -1);

    return 0;
}

/**
 * Method:  cut_clipboard
 *
 * Cut the marked text to the clipboard.
 *
 * @param flag    The editable flag
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextBufferCutClipboard, // Object_method name
            logical_t, flag,           // Editable flag
            CSELF, self)               // GTK self
{
    GtkClipboard *cb = gtk_clipboard_get(GDK_SELECTION_PRIMARY);

    gtk_text_buffer_cut_clipboard(GTK_TEXT_BUFFER(self), cb, flag);

    return 0;
}

/**
 * Method:  copy_clipboard
 *
 * Copy the marked text to the clipboard.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxTextBufferCopyClipboard, // Object_method name
            CSELF, self)               // GTK self
{
    GtkClipboard *cb = gtk_clipboard_get(GDK_SELECTION_PRIMARY);

    gtk_text_buffer_copy_clipboard(GTK_TEXT_BUFFER(self), cb);

    return 0;
}

/**
 * Method:  paste_clipboard
 *
 * Paste the text in the clipboard to the buffer.
 *
 * @param flag    The editable flag
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTextBufferPasteClipboard, // Object_method name
            logical_t, flag,           // Editable flag
            CSELF, self)               // GTK self
{
    GtkClipboard *cb = gtk_clipboard_get(GDK_SELECTION_PRIMARY);

    gtk_text_buffer_paste_clipboard(GTK_TEXT_BUFFER(self), cb, NULL, flag);

    return 0;
}

/**
 * Method:  forward_search
 *
 * Start a search in the forward direction.
 *
 * @param text    The search text
 *
 * @param text    The start position, 1 = start of buffer, 0 = current position
 *
 * @return        The iter object or Nil
 **/
RexxMethod3(POINTER,                   // Return type
            GrxTextBufferForwardSearch, // Object_method name
            CSTRING, text,             // Search text
            logical_t, flag,           // Position boolean
            CSELF, self)               // GTK self
{
    gboolean            found;
    GtkTextIter         start, begin, end;
    GtkTextMark         *mark;

    if (flag) {
        /* start search from the start of the buffer */
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(self), &start);
    }
    else {
        /* start search from the current position */
        mark = gtk_text_buffer_get_insert(GTK_TEXT_BUFFER(self));
        gtk_text_buffer_get_iter_at_mark(GTK_TEXT_BUFFER(self), &start, mark);
    }
    found = gtk_text_iter_forward_search(&start, text, GTK_TEXT_SEARCH_TEXT_ONLY,
                                         &begin, &end, NULL);
    if (found) {
        return gtk_text_iter_copy(&end);
    }
    return NULL;
}

/**
 * Method:  forward_search_next
 *
 * Continue a search in the forward direction.
 *
 * @param text    The search text
 *
 * @param text    The start position, 1 = start of buffer, 0 = current position
 *
 * @return        The iter object or Nil
 **/
RexxMethod2(POINTER,                   // Return type
            GrxTextBufferForwardSearchNext, // Object_method name
            CSTRING, text,             // Search text
            POINTER, rxiter)           // The iter returned from the last call
{
    GtkTextIter *start = (GtkTextIter *)rxiter;
    GtkTextIter         begin, end, *retiter;
    gboolean            found;


    gtk_text_iter_forward_char(start);
    found = gtk_text_iter_forward_search(start, text, GTK_TEXT_SEARCH_TEXT_ONLY,
                                         &begin, &end, NULL);
    gtk_text_iter_free(start);
    if (found) {
        return gtk_text_iter_copy(&end);
    }
    return NULL;
}

/**
 * Method:  create_tag_int
 *
 * Create an integer tag for the buffer.
 *
 * @param name    Tag name
 *
 * @param pname   Property name
 *
 * @param pvalue  Property value
 *
 * @return        Zero
 **/
RexxMethod4(int,                       // Return type
            GrxTextBufferCreateTagInt, // Object_method name
            CSTRING, name,             // Tag name
            CSTRING, pname,            // Property name
            int, pvalue,               // The property value
            CSELF, self)               // GTK self
{
    gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(self), name, pname, pvalue, NULL);

    return 0;
}

/**
 * Method:  create_tag_float
 *
 * Create a float tag for the buffer.
 *
 * @param name    Tag name
 *
 * @param pname   Property name
 *
 * @param pvalue  Property value
 *
 * @return        Zero
 **/
RexxMethod4(int,                       // Return type
            GrxTextBufferCreateTagFloat, // Object_method name
            CSTRING, name,             // Tag name
            CSTRING, pname,            // Property name
            float, pvalue,             // The property value
            CSELF, self)               // GTK self
{
    gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(self), name, pname, pvalue, NULL);

    return 0;
}

/**
 * Method:  apply_tag_by_name
 *
 * Apply a tag to the current selection.
 *
 * @param name    Tag name
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxTextBufferApplyTagByName, // Object_method name
            CSTRING, name,             // Tag name
            CSELF, self)               // GTK self
{
    GtkTextIter   start, end;

    gtk_text_buffer_get_selection_bounds(GTK_TEXT_BUFFER(self), &start, &end);
    gtk_text_buffer_apply_tag_by_name(GTK_TEXT_BUFFER(self), name, &start, &end);

    return 0;
}

/**
 * Method:  remove_all_tags
 *
 * Remove all tags.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxTextBufferRemoveAllTags, // Object_method name
            CSELF, self)               // GTK self
{
    GtkTextIter   start, end;

    gtk_text_buffer_get_selection_bounds(GTK_TEXT_BUFFER(self), &start, &end);
    gtk_text_buffer_remove_all_tags(GTK_TEXT_BUFFER(self), &start, &end);

    return 0;
}

/*
 * Method:  insert_image
 *
 * Insert an image into the buffer.
 *
 * @param image   The image
 *
 * @param linenum The line number
 *
 * @return        Zero
 */
RexxMethod3(int,                       // Return type
            GrxTextBufferInsertImage,  // Object_method name
            RexxObjectPtr, image,      // Image
            int, linenum,              // Line number
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(image, "GtkImage")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkImage"));
        return 0;
    }
    GtkImage *img = (GtkImage *)context->ObjectToCSelf(image);
    GtkTextIter   line;
    GdkPixbuf     *pixbuf;

    pixbuf = gtk_image_get_pixbuf(img);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(self), &line, linenum);
    gtk_text_buffer_insert_pixbuf(GTK_TEXT_BUFFER(self), &line, pixbuf);

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
            GrxTextBufferSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "begin_user_action") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_begin_user_action";
        g_signal_connect(G_OBJECT(self), "begin-user-action",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_changed";
        g_signal_connect(G_OBJECT(self), "changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "end_user_action") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_end_user_action";
        g_signal_connect(G_OBJECT(self), "end-user-action",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "modified_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_modified_changed";
        g_signal_connect(G_OBJECT(self), "modified-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

