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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a tree path.
 *
 * @param spath   The path string
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTreePathNew,            // Object_method name
            OPTIONAL_RexxObjectPtr, spath, // Path string
            OSELF, self)               // Self
{
    GtkTreePath *path;

    if (spath == NULL) {
        path = gtk_tree_path_new();
    }
    else if (context->IsOfType(spath, "String")) {
        path = gtk_tree_path_new_from_string(context->ObjectToStringValue(spath));
    }
    else if (context->IsOfType(spath, "Pointer")) {
        path = (GtkTreePath *)context->PointerValue((RexxPointerObject)spath);
    }
    else {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("String"));
        return 0;
    }

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(path));

    return 0;
}

/**
 * Method:  to_string
 *
 * Convert a path to a string.
 *
 * @return        Path string
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxTreePathToString,       // Object_method name
            CSELF, self)               // GTK self
{
    char * spath;

    spath = gtk_tree_path_to_string((GtkTreePath *)self);

    return (RexxObjectPtr)context->NewStringFromAsciiz(spath);
}

/**
 * Method:  up
 *
 * Move up the tree.
 *
 * @return        Zero
 **/
RexxMethod1(logical_t,                  // Return type
            GrxTreePathUp,             // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tree_path_up((GtkTreePath *)self);
}

/**
 * Method:  down
 *
 * Move down the tree.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxTreePathDown,           // Object_method name
            CSELF, self)               // GTK self
{
    gtk_tree_path_down((GtkTreePath *)self);

    return 0;
}

/**
 * Method:  prev
 *
 * Move to the previous node.
 *
 * @return        Boolean
 **/
RexxMethod1(logical_t,                 // Return type
            GrxTreePathPrev,           // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tree_path_prev((GtkTreePath *)self);
}

/**
 * Method:  next
 *
 * Move to the next node.
 *
 * @return        Boolean
 **/
RexxMethod1(int,                       // Return type
            GrxTreePathNext,           // Object_method name
            CSELF, self)               // GTK self
{
    gtk_tree_path_next((GtkTreePath *)self);

    return 0;
}

/**
 * Method:  prepend_index
 *
 * Prepend an index.
 *
 * @param idx     The index
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxTreePathPrependIndex,   // Object_method name
            int, idx,                  // Index
            CSELF, self)               // GTK self
{
    gtk_tree_path_prepend_index((GtkTreePath *)self, idx);

    return 0;
}

/**
 * Method:  append_index
 *
 * Append an index.
 *
 * @param idx     The index
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxTreePathAppendIndex,    // Object_method name
            int, idx,                  // Index
            CSELF, self)               // GTK self
{
    gtk_tree_path_append_index((GtkTreePath *)self, idx);

    return 0;
}

/**
 * Method:  get_depth
 *
 * Get the depth.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxTreePathGetDepth,       // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tree_path_get_depth((GtkTreePath *)self);
}

/**
 * Method:  is_ancestor
 *
 * Is a node an ancestor.
 *
 * @param node    The node to test
 *
 * @return        Boolean
 **/
RexxMethod2(logical_t,                 // Return type
            GrxTreePathIsAncestor,     // Object_method name
            RexxObjectPtr, obj,        // Node to test
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(obj, "GtkTreePath")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkTreePath"));
        return 0;
    }
    GtkTreePath *node = (GtkTreePath *)context->ObjectToCSelf(obj);

    return gtk_tree_path_is_ancestor((GtkTreePath *)self, node);
}

/**
 * Method:  is_descendant
 *
 * Is a node an descendant.
 *
 * @param node    The node to test
 *
 * @return        Boolean
 **/
RexxMethod2(logical_t,                 // Return type
            GrxTreePathIsDescendant,   // Object_method name
            RexxObjectPtr, obj,        // Node to test
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(obj, "GtkTreePath")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkTreePath"));
        return 0;
    }
    GtkTreePath *node = (GtkTreePath *)context->ObjectToCSelf(obj);

    return gtk_tree_path_is_descendant((GtkTreePath *)self, node);
}

/**
 * Method:  init
 *
 * Create a tree row reference.
 *
 * @param pointer The pointer
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxTreeRowReferenceNew,    // Object_method name
            RexxObjectPtr, modelobj,   // Model pointer
            OPTIONAL_RexxObjectPtr, pathobj, // Path pointer
            OSELF, self)               // Self
{
    GtkTreeRowReference *ref;

    if (pathobj == NULL) {
        if (context->IsOfType(modelobj, "Pointer")) {
            // Save ourself
            context->SetObjectVariable("CSELF", modelobj);
        }
        else {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("Pointer"));
            return 0;
        }
    }
    else {
        if (!context->IsOfType(modelobj, "GtkTreeModel")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkTreeModel"));
            return 0;
        }
        GtkTreeModel *model = (GtkTreeModel *)context->ObjectToCSelf(modelobj);
        if (!context->IsOfType(pathobj, "GtkTreePath")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkTreePath"));
            return 0;
        }
        GtkTreePath *path = (GtkTreePath *)context->ObjectToCSelf(pathobj);
        ref = gtk_tree_row_reference_new(model, path);

        // Save ourself
        context->SetObjectVariable("CSELF", context->NewPointer(ref));
    }

    return 0;
}

/**
 * Method:  valid
 *
 * Is the row reference valid.
 *
 * @return        Boolean
 **/
RexxMethod1(logical_t,                 // Return type
            GrxTreeRowReferenceValid,  // Object_method name
            CSELF, self)               // GTK self
{

    return gtk_tree_row_reference_valid((GtkTreeRowReference *)self);
}

/**
 * Method:  get_path
 *
 * Get the path from an a row reference.
 *
 * @return        Path pointer
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxTreeRowReferenceGetPath, // Object_method name
            OSELF, oself,              // Self
            CSELF, cself)              // Self
{
    GtkTreePath *path = gtk_tree_row_reference_get_path((GtkTreeRowReference *)cself);

    return context->SendMessage1(oself, "create_tree_path", context->NewPointer(path));
}

/**
 * Method:  init
 *
 * Create a tree model.
 *
 * @param pointer The pointer
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTreeModelNew,           // Object_method name
            POINTER, ptr,              // Pointer
            OSELF, self)               // Self
{
    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(ptr));

    return 0;
}

/**
 * Method:  get_n_columns
 *
 * Get the number of columns.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxTreeModelGetNColumns,   // Object_method name
            CSELF, self)               // Self
{
    return gtk_tree_model_get_n_columns((GtkTreeModel *)self);
}

/**
 * Method:  get_column_type
 *
 * Get the column type.
 *
 * @param idx     The column index
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxTreeModelGetColumnType, // Object_method name
            int, idx,                  // Column index
            CSELF, self)               // Self
{
    return gtk_tree_model_get_column_type((GtkTreeModel *)self, idx);
}

/**
 * Method:  get_iter
 *
 * Get an iterator from a path.
 *        
 * @param path    The path
 *
 * @return        Iter pointer
 **/
RexxMethod2(POINTER,                   // Return type
            GrxTreeModelGetIter,       // Object_method name
            RexxObjectPtr, pathobj,    // The path
            CSELF, self)               // Self
{
    GtkTreeIter iter;
    if (!context->IsOfType(pathobj, "GtkTreePath")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkTreePath"));
        return 0;
    }
    GtkTreePath *path = (GtkTreePath *)context->ObjectToCSelf(pathobj);

    gboolean retc = gtk_tree_model_get_iter((GtkTreeModel *)self, &iter, path);
    if (retc) {
        return gtk_tree_iter_copy(&iter);
    }
    return NULL;
}

/**
 * Method:  get_iter_from_string
 *
 * Get an iterator from a string.
 *        
 * @param str     The string
 *
 * @return        Iter pointer
 **/
RexxMethod2(POINTER,                   // Return type
            GrxTreeModelGetIterFromString, // Object_method name
            CSTRING, str,              // The string
            CSELF, self)               // Self
{
    GtkTreeIter iter;

    gboolean retc = gtk_tree_model_get_iter_from_string((GtkTreeModel *)self, &iter, str);
    if (retc) {
        return gtk_tree_iter_copy(&iter);
    }
    return NULL;
}

/**
 * Method:  get_value 
 *
 * Get the value in a column.
 *
 * @param iter    The iterator
 *
 * @param col     The column number
 *
 * @return        Value
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxTreeModelGetValue,      // Object_method name
            POINTER, rxiter,           // Row iterator
            int, col,                  // Column number
            CSELF, self)               // GTK self
{
    GtkTreeIter *iter = (GtkTreeIter *)rxiter;
    GType type = gtk_tree_model_get_column_type(GTK_TREE_MODEL(self), col);
    long long coldata;

    gtk_tree_model_get(GTK_TREE_MODEL(self), iter, col, &coldata, -1);
    switch (type) {
    case G_TYPE_POINTER:
    case G_TYPE_STRING:
    case G_TYPE_OBJECT:
        return (RexxObjectPtr)context->NewStringFromAsciiz((char *)coldata);
        break;
    case G_TYPE_INT:
    case G_TYPE_BOOLEAN:
    case G_TYPE_LONG:
    case G_TYPE_ENUM:
    case G_TYPE_FLAGS:
    case G_TYPE_CHAR:
    case G_TYPE_UCHAR:
        return (RexxObjectPtr)context->Int32ToObject((gint)coldata);
        break;
    case G_TYPE_UINT:
    case G_TYPE_ULONG:
        return (RexxObjectPtr)context->UnsignedInt32ToObject((guint)coldata);
        break;
    case G_TYPE_INT64:
        return (RexxObjectPtr)context->Int64ToObject((long long)coldata);
        break;
    case G_TYPE_UINT64:
        return (RexxObjectPtr)context->UnsignedInt64ToObject((unsigned long long)coldata);
        break;
    case G_TYPE_DOUBLE:
        return (RexxObjectPtr)context->DoubleToObject((double)coldata);
        break;
    default:
        break;
    }

    return context->Nil();
}

/**
 * Method:  iter_next
 *
 * Move the iterator to the next entry
 *        
 * @param rxiter  The iter pointer
 *
 * @return        Iter pointer
 **/
RexxMethod2(POINTER,                   // Return type
            GrxTreeModelIterNext,      // Object_method name
            POINTER, rxiter,           // The iter
            CSELF, self)               // Self
{
    GtkTreeIter *iter = (GtkTreeIter *)rxiter;

    gboolean retc = gtk_tree_model_iter_next((GtkTreeModel *)self, iter);
    if (retc) {
        return iter;
    }
    gtk_tree_iter_free(iter);
    return NULL;
}

/**
 * Method:  get_path
 *
 * Get the path from an iter.
 *        
 * @param rxiter  The iter pointer
 *
 * @return        Path pointer
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxTreeModelGetPath,       // Object_method name
            CSELF, cself,              // Self
            POINTER, rxiter,           // The iter
            OSELF, oself)              // Self
{
    GtkTreeIter *iter = (GtkTreeIter *)rxiter;

    GtkTreePath *path = gtk_tree_model_get_path((GtkTreeModel *)cself, iter);

    return context->SendMessage1(oself, "create_tree_path", context->NewPointer(path));
}

/**
 * Method:  has_parent
 *
 * Return boolean true if the iter has a parent.
 *        
 * @param rxiter  The iter pointer
 *
 * @return        Path pointer
 **/
RexxMethod2(logical_t,                 // Return type
            GrxTreeModelHasParent,     // Object_method name
            POINTER, rxiter,           // The iter
            CSELF, self)               // Self
{
    GtkTreeIter *iter = (GtkTreeIter *)rxiter;
    GtkTreeIter parent;

    return gtk_tree_model_iter_parent((GtkTreeModel *)self, &parent, iter);
}

/**
 * Method:  get_parent
 *
 * Return the parent.
 *        
 * @param rxiter  The iter pointer
 *
 * @return        Object
 **/
RexxMethod2(POINTER,                   // Return type
            GrxTreeModelGetParent,     // Object_method name
            POINTER, rxiter,           // The iter
            CSELF, self)               // Self
{
    GtkTreeIter *iter = (GtkTreeIter *)rxiter;
    GtkTreeIter parent;

    gtk_tree_model_iter_parent((GtkTreeModel *)self, &parent, iter);
    return gtk_tree_iter_copy(&parent);
}

