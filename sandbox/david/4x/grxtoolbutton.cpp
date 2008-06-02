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
 * @param label   The button label
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxToolButtonNew,          // Object_method name
            OSELF, self,               // Self
            RexxObjectPtr, icon,       // Icon widget
            CSTRING, label)            // Button label text
{
    RexxPointerObject widgetptr = (RexxPointerObject)context->SendMessage0(icon, "POINTER");
    GtkWidget *iconWidget = (GtkWidget *)context->PointerValue(widgetptr);
    GtkToolItem *toolitem;

    toolitem = gtk_tool_button_new(iconWidget, label);

    context->SetObjectVariable("!POINTER", context->NewPointer(toolitem));
    g_object_set_data(G_OBJECT(toolitem), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create an tool button from stock.
 *
 * @param id      The stockid
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxToolButtonNewFronStock, // Object_method name
            OSELF, self,               // Self
            CSTRING, id)               // Button label text
{
    GtkToolItem *toolitem;

    toolitem = gtk_tool_button_new_from_stock(id);

    context->SetObjectVariable("!POINTER", context->NewPointer(toolitem));
    g_object_set_data(G_OBJECT(toolitem), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_label
 *
 * Sets the button label.
 *
 * @param label   The button label
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToolButtonSetLabel,     // Object_method name
            CSTRING, label)            // Button label text
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    gtk_tool_button_set_label(myWidget, label);

    return 0;
}

/**
 * Method:  get_label
 *
 * Gets the button label.
 *
 * @return        Label
 **/
RexxMethod0(CSTRING,                   // Return type
            GrxToolButtonGetLabel)     // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    return gtk_tool_button_get_label(myWidget);
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
RexxMethod1(int,                       // Return type
            GrxToolButtonSetUseUnderline, // Object_method name
            logical_t, flag)           // Use underline boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    gtk_tool_button_set_use_underline(myWidget, flag);

    return 0;
}

/**
 * Method:  get_use_underline
 *
 * Gets the use underline boolean.
 *
 * @return        Flag
 **/
RexxMethod0(logical_t,                 // Return type
            GrxToolButtonGetUseUnderline) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    return gtk_tool_button_get_use_underline(myWidget);
}

/**
 * Method:  set_stock_id
 *
 * Sets the stock id.
 *
 * @param id      The stock id
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToolButtonSetStockId, // Object_method name
            CSTRING, id)               // The stock id
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    gtk_tool_button_set_stock_id(myWidget, id);

    return 0;
}

/**
 * Method:  get_stock_id
 *
 * Gets the stock_id
 *
 * @return        StockID string
 **/
RexxMethod0(CSTRING,                   // Return type
            GrxToolButtonGetStockId)   // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    return gtk_tool_button_get_stock_id(myWidget);
}

/**
 * Method:  set_icon_name
 *
 * Sets the icon name.
 *
 * @param name    The icon name
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToolButtonSetIconName, // Object_method name
            CSTRING, name)             // The icon name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    gtk_tool_button_set_icon_name(myWidget, name);

    return 0;
}

/**
 * Method:  get_icon_name
 *
 * Gets the icon name.
 *
 * @return        Icon name
 **/
RexxMethod0(CSTRING,                   // Return type
            GrxToolButtonGetIconName)  // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    return gtk_tool_button_get_icon_name(myWidget);
}

/**
 * Method:  set_icon_widget
 *
 * Sets the icon widget.
 *
 * @param name    The icon widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToolButtonSetIconWidget, // Object_method name
            RexxObjectPtr, rxobj)      // The icon widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);
    RexxPointerObject widgetptr = (RexxPointerObject)context->SendMessage0(rxobj, "POINTER");
    GtkWidget *iconWidget = (GtkWidget *)context->PointerValue(widgetptr);

    gtk_tool_button_set_icon_widget(myWidget, iconWidget);

    return 0;
}

/**
 * Method:  get_icon_widget
 *
 * Gets the icon widget.
 *
 * @return        Icon widget
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxToolButtonGetIconWidget) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    GtkWidget *iconWidget = gtk_tool_button_get_icon_widget(myWidget);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(iconWidget), "OORXOBJECT");
}

/**
 * Method:  set_label_widget
 *
 * Sets the label widget.
 *
 * @param name    The label widget.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToolButtonSetLabelWidget, // Object_method name
            RexxObjectPtr, rxobj)      // The icon widget
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);
    RexxPointerObject widgetptr = (RexxPointerObject)context->SendMessage0(rxobj, "POINTER");
    GtkWidget *labelWidget = (GtkWidget *)context->PointerValue(widgetptr);

    gtk_tool_button_set_label_widget(myWidget, labelWidget);

    return 0;
}

/**
 * Method:  get_label_widget
 *
 * Gets the label widget.
 *
 * @return        Label widget
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            GrxToolButtonGetLabelWidget) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkToolButton *myWidget = (GtkToolButton *)context->PointerValue(rxptr);

    GtkWidget *labelWidget = gtk_tool_button_get_label_widget(myWidget);
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(labelWidget), "OORXOBJECT");
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
            GrxToolButtonSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args)             // The whole argument list as an array
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "clicked") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_clicked";
        g_signal_connect(G_OBJECT(myWidget), "clicked",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->False();
}

