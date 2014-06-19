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

static void signal_func_0(GtkWidget *treeview,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeview), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static gboolean signal_func_0a(GtkWidget *treeview,
                               gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeview), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr tempobj = context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}

static gboolean signal_func_1(GtkWidget *treeview,
                              gboolean arg1,
                              gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeview), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = context->WholeNumberToObject((wholenumber_t)arg1);
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}

static gboolean signal_func_2(GtkWidget *treeview,
                              int arg1,
                              int arg2,
                              gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeview), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = context->WholeNumberToObject((wholenumber_t)arg1);
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg2);
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}

static void signal_func_2a(GtkWidget *treeview,
                           GtkTreePath *arg1,
                           GtkTreeViewColumn *arg2,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeview), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg1), "OORXOBJECT");
    context->ArrayPut(arr, tempobj, 1);
    tempobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg2), "OORXOBJECT");
    context->ArrayPut(arr, tempobj, 2);
    context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->DetachThread();
    return;
}

static gboolean signal_func_2b(GtkWidget *treeview,
                               GtkTreePath *arg1,
                               GtkTreeViewColumn *arg2,
                               gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeview), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg1), "OORXOBJECT");
    context->ArrayPut(arr, tempobj, 1);
    tempobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg2), "OORXOBJECT");
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}

static gboolean signal_func_3(GtkWidget *treeview,
                              gboolean arg1,
                              gboolean arg2,
                              gboolean arg3,
                              gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(treeview), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = context->WholeNumberToObject((wholenumber_t)arg1);
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg2);
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg3);
    context->ArrayPut(arr, tempobj, 3);
    tempobj = context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an tree view.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxTreeViewNew,            // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *treeview = gtk_tree_view_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(treeview));
    g_object_set_data(G_OBJECT(treeview), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  append_column
 *
 * Append a column to the tree view.
 *
 * @param rxobj   The column
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTreeViewAppendColumn,   // Object_method name
            RexxObjectPtr, rxobj,      // Column object
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxobj, "GtkTreeViewColumn")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkTreeViewColumn"));
        return 0;
    }
    GtkTreeViewColumn *colWidget = (GtkTreeViewColumn *)context->ObjectToCSelf(rxobj);

    gtk_tree_view_append_column(GTK_TREE_VIEW(self), colWidget);

    return 0;
}

/**
 * Method:  remove_column
 *
 * Remove a column from the tree view.
 *
 * @param rxobj   The column
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTreeViewRemoveColumn,   // Object_method name
            RexxObjectPtr, rxobj,      // Column object
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxobj, "GtkTreeViewColumn")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkTreeViewColumn"));
        return 0;
    }
    GtkTreeViewColumn *colWidget = (GtkTreeViewColumn *)context->ObjectToCSelf(rxobj);

    gtk_tree_view_remove_column(GTK_TREE_VIEW(self), colWidget);

    return 0;
}

/**
 * Method:  insert_column
 *
 * Insert a column to the tree view.
 *
 * @param rxobj   The column
 *
 * @param pos     The position
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxTreeViewInsertColumn,   // Object_method name
            RexxObjectPtr, rxobj,      // Column object
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxobj, "GtkTreeViewColumn")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkTreeViewColumn"));
        return 0;
    }
    GtkTreeViewColumn *colWidget = (GtkTreeViewColumn *)context->ObjectToCSelf(rxobj);

    gtk_tree_view_insert_column(GTK_TREE_VIEW(self), colWidget, pos);

    return 0;
}

/**
 * Method:  set/get_model
 *
 * Set/get the model for the tree.
 *
 * @param model   The model object
 *
 * @return        Zero.
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxTreeViewGetSetModel,    // Object_method name
            OSELF, oself,              // GTK self
            OPTIONAL_RexxObjectPtr, model, // Column object
            CSELF, cself)              // GTK self
{
    if (argumentExists(2)) {
        // Do not test model for its class as it can be of multiple classes
        GtkTreeModel *modelWidget = (GtkTreeModel *)context->ObjectToCSelf(model);
        gtk_tree_view_set_model(GTK_TREE_VIEW(cself), modelWidget);
    }
    else {
        GtkTreeModel *model = (GtkTreeModel *)gtk_tree_view_get_model(GTK_TREE_VIEW(cself));
        // Create the Rexx object
        return context->SendMessage1(oself, "create_tree_model",
                                     context->NewPointer((RexxPointerObject)model));
    }
    return (RexxObjectPtr)context->Nil();
}

/**
 * Method:  get_selection
 *
 * Get the selection for the tree.
 *
 * @return        Treeselectionh object
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxTreeViewGetSelection,   // Object_method name
            CSELF, cself,              // GTK self
            OSELF, oself)              // GTK self
{
    GtkTreeSelection *sel = (GtkTreeSelection *)gtk_tree_view_get_selection(GTK_TREE_VIEW(cself));

    // Create the Rexx object
    return context->SendMessage1(oself, "create_tree_selection",
                                 context->NewPointer((RexxPointerObject)sel));
}

/**
 * Method:  expand_all
 *
 * Expand all levels of the tree.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxTreeViewExpandAll,      // Object_method name
            CSELF, self)               // GTK self
{
    gtk_tree_view_expand_all(GTK_TREE_VIEW(self));

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
            GrxTreeViewSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "columns_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_columns_changed";
        g_signal_connect(G_OBJECT(self), "columns-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "cursor_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_cursor_changed";
        g_signal_connect(G_OBJECT(self), "cursor-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "expand_collapse_cursor_row") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_expand_collapse_cursor_row";
        g_signal_connect(G_OBJECT(self), "expand-collapse-cursor-row",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_cursor";
        g_signal_connect(G_OBJECT(self), "move-cursor",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "row_activated") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_row_activated";
        g_signal_connect(G_OBJECT(self), "row-activated",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    else if (strcmp(name, "row_collapsed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_row_collapsed";
        g_signal_connect(G_OBJECT(self), "row-collapsed",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    else if (strcmp(name, "row_expanded") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_row_expanded";
        g_signal_connect(G_OBJECT(self), "row-expanded",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    else if (strcmp(name, "select_all") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_select_all";
        g_signal_connect(G_OBJECT(self), "select-all",
                         G_CALLBACK(signal_func_0a), cblock);
        return context->True();
    }
    else if (strcmp(name, "select_cursor_parent") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_select_cursor_parent";
        g_signal_connect(G_OBJECT(self), "select-cursor-parent",
                         G_CALLBACK(signal_func_0a), cblock);
        return context->True();
    }
    else if (strcmp(name, "select_cursor_row") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_select_cursor_row";
        g_signal_connect(G_OBJECT(self), "select-cursor-row",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "set_scroll_adjustments") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_scroll_adjustments";
        g_signal_connect(G_OBJECT(self), "set-scroll-adjustments",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    else if (strcmp(name, "start_interactive_search") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_start_interactive_search";
        g_signal_connect(G_OBJECT(self), "start-interactive-search",
                         G_CALLBACK(signal_func_0a), cblock);
        return context->True();
    }
    else if (strcmp(name, "test_collapse_row") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_test_collapse_row";
        g_signal_connect(G_OBJECT(self), "test-collapse-row",
                         G_CALLBACK(signal_func_2b), cblock);
        return context->True();
    }
    else if (strcmp(name, "test_expand_row") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_test_expand_row";
        g_signal_connect(G_OBJECT(self), "test-expand-row",
                         G_CALLBACK(signal_func_2b), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_cursor_row") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_cursor_row";
        g_signal_connect(G_OBJECT(self), "toggle-cursor-row",
                         G_CALLBACK(signal_func_0a), cblock);
        return context->True();
    }
    else if (strcmp(name, "unselect_all") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_unselect_all";
        g_signal_connect(G_OBJECT(self), "unselect-all",
                         G_CALLBACK(signal_func_0a), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

