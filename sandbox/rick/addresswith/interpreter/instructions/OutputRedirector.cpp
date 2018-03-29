/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2018 Rexx Language Association. All rights reserved.    */
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
/* REXX Kernel                                                                */
/*                                                                            */
/* OutputRedirector implementations for the ADDRESS WITH instruction          */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#include "OutputRedirectors.hpp"


/**
 * Allocate a new stem output target
 *
 * @param size   The size of the object.
 *
 * @return Storage for creating the object.
 */
void *StemOutputTarget::operator new(size_t size)
{
    return new_object(size, T_StemOutputTarget);
}


/**
 * Construct a output target ogject
 *
 * @param stem      The stem oubect being used
 */
StemOutputTarget::StemOutputTarget(RexxObject *s)
{
    stem = s;
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void StemOutputTarget::live(size_t liveMark)
{
    memory_mark(stem);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void StemOutputTarget::liveGeneral(MarkReason reason)
{
    memory_mark_general(stem);
}


/**
 * Initialize and validate the stem condition for updates.
 *
 * @param option The append/replace option for the redirect.
 */
void StemOutputTarget::init(OutputOption option)
{
    // replace is the easy way to handle we just reset everything
    if (option == REPACE)
    {
        // empty everything out (could already be empty
        stem->empty();
        // set the stem.0 value to zero
        stem->setElement((size_t)0, IntegerZero);
        // our next element to write is 1
        index = 1;
    }
    // asked to append...this is a little more work.
    else
    {
        // get the stem.0 value
        RexxObject *count = stem->getElement(size_t(0));
        // if completely not there, then we'll just treat this like a replace
        if (count == OREF_NULL)
        {
            // empty everything out (could already be empty
            stem->empty();
            // set the stem.0 value to zero
            stem->setElement((size_t)0, IntegerZero);
            // our next element to write is 1
            index = 1;
        }
        // the stem.0 value must be an integer size. We'll start writing at the next position
        else
        {

        }
    }
}


/**
 * Write a value to the output redirector.
 *
 * @param value  The string value to write
 */
void StemOutputTarget::write(const char *value, size_t len)
{
    // add the string to the next position
    Protected<RexxString> next = new_string(value, len);
    stem->setElement(index, next);
    // update stem.0
    Protected<RexxInteger> newIndex = new_interger(index);
    stem->setElement((size_t)0, newIndex);
    // and step to the next position
    index++;
}
