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
    GtkWidget *myWidget;

    myWidget = gtk_menu_bar_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
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
RexxMethod1(int,                       // Return type
            GrxMenuBarAppend,          // Object_method name
            RexxObjectPtr, child)      // The child menu
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenuBar *myWidget = (GtkMenuBar *)context->PointerValue(rxptr);
    RexxPointerObject childptr = (RexxPointerObject)context->SendMessage0(child, "POINTER");
    GtkWidget *childWidget = (GtkWidget *)context->PointerValue(childptr);

    gtk_menu_bar_append(myWidget, childWidget);

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
RexxMethod1(int,                       // Return type
            GrxMenuBarPrepend,         // Object_method name
            RexxObjectPtr, child)      // The child menu
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenuBar *myWidget = (GtkMenuBar *)context->PointerValue(rxptr);
    RexxPointerObject childptr = (RexxPointerObject)context->SendMessage0(child, "POINTER");
    GtkWidget *childWidget = (GtkWidget *)context->PointerValue(childptr);

    gtk_menu_bar_prepend(myWidget, childWidget);

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
RexxMethod2(int,                       // Return type
            GrxMenuBarInsert,          // Object_method name
            RexxObjectPtr, child,      // The child menu
            int, pos)                  // Position
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenuBar *myWidget = (GtkMenuBar *)context->PointerValue(rxptr);
    RexxPointerObject childptr = (RexxPointerObject)context->SendMessage0(child, "POINTER");
    GtkWidget *childWidget = (GtkWidget *)context->PointerValue(childptr);

    gtk_menu_bar_insert(myWidget, childWidget, pos);

    return 0;
}

/**
 * Method:  set_pack_direction
 *
 * Set the packing direction.
 *
 * @param dir     The direction
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuBarSetPackDirection, // Object_method name
            int, dir)                  // Position
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenuBar *myWidget = (GtkMenuBar *)context->PointerValue(rxptr);

    gtk_menu_bar_set_pack_direction(myWidget, (GtkPackDirection)dir);

    return 0;
}

/**
 * Method:  get_pack_direction
 *
 * Get the packing direction.
 *
 * @return        Direction
 **/
RexxMethod0(int,                       // Return type
            GrxMenuBarGetPackDirection) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenuBar *myWidget = (GtkMenuBar *)context->PointerValue(rxptr);

    return gtk_menu_bar_get_pack_direction(myWidget);

    return 0;
}

/**
 * Method:  set_child_pack_direction
 *
 * Set the child packing direction.
 *
 * @param dir     The direction
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuBarSetChildPackDirection, // Object_method name
            int, dir)                  // Position
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenuBar *myWidget = (GtkMenuBar *)context->PointerValue(rxptr);

    gtk_menu_bar_set_child_pack_direction(myWidget, (GtkPackDirection)dir);

    return 0;
}

/**
 * Method:  get_child_pack_direction
 *
 * Get the child packing direction.
 *
 * @return        Direction
 **/
RexxMethod0(int,                       // Return type
            GrxMenuBarGetChildPackDirection) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkMenuBar *myWidget = (GtkMenuBar *)context->PointerValue(rxptr);

    return gtk_menu_bar_get_child_pack_direction(myWidget);

    return 0;
}

