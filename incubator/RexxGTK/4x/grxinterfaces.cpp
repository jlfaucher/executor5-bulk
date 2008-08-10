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
/* Public Methods                                                             */
/*============================================================================*/

/**
 * Method:  set_current_folder
 *
 * Set the current folder.
 *
 * @param pagenum The page number
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxFileChooserSetCurrentFolder, // Object_method name
            CSTRING, dir,              // Directory
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkFileChooser *myWidget = (GtkFileChooser *)context->PointerValue(rxptr);

    gtk_file_chooser_set_current_folder(myWidget, dir);

    return 0;
}

/**
 * Method:  set_select_multiple
 *
 * Set the multiple selection flag.
 *
 * @param flag    The boolean flag
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxFileChooserSetSelectMultiple, // Object_method name
            logical_t, flag,           // Flag boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkFileChooser *myWidget = (GtkFileChooser *)context->PointerValue(rxptr);

    gtk_file_chooser_set_select_multiple(myWidget, flag);

    return 0;
}

/**
 * Method:  add_filter
 *
 * Add a file filter.
 *
 * @param rxfilter The filter object
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxFileChooserAddFilter,   // Object_method name
            RexxObjectPtr, rxfilter,   // Filter object
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkFileChooser *myWidget = (GtkFileChooser *)context->PointerValue(rxptr);
    RexxPointerObject filterptr = (RexxPointerObject)context->SendMessage0(rxfilter, "POINTER");
    GtkFileFilter *filter = (GtkFileFilter *)context->PointerValue(filterptr);

    gtk_file_chooser_add_filter(myWidget, filter); 

    return 0;
}

/**
 * Method:  get_filename
 *
 * Get the filename.
 *
 * @return        Filename
 **/
RexxMethod1(CSTRING,                   // Return type
            GrxFileChooserGetFilename, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkFileChooser *myWidget = (GtkFileChooser *)context->PointerValue(rxptr);

    return gtk_file_chooser_get_filename(myWidget);
}

/**
 * Method:  get_filenames
 *
 * Get the filenames.
 *
 * @return        Array of filenames
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxFileChooserGetFilenames, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkFileChooser *myWidget = (GtkFileChooser *)context->PointerValue(rxptr);
    GSList * list;
    RexxArrayObject arr;

    list = gtk_file_chooser_get_filenames(myWidget);
    while (list != NULL) {
        RexxObjectPtr rxnewptr = context->NewStringFromAsciiz((char *)list->data);
        context->ArrayPut(arr, rxnewptr, 1);
        list = list->next;
    }

    return (RexxObjectPtr)arr;
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
            GrxFileChooserSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "current_folder_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_current_folder_changed";
        g_signal_connect(G_OBJECT(myWidget), "current-folder-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "file_activated") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_file_activated";
        g_signal_connect(G_OBJECT(myWidget), "file-activated",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_changed";
        g_signal_connect(G_OBJECT(myWidget), "selection-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "update_preview") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_update_preview";
        g_signal_connect(G_OBJECT(myWidget), "update-preview",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->False();
}

/**
 * Method:  init
 *
 * Create a new file filter.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxFileFilterNew,          // Object_method name
            OSELF, self)               // Self
{
    GtkFileFilter *filter = gtk_file_filter_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(filter));
    context->SendMessage1(self, "POINTER=", context->NewPointer(filter));
    g_object_set_data(G_OBJECT(filter), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_name
 *
 * Set the name of the filter.
 *
 * @param name    The filter name
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxFileFilterSetName,      // Object_method name
            CSTRING, name,             // Filter name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkFileFilter *filter = (GtkFileFilter *)context->PointerValue(rxptr);

    gtk_file_filter_set_name(filter, name);

    return 0;
}

/**
 * Method:  add_pattern
 *
 * Add a pattern to the filter.
 *
 * @param pattern The pattern
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxFileFilterAddPattern,   // Object_method name
            CSTRING, pattern,          // Filter pattern
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkFileFilter *filter = (GtkFileFilter *)context->PointerValue(rxptr);

    gtk_file_filter_add_pattern(filter, pattern);

    return 0;
}

