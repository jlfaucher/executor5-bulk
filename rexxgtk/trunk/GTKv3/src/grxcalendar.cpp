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

static void signal_func_0(GtkCalendar *calendar,
                          GObject *arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(calendar), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a calendar.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxCalendarNew,            // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *calendar = gtk_calendar_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(calendar));
    g_object_set_data(G_OBJECT(calendar), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  select_month
 *
 * Select a month.
 *
 * @param month   The month
 *
 * @param year    The year
 *
 * @return        Boolean
 **/
RexxMethod3(logical_t,                 // Return type
            GrxCalendarSelectMonth,    // Object_method name
            uint32_t, month,           // Month
            uint32_t, year,            // Year
            CSELF, self)               // Self
{

	gtk_calendar_select_month(GTK_CALENDAR(self), month, year);
	return TRUE;
}

/**
 * Method:  select_day
 *
 * Select a day.
 *
 * @param day     The day
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxCalendarSelectDay,      // Object_method name
            uint32_t, day,             // Day
            CSELF, self)               // Self
{

    gtk_calendar_select_day(GTK_CALENDAR(self), day);

    return 0;
}

/**
 * Method:  mark_day
 *
 * Mark a day.
 *
 * @param day     The day
 *
 * @return        Boolean
 **/
RexxMethod2(logical_t,                 // Return type
            GrxCalendarMarkDay,        // Object_method name
            uint32_t, day,             // Day
            CSELF, self)               // Self
{

	gtk_calendar_mark_day(GTK_CALENDAR(self), day);
	return TRUE;
}

/**
 * Method:  unmark_day
 *
 * Unmark a day.
 *
 * @param day     The day
 *
 * @return        Boolean
 **/
RexxMethod2(logical_t,                 // Return type
            GrxCalendarUnmarkDay,      // Object_method name
            uint32_t, day,             // Day
            CSELF, self)               // Self
{

	gtk_calendar_unmark_day(GTK_CALENDAR(self), day);
	return TRUE;
}

/**
 * Method:  clear_marks
 *
 * Clear all marks.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxCalendarClearMarks,     // Object_method name
            CSELF, self)               // Self
{

    gtk_calendar_clear_marks(GTK_CALENDAR(self));

    return 0;
}

/**
 * Method:  get_display_options
 *
 * Get the display options.
 *
 * @return        Options as a single decimal number
 **/
RexxMethod1(int,                       // Return type
            GrxCalendarGetDisplayOptions, // Object_method name
            CSELF, self)               // Self
{

    return gtk_calendar_get_display_options(GTK_CALENDAR(self));
}

/**
 * Method:  set_display_options
 *
 * Set the display options.
 *
 * @param flags   Options as a single decimal number
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxCalendarSetDisplayOptions, // Object_method name
            int, flags,                // Display options
            CSELF, self)               // Self
{

    gtk_calendar_set_display_options(GTK_CALENDAR(self),
                                     (GtkCalendarDisplayOptions)flags);

    return 0;
}

/**
 * Method:  get_date
 *
 * Get the date in Rexx format date().
 *
 * @return        Zero
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxCalendarGetDate,        // Object_method name
            CSELF, self)               // Self
{
    guint year, month, day;
    char date[24];
    const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                              "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const char *smonth;

    gtk_calendar_get_date(GTK_CALENDAR(self), &year, &month, &day);
    smonth = months[month - 1];
    g_snprintf(date, sizeof(date), "%d, %s, %d", day, smonth, year);

    return context->NewStringFromAsciiz(date);
}

/**
 * Method:  signal_connect
 *
 * Connect a signal to an ooRexx method.
 *
 * @param name    The signal name
 *
 * @return        Zero
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxCalendarSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "day_selected") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_day_selected";
        g_signal_connect(G_OBJECT(self), "day_selected",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "day_selected_double_click") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_day_selected_double_click";
        g_signal_connect(G_OBJECT(self), "day_selected_double_click",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "month_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_month_changed";
        g_signal_connect(G_OBJECT(self), "month_changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "next_month") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_next_month";
        g_signal_connect(G_OBJECT(self), "next_month",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "next_year") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_next_year";
        g_signal_connect(G_OBJECT(self), "next_year",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "prev_month") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_prev_month";
        g_signal_connect(G_OBJECT(self), "prev_month",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "prev_year") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_prev_year";
        g_signal_connect(G_OBJECT(self), "prev_year",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->False();
}

