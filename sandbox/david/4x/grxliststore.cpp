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
 * Create an adjustment.
 *
 * @param type1   The type
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxListStoretNew,          // Object_method name
            OSELF, self,               // Self
            ARGLIST, args)             // Array of column types
{
    GtkListStore    *lstore;  
    size_t members = context->ArraySize(args);
    gint *types[members];

    if (members) {
        for (int i = 0; i < members; i++) {
            context->ObjectToNumber(context->ArrayAt(args, i), types[i]);
        }
        lstore = (GtkListStore *)gtk_list_store_newv(members, (GType *)types);
    }
    context->SetObjectVariable("!POINTER", context->NewPointer(lstore));
    g_object_set_data(G_OBJECT(lstore), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  append
 *
 * Append a new row
 *
 * @return        Row iterator
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxListStoreAppend)        // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkListStore *lstore = (GtkListStore *)context->PointerValue(rxptr);
    GtkTreeIter *iter = (GtkTreeIter *)malloc(sizeof(GtkTreeIter));

    gtk_list_store_append(lstore, iter);

    return (RexxObjectPtr)context->NewPointer(iter);
}

/**
 * Method:  set_value
 *
 * Set the value in a column.
 *
 * @param iter    The iterator
 *
 * @param colnum  The column number
 *
 * @param val     The value
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxListStoreSetValue,      // Object_method name
            RexxObjectPtr, rxiter,     // Row iterator
            int, colnum,               // Column number
            CSTRING, val)              // Column value
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkListStore *lstore = (GtkListStore *)context->PointerValue(rxptr);
    GtkTreeIter *iter = (GtkTreeIter *)context->PointerValue((RexxPointerObject)rxiter);

    gtk_list_store_set_value(lstore, iter, colnum, (GValue *)val);

    return 0;
}

