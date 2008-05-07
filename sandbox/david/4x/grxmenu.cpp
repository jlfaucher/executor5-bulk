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
                          GtkScrollType arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->NumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a menu.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxMenuNew,                // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    myWidget = gtk_menu_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  reorder_child
 *
 * Reorder a child menu.
 *
 * @param child   The child widget.
 *
 * @param pos     Position
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuReorderChild,       // Object_method name
            RexxObjectPtr, child,      // The child widget
            int, pos)                  // Position
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);
    RexxPointerObject childptr = (RexxPointerObject)context->SendMessage0(child, "POINTER");
    GtkWidget *childWidget = (GtkWidget *)context->PointerValue(childptr);

    gtk_menu_reorder_child(myWidget, childWidget, pos);

    return 0;
}

/**
 * Method:  attach
 *
 * Attach a new menu item.
 *
 * @param child   The child widget.
 *
 * @param left    Left column number
 *
 * @param right   Right column number
 *
 * @param top     Top row number
 *
 * @param bottom  Bottom row number
 *
 * @return        Zero
 **/
RexxMethod5(int,                       // Return type
            GrxMenuAttach,             // Object_method name
            RexxObjectPtr, child,      // The child widget
            int, left,                 // Left column number
            int, right,                // Right column number
            int, top,                  // Top row number
            int, bottom)               // Bottom row number
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);
    RexxPointerObject childptr = (RexxPointerObject)context->SendMessage0(child, "POINTER");
    GtkWidget *childWidget = (GtkWidget *)context->PointerValue(childptr);

    gtk_menu_attach(myWidget, childWidget, left, right, top, bottom);

    return 0;
}

/**
 * Method:  popup
 *
 * Popup a menu.
 *
 * @return        Zero
 **/
RexxMethod0(int,                       // Return type
            GrxMenuPopup)              // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_popup(myWidget, NULL, NULL, NULL, NULL, 0,
                   gtk_get_current_event_time());

    return 0;
}

/**
 * Method:  set_accel_group
 *
 * Set the accelerator group.
 *
 * @param accel   The accelerator group
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuSetAccelGroup,      // Object_method name
            RexxObjectPtr, accel)      // The accel widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);
    RexxPointerObject accelptr = (RexxPointerObject)context->SendMessage0(accel, "POINTER");
    GtkAccelGroup *accelWidget = (GtkAccelGroup *)context->PointerValue(accelptr);

    gtk_menu_set_accel_group(myWidget, accelWidget);

    return 0;
}

/**
 * Method:  get_accel_group
 *
 * Get the accelerator group.
 *
 * @return        Accel group
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxMenuGetAccelGroup)      // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    GtkAccelGroup *accel = gtk_menu_get_accel_group(myWidget);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(accel), "OORXOBJECT");
}

/**
 * Method:  set_accel_path
 *
 * Set the accelerator path.
 *
 * @param path    The accelerator path
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuSetAccelPath,       // Object_method name
            CSTRING, path)             // The accel path
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_set_accel_path(myWidget, path);

    return 0;
}

/**
 * Method:  set_title
 *
 * Set the title.
 *
 * @param title   The title
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuSetTitle,           // Object_method name
            CSTRING, title)            // The accel path
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_set_title(myWidget, title);

    return 0;
}

/**
 * Method:  get_title
 *
 * Get the title.
 *
 * @return        Zero
 **/
RexxMethod0(CSTRING,                   // Return type
            GrxMenuGetTitle)           // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    return gtk_menu_get_title(myWidget);
}

/**
 * Method:  set_tearoff_state
 *
 * Set the tearoff state.
 *
 * @param state   The state
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuSetTearoffState,    // Object_method name
            logical_t, state)          // The state boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_set_tearoff_state(myWidget, state);

    return 0;
}

/**
 * Method:  get_tearoff_state
 *
 * Get the tearoff state.
 *
 * @return        State boolean
 **/
RexxMethod0(logical_t,                 // Return type
            GrxMenuGetTearoffState)    // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    return gtk_menu_get_tearoff_state(myWidget);
}

/**
 * Method:  popdown
 *
 * Popdown the menu.
 *
 * @return        Zero
 **/
RexxMethod0(int,                       // Return type
            GrxMenuPopdown)            // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_popdown(myWidget);

    return 0;
}

/**
 * Method:  reposition
 *
 * Reposition the menu.
 *
 * @return        Zero
 **/
RexxMethod0(int,                       // Return type
            GrxMenuReposition)         // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_reposition(myWidget);

    return 0;
}

/**
 * Method:  get_active
 *
 * Get the active menu item.
 *
 * @return        Menu item
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxMenuGetActive)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    GtkWidget *act = gtk_menu_get_active(myWidget);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(act), "OORXOBJECT");
}

/**
 * Method:  set_active
 *
 * Set a menu item active.
 *
 * @param item    The item index
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuSetActive,          // Object_method name
            int, item)                 // The item index
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_set_active(myWidget, item);

    return 0;
}

/**
 * Method:  attach_to_widget
 *
 * Attach a menu to a widget.
 *
 * @param target  The target widget
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuAttachToWidget,     // Object_method name
            RexxObjectPtr, target)     // The accel widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);
    RexxPointerObject targetptr = (RexxPointerObject)context->SendMessage0(target, "POINTER");
    GtkWidget *targetWidget = (GtkWidget *)context->PointerValue(targetptr);

    gtk_menu_attach_to_widget(myWidget, targetWidget, NULL);

    return 0;
}

/**
 * Method:  detach
 *
 * Detach a menu from a widget.
 *
 * @return        Zero
 **/
RexxMethod0(int,                       // Return type
            GrxMenuDetach)             // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    gtk_menu_detach(myWidget);

    return 0;
}

/**
 * Method:  get_attach_widget
 *
 * Get the attach widget.
 *
 * @return        Menu item
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxMenuGetAttachWidget)    // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenu *myWidget = (GtkMenu *)context->PointerValue(rxptr);

    GtkWidget *att = gtk_menu_get_attach_widget(myWidget);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(att), "OORXOBJECT");
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
            GrxMenuSignalConnect,      // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args)             // The whole argument list as an array
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "move_scroll") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_scroll";
        g_signal_connect(G_OBJECT(myWidget), "move-scroll",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
}

