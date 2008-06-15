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

    gint **types = (gint **)malloc(sizeof(int) * members);
    if (members) {
        for (int i = 0; i < members; i++) {
            context->ObjectToNumber(context->ArrayAt(args, i), types[i]);
        }
        lstore = (GtkListStore *)gtk_list_store_newv(members, (GType *)types);
    }
    context->SetObjectVariable("!POINTER", context->NewPointer(lstore));
    g_object_set_data(G_OBJECT(lstore), "OORXOBJECT", self);
    context->SetObjectVariable("!COLTYPES", context->NewPointer(types));

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
RexxMethod2(int,                       // Return type
            GrxListStoreSetValue,      // Object_method name
            RexxObjectPtr, rxiter,     // Row iterator
            ARGLIST, args)             // Argument array
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkListStore *lstore = (GtkListStore *)context->PointerValue(rxptr);
    GtkTreeIter *iter = (GtkTreeIter *)context->PointerValue((RexxPointerObject)rxiter);
    rxptr = (RexxPointerObject)context->GetObjectVariable("!COLTYPES");
    gint **types = (gint **)context->PointerValue(rxptr);
    size_t members = context->ArraySize(args);
    int i, col, ival;
    unsigned int uival;
    long long ival64;
    unsigned long long uival64;
    float fval;
    double dval;
    const char *val;

    for (i = 2; i <= members; i += 2) {
        context->ObjectToNumber(context->ArrayAt(args, i), &col);
        switch (*types[col]) {
        case G_TYPE_POINTER:
        case G_TYPE_STRING:
        case G_TYPE_OBJECT:
            val = context->StringData((RexxStringObject)context->ArrayAt(args, i - 1));
            gtk_list_store_set_value(lstore, iter, col, (GValue *)val);
            break;
        case G_TYPE_INT:
        case G_TYPE_BOOLEAN:
        case G_TYPE_LONG:
        case G_TYPE_ENUM:
        case G_TYPE_FLAGS:
            context->ObjectToNumber((RexxStringObject)context->ArrayAt(args, i - 1), &ival);
            gtk_list_store_set_value(lstore, iter, col, (GValue *)&ival);
            break;
        case G_TYPE_UINT:
        case G_TYPE_ULONG:
            context->ObjectToUnsignedNumber((RexxStringObject)context->ArrayAt(args, i - 1), &uival);
            gtk_list_store_set_value(lstore, iter, col, (GValue *)&uival);
            break;
        case G_TYPE_INT64:
            context->ObjectToInt64((RexxStringObject)context->ArrayAt(args, i - 1), &ival64);
            gtk_list_store_set_value(lstore, iter, col, (GValue *)&ival64);
            break;
        case G_TYPE_UINT64:
            context->ObjectToUnsignedInt64((RexxStringObject)context->ArrayAt(args, i - 1), &uival64);
            gtk_list_store_set_value(lstore, iter, col, (GValue *)&uival64);
            break;
        case G_TYPE_DOUBLE:
            context->ObjectToDouble((RexxStringObject)context->ArrayAt(args, i - 1), &dval);
            gtk_list_store_set_value(lstore, iter, col, (GValue *)&dval);
            break;
        case G_TYPE_CHAR:
            context->ObjectToNumber((RexxStringObject)context->ArrayAt(args, i - 1), &ival);
            gtk_list_store_set_value(lstore, iter, col, (GValue *)&ival);
            break;
        case G_TYPE_UCHAR:
            context->ObjectToUnsignedNumber((RexxStringObject)context->ArrayAt(args, i - 1), &uival);
            gtk_list_store_set_value(lstore, iter, col, (GValue *)&uival);
            break;
        default:
            break;
        }
    }

    return 0;
}

