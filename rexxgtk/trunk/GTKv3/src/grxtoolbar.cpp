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

static void signal_func_0(GtkWidget *toolbar,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(toolbar), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static gboolean signal_func_1a(GtkToolbar *toolbar,
                               gboolean arg1,
                               gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(toolbar), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr tempobj = context->WholeNumberToObject((wholenumber_t)arg1);;
    tempobj = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, tempobj);
    context->ObjectToInt32(tempobj, &retc);
    context->DetachThread();
    return retc;
}

static void signal_func_1b(GtkToolbar *toolbar,
                               gint arg1,
                               gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(toolbar), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr tempobj = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, tempobj);
    context->DetachThread();
    return;
}

static gboolean signal_func_3(GtkToolbar *toolbar,
                              gint arg1,
                              gint arg2,
                              gint arg3,
                              gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(toolbar), "OORXOBJECT");
    RexxThreadContext *context;
    gboolean retc;

    cblock->instance->AttachThread(&context);
    RexxArrayObject arr = context->NewArray(1);
    RexxObjectPtr tempobj = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->ArrayPut(arr, tempobj, 1);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, tempobj, 2);
    tempobj = context->WholeNumberToObject((wholenumber_t)arg3);;
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
 * Create an tool bar.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToolbarNew,             // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *toolbar = gtk_toolbar_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(toolbar));
    g_object_set_data(G_OBJECT(toolbar), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  insert
 *
 * Insert a tool item.
 *
 * @param child   The child item
 *
 * @param pos     The position
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxToolbarInsert,          // Object_method name
            RexxObjectPtr, child,      // The child item
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkToolItem")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkToolItem"));
        return 0;
    }
    GtkToolItem *childWidget = (GtkToolItem *)context->ObjectToCSelf(child);

    gtk_toolbar_insert(GTK_TOOLBAR(self), childWidget, pos);

    return 0;
}

/**
 * Method:  get_item_index
 *
 * Get an item index.
 *
 * @param child   The child item
 *
 * @return        Index
 **/
RexxMethod2(int,                       // Return type
            GrxToolbarGetItemIndex,    // Object_method name
            RexxObjectPtr, child,      // The child item
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkToolItem")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkToolItem"));
        return 0;
    }
    GtkToolItem *childWidget = (GtkToolItem *)context->ObjectToCSelf(child);

    return gtk_toolbar_get_item_index(GTK_TOOLBAR(self), childWidget);
}

/**
 * Method:  get_n_items
 *
 * Get the number of items.
 *
 * @return        Items
 **/
RexxMethod1(int,                       // Return type
            GrxToolbarGetNItems,       // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_toolbar_get_n_items(GTK_TOOLBAR(self));
}

/**
 * Method:  get_nth_item
 *
 * Get the nth item.
 *
 * @param pos     The position
 *
 * @return        Items
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxToolbarGetNthItem,      // Object_method name
            int, pos,                  // The child position
            CSELF, self)               // GTK self
{
    GtkToolItem *item;

    item = gtk_toolbar_get_nth_item(GTK_TOOLBAR(self), pos);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  get_drop_index
 *
 * Get drop index..
 *
 * @param x       The x coordinate on the toolbar
 *
 * @param y       The y coordinate on the toolbar
 *
 * @return        Index
 **/
RexxMethod3(int,                       // Return type
            GrxToolbarGetDropIndex,    // Object_method name
            int, x,                    // The x coordinate
            int, y,                    // The y coordinate
            CSELF, self)               // GTK self
{
    GtkToolItem item;

    return gtk_toolbar_get_drop_index(GTK_TOOLBAR(self), x, y);
}

/**
 * Method:  set_drop_highlight_item
 *
 * Insert a tool item.
 *
 * @param child   The child item
 *
 * @param idx     The index
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxToolbarSetDropHighlightItem, // Object_method name
            RexxObjectPtr, child,      // The child item
            int, idx,                  // Position index
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkToolItem")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkToolItem"));
        return 0;
    }
    GtkToolItem *childWidget = (GtkToolItem *)context->ObjectToCSelf(child);

    gtk_toolbar_set_drop_highlight_item(GTK_TOOLBAR(self), childWidget, idx);

    return 0;
}

/**
 * Method:  set/get_show_arrow
 *
 * Set/get to show an overflow menu.
 *
 * @param flag    Boolean flag
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToolbarGetSetShowArrow, // Object_method name
            OPTIONAL_logical_t, flag,  // Show boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_toolbar_set_show_arrow(GTK_TOOLBAR(self), flag);
    }
    else {
        return gtk_toolbar_get_show_arrow(GTK_TOOLBAR(self));
    }

    return 0;
}

/**
 * Method:  set/get_orientation
 *
 * Set/get the orientation.
 *
 * @param orient  Orientationg
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxToolbarGetSetOrientation, // Object_method name
            OPTIONAL_int, orient,      // Orientation
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,16,0)
    if (argumentExists(1)) {
        gtk_orientable_set_orientation(GTK_ORIENTABLE(self), (GtkOrientation)orient);
    }
    else {
        return (int)gtk_orientable_get_orientation(GTK_ORIENTABLE(self));
    }
    return 0;
#else
    if (argumentExists(1)) {
        gtk_toolbar_set_orientation(GTK_TOOLBAR(self), (GtkOrientation)orient);
    }
    else {
        return (int)gtk_toolbar_get_orientation(GTK_TOOLBAR(self));
    }

    return 0;
#endif // GTK_CHECK_VERSION
}

/**
 * Method:  set_tooltips
 *
 * Set to show tooltips.
 *
 * @param flag    Boolean flag
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxToolbarSetTooltips,     // Object_method name
            logical_t, flag,           // Show boolean
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,14,0)
	return -1;
#else
    gtk_toolbar_set_tooltips(GTK_TOOLBAR(self), flag);
    return 0;
#endif // GTK_CHECK_VERSION
}

/**
 * Method:  unset_icon_size
 *
 * Unset the icon size.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolbarUnsetIconSize,   // Object_method name
            CSELF, self)               // GTK self
{
    gtk_toolbar_unset_icon_size(GTK_TOOLBAR(self));

    return 0;
}

/**
 * Method:  set/get_style
 *
 * Set/get the toolbar style.
 *
 * @param style   Toolbar style
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxToolbarGetSetStyle,     // Object_method name
            OPTIONAL_int, style,       // Toolbar style
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_toolbar_set_style(GTK_TOOLBAR(self), (GtkToolbarStyle)style);
    }
    else {
        return (int)gtk_toolbar_get_style(GTK_TOOLBAR(self));
    }

    return 0;
}

/**
 * Method:  set/get_icon_size
 *
 * Set/get the toolbar icon size.
 *
 * @param size    Toolbar icon size
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxToolbarGetSetIconSize,  // Object_method name
            OPTIONAL_int, size,        // Toolbar style
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_toolbar_set_icon_size(GTK_TOOLBAR(self), (GtkIconSize)size);
    }
    else {
        return (int)gtk_toolbar_get_icon_size(GTK_TOOLBAR(self));
    }

    return 0;
}

/**
 * Method:  get_tooltips
 *
 * Get to show tooltips.
 *
 * @return        Flag
 **/
RexxMethod1(logical_t,                 // Return type
            GrxToolbarGetTooltips,     // Object_method name
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,14,0)
	return false;
#else
    return gtk_toolbar_get_tooltips(GTK_TOOLBAR(self));
#endif // GTK_CHECK_VERSION
}

/**
 * Method:  get_relief_style
 *
 * Get the relief style.
 *
 * @return        Style
 **/
RexxMethod1(int,                       // Return type
            GrxToolbarGetReliefStyle,  // Object_method name
            CSELF, self)               // GTK self
{
    return (int)gtk_toolbar_get_relief_style(GTK_TOOLBAR(self));
}

/**
 * Method:  append_item
 *
 * Append a tool item.
 *
 * @param text    Button label text
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param icon    Widget icon
 *
 * @param method  The ooRexx method name
 *
 * @return        Style
 **/
RexxMethod6(RexxObjectPtr,             // Return type
            GrxToolbarAppendItem,      // Object_method name
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(4),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,12,0)
	item = GTK_WIDGET(gtk_tool_button_new(iconWidget,text));
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(item),ttext);

	g_signal_connect(G_OBJECT(item), "clicked",
					 G_CALLBACK(signal_func_0), cblock);

	gtk_toolbar_insert(GTK_TOOLBAR(self),GTK_TOOL_ITEM(item),-1);
#else
    item = gtk_toolbar_append_item(GTK_TOOLBAR(self), text, ttext, tptext, iconWidget,
                                   GTK_SIGNAL_FUNC(signal_func_0), cblock);
#endif // GTK_CHECK_VERSION
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  prepend_item
 *
 * Prepend a tool item.
 *
 * @param text    Button label text
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param icon    Widget icon
 *
 * @param method  The ooRexx method name
 *
 * @return        Style
 **/
RexxMethod6(RexxObjectPtr,             // Return type
            GrxToolbarPrependItem,     // Object_method name
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(4),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;

#if GTK_CHECK_VERSION(2,12,0)
	item = GTK_WIDGET(gtk_tool_button_new(iconWidget,text));
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(item),ttext);

	g_signal_connect(G_OBJECT(item), "clicked",
					 G_CALLBACK(signal_func_0), cblock);

	gtk_toolbar_insert(GTK_TOOLBAR(self),GTK_TOOL_ITEM(item),0);
#else
    item = gtk_toolbar_prepend_item(GTK_TOOLBAR(self), text, ttext, tptext, iconWidget,
                                    GTK_SIGNAL_FUNC(signal_func_0), cblock);
#endif
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  insert_item
 *
 * Insert a tool item.
 *
 * @param text    Button label text
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param icon    Widget icon
 *
 * @param method  The ooRexx method name
 *
 * @param pos     The position
 *
 * @return        Style
 **/
RexxMethod7(RexxObjectPtr,             // Return type
            GrxToolbarInsertItem,      // Object_method name
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            int, pos,                  // Insert position
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(4),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;

#if GTK_CHECK_VERSION(2,12,0)
	item = GTK_WIDGET(gtk_tool_button_new(iconWidget,text));
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(item),ttext);

	g_signal_connect(G_OBJECT(item), "clicked",
					 G_CALLBACK(signal_func_0), cblock);

	gtk_toolbar_insert(GTK_TOOLBAR(self),GTK_TOOL_ITEM(item),pos);
#else
    item = gtk_toolbar_insert_item(GTK_TOOLBAR(self), text, ttext, tptext, iconWidget,
                                   GTK_SIGNAL_FUNC(signal_func_0), cblock, pos);
#endif

    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  append_space
 *
 * Append a space.
 *
 * @return        Style
 **/
RexxMethod1(int,                       // Return type
            GrxToolbarAppendSpace,     // Object_method name
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,4,0)
	gtk_toolbar_insert(GTK_TOOLBAR(self),gtk_separator_tool_item_new(),-1);
#else
    gtk_toolbar_append_space(GTK_TOOLBAR(self));
#endif // GTK_CHECK_VERSION
    return 0;
}

/**
 * Method:  prepend_space
 *
 * Prepend a space.
 *
 * @return        Style
 **/
RexxMethod1(int,                       // Return type
            GrxToolbarPrependSpace,    // Object_method name
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,4,0)
	gtk_toolbar_insert(GTK_TOOLBAR(self),gtk_separator_tool_item_new(),0);
#else
    gtk_toolbar_prepend_space(GTK_TOOLBAR(self));
#endif
    return 0;
}

/**
 * Method:  insert_space
 *
 * Insert a space.
 *
 * @param pos     The position
 *
 * @return        Style
 **/
RexxMethod2(int,                       // Return type
            GrxToolbarInsertSpace,     // Object_method name
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,4,0)
	gtk_toolbar_insert(GTK_TOOLBAR(self),gtk_separator_tool_item_new(),pos);
#else
    gtk_toolbar_insert_space(GTK_TOOLBAR(self), pos);
#endif

    return 0;
}

/**
 * Method:  append_element
 *
 * Append an element.
 *
 * @param type    Toolbar child type
 *
 * @param text    Button label text
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param icon    Widget icon
 *
 * @param method  The ooRexx method name
 *
 * @return        Style
 **/
RexxMethod8(RexxObjectPtr,             // Return type
            GrxToolbarAppendElement,   // Object_method name
            int, type,                 // Child type
            RexxObjectPtr, elem,       // Widget element
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(elem, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *elemWidget = (GtkWidget *)context->ObjectToCSelf(elem);
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(6),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,4,0)
	context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
							 context->WholeNumberToObject(6),
							 context->NewStringFromAsciiz("Deprecated"));
#else
    item = gtk_toolbar_append_element(GTK_TOOLBAR(self), (GtkToolbarChildType)type,
                                      elemWidget, text, ttext, tptext, iconWidget,
                                      GTK_SIGNAL_FUNC(signal_func_0), cblock);
#endif // GTK_CHECK_VERSION
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  prepend_element
 *
 * Prepend an element.
 *
 * @param type    Toolbar child type
 *
 * @param text    Button label text
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param icon    Widget icon
 *
 * @param method  The ooRexx method name
 *
 * @return        Style
 **/
RexxMethod8(RexxObjectPtr,             // Return type
            GrxToolbarPrependElement,  // Object_method name
            int, type,                 // Child type
            RexxObjectPtr, elem,       // Widget element
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(elem, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *elemWidget = (GtkWidget *)context->ObjectToCSelf(elem);
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(6),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,4,0)
	context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
							 context->WholeNumberToObject(6),
							 context->NewStringFromAsciiz("Deprecated"));
#else
    item = gtk_toolbar_prepend_element(GTK_TOOLBAR(self), (GtkToolbarChildType)type,
                                       elemWidget, text, ttext, tptext, iconWidget,
                                       GTK_SIGNAL_FUNC(signal_func_0), cblock);
#endif
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  insert_element
 *
 * Insert an element.
 *
 * @param type    Toolbar child type
 *
 * @param text    Button label text
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param icon    Widget icon
 *
 * @param method  The ooRexx method name
 *
 * @param pos     The position
 *
 * @return        Style
 **/
RexxMethod9(RexxObjectPtr,             // Return type
            GrxToolbarInsertElement,   // Object_method name
            int, type,                 // Child type
            RexxObjectPtr, elem,       // Widget element
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            int, pos,                  // The ooRexx method name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(elem, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *elemWidget = (GtkWidget *)context->ObjectToCSelf(elem);
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(6),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,4,0)
	context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
							 context->WholeNumberToObject(6),
							 context->NewStringFromAsciiz("Deprecated"));
#else
    item = gtk_toolbar_insert_element(GTK_TOOLBAR(self), (GtkToolbarChildType)type,
                                      elemWidget, text, ttext, tptext, iconWidget,
                                      GTK_SIGNAL_FUNC(signal_func_0), cblock, pos);
#endif
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  append_widget
 *
 * Append a widget.
 *
 * @param tool    Widget
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @return        Zero
 **/
RexxMethod8(RexxObjectPtr,             // Return type
            GrxToolbarAppendWidget,    // Object_method name
            int, type,                 // Child type
            RexxObjectPtr, tool,       // Widget icon
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(tool, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *toolWidget = (GtkWidget *)context->ObjectToCSelf(tool);
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(6),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,4,0)
	context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
							 context->WholeNumberToObject(6),
							 context->NewStringFromAsciiz("Deprecated"));
#else
    item = gtk_toolbar_append_element(GTK_TOOLBAR(self), (GtkToolbarChildType)type,
                                      toolWidget, text, ttext, tptext,
                                      iconWidget, GTK_SIGNAL_FUNC(signal_func_0),
                                      cblock);
#endif
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  prepend_widget
 *
 * Prepend a widget.
 *
 * @param tool    Widget
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @return        Zero
 **/
RexxMethod8(RexxObjectPtr,             // Return type
            GrxToolbarPrependWidget,   // Object_method name
            int, type,                 // Child type
            RexxObjectPtr, tool,       // Widget icon
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(tool, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *toolWidget = (GtkWidget *)context->ObjectToCSelf(tool);
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(6),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,4,0)
	context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
							 context->WholeNumberToObject(6),
							 context->NewStringFromAsciiz("Deprecated"));
#else
    item = gtk_toolbar_prepend_element(GTK_TOOLBAR(self), (GtkToolbarChildType)type,
                                       toolWidget, text, ttext, tptext,
                                       iconWidget, GTK_SIGNAL_FUNC(signal_func_0),
                                       cblock);
#endif
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  insert_widget
 *
 * Insert a widget.
 *
 * @param tool    Widget
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param pos     Position
 *
 * @return        Zero
 **/
RexxMethod9(RexxObjectPtr,             // Return type
            GrxToolbarInsertWidget,    // Object_method name
            int, type,                 // Child type
            RexxObjectPtr, tool,       // Widget icon
            CSTRING, text,             // Button text
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            RexxObjectPtr, icon,       // Widget icon
            CSTRING, method,           // The ooRexx method name
            int, pos,                  // Tooltips private text
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(tool, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *toolWidget = (GtkWidget *)context->ObjectToCSelf(tool);
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(6),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    cbcb *cblock;
    GtkWidget *item;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,4,0)
	context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
							 context->WholeNumberToObject(6),
							 context->NewStringFromAsciiz("Deprecated"));
#else
    item = gtk_toolbar_insert_element(GTK_TOOLBAR(self), (GtkToolbarChildType)type,
                                      toolWidget, text, ttext, tptext,
                                      iconWidget, GTK_SIGNAL_FUNC(signal_func_0),
                                      cblock, pos);
#endif
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(item), "OORXOBJECT");
}

/**
 * Method:  insert_stock
 *
 * Insert a stock item.
 *
 * @param id      Stock id
 *
 * @param ttext   Tooltips text
 *
 * @param tptext  Tooltips private text
 *
 * @param method  Method name
 *
 * @param pos     Position
 *
 * @return        Zero
 **/
RexxMethod6(RexxObjectPtr,             // Return type
            GrxToolbarInsertStock,     // Object_method name
            CSTRING, id,               // Stock id
            CSTRING, ttext,            // Tooltips text
            CSTRING, tptext,           // Tooltips private text
            CSTRING, method,           // The ooRexx method name
            int, pos,                  // Tooltips private text
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    cblock = (cbcb *)malloc(sizeof(cbcb));
    cblock->instance = context->threadContext->instance;
    cblock->signal_name = method;
#if GTK_CHECK_VERSION(2,4,0)
	GtkWidget *item = GTK_WIDGET(gtk_tool_button_new_from_stock(id));
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(item),ttext);

	g_signal_connect(G_OBJECT(item), "clicked",
					 G_CALLBACK(signal_func_0), cblock);

	gtk_toolbar_insert(GTK_TOOLBAR(self),GTK_TOOL_ITEM(item),-1);
#else
    gtk_toolbar_insert_stock(GTK_TOOLBAR(self), id, ttext, tptext,
                             GTK_SIGNAL_FUNC(signal_func_0), cblock, pos);
#endif

    return 0;
}

/**
 * Method:  remove_space
 *
 * Remove a space.
 *
 * @param pos     Position
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxToolbarRemoveSpace,     // Object_method name
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,4,0)
	return -1;
#else
    gtk_toolbar_remove_space(GTK_TOOLBAR(self), pos);
    return 0;
#endif // GTK_CHECK_VERSION

}

/**
 * Method:  unset_style
 *
 * Unset the toolbar style.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxToolbarUnsetStyle,      // Object_method name
            CSELF, self)               // GTK self
{
    gtk_toolbar_unset_style(GTK_TOOLBAR(self));

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
            GrxToolbarSignalConnect,   // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "focus_home_or_end") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus_home_or_end";
        g_signal_connect(G_OBJECT(self), "focus-home-or-end",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "orientation_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_orientation_changed";
        g_signal_connect(G_OBJECT(self), "orientation-changed",
                         G_CALLBACK(signal_func_1b), cblock);
        return context->True();
    }
    else if (strcmp(name, "popup_context_menu") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_popup_context_menu";
        g_signal_connect(G_OBJECT(self), "popup-context-menu",
                         G_CALLBACK(signal_func_3), cblock);
        return context->True();
    }
    else if (strcmp(name, "style_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_style_changed";
        g_signal_connect(G_OBJECT(self), "style-changed",
                         G_CALLBACK(signal_func_1b), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

