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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an assistant.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxAssistantNew,           // Object_method name
            OSELF, self)               // Self
{
    GtkWidget       *myWidget;

    myWidget = gtk_assistant_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_current_page
 *
 * Set/get the assistant current page number.
 *
 * @param pagenum The page number
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAssistantGetSetCurrentPage, // Object_method name
            OPTIONAL_int, pagenum,     // Page number
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_assistant_set_current_page(GTK_ASSISTANT(self), pagenum - 1);
    }
    else {
        return gtk_assistant_get_current_page(GTK_ASSISTANT(self)) + 1;
    }

    return 0;
}

/**
 * Method:  get_nth_page
 *
 * Get the assistant nth page.
 *
 * @param pagenum The page number
 *
 * @return        Page
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxAssistantGetNthPage,    // Object_method name
            int, pagenum,              // Page number
            CSELF, self)               // GTK self
{
    GtkWidget *pageWidget;

    pageWidget = gtk_assistant_get_nth_page(GTK_ASSISTANT(self), pagenum - 1);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(pageWidget), "OORXOBJECT");
}

/**
 * Method:  prepend_page
 *
 * Prepend a page to the assistant.
 *
 * @param page    The page
 *
 * @return        Page number
 **/
RexxMethod2(int,                       // Return type
            GrxAssistantPrependPage,   // Object_method name
            RexxObjectPtr, page,       // Page
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);

    return gtk_assistant_prepend_page(GTK_ASSISTANT(self), pageWidget);
}

/**
 * Method:  append_page
 *
 * Append a page to the assistant.
 *
 * @param page    The page
 *
 * @return        Page number
 **/
RexxMethod2(int,                       // Return type
            GrxAssistantAppendPage,    // Object_method name
            RexxObjectPtr, page,       // Page
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);

    return gtk_assistant_append_page(GTK_ASSISTANT(self), pageWidget);
}

/**
 * Method:  insert_page
 *
 * Insert a page to the assistant.
 *
 * @param page    The page
 *
 * @param pagenum The page number
 *
 * @return        Page number
 **/
RexxMethod3(int,                       // Return type
            GrxAssistantInsertPage,    // Object_method name
            RexxObjectPtr, page,       // Page
            int, pagenum,              // Page number
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);

    return gtk_assistant_insert_page(GTK_ASSISTANT(self), pageWidget, pagenum - 1);
}

/**
 * Method:  set_page_type
 *
 * Set the assistant page type.
 *
 * @param page    The page
 *
 * @param pagenum The page number
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxAssistantSetPageType,   // Object_method name
            RexxObjectPtr, page,       // Page
            int, type,                 // Page type
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);

    gtk_assistant_set_page_type(GTK_ASSISTANT(self), pageWidget, (GtkAssistantPageType)type);

    return 0;
}

/**
 * Method:  set_page_title
 *
 * Set the assistant page title.
 *
 * @param page    The page
 *
 * @param title   The page title
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxAssistantSetPageTitle,  // Object_method name
            RexxObjectPtr, page,       // Page
            CSTRING, title,            // Page title
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);

    gtk_assistant_set_page_title(GTK_ASSISTANT(self), pageWidget, title);

    return 0;
}

/**
 * Method:  set_page_complete
 *
 * Set the assistant page complete.
 *
 * @param page    The page
 *
 * @param flag    The page complete flag
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxAssistantSetPageComplete, // Object_method name
            RexxObjectPtr, page,       // Page
            logical_t, flag,           // Page complete boolean
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);

    gtk_assistant_set_page_complete(GTK_ASSISTANT(self), pageWidget, flag);

    return 0;
}

/**
 * Method:  set_page_header_image
 *
 * Set the assistant page header image.
 *
 * @param page    The page
 *
 * @param filename The image file name
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxAssistantSetPageHeaderImage, // Object_method name
            RexxObjectPtr, page,       // Page
            CSTRING, filename,         // Image file name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);
    GdkPixbuf *image = gdk_pixbuf_new_from_file(filename, NULL);

    gtk_assistant_set_page_header_image(GTK_ASSISTANT(self), pageWidget, image);

    return 0;
}

/**
 * Method:  set_page_side_image
 *
 * Set the assistant page side image.
 *
 * @param page    The page
 *
 * @param filename The image file name
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxAssistantSetPageSideImage, // Object_method name
            RexxObjectPtr, page,       // Page
            CSTRING, title,            // Page title
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(page, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *pageWidget = (GtkWidget *)context->ObjectToCSelf(page);

    gtk_assistant_set_page_title(GTK_ASSISTANT(self), pageWidget, title);

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
            GrxAssistantSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "apply") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_apply";
        g_signal_connect(G_OBJECT(self), "apply",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "cancel") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_cancel";
        g_signal_connect(G_OBJECT(self), "cancel",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "close") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_close";
        g_signal_connect(G_OBJECT(self), "close",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "prepare") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_prepare";
        g_signal_connect(G_OBJECT(self), "prepare",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

