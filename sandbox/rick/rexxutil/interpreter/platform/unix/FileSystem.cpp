/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2019 Rexx Language Association. All rights reserved.    */
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
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/* REXX Unix support                                                          */
/*                                                                            */
/* Unix specific file related routines.                                       */
/*                                                                            */
/******************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "RexxCore.h"
#include "StringClass.hpp"
#include "BufferClass.hpp"
#include "ProtectedObject.hpp"
#include "Interpreter.hpp"
#include "SystemInterpreter.hpp"
#include "InterpreterInstance.hpp"
#include "SysFileSystem.hpp"
#include "ActivityManager.hpp"
#include "FileNameBuffer.hpp"
#include "SysProcess.hpp"
#include "SysFile.hpp"

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>
#include <limits.h>

#if defined( HAVE_SYS_FILIO_H )
# include <sys/filio.h>
#endif

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef HAVE_STROPTS_H
# include <stropts.h>
#endif


/**
 * Read a program, returning a buffer object.
 *
 * @param file_name The target file name.
 *
 * @return A buffer object holding the program data.
 */
BufferClass* SystemInterpreter::readProgram(const char *file_name)
{
    SysFile programFile;          // the file we're reading

    // if unable to open this, return false
    if (!programFile.open(imageFile, RX_O_RDONLY, RX_S_IREAD, RX_SH_DENYWR))
    {
        return OREF_NULL;
    }

    // get the size of the file
    int64_t bufferSize = programFile.getSize();
    int64_t readSize;

    // get a buffer object to return the image
    Protected<BufferClass> buffer = new_buffer(bufferSize);
    {
        UnsafeBlock releaser;

        // read the data
        programFile.read(buffer->getData(), bufferSize, readSize);
        programFile.close();
    }
    // if there was a read error, return nothing
    if (readSize < bufferSize)
    {
        return OREF_NULL;
    }
    // ready to run
    return buffer;
}




