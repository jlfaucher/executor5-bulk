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
    RexxObjectPtr arg1type = context->NumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}

static void signal_func_1a(GtkWidget *window,
                           GtkObject *widget,
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
    RexxObjectPtr arg1type = context->NumberToObject((wholenumber_t)arg1);;
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
RexxMethod0(int,                       // Return type
            GrxWidgetNew)              // Object_method name
{
    GtkWidget *myWidget = NULL;

    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // This is an abstract class so no actual widget is created
    return 0;
}

/**
 * Method:  show
 *
 * Show the widget.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetShow)             // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_show(myWidget);

    return 0;
}

/**
 * Method:  showall
 *
 * Show the widget and all child widgets.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetShowAll)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_show_all(myWidget);

    return 0;
}

/**
 * Method:  hide
 *
 * Hide the widget and all child widgets.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetHide)             // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_hide(myWidget);

    return 0;
}

/**
 * Method:  hideall
 *
 * Hide the widget and all child widgets.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetHideAll)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_hide_all(myWidget);

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
RexxMethod2(int,                       // Return type
            GrxWidgetSetSizeRequest,   // Object_method name
            int, width,                // width of the widget
            int, height)               // height of the widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_set_size_request(myWidget, width, height);

    return 0;
}

/**
 * Method:  destroy
 *
 * Destroy the widget.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetDestroy)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_destroy(myWidget);

    return 0;
}

/**
 * Method:  grab_focus
 *
 * Set the focus to the widget.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetGrabFocus)        // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_grab_focus(myWidget);

    return 0;
}

/**
 * Method:  font=
 *
 * Set the font of the widget.
 *
 * @param font    The new font for the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetSetFont,          // Object_method name
            CSTRING, fontname)         // Font name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkStyle *style;

    style = gtk_style_copy(gtk_widget_get_style(myWidget));
    style->font_desc = pango_font_description_from_string(fontname);
    gtk_widget_set_style(myWidget, style);

    return 0;
}

/**
 * Method:  font
 *
 * Get the font of the widget.
 *
 * @return        A string with the font name.
 **/
RexxMethod0(CSTRING,                   // Return type
            GrxWidgetGetFont)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkStyle *style = NULL;

    style = gtk_widget_get_style(myWidget);
    return pango_font_description_to_string(style->font_desc);
}

/**
 * Method:  name
 *
 * Get the name of the widget.
 *
 * @return        A string with the name.
 **/
RexxMethod0(CSTRING,                   // Return type
            GrxWidgetGetName)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_widget_get_name(myWidget);
}

/**
 * Method:  name=
 *
 * Set the name of the widget.
 *
 * @param name    The new name for the widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetSetName,          // Object_method name
            CSTRING, name)             // New name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_set_name(myWidget, name);

    return 0;
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
RexxMethod1(int,                       // Return type
            GrxWidgetSetSensitive,     // Object_method name
            int, flag)                 // Sensitive flag
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_set_sensitive(myWidget, flag);

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
RexxMethod2(int,                       // Return type
            GrxWidgetModifyBG,         // Object_method name
            int, type,                 // State
            CSTRING, colorstr)         // Color string
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_widget_modify_bg(myWidget, (GtkStateType)type, &color);

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
RexxMethod2(int,                       // Return type
            GrxWidgetModifyFG,         // Object_method name
            int, type,                 // State
            CSTRING, colorstr)         // Color string
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_widget_modify_fg(myWidget, (GtkStateType)type, &color);

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
RexxMethod2(int,                       // Return type
            GrxWidgetSetData,          // Object_method name
            CSTRING, name,             // Association name
            CSTRING, data)             // Association data
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    // Note: The memory allocated from strdup is never recovered and
    // thus causes a memory leak.
    g_object_set_data(G_OBJECT(myWidget), name, strdup(data));

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
RexxMethod1(CSTRING,                   // Return type
            GrxWidgetGetData,          // Object_method name
            CSTRING, name)             // Association name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return (char *)g_object_get_data(G_OBJECT(myWidget), name);
}

/**
 * Method:  activate
 *
 * Activate the widget.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetActivate)         // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_activate(myWidget);

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
RexxMethod1(int,                       // Return type
            GrxWidgetReparent,         // Object_method name
            RexxObjectPtr, parent)     // New parent widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject parentptr = (RexxPointerObject)context->SendMessage0(parent, "POINTER");
    GtkWidget *parentWidget = (GtkWidget *)context->PointerValue(parentptr);

    gtk_widget_reparent(myWidget, parentWidget);

    return 0;
}

/**
 * Method:  isfocus
 *
 * Determine if the widget has the focus.
 *
 * @return        1 (if it has the focus) or zero.
 **/
RexxMethod0(logical_t,                 // Return type
            GrxWidgetIsFocus)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_widget_is_focus(myWidget);
}

/**
 * Method:  grab_default
 *
 * Cause the widget to become the default.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetGrabDefault)      // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_grab_default(myWidget);

    return 0;
}

/**
 * Method:  set_parent_window
 *
 * Set the parent window.
 *
 * @param parent  The new parent widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetSetParentWindow,  // Object_method name
            RexxObjectPtr, parent)     // New parent widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject parentptr = (RexxPointerObject)context->SendMessage0(parent, "POINTER");
    GdkWindow *parentWidget = (GdkWindow *)context->PointerValue(parentptr);

    gtk_widget_set_parent_window(myWidget, parentWidget);

    return 0;
}

/**
 * Method:  get_parent_window
 *
 * Get the parent window.
 *
 * @return        The parent window object.
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxWidgetGetParentWindow)  // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkWidget *parent;
    RexxObjectPtr parentptr;

    parent = (GtkWidget *)gtk_widget_get_parent_window(myWidget);
    if (parent == NULL) {
        return context->Nil(); 
    }
    parentptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(myWidget), "OORXOBJECT");
    if (parentptr == NULL) {
        return context->Nil(); 
    }

    return parentptr;
}

/**
 * Method:  get_toplevel
 *
 * Get the top level window.
 *
 * @return        The top level window object.
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxWidgetGetToplevel)      // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkWidget *parentWidget;
    RexxObjectPtr parentptr;

    parentWidget = gtk_widget_get_toplevel(myWidget);
    if (parentWidget == NULL) {
        return context->Nil(); 
    }
    parentptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(myWidget), "OORXOBJECT");
    if (parentptr == NULL) {
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
RexxMethod1(logical_t,                 // Return type
            GrxWidgetIsAncestor,       // Object_method name
            RexxObjectPtr, ancestor)   // Ancestor widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject ancestorptr = (RexxPointerObject)context->SendMessage0(ancestor, "POINTER");
    GtkWidget *ancestorWidget = (GtkWidget *)context->PointerValue(ancestorptr);

    return gtk_widget_is_ancestor(myWidget, ancestorWidget);
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
RexxMethod1(RexxObjectPtr,             // Return type
            GrxWidgetGetAncestor,      // Object_method name
            int, type)                 // Ancestor type
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkWidget *ancestor;
    RexxObjectPtr ancestorptr;

    ancestor = gtk_widget_get_ancestor(myWidget, type);
    if (ancestor == NULL) {
        return context->Nil(); 
    }
    ancestorptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(myWidget), "OORXOBJECT");
    if (ancestorptr == NULL) {
        return context->Nil(); 
    }

    return ancestorptr;
}

/**
 * Method:  set_direction
 *
 * Set the reading direction for the text of the widget.
 *
 * @param dir     The reading direction.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxWidgetSetDirection,     // Object_method name
            int, dir)                  // Direction
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_set_direction(myWidget, (GtkTextDirection)dir);

    return 0;
}

/**
 * Method:  get_direction
 *
 * Get the reading direction for the text of the widget.
 *
 * @return        A number representing the reading direction.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetGetDirection)     // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_widget_get_direction(myWidget);
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
RexxMethod4(int,                       // Return type
            GrxWidgetQueueDrawArea,    // Object_method name
            int, x,                    // X coordinate  
            int, y,                    // Y coordinate  
            int, width,                // Width
            int, height)               // Heaight
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_queue_draw_area(myWidget, x, y, width, height);

    return 0;
}

/**
 * Method:  reset_shapes
 *
 * Reset the widget shapes.
 *
 * @return        Zero.
 **/
RexxMethod0(int,                       // Return type
            GrxWidgetResetShapes)      // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_reset_shapes(myWidget);

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
RexxMethod2(logical_t,                 // Return type
            GrxWidgetSetScrollAdjustments, // Object_method name
            RexxObjectPtr, hadj,       // Horizontal Adjustment  
            RexxObjectPtr, vadj)       // Vertical adjustment
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject hptr = (RexxPointerObject)context->SendMessage0(hadj, "POINTER");
    GtkAdjustment *hWidget = (GtkAdjustment *)context->PointerValue(hptr);
    RexxPointerObject vptr = (RexxPointerObject)context->SendMessage0(vadj, "POINTER");
    GtkAdjustment *vWidget = (GtkAdjustment *)context->PointerValue(vptr);

    return (logical_t)gtk_widget_set_scroll_adjustments(myWidget, hWidget, vWidget);
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
RexxMethod1(logical_t,                 // Return type
            GrxWidgetMnemonicActivate, // Object_method name
            logical_t, flag)           // Group cycling flag
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return (logical_t)gtk_widget_mnemonic_activate(myWidget, flag);
}

/**
 * Method:  get_accessible
 *
 * Get the widget accessible object.
 *
 * @return        Accessible object
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxWidgetGetAccessible)    // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    AtkObject *atk;
    RexxObjectPtr atkptr;

    atk = gtk_widget_get_accessible(myWidget);
    if (atk == NULL) {
        return context->Nil(); 
    }
    atkptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(myWidget), "OORXOBJECT");
    if (atkptr == NULL) {
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
RexxMethod0(logical_t,                 // Return type
            GrxWidgetGetChildVisible)  // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return (logical_t)gtk_widget_get_child_visible(myWidget);
}

/**
 * Method:  get_parent
 *
 * Return the parent widget.
 *
 * @return        Parent widget object
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxWidgetGetParent)        // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkWidget *root;
    RexxObjectPtr rootptr;

    root = gtk_widget_get_parent(myWidget);
    if (root == NULL) {
        return context->Nil(); 
    }
    rootptr = (RexxObjectPtr)g_object_get_data(G_OBJECT(myWidget), "OORXOBJECT");
    if (rootptr == NULL) {
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
RexxMethod0(RexxObjectPtr,              // Return type
            GrxWidgetGetMenus)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxObjectPtr entry;

    GList *list = gtk_menu_get_for_attach_widget(myWidget);
    RexxArrayObject arr = context->NewArray(g_list_length(list));
    list = g_list_first(list);
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
RexxMethod1(int,                        // Return type
            GrxWidgetSetEvents,         // Object_method name
            int, events)                // Event mask
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_set_events(myWidget, events);

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
RexxMethod5(int,                       // Return type
            GrxWidgetAddAccelerator,   // Object_method name
            CSTRING, signame,          // Accelerator signal name
            RexxObjectPtr, accelgrp,   // Accelerator group
            int, key,                  // Accelerator key
            int, mods,                 // Accelerator modifiers
            int, flags)                // Accelerator flags
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject grpptr = (RexxPointerObject)context->SendMessage0(accelgrp, "POINTER");
    GtkAccelGroup *grpWidget = (GtkAccelGroup *)context->PointerValue(grpptr);

    gtk_widget_add_accelerator(myWidget, signame, grpWidget, key, 
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
RexxMethod3(int,                       // Return type
            GrxWidgetRemoveAccelerator, // Object_method name
            RexxObjectPtr, accelgrp,   // Accelerator group
            int, key,                  // Accelerator key
            int, mods)                 // Accelerator modifiers
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject grpptr = (RexxPointerObject)context->SendMessage0(accelgrp, "POINTER");
    GtkAccelGroup *grpWidget = (GtkAccelGroup *)context->PointerValue(grpptr);

    gtk_widget_remove_accelerator(myWidget, grpWidget, key, 
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
RexxMethod2(int,                       // Return type
            GrxWidgetSetAccelPath,     // Object_method name
            CSTRING, path,             // Accelerator path
            RexxObjectPtr, accelgrp)   // Accelerator group
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject grpptr = (RexxPointerObject)context->SendMessage0(accelgrp, "POINTER");
    GtkAccelGroup *grpWidget = (GtkAccelGroup *)context->PointerValue(grpptr);

    gtk_widget_set_accel_path(myWidget, path, grpWidget);

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
            ARGLIST, args)             // The whole argument list as an array
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "destroy") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_destroy";
        g_signal_connect(G_OBJECT(myWidget), "destroy",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "unrealize") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_unrealize";
        g_signal_connect(G_OBJECT(myWidget), "unrealize",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "unmap") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_unmap";
        g_signal_connect(G_OBJECT(myWidget), "unmap",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "show") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_show";
        g_signal_connect(G_OBJECT(myWidget), "show",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "realize") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_realize";
        g_signal_connect(G_OBJECT(myWidget), "realize",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "popup_menu") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_popup_menu";
        g_signal_connect(G_OBJECT(myWidget), "popup-menu",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "map") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_map";
        g_signal_connect(G_OBJECT(myWidget), "map",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "hide") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_hide";
        g_signal_connect(G_OBJECT(myWidget), "hide",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "grab_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_grab_focus";
        g_signal_connect(G_OBJECT(myWidget), "grab-focus",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "accel_closures_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_accel_closures_changed";
        g_signal_connect(G_OBJECT(myWidget), "accel-closures-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "composited_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_composited_changed";
        g_signal_connect(G_OBJECT(myWidget), "composited-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "state_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_state_changed";
        g_signal_connect(G_OBJECT(myWidget), "state-changed",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "keynav_failed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_keynav_failed";
        g_signal_connect(G_OBJECT(myWidget), "keynav-failed",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_focus";
        g_signal_connect(G_OBJECT(myWidget), "move-focus",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus";
        g_signal_connect(G_OBJECT(myWidget), "focus",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "parent_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_parent_set";
        g_signal_connect(G_OBJECT(myWidget), "parent-set",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "hiearchy_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_hiearchy_changed";
        g_signal_connect(G_OBJECT(myWidget), "hiearchy_changed",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "mnemonic_activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_mnemonic_activate";
        g_signal_connect(G_OBJECT(myWidget), "mnemonic-activate",
                         G_CALLBACK(signal_func_1b), cblock);
        return context->True();
    }
    else if (strcmp(name, "grab_notify") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_grab_notify";
        g_signal_connect(G_OBJECT(myWidget), "grab_notify",
                         G_CALLBACK(signal_func_1b), cblock);
        return context->True();
    }
    else if (strcmp(name, "button_press_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_button_press_event";
        g_signal_connect(G_OBJECT(myWidget), "button-press-event",
                         G_CALLBACK(signal_GdkEventButton), cblock);
        return context->True();
    }
    else if (strcmp(name, "button_release_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_button_release_event";
        g_signal_connect(G_OBJECT(myWidget), "button-release-event",
                         G_CALLBACK(signal_GdkEventButton), cblock);
        return context->True();
    }
    else if (strcmp(name, "client_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_client_event";
        g_signal_connect(G_OBJECT(myWidget), "client-event",
                         G_CALLBACK(signal_GdkEventClient), cblock);
        return context->True();
    }
    else if (strcmp(name, "configure_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_configure_event";
        g_signal_connect(G_OBJECT(myWidget), "configure-event",
                         G_CALLBACK(signal_GdkEventConfigure), cblock);
        return context->True();
    }
    else if (strcmp(name, "delete_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_delete_event";
        g_signal_connect(G_OBJECT(myWidget), "delete-event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "destroy_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_destroy_event";
        g_signal_connect(G_OBJECT(myWidget), "destroy-event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "enter_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_enter_notify_event";
        g_signal_connect(G_OBJECT(myWidget), "enter-notify-event",
                         G_CALLBACK(signal_GdkEventCrossing), cblock);
        return context->True();
    }
    else if (strcmp(name, "event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_event";
        g_signal_connect(G_OBJECT(myWidget), "event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "event_after") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_event_after";
        g_signal_connect(G_OBJECT(myWidget), "event-after",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "expose_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_expose_event";
        g_signal_connect(G_OBJECT(myWidget), "expose-event",
                         G_CALLBACK(signal_GdkEventExpose), cblock);
        return context->True();
    }
    else if (strcmp(name, "focus_in_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus_in_event";
        g_signal_connect(G_OBJECT(myWidget), "focus-in-event",
                         G_CALLBACK(signal_GdkEventFocus), cblock);
        return context->True();
    }
    else if (strcmp(name, "focus_out_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus_out_event";
        g_signal_connect(G_OBJECT(myWidget), "focus-out-event",
                         G_CALLBACK(signal_GdkEventFocus), cblock);
        return context->True();
    }
    else if (strcmp(name, "grab_broken_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_grab_broken_event";
        g_signal_connect(G_OBJECT(myWidget), "grab-broken-event",
                         G_CALLBACK(signal_GdkEventGrabBroken), cblock);
        return context->True();
    }
    else if (strcmp(name, "key_press_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_key_press_event";
        g_signal_connect(G_OBJECT(myWidget), "key-press-event",
                         G_CALLBACK(signal_GdkEventKey), cblock);
        return context->True();
    }
    else if (strcmp(name, "key_release_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_key_release_event";
        g_signal_connect(G_OBJECT(myWidget), "key-release-event",
                         G_CALLBACK(signal_GdkEventKey), cblock);
        return context->True();
    }
    else if (strcmp(name, "leave_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_leave_notify_event";
        g_signal_connect(G_OBJECT(myWidget), "leave-notify-event",
                         G_CALLBACK(signal_GdkEventCrossing), cblock);
        return context->True();
    }
    else if (strcmp(name, "map_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_map_event";
        g_signal_connect(G_OBJECT(myWidget), "map-event",
                         G_CALLBACK(signal_GdkEvent), cblock);
        return context->True();
    }
    else if (strcmp(name, "motion_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_motion_notify_event";
        g_signal_connect(G_OBJECT(myWidget), "motion-notify-event",
                         G_CALLBACK(signal_GdkEventMotion), cblock);
        return context->True();
    }
    else if (strcmp(name, "no_expose_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_no_expose_event";
        g_signal_connect(G_OBJECT(myWidget), "no-expose-event",
                         G_CALLBACK(signal_GdkEventNoExpose), cblock);
        return context->True();
    }
    else if (strcmp(name, "property_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_property_notify_event";
        g_signal_connect(G_OBJECT(myWidget), "property-notify-event",
                         G_CALLBACK(signal_GdkEventProperty), cblock);
        return context->True();
    }
    else if (strcmp(name, "proximity_in_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_proximity_in_event";
        g_signal_connect(G_OBJECT(myWidget), "proximity-in-event",
                         G_CALLBACK(signal_GdkEventProximity), cblock);
        return context->True();
    }
    else if (strcmp(name, "proximity_out_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_proximity_out_event";
        g_signal_connect(G_OBJECT(myWidget), "proximity-out-event",
                         G_CALLBACK(signal_GdkEventProximity), cblock);
        return context->True();
    }
    else if (strcmp(name, "scroll_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_scroll_event";
        g_signal_connect(G_OBJECT(myWidget), "scroll-event",
                         G_CALLBACK(signal_GdkEventScroll), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_clear_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_clear_event";
        g_signal_connect(G_OBJECT(myWidget), "selection-clear-event",
                         G_CALLBACK(signal_GdkEventSelection), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_notify_event";
        g_signal_connect(G_OBJECT(myWidget), "selection-notify-event",
                         G_CALLBACK(signal_GdkEventSelection), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_request_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_request_event";
        g_signal_connect(G_OBJECT(myWidget), "selection-request-event",
                         G_CALLBACK(signal_GdkEventSelection), cblock);
        return context->True();
    }
    else if (strcmp(name, "visibility_notify_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_visibility_notify_event";
        g_signal_connect(G_OBJECT(myWidget), "visibility-notify-event",
                         G_CALLBACK(signal_GdkEventVisibility), cblock);
        return context->True();
    }
    else if (strcmp(name, "window_state_event") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_window_state_event";
        g_signal_connect(G_OBJECT(myWidget), "window-state-event",
                         G_CALLBACK(signal_GdkEventWindowState), cblock);
        return context->True();
    }
    return context->False();
}

