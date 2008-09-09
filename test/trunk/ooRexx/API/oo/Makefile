#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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
ORXLFLAGS = --export-dynamic -shared -nostartfiles
ORXCFLAGS =  -fPIC
COMMON_SRC_DEPS =
FUNCTION_SRC_DEPS = $(COMMON_SRC_DEPS)
METHOD_SRC_DEPS = $(COMMON_SRC_DEPS)


# Build the libraries
all: liborxfunction.so liborxmethod.so


# Build the shared library
liborxfunction.so: orxfunction.o
	gcc $(ORXLFLAGS) -lrexx -lrexxapi -o liborxfunction.so orxfunction.o

# Build the objects
orxfunction.o: orxfunction.cpp $(FUNCTION_SRC_DEPS)
	gcc -c $(ORXCFLAGS) orxfunction.cpp


# Build the shared library
liborxmethod.so: orxmethod.o
	gcc $(ORXLFLAGS) -lrexx -lrexxapi -o liborxmethod.so orxmethod.o

# Build the objects
orxmethod.o: orxmethod.cpp $(METHOD_SRC_DEPS)
	gcc -c $(ORXCFLAGS) orxmethod.cpp



clean:
	rm *.so *.o

