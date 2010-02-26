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
/* Public Methods                                                             */
/*============================================================================*/

/**
 * Method: GrxGSListNew
 *
 * Create a new GSList.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxGSListNew,              // Routine name
            OPTIONAL_POINTER, ptr,     // List head
            OSELF, self)               // Self
{

    if (ptr == NULL) {
        context->SetObjectVariable("CSELF", context->NewPointer(NULL));
    }
    else {
        GSList * head = (GSList *)ptr;
        context->SetObjectVariable("CSELF", context->NewPointer(head));
    }
    return 0;
}

/**
 * Method: GrxGSListUninit
 *
 * Destroy a new GSList.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxGSListUninit,           // Routine name
            CSELF, self)               // GTK self
{
    g_slist_free((GSList *)self);

    return 0;
}

/**
 * Method: GrxGSListAppend
 *
 * Append an item to the list.
 *
 * @param item    The item string
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxGSListAppend,           // Routine name
            CSTRING, item,             // Item to append
            CSELF, self)               // GTK self
{
    GSList *head = g_slist_append((GSList *)self, (void *)item);
    context->SetObjectVariable("CSELF", context->NewPointer(head));

    return 0;
}

/**
 * Method: GrxGSListPrepend
 *
 * prepend an item to the list.
 *
 * @param item    The item string
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxGSListPrepend,          // Routine name
            CSTRING, item,             // Item to append
            CSELF, self)               // GTK self
{
    GSList *head = g_slist_prepend((GSList *)self, (void *)item);
    context->SetObjectVariable("CSELF", context->NewPointer(head));

    return 0;
}

/**
 * Method: GrxGSListInsert
 *
 * Insert an item to the list.
 *
 * @param item    The item string
 *
 * @param pos     The position (1-based)
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxGSListInsert,           // Routine name
            CSTRING, item,             // Item to append
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
    GSList *head = g_slist_insert((GSList *)self, (void *)item, pos - 1);
    context->SetObjectVariable("CSELF", context->NewPointer(head));

    return 0;
}

/**
 * Method: GrxGSListItem
 *
 * Get the list item.
 *
 * @param item    Pointer to the item
 *
 * @return        Zero
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxGSListItem,             // Routine name
            POINTER, rxptr)            // Pointer to the item
{
    GSList *item = (GSList *)rxptr;

    return context->NewStringFromAsciiz((char *)item->data);
}

/**
 * Method: GrxGSListFirst
 *
 * Get the first list item.
 *
 * @return        Zero
 **/
RexxMethod1(POINTER,                   // Return type
            GrxGSListFirst,            // Routine name
            CSELF, self)               // GTK self
{
    if (self == NULL) {
        return context->Nil();
    }
    return self;
}

/**
 * Method: GrxGSListNext
 *
 * Get the next list item.
 *
 * @return        Zero
 **/
RexxMethod1(POINTER,                   // Return type
            GrxGSListNext,             // Routine name
            POINTER, rxptr)            // Routine name
{
    GSList *item = (GSList *)rxptr;

    if (item->next == NULL) {
        return context->Nil();
    }
    return item->next;
}


/**
 * Method: GrxGListNew
 *
 * Create a new GList.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxGListNew,               // Routine name
            OSELF, self)               // Self
{
    context->SetObjectVariable("CSELF", context->NewPointer(NULL));

    return 0;
}

/**
 * Method: GrxGListUninit
 *
 * Destroy a GList.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxGListUninit,            // Routine name
            CSELF, self)               // GTK self
{
    g_list_free((GList *)self);

    return 0;
}

/**
 * Method: GrxGListAppend
 *
 * Append an item to the list.
 *
 * @param item    The item string
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxGListAppend,            // Routine name
            CSTRING, item,             // Item to append
            CSELF, self)               // GTK self
{
    GList *head = g_list_append((GList *)self, (void *)item);
    context->SetObjectVariable("CSELF", context->NewPointer(head));

    return 0;
}

/**
 * Method: GrxGListPrepend
 *
 * Prepend an item to the list.
 *
 * @param item    The item string
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxGListPrepend,           // Routine name
            CSTRING, item,             // Item to append
            CSELF, self)               // GTK self
{
    GList *head = g_list_prepend((GList *)self, (void *)item);
    context->SetObjectVariable("CSELF", context->NewPointer(head));

    return 0;
}

/**
 * Method: GrxGListInsert
 *
 * Insert an item to the list.
 *
 * @param item    The item string
 *
 * @param pos     The position (1-based)
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxGListInsert,            // Routine name
            CSTRING, item,             // Item to append
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
    GList *head = g_list_insert((GList *)self, (void *)item, pos - 1);
    context->SetObjectVariable("CSELF", context->NewPointer(head));

    return 0;
}

/**
 * Method: GrxGListItem
 *
 * Get the list item.
 *
 * @param item    Pointer to the item
 *
 * @return        Zero
 **/
RexxMethod1(POINTER,                   // Return type
            GrxGListItem,              // Routine name
            POINTER, rxptr)            // Pointer to the item
{
    GList *item = (GList *)rxptr;

    return item->data;
}


/**
 * Method: GrxGListFirst
 *
 * Get the first list item.
 *
 * @return        Zero
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxGListFirst,             // Routine name
            CSELF, self)               // GTK self
{
    if (self == NULL) {
        return context->Nil();
    }
    return (RexxObjectPtr)context->NewPointer(self);
}

/**
 * Method: GrxGListNext
 *
 * Get the next list item.
 *
 * @return        Zero
 **/
RexxMethod1(POINTER,                   // Return type
            GrxGListNext,              // Routine name
            POINTER, rxptr)            // Routine name
{
    GList *item = (GList *)rxptr;

    if (item->next == NULL) {
        return context->Nil();
    }
    return item->next;
}

