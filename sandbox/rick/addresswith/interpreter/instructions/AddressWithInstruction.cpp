/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
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
/* REXX Translator                               AddressWithInstruction.cpp   */
/*                                                                            */
/* Implementation of the ADDRESS WITH instruction                             */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "StringClass.hpp"
#include "RexxActivation.hpp"
#include "AddressWithInstruction.hpp"
#include "SystemInterpreter.hpp"
#include "MethodArguments.hpp"

/**
 * Constructor for an Address instruction object.
 *
 * @param _environment
 *                 A static environment name.
 * @param _command A command expression to be issued.
 */
RexxInstructionAddressWith::RexxInstructionAddressWith(RexxInternalObject *_expression,
    RexxString *_environment, RexxInternalObject *_command)
{
    environment = _environment;
    command = _command;
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void RexxInstructionAddressWith::live(size_t liveMark)
{
    // must be first one marked
    memory_mark(nextInstruction);
    memory_mark(environment);
    memory_mark(command);
    memory_mark(inputSource);
    memory_mark(outputTarget);
    memory_mark(errorTarget);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void RexxInstructionAddressWith::liveGeneral(MarkReason reason)
{
    // must be first one marked
    memory_mark_general(nextInstruction);
    memory_mark_general(environment);
    memory_mark_general(command);
    memory_mark_general(outputTarget);
    memory_mark_general(errorTarget);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void RexxInstructionAddressWith::flatten(Envelope *envelope)
{
    setUpFlatten(RexxInstructionAddressWith)

    flattenRef(nextInstruction);
    flattenRef(environment);
    flattenRef(command);
    flattenRef(inputSource);
    flattenRef(outputTarget);
    flattenRef(outputTarget);

    cleanUpFlatten
}

/**
 * Execute an addres instruction.
 *
 * @param context The current program execution context.
 * @param stack   The current evaluation stack.
 */
void RexxInstructionAddressWith::execute(RexxActivation *context, ExpressionStack *stack )
{
    // trace if necessary
    context->traceInstruction(this);
    // Address With always has a static name. We also have a command to issue, which
    // is a general expression, as well as potential expressions for each of the
    // redirect types
    RexxObject *result = command->evaluate(context, stack);
    // this must be a string
    RexxString *_command = result->requestString();
    // protect this
    stack->push(_command);
    // need to trace this if on
    context->traceResult(_command);
    // validate the address name using system rules
    SystemInterpreter::validateAddressName(environment);

    // create the runtime I/O context object and fill in the needed sections
    Protected<CommandIOContext> ioContext = new ProtectedIOContext();

    // we have an input source, so evaluate it and create
    // an input object that handles that source type
    if (inputSource != OREF_NULL)
    {
        RexxObject *inputObject = inputSource->evaluate();
        // need to trace this if on
        context->traceResult(inputObject);
        ioContext->input = createInputSource(inputObject, inputType);
    }
    if (outputTarget != OREF_NULL)
    {
        RexxObject *outputObject = outputTarget->evaluate();
        // need to trace this if on
        context->traceResult(outputObject);
        ioContext->output = createOutputTarget(outputObject, outputType);
    }

    if (errorTarget != OREF_NULL)
    {
        RexxObject *errorObject = errorTarget->evaluate();
        // need to trace this if on
        context->traceResult(errorObject);
        ioContext->error = createOutputTarget(errorObject, errorType);
    }
    else
    {
        // if an error handler has not been specified, everything
        // goes to the output handler (which might be null also)
        ioContext->error = ioContext->output;
    }

    // and execute the command
    context->command(environment, _command, ioContext);

    // perform any post command cleanup
    ioContext->cleanup();
}


/**
 * Create an input source redirector from an evaluated
 * source object.
 *
 * @param inputObject
 *               The evaluated source object
 * @param type   The type specified on the ADDRESS WITH instruction. This
 *               can specify that an explicit type of object is required
 *               or we can dynamically determine appropriate source from
 *               the type of object.
 *
 * @return An appropriate input source for the object.
 */
InputRedirector *RexxInstructionAddressWith::createInputSource(RexxObject *inputObject, RedirectionType type)
{
    // process based on the type of object we've been presented with.
    switch (type)
    {
        // if STEM was specified, then the syntax checks requires a STEM variable
        // symbol follow the option. Therefore, we know this will have evaluated
        // to stem object. This is the easiest of the checks;
        case STEM_VARIABLE:
        {
            Protected<InputRedirector> redirector = new StemInputSource((StemClass *)inputObject);
            // initialize the source, which will confirm that the stem variable
            // follows the stem.0 array convention
            redirector->init();
            return redirector;
        }
        // a stream specified by name. This must be convertable to string form and
        // will be used to create a stream object.
        case STREAM_NAME:
        {
            // this must be a string
            Protected<RexxString> streamName = inputObject->requestString();

            Protected<InputRedirector> redirector = new StreamInputSource(streamName);
            // initialize the source, which will confirm that the stream exists
            // and can be opened
            redirector->init();
            return redirector;
        }

        // this was ADDRESS .. WITH INPUT USING expr. We dynamically determine from
        // the type what to do.
        default:
        {
            // Checks proceed in the following order, with the result
            // 1) string object. This will be a single line written to the input pipe
            // (uses the array input source to produce).
            // 2) stem object. This is handled like the explicit STEM option.
            // 3) stream object. This is a stream input source.
            // 4) a makearray is performed on the object. If convertible, we use
            // an array source.

            if (::isString(inputObject))
            {
                Protected<ArrayClass> source = new_array(inputObject);
                Protected<InputRedirector> redirector = new ArrayInputSource(inputObject);
                // initialize the source, which sets up initial positions
                redirector->init();
                return redirector;
            }
            // an evaluated stem object
            if (::isStem(inputObject))
            {
                Protected<InputRedirector> redirector = new StemInputSource((StemClass *)inputObject);
                // initialize the source, which will confirm that the stem variable
                // follows the stem.0 array convention
                redirector->init();
                return redirector;
            }

            // Now we need to check for input streams
            RexxClass *streamClass = TheRexxPackage->findClass(GlobalNames::INPUTSTREAM);
            if (inputObject->instanceOf(streamClass))
            {

                Protected<InputRedirector> redirector = new StreamObjectInputSource(streamName);
                // initialize the source, which will confirm that the stream exists
                // and can be opened
                redirector->init();
                return redirector;
            }

            // this must be convertable to some sort of array past this point
            Protected<ArrayClass> *array;
            if (isArray(inputObject))
            {
                array = ((ArrayClass *)inputObject)->makeArray();
            }
            else
            {
                // some other type of collection, use the less direct means
                // of requesting an array
                array = inputObject->requestArray();
                // raise an error if this did not convert ok, or we got
                // back something other than a real Rexx array.
                if (!isArray(array))
                {
                    reportException(Error_Execution_inputsource, inputObject);
                }
            }
            Protected<InputRedirector> redirector = new ArrayInputSource(array);
            // initialize the source, which sets up initial positions
            redirector->init();
            return redirector;
        }
    }
    // nothing to create
    return OREF_NULL;
}


/**
 * Create an appropriate output target for an ADDRESS WITH
 * instruction.
 *
 * @param outputObject
 *               The resolved output target
 * @param type   The specified type of target
 *
 * @return A resolved and constructed output target.
 */
OutputRedirector *RexxInstructionAddressWith::createOutputSource(RexxObject *outputObject, RedirectionType type, OutputOption, option)
{
    switch (type)
    {
        case STEM_VARIABLE:
        {
            Protected<OutputRedirector> redirector = new StemOutputTarget((StemClass *)outputObject);
            // initialize the source, which will confirm that the stem variable
            // follows the stem.0 array convention
            redirector->init(option);
            return redirector;
        }
        case STREAM_NAME:
        {
            // this must be a string
            Protected<RexxString> streamName = outputObject->requestString();

            Protected<OutputRedirector> redirector = new StreamOutputTarget(streamName);
            // initialize the source, which will confirm that the stream exists
            // and can be opened
            redirector->init(option);
            return redirector;
        }
        default:
        {
            // an evaluated stem object
            if (::isStem(outputObject))
            {
                Protected<OutputRedirector> redirector = new StemOutputTarget((StemClass *)outputObject);
                // initialize the source, which will confirm that the stem variable
                // follows the stem.0 array convention
                redirector->init(option);
                return redirector;
            }

            // Now we need to check for output streams
            RexxClass *streamClass = TheRexxPackage->findClass(GlobalNames::OUTPUTSTREAM);
            if (outputObject->instanceOf(streamClass))
            {
                Protected<InputRedirector> redirector = new StreamObjectOutputTarget(outputObject);
                // initialize the source, which will confirm that the stream exists
                // and can be opened
                redirector->init(option);
                return redirector;
            }

            // Now some sort of ordered collection
            RexxClass *orderedCollection = TheRexxPackage->findClass(GlobalNames::ORDEREDCOLLECTION);
            if (outputObject->instanceOf(orderedCollection))
            {
                Protected<InputRedirector> redirector = new CollectionOutputTarget(outputObject);
                // initialize the source, which will confirm that the stream exists
                // and can be opened
                redirector->init(option);
                return redirector;
            }
            // an unknown type of target
            reportException(Error_Execution_output_target, outputObject);
        }
    }
    return OREF_NULL;
}
