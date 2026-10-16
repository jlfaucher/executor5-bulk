/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2026 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
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
/* REXX Kernel                                                                */
/*                                                                            */
/* Primitive Rexx Unicode String Indexer                                      */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "ProtectedObject.hpp"
#include "ActivityManager.hpp" // for reportException
#include "MethodArguments.hpp"

#include "UnicodeStringIndexer.hpp"
#include "RexxUnicodeStringIndexer.hpp"

// singleton class instance
RexxClass *RexxUnicodeStringIndexer::classInstance = OREF_NULL;

/**
 * Create initial bootstrap objects
 */
void RexxUnicodeStringIndexer::createInstance()
{
    CLASS_CREATE(RexxUnicodeStringIndexerNative);
}


// CLASS methods of the RexxUnicodeStringIndexer class

/**
 * Create a non-incremental string indexer object from Rexx code.
 *
 * @param msgArgs  The pointer to the new arguments.
 * @param argCount The argument count.
 *
 * @return A new instance of the .RexxStringIndexer class.
 */
RexxObject *RexxUnicodeStringIndexer::newRexx(RexxObject **msgArgs, size_t argCount)
{
    // this class is defined on the object class, but this is actually attached
    // to a class object instance.  Therefore, any use of the this pointer
    // will be touching the wrong data.  Use the classThis pointer for calling
    // any methods on this object from this method.
    RexxClass *classThis = (RexxClass *)this;

    size_t num_args = argCount;

    // the first argument is required
    if (num_args < 1 )
    {
        reportException(Error_Incorrect_method_minarg,  IntegerOne);
    }

    // The receiver and the message name are required
    RexxString *_string   = (RexxString *)msgArgs[0];
    requiredArgument(_string, "string");

    // we're all ready to create this now
    RexxUnicodeStringIndexer *stringIndexer = new RexxUnicodeStringIndexer(_string);
    ProtectedObject p(stringIndexer);

    // handle Rexx class completion
    classThis->completeNewObject(stringIndexer);

    return stringIndexer;
}


// End of Class methods for the Array class


/**
 * Allocate a new RexxUnicodeStringIndexer object
 *
 * @param size   The size of the object.
 *
 * @return The newly allocated object.
 */
void *RexxUnicodeStringIndexer::operator new(size_t size)
{
    return new_object(size, T_RexxUnicodeStringIndexerNative);
}


/**
 * Constructor for a RexxUnicodeStringIndexer object.
 *
 * @param s      The string to index.
 */
RexxUnicodeStringIndexer::RexxUnicodeStringIndexer(RexxString *s)
{
    string = s;
}


/**
 * Generalized object marking.
 *
 * @param reason The reason for this live marking operation.
 */
void RexxUnicodeStringIndexer::live(size_t liveMark)
{
    memory_mark(objectVariables);
    memory_mark(string);
}


/**
 * Generalized object marking.
 *
 * @param reason The reason for this live marking operation.
 */
void RexxUnicodeStringIndexer::liveGeneral(MarkReason reason)
{
    memory_mark_general(objectVariables);
    memory_mark_general(string);
}
