#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
#/*                                                                            */
#/* This program and the accompanying materials are made available under       */
#/* the terms of the Common Public License v1.0 which accompanies this         */
#/* distribution. A copy is also available at the following address:           */
#/* http://www.oorexx.org/license.html                                         */
#/*                                                                            */
#/* Redistribution and use in source and binary forms, with or                 */
#/* without modification, are permitted provided that the following            */
#/* conditions are met:                                                        */
#/*                                                                            */
#/* Redistributions of source code must retain the above copyright             */
#/* notice, this list of conditions and the following disclaimer.              */
#/* Redistributions in binary form must reproduce the above copyright          */
#/* notice, this list of conditions and the following disclaimer in            */
#/* the documentation and/or other materials provided with the distribution.   */
#/*                                                                            */
#/* Neither the name of Rexx Language Association nor the names                */
#/* of its contributors may be used to endorse or promote products             */
#/* derived from this software without specific prior written permission.      */
#/*                                                                            */
#/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
#/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
#/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
#/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
#/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
#/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
#/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
#/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
#/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
#/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
#/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
#/*                                                                            */
#/*----------------------------------------------------------------------------*/


# Setup
include ver.mak
GTK = gtk+-2.0
GRXLFLAGS = `pkg-config --libs gtk+-2.0 gdk-2.0 glib-2.0 gthread-2.0` \
	--export-dynamic -shared -nostartfiles
GRXCFLAGS = `pkg-config --cflags gtk+-2.0 gdk-2.0 glib-2.0 gthread-2.0` \
        -fPIC -DVMAJOR=$(VMAJOR) \
        -DVMINOR=$(VMINOR) -DVREL=$(VREL)
OBJECTS = rexxgtk.o \
        grxwidget.o \
	grxwindow.o \
	grxcontainer.o \
	grxbutton.o \
	grxpaned.o \
	grxtable.o \
        grxcombobox.o \
        grxentry.o \
        grxdialog.o \
        grxassistant.o \
	grxlabel.o \
        grxfixed.o \
        grxexpander.o \
        grxhandlebox.o \
        grxnotebook.o \
        grxrange.o \
        grximage.o \
        grxprogressbar.o \
        grxtextview.o \
        grxtextbuffer.o \
        grxglib.o \
        grxinterfaces.o \
        grxclipboard.o \
        grxadjustment.o \
        grxscrolledwindow.o \
	grxliststore.o \
	grxeventbox.o \
	grxmenushell.o \
	grxmenu.o \
	grxmenuitem.o \
	grxmenubar.o \
	grxevents.o \
	grxaccelgroup.o \
	grxstatusbar.o \
	grxtoolbar.o \
	grxtoolitem.o \
	grxtoolbutton.o \
	grxmenutoolbutton.o \
	grxcellrenderer.o \
	grxtreeview.o \
	grxtreeviewcolumn.o \
	grxtreestore.o \
	grxtreemodel.o \
	grxcalendar.o \
	grxtreeselection.o


# Build the shared library
librexxgtk.so: $(OBJECTS)
	gcc $(GRXLFLAGS) -lrexx -lrexxapi -o librexxgtk.so $(OBJECTS)


# Build the objects
# Compile *.cpp to *.o inference rule
.cpp.o:
	gcc -c $(GRXCFLAGS) $<

# Dependencies
rexxgtk.obj: rexxgtk.cpp rexxgtk.h

grxwidget.obj: grxwidget.cpp rexxgtk.h

grxwindow.obj: grxwindow.cpp rexxgtk.h

grxcontainer.obj: grxcontainer.cpp rexxgtk.h

grxlabel.obj: grxlabel.cpp rexxgtk.h

grxbutton.obj: grxbutton.cpp rexxgtk.h

grxpaned.obj: grxpaned.cpp rexxgtk.h

grxtable.obj: grxtable.cpp rexxgtk.h

grxentry.obj: grxentry.cpp rexxgtk.h

grxfixed.obj: grxfixed.cpp rexxgtk.h

grxexpander.obj: grxexpander.cpp rexxgtk.h

grxhandlebox.obj: grxhandlebox.cpp rexxgtk.h

grxnotebook.obj: grxnotebook.cpp rexxgtk.h

grxrange.obj: grxrange.cpp rexxgtk.h

grxinterfaces.obj: grxinterfaces.cpp rexxgtk.h

grximage.obj: grximage.cpp rexxgtk.h

grxdialog.obj: grxdialog.cpp rexxgtk.h

grxglib.obj: grxglib.cpp rexxgtk.h

grxassistant.obj: grxassistant.cpp rexxgtk.h

grxprogressbar.obj: grxprogressbar.cpp rexxgtk.h

grxadjustment.obj: grxadjustment.cpp rexxgtk.h

grxscrolledwindow.obj: grxscrolledwindow.cpp rexxgtk.h

grxtextview.obj: grxtextview.cpp rexxgtk.h

grxtextbuffer.obj: grxtextbuffer.cpp rexxgtk.h

grxclipboard.obj: grxclipboard.cpp rexxgtk.h

grxcombobox.obj: grxcombobox.cpp rexxgtk.h

grxliststore.obj: grxliststore.cpp rexxgtk.h

grxeventbox.obj: grxeventbox.cpp rexxgtk.h

grxmenushell.obj: grxmenushell.cpp rexxgtk.h

grxmenu.obj: grxmenu.cpp rexxgtk.h

grxmenuitem.obj: grxmenuitem.cpp rexxgtk.h

grxmenubar.obj: grxmenubar.cpp rexxgtk.h

grxevents.obj: grxevents.cpp rexxgtk.h

grxaccelgroup.obj: grxaccelgroup.cpp rexxgtk.h

grxstatusbar.obj: grxstatusbar.cpp rexxgtk.h

grxtoolbar.obj: grxtoolbar.cpp rexxgtk.h

grxtoolitem.obj: grxtoolitem.cpp rexxgtk.h

grxtoolbutton.obj: grxtoolbutton.cpp rexxgtk.h

grxmenutoolbutton.obj: grxmenutoolbutton.cpp rexxgtk.h

grxcellrenderer.obj: grxcellrenderer.cpp rexxgtk.h

grxtreeview.obj: grxtreeview.cpp rexxgtk.h

grxtreeviewcolumn.obj: grxtreeviewcolumn.cpp rexxgtk.h

grxtreestore.obj: grxtreestore.cpp rexxgtk.h

grxtreemodel.obj: grxtreemodel.cpp rexxgtk.h

grxcalendar.obj: grxcalendar.cpp rexxgtk.h

grxtreeselection.obj: grxtreeselection.cpp rexxgtk.h



clean:
	rm *.so *.o

