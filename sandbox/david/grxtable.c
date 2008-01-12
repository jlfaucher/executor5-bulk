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
/* Rexx External Function: GrxTableNew                                        */
/* Description: Create a table      .                                         */
/* Rexx Args:   Rows                                                          */
/*              Columns                                                       */
/*              Homogeneous flag                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTableNew(char * Name, size_t Argc, RXSTRING Argv[],
                            char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint rows, cols;
    gboolean homogeneous;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    sscanf(Argv[0].strptr, "%d", &rows);
    sscanf(Argv[1].strptr, "%d", &cols);
    sscanf(Argv[2].strptr, "%d", &homogeneous);
    myWidget = gtk_table_new(rows, cols, homogeneous);
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTableAttach                                     */
/* Description: Attach a widget to the table.                                 */
/* Rexx Args:   Pointer to the widget                                         */
/*              Pointer to the widget to be added                             */
/*              Left                                                          */
/*              Right                                                         */
/*              Top                                                           */
/*              Bottom                                                        */
/*              xoptions                                                      */
/*              yoptions                                                      */
/*              xpading                                                       */
/*              yPadding                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTableAttach(char * Name, size_t Argc, RXSTRING Argv[],
                               char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *tableWidget, *myWidget;
    gint left, right, top, bottom;
    GtkAttachOptions xoptions, yoptions;
    gint xpad, ypad;

    /* Check for valid arguments */
    if (GrxCheckArgs(10, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &tableWidget);
    sscanf(Argv[1].strptr, "%p", &myWidget);
    sscanf(Argv[2].strptr, "%d", &left);
    sscanf(Argv[3].strptr, "%d", &right);
    sscanf(Argv[4].strptr, "%d", &top);
    sscanf(Argv[5].strptr, "%d", &bottom);
    if(strcmp(Argv[6].strptr,"GTK_EXPAND") == 0)
        xoptions = GTK_EXPAND;
    else if(strcmp(Argv[6].strptr,"GTK_SHRINK") == 0)
        xoptions = GTK_SHRINK;
    else if(strcmp(Argv[6].strptr,"GTK_FILL") == 0)
        xoptions = GTK_FILL;
    else 
        return RXFUNC_BADCALL;
    if(strcmp(Argv[7].strptr,"GTK_EXPAND") == 0)
        yoptions = GTK_EXPAND;
    else if(strcmp(Argv[7].strptr,"GTK_SHRINK") == 0)
        yoptions = GTK_SHRINK;
    else if(strcmp(Argv[7].strptr,"GTK_FILL") == 0)
        yoptions = GTK_FILL;
    else 
        return RXFUNC_BADCALL;
    sscanf(Argv[8].strptr, "%d", &xpad);
    sscanf(Argv[8].strptr, "%d", &ypad);

    if (GTK_IS_WIDGET(GTK_OBJECT(tableWidget))) {
        gtk_table_attach(GTK_TABLE(tableWidget), myWidget, left, right, top,
                         bottom, xoptions, yoptions, xpad, ypad);         
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTableSetRowSpacings                             */
/* Description: Set the spacing of table rows.                                */
/* Rexx Args:   Pointer to the widget                                         */
/*              Spacing                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTableSetRowSpacings(char * Name, size_t Argc, RXSTRING Argv[],
                                       char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint spacing;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &spacing);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_table_set_row_spacings(GTK_TABLE(myWidget), spacing);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxTableSetColSpacings                             */
/* Description: Set the spacing of table columns                              */
/* Rexx Args:   Pointer to the widget                                         */
/*              Spacing                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxTableSetColSpacings(char * Name, size_t Argc, RXSTRING Argv[],
                                       char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint spacing;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &spacing);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_table_set_col_spacings(GTK_TABLE(myWidget), spacing);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

