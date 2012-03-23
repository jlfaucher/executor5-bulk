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

static void signal_func_0(GtkWidget *toolitem,
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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create an tool button.
 *
 * @param icon    The button icon
 *
 * @param label   The button label
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxToolButtonNew,          // Object_method name
            RexxObjectPtr, icon,       // Icon widget
            CSTRING, label,            // Button label text
            OSELF, self)               // Self
{
    if (!context->IsOfType(icon, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
    GtkToolItem *toolitem = gtk_tool_button_new(iconWidget, label);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(toolitem));
    g_object_set_data(G_OBJECT(toolitem), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create an tool button from stock.
 *
 * @param label   The button stock id.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxToolButtonNewFromStock, // Object_method name
            CSTRING, sid,              // Button stock id
            OSELF, self)               // Self
{
    GtkToolItem *toolitem = gtk_tool_button_new_from_stock(sid);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(toolitem));
    g_object_set_data(G_OBJECT(toolitem), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_label
 *
 * Sets/gets the button label.
 *
 * @param label   The button label
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxToolButtonGetSetLabel,  // Object_method name
            OPTIONAL_CSTRING, label,   // Button label text
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_tool_button_set_label(GTK_TOOL_BUTTON(self), label);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_tool_button_get_label(GTK_TOOL_BUTTON(self)));
    }

    return retc;
}

/**
 * Method:  set_use_underline
 *
 * Sets the accelerator key..
 *
 * @param flag    The use underline boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxToolButtonSetUseUnderline, // Object_method name
            logical_t, flag,           // Use underline boolean
            CSELF, self)               // GTK self
{
    gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(self), flag);

    return 0;
}

/**
 * Method:  get_use_underline
 *
 * Gets the use underline boolean.
 *
 * @return        Flag
 **/
RexxMethod1(logical_t,                 // Return type
            GrxToolButtonGetUseUnderline, // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_tool_button_get_use_underline(GTK_TOOL_BUTTON(self));
}

/**
 * Method:  set/get_stock_id
 *
 * Sets/gets the stock id.
 *
 * @param id      The stock id
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxToolButtonGetSetStockId, // Object_method name
            OPTIONAL_CSTRING, id,      // The stock id
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(self), id);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_tool_button_get_stock_id(GTK_TOOL_BUTTON(self)));
    }

    return 0;
}

/**
 * Method:  set/get_icon_name
 *
 * Sets/gets the icon name.
 *
 * @param name    The icon name
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxToolButtonGetSetIconName, // Object_method name
            OPTIONAL_CSTRING, name,    // The icon name
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(self), name);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_tool_button_get_icon_name(GTK_TOOL_BUTTON(self)));
    }

    return retc;
}

/**
 * Method:  set/get_icon_widget
 *
 * Sets/gets the icon widget.
 *
 * @param name    The icon widget.
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxToolButtonGetSetIconWidget, // Object_method name
            OPTIONAL_RexxObjectPtr, icon, // The icon widget
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc = (RexxObjectPtr)context->Nil();

    if (argumentExists(1)) {
        if (!context->IsOfType(icon, "GtkWidget")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkWidget"));
            return retc;
        }
        GtkWidget *iconWidget = (GtkWidget *)context->ObjectToCSelf(icon);
        gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(self), iconWidget);
    }
    else {
        GtkWidget *iconWidget = gtk_tool_button_get_icon_widget(GTK_TOOL_BUTTON(self));
        retc = (RexxObjectPtr)g_object_get_data(G_OBJECT(iconWidget), "OORXOBJECT");
    }

    return retc;
}

/**
 * Method:  set/set_label_widget
 *
 * Sets/gets the label widget.
 *
 * @param name    The label widget.
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxToolButtonGetSetLabelWidget, // Object_method name
            OPTIONAL_RexxObjectPtr, label, // The label
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc = (RexxObjectPtr)context->Nil();

    if (argumentExists(1)) {
        if (!context->IsOfType(label, "GtkWidget")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkWidget"));
            return retc;
        }
        GtkWidget *labelWidget = (GtkWidget *)context->ObjectToCSelf(label);
        gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(self), labelWidget);
    }
    else {
        GtkWidget *labelWidget = gtk_tool_button_get_label_widget(GTK_TOOL_BUTTON(self));
        retc = (RexxObjectPtr)g_object_get_data(G_OBJECT(labelWidget), "OORXOBJECT");
    }

    return retc;
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
            GrxToolButtonSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "clicked") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_clicked";
        g_signal_connect(G_OBJECT(self), "clicked",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

/**
 * Method:  init
 *
 * Create an tool button separator.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxSeparatorToolItemNew,   // Object_method name
            OSELF, self)               // Self
{
    GtkToolItem *toolitem = gtk_separator_tool_item_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(toolitem));
    g_object_set_data(G_OBJECT(toolitem), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_draw
 *
 * Sets/gets the draw flag.
 *
 * @param flag    The draw boolean
 *
 * @return        Zero.
 **/
RexxMethod2(logical_t,                 // Return type
            GrxSeparatorToolItemGetSetDraw, // Object_method name
            OPTIONAL_logical_t, flag,  // The draw boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(self), flag);
    }
    else {
        return gtk_separator_tool_item_get_draw(GTK_SEPARATOR_TOOL_ITEM(self));
    }

    return 0;
}

