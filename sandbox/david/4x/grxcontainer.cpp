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
                          GtkWidget *widget,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxObjectPtr rxwidget = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxwidget);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  add
 *
 * Add a widget to the container widget.
 *
 * @param add     The widget to add.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxContainerAdd,           // Object_method name
            RexxObjectPtr, rxaddptr)   // Object to add
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject addptr = (RexxPointerObject)context->SendMessage0(rxaddptr, "POINTER");
    GtkWidget *addWidget = (GtkWidget *)context->PointerValue(addptr);

    gtk_container_add(GTK_CONTAINER(myWidget), addWidget); 

    return 0;
}

/**
 * Method:  remove
 *
 * Remove a widget from the container widget.
 *
 * @param remove  The widget to remove.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxContainerRemove,        // Object_method name
            RexxObjectPtr, rxremptr)   // Object to add
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject remptr = (RexxPointerObject)context->SendMessage0(rxremptr, "POINTER");
    GtkWidget *remWidget = (GtkWidget *)context->PointerValue(remptr);

    gtk_container_remove(GTK_CONTAINER(myWidget), remWidget); 

    return 0;
}

/**
 * Method:  set_border_width
 *
 * Set the border width for the container.
 *
 * @param width   The width of the border
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxContainerSetBorderWidth,// Object_method name
            int, width)                // Border widthd
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_container_set_border_width(GTK_CONTAINER(myWidget), width); 

    return 0;
}

/**
 * Method:  pack_start
 *
 * Pack a box.
 *
 * @param widget  The widget to pack.
 *
 * @param expand  Expand boolean.
 *
 * @param fill    Fill boolean.
 *
 * @param pad     Padding amount.
 *
 * @return        Zero.
 */
RexxMethod4(int,                       // Return type
            GrxBoxPackStart,           // Object_method name
            RexxObjectPtr, rxpackptr,  // Widget to pack
            logical_t, expand,         // Expand boolean
            logical_t, fill,           // Fill boolean
            int, padding)              // Padding amount
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject packptr = (RexxPointerObject)context->SendMessage0(rxpackptr, "POINTER");
    GtkWidget *packWidget = (GtkWidget *)context->PointerValue(packptr);

    gtk_box_pack_start(GTK_BOX(myWidget), packWidget, expand, fill, (unsigned int)padding); 

    return 0;
}

/**
 * Method:  pack_end
 *
 * Pack a box.
 *
 * @param widget  The widget to pack.
 *
 * @param expand  Expand boolean.
 *
 * @param fill    Fill boolean.
 *
 * @param pad     Padding amount.
 *
 * @return        Zero.
 */
RexxMethod4(int,                       // Return type
            GrxBoxPackEnd,             // Object_method name
            RexxObjectPtr, rxpackptr,  // Widget to pack
            logical_t, expand,         // Expand boolean
            logical_t, fill,           // Fill boolean
            int, padding)              // Padding amount
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject packptr = (RexxPointerObject)context->SendMessage0(rxpackptr, "POINTER");
    GtkWidget *packWidget = (GtkWidget *)context->PointerValue(packptr);

    gtk_box_pack_end(GTK_BOX(myWidget), packWidget, expand, fill, (unsigned int)padding); 

    return 0;
}

/**
 * Method:  init
 *
 * Create a new VBox.
 *
 * @param homog   The homogeneous boolean
 *
 * @param spacing The spacing amoint
 *
 * @return        Zero.
 */
RexxMethod3(int,                       // Return type
            GrxVBoxNew,                // Object_method name
            OSELF, self,               // Self
            logical_t, homogeneous,    // Homogeneous boolean
            int, spacing)              // Spacing amount
{
    GtkWidget *myWidget;


    myWidget = gtk_vbox_new(homogeneous, spacing);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new VBox from a pointer.
 *
 * @param ptr     The VBoc pointer
 *
 * @return        Zero.
 */
RexxMethod2(int,                       // Return type
            GrxVBoxNewFromPointer,     // Object_method name
            OSELF, self,               // Self
            RexxObjectPtr, ptr)        // The pointer
{
    RexxPointerObject vboxptr = (RexxPointerObject)context->SendMessage0(ptr, "POINTER");

    context->SetObjectVariable("!POINTER", vboxptr);
    g_object_set_data(G_OBJECT(ptr), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new HBox.
 *
 * @param homog   The homogeneous boolean
 *
 * @param spacing The spacing amoint
 *
 * @return        Zero.
 */
RexxMethod3(int,                       // Return type
            GrxHBoxNew,                // Object_method name
            OSELF, self,               // Self
            logical_t, homogeneous,    // Homogeneous boolean
            int, spacing)              // Spacing amount
{
    GtkWidget *myWidget;

    myWidget = gtk_hbox_new(homogeneous, spacing);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new Alignment.
 *
 * @param xalign  The X alignment
 *
 * @param yalign  The x alignment
 *
 * @param xscale  The x scale
 *
 * @param yscale  The x scale
 *
 * @return        Zero.
 */
RexxMethod5(int,                       // Return type
            GrxAlignmentNew,           // Object_method name
            OSELF, self,               // Self
            float, xalign,             // X alignment
            float, yalign,             // Y alignment
            float, xscale,             // Y scale
            float, yscale)             // Y scale
{
    GtkWidget *myWidget;

    myWidget = gtk_alignment_new(xalign, yalign, xscale, yscale);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

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
 */
RexxMethod1(RexxObjectPtr,             // Return type
            GrxContainerSignalConnect, // Object_method name
            CSTRING, name)             // Signal name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "add") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_add";
        g_signal_connect(G_OBJECT(myWidget), "add",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "check-resize") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_check_resize";
        g_signal_connect(G_OBJECT(myWidget), "check-resize",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "remove") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_remove";
        g_signal_connect(G_OBJECT(myWidget), "remove",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "set-focus-child") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_focus_child";
        g_signal_connect(G_OBJECT(myWidget), "set-focus-child",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    RexxObjectPtr parent = context->GetSuper();
    return context->SendMessage0(parent, name);
}


/**
 * Method:  init
 *
 * Create a new Alignment.
 *
 * @param xalign  The X alignment
 *
 * @param hadj    The horizontal adjustment
 *
 * @param vadj    The vertical adjustment
 *
 * @return        Zero.
 */
RexxMethod3(int,                       // Return type
            GrxViewportNew,            // Object_method name
            OSELF, self,               // Self
            RexxObjectPtr, rxhadj,     // Horizontal adjustment object
            RexxObjectPtr, rxvadj)     // Vertical adjustment object
{
    RexxPointerObject hadjptr = (RexxPointerObject)context->SendMessage0(rxhadj, "POINTER");
    GtkAdjustment *hadj = (GtkAdjustment *)context->PointerValue(hadjptr);
    RexxPointerObject vadjptr = (RexxPointerObject)context->SendMessage0(rxvadj, "POINTER");
    GtkAdjustment *vadj = (GtkAdjustment *)context->PointerValue(vadjptr);
    GtkWidget *myWidget;

    myWidget = gtk_viewport_new(hadj, vadj);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

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
 */
RexxMethod1(RexxObjectPtr,             // Return type
            GrxViewportSignalConnect,  // Object_method name
            CSTRING, name)             // Signal name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "set-scroll-adjustments") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_scroll_adjustments";
        g_signal_connect(G_OBJECT(myWidget), "set-scroll-adjustments",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    RexxObjectPtr parent = context->GetSuper();
    return context->SendMessage0(parent, name);
}
