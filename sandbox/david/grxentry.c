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
/* Rexx External Function: GrxEntryNew                                        */
/* Description: Create a new entry box                                        */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxEntryNew(const char * Name,
                            const size_t Argc, const RXSTRING Argv[],
                            const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_entry_new();

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxEntrySetMaxLength                               */
/* Description: Set the max number of characters for the entry box            */
/* Rexx Args:   Pointer to the container widget                               */
/*              Max length                                                    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxEntrySetMaxLength(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint maxlen;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &maxlen);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_entry_set_max_length(GTK_ENTRY(myWidget), maxlen); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxEntryGetMaxLength                               */
/* Description: Get the max number of characters for the entry box            */
/* Rexx Args:   Pointer to the container widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxEntryGetMaxLength(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint maxlen = 0;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        maxlen = gtk_entry_get_max_length(GTK_ENTRY(myWidget)); 
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", maxlen);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxEntrySetText                                    */
/* Description: Set the text in the entry box                                 */
/* Rexx Args:   Pointer to the container widget                               */
/*              Text                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxEntrySetText(const char * Name, 
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_entry_set_text(GTK_ENTRY(myWidget), Argv[1].strptr); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxEntryGetText                                    */
/* Description: Get the text in the entry box                                 */
/* Rexx Args:   Pointer to the container widget                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxEntryGetText(const char * Name,
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    const gchar *text = "\0";

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        text = gtk_entry_get_text(GTK_ENTRY(myWidget)); 
    }

    /* Set up the REXX return code */
    if (strlen(text) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(text) + 1);
    }
    sprintf(Retstr->strptr, "%s", text);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxEntrySetWidthChars                              */
/* Description: Set the width (in characters) of the entry box                */
/* Rexx Args:   Pointer to the container widget                               */
/*              Width                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxEntrySetWidthChars(const char * Name, 
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint width;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &width);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_entry_set_width_chars(GTK_ENTRY(myWidget), width); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

