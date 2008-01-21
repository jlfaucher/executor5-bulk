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

static void signal_func_1(GtkWidget *window,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s", window, data);
#else
    snprintf(buffer, sizeof(buffer), "%p %s", window, data);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonNew                                       */
/* Description: Create a push button.                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonNew(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_button_new();
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonNewFromStock                              */
/* Description: Create a push button.                                         */
/* Rexx Args:   Button type                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonNewFromStock(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;


    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_button_new_from_stock(Argv[0].strptr);
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonSetLabel                                  */
/* Description: Set the label of the button.                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*              Label                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonSetLabel(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_button_set_label(GTK_BUTTON(myWidget), Argv[1].strptr);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonGetLabel                                  */
/* Description: Get the label of the button.                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonGetLabel(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    const gchar *label = NULL;
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        label = gtk_button_get_label(GTK_BUTTON(myWidget));
    }

    /* Set up the REXX return code */
    if (label != NULL) {
        if (strlen(label) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory(strlen(label) + 1);
        }
        strcpy(Retstr->strptr, label);
    }
    else {
        strcpy(Retstr->strptr, "\0");
    }
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonSetRelief                                 */
/* Description: Set the releif stype of the button.                           */
/* Rexx Args:   Pointer to the widget                                         */
/*              Relief Styls                                                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonSetRelief(const char * Name,
                                   const size_t Argc, const RXSTRING Argv[],
                                   const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkReliefStyle style;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    if(strcmp(Argv[1].strptr,"GTK_RELIEF_NORMAL") == 0)
        style = GTK_RELIEF_NORMAL;
    else if(strcmp(Argv[1].strptr,"GTK_RELIEF_HALF") == 0)
        style = GTK_RELIEF_HALF;
    else if(strcmp(Argv[1].strptr,"GTK_RELIEF_NONE") == 0)
        style = GTK_RELIEF_NONE;
    else 
        sscanf(Argv[1].strptr, "%d", &style);

    gtk_button_set_relief(GTK_BUTTON(myWidget), style);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonUseUnderline                              */
/* Description: Set the button to use mnemonics                               */
/* Rexx Args:   Pointer to the widget                                         */
/*              Flag                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonUseUnderline(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    gtk_button_set_use_underline(GTK_BUTTON(myWidget), flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonNew                                 */
/* Description: Create a toggle push button.                                  */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonNew(const char * Name,
                                   const size_t Argc, const RXSTRING Argv[],
                                   const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_toggle_button_new();
	
    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
              Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonGetMode                             */
/* Description: Get the display mode of the button                            */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonGetMode(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean mode;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        mode = gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(myWidget));
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", (int)mode);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonSetMode                             */
/* Description: Set the display mode of the button                            */
/* Rexx Args:   Pointer to the widget                                         */
/*              New mode                                                      */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonSetMode(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean mode;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &mode);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(myWidget), mode);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonGetActive                           */
/* Description: Get the active state of the button                            */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonGetActive(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean state;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(myWidget));
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", (int)state);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonSetActive                           */
/* Description: Set the active state of the button                            */
/* Rexx Args:   Pointer to the widget                                         */
/*              New state                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonSetActive(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean state;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &state);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(myWidget), state);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonGetInconsistent                     */
/* Description: Get the inconsistent of the button                            */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonGetInconsistent(const char * Name,
                                               const size_t Argc, const RXSTRING Argv[],
                                               const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean state;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        state = gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(myWidget));
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", (int)state);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonSetInconsistent                     */
/* Description: Set the inconsistent of the button                            */
/* Rexx Args:   Pointer to the widget                                         */
/*              New state                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonSetInconsistent(const char * Name,
                                               const size_t Argc, const RXSTRING Argv[],
                                               const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean state;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &state);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(myWidget), state);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxCheckButtonNew                                  */
/* Description: Create a check button.                                        */
/* Rexx Args:   Label                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxCheckButtonNew(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (Argc > 1) {
        return RXFUNC_BADCALL;
    }

    if (Argc == 1 && RXVALIDSTRING(Argv[0])) {
       myWidget = gtk_check_button_new_with_label(Argv[0].strptr);
    }
    else {
        myWidget = gtk_check_button_new();
		
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRadioButtonNew                                  */
/* Description: Create a radio button.                                        */
/* Rexx Args:   Group list                                                    */
/*              Label (optional)                                              */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRadioButtonNew(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GSList *head;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &head);

    myWidget = gtk_radio_button_new(head);

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRadioButtonNewFromWidget                        */
/* Description: Create a radio button from another widget.                    */
/* Rexx Args:   Pointer to the source widget                                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRadioButtonNewFromWidget(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *srcWidget, *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &srcWidget);

    myWidget = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(srcWidget));

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRadioButtonGetGroup                             */
/* Description: Get the radio button group.                                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRadioButtonGetGroup(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GSList *head = NULL;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        head = gtk_radio_button_get_group(GTK_RADIO_BUTTON(myWidget));
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", head);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRadioButtonSetGroup                             */
/* Description: Get the radio button group.                                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRadioButtonSetGroup(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GSList *head = NULL;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%p", &head);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        gtk_radio_button_set_group(GTK_RADIO_BUTTON(myWidget), head);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorButtonNew                                  */
/* Description: Create a color button.                                        */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorButtonNew(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_color_button_new();

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorButtonNewWithColor                         */
/* Description: Create a color button.                                        */
/* Rexx Args:   Color                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorButtonNewWithColor(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GdkColor color;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    gdk_color_parse(Argv[0].strptr, &color);

    myWidget = gtk_color_button_new_with_color(&color);

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorButtonSetTitle                             */
/* Description: Create a color button.                                        */
/* Rexx Args:   Pointer to the widget                                         */
/*              Title                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorButtonSetTitle(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_color_button_set_title(GTK_COLOR_BUTTON(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorButtonGetColor                             */
/* Description: Create a color button.                                        */
/* Rexx Args:   Pointer to the widget                                         */
/*              Title                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorButtonGetColor(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GdkColor color;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_color_button_get_color(GTK_COLOR_BUTTON(myWidget), &color);

    /* Set up the REXX return code */
#ifdef WIN32
    sprintf(Retstr->strptr, "#%04X%04X%04", color.red, color.green, color.blue);
#else
    snprintf(Retstr->strptr, RXAUTOBUFLEN, "#%04X%04X%04X", color.red, color.green, color.blue);
#endif
    Retstr->strlength = strlen(Retstr->strptr);;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorButtonSetColor                             */
/* Description: Set the color of the button.                                  */
/* Rexx Args:   Pointer to the widget                                         */
/*              Color                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorButtonSetColor(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GdkColor color;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    gdk_color_parse(Argv[1].strptr, &color);

    gtk_color_button_set_color(GTK_COLOR_BUTTON(myWidget), &color);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFileChooserButtonNew                            */
/* Description: Create a file chooser button                                  */
/* Rexx Args:   Title                                                         */
/*              Action                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFileChooserButtonNew(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GtkFileChooserAction action;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    if(strcmp(Argv[1].strptr,"GTK_FILE_CHOOSER_ACTION_OPEN") == 0)
        action = GTK_FILE_CHOOSER_ACTION_OPEN;
    else if(strcmp(Argv[1].strptr,"GTK_FILE_CHOOSER_ACTION_SAVE") == 0)
        action = GTK_FILE_CHOOSER_ACTION_SAVE;
    else if(strcmp(Argv[1].strptr,"GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER") == 0)
        action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
    else if(strcmp(Argv[1].strptr,"GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER") == 0)
        action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
    else 
        sscanf(Argv[1].strptr, "%d", &action);

    myWidget = gtk_file_chooser_button_new(Argv[0].strptr, action);

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontButtonNew                                   */
/* Description: Create a font button.                                         */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontButtonNew(const char * Name,
                                 const size_t Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_font_button_new();

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontButtonSetTitle                              */
/* Description: Set the title of the font button.                             */
/* Rexx Args:   Pointer to the widget                                         */
/*              Title                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontButtonSetTitle(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_font_button_set_title(GTK_FONT_BUTTON(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontButtonGetFontName                           */
/* Description: Get the font name.   .                                        */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontButtonGetFontName(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    const gchar * font;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    font = gtk_font_button_get_font_name(GTK_FONT_BUTTON(myWidget));

    /* Set up the REXX return code */
    strcpy(Retstr->strptr, font);
    Retstr->strlength = strlen(Retstr->strptr);;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxButtonConnectSignal                             */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxButtonConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "pressed") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "pressed",
                             G_CALLBACK(signal_func_1), "signal_pressed");
        }
        else if (strcmp(Argv[1].strptr, "released") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "released",
                             G_CALLBACK(signal_func_1), "signal_released");
        }
        else if (strcmp(Argv[1].strptr, "clicked") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "clicked",
                             G_CALLBACK(signal_func_1), "signal_clicked");
        }
        else if (strcmp(Argv[1].strptr, "enter") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "enter",
                             G_CALLBACK(signal_func_1), "signal_enter");
        }
        else if (strcmp(Argv[1].strptr, "leave") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "leave",
                             G_CALLBACK(signal_func_1), "signal_leave");
        }
        else {
            return RXFUNC_BADCALL;
        }
    }
    else {
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxToggleButtonConnectSignal                       */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxToggleButtonConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "toggled") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "toggled",
                             G_CALLBACK(signal_func_1), "signal_toggled");
        }
        else {
            return RXFUNC_BADCALL;
        }
    }
    else {
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxRadioButtonConnectSignal                        */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxRadioButtonConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "group_changed") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "group-changed",
                             G_CALLBACK(signal_func_1), "signal_group_changed");
        }
        else {
            return RXFUNC_BADCALL;
        }
    }
    else {
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorButtonConnectSignal                        */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorButtonConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "color_set") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "color-set",
                             G_CALLBACK(signal_func_1), "signal_color_set");
        }
        else {
            printf("Bad signal type!\n");
            return RXFUNC_BADCALL;
        }
    }
    else {
        printf("Not a widget!\n");
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxChooserButtonConnectSignal                      */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxChooserButtonConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "file_set") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "file-set",
                             G_CALLBACK(signal_func_1), "signal_file_set");
        }
        else {
            printf("Bad signal type!\n");
            return RXFUNC_BADCALL;
        }
    }
    else {
        printf("Not a widget!\n");
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontButtonConnectSignal                         */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontButtonConnectSignal(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "font_set") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "font-set",
                             G_CALLBACK(signal_func_1), "signal_font_set");
        }
        else {
            printf("Bad signal type!\n");
            return RXFUNC_BADCALL;
        }
    }
    else {
        printf("Not a widget!\n");
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

