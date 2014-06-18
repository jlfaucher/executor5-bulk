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

/**
 * Method:  init
 *
 * Create a tree selection.
 *
 * @param rxptr   The reference pointer
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTreeSelectionNewFromPtr, // Object_method name
            POINTER, rxptr,            // Widget pointer
            OSELF, self)               // Self
{
    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(rxptr));

    return 0;
}

/**
 * Method:  get_selected_rows
 *
 * Return an array of selected rows (tree row references).
 *
 * @return        An array
 **/
RexxMethod3(RexxArrayObject,           // Return type
            GrxTreeSelectionGetSelectedRows, // Object_method name
            RexxObjectPtr, treemodel,  // The tree model
            OSELF, oself,              // Self
            CSELF, cself)              // Self
{
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr ref;
    GList *list, *ptr;
    int i = 1;
    if (!context->IsOfType(treemodel, "GtkTreeModel")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkTreeModel"));
        return 0;
    }
    GtkTreeModel *model = (GtkTreeModel *)context->ObjectToCSelf(treemodel);

    list = gtk_tree_selection_get_selected_rows((GtkTreeSelection *)cself,
                                                &model);
    ptr = list;
    while (ptr != NULL) {
        GtkTreeRowReference *treeref = gtk_tree_row_reference_new(model, (GtkTreePath *)ptr->data);
        ref = context->SendMessage1(oself, "create_tree_row_reference",
                                    context->NewPointer(gtk_tree_row_reference_copy(treeref)));
        context->ArrayPut(arr, (RexxObjectPtr)ref, i++);
        gtk_tree_row_reference_free(treeref);
        ptr = ptr->next;
    }
    g_list_foreach(list, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(list);

    return arr;
}

/**
 * Method:  set_mode
 *
 * Set the selection mode.
 *
 * @param mode    The mode
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxTreeSelectionSetMode,   // Object_method name
            int, mode,                 // The mode
            CSELF, cself)              // Self
{

    gtk_tree_selection_set_mode((GtkTreeSelection *)cself,
                                (GtkSelectionMode)mode);

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
            GrxTreeSelectionSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_changed";
        g_signal_connect(G_OBJECT(self), "changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->False();
}

