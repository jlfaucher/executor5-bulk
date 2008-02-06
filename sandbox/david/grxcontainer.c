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

static void signal_func_1(GtkWidget *window,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s", window, data);
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_2(GtkWidget *window,
                          GtkWidget *widget,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s %p", window, data, widget);
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
/* Rexx External Function: GrxContainerAdd                                    */
/* Description: Add a widget to a container                                   */
/* Rexx Args:   Pointer to the container widget                               */
/*              Pointer to the widget to add                                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxContainerAdd(const char * Name, 
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkWidget *addWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &addWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_container_add(GTK_CONTAINER(myWidget), addWidget); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxContainerRemove                                 */
/* Description: Remove a widget from a container                              */
/* Rexx Args:   Pointer to the container widget                               */
/*              Pointer to the widget to remove                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxContainerRemove(const char * Name,
                                   const size_t Argc, const RXSTRING Argv[],
                                   const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkWidget *addWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &addWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_container_remove(GTK_CONTAINER(myWidget), addWidget); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxContainerSetBorderWidth                         */
/* Description: Set the border width of the container                         */
/* Rexx Args:   Pointer to the container widget                               */
/*              Width                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxContainerSetBorderWidth(const char * Name,
                                           const size_t Argc, const RXSTRING Argv[],
                                           const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    guint width;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &width);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_container_set_border_width(GTK_CONTAINER(myWidget), width); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxBoxPackStart                                    */
/* Description: pack a child widget                                           */
/* Rexx Args:   Pointer to the container widget                               */
/*              Pointer to the widget to pack                                 */
/*              Expand                                                        */
/*              Fill                                                          */
/*              Padding                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxBoxPackStart(const char * Name,
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkWidget *packWidget;
    gboolean expand;
    gboolean fill;
    guint padding;

    /* Check for valid arguments */
    if (GrxCheckArgs(5, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &packWidget);
    sscanf(Argv[2].strptr, "%d", &expand);
    sscanf(Argv[3].strptr, "%d", &fill);
    sscanf(Argv[4].strptr, "%d", &padding);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_box_pack_start(GTK_BOX(myWidget), packWidget, expand, fill,
                           padding); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxBoxPackEnd                                      */
/* Description: pack a child widget                                           */
/* Rexx Args:   Pointer to the container widget                               */
/*              Pointer to the widget to pack                                 */
/*              Expand                                                        */
/*              Fill                                                          */
/*              Padding                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxBoxPackEnd(const char * Name,
                              const size_t Argc, const RXSTRING Argv[],
                              const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkWidget *packWidget;
    gboolean expand;
    gboolean fill;
    guint padding;

    /* Check for valid arguments */
    if (GrxCheckArgs(5, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &packWidget);
    sscanf(Argv[2].strptr, "%d", &expand);
    sscanf(Argv[3].strptr, "%d", &fill);
    sscanf(Argv[4].strptr, "%d", &padding);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_box_pack_end(GTK_BOX(myWidget), packWidget, expand, fill, padding); 
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxVBoxNew                                         */
/* Description: Create a new vbox                                             */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxVBoxNew(const char * Name, 
                           const size_t Argc, const RXSTRING Argv[],
                           const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean homogeneous;
    gint spacing;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%d", &homogeneous);
    sscanf(Argv[1].strptr, "%d", &spacing);

    myWidget = gtk_vbox_new(homogeneous, spacing);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHBoxNew                                         */
/* Description: Create a new hbox                                             */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHBoxNew(const char * Name,
                           const size_t Argc, const RXSTRING Argv[],
                           const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean homogeneous;
    gint spacing;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%d", &homogeneous);
    sscanf(Argv[1].strptr, "%d", &spacing);

    myWidget = gtk_hbox_new(homogeneous, spacing);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAlignmentNew                                    */
/* Description: Create a new alignment                                        */
/* Rexx Args:   xalign                                                        */
/*              yalign                                                        */
/*              xscale                                                        */
/*              yscale                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAlignmentNew(const char * Name,
                                const size_t Argc, const RXSTRING Argv[],
                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gfloat xalign, yalign, xscale, yscale;

    /* Check for valid arguments */
    if (GrxCheckArgs(4, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%f", &xalign);
    sscanf(Argv[1].strptr, "%f", &yalign);
    sscanf(Argv[2].strptr, "%f", &xscale);
    sscanf(Argv[3].strptr, "%f", &yscale);

    myWidget = gtk_alignment_new(xalign, yalign, xscale, yscale);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxContainerConnectSignal                          */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxContainerConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "add") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "add",
                             G_CALLBACK(signal_func_2), "signal_add");
        }
        else if (strcmp(Argv[1].strptr, "check-resize") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "check-resize",
                             G_CALLBACK(signal_func_1), "signal_check_resize");
        }
        else if (strcmp(Argv[1].strptr, "remove") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "remove",
                             G_CALLBACK(signal_func_2), "signal_remove");
        }
        else if (strcmp(Argv[1].strptr, "set-focus-child") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "set-focus-child",
                             G_CALLBACK(signal_func_2), "signal_set_focus_child");
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

