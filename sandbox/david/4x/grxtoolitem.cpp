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
    context->ObjectToNumber(tempobj, &retc);
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
    tempobj = context->NewStringFromAsciiz(arg2);;
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->NewStringFromAsciiz(arg3);;
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->ObjectToNumber(tempobj, &retc);
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
    GtkToolItem *toolitem;

    toolitem = gtk_tool_item_new();

    context->SetObjectVariable("!POINTER", context->NewPointer(toolitem));
    g_object_set_data(G_OBJECT(toolitem), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_homogeneous
 *
 * Set the item homogeneous.
 *
 * @param homo    The homogeneous boolean
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemSetHomogemeous, // Object_method name
            logical_t, homo)           // Homogeneous boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    gtk_tool_item_set_homogeneous(myWidget, homo);

    return 0;
}

/**
 * Method:  get_homogeneous
 *
 * Get the item homogeneous.
 *
 * @return        Homogeneous boolean
 **/
RexxMethod0(logical_t,                 // Return type
            GrxToolItemGetHomogemeous) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_homogeneous(myWidget);
}

/**
 * Method:  set_expand
 *
 * Set the item expand flag.
 *
 * @param expand  The expand boolean
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemSetExpand,      // Object_method name
            logical_t, expand)         // Expand boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    gtk_tool_item_set_expand(myWidget, expand);

    return 0;
}

/**
 * Method:  get_expand
 *
 * Get the item expand.
 *
 * @return        Expand boolean
 **/
RexxMethod0(logical_t,                 // Return type
            GrxToolItemGetExoand)      // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_expand(myWidget);
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
RexxMethod3(int,                       // Return type
            GrxToolItemSetTooltip,     // Object_method name
            RexxObjectPtr, tooltip,    // The tooltips
            CSTRING, text,             // The tooltip text
            CSTRING, ptext)            // The tooltip private text
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);
    RexxPointerObject toolptr = (RexxPointerObject)context->SendMessage0(tooltip, "POINTER");
    GtkTooltips *tooltWidget = (GtkTooltips *)context->PointerValue(toolptr);

    gtk_tool_item_set_tooltip(myWidget, tooltWidget, text, ptext);

    return 0;
}

/**
 * Method:  set_use_drag_window
 *
 * Set the item use drag window flag.
 *
 * @param drag    The drag boolean
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemSetUseDragWindow, // Object_method name
            logical_t, drag)           // Drag boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    gtk_tool_item_set_use_drag_window(myWidget, drag);

    return 0;
}

/**
 * Method:  get_use_drag_window
 *
 * Get the item use drag window flag.
 *
 * @return        Flag
 **/
RexxMethod0(logical_t,                 // Return type
            GrxToolItemGetUseDragWindow) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_use_drag_window(myWidget);
}

/**
 * Method:  set_visible_horizontal
 *
 * Set the item visible horizontal flag.
 *
 * @param visible The visible boolean
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemSetVisibleHorizontal, // Object_method name
            logical_t, visible)        // Visible boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    gtk_tool_item_set_visible_horizontal(myWidget, visible);

    return 0;
}

/**
 * Method:  get_visible_horizontal
 *
 * Get the item visible horizontal flag.
 *
 * @return        Zero
 **/
RexxMethod0(logical_t,                 // Return type
            GrxToolItemGetVisibleHorizontal) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_visible_horizontal(myWidget);
}

/**
 * Method:  set_visible_vertical
 *
 * Set the item visible vertical flag.
 *
 * @param visible The visible boolean
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemSetVisibleVertical, // Object_method name
            logical_t, visible)        // Visible boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    gtk_tool_item_set_visible_vertical(myWidget, visible);

    return 0;
}

/**
 * Method:  get_visible_vertical
 *
 * Get the item visible vertical flag.
 *
 * @return        Zero
 **/
RexxMethod0(logical_t,                 // Return type
            GrxToolItemGetVisibleVertical) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_visible_vertical(myWidget);
}

/**
 * Method:  set_is_important
 *
 * Set the item is important flag.
 *
 * @param import  The import boolean
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolItemSetIsImportant, // Object_method name
            logical_t, import)         // Important boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    gtk_tool_item_set_is_important(myWidget, import);

    return 0;
}

/**
 * Method:  get_is_important
 *
 * Get the item is important flag.
 *
 * @return        Flag
 **/
RexxMethod0(logical_t,                 // Return type
            GrxToolItemGetIsImportant) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_is_important(myWidget);
}

/**
 * Method:  get_icon_size
 *
 * Get the item icon size.
 *
 * @return        Size
 **/
RexxMethod0(int,                       // Return type
            GrxToolItemGetIconSize)    // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_icon_size(myWidget);
}

/**
 * Method:  get_orientation
 *
 * Get the item orientation.
 *
 * @return        Orientation
 **/
RexxMethod0(int,                       // Return type
            GrxToolItemGetOrientation) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_orientation(myWidget);
}

/**
 * Method:  get_toolbar_style
 *
 * Get the item toolbar style.
 *
 * @return        Style
 **/
RexxMethod0(int,                       // Return type
            GrxToolItemGetToolbarStyle) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_toolbar_style(myWidget);
}

/**
 * Method:  get_relief_style
 *
 * Get the item relief style.
 *
 * @return        Style
 **/
RexxMethod0(int,                       // Return type
            GrxToolItemGetReliefStyle) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    return gtk_tool_item_get_relief_style(myWidget);
}

/**
 * Method:  retrieve_proxy_menu_item
 *
 * Get the item proxy menu item.
 *
 * @return        Iteme
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxToolItemRetriveProxyMenuItem) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);
    GtkWidget *temp;

    temp = gtk_tool_item_retrieve_proxy_menu_item(myWidget);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(temp), "OORXOBJECT");
}

/**
 * Method:  get_proxy_meny_item
 *
 * Get the item proxy menu item.
 *
 * @return        Iteme
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxToolItemGetProxyMenuItem, // Object_method name
            CSTRING, id)               // The id
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);
    GtkWidget *temp;

    temp = gtk_tool_item_get_proxy_menu_item(myWidget, id);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(temp), "OORXOBJECT");
}

/**
 * Method:  set_proxy_meny_item
 *
 * Set the item proxy menu item.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxToolItemSetProxyMenuItem, // Object_method name
            CSTRING, id,               // Item id
            RexxObjectPtr, item)       // Proxy menu itemame
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);
    RexxPointerObject itemptr = (RexxPointerObject)context->SendMessage0(item, "POINTER");
    GtkWidget *itemWidget = (GtkWidget *)context->PointerValue(itemptr);

    gtk_tool_item_set_proxy_menu_item(myWidget, id, itemWidget);

    return 0;
}

/**
 * Method:  rebuild_menu
 *
 * Rebuild the menu.
 *
 * @return        Zero
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxToolItemRebuildMenu)    // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolItem *myWidget = (GtkToolItem *)context->PointerValue(rxptr);

    gtk_tool_item_rebuild_menu(myWidget);

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
            GrxToolItemSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args)             // The whole argument list as an array
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "create_menu_proxy") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_create_menu_proxy";
        g_signal_connect(G_OBJECT(myWidget), "create-menu-proxy",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "set_tooltip") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_tooltip";
        g_signal_connect(G_OBJECT(myWidget), "set-tooltip",
                         G_CALLBACK(signal_func_3), cblock);
        return context->True();
    }
    else if (strcmp(name, "toolbar_reconfigured") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toolbar_reconfigured";
        g_signal_connect(G_OBJECT(myWidget), "toolbar-reconfigured",
                         G_CALLBACK(signal_func_0a), cblock);
        return context->True();
    }
    return context->False();
}

