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


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a menu bar.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxMenuBarNew,             // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_menu_bar_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  append
 *
 * Append a menu.
 *
 * @param child   The child menu.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuBarAppend,          // Object_method name
            RexxObjectPtr, child,      // The child menu
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_bar_append(GTK_MENU_BAR(self), childWidget);

    return 0;
}

/**
 * Method:  prepend
 *
 * Prepend a menu.
 *
 * @param child   The child menu.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuBarPrepend,         // Object_method name
            RexxObjectPtr, child,      // The child menu
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_bar_prepend(GTK_MENU_BAR(self), childWidget);

    return 0;
}

/**
 * Method:  insert
 *
 * Insert a menu.
 *
 * @param child   The child menu
 *
 * @param pos     The position
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxMenuBarInsert,          // Object_method name
            RexxObjectPtr, child,      // The child menu
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_bar_insert(GTK_MENU_BAR(self), childWidget, pos);

    return 0;
}

/**
 * Method:  set/get_pack_direction
 *
 * Set/get the packing direction.
 *
 * @param dir     The direction
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuBarGetSetPackDirection, // Object_method name
            OPTIONAL_int, dir,         // Position
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_menu_bar_set_pack_direction(GTK_MENU_BAR(self), (GtkPackDirection)dir);
    }
    else {
        return gtk_menu_bar_get_pack_direction(GTK_MENU_BAR(self));
    }

    return 0;
}

/**
 * Method:  set/get_child_pack_direction
 *
 * Set/get the child packing direction.
 *
 * @param dir     The direction
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuBarGetSetChildPackDirection, // Object_method name
            OPTIONAL_int, dir,         // Position
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_menu_bar_set_child_pack_direction(GTK_MENU_BAR(self), (GtkPackDirection)dir);
    }
    else {
        return gtk_menu_bar_get_child_pack_direction(GTK_MENU_BAR(self));
    }

    return 0;
}

