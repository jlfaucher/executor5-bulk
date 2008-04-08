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

    cblock->context->SendMessage0(GrxDBFindObject(widget), ((cbcb *)data)->signal_name);
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
 */
RexxMethod0(int,                       // Return type
            GrxWidgetNew)              // Object_method name
{
    GtkWidget *myWidget = NULL;

    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    context->SetObjectVariable("!ACCESS_OBJ", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);
    return 0;
}

/**
 * Method:  show
 *
 * Show the widget.
 *
 * @return        Zero.
 */
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
 */
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
 */
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
 */
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
 */
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
 */
RexxMethod0(int,                       // Return type
            GrxWidgetDestroy)          // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_widget_destroy(myWidget);
    rxptr = NULL;
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    GrxDBRemoveObject(myWidget);

    return 0;
}

/**
 * Method:  grab_focus
 *
 * Set the focus to the widget.
 *
 * @return        Zero.
 */
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
 */
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
 */
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
 */
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
 */
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
 */
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
 * Method:  modifyFG
 *
 * Modify the forground color of the widget.
 *
 * @param type    A number representing the state of the widget 
 *                to modify (normal, active, etc)
 *
 * @param color   A sting representing the color
 *
 * @return        Zero.
 */
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
 * Method:  modifyBG
 *
 * Modify the background color of the widget.
 *
 * @param type    A number representing the state of the widget 
 *                to modify (normal, active, etc)
 *
 * @param color   A sting representing the color
 *
 * @return        Zero.
 */
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
 */
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
 */
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
 */
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
 */
RexxMethod1(int,                       // Return type
            GrxWidgetReparent,         // Object_method name
            RexxObjectPtr, parent)     // New parent widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxObjectPtr tparentptr = context->functions->GetObjectVariable(parent, "!POINTER");
    RexxPointerObject parentptr = (RexxPointerObject)tparentptr;
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
 */
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
 */
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
 */
RexxMethod1(int,                       // Return type
            GrxWidgetSetParentWindow,  // Object_method name
            RexxObjectPtr, parent)     // New parent widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject parentptr = (RexxPointerObject)context->functions->GetObjectVariable(parent, "!POINTER");
    GtkWidget *parentWidget = (GtkWidget *)context->PointerValue(parentptr);

    gtk_widget_set_parent_window(myWidget, parentWidget);

    return 0;
}

/**
 * Method:  get_parent_window
 *
 * Get the parent window.
 *
 * @return        The parent window object.
 */
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
    parentptr = GrxDBFindObject(myWidget);
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
 */
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
    parentptr = GrxDBFindObject(parentWidget);
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
 */
RexxMethod1(logical_t,                 // Return type
            GrxWidgetIsAncestor,       // Object_method name
            RexxObjectPtr, ancestor)   // Ancestor widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject ancestorptr = (RexxPointerObject)context->functions->GetObjectVariable(ancestor, "!POINTER");
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
 */
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
    ancestorptr = GrxDBFindObject(ancestor);
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
 */
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
 */
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
 */
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
 */
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
 */
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
 */
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
 */
RexxMethod2(logical_t,                 // Return type
            GrxWidgetSetScrollAdjustments, // Object_method name
            RexxObjectPtr, hadj,       // Horizontal Adjustment  
            RexxObjectPtr, vadj)       // Vertical adjustment
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject hptr = (RexxPointerObject)context->functions->GetObjectVariable(hadj, "!POINTER");
    GtkAdjustment *hWidget = (GtkAdjustment *)context->PointerValue(hptr);
    RexxPointerObject vptr = (RexxPointerObject)context->functions->GetObjectVariable(vadj, "!POINTER");
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
 */
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
 */
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
    atkptr = GrxDBFindObject((GtkWidget *)atk);
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
 */
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
 */
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
    rootptr = GrxDBFindObject(root);
    if (rootptr == NULL) {
        return context->Nil(); 
    }

    return rootptr;
}

/**
 * Method:  signal_connect
 *
 * Connect a signal to an ooRexx method.
 *
 * @param name    The signal name
 *
 * @return        Zero
 */
RexxMethod1(int,                       // Return type
            GrxWidgetSignalConnect,    // Object_method name
            CSTRING, name)             // Signal name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "destroy") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->context = context->threadContext;
        cblock->signal_name = "signal_destroy";
        g_signal_connect(G_OBJECT(myWidget), "destroy",
                         G_CALLBACK(signal_func_0), cblock);
        GrxDBRemoveObject(myWidget);
    }
    else {
        return 0;
    }

    return 0;
}

