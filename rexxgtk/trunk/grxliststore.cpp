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
 * Create a liststore.
 *
 * @param type1   The type
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxListStoreNew,           // Object_method name
            ARGLIST, args,             // Array of column types
            OSELF, self)               // Self
{
    GtkListStore *lstore = NULL;
    size_t       members = context->ArraySize(args);

    GType *types = (GType *)malloc(sizeof(GType) * members);
    if (!types) {
        context->RaiseException0(Rexx_Error_System_resources);
        return 0;
    }
    for (int i = 1; i <= members; i++) {
        GType type;
        int32_t temp;
        context->ObjectToInt32(context->ArrayAt(args, i), &temp);
        types[i - 1] = (GType) temp;
    }
    printf("Got here\n");
    lstore = (GtkListStore *)gtk_list_store_newv((gint)members, types);

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(lstore));
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
RexxMethod1(POINTER,                   // Return type
            GrxListStoreAppend,        // Object_method name
            CSELF, self)               // GTK self
{
    GtkTreeIter iter;

    gtk_list_store_append(GTK_LIST_STORE(self), &iter);

    return gtk_tree_iter_copy(&iter);
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
            POINTER, rxiter,           // Row iterator
            ARGLIST, args,             // Argument array
            CSELF, self)               // GTK self
{
    GtkTreeIter *iter = (GtkTreeIter *)rxiter;
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!COLTYPES");
    gint *types = (gint *)context->PointerValue(rxptr);
    size_t members = context->ArraySize(args);
    int i, col, ival;
    uint32_t uival;
    int64_t ival64;
    uint64_t uival64;
    double dval;
    char *val;
    GValue coldata;

    // The following works for 32/64 bit systems (Windows and Linux). It may need to be
    // adjusted if we ever go to 128 bit word sizes.

    for (i = 2; i <= members; i += 2) {
        context->ObjectToInt32(context->ArrayAt(args, i), &col);
        coldata.g_type = types[col];
        switch (types[col]) {
        case G_TYPE_POINTER:
        case G_TYPE_STRING:
        case G_TYPE_OBJECT:
            val = (char *)context->ObjectToStringValue(context->ArrayAt(args, i + 1));
            coldata.data[0].v_pointer = val;
            break;
        case G_TYPE_INT:
        case G_TYPE_CHAR:
            context->ObjectToInt32(context->ArrayAt(args, i + 1), &ival);
            coldata.data[0].v_int = ival;
            break;
        case G_TYPE_UINT:
        case G_TYPE_UCHAR:
        case G_TYPE_BOOLEAN:
        case G_TYPE_ENUM:
        case G_TYPE_FLAGS:
            context->ObjectToUnsignedInt32(context->ArrayAt(args, i + 1), &uival);
            coldata.data[0].v_uint = uival;
            break;
        case G_TYPE_LONG:
#if __WORDSIZE == 64
            context->ObjectToInt64(context->ArrayAt(args, i + 1), &ival64);
            coldata.data[0].v_int = ival64;
#else
            context->ObjectToInt32(context->ArrayAt(args, i + 1), &ival);
            coldata.data[0].v_int = ival;
#endif
            break;
        case G_TYPE_ULONG:
#if __WORDSIZE == 64
            context->ObjectToUnsignedInt64(context->ArrayAt(args, i + 1), &uival64);
            coldata.data[0].v_int = uival64;
#else
            context->ObjectToUnsignedInt32(context->ArrayAt(args, i + 1), &uival);
            coldata.data[0].v_int = uival;
#endif
            break;
        case G_TYPE_INT64:
            context->ObjectToInt64(context->ArrayAt(args, i + 1), &ival64);
            coldata.data[0].v_int64 = ival64;
            break;
        case G_TYPE_UINT64:
            context->ObjectToUnsignedInt64(context->ArrayAt(args, i + 1), &uival64);
            coldata.data[0].v_uint64 = uival64;
            break;
        case G_TYPE_DOUBLE:
            context->ObjectToDouble(context->ArrayAt(args, i + 1), &dval);
            coldata.data[0].v_double = dval;
            break;
        default:
            break;
        }
        gtk_list_store_set_value(GTK_LIST_STORE(self), iter, col, &coldata);
    }

    return 0;
}

