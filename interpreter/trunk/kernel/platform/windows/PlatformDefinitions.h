/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/* REXX WIN32 Support                                           winrexx.h     */
/*                                                                            */
/* WIN32 master definition file                                               */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Template for system specific declarations for Object REXX.  The following  */
/* sections are a series of required and optional sections where system       */
/* specific information is provided to the rest of the interpreter code.      */
/* Wherever possible, reasonable defaults are provided for these settings.    */
/******************************************************************************/

#ifndef PlatformDefinitions_H            /* provide a define here to protect  */
#define PlatformDefinitions_H            /* against multiple includes         */

/******************************************************************************/
/* REQUIRED:  The implemenation must decide on the C_STACK_SIZE defining      */
/* constants that are compiler/linker dependent.                              */
/******************************************************************************/
#define MIN_C_STACK 1024*32
#define TOTAL_STACK_SIZE 1024*512
#define C_STACK_SIZE 60000

/******************************************************************************/
/* OPTIONAL:  If the implementation is going to support a cross-process       */
/* shared memory model, then include a define for SHARED                      */
/******************************************************************************/
//#define SHARED                         // no shared memory sup

/******************************************************************************/
/* OPTIONAL:  If the implementation is going to support multiple threads,     */
/* include a define for THREADS.  If not defined, then the REPLY instruction  */
/* will not work.                                                             */
/******************************************************************************/
#define THREADS

/******************************************************************************/
/* OPTIONAL:  Perform stack bounds checking on new message invocations.  If   */
/* this is a non-stack based calling convention, or it is not possible to     */
/* determine the bounds of the stack, leave this undefined.                   */
/******************************************************************************/
#define STACKCHECK

/******************************************************************************/
/* OPTIONAL:  No time slicing support for REXX activation yields is available.*/
/* Code will yield after a given count of instructions.                       */
/******************************************************************************/
//
// timers in windows only get called back at yield boundaries
//
//#define NOTIMER                        // used in activa, activi for
                                        // count based yielding

/******************************************************************************/
/* OPTIONAL:  Enable concurrency timeslice dispatching support.  Default is   */
/* only yield at specific event points.                                       */
/******************************************************************************/
// We're using a timeslicing sharing model with fixed timer support.
#define TIMESLICE
#define FIXEDTIMERS

/******************************************************************************/
/* OPTIONAL:  If the implementation enables external scripting support, then  */
/* additional hand-shaking with an the exernal environment is enabled for     */
/* providing default values for uninitialized variables.                      */
/******************************************************************************/
#define SCRIPTING

/******************************************************************************/
/* OPTIONAL:  Overrides for any functions defined in sysdef.h.  These         */
/* can map the calls directly to inline code or comment them out all together.*/
/******************************************************************************/

#define DEFRXSTRING 256                /* Default RXSTRING return size      */

// Options character used for command line programs
#define OPT_CHAR  '/'                  /* Option char for win                */

#define OLDMETAVERSION       29        /* highest meta version number:                  */

/******************************************************************************/
/* REQUIRED:  define the path delimiter and end of line information           */
/* appropriate for this file system.                                          */
/******************************************************************************/

#define delimiter '\\'
#define line_end "\r\n"
#define line_end_size 2

#endif

