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
 **/
RexxMethod2(int,                       // Return type
            GrxContainerAdd,           // Object_method name
            RexxObjectPtr, rxadd,      // Object to add
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxadd, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *addWidget = (GtkWidget *)context->ObjectToCSelf(rxadd);

    gtk_container_add(GTK_CONTAINER(self), addWidget);

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
 **/
RexxMethod2(int,                       // Return type
            GrxContainerRemove,        // Object_method name
            RexxObjectPtr, rxrem,      // Object to add
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxrem, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *remWidget = (GtkWidget *)context->ObjectToCSelf(rxrem);

    gtk_container_remove(GTK_CONTAINER(self), remWidget);

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
 **/
RexxMethod2(int,                       // Return type
            GrxContainerSetBorderWidth,// Object_method name
            int, width,                // Border widthd
            CSELF, self)               // GTK self
{
    gtk_container_set_border_width(GTK_CONTAINER(self), width);

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
 **/
RexxMethod5(int,                       // Return type
            GrxBoxPackStart,           // Object_method name
            RexxObjectPtr, rxpack,     // Widget to pack
            logical_t, expand,         // Expand boolean
            logical_t, fill,           // Fill boolean
            int, padding,              // Padding amount
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxpack, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *packWidget = (GtkWidget *)context->ObjectToCSelf(rxpack);

    gtk_box_pack_start(GTK_BOX(self), packWidget, expand, fill, (unsigned int)padding);

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
 **/
RexxMethod5(int,                       // Return type
            GrxBoxPackEnd,             // Object_method name
            RexxObjectPtr, rxpack,     // Widget to pack
            logical_t, expand,         // Expand boolean
            logical_t, fill,           // Fill boolean
            int, padding,              // Padding amount
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(rxpack, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *packWidget = (GtkWidget *)context->ObjectToCSelf(rxpack);

    gtk_box_pack_end(GTK_BOX(self), packWidget, expand, fill, (unsigned int)padding);

    return 0;
}

/**
 * Method:  init (2 args)
 *
 * Create a new VBox.
 *
 * @param homog   The homogeneous boolean
 *
 * @param spacing The spacing amount
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxVBoxNew,                // Object_method name
            logical_t, homogeneous,    // Homogeneous boolean
            int, spacing,              // Spacing amount
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_vbox_new(homogeneous, spacing);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init (1 arg)
 *
 * Create a new VBox.
 *
 * @param rxptr   The vbox widget pointer
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxVBoxNewFromPtr,         // Object_method name
            POINTER, rxptr,            // Vbox pointer
            OSELF, self)               // Self
{
    GtkVBox *vbox = (GtkVBox *)rxptr;

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(vbox));
    g_object_set_data(G_OBJECT(vbox), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new HBox from a pointer.
 *
 * @param ptr     The VBox pointer
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxVBoxNewFromPointer,     // Object_method name
            POINTER, ptr,              // The pointer
            OSELF, self)               // Self
{
    GtkWidget *myWidget = (GtkWidget *)ptr;

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
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
 * @param spacing The spacing amount
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxHBoxNew,                // Object_method name
            logical_t, homogeneous,    // Homogeneous boolean
            int, spacing,              // Spacing amount
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_hbox_new(homogeneous, spacing);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init (1 arg)
 *
 * Create a new HBox.
 *
 * @param rxptr   The hbox widget pointer
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxHBoxNewFromPtr,         // Object_method name
            POINTER, rxptr,            // Homogeneous boolean
            OSELF, self)               // Self
{
    GtkVBox *hbox = (GtkVBox *)rxptr;

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(hbox));
    g_object_set_data(G_OBJECT(hbox), "OORXOBJECT", self);

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
 **/
RexxMethod5(int,                       // Return type
            GrxAlignmentNew,           // Object_method name
            float, xalign,             // X alignment
            float, yalign,             // Y alignment
            float, xscale,             // Y scale
            float, yscale,             // Y scale
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_alignment_new(xalign, yalign, xscale, yscale);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
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
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxContainerSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "add") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_add";
        g_signal_connect(G_OBJECT(self), "add",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "check_resize") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_check_resize";
        g_signal_connect(G_OBJECT(self), "check-resize",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "remove") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_remove";
        g_signal_connect(G_OBJECT(self), "remove",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "set_focus_child") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_focus_child";
        g_signal_connect(G_OBJECT(self), "set-focus-child",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}


/**
 * Method:  init
 *
 * Create a new viewport.
 *
 * @param hadj    The horizontal adjustment
 *
 * @param vadj    The vertical adjustment
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxViewportNew,            // Object_method name
            RexxObjectPtr, rxhadj,     // Horizontal adjustment object
            RexxObjectPtr, rxvadj,     // Vertical adjustment object
            OSELF, self)               // Self
{
    if (!context->IsOfType(rxhadj, "GtkAdjustment")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkAdjustment"));
        return 0;
    }
    GtkAdjustment *hadj = (GtkAdjustment *)context->ObjectToCSelf(rxhadj);
    if (!context->IsOfType(rxvadj, "GtkAdjustment")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(2),
                                 context->NewStringFromAsciiz("GtkAdjustment"));
        return 0;
    }
    GtkAdjustment *vadj = (GtkAdjustment *)context->ObjectToCSelf(rxvadj);
    GtkWidget *myWidget = gtk_viewport_new(hadj, vadj);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
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
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxViewportSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "set_scroll_adjustments") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_set_scroll_adjustments";
        g_signal_connect(G_OBJECT(self), "set-scroll-adjustments",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}
