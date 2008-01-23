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
/* Rexx External Function: GrxFixedNew                                        */
/* Description: Create a fixed      .                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFixedNew(const char * Name,
                            const size_t Argc, const RXSTRING Argv[],
                            const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_fixed_new();
	
    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFixedPut                                        */
/* Description: Attach a widget to the fixed.                                 */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the widget to be fixed                             */
/*              xpos                                                          */
/*              ypos                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFixedPut(const char * Name,
                            const size_t Argc, const RXSTRING Argv[],
                            const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *fixedWidget, *myWidget;
    gint xpos, ypos;

    /* Check for valid arguments */
    if (GrxCheckArgs(4, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &fixedWidget);
    sscanf(Argv[1].strptr, "%p", &myWidget);
    sscanf(Argv[2].strptr, "%d", &xpos);
    sscanf(Argv[3].strptr, "%d", &ypos);

    if (GTK_IS_WIDGET(GTK_OBJECT(fixedWidget))) {
        gtk_fixed_put(GTK_FIXED(fixedWidget), myWidget, xpos, ypos);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFixedMove                                       */
/* Description: Move a widget in the fixed.                                   */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the widget to be moved                             */
/*              xpos                                                          */
/*              ypos                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFixedMove(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *fixedWidget, *myWidget;
    gint xpos, ypos;

    /* Check for valid arguments */
    if (GrxCheckArgs(4, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &fixedWidget);
    sscanf(Argv[1].strptr, "%p", &myWidget);
    sscanf(Argv[2].strptr, "%d", &xpos);
    sscanf(Argv[3].strptr, "%d", &ypos);

    if (GTK_IS_WIDGET(GTK_OBJECT(fixedWidget))) {
        gtk_fixed_move(GTK_FIXED(fixedWidget), myWidget, xpos, ypos);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

