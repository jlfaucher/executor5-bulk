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
#include <errno.h>
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

static void signal_func_1(GtkWidget *window,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s", window, data);
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


/*============================================================================*/
/* Public Methods                                                             */
/*============================================================================*/


RexxMethod1(int,                       // Return type
            GrxWidgetNew,              // Object_method name
            OSELF, self)               // Pointer to self
{
    GtkWidget *myWidget;

    myWidget = gtk_widget_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    return 0;
}


RexxMethod1(int,                       // Return type
            GrxWidgetShow,             // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_show(myWidget);

    return 0;
}


RexxMethod1(int,                       // Return type
            GrxWidgetShowAll,          // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_show_all(myWidget);

    return 0;
}


RexxMethod1(int,                       // Return type
            GrxWidgetHide,             // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_hide(myWidget);

    return 0;
}


RexxMethod1(int,                       // Return type
            GrxWidgetHideAll,          // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_hide_all(myWidget);

    return 0;
}


RexxMethod3(int,                       // Return type
            GrxWidgetHideAll,          // Object_method name
            OSELF, self,               // Pointer to self
            int width,                 // width of the widget
            int height)                // height of the widget
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_set_size_request(myWidget, width, height);

    return 0;
}


RexxMethod1(int,                       // Return type
            GrxWidgetDestroy,          // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_destroy(myWidget);

    return 0;
}


RexxMethod1(int,                       // Return type
            GrxWidgetGrabFocus,        // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_grab_focus(myWidget);

    return 0;
}


RexxMethod2(int,                       // Return type
            GrxWidgetSetFont,          // Object_method name
            OSELF, self,               // Pointer to self
            CSTRING, fontname)         // Font name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);
    GtkStyle *style;

    style = gtk_style_copy(gtk_widget_get_style(myWidget));
    style->font_desc = pango_font_description_from_string(fontname);
    gtk_widget_set_style(myWidget, style);

    return 0;
}


RexxMethod1(CSTRING,                   // Return type
            GrxWidgetGetFont,          // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);
    GtkStyle *style = NULL;

    style = gtk_widget_get_style(myWidget);
    return style->font_desc;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetGetAccessible                             */
/* Description: Get the pointer to the accessibility object                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

//  APIRET APIENTRY GrxWidgetGetAccessible(const char * Name,
//                                         const size_t Argc, const RXSTRING Argv[],
//                                         const char * Queuename, PRXSTRING Retstr)
//  {
//      GtkWidget *myWidget;
//      AtkObject *access = NULL;
//
//      /* Check for valid arguments */
//      if (GrxCheckArgs(1, Argc, Argv)) {
//          return RXFUNC_BADCALL;
//      }
//
//      /* Initialize function parameters */
//      sscanf(Argv[0].strptr, "%p", &myWidget);
//
//      /* get the accessibility pointer */
//      access = gtk_widget_get_accessible(myWidget);
//
//      /* Set up the REXX return code */
//      g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", access);
//      Retstr->strlength = strlen(Retstr->strptr);
//
//
//      return RXFUNC_OK;
//  }


RexxMethod1(CSTRING,                   // Return type
            GrxWidgetGetName,          // Object_method name
            OSELF, self)               // Pointer to self
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    return gtk_widget_get_name(myWidget);
}


RexxMethod2(int,                       // Return type
            GrxWidgetSetName,          // Object_method name
            OSELF, self,               // Pointer to self
            CSTRING, name)             // New name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_set_name(myWidget, name);

    return 0;
}


RexxMethod2(int,                       // Return type
            GrxWidgetSetSensitive,     // Object_method name
            OSELF, self,               // Pointer to self
            int, flag)                 // Sensitive flag
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    gtk_widget_set_sensitive(myWidget, flag);

    return 0;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxWidgetGetTopLevel                               */
/* Description: Get the top level parent window of a widget                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

//  APIRET APIENTRY GrxWidgetGetTopLevel(const char * Name,
//                                       const size_t Argc, const RXSTRING Argv[],
//                                       const char * Queuename, PRXSTRING Retstr)
//  {
//      GtkWidget *myWidget;
//      GtkWidget *parentWidget;
//
//      /* Check for valid arguments */
//      if (GrxCheckArgs(1, Argc, Argv)) {
//          return RXFUNC_BADCALL;
//      }
//
//      /* Initialize function parameters */
//      sscanf(Argv[0].strptr, "%p", &myWidget);
//
//      if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
//          parentWidget = gtk_widget_get_toplevel(myWidget);
//      }
//      else {
//          return RXFUNC_BADCALL;
//      }
//
//      /* Set up the REXX return code */
//      g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", parentWidget);
//      Retstr->strlength = strlen(Retstr->strptr);
//
//      return RXFUNC_OK;
//  }


RexxMethod3(int,                       // Return type
            GrxWidgetModifyBG,         // Object_method name
            OSELF, self,               // Pointer to self
            int, type,                 // State
            CSTRING, colorstr)         // Color string
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_widget_modify_bg(myWidget, type, &color);

    return 0;
}


RexxMethod3(int,                       // Return type
            GrxWidgetModifyFG,         // Object_method name
            OSELF, self,               // Pointer to self
            int, type,                 // State
            CSTRING, colorstr)         // Color string
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_widget_modify_fg(myWidget, type, &color);

    return 0;
}


RexxMethod3(int,                       // Return type
            GrxWidgetSetData,          // Object_method name
            OSELF, self,               // Pointer to self
            CSTRING, name,             // Association name
            CSTRING, data)             // Association data
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    // Note: The memory allocated from strdup is never recovered and
    // thus causes a memory leak.
    g_object_set_data(G_OBJECT(myWidget), name, strdup(data));

    return 0;
}


RexxMethod2(CSTRING,                   // Return type
            GrxWidgetGetData,          // Object_method name
            OSELF, self,               // Pointer to self
            CSTRING, name)             // Association name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    return (char *)g_object_get_data(G_OBJECT(myWidget), name);
}


RexxMethod2(int,                       // Return type
            GrxWidgetConnectSignal,    // Object_method name
            OSELF, self,               // Pointer to self
            CSTRING, name)             // Signal name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue((rxptr);

    if (strcmp(name, "destroy") == 0) {
        g_signal_connect(G_OBJECT(myWidget), "destroy",
                         G_CALLBACK(signal_func_1), "signal_destroy");
    }
    else {
        return RXFUNC_BADCALL;
    }

        return 0;
}

