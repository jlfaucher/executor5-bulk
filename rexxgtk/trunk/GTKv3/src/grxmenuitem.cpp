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
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
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
            OPTIONAL_CSTRING, label,   // Menu label
            OPTIONAL_logical_t, mflag, // Mnemonic flag
            OSELF, self)               // Self
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

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_right_justified
 *
 * Set/get the right justification flag.
 *
 * @param state   The right justification state
 *
 * @return        Zero
 **/
RexxMethod2(logical_t,                 // Return type
            GrxMenuItemGetSetRightJustified, // Object_method name
            OPTIONAL_logical_t, state, // The state boolean
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_menu_item_set_right_justified(GTK_MENU_ITEM(self), state);
    }
    else {
        return gtk_menu_item_get_right_justified(GTK_MENU_ITEM(self));
    }

    return 0;
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
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(subm, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *subWidget = (GtkWidget *)context->ObjectToCSelf(subm);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(self), subWidget);

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
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,12,0)
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(self),NULL);
#else
    gtk_menu_item_remove_submenu(GTK_MENU_ITEM(self));
#endif // GTK_CHECK_VERSION

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
            CSELF, self)               // GTK self
{
    gtk_menu_item_set_accel_path(GTK_MENU_ITEM(self), path);

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
            CSELF, self)               // GTK self
{
    gtk_menu_item_select(GTK_MENU_ITEM(self));

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
            CSELF, self)               // GTK self
{
    gtk_menu_item_deselect(GTK_MENU_ITEM(self));

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
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate";
        g_signal_connect(G_OBJECT(self), "activate",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "activate_item") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate_item";
        g_signal_connect(G_OBJECT(self), "activate-item",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_size_allocate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_size_allocate";
        g_signal_connect(G_OBJECT(self), "toggle-size-allocate",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_size_request") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_size_request";
        g_signal_connect(G_OBJECT(self), "toggle-size-request",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
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
    GtkWidget *myWidget = gtk_separator_menu_item_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a menu item.
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxImageMenuItemNew,       // Object_method name
            CSTRING, label,            // Menu label
            OPTIONAL_int, flag,        // Menu flag
            OPTIONAL_RexxObjectPtr, accelgrp,
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    if (flag == 0) {
        myWidget = gtk_image_menu_item_new_with_label(label);
    }
    else if (flag == 1) {
        myWidget = gtk_image_menu_item_new_with_mnemonic(label);
    }
    else {
        if (!context->IsOfType(accelgrp, "GtkAccelGroup")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkAccelGroup"));
            return 0;
        }
        GtkAccelGroup *accelgrpWidget = (GtkAccelGroup *)context->ObjectToCSelf(accelgrp);
        myWidget = gtk_image_menu_item_new_from_stock(label, accelgrpWidget);
    }

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

