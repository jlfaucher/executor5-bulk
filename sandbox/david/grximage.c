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
/* Rexx External Function: GrxImageNew                                        */
/* Description: Create a new image                                            */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxImageNew(const char * Name,
                            const size_t Argc, const RXSTRING Argv[],
                            const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc,Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_image_new();

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxImageSetFromFile                                */
/* Description: Set the image.                                                */
/* Rexx Args:   Pointer to the image widget                                   */
/*              File name                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxImageSetFromFile(const char * Name,
                                    const size_t Argc, const RXSTRING Argv[],
                                    const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc,Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_image_set_from_file(GTK_IMAGE(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxImageSetFromStock                               */
/* Description: Set the image.                                                */
/* Rexx Args:   Pointer to the image widget                                   */
/*              Stock id                                                      */
/*              Size                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxImageSetFromStock(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkIconSize size;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc,Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_INVALID"))
        size = GTK_ICON_SIZE_INVALID;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_INVALID"))
        size = GTK_ICON_SIZE_INVALID;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_MENU"))
        size = GTK_ICON_SIZE_MENU;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_SMALL_TOOLBAR"))
        size = GTK_ICON_SIZE_SMALL_TOOLBAR;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_LARGE_TOOLBAR"))
        size = GTK_ICON_SIZE_LARGE_TOOLBAR;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_BUTTON"))
        size = GTK_ICON_SIZE_BUTTON;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_DND"))
        size = GTK_ICON_SIZE_DND;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_DIALOG"))
        size = GTK_ICON_SIZE_DIALOG;
    else
        sscanf(Argv[2].strptr, "%d", &size);

    gtk_image_set_from_stock(GTK_IMAGE(myWidget), Argv[1].strptr, size);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxImageSetFromIconName                            */
/* Description: Set the image.                                                */
/* Rexx Args:   Pointer to the image widget                                   */
/*              Icon name                                                     */
/*              Size                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxImageSetFromIconName(const char * Name,
                                    const size_t Argc, const RXSTRING Argv[],
                                    const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkIconSize size;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc,Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_INVALID"))
        size = GTK_ICON_SIZE_INVALID;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_INVALID"))
        size = GTK_ICON_SIZE_INVALID;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_MENU"))
        size = GTK_ICON_SIZE_MENU;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_SMALL_TOOLBAR"))
        size = GTK_ICON_SIZE_SMALL_TOOLBAR;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_LARGE_TOOLBAR"))
        size = GTK_ICON_SIZE_LARGE_TOOLBAR;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_BUTTON"))
        size = GTK_ICON_SIZE_BUTTON;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_DND"))
        size = GTK_ICON_SIZE_DND;
    else if (!strcmp(Argv[2].strptr, "GTK_ICON_SIZE_DIALOG"))
        size = GTK_ICON_SIZE_DIALOG;
    else
        sscanf(Argv[2].strptr, "%d", &size);

    gtk_image_set_from_icon_name(GTK_IMAGE(myWidget), Argv[1].strptr, size);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

