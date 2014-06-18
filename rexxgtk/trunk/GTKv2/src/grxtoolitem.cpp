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

static gboolean signal_func_0(GtkWidget *toolitem,
                              gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(toolitem), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr tempobj = context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}

static void signal_func_0a(GtkWidget *toolitem,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(toolitem), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static gboolean signal_func_3(GtkToolbar *toolitem,
                              GtkTooltips *arg1,
                              char *arg2,
                              char *arg3,
                              gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(toolitem), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(arg1), "OORXOBJECT");
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->NewStringFromAsciiz(arg2);
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->NewStringFromAsciiz(arg3);
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an tool item.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemNew,            // Object_method name
            OSELF, self)               // Self
{
    GtkToolItem *toolitem = gtk_tool_item_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(toolitem));
    g_object_set_data(G_OBJECT(toolitem), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_homogeneous
 *
 * Set/get the item homogeneous.
 *
 * @param homo    The homogeneous boolean
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToolItemGetSetHomogeneous, // Object_method name
            OPTIONAL_logical_t, homo,  // Homogeneous boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_tool_item_set_homogeneous(GTK_TOOL_ITEM(self), homo);
    }
    else {
        return gtk_tool_item_get_homogeneous(GTK_TOOL_ITEM(self));
    }

    return 0;
}

/**
 * Method:  set/get_expand
 *
 * Set/get the item expand flag.
 *
 * @param expand  The expand boolean
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToolItemGetSetExpand,   // Object_method name
            OPTIONAL_logical_t, expand,// Expand boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_tool_item_set_expand(GTK_TOOL_ITEM(self), expand);
    }
    else {
        return gtk_tool_item_get_expand(GTK_TOOL_ITEM(self));
    }

    return 0;
}

/**
 * Method:  set_tooltip
 *
 * Set an item tooltip.
 *
 * @param tooltip The tooltip
 *
 * @param text    The tooltip text
 *
 * @param ptext   The tooltip private text
 *
 * @return        Zero
 **/
RexxMethod4(int,                       // Return type
            GrxToolItemSetTooltip,     // Object_method name
            RexxObjectPtr, tooltip,    // The tooltips
            CSTRING, text,             // The tooltip text
            CSTRING, ptext,            // The tooltip private text
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(tooltip, "GtkToolTips")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkToolTips"));
        return 0;
    }
    GtkTooltips *tooltWidget = (GtkTooltips *)context->ObjectToCSelf(tooltip);

    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(self), tooltWidget, text, ptext);

    return 0;
}

/**
 * Method:  set/get_use_drag_window
 *
 * Set/get the item use drag window flag.
 *
 * @param drag    The drag boolean
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToolItemGetSetUseDragWindow, // Object_method name
            OPTIONAL_logical_t, drag,  // Drag boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_tool_item_set_use_drag_window(GTK_TOOL_ITEM(self), drag);
    }
    else {
        return gtk_tool_item_get_use_drag_window(GTK_TOOL_ITEM(self));
    }

    return 0;
}

/**
 * Method:  set/get_visible_horizontal
 *
 * Set/get the item visible horizontal flag.
 *
 * @param visible The visible boolean
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToolItemGetSetVisibleHorizontal, // Object_method name
            OPTIONAL_logical_t, visible, // Visible boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_tool_item_set_visible_horizontal(GTK_TOOL_ITEM(self), visible);
    }
    else {
        return gtk_tool_item_get_visible_horizontal(GTK_TOOL_ITEM(self));
    }

    return 0;
}

/**
 * Method:  set/get_visible_vertical
 *
 * Set/get the item visible vertical flag.
 *
 * @param visible The visible boolean
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToolItemGetSetVisibleVertical, // Object_method name
            OPTIONAL_logical_t, visible, // Visible boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_tool_item_set_visible_vertical(GTK_TOOL_ITEM(self), visible);
    }
    else {
        return gtk_tool_item_get_visible_vertical(GTK_TOOL_ITEM(self));
    }

    return 0;
}

/**
 * Method:  set/get_is_important
 *
 * Set/get the item is important flag.
 *
 * @param import  The import boolean
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToolItemGetSetIsImportant, // Object_method name
            OPTIONAL_logical_t, import,// Important boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_tool_item_set_is_important(GTK_TOOL_ITEM(self), import);
    }
    else {
        return gtk_tool_item_get_is_important(GTK_TOOL_ITEM(self));
    }

    return 0;
}

/**
 * Method:  get_icon_size
 *
 * Get the item icon size.
 *
 * @return        Size
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemGetIconSize,    // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tool_item_get_icon_size(GTK_TOOL_ITEM(self));
}

/**
 * Method:  get_orientation
 *
 * Get the item orientation.
 *
 * @return        Orientation
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemGetOrientation, // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tool_item_get_orientation(GTK_TOOL_ITEM(self));
}

/**
 * Method:  get_toolbar_style
 *
 * Get the item toolbar style.
 *
 * @return        Style
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemGetToolbarStyle, // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tool_item_get_toolbar_style(GTK_TOOL_ITEM(self));
}

/**
 * Method:  get_relief_style
 *
 * Get the item relief style.
 *
 * @return        Style
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemGetReliefStyle, // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tool_item_get_relief_style(GTK_TOOL_ITEM(self));
}

/**
 * Method:  retrieve_proxy_menu_item
 *
 * Get the item proxy menu item.
 *
 * @return        Iteme
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxToolItemRetrieveProxyMenuItem, // Object_method name
            CSELF, self)               // GTK self
{
    GtkWidget *temp;

    temp = gtk_tool_item_retrieve_proxy_menu_item(GTK_TOOL_ITEM(self));
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(temp), "OORXOBJECT");
}

/**
 * Method:  get_proxy_meny_item
 *
 * Get the item proxy menu item.
 *
 * @return        Iteme
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxToolItemGetProxyMenuItem, // Object_method name
            CSTRING, id,               // The id
            CSELF, self)               // GTK self
{
    GtkWidget *temp;

    temp = gtk_tool_item_get_proxy_menu_item(GTK_TOOL_ITEM(self), id);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(temp), "OORXOBJECT");
}

/**
 * Method:  set_proxy_meny_item
 *
 * Set the item proxy menu item.
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxToolItemSetProxyMenuItem, // Object_method name
            CSTRING, id,               // Item id
            RexxObjectPtr, item,       // Proxy menu itemame
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(item, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *itemWidget = (GtkWidget *)context->ObjectToCSelf(item);

    gtk_tool_item_set_proxy_menu_item(GTK_TOOL_ITEM(self), id, itemWidget);

    return 0;
}

/**
 * Method:  rebuild_menu
 *
 * Rebuild the menu.
 *
 * @return        Zero
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxToolItemRebuildMenu,    // Object_method name
            CSELF, self)               // GTK self
{
    gtk_tool_item_rebuild_menu(GTK_TOOL_ITEM(self));

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
RexxMethod3(RexxObjectPtr,             // Return type
            GrxToolItemSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "create_menu_proxy") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_create_menu_proxy";
        g_signal_connect(G_OBJECT(self), "create-menu-proxy",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "set_tooltip") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_tooltip";
        g_signal_connect(G_OBJECT(self), "set-tooltip",
                         G_CALLBACK(signal_func_3), cblock);
        return context->True();
    }
    else if (strcmp(name, "toolbar_reconfigured") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toolbar_reconfigured";
        g_signal_connect(G_OBJECT(self), "toolbar-reconfigured",
                         G_CALLBACK(signal_func_0a), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

