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
            OSELF, self,               // Self
            OPTIONAL_CSTRING, spath)   // Path string
{
    GtkTreePath *path;

    if (spath == NULL) {
        path = gtk_tree_path_new();
    }
    else {
        path = gtk_tree_path_new_from_string(spath);
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
            RexxObjectPtr, obj,        // Note to test
            CSELF, self)               // GTK self
{
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
            GrxTreePathIdDescendant,   // Object_method name
            RexxObjectPtr, obj,        // Note to test
            CSELF, self)               // GTK self
{
    GtkTreePath *node = (GtkTreePath *)context->ObjectToCSelf(obj);

    return gtk_tree_path_is_descendant((GtkTreePath *)self, node);
}

