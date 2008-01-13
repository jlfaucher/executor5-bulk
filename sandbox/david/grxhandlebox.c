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

static void signal_func_2(GtkWidget *window,
                          GtkWidget *widget,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %p", window, data, widget);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %p", window, data, widget);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHandleBoxNew                                    */
/* Description: Create a handlebox.                                           */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHandleBoxNew(const char * Name,
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_handle_box_new();
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHandleBoxSetShadowType                          */
/* Description: Set the expanded state.                                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              State                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHandleBoxSetShadowType(const char * Name,
                                          const size_t Argc, const RXSTRING Argv[],
                                          const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkShadowType type;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    if(strcmp(Argv[1].strptr,"GTK_SHADOW_NONE") == 0)
        type = GTK_SHADOW_NONE;
    else if(strcmp(Argv[1].strptr,"GTK_SHADOW_IN") == 0)
        type = GTK_SHADOW_IN;
    else if(strcmp(Argv[1].strptr,"GTK_SHADOW_OUT") == 0)
        type = GTK_SHADOW_OUT;
    else if(strcmp(Argv[1].strptr,"GTK_SHADOW_ETCHED_IN") == 0)
        type = GTK_SHADOW_ETCHED_IN;
    else if(strcmp(Argv[1].strptr,"GTK_SHADOW_ETCHED_OUT") == 0)
        type = GTK_SHADOW_ETCHED_OUT;
    else 
        return RXFUNC_BADCALL;

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_handle_box_set_shadow_type(GTK_HANDLE_BOX(myWidget), type);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHandleBoxSetHandlePosition                      */
/* Description: Set the position type of the handle box.                      */
/* Rexx Args:   Pointer to the widget                                         */
/*              Position type                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHandleBoxSetHandlePosition(const char * Name,
                                              const size_t Argc, const RXSTRING Argv[],
                                              const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkPositionType type;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    if(strcmp(Argv[1].strptr,"GTK_POS_LEFT") == 0)
        type = GTK_POS_LEFT;
    else if(strcmp(Argv[1].strptr,"GTK_POS_RIGHT") == 0)
        type = GTK_POS_RIGHT;
    else if(strcmp(Argv[1].strptr,"GTK_POS_TOP") == 0)
        type = GTK_POS_TOP;
    else if(strcmp(Argv[1].strptr,"GTK_POS_BOTTOM") == 0)
        type = GTK_POS_BOTTOM;
    else 
        return RXFUNC_BADCALL;

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(myWidget), type);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHandleBoxSetSnapEdge                            */
/* Description: Set the snap edge type of the handle box.                     */
/* Rexx Args:   Pointer to the widget                                         */
/*              Position type                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHandleBoxSetSnapEdge(const char * Name,
                                        const size_t Argc, const RXSTRING Argv[],
                                        const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkPositionType type;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    if(strcmp(Argv[1].strptr,"GTK_POS_LEFT") == 0)
        type = GTK_POS_LEFT;
    else if(strcmp(Argv[1].strptr,"GTK_POS_RIGHT") == 0)
        type = GTK_POS_RIGHT;
    else if(strcmp(Argv[1].strptr,"GTK_POS_TOP") == 0)
        type = GTK_POS_TOP;
    else if(strcmp(Argv[1].strptr,"GTK_POS_BOTTOM") == 0)
        type = GTK_POS_BOTTOM;
    else 
        return RXFUNC_BADCALL;

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_handle_box_set_snap_edge(GTK_HANDLE_BOX(myWidget), type);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHandleBoxConnectSignal                          */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHandleBoxConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "child_attached") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "child-attached",
                             G_CALLBACK(signal_func_2), "signal_child_attached");
        }
        else if (strcmp(Argv[1].strptr, "child_detached") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "child-detached",
                             G_CALLBACK(signal_func_2), "signal_child_detached");
        }
        else {
            return RXFUNC_BADCALL;
        }
    }
    else {
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

