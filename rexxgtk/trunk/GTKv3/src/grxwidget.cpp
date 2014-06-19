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
#include <errno.h>
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

static void signal_func_0(GtkWidget *widget,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static void signal_func_1(GtkWidget *window,
                          GtkStateType arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}

static void signal_func_1a(GtkWidget *window,
#if GTK_CHECK_VERSION(3,0,0)
                           GObject *widget,
#else
                           GtkObject *widget,
#endif // GTK_CHECK_VERSION
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxObjectPtr rxwidget = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxwidget);
    context->DetachThread();
    return;
}

static void signal_func_1b(GtkWidget *window,
                           gboolean arg1,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Methods                                                             */
/*============================================================================*/

/**
 * Method:  init
 *
 * Initialize the widget instance.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetNew,              // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = NULL;

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);
    return 0;
}

/**
 * Method:  set_cself
 *
 * Set the CSELF variable.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetSetCSELF,         // Object_method name
            RexxObjectPtr, self)       // GTK self
{
    context->SetObjectVariable("CSELF", self);

    return 0;
}

/**
 * Method:  show
 *
 * Show the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetShow,             // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_show(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  showall
 *
 * Show the widget and all child widgets.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetShowAll,          // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_show_all(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  hide
 *
 * Hide the widget and all child widgets.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetHide,             // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_hide(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  hideall
 *
 * Hide the widget and all child widgets.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetHideAll,          // Object_method name
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(3,0,0)
	#warning TO DO
#else
    gtk_widget_hide_all(GTK_WIDGET(self));
#endif // GTK_CHECK_VERSION

    return 0;
}

/**
 * Method:  set_size_request
 *
 * Resize the widget.
 *
 * @param width   The new width of the widget
 *
 * @param height  The new height of the widget
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxWidgetSetSizeRequest,   // Object_method name
            int, width,                // width of the widget
            int, height,               // height of the widget
            CSELF, self)               // GTK self
{
    gtk_widget_set_size_request(GTK_WIDGET(self), width, height);

    return 0;
}

/**
 * Method:  destroy
 *
 * Destroy the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetDestroy,          // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_destroy(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  grab_focus
 *
 * Set the focus to the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetGrabFocus,        // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_grab_focus(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  get/set font
 *
 * Set/set the font of the widget.
 *
 * @param font    The new font for the widget.
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxWidgetGetSetFont,       // Object_method name
            OPTIONAL_CSTRING, fontname,// Font name
            CSELF, self)               // GTK self
{
    GtkStyle *style;
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        style = gtk_style_copy(gtk_widget_get_style(GTK_WIDGET(self)));
        style->font_desc = pango_font_description_from_string(fontname);
        gtk_widget_set_style(GTK_WIDGET(self), style);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        style = gtk_widget_get_style(GTK_WIDGET(self));
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(pango_font_description_to_string(style->font_desc));
    }

    return retc;
}

/**
 * Method:  get/set name
 *
 * Get/set the name of the widget.
 *
 * @return        A string with the name.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxWidgetGetSetName,       // Object_method name
            OPTIONAL_CSTRING, name,    // New name
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_widget_set_name(GTK_WIDGET(self), name);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_widget_get_name(GTK_WIDGET(self)));
    }

    return retc;
}

/**
 * Method:  sensitive=
 *
 * Make the widget sensitive or insensitive.
 *
 * @param flag    1 or 0.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxWidgetSetSensitive,     // Object_method name
            logical_t, flag,           // Sensitive flag
            CSELF, self)               // GTK self
{
    gtk_widget_set_sensitive(GTK_WIDGET(self), flag);

    return 0;
}

/**
 * Method:  modifyBG
 *
 * Modify the forground color of the widget.
 *
 * @param type    A number representing the state of the widget
 *                to modify (normal, active, etc)
 *
 * @param color   A sting representing the color
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxWidgetModifyBG,         // Object_method name
            int, type,                 // State
            CSTRING, colorstr,         // Color string
            CSELF, self)               // GTK self
{
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_widget_modify_bg(GTK_WIDGET(self), (GtkStateType)type, &color);

    return 0;
}

/**
 * Method:  modifyFG
 *
 * Modify the background color of the widget.
 *
 * @param type    A number representing the state of the widget
 *                to modify (normal, active, etc)
 *
 * @param color   A sting representing the color
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxWidgetModifyFG,         // Object_method name
            int, type,                 // State
            CSTRING, colorstr,         // Color string
            CSELF, self)               // GTK self
{
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_widget_modify_fg(GTK_WIDGET(self), (GtkStateType)type, &color);

    return 0;
}

/**
 * Method:  data=
 *
 * Set a name value pair attached to the widget.
 *
 * @param name    The name of the object.
 *
 * @param value   The value of the object.
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxWidgetSetData,          // Object_method name
            CSTRING, name,             // Association name
            CSTRING, data,             // Association data
            CSELF, self)               // GTK self
{
    g_object_set_data(G_OBJECT(self), strdup(name), strdup(data));

    return 0;
}

/**
 * Method:  data
 *
 * Return the value of a name value pair attached to the widget.
 *
 * @param name    The name of the object.
 *
 * @return        The value of the object.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxWidgetGetData,          // Object_method name
            CSTRING, name,             // Association name
            CSELF, self)               // GTK self
{

    char *data = (char *)g_object_get_data(G_OBJECT(self), name);
    if (data == NULL) {
        return context->NewStringFromAsciiz("");
    }
    return context->NewStringFromAsciiz(data);
}

/**
 * Method:  activate
 *
 * Activate the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetActivate,         // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_activate(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  reparent
 *
 * Reparent the widget.
 *
 * @param parent  The new parent widget.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxWidgetReparent,         // Object_method name
            RexxObjectPtr, parent,     // New parent widget
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(parent, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *parentWidget = (GtkWidget *)context->ObjectToCSelf(parent);

    gtk_widget_reparent(GTK_WIDGET(self), parentWidget);

    return 0;
}

/**
 * Method:  isfocus
 *
 * Determine if the widget has the focus.
 *
 * @return        1 (if it has the focus) or zero.
 **/
RexxMethod1(logical_t,                 // Return type
            GrxWidgetIsFocus,          // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_widget_is_focus(GTK_WIDGET(self));
}

/**
 * Method:  grab_default
 *
 * Cause the widget to become the default.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetGrabDefault,      // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_grab_default(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  set/get_parent_window
 *
 * Set/get the parent window.
 *
 * @param parent  The new parent widget.
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxWidgetGetSetParentWindow, // Object_method name
            OPTIONAL_RexxObjectPtr, parent, // New parent widget
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc = (RexxObjectPtr) context->Nil();

    if (argumentExists(1)) {
        if (!context->IsOfType(parent, "GtkWindow")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkWindow"));
            return retc;
        }
        GdkWindow *parentWidget = (GdkWindow *)context->ObjectToCSelf(parent);
        gtk_widget_set_parent_window(GTK_WIDGET(self), parentWidget);
    }
    else {
        GtkWidget *parent = (GtkWidget *)gtk_widget_get_parent_window(GTK_WIDGET(self));
        if (parent == NULL) {
            return retc;
        }
        retc = (RexxObjectPtr)g_object_get_data(G_OBJECT(parent), "OORXOBJECT");
    }

    return retc;
}

/**
 * Method:  get_toplevel
 *
 * Get the top level window.
 *
 * @return        The top level window object.
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxWidgetGetToplevel,      // Object_method name
            CSELF, self)               // GTK self
{
    GtkWidget *parentWidget = gtk_widget_get_toplevel(GTK_WIDGET(self));
    if (parentWidget == NULL) {
        return context->Nil();
    }
    RexxObjectPtr parentptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(parentWidget),
                                                               "OORXOBJECT");
    if (parentptr == context->Nil()) {
        return context->Nil();
    }

    return parentptr;
}

/**
 * Method:  isancestor
 *
 * Determine if the wiget is an ancestor of another widgety.
 *
 * @param ancestor The ancestor widget.
 *
 * @return        1 or zero.
 **/
RexxMethod2(logical_t,                 // Return type
            GrxWidgetIsAncestor,       // Object_method name
            RexxObjectPtr, ancestor,   // Ancestor widget
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(ancestor, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *ancestorWidget = (GtkWidget *)context->ObjectToCSelf(ancestor);

    return gtk_widget_is_ancestor(GTK_WIDGET(self), ancestorWidget);
}

/**
 * Method:  get_ancestor
 *
 * Get the first ancestor widget of the specified type.
 *
 * @param type    The widget type.
 *
 * @return        The ancestor object or .Nil.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxWidgetGetAncestor,      // Object_method name
            int, type,                 // Ancestor type
            CSELF, self)               // GTK self
{
    GtkWidget *ancestor = gtk_widget_get_ancestor(GTK_WIDGET(self), type);
    if (ancestor == NULL) {
        return context->Nil();
    }
    RexxObjectPtr ancestorptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(self),
                                                                 "OORXOBJECT");
    if (ancestorptr == context->Nil()) {
        return context->Nil();
    }

    return ancestorptr;
}

/**
 * Method:  set/get_direction
 *
 * Set/get the reading direction for the text of the widget.
 *
 * @param dir     The reading direction.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxWidgetGetSetDirection,  // Object_method name
            OPTIONAL_int, dir,         // Direction
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_widget_set_direction(GTK_WIDGET(self), (GtkTextDirection)dir);
    }
    else {
        return gtk_widget_get_direction(GTK_WIDGET(self));
    }
    return 0;
}

/**
 * Method:  set_default_direction
 *
 * Set the default reading direction for the text of the widget.
 *
 * @param dir     The reading direction.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetSetDefaultDirection, // Object_method name
            int, dir)                  // Direction
{

    gtk_widget_set_default_direction((GtkTextDirection)dir);

    return 0;
}

/**
 * Method:  get_default_direction
 *
 * Get the default reading direction for the text of the widget.
 *
 * @return        A number representing the reading direction.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetGetDefaultDirection) // Object_method name
{

    return (int)gtk_widget_get_default_direction();
}

/**
 * Method:  queue_draw_area
 *
 * Redraw an area.
 *
 * @param x       The x coordinate
 *
 * @param y       The y coordinate
 *
 * @param width   The width of the area
 *
 * @param height  The height of the area
 *
 * @return        Zero.
 **/
RexxMethod5(int,                       // Return type
            GrxWidgetQueueDrawArea,    // Object_method name
            int, x,                    // X coordinate
            int, y,                    // Y coordinate
            int, width,                // Width
            int, height,               // Heaight
            CSELF, self)               // GTK self
{
    gtk_widget_queue_draw_area(GTK_WIDGET(self), x, y, width, height);

    return 0;
}

/**
 * Method:  reset_shapes
 *
 * Reset the widget shapes.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetResetShapes,      // Object_method name
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(3,0,0)
	#warning TODO
#else
    gtk_widget_reset_shapes(GTK_WIDGET(self));
#endif // GTK_CHECK_VERSION

    return 0;
}

/**
 * Method:  set_scroll_adjustments
 *
 * Set the adjustment settings if the widget supports
 * them.
 *
 * @param hadj    The horizontal adjustment
 *
 * @param vadj    The vertical adjustment
 *
 * @return        Boolean.
 **/
RexxMethod3(logical_t,                 // Return type
            GrxWidgetSetScrollAdjustments, // Object_method name
            RexxObjectPtr, hadj,       // Horizontal Adjustment
            RexxObjectPtr, vadj,       // Vertical adjustment
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(hadj, "GtkAdjustment")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkAdjustment"));
        return 0;
    }
    GtkAdjustment *hWidget = (GtkAdjustment *)context->ObjectToCSelf(hadj);
    if (!context->IsOfType(vadj, "GtkAdjustment")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkAdjustment"));
        return 0;
    }
    GtkAdjustment *vWidget = (GtkAdjustment *)context->ObjectToCSelf(vadj);

#if GTK_CHECK_VERSION(3,0,0)

	if(!GTK_IS_SCROLLABLE(GTK_WIDGET(self)))
		return false;

	gtk_scrollable_set_hadjustment(GTK_SCROLLABLE(self),hWidget);
	gtk_scrollable_set_vadjustment(GTK_SCROLLABLE(self),vWidget);
	return true;

#else
    return (logical_t)gtk_widget_set_scroll_adjustments(GTK_WIDGET(self),
                                                        hWidget, vWidget);
#endif // GTK_CHECK_VERSION
}

/**
 * Method:  mnemonic_activate
 *
 * Activate a widget's mnemonic.
 *
 * @param flag    1 = activate, 0 = deactivate
 *
 * @return        Boolean.
 **/
RexxMethod2(logical_t,                 // Return type
            GrxWidgetMnemonicActivate, // Object_method name
            logical_t, flag,           // Group cycling flag
            CSELF, self)               // GTK self
{
    return (logical_t)gtk_widget_mnemonic_activate(GTK_WIDGET(self), flag);
}

/**
 * Method:  get_accessible
 *
 * Get the widget accessible object.
 *
 * @return        Accessible object
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxWidgetGetAccessible,    // Object_method name
            CSELF, self)               // GTK self
{
    AtkObject *atk;
    RexxObjectPtr atkptr;

    atk = gtk_widget_get_accessible(GTK_WIDGET(self));
    if (atk == NULL) {
        return context->Nil();
    }
    atkptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(self), "OORXOBJECT");
    if (atkptr == context->Nil()) {
        return context->Nil();
    }

    return atkptr;
}

/**
 * Method:  get_child_visible
 *
 * Determine if a widgets children are visible.
 *
 * @return        Boolean
 **/
RexxMethod1(logical_t,                 // Return type
            GrxWidgetGetChildVisible,  // Object_method name
            CSELF, self)               // GTK self
{
    return (logical_t)gtk_widget_get_child_visible(GTK_WIDGET(self));
}

/**
 * Method:  get_parent
 *
 * Return the parent widget.
 *
 * @return        Parent widget object
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxWidgetGetParent,        // Object_method name
            CSELF, self)               // GTK self
{
    GtkWidget *root;
    RexxObjectPtr rootptr;

    root = gtk_widget_get_parent(GTK_WIDGET(self));
    if (root == NULL) {
        return context->Nil();
    }
    rootptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(self), "OORXOBJECT");
    if (rootptr == context->Nil()) {
        return context->Nil();
    }

    return rootptr;
}

/**
 * Method:  get_menus
 *
 * Get the list if menus attached to a widget.
 *
 * @return        Array of menu objects
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxWidgetGetMenus,         // Object_method name
            CSELF, self)               // GTK self
{
    RexxObjectPtr entry;

    GList *list = gtk_menu_get_for_attach_widget(GTK_WIDGET(self));
    RexxArrayObject arr = context->NewArray(1);
    int i = 0;
    while (list != NULL) {
        i++;
        entry = (RexxObjectPtr)g_object_get_data(G_OBJECT(list->data), "OORXOBJECT");
        context->ArrayPut(arr, entry, i);
        list = g_list_next(list);
    }
    return (RexxObjectPtr)arr;
}

/**
 * Method:  set_events
 *
 * Set the event mask.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxWidgetSetEvents,        // Object_method name
            int, events,               // Event mask
            CSELF, self)               // GTK self
{
    gtk_widget_set_events(GTK_WIDGET(self), events);

    return 0;
}

/**
 * Method:  add_accelerator
 *
 * Add an accelerator to the widget
 *
 * @param signame The accel signal name
 *
 * @param accelgrp The accelerator group
 *
 * @param key     The accel key binding
 *
 * @param mods    The accel modifiers
 *
 * @param flags   The accel flags
 *
 * @return        Zero.
 **/
RexxMethod6(int,                       // Return type
            GrxWidgetAddAccelerator,   // Object_method name
            CSTRING, signame,          // Accelerator signal name
            RexxObjectPtr, accelgrp,   // Accelerator group
            int, key,                  // Accelerator key
            int, mods,                 // Accelerator modifiers
            int, flags,                // Accelerator flags
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(accelgrp, "GtkAccelGroup")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkAccelGroup"));
        return 0;
    }
    GtkAccelGroup *grpWidget = (GtkAccelGroup *)context->ObjectToCSelf(accelgrp);

    gtk_widget_add_accelerator(GTK_WIDGET(self), signame, grpWidget, key,
                               (GdkModifierType)mods, (GtkAccelFlags)flags);

    return 0;
}

/**
 * Method:  remove_accelerator
 *
 * Remove an accelerator from the widget
 *
 * @param accelgrp The accelerator group
 *
 * @param key     The accel key binding
 *
 * @param mods    The accel modifiers
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxWidgetRemoveAccelerator, // Object_method name
            RexxObjectPtr, accelgrp,   // Accelerator group
            int, key,                  // Accelerator key
            int, mods,                 // Accelerator modifiers
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(accelgrp, "GtkAccelGroup")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkAccelGroup"));
        return 0;
    }
    GtkAccelGroup *grpWidget = (GtkAccelGroup *)context->ObjectToCSelf(accelgrp);

    gtk_widget_remove_accelerator(GTK_WIDGET(self), grpWidget, key,
                                  (GdkModifierType)mods);

    return 0;
}

/**
 * Method:  set_accel_path
 *
 * Set the path for an accelerator.
 *
 * @param accelgrp The accelerator group
 *
 * @param key     The accel key binding
 *
 * @param mods    The accel modifiers
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxWidgetSetAccelPath,     // Object_method name
            CSTRING, path,             // Accelerator path
            RexxObjectPtr, accelgrp,   // Accelerator group
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(accelgrp, "GtkAccelGroup")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkAccelGroup"));
        return 0;
    }
    GtkAccelGroup *grpWidget = (GtkAccelGroup *)context->ObjectToCSelf(accelgrp);

    gtk_widget_set_accel_path(GTK_WIDGET(self), path, grpWidget);

    return 0;
}

/**
 * Method:  realize
 *
 * Realize the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetRealize,          // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_realize(GTK_WIDGET(self));

    return 0;
}

/**
 * Method:  unrealize
 *
 * Unrealize the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetUnrealize,        // Object_method name
            CSELF, self)               // GTK self
{
    gtk_widget_unrealize(GTK_WIDGET(self));

    return 0;
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
            GrxWidgetSignalConnect,    // Object_method name
            CSTRING, name,             // Signal name
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "destroy") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_destroy";
        g_signal_connect(G_OBJECT(self), "destroy",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "unrealize") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_unrealize";
        g_signal_connect(G_OBJECT(self), "unrealize",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "unmap") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_unmap";
        g_signal_connect(G_OBJECT(self), "unmap",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "show") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_show";
        g_signal_connect(G_OBJECT(self), "show",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "realize") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_realize";
        g_signal_connect(G_OBJECT(self), "realize",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "popup_menu") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_popup_menu";
        g_signal_connect(G_OBJECT(self), "popup-menu",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "map") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_map";
        g_signal_connect(G_OBJECT(self), "map",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "hide") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_hide";
        g_signal_connect(G_OBJECT(self), "hide",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "grab_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_grab_focus";
        g_signal_connect(G_OBJECT(self), "grab-focus",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "accel_closures_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_accel_closures_changed";
        g_signal_connect(G_OBJECT(self), "accel-closures-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "composited_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_composited_changed";
        g_signal_connect(G_OBJECT(self), "composited-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "state_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_state_changed";
        g_signal_connect(G_OBJECT(self), "state-changed",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "keynav_failed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_keynav_failed";
        g_signal_connect(G_OBJECT(self), "keynav-failed",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_focus";
        g_signal_connect(G_OBJECT(self), "move-focus",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus";
        g_signal_connect(G_OBJECT(self), "focus",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "parent_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_parent_set";
        g_signal_connect(G_OBJECT(self), "parent-set",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "hiearchy_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_hiearchy_changed";
        g_signal_connect(G_OBJECT(self), "hiearchy_changed",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "mnemonic_activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_mnemonic_activate";
        g_signal_connect(G_OBJECT(self), "mnemonic-activate",
                         G_CALLBACK(signal_func_1b), cblock);
        return context->True();
    }
    else if (strcmp(name, "grab_notify") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_grab_notify";
        g_signal_connect(G_OBJECT(self), "grab_notify",
                         G_CALLBACK(signal_func_1b), cblock);
        return context->True();
    }
    else if (strcmp(name, "button_press_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_button_press_event";
        g_signal_connect(G_OBJECT(self), "button-press-event",
                         G_CALLBACK(signal_GdkEventButton), cblock);
        return context->True();
    }
    else if (strcmp(name, "button_release_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_button_release_event";
        g_signal_connect(G_OBJECT(self), "button-release-event",
                         G_CALLBACK(signal_GdkEventButton), cblock);
        return context->True();
    }
#if ! GTK_CHECK_VERSION(3,0,0)
    else if (strcmp(name, "client_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_client_event";
        g_signal_connect(G_OBJECT(self), "client-event",
                         G_CALLBACK(signal_GdkEventClient), cblock);
        return context->True();
    }
#endif
    else if (strcmp(name, "configure_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_configure_event";
        g_signal_connect(G_OBJECT(self), "configure-event",
                         G_CALLBACK(signal_GdkEventConfigure), cblock);
        return context->True();
    }
    else if (strcmp(name, "delete_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_delete_event";
        g_signal_connect(G_OBJECT(self), "delete-event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "destroy_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_destroy_event";
        g_signal_connect(G_OBJECT(self), "destroy-event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "enter_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_enter_notify_event";
        g_signal_connect(G_OBJECT(self), "enter-notify-event",
                         G_CALLBACK(signal_GdkEventCrossing), cblock);
        return context->True();
    }
    else if (strcmp(name, "event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_event";
        g_signal_connect(G_OBJECT(self), "event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "event_after") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_event_after";
        g_signal_connect(G_OBJECT(self), "event-after",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "expose_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_expose_event";
        g_signal_connect(G_OBJECT(self), "expose-event",
                         G_CALLBACK(signal_GdkEventExpose), cblock);
        return context->True();
    }
    else if (strcmp(name, "focus_in_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus_in_event";
        g_signal_connect(G_OBJECT(self), "focus-in-event",
                         G_CALLBACK(signal_GdkEventFocus), cblock);
        return context->True();
    }
    else if (strcmp(name, "focus_out_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus_out_event";
        g_signal_connect(G_OBJECT(self), "focus-out-event",
                         G_CALLBACK(signal_GdkEventFocus), cblock);
        return context->True();
    }
    else if (strcmp(name, "grab_broken_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_grab_broken_event";
        g_signal_connect(G_OBJECT(self), "grab-broken-event",
                         G_CALLBACK(signal_GdkEventGrabBroken), cblock);
        return context->True();
    }
    else if (strcmp(name, "key_press_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_key_press_event";
        g_signal_connect(G_OBJECT(self), "key-press-event",
                         G_CALLBACK(signal_GdkEventKey), cblock);
        return context->True();
    }
    else if (strcmp(name, "key_release_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_key_release_event";
        g_signal_connect(G_OBJECT(self), "key-release-event",
                         G_CALLBACK(signal_GdkEventKey), cblock);
        return context->True();
    }
    else if (strcmp(name, "leave_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_leave_notify_event";
        g_signal_connect(G_OBJECT(self), "leave-notify-event",
                         G_CALLBACK(signal_GdkEventCrossing), cblock);
        return context->True();
    }
    else if (strcmp(name, "map_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_map_event";
        g_signal_connect(G_OBJECT(self), "map-event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "motion_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_motion_notify_event";
        g_signal_connect(G_OBJECT(self), "motion-notify-event",
                         G_CALLBACK(signal_GdkEventMotion), cblock);
        return context->True();
    }
#if ! GTK_CHECK_VERSION(3,0,0)
    else if (strcmp(name, "no_expose_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_no_expose_event";
        g_signal_connect(G_OBJECT(self), "no-expose-event",
                         G_CALLBACK(signal_GdkEventNoExpose), cblock);
        return context->True();
    }
#endif // !GTK_CHECK_VERSION(3,0,0)
    else if (strcmp(name, "property_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_property_notify_event";
        g_signal_connect(G_OBJECT(self), "property-notify-event",
                         G_CALLBACK(signal_GdkEventProperty), cblock);
        return context->True();
    }
    else if (strcmp(name, "proximity_in_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_proximity_in_event";
        g_signal_connect(G_OBJECT(self), "proximity-in-event",
                         G_CALLBACK(signal_GdkEventProximity), cblock);
        return context->True();
    }
    else if (strcmp(name, "proximity_out_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_proximity_out_event";
        g_signal_connect(G_OBJECT(self), "proximity-out-event",
                         G_CALLBACK(signal_GdkEventProximity), cblock);
        return context->True();
    }
    else if (strcmp(name, "scroll_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_scroll_event";
        g_signal_connect(G_OBJECT(self), "scroll-event",
                         G_CALLBACK(signal_GdkEventScroll), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_clear_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_clear_event";
        g_signal_connect(G_OBJECT(self), "selection-clear-event",
                         G_CALLBACK(signal_GdkEventSelection), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_notify_event";
        g_signal_connect(G_OBJECT(self), "selection-notify-event",
                         G_CALLBACK(signal_GdkEventSelection), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_request_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_request_event";
        g_signal_connect(G_OBJECT(self), "selection-request-event",
                         G_CALLBACK(signal_GdkEventSelection), cblock);
        return context->True();
    }
    else if (strcmp(name, "visibility_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_visibility_notify_event";
        g_signal_connect(G_OBJECT(self), "visibility-notify-event",
                         G_CALLBACK(signal_GdkEventVisibility), cblock);
        return context->True();
    }
    else if (strcmp(name, "window_state_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_window_state_event";
        g_signal_connect(G_OBJECT(self), "window-state-event",
                         G_CALLBACK(signal_GdkEventWindowState), cblock);
        return context->True();
    }
    return context->False();
}

