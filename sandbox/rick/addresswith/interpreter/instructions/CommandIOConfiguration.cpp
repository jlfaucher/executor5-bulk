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
/*                                                 CommandIOConfiguration.cpp */
/*                                                                            */
/* A configuration for issuing commands with I/O redirection                  */
/*                                                                            */
/******************************************************************************/
#include "CommandIOConfiguration.hpp"


/**
 * Allocate a CommandIOConfiguration object
 *
 * @param size   The base size of this object.
 *
 * @return A newly allocated Rexx object.
 */
void  *CommandIOConfiguration::operator new(size_t size)
{
    return new_object(size, T_CommandIOConfiguration);
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void CommandIOConfiguration::live(size_t liveMark)
{
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
void CommandIOContext::liveGeneral(MarkReason reason)
{
    memory_mark_general(inputSource);
    memory_mark_general(outputTarget);
    memory_mark_general(errorTarget);
}


/**
 * Create an active IO redirection context for issuing
 * a command.
 *
 * @param context The current activation context the command is running under
 * @param stack   The expression stack used for evaluations
 * @param commandConfig
 *                A configuration explicitly specified with this command
 *                which can override the global configuration.
 *
 * @return An instance of an I/O context for this configuration.
 */
CommandIOContext *CommandIOConfiguration::createIOContext(RexxActivation *context, ExpressionStack *stack, CommandIOConfiguration *commandConfig)
{
    // create the runtime I/O context object and fill in the needed sections
    Protected<CommandIOContext> ioContext = new ProtectedIOContext();

    // we use a chaining strategy here. If we have a configuration from the command, then we ask
    // it to merge each item. Otherwise we create the item directly from us

    // the local configuration takes priority, so try that first.
    if (commandConfig != OREF_NULL)
    {
        ioContext->input = commandConfig->createInputSource(context, stack, this);
        ioContext->output = commandConfig->createOutputSource(context, stack, this);
        ioContext->error = commandConfig->createErrorSource(context, stack, this);
    }
    // we just use what we already have
    else
    {
        ioContext->input = createInputSource(context, stack);
        ioContext->output = createOutputSource(context, stack);
        ioContext->error = createErrorSource(context, stack);
    }

    // if the same objects are getting used for both input and output, then
    // we need to insert a buffering layer in front of the output object.
    ioContext->resolveConflicts();
}


/**
 * Create a input source by resolving between a configuration
 * on an individual ADDRESS command and the previous set
 * global environment.
 *
 * @param context    The execution context
 * @param stack      The current expression stack
 * @param mainConfig The global configuration (if any) we use for the
 *                   merge.
 *
 * @return An appropriate INPUT object from this configuration.
 */
InputRedirector *CommandIOConfiguration::createInputSource(RexxActivation *context, ExpressionStack *stack, CommandIOConfiguration *mainConfig)
{
    // if we're explicitly configured for NORMAL, this is
    // a reset
    if (inputType == NORMAL)
    {
        return OREF_NULL;
    }

    // if we have a configuration, return it
    if (inputSource != OREF_NULL)
    {
        return createInputSource(context, stack);
    }

    // create from the main configuration
    return mainConfig->createInputSource(context, stack);
}


/**
 * Create an output source by resolving between a configuration
 * on an individual ADDRESS command and the previous set global
 * environment.
 *
 * @param context    The execution context
 * @param stack      The current expression stack
 * @param mainConfig The global configuration (if any) we use for the
 *                   merge.
 *
 * @return An appropriate OUTPUT object from this configuration.
 */
OutputRedirector *CommandIOConfiguration::createOutputSource(RexxActivation *context, ExpressionStack *stack, CommandIOConfiguration *mainConfig)
{
    // if we're explicitly configured for NORMAL, this is
    // a reset
    if (outputType == NORMAL)
    {
        return OREF_NULL;
    }

    // if we have a configuration, return it
    if (outputSource != OREF_NULL)
    {
        return createOutputSource(context, stack);
    }

    // create from the main configuration
    return mainConfig->createOutputSource(context, stack);
}


/**
 * Create an error source by resolving between a configuration
 * on an individual ADDRESS command and the previous set global
 * environment.
 *
 * @param context    The execution context
 * @param stack      The current expression stack
 * @param mainConfig The global configuration (if any) we use for the
 *                   merge.
 *
 * @return An appropriate ERROR object from this configuration.
 */
OutputRedirector *CommandIOConfiguration::createErrorSource(RexxActivation *context, ExpressionStack *stack, CommandIOConfiguration *mainConfig)
{
    // if we're explicitly configured for NORMAL, this is
    // a reset
    if (outputType == NORMAL)
    {
        return OREF_NULL;
    }

    // if we have a configuration, return it
    if (outputSource != OREF_NULL)
    {
        return createErrorSource(context, stack);
    }

    // create from the main configuration
    return mainConfig->createErrorSource(context, stack);
}


/**
 * Create an input source redirector from an evaluated
 * source object.
 *
 * @param context The current execution context.
 * @param stack   The current context stack.
 *
 * @return An appropriate input source for the object.
 */
InputRedirector CommandIOConfiguration::createInputSource(RexxActivation *context, ExpressionStack *stack)
{
    // if there's nothing configured, just return null
    if (inputSource == OREF_NULL)
    {
        return OREF_NULL;
    }

    RexxObject *inputObject = inputSource->evaluate(context, stack);
    // need to trace this if on
    context->traceResult(inputObject);

    // process based on the type of object we've been presented with.
    switch (inputType)
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
 * Create the output target for this configuration.
 *
 * @param context The current execution context.
 * @param stack   The current context stack.
 *
 * @return a configured output redirection object
 */
OutputRedirector *CommandIOConfiguration::createOutputTarget(RexxActivation *context, ExpressionStack *stack)
{
    // if no target configured, return nothing
    if (outputTarget == OREF_NULL)
    {
        return OREF_NULL;
    }

    return createOutputTarget(context, stack, outputTarget, outputType, outputOption);
}


/**
 * Create the output target for this configuration.
 *
 * @param context The current execution context.
 * @param stack   The current context stack.
 *
 * @return a configured output redirection object
 */
OutputRedirector *CommandIOConfiguration::createErrorTarget(RexxActivation *context, ExpressionStack *stack)
{
    // if no target configured, return nothing
    if (errorTarget == OREF_NULL)
    {
        return OREF_NULL;
    }

    return createOutputTarget(context, stack, errorTarget, errorType, errorOption);
}



/**
 * Create an appropriate output target for an ADDRESS WITH
 * instruction.
 *
 * @param context The current execution context.
 * @param stack   The current context stack.
 * @param outputObject
 *               The resolved output target
 * @param type   The specified type of target
 *
 * @return A resolved and constructed output target.
 */
OutputRedirector *CommandIOConfiguration::createOutputSource(RexxActivation *context, ExpressionStack *stack, RexxInternalObject *outputTarget, RedirectionType type, OutputOption, option)
{
    RexxObject *outputObject = outputTarget->evaluate(context, stack);
    // need to trace this if on
    context->traceResult(outputObject);
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
