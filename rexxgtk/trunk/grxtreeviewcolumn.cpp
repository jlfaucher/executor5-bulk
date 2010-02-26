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

static void signal_func_0(GtkWidget *treeviewcol,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeviewcol), "OORXOBJECT");
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
 * Create an tree view column.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxTreeViewColumnNew,      // Object_method name
            OSELF, self)               // Self
{
    GtkTreeViewColumn *treeviewcol = gtk_tree_view_column_new();

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(treeviewcol));
    g_object_set_data(G_OBJECT(treeviewcol), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_title
 *
 * Set/get the title of a tree view column.
 *
 * @param title   The title
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxTreeViewColumnGetSetTitle, // Object_method name
            OPTIONAL_CSTRING, title,   // Title
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_tree_view_column_set_title(GTK_TREE_VIEW_COLUMN(self), title);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_tree_view_column_get_title(GTK_TREE_VIEW_COLUMN(self)));
    }

    return retc;
}

/**
 * Method:  pack_start
 *
 * Pack the tree view column.
 *
 * @param flag    The expand boolean
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxTreeViewColumnPackStart, // Object_method name
            RexxObjectPtr, rxobj,      // Renderer
            logical_t, flag,           // Expand boolean
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxobj, "GtkCellRenderer")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkCellRenderer"));
        return 0;
    }
    GtkCellRenderer *renWidget = (GtkCellRenderer *)context->ObjectToCSelf(rxobj);

    gtk_tree_view_column_pack_start(GTK_TREE_VIEW_COLUMN(self), renWidget, flag);

    return 0;
}

/**
 * Method:  pack_end
 *
 * Pack the tree view column.
 *
 * @param flag    The expand boolean
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxTreeViewColumnPackEnd, // Object_method name
            RexxObjectPtr, rxobj,      // Renderer
            logical_t, flag,           // Expand boolean
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxobj, "GtkCellRenderer")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkCellRenderer"));
        return 0;
    }
    GtkCellRenderer *renWidget = (GtkCellRenderer *)context->ObjectToCSelf(rxobj);

    gtk_tree_view_column_pack_end(GTK_TREE_VIEW_COLUMN(self), renWidget, flag);

    return 0;
}

/**
 * Method:  set_attribute
 *
 * Set an attribute of a tree view column.
 *
 * @param flag    The expand boolean
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxTreeViewColumnSetAttribute, // Object_method name
            RexxObjectPtr, rxobj,      // Renderer
            CSTRING, attr,             // Attribute
            int, col,                  // Column
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxobj, "GtkCellRenderer")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkCellRenderer"));
        return 0;
    }
    GtkCellRenderer *renWidget = (GtkCellRenderer *)context->ObjectToCSelf(rxobj);

    gtk_tree_view_column_set_attributes(GTK_TREE_VIEW_COLUMN(self), renWidget,
                                        attr, col, NULL);

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
RexxMethod2(RexxObjectPtr,             // Return type
            GrxTreeViewColumnSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "columns_clicked") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_clicked";
        g_signal_connect(G_OBJECT(self), "clicked",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->False();
}

