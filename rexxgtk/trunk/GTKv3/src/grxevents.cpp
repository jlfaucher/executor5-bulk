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

/**
 * Function: create_GdkEvent
 *
 * Handle a GdkEvent signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEvent(GtkWidget *window,
                   GdkEvent *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEvent"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventKey
 *
 * Handle a GdkEventKey signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventKey(GtkWidget *window,
                   GdkEventKey *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventKey"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "state=",
                          context->StringSizeToObject((size_t)event->state));
    context->SendMessage1(rxevent, "keyval=",
                          context->StringSizeToObject((size_t)event->keyval));
    context->SendMessage1(rxevent, "length=",
                          context->WholeNumberToObject((wholenumber_t)event->length));
    context->SendMessage1(rxevent, "string=",
                          context->NewStringFromAsciiz(event->string));
    context->SendMessage1(rxevent, "hardware_keycode=",
                          context->StringSizeToObject((size_t)event->hardware_keycode));
    context->SendMessage1(rxevent, "group=",
                          context->StringSizeToObject((size_t)event->group));
    context->SendMessage1(rxevent, "is_modifier=",
                          context->StringSizeToObject((size_t)event->is_modifier));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventButton
 *
 * Handle a GdkEventButton signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventButton(GtkWidget *window,
                   GdkEventButton *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventButton"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "x=",
                          context->DoubleToObject(event->x));
    context->SendMessage1(rxevent, "y=",
                          context->DoubleToObject(event->y));
    // TODO - fix this.
    context->SendMessage1(rxevent, "axes=", context->Nil());
    context->SendMessage1(rxevent, "state=",
                          context->StringSizeToObject((size_t)event->state));
    context->SendMessage1(rxevent, "button=",
                          context->StringSizeToObject((size_t)event->button));
    // TODO - fix this.
    context->SendMessage1(rxevent, "device=", context->Nil());
    context->SendMessage1(rxevent, "x_root=",
                          context->DoubleToObject(event->x_root));
    context->SendMessage1(rxevent, "y_root=",
                          context->DoubleToObject(event->y_root));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventScroll
 *
 * Handle a GdkEventScroll signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventScroll(GtkWidget *window,
                   GdkEventScroll *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventScroll"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "x=",
                          context->DoubleToObject(event->x));
    context->SendMessage1(rxevent, "y=",
                          context->DoubleToObject(event->y));
    context->SendMessage1(rxevent, "state=",
                          context->StringSizeToObject((size_t)event->state));
    context->SendMessage1(rxevent, "direction=",
                          context->WholeNumberToObject((wholenumber_t)event->direction));
    // TODO - fix this.
    context->SendMessage1(rxevent, "device=", context->Nil());
    context->SendMessage1(rxevent, "x_root=",
                          context->DoubleToObject(event->x_root));
    context->SendMessage1(rxevent, "y_root=",
                          context->DoubleToObject(event->y_root));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventMotion
 *
 * Handle a GdkEventMotion signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventMotion(GtkWidget *window,
                   GdkEventMotion *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventMotion"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "x=",
                          context->DoubleToObject(event->x));
    context->SendMessage1(rxevent, "y=",
                          context->DoubleToObject(event->y));
    context->SendMessage1(rxevent, "axes=",
                          context->DoubleToObject(*event->axes));
    context->SendMessage1(rxevent, "state=",
                          context->StringSizeToObject((size_t)event->state));
    context->SendMessage1(rxevent, "is_hint=",
                          context->StringSizeToObject((size_t)event->is_hint));
    // TODO - fix this.
    context->SendMessage1(rxevent, "device=", context->Nil());
    context->SendMessage1(rxevent, "x_root=",
                          context->DoubleToObject(event->x_root));
    context->SendMessage1(rxevent, "y_root=",
                          context->DoubleToObject(event->y_root));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventExpose
 *
 * Handle a GdkEventExpose signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventExpose(GtkWidget *window,
                   GdkEventExpose *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventExpose"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(temp, "x=",
                          context->WholeNumberToObject((wholenumber_t)event->area.x));
    context->SendMessage1(temp, "y=",
                          context->WholeNumberToObject((wholenumber_t)event->area.y));
    context->SendMessage1(temp, "width=",
                          context->WholeNumberToObject((wholenumber_t)event->area.width));
    context->SendMessage1(temp, "height=",
                          context->WholeNumberToObject((wholenumber_t)event->area.height));
    context->SendMessage1(rxevent, "area=", temp);
    // TODO - fix this.
    context->SendMessage1(rxevent, "region=", context->Nil());
    context->SendMessage1(rxevent, "count=",
                          context->WholeNumberToObject((wholenumber_t)event->count));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventVisibility
 *
 * Handle a GdkEventVisibility signal by invoking the Rexx
 * method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventVisibility(GtkWidget *window,
                   GdkEventVisibility *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventVisibility"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "state=",
                          context->StringSizeToObject((size_t)event->state));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventCrossing
 *
 * Handle a GdkEventCrossing signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventCrossing(GtkWidget *window,
                   GdkEventCrossing *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventCrossing"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    if (event->subwindow != NULL) {
        temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->subwindow), "OORXOBJECT");
        if (temp == NULL) {
            context->SendMessage1(rxevent, "subwindow=", context->Nil());
        }
        else {
            context->SendMessage1(rxevent, "subwindow=", temp);
        }
    }
    else {
        context->SendMessage1(rxevent, "subwindow=", context->Nil());
    }
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "x=",
                          context->DoubleToObject(event->x));
    context->SendMessage1(rxevent, "y=",
                          context->DoubleToObject(event->y));
    context->SendMessage1(rxevent, "x_root=",
                          context->DoubleToObject(event->x_root));
    context->SendMessage1(rxevent, "y_root=",
                          context->DoubleToObject(event->y_root));
    context->SendMessage1(rxevent, "mode=",
                          context->WholeNumberToObject((wholenumber_t)event->mode));
    context->SendMessage1(rxevent, "detail=",
                          context->WholeNumberToObject((wholenumber_t)event->detail));
    context->SendMessage1(rxevent, "focus=",
                          context->WholeNumberToObject((wholenumber_t)event->focus));
    context->SendMessage1(rxevent, "state=",
                          context->StringSizeToObject((size_t)event->state));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventFocus
 *
 * Handle a GdkEventFocus signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventFocus(GtkWidget *window,
                   GdkEventFocus *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventFocus"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "in=",
                          context->WholeNumberToObject((wholenumber_t)event->in));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventConfigure
 *
 * Handle a GdkEventConfigure signal by invoking the Rexx
 * method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventConfigure(GtkWidget *window,
                   GdkEventConfigure *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventConfigure"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "x=",
                          context->WholeNumberToObject((wholenumber_t)event->x));
    context->SendMessage1(rxevent, "y=",
                          context->WholeNumberToObject((wholenumber_t)event->y));
    context->SendMessage1(rxevent, "width=",
                          context->WholeNumberToObject((wholenumber_t)event->width));
    context->SendMessage1(rxevent, "height=",
                          context->WholeNumberToObject((wholenumber_t)event->height));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventProperty
 *
 * Handle a GdkEventProperty signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventProperty(GtkWidget *window,
                   GdkEventProperty *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventProperty"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "atom=",
                          context->WholeNumberToObject((wholenumber_t)event->atom));
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "state=",
                          context->StringSizeToObject((size_t)event->state));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventSelection
 *
 * Handle a GdkEventSelection signal by invoking the Rexx
 * method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventSelection(GtkWidget *window,
                   GdkEventSelection *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventSelection"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "selection=",
                          context->WholeNumberToObject((wholenumber_t)event->selection));
    context->SendMessage1(rxevent, "target=",
                          context->WholeNumberToObject((wholenumber_t)event->target));
    context->SendMessage1(rxevent, "property=",
                          context->WholeNumberToObject((wholenumber_t)event->property));
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "requestor=", context->Nil());

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventDND
 *
 * Handle a GdkEventDND signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventDND(GtkWidget *window,
                   GdkEventDND *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventDND"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    // TODO - fix this.
    context->SendMessage1(rxevent, "context=", context->Nil());
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "x_root=",
                          context->WholeNumberToObject((wholenumber_t)event->x_root));
    context->SendMessage1(rxevent, "y_root=",
                          context->WholeNumberToObject((wholenumber_t)event->y_root));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventProximity
 *
 * Handle a GdkEventProximity signal by invoking the Rexx
 * method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventProximity(GtkWidget *window,
                   GdkEventProximity *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventProximity"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "device=", context->Nil());

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventClient
 *
 * Handle a GdkEventClient signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
#if !GTK_CHECK_VERSION(3,0,0)
gboolean signal_GdkEventClient(GtkWidget *window,
                   GdkEventClient *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventClient"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "message_type=",
                          context->WholeNumberToObject((wholenumber_t)event->message_type));
    context->SendMessage1(rxevent, "data_format=",
                          context->WholeNumberToObject((wholenumber_t)event->data_format));
    RexxArrayObject temparr = context->NewArray(1);
    if (event->data_format == 8) {
        for (int i = 0; i < 20; i++) {
            context->ArrayPut(temparr,
                              context->WholeNumberToObject((wholenumber_t)event->data.b[i]),
                              i + 1);
        }
    }
    else if (event->data_format == 16) {
        for (int i = 0; i < 10; i++) {
            context->ArrayPut(temparr,
                              context->WholeNumberToObject((wholenumber_t)event->data.s[i]),
                              i + 1);
        }
    }
    else if (event->data_format == 32) {
        for (int i = 0; i < 5; i++) {
            context->ArrayPut(temparr,
                              context->WholeNumberToObject((wholenumber_t)event->data.l[i]),
                              i + 1);
        }
    }
    context->SendMessage1(rxevent, "data=", temp);

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventNoExpose
 *
 * Handle a GdkEventNoExpose signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventNoExpose(GtkWidget *window,
                   GdkEventNoExpose *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventNoExpose"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}
#endif

/**
 * Function: create_GdkEventWindowState
 *
 * Handle a GdkEventWindowState signal by invoking the Rexx
 * method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventWindowState(GtkWidget *window,
                   GdkEventWindowState *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventWindowState"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "changed_mask=",
                          context->WholeNumberToObject((wholenumber_t)event->changed_mask));
    context->SendMessage1(rxevent, "new_window_state=",
                          context->WholeNumberToObject((wholenumber_t)event->new_window_state));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventSetting
 *
 * Handle a GdkEventSetting signal by invoking the Rexx method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventSetting(GtkWidget *window,
                   GdkEventSetting *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventSetting"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "action=",
                          context->WholeNumberToObject((wholenumber_t)event->action));
    context->SendMessage1(rxevent, "name=",
                          context->NewStringFromAsciiz(event->name));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventOwnerChange
 *
 * Handle a GdkEventOwnerChange signal by invoking the Rexx
 * method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventOwnerChange(GtkWidget *window,
                   GdkEventOwnerChange *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventOwnerChange"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "owner=", context->Nil());
    context->SendMessage1(rxevent, "reason=",
                          context->WholeNumberToObject((wholenumber_t)event->reason));
    context->SendMessage1(rxevent, "selection=",
                          context->WholeNumberToObject((wholenumber_t)event->selection));
    context->SendMessage1(rxevent, "time=",
                          context->StringSizeToObject((size_t)event->time));
    context->SendMessage1(rxevent, "selection_time=",
                          context->StringSizeToObject((size_t)event->selection_time));

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

/**
 * Function: create_GdkEventGrabBroken
 *
 * Handle a GdkEventGrabBroken signal by invoking the Rexx
 * method.
 *
 * @param window  The widget pointer of the window
 *
 * @param event   The event being signaled
 *
 * @param data    The user specific data
 *
 * @return        boolean
 **/
gboolean signal_GdkEventGrabBroken(GtkWidget *window,
                   GdkEventGrabBroken *event,
                   gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr rxevent, temp;
    gboolean retc;

    cblock->instance->AttachThread(&context);

    // Create the instance
    rxevent = context->SendMessage1(rxobj, "create_event_obj",
                                    (RexxObjectPtr)context->NewStringFromAsciiz("GdkEventGrabBroken"));
    // Assign the standard event data
    context->SendMessage1(rxevent, "type=",
                          context->WholeNumberToObject((wholenumber_t)event->type));
    temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->window), "OORXOBJECT");
    if (temp == NULL) {
        context->SendMessage1(rxevent, "window=", context->Nil());
    }
    else {
        context->SendMessage1(rxevent, "window=", temp);
    }
    context->SendMessage1(rxevent, "send_event=",
                          context->WholeNumberToObject((wholenumber_t)event->send_event));
    // Assign the event specific data
    context->SendMessage1(rxevent, "keyboard=",
                          context->WholeNumberToObject((wholenumber_t)event->keyboard));
    context->SendMessage1(rxevent, "implicit=",
                          context->WholeNumberToObject((wholenumber_t)event->implicit));
    if (event->grab_window != NULL) {
        temp = (RexxObjectPtr)g_object_get_data(G_OBJECT(event->grab_window), "OORXOBJECT");
        if (temp == NULL) {
            context->SendMessage1(rxevent, "grab_window=", context->Nil());
        }
        else {
            context->SendMessage1(rxevent, "grab_window=", temp);
        }
    }
    else {
        context->SendMessage1(rxevent, "grab_window=", context->Nil());
    }

    temp = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxevent);
    context->ObjectToInt32(temp, &retc);
    context->DetachThread();
    return retc;
}

