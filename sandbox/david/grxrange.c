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
#ifdef WIN32
    sprintf(buffer, "%p %s", window, data);
#else
    snprintf(buffer, sizeof(buffer), "%p %s", window, data);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_2(GtkWidget *window,
                          GtkScrollType arg1,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %d", window, data, arg1);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %d", window, data, arg1);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_2a(GtkWidget *window,
                           gdouble arg1,
                           gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %lf", window, data, arg1);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %f", window, data, arg1);
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
/* Rexx External Function: GrxScaleSetDigits                                  */
/* Description: Set the scale digits.                                         */
/* Rexx Args:   Pointer to the hscale or vscale                               */
/*              Digits                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScaleSetDigits(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint digits;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &digits);

    gtk_scale_set_digits(GTK_SCALE(myWidget), digits);
	
    /* Set up the REXX return code */
    *Retstr->strptr = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScaleGetDigits                                  */
/* Description: Set the scale digits.                                         */
/* Rexx Args:   Pointer to the hscale or vscale                               */
/*              Digits                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScaleGetDigits(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint digits;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    digits = gtk_scale_get_digits(GTK_SCALE(myWidget));
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", digits);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScaleSetValuePos                                */
/* Description: Set the scale digits.                                         */
/* Rexx Args:   Pointer to the hscale or vscale                               */
/*              Value                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScaleSetValuePos(const char * Name,
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

    gtk_scale_set_value_pos(GTK_SCALE(myWidget), type);
	
    /* Set up the REXX return code */
    *Retstr->strptr = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxScaleGetValuePos                                */
/* Description: Get the scale digits.                                         */
/* Rexx Args:   Pointer to the hscale or vscale                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxScaleGetVlauePos(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkPositionType type;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    type = gtk_scale_get_value_pos(GTK_SCALE(myWidget));
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", type);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHScaleNew                                       */
/* Description: Create a hscale.                                              */
/* Rexx Args:   value                                                         */
/*          :   lower                                                         */
/*          :   upper                                                         */
/*          :   step increment                                                */
/*          :   page increment                                                */
/*          :   page size                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHScaleNew(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkAdjustment *adj;
    gdouble lower, upper, value, step_increment, page_increment, page_size;

    /* Check for valid arguments */
    if (GrxCheckArgs(6, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%lf", &value);
    sscanf(Argv[1].strptr, "%lf", &lower);
    sscanf(Argv[2].strptr, "%lf", &upper);
    sscanf(Argv[3].strptr, "%lf", &step_increment);
    sscanf(Argv[4].strptr, "%lf", &page_increment);
    sscanf(Argv[5].strptr, "%lf", &page_size);

    adj = (GtkAdjustment *) gtk_adjustment_new(value, lower, upper, step_increment,
                                               page_increment, page_size);
    myWidget = gtk_hscale_new(adj);
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxHScaleNewWithRange                              */
/* Description: Create a hscale.                                              */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxHScaleNewWithRange(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gdouble min, max, step;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%lf", &min);
    sscanf(Argv[1].strptr, "%lf", &max);
    sscanf(Argv[2].strptr, "%lf", &step);

    myWidget = gtk_hscale_new_with_range(min, max, step);
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxVScaleNew                                       */
/* Description: Create a vscale.                                              */
/* Rexx Args:   value                                                         */
/*          :   lower                                                         */
/*          :   upper                                                         */
/*          :   step increment                                                */
/*          :   page increment                                                */
/*          :   page size                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxVScaleNew(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkAdjustment *adj;
    gdouble lower, upper, value, step_increment, page_increment, page_size;

    /* Check for valid arguments */
    if (GrxCheckArgs(6, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%lf", &value);
    sscanf(Argv[1].strptr, "%lf", &lower);
    sscanf(Argv[2].strptr, "%lf", &upper);
    sscanf(Argv[3].strptr, "%lf", &step_increment);
    sscanf(Argv[4].strptr, "%lf", &page_increment);
    sscanf(Argv[5].strptr, "%lf", &page_size);

    adj = (GtkAdjustment *) gtk_adjustment_new(value, lower, upper, step_increment,
                                               page_increment, page_size);
    myWidget = gtk_vscale_new(adj);
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxVScaleNewWithRange                                       */
/* Description: Create a vscale.                                              */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxVScaleNewWithRange(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gdouble min, max, step;

    /* Check for valid arguments */
    if (GrxCheckArgs(6, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%lf", &min);
    sscanf(Argv[1].strptr, "%lf", &max);
    sscanf(Argv[2].strptr, "%lf", &step);

    myWidget = gtk_vscale_new_with_range(min, max, step);
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRangeConnectSignal                              */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRangeConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "adjust_bounds") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "adjust-bounds",
                             G_CALLBACK(signal_func_2a), "signal_adjust_bounds");
        }
        else if (strcmp(Argv[1].strptr, "move_slider") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "move-slider",
                             G_CALLBACK(signal_func_2), "signal_move_slider");
        }
        else if (strcmp(Argv[1].strptr, "value_changed") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "value-changed",
                             G_CALLBACK(signal_func_1), "signal_value_changed");
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

