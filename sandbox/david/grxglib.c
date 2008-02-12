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


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxMessage                                         */
/* Description: Message logging                                               */
/* Rexx Args:   Message string                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxMessage(const char * Name,
                           const size_t Argc, const RXSTRING Argv[],
                           const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    g_message(Argv[0].strptr);
	
    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWarning                                         */
/* Description: Warning logging                                               */
/* Rexx Args:   Message string                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxWarning(const char * Name,
                           const size_t Argc, const RXSTRING Argv[],
                           const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    g_warning(Argv[0].strptr);
	
    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxCritical                                        */
/* Description: Critical logging                                              */
/* Rexx Args:   Message string                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxCritical(const char * Name,
                            const size_t Argc, const RXSTRING Argv[],
                            const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    g_critical(Argv[0].strptr);
	
    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxError                                           */
/* Description: Error logging                                                 */
/* Rexx Args:   Message string                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxError(const char * Name,
                         const size_t Argc, const RXSTRING Argv[],
                         const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    g_error(Argv[0].strptr);
	
    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDebug                                           */
/* Description: Debug logging                                                 */
/* Rexx Args:   Message string                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDebug(const char * Name,
                         const size_t Argc, const RXSTRING Argv[],
                         const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    g_debug(Argv[0].strptr);
	
    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGetUserName                                     */
/* Description: Return the user name.                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxGetUserName(const char * Name,
                               const size_t Argc, const RXSTRING Argv[],
                               const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    user = g_get_user_name();
	
    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", user);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGetRealName                                     */
/* Description: Return the user name.                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxGetRealName(const char * Name,
                               const size_t Argc, const RXSTRING Argv[],
                               const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    user = g_get_real_name();
	
    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", user);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGetHomeDir                                      */
/* Description: Return the user name.                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxGetHomeDir(const char * Name,
                              const size_t Argc, const RXSTRING Argv[],
                              const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    user = g_get_home_dir();
	
    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", user);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGetHostName                                     */
/* Description: Return the user name.                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxGetHostName(const char * Name,
                               const size_t Argc, const RXSTRING Argv[],
                               const char * Queuename, PRXSTRING Retstr)
{
    const gchar * user;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    user = g_get_host_name();
	
    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", user);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGSListUninit                                    */
/* Description: Free list storage                                             */
/* Rexx Args:   Pointer to the list head                                      */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGSListUninit(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
{
    GSList *myList;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myList);

    g_slist_free(myList);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGSListAppend                                    */
/* Description: Append an item to the list.                                   */
/* Rexx Args:   Group list head                                               */
/*              Item string                                                   */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGSListAppend(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
{
    GSList *head;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);

    head = g_slist_append(head, Argv[1].strptr);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGSListPrepend                                   */
/* Description: Prepend an item to the list.                                  */
/* Rexx Args:   Group list head                                               */
/*              Item string                                                   */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGSListPrepend(PSZ Name, LONG Argc, RXSTRING Argv[],
                                PSZ Queuename, PRXSTRING Retstr)
{
    GSList *head;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);

    head = g_slist_prepend(head, Argv[1].strptr);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGSListInsert                                    */
/* Description: Insert an item to the list.                                   */
/* Rexx Args:   Group list head                                               */
/*              Item string                                                   */
/*              Position (one-based)                                          */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGSListInsert(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
{
    GSList *head;
    gint pos;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);
    sscanf(Argv[2].strptr, "%d", &pos);

    head = g_slist_insert(head, Argv[1].strptr, pos - 1);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGSListItem                                      */
/* Description: Get the list item                                             */
/* Rexx Args:   Item pointer                                                  */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGSListItem(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
{
    GSList *item;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &item);

    /* Set up the REXX return code */
    if (strlen(item->data) > RXAUTOBUFLEN) {
        Retstr->strptr = (char *) RexxAllocateMemory(strlen(item->data) + 1);
    }
    strcpy(Retstr->strptr, item->data);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGSListNext                                      */
/* Description: Get the pointer to the next list item                         */
/* Rexx Args:   Item pointer                                                  */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGSListNext(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
{
    GSList *item;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &item);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", item->next);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGListUninit                                     */
/* Description: Free list storage                                             */
/* Rexx Args:   Pointer to the list head                                      */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGListUninit(PSZ Name, LONG Argc, RXSTRING Argv[],
                              PSZ Queuename, PRXSTRING Retstr)
{
    GList *myList;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myList);

    g_list_free(myList);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGListAppend                                     */
/* Description: Append an item to the list.                                   */
/* Rexx Args:   Group list head                                               */
/*              Item string                                                   */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGListAppend(PSZ Name, LONG Argc, RXSTRING Argv[],
                              PSZ Queuename, PRXSTRING Retstr)
{
    GList *head;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);

    head = g_list_append(head, Argv[1].strptr);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGListPrepend                                    */
/* Description: Prepend an item to the list.                                  */
/* Rexx Args:   Group list head                                               */
/*              Item string                                                   */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGListPrepend(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
{
    GList *head;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);

    head = g_list_prepend(head, Argv[1].strptr);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGListInsert                                     */
/* Description: Insert an item to the list.                                   */
/* Rexx Args:   Group list head                                               */
/*              Item string                                                   */
/*              Position (one-based)                                          */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGListInsert(PSZ Name, LONG Argc, RXSTRING Argv[],
                              PSZ Queuename, PRXSTRING Retstr)
{
    GList *head;
    gint pos;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);
    sscanf(Argv[2].strptr, "%d", &pos);

    head = g_list_insert(head, Argv[1].strptr, pos - 1);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGListItem                                       */
/* Description: Get the list item                                             */
/* Rexx Args:   Item pointer                                                  */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGListItem(PSZ Name, LONG Argc, RXSTRING Argv[],
                            PSZ Queuename, PRXSTRING Retstr)
{
    GList *item;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &item);

    /* Set up the REXX return code */
    if (strlen(item->data) > RXAUTOBUFLEN) {
        Retstr->strptr = (char *) RexxAllocateMemory(strlen(item->data) + 1);
    }
    strcpy(Retstr->strptr, item->data);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxGListNext                                       */
/* Description: Get the pointer to the next list item                         */
/* Rexx Args:   Item pointer                                                  */
/*----------------------------------------------------------------------------*/

ULONG APIENTRY GrxGListNext(PSZ Name, LONG Argc, RXSTRING Argv[],
                            PSZ Queuename, PRXSTRING Retstr)
{
    GList *item;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &item);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", item->next);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}

