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

static void signal_func_0(GtkWidget *window,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static void signal_func_1(GtkWidget *window,
                          gint arg1,
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
 * Create a menu item.
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxMenuItemNew,            // Object_method name
            OSELF, self,               // Self
            OPTIONAL_CSTRING, label,   // Menu label
            OPTIONAL_logical_t, mflag) // Mnemonic flag
{
    GtkWidget *myWidget;

    if (label == NULL) {
        myWidget = gtk_menu_item_new();
    }
    else {
        if (mflag) {
            myWidget = gtk_menu_item_new_with_mnemonic(label);
        }
        else {
            myWidget = gtk_menu_item_new_with_label(label);
        }
    }
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_right_justified
 *
 * Set the right justification flag.
 *
 * @param state   The right justification state
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuItemSetRightJustified, // Object_method name
            logical_t, state,          // The state boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkMenuItem *myWidget = (GtkMenuItem *)context->PointerValue(rxptr);

    gtk_menu_item_set_right_justified(myWidget, state);

    return 0;
}

/**
 * Method:  get_right_justified
 *
 * Get the right justification flag.
 *
 * @return        Zero
 **/
RexxMethod1(logical_t,                 // Return type
            GrxMenuItemGetRightJustified, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkMenuItem *myWidget = (GtkMenuItem *)context->PointerValue(rxptr);

    return gtk_menu_item_get_right_justified(myWidget);
}

/**
 * Method:  set_submenu
 *
 * Set a submenu.
 *
 * @param subm    The submenu
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuItemSetSubmenu,     // Object_method name
            RexxObjectPtr, subm,       // The submenu
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkMenuItem *myWidget = (GtkMenuItem *)context->PointerValue(rxptr);
    RexxPointerObject submptr = (RexxPointerObject)context->SendMessage0(subm, "POINTER");
    GtkWidget *subWidget = (GtkWidget *)context->PointerValue(submptr);

    gtk_menu_item_set_submenu(myWidget, subWidget);

    return 0;
}

/**
 * Method:  remove_submenu
 *
 * Remove a submenu.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuItemRemoveSubmenu,  // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkMenuItem *myWidget = (GtkMenuItem *)context->PointerValue(rxptr);

    gtk_menu_item_remove_submenu(myWidget);

    return 0;
}

/**
 * Method:  set_accel_path
 *
 * Set the accelerator path.
 *
 * @param path    The accel path
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuItemSetAccelPath,   // Object_method name
            CSTRING, path,             // The accel path
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkMenuItem *myWidget = (GtkMenuItem *)context->PointerValue(rxptr);

    gtk_menu_item_set_accel_path(myWidget, path);

    return 0;
}

/**
 * Method:  select
 *
 * Select the menu item.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuItemSelect,         // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkMenuItem *myWidget = (GtkMenuItem *)context->PointerValue(rxptr);

    gtk_menu_item_select(myWidget);

    return 0;
}

/**
 * Method:  deselect
 *
 * Deselect the menu item.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuItemDeselect,       // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkMenuItem *myWidget = (GtkMenuItem *)context->PointerValue(rxptr);

    gtk_menu_item_deselect(myWidget);

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
            GrxMenuItemSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate";
        g_signal_connect(G_OBJECT(myWidget), "activate",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "activate_item") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate_item";
        g_signal_connect(G_OBJECT(myWidget), "activate-item",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_size_allocate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_size_allocate";
        g_signal_connect(G_OBJECT(myWidget), "toggle-size-allocate",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_size_request") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_size_request";
        g_signal_connect(G_OBJECT(myWidget), "toggle-size-request",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
}

/**
 * Method:  init
 *
 * Create a seperator menu item.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxSeparatorMenuItemNew,   // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    myWidget = gtk_separator_menu_item_new();
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create an image menu item.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxImageMenuItemNew,       // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    myWidget = gtk_image_menu_item_new();
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create an image menu item.
 *
 * @param stockid The stock id string
 *
 * @param accelgrp The accelerator group
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxImageMenuItemNewFromStock, // Object_method name
            OSELF, self,               // Self
            CSTRING, stockid,          // Stock id string
            RexxObjectPtr, accelgrp)   // Self
{
    GtkWidget *myWidget;
    RexxPointerObject accelgrpptr = (RexxPointerObject)context->SendMessage0(accelgrp, "POINTER");
    GtkAccelGroup *accelgrpWidget = (GtkAccelGroup *)context->PointerValue(accelgrpptr);

    myWidget = gtk_image_menu_item_new_from_stock(stockid, accelgrpWidget);
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create an image menu item.
 *
 * @param text    The label text
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxImageMenuItemNewWithLabel, // Object_method name
            OSELF, self,               // Self
            CSTRING, text)             // label string
{
    GtkWidget *myWidget;

    myWidget = gtk_image_menu_item_new_with_label(text);
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create an image menu item.
 *
 * @param text    The label text
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxImageMenuItemNewWithMnemonic, // Object_method name
            OSELF, self,               // Self
            CSTRING, text)             // label string
{
    GtkWidget *myWidget;

    myWidget = gtk_image_menu_item_new_with_mnemonic(text);
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

