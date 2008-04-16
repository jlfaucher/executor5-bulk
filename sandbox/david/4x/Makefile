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
        -DVMINOR=$(VMINOR) -DVREL=$(VREL) \
	-I/home/dashley/ad/svn/sandbox/rick/opt/api \
	-I/home/dashley/ad/svn/sandbox/rick/opt/api/platform/unix
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
#       grxnotebook.o \
#       grxeventbox.o \
#       grxrange.o \
#       grxinterfaces.o \
#       grxfilefilter.o \
#       grximage.o \
#       grxglib.o \
#       grxprogressbar.o \
#       grxadjustment.o \
#       grxscrolledwindow.o \
#       grxtextview.o \
#       grxtextbuffer.o \
#       grxclipboard.o \


# Build the shared library
librexxgtk.so: $(OBJECTS)
	gcc $(GRXLFLAGS) -lrexx -lrexxapi -o librexxgtk.so $(OBJECTS)


# Build the objects
rexxgtk.o: rexxgtk.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) rexxgtk.cpp

grxwidget.o: grxwidget.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxwidget.cpp

grxwindow.o: grxwindow.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxwindow.cpp

grxcontainer.o: grxcontainer.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxcontainer.cpp

grxlabel.o: grxlabel.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxlabel.cpp

grxbutton.o: grxbutton.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxbutton.cpp

grxpaned.o: grxpaned.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxpaned.cpp

grxtable.o: grxtable.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxtable.cpp

grxentry.o: grxentry.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxentry.cpp

grxfixed.o: grxfixed.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxfixed.cpp

grxexpander.o: grxexpander.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxexpander.cpp

grxhandlebox.o: grxhandlebox.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxhandlebox.cpp

grxnotebook.o: grxnotebook.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxnotebook.c

grxeventbox.o: grxeventbox.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxeventbox.c

grxrange.o: grxrange.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxrange.c

grxinterfaces.o: grxinterfaces.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxinterfaces.c

grxfilefilter.o: grxfilefilter.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxfilefilter.c

grximage.o: grximage.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grximage.c

grxdialog.o: grxdialog.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxdialog.cpp

grxglib.o: grxglib.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxglib.c

grxassistant.o: grxassistant.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxassistant.cpp

grxprogressbar.o: grxprogressbar.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxprogressbar.c

grxadjustment.o: grxadjustment.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxadjustment.c

grxscrolledwindow.o: grxscrolledwindow.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxscrolledwindow.c

grxtextview.o: grxtextview.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxtextview.c

grxtextbuffer.o: grxtextbuffer.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxtextbuffer.c

grxclipboard.o: grxclipboard.c rexxgtk.h
	gcc -c $(GRXCFLAGS) grxclipboard.c

grxcombobox.o: grxcombobox.cpp rexxgtk.h
	gcc -c $(GRXCFLAGS) grxcombobox.cpp



clean:
	rm *.so *.o

