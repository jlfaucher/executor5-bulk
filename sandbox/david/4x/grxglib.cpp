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
RexxMethod0(int,                       // Return type
            GrxGSListNew)              // Routine name
{
    context->SetObjectVariable("!POINTER", context->NewPointer(NULL));

    return 0;
}

/**
 * Method: GrxGSListUninit
 *
 * Destroy a new GSList.
 *
 * @return        Zero
 **/
RexxMethod0(int,                       // Return type
            GrxGSListUninit)           // Routine name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GSList *head = (GSList *)context->PointerValue(rxptr);

    g_slist_free(head);

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
RexxMethod1(int,                       // Return type
            GrxGSListAppend,           // Routine name
            CSTRING, item)             // Item to append
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GSList *head = (GSList *)context->PointerValue(rxptr);

    head = g_slist_append(head, (void *)item);
    context->SetObjectVariable("!POINTER", context->NewPointer(head));

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
RexxMethod1(int,                       // Return type
            GrxGSListPrepend,          // Routine name
            CSTRING, item)             // Item to append
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GSList *head = (GSList *)context->PointerValue(rxptr);

    head = g_slist_prepend(head, (void *)item);
    context->SetObjectVariable("!POINTER", context->NewPointer(head));

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
RexxMethod2(int,                       // Return type
            GrxGSListInsert,           // Routine name
            CSTRING, item,             // Item to append
            int, pos)                  // Position
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GSList *head = (GSList *)context->PointerValue(rxptr);

    head = g_slist_insert(head, (void *)item, pos - 1);
    context->SetObjectVariable("!POINTER", context->NewPointer(head));

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
            RexxObjectPtr, rxptr)      // Pointer to the item
{
    GSList *item = (GSList *)context->PointerValue((RexxPointerObject)rxptr);

    return context->NewStringFromAsciiz((char *)item->data);
}

/**
 * Method: GrxGSListFirst
 *
 * Get the first list item.
 *
 * @return        Zero
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxGSListFirst)            // Routine name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GSList *head = (GSList *)context->PointerValue(rxptr);

    if (head == NULL) {
        return context->Nil();
    }
    return (RexxObjectPtr)head;
}

/**
 * Method: GrxGSListNext
 *
 * Get the next list item.
 *
 * @return        Zero
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxGSListNext,             // Routine name
            RexxObjectPtr, rxptr)      // Routine name
{
    GSList *item = (GSList *)context->PointerValue((RexxPointerObject)rxptr);

    if (item->next == NULL) {
        return context->Nil();
    }
    return (RexxObjectPtr)context->NewPointer(item->next);
}


/**
 * Method: GrxGListNew
 *
 * Create a new GList.
 *
 * @return        Zero
 **/
RexxMethod0(int,                       // Return type
            GrxGListNew)               // Routine name
{
    context->SetObjectVariable("!POINTER", context->NewPointer(NULL));

    return 0;
}

/**
 * Method: GrxGListUninit
 *
 * Destroy a GList.
 *
 * @return        Zero
 **/
RexxMethod0(int,                       // Return type
            GrxGListUninit)            // Routine name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GList *head = (GList *)context->PointerValue(rxptr);

    g_list_free(head);

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
RexxMethod1(int,                       // Return type
            GrxGListAppend,            // Routine name
            CSTRING, item)             // Item to append
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GList *head = (GList *)context->PointerValue(rxptr);

    head = g_list_append(head, (void *)item);
    context->SetObjectVariable("!POINTER", context->NewPointer(head));

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
RexxMethod1(int,                       // Return type
            GrxGListPrepend,           // Routine name
            CSTRING, item)             // Item to append
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GList *head = (GList *)context->PointerValue(rxptr);

    head = g_list_prepend(head, (void *)item);
    context->SetObjectVariable("!POINTER", context->NewPointer(head));

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
RexxMethod2(int,                       // Return type
            GrxGListInsert,            // Routine name
            CSTRING, item,             // Item to append
            int, pos)                  // Position
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GList *head = (GList *)context->PointerValue(rxptr);

    head = g_list_insert(head, (void *)item, pos - 1);
    context->SetObjectVariable("!POINTER", context->NewPointer(head));

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
RexxMethod1(RexxObjectPtr,             // Return type
            GrxGListItem,              // Routine name
            RexxObjectPtr, rxptr)      // Pointer to the item
{
    GList *item = (GList *)context->PointerValue((RexxPointerObject)rxptr);

    return context->NewStringFromAsciiz((char *)item->data);
}


/**
 * Method: GrxGListFirst
 *
 * Get the first list item.
 *
 * @return        Zero
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxGListFirst)             // Routine name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GList *head = (GList *)context->PointerValue(rxptr);

    if (head == NULL) {
        return context->Nil();
    }
    return (RexxObjectPtr)head;
}

/**
 * Method: GrxGListNext
 *
 * Get the next list item.
 *
 * @return        Zero
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxGListNext,              // Routine name
            RexxObjectPtr, rxptr)      // Routine name
{
    GList *item = (GList *)context->PointerValue((RexxPointerObject)rxptr);

    if (item->next == NULL) {
        return context->Nil();
    }
    return (RexxObjectPtr)context->NewPointer(item->next);
}

