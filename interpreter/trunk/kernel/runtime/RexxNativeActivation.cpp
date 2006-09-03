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
/* REXX Kernel                                                  RexxNativeActivation.c    */
/*                                                                            */
/* Primitive Native Activation Class                                          */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#include "StringClass.hpp"
#include "ArrayClass.hpp"
#include "DirectoryClass.hpp"
#include "MethodClass.hpp"
#include "RexxNativeCode.hpp"
#include "RexxActivation.hpp"
#include "RexxNativeActivation.hpp"
#include "RexxBuffer.hpp"
#include "MessageClass.hpp"
#include "RexxVariableDictionary.hpp"
#include "SourceFile.hpp"
#include "RexxCode.hpp"
#include "RexxInstruction.hpp"
#include "ExpressionBaseVariable.hpp"
#include "PointerClass.hpp"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"

#include "ASCIIDBCSStrings.hpp"
#include "oorexx.h"

void * RexxNativeActivation::operator new(size_t size)
/******************************************************************************/
/* Function:  Create a new native activation object                           */
/******************************************************************************/
{
    return new_object(size, TheNativeActivationBehaviour);
}

RexxNativeActivation::RexxNativeActivation(
     RexxObject         * receiver,    /* receiver object                   */
     RexxMethod         * method,      /* method to run                     */
     RexxActivity       * a,           /* current activity                  */
     RexxString         * m)           /* invoked message                   */
/******************************************************************************/
/* Function:  Create a new native activation object                           */
/******************************************************************************/
{
  u_receiver = receiver;    /* the receiving object              */
  method = method;          /* the method to run                 */
  activity = a;             /* the activity running on           */
  msgname = m;              /* the message name                  */
  argcount = 0;             /* no arguments until we've been given some */
  activationType = METHOD_ACTIVATION;
  resetNext();              // initialize the vp state
}

RexxNativeActivation::RexxNativeActivation(
     RexxMethod         * method,      /* method to run                     */
     RexxActivity       * a,           /* current activity                  */
     RexxString         * n)           /* invoked call name                 */
/******************************************************************************/
/* Function:  Create a new native activation object                           */
/******************************************************************************/
{
  u_receiver = OREF_NULL;   /* the receiving object              */
  method = method;          /* the method to run                 */
  activity = a;             /* the activity running on           */
  msgname = n;              /* the message name                  */
  argcount = 0;             /* no arguments until we've been given some */
  activationType = FUNCTION_ACTIVATION;
  resetNext();              // initialize the vp state
}

RexxNativeActivation::RexxNativeActivation(
     RexxActivity       * a)           /* current activity                  */
/******************************************************************************/
/* Function:  Create a new native activation object                           */
/******************************************************************************/
{
  u_receiver = OREF_NULL;   /* the receiving object              */
  method = OREF_NULL;       /* the method to run                 */
  activity = a;             /* the activity running on           */
  msgname = OREF_NULL;      /* the message name                  */
  argcount = 0;             /* no arguments until we've been given some */
                            // this is a "top of the stack" activation
  activationType = PROGRAM_ACTIVATION;
  resetNext();              // initialize the vp state
}

RexxNativeActivation::RexxNativeActivation(
     RexxActivity       * a,           // the current activation
     RexxActivation     * c,           // the calling context
     ActivationType       t)           // the activation reason
/******************************************************************************/
/* Function:  Create a new native activation object                           */
/******************************************************************************/
{
  u_receiver = OREF_NULL;   /* the receiving object              */
  method = OREF_NULL;       /* the method to run                 */
  activity = a;             /* the activity running on           */
  context = c;              // save our calling context too
  msgname = OREF_NULL;      /* the message name                  */
  argcount = 0;             /* no arguments until we've been given some */
  activationType = t;       // save the activation type so we know what we're doing
  resetNext();              // initialize the vp state

  // if this is an exit or external command activation, we need to
  // initialize the call context so we can hand this out on
  // classic-style API callbacks.
  if (t == EXIT_ACTIVATION || t == EXTERNAL_COMMAND)
  {
      activity->createCallContext(&callContext, this);
      // these calls have no arguments.
      callContext.threadContext.arguments = NULL;
  }
}

void RexxNativeActivation::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->argArray);
  memory_mark(this->u_receiver);
  memory_mark(this->context);
  memory_mark(this->method);
  memory_mark(this->activity);
  memory_mark(this->msgname);
  memory_mark(this->savelist);
  memory_mark(this->errorListener);
  memory_mark(this->result);
  memory_mark(this->nextstem);
  memory_mark(this->compoundelement);
  memory_mark(this->nextcurrent);
  memory_mark(this->objectVariables);

  /* We're hold a pointer back to our arguments directly where they */
  /* are created.  Since in some places, this argument list comes */
  /* from the C stack, we need to handle the marker ourselves. */
  size_t i;
  for (i = 0; i < argcount; i++) {
      memory_mark(arglist[i]);
  }
  cleanUpMemoryMark
}

void RexxNativeActivation::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->argArray);
  memory_mark_general(this->u_receiver);
  memory_mark_general(this->context);
  memory_mark_general(this->method);
  memory_mark_general(this->activity);
  memory_mark_general(this->msgname);
  memory_mark_general(this->savelist);
  memory_mark_general(this->errorListener);
  memory_mark_general(this->result);
  memory_mark_general(this->nextstem);
  memory_mark_general(this->compoundelement);
  memory_mark_general(this->nextcurrent);
  memory_mark_general(this->objectVariables);

  /* We're hold a pointer back to our arguments directly where they */
  /* are created.  Since in some places, this argument list comes */
  /* from the C stack, we need to handle the marker ourselves. */
  size_t i;
  for (i = 0; i < argcount; i++) {
      memory_mark_general(arglist[i]);
  }
  cleanUpMemoryMarkGeneral
}

void RexxNativeActivation::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
    // activations are never flattened, so this is a NOP;
}

void RexxNativeActivation::reportSignatureError()
/******************************************************************************/
/* Function:  Report a method signature error                                 */
/******************************************************************************/
{
    if (activationType == METHOD_ACTIVATION)
    {
        reportException(Error_Incorrect_method_signature);
    }
    else
    {
        reportException(Error_Incorrect_call_signature);
    }
}

void RexxNativeActivation::reportStemError(size_t position, RexxObject *object)
/******************************************************************************/
/* Function:  Report a method signature error                                 */
/******************************************************************************/
{
    if (activationType == METHOD_ACTIVATION)
    {
        reportException(Error_Incorrect_method_nostem, position + 1, object);
    }
    else
    {
        reportException(Error_Incorrect_call_nostem, position + 1, object);
    }
}


void RexxNativeActivation::processArguments(
    size_t       argcount,             /* argument count                    */
    RexxObject **arglist,              /* method argument list              */
    uint16_t    *argumentTypes,        // the type descriptor string
    ValueDescriptor *descriptors,      // descriptors for the arguments
    size_t       maximumArgumentCount) // our maximum argument count
/******************************************************************************/
/* Function:  Execute a REXX native method                                    */
/******************************************************************************/
{
    size_t inputIndex = 0;            // no arguments used yet             */
    size_t outputIndex = 1;           // we start filling in from the second (first is return value)
    bool usedArglist = false;         // if we request the argument list, then there's no requirement
                                      // to use up all of the passed arguments individually.

    // set up the return value descriptor
    descriptors[0].type = *argumentTypes;
    descriptors[0].value.value_int64_t = 0;
    // the first type in the signature is the return value, which we skip for now
    uint16_t *currentType = argumentTypes + 1;
    // now loop through the type information.  Some of the types are "created"
    // arguments that don't appear directly in the method/function invocation.
    // We can't directly do a 1-for-1 mapping
    for (; *currentType != REXX_ARGUMENT_TERMINATOR; currentType++)
    {
        // make sure we don't get a buffer overload
        if (outputIndex >= maximumArgumentCount)
        {
            reportSignatureError();
        }

        uint16_t type = ARGUMENT_TYPE(*currentType);
        bool isOptional = IS_OPTIONAL_ARGUMENT(*currentType);

        descriptors[outputIndex].type = type;           // fill in the type
        switch (type)
        {
            // reference to the receiver object...if this is a function call,
            // then this will be OREF NULL.
            case REXX_VALUE_OSELF:                /* reference to SELF                 */
            {
                // this doesn't make any sense for a function call
                if (activationType != METHOD_ACTIVATION)
                {
                    reportSignatureError();
                }
                // fill in the receiver object and mark it...
                descriptors[outputIndex].value.value_RexxObjectPtr = this->u_receiver;
                descriptors[outputIndex].flags = ARGUMENT_EXISTS | SPECIAL_ARGUMENT;
                break;
            }

            case REXX_VALUE_CSELF:                /* reference to CSELF                */
            {
                // this doesn't make any sense for a function call
                if (activationType != METHOD_ACTIVATION)
                {
                    reportSignatureError();
                }
                descriptors[outputIndex].value.value_POINTER = this->cself();
                descriptors[outputIndex].flags = ARGUMENT_EXISTS | SPECIAL_ARGUMENT;
                break;
            }

            case REXX_VALUE_BUFFER:               /* reference to Buffered storage     */
            {
                // this doesn't make any sense for a function call
                if (activationType != METHOD_ACTIVATION)
                {
                    reportSignatureError();
                }
                descriptors[outputIndex].flags = ARGUMENT_EXISTS | SPECIAL_ARGUMENT;
                descriptors[outputIndex].value.value_POINTER = this->buffer();
                break;
            }

            case REXX_VALUE_ARGLIST:              /* need the argument list            */
            {
                descriptors[outputIndex].flags = ARGUMENT_EXISTS | SPECIAL_ARGUMENT;
                descriptors[outputIndex].value.value_RexxArrayObject = getArguments();
                // we've used this
                usedArglist = true;
                break;
            }

            // This is either the message name used to invoke a method or
            // the name used to call a function
            case REXX_VALUE_NAME:
            {
                descriptors[outputIndex].flags = ARGUMENT_EXISTS | SPECIAL_ARGUMENT;
                descriptors[outputIndex].value.value_CSTRING = (CSTRING)this->msgname->getStringData();
                break;
            }

            // this is a real argument taken from the argument list
            default:                         /* still within argument bounds?     */
            {
                if (inputIndex < argcount && arglist[inputIndex] != OREF_NULL)
                {
                    // all of these arguments exist
                    descriptors[outputIndex].flags = ARGUMENT_EXISTS;
                    RexxObject *argument = arglist[inputIndex];    /* get the next argument             */
                    switch (*currentType)
                    {               /* process this type                 */

                        case REXX_VALUE_RexxObjectPtr:  /* arbitrary object reference        */
                        {
                            descriptors[outputIndex].value.value_RexxObjectPtr = argument;
                            break;
                        }

                        case REXX_VALUE_int:            /* integer value                     */
                        {
                            // convert and copy                  */
                            descriptors[outputIndex].value.value_int = (int)wholeNumberValue(argument, inputIndex, INT_MAX, INT_MIN);
                            break;
                        }

                        case REXX_VALUE_int8_t:            /* 8-bit integer value               */
                        {
                            descriptors[outputIndex].value.value_int8_t = (int8_t)wholeNumberValue(argument, inputIndex, INT8_MAX, INT8_MIN);
                            break;
                        }

                        case REXX_VALUE_int16_t:            /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_int16_t = (int16_t)wholeNumberValue(argument, inputIndex, INT16_MAX, INT16_MIN);
                            break;
                        }

                        case REXX_VALUE_int32_t:            /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_int32_t = (int32_t)wholeNumberValue(argument, inputIndex, INT32_MAX, INT32_MIN);
                            break;
                        }

                        case REXX_VALUE_int64_t:            /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_int64_t = (int64_t)int64Value(argument, inputIndex);
                            break;
                        }

                        case REXX_VALUE_uint8_t:            /* 8-bit integer value               */
                        {
                            descriptors[outputIndex].value.value_uint8_t = (uint8_t)unsignedNumberValue(argument, inputIndex, UINT8_MAX);
                            break;
                        }

                        case REXX_VALUE_uint16_t:            /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_uint16_t = (uint16_t)unsignedNumberValue(argument, inputIndex, UINT16_MAX);
                            break;
                        }

                        case REXX_VALUE_uint32_t:            /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_uint32_t = (uint32_t)unsignedNumberValue(argument, inputIndex, UINT32_MAX);
                            break;
                        }

                        case REXX_VALUE_uint64_t:            /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_uint64_t = (uint64_t)unsignedInt64Value(argument, inputIndex);
                            break;
                        }

                        case REXX_VALUE_size_t:            /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_size_t = (size_t)unsignedNumberValue(argument, inputIndex, SIZE_MAX);
                            break;
                        }

                        case REXX_VALUE_RexxBoolean:       /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_RexxBoolean = (RexxBoolean)argument->truthValue(Error_Logical_value_method);
                            break;
                        }

                        // The Rexx whole number one is checked against the human digits limit
                        case REXX_VALUE_RexxNumber:  /* number value                      */
                        {
                            descriptors[outputIndex].value.value_RexxNumber = (RexxNumber)wholeNumberValue(argument, inputIndex, Numerics::MAX_WHOLENUMBER, Numerics::MIN_WHOLENUMBER);
                            break;
                        }

                        // an unsigned string number value
                        case REXX_VALUE_RexxUnsignedNumber:
                        {
                            descriptors[outputIndex].value.value_RexxUnsignedNumber = (stringsize_t)unsignedNumberValue(argument, inputIndex, Numerics::MAX_STRINGSIZE);
                            break;
                        }

                        case REXX_VALUE_double:         /* double value                      */
                        {
                            descriptors[outputIndex].value.value_double = this->getDoubleValue(argument);
                            break;
                        }


                        case REXX_VALUE_float:          /* float value                      */
                        {
                            descriptors[outputIndex].value.value_float = (float)this->getDoubleValue(argument);
                            break;
                        }

                        case REXX_VALUE_CSTRING:        /* ASCII-Z string value              */
                        {
                            descriptors[outputIndex].value.value_CSTRING = this->cstring(argument);
                            break;
                        }

                        case REXX_VALUE_RexxStringObject: /* Required STRING object            */
                        {
                            /* force to a string value           */
                            RexxString *temp = REQUIRED_STRING(argument, inputIndex + 1) ;
                            // if this forced a string object to be created,
                            // we need to protect it here.
                            if (temp != argument)
                            {
                                                     /* make it safe                      */
                                createLocalReference(temp);
                            }
                            /* set the result in                 */
                            descriptors[outputIndex].value.value_RexxStringObject = temp;
                            break;

                        }

                        case REXX_VALUE_RexxArrayObject: /* Required ARRAY object            */
                        {
                            /* force to a string value           */
                            RexxArray *temp = REQUIRED_ARRAY(argument, inputIndex + 1) ;
                            // if this forced a string object to be created,
                            // we need to protect it here.
                            if (temp != argument)
                            {
                                                     /* make it safe                      */
                                createLocalReference(temp);
                            }
                            /* set the result in                 */
                            descriptors[outputIndex].value.value_RexxArrayObject = temp;
                            break;

                        }

                        case REXX_VALUE_RexxStemObject: /* Required Stem object            */
                        {
                            // Stem arguments get special handling.  If the argument
                            // object is already a stem object, we're done.  Otherwise,
                            // we get the string value of the argument and use that
                            // to resolve a stem name in the current context.  If the
                            // trailing period is not given on the name, one will be added.
                            // Note that the second form is only available if this is a
                            // call context.  This is an error for a method context.

                            // is this a stem already?
                            if (isStem(argument))
                            {
                                /* set the result in                 */
                                descriptors[outputIndex].value.value_RexxStemObject = argument;
                                break;
                            }

                            // this spesn't make any sense for a function call
                            if (activationType == METHOD_ACTIVATION)
                            {
                                reportStemError(inputIndex, argument);
                            }

                            /* force to a string value           */
                            RexxString *temp = REQUIRED_STRING(argument, inputIndex + 1) ;
                            // if this forced a string object to be created,
                            // we need to protect it here.
                            if (temp != argument)
                            {
                                                     /* make it safe                      */
                                createLocalReference(temp);
                            }

                            // see if we can retrieve this stem
                            RexxObject *stem = getContextStem(temp);
                            if (stem == OREF_NULL)
                            {
                                reportStemError(inputIndex, argument);
                            }
                            /* set the result in                 */
                            descriptors[outputIndex].value.value_RexxStemObject = stem;
                            break;

                        }

                        case REXX_VALUE_POINTER:
                        {
                            descriptors[outputIndex].value.value_POINTER = this->pointer(argument);
                            break;
                        }

                        default:                   /* something messed up               */
                        {
                            reportSignatureError();
                            break;
                        }
                    }
                }
                else
                {
                    // if this was not an option argument
                    if (!isOptional)
                    {
                        missingArgument(inputIndex + 1);
                    }

                    // this is a non-specified argument
                    descriptors[outputIndex].flags = 0;
                    switch (type)
                    {

                        case REXX_VALUE_RexxStringObject: /* no object here                    */
                        case REXX_VALUE_RexxArrayObject: /* no object here                    */
                        case REXX_VALUE_RexxObjectPtr:  /* no object here                    */
                        case REXX_VALUE_int:            /* non-integer value                 */
                        case REXX_VALUE_int64_t:        /* non-integer value                 */
                        case REXX_VALUE_int32_t:        /* non-integer value                 */
                        case REXX_VALUE_int16_t:        /* non-integer value                 */
                        case REXX_VALUE_int8_t:         /* non-integer value                 */
                        case REXX_VALUE_RexxNumber:     /* non-existent long                 */
                        case REXX_VALUE_RexxUnsignedNumber: /* non-existent long                 */
                        case REXX_VALUE_RexxBoolean:    // this must be a boolean value
                        case REXX_VALUE_double:         /* non-existent double               */
                        case REXX_VALUE_float:          /* non-existent double               */
                        case REXX_VALUE_size_t:         /* non-existent double               */
                        case REXX_VALUE_CSTRING:        /* missing character string          */
                        case REXX_VALUE_POINTER:
                        {
                            // set this as a 64-bit value to clear everything out
                            descriptors[outputIndex].value.value_int64_t = 0;
                            break;
                        }
                                                   /* still an error if not there       */
                        default:                   /* something messed up               */
                        {
                            reportSignatureError();
                            break;
                        }
                    }
                }
                inputIndex++;              // we've used up one more input argument
                break;
            }
        }
        outputIndex++;                 /* step to the next argument         */
        argumentTypes++;               // and the next output position pointer
    }
    if (inputIndex < argcount && !usedArglist)    /* extra, unwanted arguments?        */
    {
                                         /* got too many                      */
        reportException(Error_Incorrect_method_maxarg, inputIndex);
    }
}

RexxObject *RexxNativeActivation::valueToObject(
    ValueDescriptor *value)     // this is passed as a pointer so we dereference properly
/******************************************************************************/
/* Function:  Convert a return value into a Rexx object based on type sig.    */
/******************************************************************************/
{
    switch (value->type)
    {
        case REXX_VALUE_RexxObjectPtr:          /* Object reference                  */
        {
            return value->value.value_RexxObjectPtr; // just return the object value
        }

        case REXX_VALUE_int:                    /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_int);
        }

        case REXX_VALUE_int8_t:                         /* integer value                     */
        {
            return Numerics::toObject(value->value.value_int8_t);
        }

        case REXX_VALUE_int16_t:                        /* integer value                     */
        {
            return Numerics::toObject(value->value.value_int16_t);
        }

        case REXX_VALUE_int32_t:                        /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_int32_t);
        }

        case REXX_VALUE_int64_t:                        /* integer value                     */
        {
            return Numerics::toObject(value->value.value_int64_t);
        }

        case REXX_VALUE_intptr_t:                       /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_intptr_t);
        }

        case REXX_VALUE_size_t:                        /* integer value                     */
        {
            return Numerics::toObject((stringsize_t)value->value.value_size_t);
        }

        case REXX_VALUE_RexxNumber:        /* long integer value                */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_RexxNumber);
        }

        case REXX_VALUE_RexxUnsignedNumber:     /* long integer value                */
        {
            return Numerics::toObject((stringsize_t)value->value.value_RexxUnsignedNumber);
        }

        case REXX_VALUE_double:                 /* double value                      */
        {
            return new_string(value->value.value_double);
        }

        case REXX_VALUE_float:                  /* float value                      */
        {
            return new_string(value->value.value_float);
        }

        case REXX_VALUE_CSTRING:                /* ASCII-Z string                    */
        {
            char *string = value->value.value_CSTRING;
            // return return nothing if a null pointer is returned.
            if (string == NULL)
            {
                return OREF_NULL;
            }
            return new_string(string);             // make a Rexx string from this
        }

        case REXX_VALUE_POINTER:
        {
            // just wrap the pointer in a pointer object
            return new_pointer(value->value.value_POINTER);
        }

        default:
        {
            reportSignatureError();      // bad method signature problem
            break;
        }
    }
    return OREF_NULL;
}


/**
 * Convert a Rexx object into the requested value type, if possible.
 *
 * @param o      The source object.
 * @param value  The receiving value structure, which also defines the type.
 *
 * @return true if the value was convertable, false otherwise.
 */
bool RexxNativeActivation::objectToValue(RexxObject *o, ValueDescriptor *value)
{
    switch (value->type)
    {
        case REXX_VALUE_RexxObjectPtr:          /* Object reference                  */
        {
            // silly, but this always works.
            value->value.value_RexxObjectPtr = (RexxObjectPtr)o;
            return true;
        }
        case REXX_VALUE_int:            /* integer value                     */
        {
            wholenumber_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToWholeNumber(o, temp, INT_MAX, INT_MIN);
            value->value.value_int = (int)temp;
            return success;
        }

        case REXX_VALUE_int8_t:            /* 8-bit integer value               */
        {
            wholenumber_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToWholeNumber(o, temp, INT8_MAX, INT8_MIN);
            value->value.value_int8_t = (int8_t)temp;
            return success;
        }

        case REXX_VALUE_int16_t:            /* integer value                     */
        {
            wholenumber_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToWholeNumber(o, temp, INT16_MAX, INT16_MIN);
            value->value.value_int16_t = (int16_t)temp;
            return success;
        }

        case REXX_VALUE_int32_t:            /* integer value                     */
        {
            wholenumber_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToWholeNumber(o, temp, INT32_MAX, INT32_MIN);
            value->value.value_int32_t = (int32_t)temp;
            return success;
        }

        case REXX_VALUE_int64_t:            /* integer value                     */
        {
            int64_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToInt64(o, temp);
            value->value.value_int64_t = (int64_t)temp;
            return success;
        }

        case REXX_VALUE_uint8_t:            /* 8-bit integer value               */
        {
            stringsize_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToStringSize(o, temp, UINT8_MAX);
            value->value.value_uint8_t = (uint8_t)temp;
            return success;
        }

        case REXX_VALUE_uint16_t:            /* integer value                     */
        {
            stringsize_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToStringSize(o, temp, UINT16_MAX);
            value->value.value_uint16_t = (uint16_t)temp;
            return success;
        }

        case REXX_VALUE_uint32_t:            /* integer value                     */
        {
            stringsize_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToStringSize(o, temp, UINT32_MAX);
            value->value.value_uint32_t = (uint32_t)temp;
            return success;
        }

        case REXX_VALUE_uint64_t:            /* integer value                     */
        {
            uint64_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToUnsignedInt64(o, temp);
            value->value.value_uint64_t = (uint64_t)temp;
            return success;
        }

        case REXX_VALUE_size_t:            /* integer value                     */
        {
            stringsize_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToStringSize(o, temp, SIZE_MAX);
            value->value.value_size_t = (size_t)temp;
            return success;
        }

        case REXX_VALUE_RexxBoolean:       /* integer value                     */
        {
            // this will raise an error, which we'll clear before return
            value->value.value_RexxBoolean = (RexxBoolean)o->truthValue(Error_Logical_value_method);
            return true;
        }

        // The Rexx whole number one is checked against the human digits limit
        case REXX_VALUE_RexxNumber:  /* number value                      */
        {
            wholenumber_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToWholeNumber(o, temp, Numerics::MAX_WHOLENUMBER, Numerics::MIN_WHOLENUMBER);
            value->value.value_RexxNumber = (RexxNumber)temp;
            return success;
        }

        // an unsigned string number value
        case REXX_VALUE_RexxUnsignedNumber:
        {
            stringsize_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToStringSize(o, temp, Numerics::MAX_STRINGSIZE);
            value->value.value_RexxUnsignedNumber = (RexxUnsignedNumber)temp;
            return success;
        }

        case REXX_VALUE_double:         /* double value                      */
        {
            return o->doubleValue(&value->value.value_double);
        }


        case REXX_VALUE_float:          /* float value                      */
        {
            double temp = 0.0;
            bool success = o->doubleValue(&temp);
            value->value.value_float = (float)temp;
            return success;
        }

        case REXX_VALUE_CSTRING:        /* ASCII-Z string value              */
        {
            value->value.value_CSTRING = this->cstring(o);
            return true;
        }

        case REXX_VALUE_RexxStringObject: /* Required STRING object            */
        {
            /* force to a string value           */
            RexxString *temp = REQUIRED_STRING(o, 1) ;
            // if this forced a string object to be created,
            // we need to protect it here.
            if (temp != o)
            {
                                     /* make it safe                      */
                createLocalReference(temp);
            }
            /* set the result in                 */
            value->value.value_RexxStringObject = temp;
            return true;

        }

        case REXX_VALUE_RexxArrayObject: /* Required ARRAY object            */
        {
            /* force to a string value           */
            RexxArray *temp = REQUIRED_ARRAY(o, 1) ;
            // if this forced a string object to be created,
            // we need to protect it here.
            if (temp != o)
            {
                                     /* make it safe                      */
                createLocalReference(temp);
            }
            /* set the result in                 */
            value->value.value_RexxArrayObject = temp;
            return true;

        }

        case REXX_VALUE_RexxStemObject: /* Required Stem object            */
        {
            // Stem os get special handling.  If the o
            // object is already a stem object, we're done.  Otherwise,
            // we get the string value of the o and use that
            // to resolve a stem name in the current context.  If the
            // trailing period is not given on the name, one will be added.
            // Note that the second form is only available if this is a
            // call context.  This is an error for a method context.

            // is this a stem already?
            if (isStem(o))
            {
                /* set the result in                 */
                value->value.value_RexxStemObject = o;
                return true;
            }

            // this doesn't make any sense for a function call
            if (activationType == METHOD_ACTIVATION)
            {
                return false;
            }

            /* force to a string value           */
            RexxString *temp = REQUIRED_STRING(o, 1) ;
            // if this forced a string object to be created,
            // we need to protect it here.
            if (temp != o)
            {
                                     /* make it safe                      */
                createLocalReference(temp);
            }

            // see if we can retrieve this stem
            RexxObject *stem = getContextStem(temp);
            if (stem == OREF_NULL)
            {
                return false;
            }
            /* set the result in                 */
            value->value.value_RexxStemObject = stem;
            return true;
        }

        case REXX_VALUE_POINTER:
        {
            value->value.value_POINTER = this->pointer(o);
            return true;
        }

        default:                   /* something messed up               */
        {
            return false;
        }
    }
    return false;
}

void RexxNativeActivation::createLocalReference(
    RexxObject *objr)                  /* object to save                    */
/******************************************************************************/
/* Function:  Protect an object until the native activation terminates        */
/******************************************************************************/
{
  if (objr != OREF_NULL) {             /* have an object?                   */
      // make sure we protect this from a GC triggered by this table creation.
      ProtectedObject p1(objr);
      if (this->savelist == OREF_NULL) /* second saved object?              */
      {
                                         /* create the save list now          */
          this->savelist = new_object_table();
      }
                                       /* add to the save table             */
      this->savelist->put(TheNilObject, objr);
  }
}

void RexxNativeActivation::removeLocalReference(
    RexxObject *objr)                  /* object to save                    */
/******************************************************************************/
/* Function:  Protect an object until the native activation terminates        */
/******************************************************************************/
{
    // if the reference is non-null
  if (objr != OREF_NULL)
  {
      // make sure we have a savelist before trying to remove this
      if (savelist != OREF_NULL)
      {
          savelist->remove(objr);
      }
  }
}

void RexxNativeActivation::run(
    size_t       count,                /* argument count                    */
    RexxObject **list,                 /* method argument list              */
    ProtectedObject &result)           // the return result
/******************************************************************************/
/* Function:  Execute a REXX native method                                    */
/******************************************************************************/
{
    this->arglist = list;                /* save the argument information     */
    this->argcount = count;              /* set the argument count            */

    ValueDescriptor arguments[MAX_NATIVE_ARGUMENTS];

    MethodContext context;               // the passed out method context

    // build a context pointer to pass out
    activity->createMethodContext(&context, this);
    context.threadContext.arguments = arguments;

    // get the entry point address of the target method
    PNMF methp = (PNMF)this->method->getNativeEntry();

    // retrieve the argument signatures and process them
    uint16_t *types = (*methp)((RexxMethodContext *)&context, NULL);
    processArguments(argcount, arglist, types, arguments, MAX_NATIVE_ARGUMENTS);

    try
    {
        // NOTE:  If we get out of here because of an exception, the caller already
        // owns kernel access.  We don't want to nest the request by using
        // UnsafeBlock.
        this->activity->releaseAccess();     /* force this to "safe" mode         */
                                             /* process the method call           */
        (*methp)((RexxMethodContext *)&context, arguments);
        this->activity->requestAccess();     /* now in unsafe mode again          */

        result = valueToObject(arguments);
    }
    catch (RexxNativeActivation *)
    {
        result = OREF_NULL;              // no result if an exception occurs
    }

    this->guardOff();                    /* release any variable locks        */

    checkConditions();                   // see if we have conditions to raise now
}


/**
 * Process a native function call.
 *
 * @param entryPoint The target function entry point.
 * @param count      The number of arguments.
 * @param list       The list of arguments.
 * @param result     A protected object to receive the function result.
 */
void RexxNativeActivation::call(
    PNFUNCTION   entryPoint,           // the call target entry point
    size_t       count,                /* argument count                    */
    RexxObject **list,                 /* method argument list              */
    ProtectedObject &result)           // the return result
/******************************************************************************/
/* Function:  Execute a REXX native method as a function                      */
/******************************************************************************/
{
    this->arglist = list;                /* save the argument information     */
    this->argcount = count;              /* set the argument count            */
    activationType = EXTERNAL_FUNCTION;  // we're processing an external function

    // save a link to the current Rexx execution context
    context = activity->getCurrentContext();
    result = OREF_NULL;                  // make sure this is cleared in case an exception occurs

    ValueDescriptor arguments[MAX_NATIVE_ARGUMENTS];

    // build a context pointer to pass out
    activity->createCallContext(&callContext, this);
    callContext.threadContext.arguments = arguments;

    // get the types and marshall them
    uint16_t *types = (*entryPoint)((RexxCallContext *)&callContext, NULL);
    processArguments(argcount, arglist, types, arguments, MAX_NATIVE_ARGUMENTS);

    try
    {
        // NOTE:  If we get out of here because of an exception, the caller already
        // owns kernel access.  We don't want to nest the request by using
        // UnsafeBlock.
        this->activity->releaseAccess();     /* force this to "safe" mode         */
        (*entryPoint)((RexxCallContext *)&callContext, arguments);    /* process the method call           */
        this->activity->requestAccess();     /* now in unsafe mode again          */

        result = valueToObject(arguments);
    }
    catch (RexxNativeActivation *)
    {
        result = OREF_NULL;                  // clear this out with errors
    }

    checkConditions();                   // see if we have conditions to raise now
}

RexxSupplier *RexxNativeActivation::getAllContextVariables()
/******************************************************************************/
/* Function:  Retriev a list of all variables in the current context.         */
/******************************************************************************/
{
    return context->getAllLocalVariables();
}

/**
 * Get nn object variable in the current method scope.  Returns
 * a NULL object reference if the variable does not exist.
 *
 * @param name   The variable name.
 *
 * @return The variable value or OREF_NULL if the variable does not
 *         exist.
 */
RexxObject *RexxNativeActivation::getObjectVariable(stringchar_t *name)
{
    return methodVariables()->realValue(new_string(name));
}

/**
 * The an object variable to a new value.
 *
 * @param name   The name of the variable.
 * @param value  The new variable value.
 */
void RexxNativeActivation::setObjectVariable(stringchar_t *name, RexxObject *value)
{
    methodVariables()->set(new_string(name), value);
}

/**
 * Drop an object variable in the current method scope.
 *
 * @param name   The name of the variable.
 */
void RexxNativeActivation::dropObjectVariable(stringchar_t *name)
{
    methodVariables()->drop(new_string(name));
}


/**
 * Retrieve the current variable dictionary context, either a
 * caller's local context or a method's scope context.
 *
 * @return A variable dictionary for the context.
 */
RexxVariableDictionary *RexxNativeActivation::methodVariables()
{
                                       /* first retrieval?                  */
  if (this->objectVariables == OREF_NULL) {
                                       /* is this a function or exit?       */
    if (activationType != METHOD_ACTIVATION)
    {
                                         /* retrieve the method variables     */
        this->objectVariables = context->getLocalVariables();
    }
    else
    {
                                       /* must be wanting the ovd set of    */
                                       /*variables                          */
      this->objectVariables = (RexxVariableDictionary *)u_receiver->getObjectVariables(method->getScope());
                                       /* guarded method?                   */
      if (this->object_scope == SCOPE_RELEASED && this->method->isGuarded()) {
                                       /* reserve the variable scope        */
        this->objectVariables->reserve(this->activity);
                                       /* and remember for later            */
        this->object_scope = SCOPE_RESERVED;
      }
    }
  }
  return this->objectVariables;        /* return the dictionary             */
}

wholenumber_t RexxNativeActivation::wholeNumberValue(RexxObject *o, size_t position, wholenumber_t maxValue, wholenumber_t minValue)
{
    wholenumber_t temp;

    // convert using the whole value range
    if (!Numerics::objectToWholeNumber(o, temp, maxValue, minValue))
    {
        /* this is an error                  */
        reportException(Error_Incorrect_method_whole, position + 1, o);
    }
    return temp;
}

stringsize_t RexxNativeActivation::unsignedNumberValue(RexxObject *o, size_t position, stringsize_t maxValue)
{
    stringsize_t temp;

    // convert using the whole value range
    if (!Numerics::objectToStringSize(o, temp, maxValue))
    {
        /* this is an error                  */
        reportException(Error_Incorrect_method_whole, position + 1, o);
    }
    return temp;
}

int64_t RexxNativeActivation::int64Value(RexxObject *o, size_t position)
{
    int64_t temp;

    // convert using the whole value range
    if (!Numerics::objectToInt64(o, temp))
    {
        /* this is an error                  */
        reportException(Error_Incorrect_method_whole, position + 1, o);
    }
    return temp;
}


uint64_t RexxNativeActivation::unsignedInt64Value(RexxObject *o, size_t position)
{
    uint64_t temp;

    // convert using the whole value range
    if (!Numerics::objectToUnsignedInt64(o, temp))
    {
        /* this is an error                  */
        reportException(Error_Incorrect_method_whole, position + 1, o);
    }
    return temp;
}

char * RexxNativeActivation::cstring(
    /* object to convert                 */
    RexxObject *object)
/******************************************************************************/
/* Function:  Return an object as a CSTRING                                   */
/******************************************************************************/
{
  RexxString *string;                  /* object string value               */

                                       /* force to a string value           */
  string = (RexxString *)object->stringValue();
  if (string != object)                /* different value?                  */
                                       /* make it safe                      */
    createLocalReference(string);
  return (char *)string->getStringData(); /* just point to the string data     */
}

double RexxNativeActivation::getDoubleValue(
    RexxObject *object)                /* object to convert                 */
/******************************************************************************/
/* Function:  Convert an object to a double                                   */
/******************************************************************************/
{
  double result;                       /* returned result                   */

                                       /* convert and check result          */
  if (!object->doubleValue(&result))
                                       /* conversion error                  */
    reportException(Error_Execution_nodouble, object);
  return result;                       /* return converted number           */
}


void *RexxNativeActivation::cself()
/******************************************************************************/
/* Function:  Returns "unwrapped" C or C++ object associated with this        */
/*            object instance.  If the variable CSELF does not exist, then    */
/*            NULL is returned.                                               */
/******************************************************************************/
{
    RexxPointer *C_self;                 /* accessed pointer object           */

                                         /* retrieve from object dictionary   */
    C_self = (RexxPointer *)this->methodVariables()->realValue(OREF_CSELF);
    if (C_self != OREF_NULL && isOfClass(Pointer, C_self))             /* got an item?                      */
    {
        return C_self->pointer();          /* return the pointer value          */
    }
    else
    {
        return NULL;                       /* no object available               */
    }
}

void *RexxNativeActivation::buffer()
/******************************************************************************/
/* Function:  Returns "unwrapped" C or C++ object stored in a buffer object.  */
/*            If the variable CSELF does not exist, then NULL is returned.    */
/******************************************************************************/
{
    RexxBuffer  *C_self;                 /* accessed pointer object           */

                                         /* retrieve from object dictionary   */
    C_self = (RexxBuffer *)this->methodVariables()->realValue(OREF_CSELF);
    if (C_self != OREF_NULL && isOfClass(Buffer, C_self))             /* got an item?                      */
    {
        return(void *)C_self->address();   /* return a pointer to the address   */
    }
    else
    {
        return NULL;                       /* no object available               */
    }
}

void *RexxNativeActivation::pointer(
    RexxObject *object)                /* object to convert                 */
/******************************************************************************/
/* Function:  Return as a pointer the value of an integer                     */
/******************************************************************************/
{
    // this MUST be a pointer object
    if (!isOfClass(Pointer, object))
    {
                                       /* conversion error                  */
        reportException(Error_Execution_wrongobject, OREF_POINTER);
    }
                                       /* just "unwrap" the pointer         */
    return ((RexxPointer *)object)->pointer();
}

void RexxNativeActivation::dispatch(ProtectedObject &result)
/******************************************************************************/
/* Function:  Redispatch an activation on a different activity                */
/******************************************************************************/
{
  this->run(0, NULL, result);     /* just do a method run              */
}

void RexxNativeActivation::traceBack(
    RexxList *traceback_list)          /* list of traceback items           */
/******************************************************************************/
/* Function:  Add a trace back item to the list.  For native activations,     */
/*            this is a no-op.                                                */
/******************************************************************************/
{
  return;                              /* just return                       */
}

stringsize_t RexxNativeActivation::digits()
/******************************************************************************/
/* Function:  Return the current digits setting                               */
/******************************************************************************/
{
    return context == OREF_NULL ? Numerics::DEFAULT_DIGITS : context->digits();
}

stringsize_t RexxNativeActivation::fuzz()
/******************************************************************************/
/* Function:  Return the current fuzz setting                                 */
/******************************************************************************/
{
    return context == OREF_NULL ? Numerics::DEFAULT_FUZZ : context->fuzz();
}

bool RexxNativeActivation::form()
/******************************************************************************/
/* Function:  Return the curren form setting                                  */
/******************************************************************************/
{
    return context == OREF_NULL ? Numerics::DEFAULT_FORM : context->form();
}

void RexxNativeActivation::setDigits(
    stringsize_t digits)               /* new NUMERIC DIGITS value          */
/******************************************************************************/
/* Function:  Set a new numeric digits setting                                */
/******************************************************************************/
{
    // only set if the context is there
    if (context != OREF_NULL)
    {
        context->setDigits(digits);
    }
}

void RexxNativeActivation::setFuzz(
    stringsize_t fuzz )                /* new NUMERIC FUZZ value            */
/******************************************************************************/
/* Function:  Set a new numeric fuzz setting                                  */
/******************************************************************************/
{
    // only set if the context is there
    if (context != OREF_NULL)
    {
        context->setFuzz(fuzz);
    }
}

void RexxNativeActivation::setForm(
    bool form )                      /* new NUMERIC FORM value            */
/******************************************************************************/
/* Function:  Set a new numeric form setting                                  */
/******************************************************************************/
{
    // only set if the context is there
    if (context != OREF_NULL)
    {
        context->setForm(form);
    }
}

void RexxNativeActivation::guardOff()
/******************************************************************************/
/* Function:  Release a variable pool guard lock                              */
/******************************************************************************/
{
  // this is only allowed for method calls
  if (activationType != METHOD_ACTIVATION)
  {
      return;
  }
                                       /* currently locked?                 */
  if (this->object_scope == SCOPE_RESERVED) {
                                       /* release the variable dictionary   */
    this->objectVariables->release(this->activity);
                                       /* set the state to released         */
    this->object_scope = SCOPE_RELEASED;
  }
}

void RexxNativeActivation::guardOn()
/******************************************************************************/
/* Function:  Acquire a variable pool guard lock                              */
/******************************************************************************/
{

    /* not currently holding the lock? */
    if (this->object_scope == SCOPE_RELEASED) {
      /* reserve the variable scope */
      this->methodVariables()->reserve(this->activity);
      /* and remember for later */
      this->object_scope = SCOPE_RESERVED;
    }
}

void RexxNativeActivation::resetNext()
/******************************************************************************/
/* Function: Reset the next state of the variable pool                        */
/******************************************************************************/
{
  this->nextvariable = -1;             /* turn off next index               */
  this->nextcurrent = OREF_NULL;       /* clear the next value              */
  this->nextstem = OREF_NULL;          /* clear the secondary pointer       */
  this->compoundelement = OREF_NULL;
}


bool RexxNativeActivation::fetchNext(
    RexxString **name,                 /* the returned name                 */
    RexxObject **value)                /* the return value                  */
/******************************************************************************/
/* Function:  Fetch the next variable of a variable pool traversal            */
/******************************************************************************/
{
  RexxVariable *variable;              /* retrieved variable value          */
  RexxCompoundElement *compound;       /* retrieved variable value          */
  RexxStem     *stem;                  /* a potential stem variable collection */
  bool result = false;               /* default to the "we've reached the end" scenario */

                                       /* starting off fresh?               */
  if (nextCurrent() == OREF_NULL) {
    /* grab the activation context */
    setNextVariable(-1);               /* request the first item            */
    /* Get the local variable dictionary from the context. */
    setNextCurrent(context->getLocalVariables());
                                       /* we are not on a stem              */
    setNextStem(OREF_NULL);
    setCompoundElement(OREF_NULL);
  }

  for (;;) {                           /* loop until we get something       */
    stem = nextStem();                 /* see if we're processing a stem variable */
    if (stem != OREF_NULL) {           /* were we in the middle of processing a stem? */
        compound = stem->nextVariable(this);
        if (compound != OREF_NULL) {   /* if we still have elements here */
                                       /* create a full stem name           */
          *name = compound->createCompoundName(stem->getName());
                                       /* get the value                     */
          *value = compound->getVariableValue();
          result = true;
          break;
        }
        else {                         /* we've reached the end of the stem, reset */
                                       /* to the main dictionary and continue */
            setNextStem(OREF_NULL);
            setCompoundElement(OREF_NULL);
        }
    }
                                       /* get the next variable             */
    variable = nextCurrent()->nextVariable(this);
    if (variable == OREF_NULL) {       /* reached the end of the table      */
        break;                         /* reached the end                   */
    }
    else {                             /* have a real variable              */
                                       /* get the value                     */
      RexxObject *variable_value = variable->getVariableValue();
                                       /* found a stem item?                */
      if (isStem(variable_value)) {
                                       /* we are not on a stem              */
        setNextStem((RexxStem *)variable_value);
        setCompoundElement(((RexxStem *)variable_value)->first());
                                       /* set up an iterator for the stem   */
      }
      else {                           /* found a real variable             */
        *value = variable_value;       /* pass back the value (name already set) */
        *name = variable->getName();
        result = true;                 /* we have another variable to return */
        break;                         /* finished                          */
      }
    }
  }
  return result;                       /* return the next item              */
}


bool RexxNativeActivation::trap(
    RexxString    * condition,         /* name of the condition             */
    RexxDirectory * exception_object)  /* exception information             */
/******************************************************************************/
/*                                                                            */
/*  Function: In almost all cases NativeActs don't care about conditions      */
/*     however in the case of Message objects, they need to know about        */
/*     conditions so cleanups can be done.  We know we are to notify a        */
/*     message object by checking our objnotify field, if ther is anything    */
/*     there, it will be a message object and we simple send a error message  */
/*     to this object.                                                        */
/*                                                                            */
/******************************************************************************/
{
    /* is this a syntax condition?       */
    if (condition->strCompare(CHAR_SYNTAX))
    {
        /* do we need to notify a message    */
        /*obj?                               */
        if (this->errorListener != OREF_NULL)
        {
            /* yes, send error message and       */
            /* condition to the object           */
            this->errorListener->error(exception_object);
        }
    }
    // record this in case any callers want to know about it.
    setConditionInfo(exception_object);
    return false;                        /* this wasn't handled               */
}

void RexxNativeActivation::setErrorListener(
    RexxMessage *listener)             /* message object to notify          */
/******************************************************************************/
/* Function:  Put a notification trap on syntax conditions on this activation.*/
/******************************************************************************/
{
  this->errorListener = listener;      /* save the notification             */
}


/******************************************************************************/
/* copy_value -                                                               */
/******************************************************************************/
RexxReturnCode RexxNativeActivation::copyValue(
    RexxObject * value,                /* value to copy                     */
    RxString   * rxstring,             /* target rxstring                   */
    RexxStringLength *length)          /* updated real length               */
{
   RexxString * stringValue;           /* converted object value            */
   stringsize_t string_length;         /* length of the string              */
   uint32_t     rc;                    /* return code                       */

   rc = 0;                             /* default to success                */
                                       /* get the string value              */
   stringValue = value->stringValue();
   string_length = stringValue->getLength();/* get the string length             */
   // caller allowing use to allocate this?
   if (rxstring->strptr == NULL)
   {
       rxstring->strptr = (RexxStringPointer)SysInterpreter::allocateResultMemory(string_length + 1);
       if (rxstring->strptr == NULL)
       {
           return RXSHV_MEMFL;                  /* couldn't allocate, return flag */
       }
       rxstring->strlength = string_length;
   }
                                            /* buffer too short?              */
   if (string_length > rxstring->strlength){
     rc = RXSHV_TRUNC;                      /* set truncated return code      */
                                            /* copy the short piece           */
     memcpy(rxstring->strptr, stringValue->getStringData(), rxstring->strlength);
   }
   else
   {
                                            /* copy entire string             */
     memcpy(rxstring->strptr, stringValue->getStringData(), string_length);
                                            /* room for a null?               */
     if (rxstring->strlength > string_length)
     {
                                            /* yes, add one                   */
       rxstring->strptr[string_length] = '\0';
     }
     rxstring->strlength = string_length;   /* string length doesn't include terminating 0 */
   }
   *length = string_length;                 /* return actual string length    */
   return rc;                               /* give back the return code      */
}

RexxVariableBase *RexxNativeActivation::getVariableRetriever(RexxActivation *activation, RxString *name, bool direct)
/******************************************************************************/
/* Function:  get a variable retriever for a named variable.                  */
/******************************************************************************/
{
    /* no name given?                    */
    if (RxIsValidString(*name))
    {
        /* get the variable as a string      */
        RexxString *variable = new_string(name);
        ProtectedObject p1(variable);
        // retrievers can be direct or symbolic
        if (direct)
        {
            return activation->getDirectVariableRetriever(variable);
        }
        else
        {
            return activation->getVariableRetriever(variable);
        }
    }
    return OREF_NULL;    // not a valid name request
}


/**
 * Get a variable retriever for the current variable context.
 *
 * @param name   The target variable name.
 *
 * @return A variable retriever for the variable name.  Returns OREF_NULL
 *         if there was a name parsing problem.
 */
RexxVariableBase *RexxNativeActivation::getVariableRetriever(stringchar_t *name)
{
    return context->getDirectVariableRetriever(new_string(name));
}


wholenumber_t RexxNativeActivation::getContextVariable(
    RxString *name,
    RxString *value)
/******************************************************************************/
/* Function:  retrieve a variable value (directly) from current context.      */
/******************************************************************************/
{
    RexxVariableBase *retriever = context->getVariableRetriever(new_upper_string(name));
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL)
    {
        return RXSHV_BADN;

    }
    this->resetNext();               // all next operations must be reset

    RexxObject *varValue;
    // have a non-name retriever?
    if (isString((RexxObject *)retriever))
    {
        // the value is the retriever
        varValue = (RexxObject *)retriever;
    }
    else
    {
        // get the variable value
        varValue = retriever->getValue(context);
    }
    // this API ALWAYS allocates the buffer for the user
    value->strptr = NULL;

    return copyValue(varValue, value, &value->strlength);
}

RexxObject *RexxNativeActivation::getContextStem(
    RexxString *name)
/******************************************************************************/
/* Function:  retrieve a stem variable stem from the current context.         */
/******************************************************************************/
{
    // if this is not a stem name, add it now
    if (name->getChar(name->getLength() - 1) != '.')
    {
        name = name->concatWithCstring(".");
    }

    RexxVariableBase *retriever = context->getVariableRetriever(name);
    // if this didn't parse, it's an illegal name
    // it must also resolve to a stem type...this could be a compound one
    if (retriever == OREF_NULL || !isOfClass(StemVariable, retriever))
    {
        return OREF_NULL;
    }
    // get the variable value
    return retriever->getValue(context);
}


wholenumber_t RexxNativeActivation::setContextVariable(
    RxString *name,
    RxString *value)
/******************************************************************************/
/* Function:  set a variable value (directly) in current context.             */
/******************************************************************************/
{
    // get the REXX activation for the target context
    RexxVariableBase *retriever = getVariableRetriever(context, name,  true);
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL || isString((RexxObject *)retriever))
    {
        return RXSHV_BADN;
    }
    this->resetNext();               // all next operations must be reset

    // do the assignment
    retriever->set(context, new_string(value));
    return RXSHV_OK;
}


wholenumber_t RexxNativeActivation::dropContextVariable(
    RxString *name)
/******************************************************************************/
/* Function:  retrieve a variable value (directly) from current context.      */
/******************************************************************************/
{
    // get the REXX activation for the target context
    RexxVariableBase *retriever = getVariableRetriever(context, name,  true);
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL || isString((RexxObject *)retriever))
    {
        return RXSHV_BADN;
    }
    this->resetNext();               // all next operations must be reset

    // perform the drop
    retriever->drop(context);
    return RXSHV_OK;
}


/**
 * Retrieve the value of a context variable via the API calls.
 * Returns OREF_NULL if the variable does not exist.
 *
 * @param name   The variable name.
 *
 * @return The variable value, if any.
 */
RexxObject *RexxNativeActivation::getContextVariable(stringchar_t *name)
{
    RexxVariableBase *retriever = getVariableRetriever(name);
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL)
    {
        return OREF_NULL;
    }
    this->resetNext();               // all next operations must be reset

    // have a non-name retriever?
    if (isString((RexxObject *)retriever))
    {
        // the value is the retriever
        return (RexxObject *)retriever;
    }
    else
    {
        // get the variable value
        return retriever->getRealValue(context);
    }
}

/**
 * Set a context variable on behalf of an API call.
 *
 * @param name   The name of the variable.
 * @param value  The variable value.
 */
void RexxNativeActivation::setContextVariable(stringchar_t *name, RexxObject *value)
{
    // get the REXX activation for the target context
    RexxVariableBase *retriever = getVariableRetriever(name);
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL || isString((RexxObject *)retriever))
    {
        return;
    }
    this->resetNext();               // all next operations must be reset

    // do the assignment
    retriever->set(context, value);
}


/**
 * Drop a context variable for an API call.
 *
 * @param name   The target variable name.
 */
void RexxNativeActivation::dropContextVariable(stringchar_t *name)
{
    // get the REXX activation for the target context
    RexxVariableBase *retriever = getVariableRetriever(name);
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL || isString((RexxObject *)retriever))
    {
        return;
    }
    this->resetNext();               // all next operations must be reset

    // perform the drop
    retriever->drop(context);
}

void RexxNativeActivation::checkConditions()
/******************************************************************************/
/* Function:  check to see if a condition was raised on return from a call out*/
/******************************************************************************/
{
    // if we have a stashed condition object, we need to raise this now
    if (conditionobj != OREF_NULL)
    {
        // we're raising this in the previous stack frame.  If it doesn't exist,
        // se just leave this in place so top-level callers can check this.
        if (previousStackFrame != OREF_NULL)
        {
            activity->reraiseException(conditionobj);
        }
    }
}


/******************************************************************************/
/*                                                                            */
/* variablePool - Native activation method that handles external variable     */
/*                   pool requests.                                           */
/*                                                                            */
/* Inputs:                                                                    */
/*   requests - Pointer to a chain of one or more shared variable pool        */
/*              request blocks.                                               */
/*                                                                            */
/* Outputs:                                                                   */
/*   RexxReturnCode - Composite return code that results from processing      */
/*           request blocks.                                                  */
/*                                                                            */
/* Notes:                                                                     */
/******************************************************************************/
RexxReturnCode RexxNativeActivation::variablePoolInterface(VariableRequest *pshvblock)
{
    RexxReturnCode retcode = 0;          /* initialize composite rc           */

    try
    {
        while (pshvblock)
        {                   /* while more request blocks         */
            variablePoolRequest(pshvblock);
            retcode |= pshvblock->shvret;        /* accumulate the return code        */
            pshvblock = pshvblock->shvnext;      /* step to the next block            */
        }
        return retcode;                       /* return composite return code      */

    }
    // intercept any termination failures
    catch (ActivityException)
    {
        /* set failure in current            */
        pshvblock->shvret |= (uint8_t)RXSHV_MEMFL;
        retcode |= pshvblock->shvret;       /* OR with the composite             */
        return retcode;                     /* stop processing requests now      */
    }
}

/**
 * Get a variable retriever for the target variable.
 *
 * @param pshvblock The variable pool request block.
 * @param symbolic  The symbolic vs. direct indicator.
 *
 * @return A variable retriever for the variable.  Returns OREF_NULL
 *         if the variable is not resolvable from the name.
 */
RexxVariableBase *RexxNativeActivation::variablePoolGetVariable(VariableRequest *pshvblock, bool symbolic)
{
    /* no name given?                    */
    if (pshvblock->shvname.strptr==NULL)
    {
        pshvblock->shvret|=RXSHV_BADN;   /* this is bad                       */
    }
    else
    {
        /* get the variable as a string      */
        RexxString *variable = new_string(pshvblock->shvname);
        RexxVariableBase *retriever = OREF_NULL;
        /* symbolic access?                  */
        if (symbolic)
        {
            /* get a symbolic retriever          */
            retriever = context->getVariableRetriever(variable);
        }
        else                             /* need a direct retriever           */
        {
            retriever = context->getDirectVariableRetriever(variable);
        }
        if (retriever == OREF_NULL)      /* have a bad name?                  */
        {
            pshvblock->shvret|=RXSHV_BADN; /* this is bad                       */
        }
        else
        {
            resetNext();             /* reset any next operations         */
            return retriever;
        }
    }
    return OREF_NULL;
}

/**
 * Perform a variable pool fetch operation.
 *
 * @param pshvblock The operation shared variable block.
 */
void RexxNativeActivation::variablePoolFetchVariable(VariableRequest *pshvblock)
{
    RexxVariableBase *retriever = variablePoolGetVariable(pshvblock, pshvblock->shvcode == RXSHV_SYFET);
    RexxObject *value = OREF_NULL;
    if (retriever != OREF_NULL)
    {
        /* have a non-name retriever?        */
        if (isString((RexxObject *)retriever))
        {
            /* the value is the retriever        */
            value = (RexxObject *)retriever;
        }
        else
        {
                                           /* have a non-name retriever         */
                                           /* and a new variable?               */
            if (!retriever->exists(context))
            {
                /* flag this in the block            */
                pshvblock->shvret |= RXSHV_NEWV;
            }
            /* get the variable value            */
            value = retriever->getValue(context);
        }

        /* copy the value                    */
        pshvblock->shvret |= copyValue(value, &pshvblock->shvvalue, (size_t *)&pshvblock->shvvaluelen);
    }
}


/**
 * Perform a variable pool set operation.
 *
 * @param pshvblock The operation shared variable block.
 */
void RexxNativeActivation::variablePoolSetVariable(VariableRequest *pshvblock)
{
    RexxVariableBase *retriever = variablePoolGetVariable(pshvblock, pshvblock->shvcode == RXSHV_SYFET);
    if (retriever != OREF_NULL)
    {
        /* have a non-name retriever?        */
        if (isString((RexxObject *)retriever))
        {
            /* this is bad                       */
            pshvblock->shvret = RXSHV_BADN;
        }
        else
        {
                                           /* have a non-name retriever         */
                                           /* and a new variable?               */
            if (!retriever->exists(context))
            {
                /* flag this in the block            */
                pshvblock->shvret |= RXSHV_NEWV;
            }
            /* do the assignment                 */
            retriever->set(context, new_string(pshvblock->shvvalue));
        }
    }
}


/**
 * Perform a variable pool drop operation.
 *
 * @param pshvblock The operation shared variable block.
 */
void RexxNativeActivation::variablePoolDropVariable(VariableRequest *pshvblock)
{
    RexxVariableBase *retriever = variablePoolGetVariable(pshvblock, pshvblock->shvcode == RXSHV_SYFET);
    if (retriever != OREF_NULL)
    {
        /* have a non-name retriever?        */
        if (isString((RexxObject *)retriever))
        {
            /* this is bad                       */
            pshvblock->shvret = RXSHV_BADN;
        }
        else
        {
                                           /* have a non-name retriever         */
                                           /* and a new variable?               */
            if (!retriever->exists(context))
            {
                /* flag this in the block            */
                pshvblock->shvret |= RXSHV_NEWV;
            }
            /* perform the drop                  */
            retriever->drop(context);
        }
    }
}


/**
 * Perform a variable pool fetch next operation.
 *
 * @param pshvblock The operation shared variable block.
 */
void RexxNativeActivation::variablePoolNextVariable(VariableRequest *pshvblock)
{
    RexxString *name;
    RexxObject *value;
    /* get the next variable             */
    if (!this->fetchNext(&name, &value))
    {
        pshvblock->shvret |= RXSHV_LVAR; /* flag as such                      */
    }
    else
    {                             /* need to copy the name and value   */
                                  /* copy the name                     */
        pshvblock->shvret |= copyValue(name, &pshvblock->shvname, (size_t *)&pshvblock->shvnamelen);
        /* copy the value                    */
        pshvblock->shvret |= copyValue(value, &pshvblock->shvvalue, (size_t *)&pshvblock->shvvaluelen);
    }
}


/**
 * Perform a variable pool fetch private operation.
 *
 * @param pshvblock The operation shared variable block.
 */
void RexxNativeActivation::variablePoolFetchPrivate(VariableRequest *pshvblock)
{
    /* and VP is enabled                 */
    /* private block should always be enabled */
    /* no name given?                    */
    if (pshvblock->shvname.strptr==NULL)
    {
        pshvblock->shvret|=RXSHV_BADN;   /* this is bad                       */
    }
    else
    {
        /* get the variable as a string      */
        char *variable = pshvblock->shvname.strptr;
        /* want the version string?          */
        if (strcmp(variable, "VERSION") == 0)
        {
            /* copy the value                    */
            pshvblock->shvret |= copyValue(RexxInterpreter::versionString(), &pshvblock->shvvalue, (size_t *)&pshvblock->shvvaluelen);
        }
        /* want the the current queue?       */
        else if (strcmp(variable, "QUENAME") == 0)
        {
            /* copy the value                    */
            pshvblock->shvret |= copyValue(activity->getCurrentQueue(), &pshvblock->shvvalue, (size_t *)&pshvblock->shvvaluelen);
        }
        /* want the version string?          */
        else if (strcmp(variable, "SOURCE") == 0)
        {
            /* retrieve the source string        */
            RexxString *value = context->sourceString();
            /* copy the value                    */
            pshvblock->shvret |= copyValue(value, &pshvblock->shvvalue, (size_t *)&pshvblock->shvvaluelen);
        }
        /* want the parameter count?         */
        else if (strcmp(variable, "PARM") == 0)
        {
            RexxInteger *value = new_integer(context->getProgramArgumentCount());
            /* copy the value                    */
            pshvblock->shvret |= copyValue(value, &pshvblock->shvvalue, (size_t *)&pshvblock->shvvaluelen);
        }
        /* some other parm form              */
        else if (!memcmp(variable, "PARM.", sizeof("PARM.") - 1))
        {
            wholenumber_t value_position;
            /* extract the numeric piece         */
            RexxString *tail = new_string(variable + strlen("PARM."));
            /* get the binary value              */
            /* not a good number?                */
            if (!tail->numberValue(&value_position) || value_position <= 0)
            {
                /* this is a bad name                */
                pshvblock->shvret|=RXSHV_BADN;
            }
            else
            {
                /* get the arcgument from the parent activation */
                RexxObject *value = context->getProgramArgument(value_position);
                if (value == OREF_NULL)
                {    /* doesn't exist?                    */
                    value = OREF_NULLSTRING; /* return a null string              */
                }
                /* copy the value                    */
                pshvblock->shvret |= copyValue(value, &pshvblock->shvvalue, (size_t *)&pshvblock->shvvaluelen);
            }
        }
        else
        {
            pshvblock->shvret|=RXSHV_BADN; /* this is a bad name                */
        }
    }
}


/**
 * Process a single variable pool request.
 *
 * @param pshvblock The request block for this request.
 */
void RexxNativeActivation::variablePoolRequest(VariableRequest *pshvblock)
{
    pshvblock->shvret = 0;               /* set the block return code         */
    RexxNumber code = pshvblock->shvcode;  /* get the request code              */

    switch (code)
    {
        case RXSHV_FETCH:
        case RXSHV_SYFET:
        {
            variablePoolFetchVariable(pshvblock);
            break;
        }
        case RXSHV_SET:
        case RXSHV_SYSET:
        {
            variablePoolSetVariable(pshvblock);
            break;
        }
        case RXSHV_DROPV:
        case RXSHV_SYDRO:
        {
            variablePoolDropVariable(pshvblock);
            break;
        }
        case RXSHV_NEXTV:
        {
            variablePoolNextVariable(pshvblock);
            break;
        }
        case RXSHV_PRIV:
        {
            variablePoolFetchPrivate(pshvblock);
            break;
        }
        default:
        {
            pshvblock->shvret |= RXSHV_BADF;   /* bad function                      */
            break;
        }
    }
}

/**
 * Get a RexxCallContext pointer for the current context
 * activation.  If has been created for an exit, external
 * command, or external function, we return a call context
 * object that was created when the activation was pushed on
 * the stack.  Otherwise, this returns NULL to indicate this
 * was an improper call.
 *
 * @return Either the current call context pointer or NULL.
 */
RexxCallContext *RexxNativeActivation::getCurrentCallContext()
{
    // call contexts are only valid while executing exits, commands, and external
    // functions.  We return NULL if not the proper type.
    if (activationType == EXIT_ACTIVATION || activationType == EXTERNAL_COMMAND || activationType == EXTERNAL_FUNCTION)
    {
        return (RexxCallContext *)&callContext;
    }
    return NULL;
}

/**
 * Return the method context arguments as an array.
 *
 * @return An array containing the method arguments.
 */
RexxArray *RexxNativeActivation::getArguments()
{
    // if we've not requested this before, convert the arguments to an
    // array object.
    if (argArray == OREF_NULL)
    {
        /* create the argument array */
        argArray = new (argcount, arglist) RexxArray;
        // make sure the array is anchored in our activation
        createLocalReference(argArray);
    }
    return argArray;
}

/**
 * Retrieve a specific argument from the method invocation context.
 *
 * @param index  The argument of interest.
 *
 * @return The object mapped to that argument.  Returns OREF_NULL for
 *         omitted arguments.
 */
RexxObject *RexxNativeActivation::getArgument(size_t index)
{
    if (index < argcount)
    {
        return arglist[index];
    }
    return OREF_NULL;
}

/**
 * Return the super class scope of the current method context.
 *
 * @return The superclass scope object.
 */
RexxObject *RexxNativeActivation::getSuper()
{
    return getSelf()->superScope(this->method->getScope());
}

/**
 * Resolve an argument object into a stem object.  The argument
 * object may already be a stem, or may the a variable name
 * used to resolve a stem.
 *
 * @param s      The source object used for the resolution.
 *
 * @return A resolved stem object.  Returns a NULLOBJECT if there is
 *         a resolution problem.
 */
RexxStem *RexxNativeActivation::resolveStemVariable(RexxObject *s)
{
    // is this a stem already?
    if (isStem(s))
    {
        return (RexxStem *)s;
    }

    /* force to a string value           */
    RexxString *temp = REQUIRED_STRING(s, 1);
    // see if we can retrieve this stem
    return (RexxStem *)getContextStem(temp);
}
