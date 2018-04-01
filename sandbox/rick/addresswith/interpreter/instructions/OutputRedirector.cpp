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
#include "InputRedirector.hpp"
#include "OutputRedirector.hpp"
#include "Activity.hpp"
#include "ActivityManager.hpp"
#include "ProtectedObject.hpp"
#include "PackageClass.hpp"
#include "StemClass.hpp"


/**
 * Test if an output redirector is using the same source
 * as the input redirector, and thus needs buffering.
 *
 * @param in     The input redirector
 *
 * @return True if the input redirector and the output redirector
 *         are using the same target object.
 */
bool OutputRedirector::needsBuffering(InputRedirector *in)
{
    return type() == in->type() && target() == in->target();
}


/**
 * Test if an output redirector is using the same target as the
 * error redirector.
 *
 * @param e      The error redirector
 *
 * @return True if the error redirector and the output
 *         redirector are using the same target object.
 */
bool OutputRedirector::isSameTarget(OutputRedirector *e)
{
    return type() == e->type() && target() == e->target();
}


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
 * @param o         The append/replace option for the redirect.
 */
StemOutputTarget::StemOutputTarget(StemClass *s, OutputOption::Enum o)
{
    stem = s;
    option = o;
    initialized = false;
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
 */
void StemOutputTarget::init()
{
    // only one per customer!
    if (initialized)
    {
        return;
    }
    initialized = true;
    // replace is the easy way to handle we just reset everything
    if (option == OutputOption::REPLACE || option == OutputOption::DEFAULT)
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
            // a valid whole number index
            if (!count->unsignedNumberValue(index, Numerics::ARGUMENT_DIGITS))
            {
                reportException(Error_Invalid_whole_number_stem_array_index, stem->getName(), count);
            }
            // we write to the next position
            index ++;
        }
    }
}


/**
 * Write a value to the output redirector.
 *
 * @param value  The string value to write
 */
void StemOutputTarget::write(RexxString *value)
{
    stem->setElement(index, value);
    // update stem.0
    Protected<RexxInteger> newIndex = new_integer(index);
    stem->setElement((size_t)0, newIndex);
    // and step to the next position
    index++;
}


/**
 * Allocate a new stem output target
 *
 * @param size   The size of the object.
 *
 * @return Storage for creating the object.
 */
void *StreamObjectOutputTarget::operator new(size_t size)
{
    return new_object(size, T_StreamObjectOutputTarget);
}


/**
 * Construct a output target ogject
 *
 * @param stem      The stem oubect being used
 * @param o         The append/replace option for the redirect.
 */
StreamObjectOutputTarget::StreamObjectOutputTarget(RexxObject *s, OutputOption::Enum o)
{
    stream = s;
    option = o;
    initialized = false;
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void StreamObjectOutputTarget::live(size_t liveMark)
{
    memory_mark(stream);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void StreamObjectOutputTarget::liveGeneral(MarkReason reason)
{
    memory_mark_general(stream);
}


/**
 * Write a value to the output redirector.
 *
 * @param value  The string value to write
 */
void StreamObjectOutputTarget::write(RexxString *value)
{
    // add the string to the next position
    ProtectedObject result;
    // this just uses lineout
    stream->sendMessage(GlobalNames::LINEOUT, value, result);
}


/**
 * Allocate a new output target
 *
 * @param size   The size of the object.
 *
 * @return Storage for creating the object.
 */
void *StreamOutputTarget::operator new(size_t size)
{
    return new_object(size, T_StreamOutputTarget);
}


/**
 * Construct a output target ogject
 *
 * @param n         The stream name
 * @param o         The append/replace option for the redirect.
 */
StreamOutputTarget::StreamOutputTarget(RexxString *n, OutputOption::Enum o)
{
    name = n;
    option = o;
    initialized = false;
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void StreamOutputTarget::live(size_t liveMark)
{
    memory_mark(stream);
    memory_mark(name);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void StreamOutputTarget::liveGeneral(MarkReason reason)
{
    memory_mark_general(stream);
    memory_mark_general(name);
}


/**
 * Initialize and validate the stream condition for updates.
 *
 * @param option The append/replace option for the redirect.
 */
void StreamOutputTarget::init()
{
    // only one per customer!
    if (initialized)
    {
        return;
    }
    initialized = true;

    // create an instance of the stream class for the given name
    RexxClass *streamClass = TheRexxPackage->findClass(GlobalNames::STREAM);
    ProtectedObject result;
    stream = streamClass->sendMessage(GlobalNames::NEW, name, result);

    RexxString *openResult = OREF_NULL;

    // If replace is specified, we open this WRITE REPLACE
    if (option == OutputOption::REPLACE || option == OutputOption::DEFAULT)
    {
        openResult = (RexxString *)stream->sendMessage(GlobalNames::OPEN, GlobalNames::WRITE_REPLACE, result);
    }
    // asked to append...just a different open option
    else
    {
        openResult = (RexxString *)stream->sendMessage(GlobalNames::OPEN, GlobalNames::WRITE_APPEND, result);
    }
    // the open should return ready
    if (!openResult->strCompare(GlobalNames::READY))
    {
        reportException(Error_Execution_file_not_writeable, name, openResult);
    }
}


/**
 * Perform post-command housekeeping on this operation.
 */
void StreamOutputTarget::cleanup()
{
    ProtectedObject result;
    stream->sendMessage(GlobalNames::CLOSE, result);
}


/**
 * Allocate a new output target
 *
 * @param size   The size of the object.
 *
 * @return Storage for creating the object.
 */
void *CollectionOutputTarget::operator new(size_t size)
{
    return new_object(size, T_CollectionOutputTarget);
}


/**
 * Construct a output target ogject
 *
 * @param n         The stream name
 * @param o         The append/replace option for the redirect.
 */
CollectionOutputTarget::CollectionOutputTarget(RexxObject *c, OutputOption::Enum o)
{
    collection = c;
    option = o;
    initialized = false;
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void CollectionOutputTarget::live(size_t liveMark)
{
    memory_mark(collection);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void CollectionOutputTarget::liveGeneral(MarkReason reason)
{
    memory_mark_general(collection);
}


/**
 * Write a value to the output redirector.
 *
 * @param value  The string value to write
 */
void CollectionOutputTarget::write(RexxString *value)
{
    ProtectedObject result;
    // this just uses lineout
    collection->sendMessage(GlobalNames::APPEND, value, result);
}


/**
 * Initialize and validate the stream condition for updates.
 *
 * @param option The append/replace option for the redirect.
 */
void CollectionOutputTarget::init()
{
    // only one per customer!
    if (initialized)
    {
        return;
    }
    initialized = true;

    // If replace is specified, then empty the collection
    if (option == OutputOption::REPLACE)
    {
        ProtectedObject result;
        // this just uses lineout
        collection->sendMessage(GlobalNames::EMPTY, result);
    }
}


/**
 * Allocate a new output target
 *
 * @param size   The size of the object.
 *
 * @return Storage for creating the object.
 */
void *BufferingOutputTarget::operator new(size_t size)
{
    return new_object(size, T_BufferingOutputTarget);
}


/**
 * Construct a output target ogject
 *
 * @param n         The stream name
 * @param o         The append/replace option for the redirect.
 */
BufferingOutputTarget::BufferingOutputTarget(OutputRedirector *t)
{
    target = t;
    initialized = false;
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void BufferingOutputTarget::live(size_t liveMark)
{
    memory_mark(collector);
    memory_mark(target);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void BufferingOutputTarget::liveGeneral(MarkReason reason)
{
    memory_mark_general(collector);
    memory_mark_general(target);
}


/**
 * Initialize and validate the stream condition for updates.
 *
 * @param option The append/replace option for the redirect.
 */
void BufferingOutputTarget::init()
{
    // only one per customer!
    if (initialized)
    {
        return;
    }
    initialized = true;
    collector = new_array();
}


/**
 * Add the output item to our buffer
 *
 * @param value  The string value to add.
 */
void BufferingOutputTarget::write(RexxString *value)
{
    collector->append(value);
}


/**
 * Perform post-command housekeeping on this operation.
 */
void BufferingOutputTarget::cleanup()
{
    // we've hit the end of things, so now the target redirector
    // gets activated and pumped full of data

    target->init();
    size_t count = collector->items();

    for (size_t i = 1; i <= count; i++)
    {
        target->write((RexxString *)collector->get(i));
    }

    // and finally the cleanup
    target->cleanup();
}
