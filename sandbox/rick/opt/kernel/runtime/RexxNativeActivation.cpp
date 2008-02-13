/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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
/* REXX Kernel                                      RexxNativeActivation.c    */
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
#include "BufferClass.hpp"
#include "MessageClass.hpp"
#include "RexxVariableDictionary.hpp"
#include "SourceFile.hpp"
#include "RexxCode.hpp"
#include "RexxInstruction.hpp"
#include "ExpressionBaseVariable.hpp"
#include "ProtectedObject.hpp"
#include "RexxNativeAPI.h"                      /* bring in the native code defines  */
#include "PointerClass.hpp"
#include "ActivityDispatcher.hpp"
#include "CallbackDispatcher.hpp"

#include <math.h>
#include <limits.h>


/**
 * Constructor for a new native activation used to create a
 * callback context for exit call outs.
 *
 * @param _activity The activity we're running under.
 */
RexxNativeActivation::RexxNativeActivation(RexxActivity *_activity, RexxActivation*_activation)
{
    this->activity = _activity;      /* the activity running on           */
    this->activation = _activation;  // our parent context
}


/**
 * Constructor for a new native activation used to create a
 * callback context for exit call outs.
 *
 * @param _activity The activity we're running under.
 */
RexxNativeActivation::RexxNativeActivation(RexxActivity *_activity)
{
    this->activity = _activity;      /* the activity running on           */
}


void RexxNativeActivation::live(size_t liveMark)
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  memory_mark(this->argArray);
  memory_mark(this->receiver);
  memory_mark(this->method);
  memory_mark(this->routine);
  memory_mark(this->activity);
  memory_mark(this->activation);
  memory_mark(this->msgname);
  memory_mark(this->savelist);
  memory_mark(this->result);
  memory_mark(this->nextstem);
  memory_mark(this->compoundelement);
  memory_mark(this->nextcurrent);
  memory_mark(this->objectVariables);
  memory_mark(this->conditionObj);

  /* We're hold a pointer back to our arguments directly where they */
  /* are created.  Since in some places, this argument list comes */
  /* from the C stack, we need to handle the marker ourselves. */
  size_t i;
  for (i = 0; i < argcount; i++)
  {
      memory_mark(arglist[i]);
  }
}

void RexxNativeActivation::liveGeneral(int reason)
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  memory_mark_general(this->argArray);
  memory_mark_general(this->receiver);
  memory_mark_general(this->method);
  memory_mark_general(this->routine);
  memory_mark_general(this->activity);
  memory_mark_general(this->activation);
  memory_mark_general(this->msgname);
  memory_mark_general(this->savelist);
  memory_mark_general(this->result);
  memory_mark_general(this->nextstem);
  memory_mark_general(this->compoundelement);
  memory_mark_general(this->nextcurrent);
  memory_mark_general(this->objectVariables);
  memory_mark_general(this->conditionObj);

  /* We're hold a pointer back to our arguments directly where they */
  /* are created.  Since in some places, this argument list comes */
  /* from the C stack, we need to handle the marker ourselves. */
  size_t i;
  for (i = 0; i < argcount; i++)
  {
      memory_mark_general(arglist[i]);
  }
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


/**
 * Process the arguments for a typed function/method call.
 *
 * @param argcount The count of arguments.
 * @param arglist  The original Rexx arguments.
 * @param argumentTypes
 *                 The type descriptor from the target.
 * @param descriptors
 *                 The maximum argument count for the target.
 * @param maximumArgumentCount
 */
void RexxNativeActivation::processArguments(size_t argcount, RexxObject **arglist, uint16_t *argumentTypes,
    ValueDescriptor *descriptors, size_t maximumArgumentCount)
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
                descriptors[outputIndex].value.value_RexxObjectPtr = this->receiver;
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

                        case REXX_VALUE_logical_t:         /* integer value                     */
                        {
                            descriptors[outputIndex].value.value_logical_t = argument->truthValue(Error_Logical_value_method);
                            break;
                        }

                        // The Rexx whole number one is checked against the human digits limit
                        case REXX_VALUE_wholenumber_t:  /* number value                      */
                        {
                            descriptors[outputIndex].value.value_wholenumber_t = (wholenumber_t)wholeNumberValue(argument, inputIndex, Numerics::MAX_WHOLENUMBER, Numerics::MIN_WHOLENUMBER);
                            break;
                        }

                        // an unsigned string number value
                        case REXX_VALUE_stringsize_t:
                        {
                            descriptors[outputIndex].value.value_stringsize_t = (stringsize_t)unsignedNumberValue(argument, inputIndex, Numerics::MAX_STRINGSIZE);
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
                        missing_argument(inputIndex + 1);
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
                        case REXX_VALUE_wholenumber_t:     /* non-existent long                 */
                        case REXX_VALUE_stringsize_t:   /* non-existent long                 */
                        case REXX_VALUE_logical_t:      // this must be a boolean value
                        case REXX_VALUE_size_t:         /* non-existent double               */
                        case REXX_VALUE_CSTRING:        /* missing character string          */
                        case REXX_VALUE_POINTER:
                        {
                            // set this as a 64-bit value to clear everything out
                            descriptors[outputIndex].value.value_int64_t = 0;
                            break;
                        }
                        case REXX_VALUE_double:         /* non-existent double               */
                        {
                            descriptors[outputIndex].value.value_double = 0.0;
                            break;
                        }
                        case REXX_VALUE_float:          /* non-existent double               */
                        {
                            descriptors[outputIndex].value.value_float = 0.0;
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


/**
 * Convert an API value descriptor into a Rexx object.
 *
 * @param value  The self-describing value descriptor.
 *
 * @return The described value converted to an appropriate Rexx object.
 */
RexxObject *RexxNativeActivation::valueToObject(ValueDescriptor *value)
{
    switch (value->type)
    {
        case REXX_VALUE_RexxObjectPtr:          /* Object reference                  */
        {
            return (RexxObject *)value->value.value_RexxObjectPtr; // just return the object value
        }

        case REXX_VALUE_int:                    /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_int);
        }

        case REXX_VALUE_int8_t:                         /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_int8_t);
        }

        case REXX_VALUE_int16_t:                        /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_int16_t);
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

        case REXX_VALUE_uint8_t:                         /* integer value                     */
        {
            return Numerics::toObject((stringsize_t)value->value.value_uint8_t);
        }

        case REXX_VALUE_uint16_t:                        /* integer value                     */
        {
            return Numerics::toObject((stringsize_t)value->value.value_uint16_t);
        }

        case REXX_VALUE_uint32_t:                        /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_uint32_t);
        }

        case REXX_VALUE_uint64_t:                        /* integer value                     */
        {
            return Numerics::toObject(value->value.value_uint64_t);
        }

        case REXX_VALUE_uintptr_t:                       /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_uintptr_t);
        }

        case REXX_VALUE_size_t:                        /* integer value                     */
        {
            return Numerics::toObject((stringsize_t)value->value.value_size_t);
        }

        case REXX_VALUE_ssize_t:                        /* integer value                     */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_size_t);
        }

        case REXX_VALUE_wholenumber_t:        /* long integer value                */
        {
            return Numerics::toObject((wholenumber_t)value->value.value_wholenumber_t);
        }

        case REXX_VALUE_stringsize_t:     /* long integer value                */
        {
            return Numerics::toObject((stringsize_t)value->value.value_stringsize_t);
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
            const char *string = value->value.value_CSTRING;
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

        case REXX_VALUE_logical_t:         /* integer value                     */
        {
            // this will raise an error, which we'll clear before return
            value->value.value_logical_t = o->truthValue(Error_Logical_value_method);
            return true;
        }

        // The Rexx whole number one is checked against the human digits limit
        case REXX_VALUE_wholenumber_t:  /* number value                      */
        {
            wholenumber_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToWholeNumber(o, temp, Numerics::MAX_WHOLENUMBER, Numerics::MIN_WHOLENUMBER);
            value->value.value_wholenumber_t = (wholenumber_t)temp;
            return success;
        }

        // The Rexx whole number one is checked against the human digits limit
        case REXX_VALUE_ssize_t:  /* ssize_t value                     */
        {
            wholenumber_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToWholeNumber(o, temp, SSIZE_MAX, SSIZE_MIN);
            value->value.value_wholenumber_t = (wholenumber_t)temp;
            return success;
        }

        // an unsigned string number value
        case REXX_VALUE_stringsize_t:
        {
            stringsize_t temp = 0;
            // convert and copy                  */
            bool success = Numerics::objectToStringSize(o, temp, Numerics::MAX_STRINGSIZE);
            value->value.value_stringsize_t = temp;
            return success;
        }

        case REXX_VALUE_double:         /* double value                      */
        {
            return o->doubleValue(value->value.value_double);
        }


        case REXX_VALUE_float:          /* float value                      */
        {
            double temp = 0.0;
            bool success = o->doubleValue(temp);
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


/**
 * Create a local reference for an object.  The protects the object
 * from GC until the environment terminates.
 *
 * @param objr   The object to protect.
 */
void RexxNativeActivation::createLocalReference(RexxObject *objr)
{
    // if we have a real object, then add to the list
    if (objr != OREF_NULL)
    {
        // make sure we protect this from a GC triggered by this table creation.
        ProtectedObject p1(objr);
        if (this->savelist == OREF_NULL) /* second saved object?              */
        {
            /* create the save list now          */
            this->savelist = new_list();
        }
        /* add to the save table             */
        this->savelist->append(objr);
    }
}


/**
 * Remove an object from the local reference table.
 *
 * @param objr   The object to remove.
 */
void RexxNativeActivation::removeLocalReference(RexxObject *objr)
{
    // if the reference is non-null
  if (objr != OREF_NULL)
  {
      // make sure we have a savelist before trying to remove this
      if (savelist != OREF_NULL)
      {
          savelist->removeItem(objr);
      }
  }
}


/**
 * Run a native method or function under the context of this activation.
 *
 * @param _receiver The receiver object (NULL if not a method invocation).
 * @param _msgname  The message name.
 * @param _argcount The argument count
 * @param _arglist  The list of arguments.
 * @param resultObj The returned result object.
 */
void RexxNativeActivation::run(RexxMethod *_method, RexxNativeMethod *_code, RexxObject  *_receiver,
    RexxString  *_msgname, RexxObject **_arglist, size_t _argcount, ProtectedObject &resultObj)
{
    // anchor the relevant context information
    method = _method;
    receiver = _receiver;
    msgname = _msgname;
    arglist = _arglist;
    argcount = _argcount;
    activationType = METHOD_ACTIVATION;      // this is for running a method

    ValueDescriptor arguments[MAX_NATIVE_ARGUMENTS];

    MethodContext context;               // the passed out method context

    // build a context pointer to pass out
    activity->createMethodContext(context, this);

    context.threadContext.arguments = arguments;

    // get the entry point address of the target method
    PNATIVEMETHOD methp = _code->getEntry();

    // retrieve the argument signatures and process them
    uint16_t *types = (*methp)((RexxMethodContext *)&context, NULL);
    processArguments(argcount, arglist, types, arguments, MAX_NATIVE_ARGUMENTS);

    size_t activityLevel = this->activity->getActivationLevel();
    try
    {
        activity->releaseAccess();           /* force this to "safe" mode         */
                                             /* process the method call           */
        (*methp)((RexxMethodContext *)&context, arguments);
        activity->requestAccess();           /* now in unsafe mode again          */

        // process the returned result
        result = valueToObject(arguments);
    }
    catch (RexxActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }

        // it's possible that we can get terminated by a throw during condition processing.
        // we intercept this here, perform any cleanup we need to perform, then let the
        // condition trap propagate.
        this->guardOff();                  /* release any variable locks        */
        this->argcount = 0;                /* make sure we don't try to mark any arguments */
        // the lock holder gets here by longjmp from a kernel reentry.  We need to
        // make sure the activation count gets reset, else we'll accumulate bogus
        // nesting levels that will make it look like this activity is still in use
        // when in fact we're done with it.
        this->activity->restoreActivationLevel(activityLevel);
        // IMPORTANT NOTE:  We don't pop our activation off the stack.  This will be
        // handled by the catcher.  Attempting to pop the stack when an error or condition has
        // occurred can munge the activation stack, resulting bad stuff.
        this->setHasNoReferences();        /* mark this as not having references in case we get marked */
        // now rethrow the trapped condition so that real target can handle this.
        throw;
    }
    catch (RexxNativeActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }
        this->guardOff();                  /* release any variable locks        */
        this->argcount = 0;                /* make sure we don't try to mark any arguments */
        // the lock holder gets here by longjmp from a kernel reentry.  We need to
        // make sure the activation count gets reset, else we'll accumulate bogus
        // nesting levels that will make it look like this activity is still in use
        // when in fact we're done with it.
        this->activity->restoreActivationLevel(activityLevel);
        this->activity->popStackFrame(this);   /* pop this from the activity        */
        this->setHasNoReferences();        /* mark this as not having references in case we get marked */
        // set the return value and get outta here
        resultObj = this->result;
        return;
    }

    // belt and braces...this restores the activity level to whatever
    // level we had when we made the callout.
    this->activity->restoreActivationLevel(activityLevel);

    /* give up reference to receiver so that it can be garbage collected */
    this->receiver = OREF_NULL;

    // set the return value and get outta here
    resultObj = this->result;
    // Use protected object to pass back the result
    this->guardOff();                    /* release any variable locks        */
    this->argcount = 0;                  /* make sure we don't try to mark any arguments */
    this->activity->popStackFrame(this); /* pop this from the activity        */
    this->setHasNoReferences();          /* mark this as not having references in case we get marked */
}


/**
 * Process a native function call.
 *
 * @param routine   The routine we're executing (used for context resolution).
 * @param code      The code object.
 * @param functionName
 *                  The name of the function.
 * @param list      The list of arguments.
 * @param count     The number of arguments.
 * @param resultObj The return value.
 */
void RexxNativeActivation::callNativeRoutine(RoutineClass *_routine, RexxNativeRoutine *code, RexxString *functionName,
    RexxObject **list, size_t count, ProtectedObject &resultObj)
{
    // anchor the context stuff
    routine = routine;
    msgname = functionName;
    arglist = list;
    argcount = count;
    activationType = FUNCTION_ACTIVATION;      // this is for running a method

    ValueDescriptor arguments[MAX_NATIVE_ARGUMENTS];

    CallContext context;               // the passed out method context

    // build a context pointer to pass out
    activity->createCallContext(context, this);

    context.threadContext.arguments = arguments;

    // get the entry point address of the target method
    PNATIVEROUTINE methp = code->getEntry();

    // retrieve the argument signatures and process them
    uint16_t *types = (*methp)((RexxCallContext *)&context, NULL);
    processArguments(argcount, arglist, types, arguments, MAX_NATIVE_ARGUMENTS);

    size_t activityLevel = this->activity->getActivationLevel();
    try
    {
        activity->releaseAccess();           /* force this to "safe" mode         */
                                             /* process the method call           */
        (*methp)((RexxCallContext *)&context, arguments);
        activity->requestAccess();           /* now in unsafe mode again          */

        // process the returned result
        result = valueToObject(arguments);
    }
    catch (RexxActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }

        this->argcount = 0;                /* make sure we don't try to mark any arguments */
        // the lock holder gets here by longjmp from a kernel reentry.  We need to
        // make sure the activation count gets reset, else we'll accumulate bogus
        // nesting levels that will make it look like this activity is still in use
        // when in fact we're done with it.
        this->activity->restoreActivationLevel(activityLevel);
        // IMPORTANT NOTE:  We don't pop our activation off the stack.  This will be
        // handled by the catcher.  Attempting to pop the stack when an error or condition has
        // occurred can munge the activation stack, resulting bad stuff.
        this->setHasNoReferences();        /* mark this as not having references in case we get marked */
        // now rethrow the trapped condition so that real target can handle this.
        throw;
    }
    catch (RexxNativeActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }
        this->argcount = 0;                /* make sure we don't try to mark any arguments */
        // the lock holder gets here by longjmp from a kernel reentry.  We need to
        // make sure the activation count gets reset, else we'll accumulate bogus
        // nesting levels that will make it look like this activity is still in use
        // when in fact we're done with it.
        this->activity->restoreActivationLevel(activityLevel);
        this->activity->popStackFrame(this);   /* pop this from the activity        */
        this->setHasNoReferences();        /* mark this as not having references in case we get marked */
        // set the return value and get outta here
        resultObj = this->result;
        return;
    }

    // belt and braces...this restores the activity level to whatever
    // level we had when we made the callout.
    this->activity->restoreActivationLevel(activityLevel);

    /* give up reference to receiver so that it can be garbage collected */
    this->receiver = OREF_NULL;

    // set the return value and get outta here
    resultObj = this->result;
    this->argcount = 0;                  /* make sure we don't try to mark any arguments */
    this->activity->popStackFrame(this); /* pop this from the activity        */
    this->setHasNoReferences();          /* mark this as not having references in case we get marked */
}


/**
 * Process a native function call.
 *
 * @param entryPoint The target function entry point.
 * @param list       The list of arguments.
 * @param count      The number of arguments.
 * @param result     A protected object to receive the function result.
 */
void RexxNativeActivation::callRegisteredRoutine(RoutineClass *_routine, RegisteredRoutine *code, RexxString *functionName,
    RexxObject **list, size_t count, ProtectedObject &resultObj)
{
    // anchor the context stuff
    msgname = functionName;
    routine = _routine;
    arglist = list;
    argcount = count;

    activationType = FUNCTION_ACTIVATION;      // this is for running a method

    // get the entry point address of the target method
    RexxRoutineHandler *methp = code->getEntry();

    CONSTRXSTRING   arguments[MAX_NATIVE_ARGUMENTS];
    CONSTRXSTRING *argPtr = arguments;

    // unlike the other variants, we don't have a cap on arguments.  If we have more than the preallocated
    // set, then dynamically allocate a buffer object to hold the memory and anchor it in the
    // activation saved objects.
    if (count > MAX_NATIVE_ARGUMENTS)
    {
        RexxBuffer *buffer = new_buffer(sizeof(CONSTRXSTRING) * count);
        // this keeps the buffer alive until the activation is popped.
        createLocalReference(buffer);
        argPtr = (CONSTRXSTRING *)buffer->address();
    }

    // all of the arguments now need to be converted to string arguments
    for (size_t argindex=0; argindex < count; argindex++)
    {
        /* get the next argument             */
        RexxObject *argument = list[argindex];
        if (argument != OREF_NULL)       /* have an argument?                 */
        {
            /* force to string form              */
            RexxString *stringArgument = argument->stringValue();
            // if the string version is not the same, we're going to need to make
            // sure the string version is protected from garbage collection until
            // the call is finished
            if (stringArgument != argument)
            {
                // make sure this is protected
                createLocalReference(stringArgument);
            }
            stringArgument->toRxstring(argPtr[argindex]);
        }
        else                             /* have an omitted argument          */
        {
            /* give it zero length               */
            argPtr[argindex].strlength = 0;
            /* and a zero pointer                */
            argPtr[argindex].strptr = NULL;
        }
    }
    /* get the current queue name        */
    const char *queuename = SysGetCurrentQueue()->getStringData();
    RXSTRING funcresult;
    int functionrc;                      /* Return code from function         */
    /* default return code buffer        */
    char default_return_buffer[DEFRXSTRING];

    /* make the RXSTRING result          */
    MAKERXSTRING(funcresult, default_return_buffer, sizeof(default_return_buffer));

    size_t activityLevel = this->activity->getActivationLevel();
    try
    {
        activity->releaseAccess();           /* force this to "safe" mode         */
        // now process the function call
        functionrc = (*methp)(functionName->getStringData(), count, argPtr, queuename, &funcresult);
        activity->requestAccess();           /* now in unsafe mode again          */
    }
    catch (RexxActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }

        this->argcount = 0;                /* make sure we don't try to mark any arguments */
        // the lock holder gets here by longjmp from a kernel reentry.  We need to
        // make sure the activation count gets reset, else we'll accumulate bogus
        // nesting levels that will make it look like this activity is still in use
        // when in fact we're done with it.
        this->activity->restoreActivationLevel(activityLevel);
        // IMPORTANT NOTE:  We don't pop our activation off the stack.  This will be
        // handled by the catcher.  Attempting to pop the stack when an error or condition has
        // occurred can munge the activation stack, resulting bad stuff.
        this->setHasNoReferences();        /* mark this as not having references in case we get marked */
        // now rethrow the trapped condition so that real target can handle this.
        throw;
    }
    catch (RexxNativeActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }
        this->argcount = 0;                /* make sure we don't try to mark any arguments */
        // the lock holder gets here by longjmp from a kernel reentry.  We need to
        // make sure the activation count gets reset, else we'll accumulate bogus
        // nesting levels that will make it look like this activity is still in use
        // when in fact we're done with it.
        this->activity->restoreActivationLevel(activityLevel);
        this->activity->popStackFrame(this);   /* pop this from the activity        */
        this->setHasNoReferences();        /* mark this as not having references in case we get marked */
        // set the return value and get outta here
        resultObj = this->result;
        return;
    }

    // belt and braces...this restores the activity level to whatever
    // level we had when we made the callout.
    this->activity->restoreActivationLevel(activityLevel);

    // now process the function return value
    if (functionrc == 0)             /* If good rc from function          */
    {
        if (funcresult.strptr != NULL)         /* If we have a result, return it    */
        {
            /* make a string result              */
            resultObj = new_string(funcresult);
            /* user give us a new buffer?        */
            if (funcresult.strptr != default_return_buffer )
            {
                /* free it                           */
                SysReleaseResultMemory(funcresult.strptr);
            }
        }
    }
    else                             /* Bad rc from function, signal      */
    {
        /* error                             */
        reportException(Error_Incorrect_call_external, functionName);
    }

    this->argcount = 0;                  /* make sure we don't try to mark any arguments */
    this->activity->popStackFrame(this); /* pop this from the activity        */
    this->setHasNoReferences();          /* mark this as not having references in case we get marked */
}


/**
 * Run a task under the scope of a native activation.  This is
 * generally a bootstrapping call, such as a top-level program call,
 * method translation, etc.
 *
 * @param dispatcher The dispatcher instance we're going to run.
 */
void RexxNativeActivation::run(ActivityDispatcher &dispatcher)
{
    activationType = DISPATCHER_ACTIVATION;  // this is for running a dispatcher
    size_t activityLevel = this->activity->getActivationLevel();
    // make the activation hookup
    dispatcher.setContext(activity, this);
    dispatcher.run();

    // belt and braces...this restores the activity level to whatever
    // level we had when we made the callout.
    this->activity->restoreActivationLevel(activityLevel);

    this->activity->popStackFrame(this); /* pop this from the activity        */
    this->setHasNoReferences();          /* mark this as not having references in case we get marked */
    return;                              /* and finished                      */
}


/**
 * Run a callback under the scope of a native actvation. This is
 * generally a call out, such as a system exit, argument
 * callback, etc.
 *
 * @param dispatcher The dispatcher instance we're going to run.
 */
void RexxNativeActivation::run(CallbackDispatcher &dispatcher)
{
    activationType = CALLBACK_ACTIVATION;    // we're handling a callback
    size_t activityLevel = this->activity->getActivationLevel();
    try
    {
        // make the activation hookup
        // make the activation hookup
        dispatcher.setContext(activity, this);
        activity->releaseAccess();           /* force this to "safe" mode         */
        dispatcher.run();
        activity->requestAccess();           /* now in unsafe mode again          */
    }
    catch (ActivityException)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }
        // pass the condition information on to the dispatch unig
        dispatcher.handleError(activity->getCurrentCondition());
        return;
    }
    catch (RexxActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }

        this->activity->restoreActivationLevel(activityLevel);
        // IMPORTANT NOTE:  We don't pop our activation off the stack.  This will be
        // handled by the catcher.  Attempting to pop the stack when an error or condition has
        // occurred can munge the activation stack, resulting bad stuff.
        this->setHasNoReferences();        /* mark this as not having references in case we get marked */
        // now rethrow the trapped condition so that real target can handle this.
        throw;
    }
    catch (RexxNativeActivation *)
    {
        // if we're not the current kernel holder when things return, make sure we
        // get the lock before we continue
        if (ActivityManager::currentActivity != activity)
        {
            activity->requestAccess();
        }
        this->activity->restoreActivationLevel(activityLevel);
        return;
    }
    return;                             /* and finished                      */
}


RexxVariableDictionary *RexxNativeActivation::methodVariables()
/******************************************************************************/
/* Function:  Retrieve the set of method variables                            */
/******************************************************************************/
{
    /* first retrieval?                  */
    if (this->objectVariables == OREF_NULL)
    {
        // not a method invocation?
        if (receiver == OREF_NULL)
        {
            /* retrieve the method variables     */
            this->objectVariables = ((RexxActivation *)this->receiver)->getLocalVariables();
        }
        else
        {
            /* must be wanting the ovd set of    */
            /*variables                          */
            this->objectVariables = (RexxVariableDictionary *)this->receiver->getObjectVariables(this->method->getScope());
            /* guarded method?                   */
            if (this->object_scope == SCOPE_RELEASED && this->method->isGuarded())
            {
                /* reserve the variable scope        */
                this->objectVariables->reserve(this->activity);
                /* and remember for later            */
                this->object_scope = SCOPE_RESERVED;
            }
        }
    }
    return this->objectVariables;        /* return the dictionary             */
}


bool RexxNativeActivation::isInteger(
    RexxObject *object)                /* object to validate                */
/******************************************************************************/
/* Function:  Validate that an object has an integer value                    */
/******************************************************************************/
{
    wholenumber_t temp;
    return object->numberValue(temp, this->digits());
}

/**
 * Convert a value to a wholenumber value.
 *
 * @param o        The object to convert.
 * @param position The argument position.
 * @param maxValue The maximum value allowed in the range.
 * @param minValue The minimum range value.
 *
 * @return The converted number.
 */
wholenumber_t RexxNativeActivation::wholeNumberValue(RexxObject *o, size_t position, wholenumber_t maxValue, wholenumber_t minValue)
{
    wholenumber_t temp;

    // convert using the whole value range
    if (!Numerics::objectToWholeNumber(o, temp, maxValue, minValue))
    {
        if (activationType == METHOD_ACTIVATION)
        {
            /* this is an error                  */
            reportException(Error_Incorrect_method_whole, position + 1, o);
        }
        else
        {
            /* this is an error                  */
            reportException(Error_Incorrect_call_whole, position + 1, o);
        }
    }
    return temp;
}



/**
 * Convert a value to a stringsize value.
 *
 * @param o        The object to convert.
 * @param position The argument position.
 * @param maxValue The maximum value allowed in the range.
 *
 * @return The converted number.
 */
stringsize_t RexxNativeActivation::unsignedNumberValue(RexxObject *o, size_t position, stringsize_t maxValue)
{
    stringsize_t temp;

    // convert using the whole value range
    if (!Numerics::objectToStringSize(o, temp, maxValue))
    {
        if (activationType == METHOD_ACTIVATION)
        {
            /* this is an error                  */
            reportException(Error_Incorrect_method_whole, position + 1, o);
        }
        else
        {
            /* this is an error                  */
            reportException(Error_Incorrect_call_whole, position + 1, o);
        }
    }
    return temp;
}


/**
 * Convert a value to an int64_t value
 *
 * @param o        The object to convert.
 * @param position The argument position.
 *
 * @return The converted number.
 */
int64_t RexxNativeActivation::int64Value(RexxObject *o, size_t position)
{
    int64_t temp;

    // convert using the whole value range
    if (!Numerics::objectToInt64(o, temp))
    {
        if (activationType == METHOD_ACTIVATION)
        {
            /* this is an error                  */
            reportException(Error_Incorrect_method_whole, position + 1, o);
        }
        else
        {
            /* this is an error                  */
            reportException(Error_Incorrect_call_whole, position + 1, o);
        }
    }
    return temp;
}


/**
 * Convert a value to a  uint64_t value
 *
 * @param o        The object to convert.
 * @param position The argument position.
 *
 * @return The converted number.
 */
uint64_t RexxNativeActivation::unsignedInt64Value(RexxObject *o, size_t position)
{
    uint64_t temp;

    // convert using the whole value range
    if (!Numerics::objectToUnsignedInt64(o, temp))
    {
        if (activationType == METHOD_ACTIVATION)
        {
            /* this is an error                  */
            reportException(Error_Incorrect_method_whole, position + 1, o);
        }
        else
        {
            /* this is an error                  */
            reportException(Error_Incorrect_call_whole, position + 1, o);
        }
    }
    return temp;
}


const char *RexxNativeActivation::cstring(
    RexxObject *object)                /* object to convert                 */
/******************************************************************************/
/* Function:  Return an object as a CSTRING                                   */
/******************************************************************************/
{
    /* force to a string value           */
    RexxString *string = (RexxString *)object->stringValue();
    if (string != object)                /* different value?                  */
    {
        /* make it safe                      */
        createLocalReference(string);
    }
    return string->getStringData();           /* just point to the string data     */
}


double RexxNativeActivation::getDoubleValue(
    RexxObject *object)                /* object to convert                 */
/******************************************************************************/
/* Function:  Convert an object to a double                                   */
/******************************************************************************/
{
    double r;                            /* returned result                   */
                                         /* convert and check result          */
    if (!object->doubleValue(r))
    {
        /* conversion error                  */
        reportException(Error_Execution_nodouble, object);
    }
    return r;                            /* return converted number           */
}


bool RexxNativeActivation::isDouble(
    RexxObject *object)                /* object to check                   */
/******************************************************************************/
/* Function:  Test to see if an object is a valid double                      */
/******************************************************************************/
{
    double r;                            /* returned result                   */
                                         /* convert and check result          */
    return object->doubleValue(r);
}

void *RexxNativeActivation::cself()
/******************************************************************************/
/* Function:  Returns "unwrapped" C or C++ object associated with this        */
/*            object instance.  If the variable CSELF does not exist, then    */
/*            NULL is returned.                                               */
/******************************************************************************/
{
    /* retrieve from object dictionary   */
    RexxPointer *C_self = (RexxPointer *)methodVariables()->realValue(OREF_CSELF);
    if (C_self != OREF_NULL)             /* got an item?                      */
    {
        return C_self->pointer();        /* return the pointer value          */
    }
    else
    {
        return NULL;                     /* no object available               */
    }
}


void *RexxNativeActivation::buffer()
/******************************************************************************/
/* Function:  Returns "unwrapped" C or C++ object stored in a buffer object.  */
/*            If the variable CSELF does not exist, then NULL is returned.    */
/******************************************************************************/
{
    /* retrieve from object dictionary   */
    RexxBuffer *C_self = (RexxBuffer *)this->methodVariables()->realValue(OREF_CSELF);
    if (C_self != OREF_NULL)             /* got an item?                      */
    {
        return(void *)C_self->address();  /* return a pointer to the address   */
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
    if (!object->isInstanceOf(ThePointerClass))
    {
        return NULL;
    }
    // just unwrap thee pointer
    return ((RexxPointer *)object)->pointer();
}


RexxObject *RexxNativeActivation::dispatch()
/******************************************************************************/
/* Function:  Redispatch an activation on a different activity                */
/******************************************************************************/
{
    ProtectedObject r;
    this->run(receiver, msgname, argcount, arglist, r);  /* just do a method run              */
    return (RexxObject *)r;
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

size_t RexxNativeActivation::digits()
/******************************************************************************/
/* Function:  Return the current digits setting                               */
/******************************************************************************/
{
    /* have a real one?                  */
    if (activation == OREF_NULL)
    {
        return Numerics::DEFAULT_DIGITS;   /*  no, just return default value    */
    }
    else
    {
        return activation->digits();       /* pass on the the sender            */
    }
}

size_t RexxNativeActivation::fuzz()
/******************************************************************************/
/* Function:  Return the current fuzz setting                                 */
/******************************************************************************/
{
    /* have a real one?                  */
    if (activation == OREF_NULL)
    {
        return Numerics::DEFAULT_FUZZ;     /*  no, just return default value    */
    }
    else
    {
        return activation->fuzz();         /* pass on the the sender            */
    }
}

bool RexxNativeActivation::form()
/******************************************************************************/
/* Function:  Return the curren form setting                                  */
/******************************************************************************/
{
    /* have a real one?                  */
    if (activation == OREF_NULL)
    {
        return Numerics::DEFAULT_FORM;     /*  no, just return default value    */
    }
    else
    {
        return activation->form();         /* pass on the the sender            */
    }
}

void RexxNativeActivation::setDigits(
    size_t _digits)                     /* new NUMERIC DIGITS value          */
/******************************************************************************/
/* Function:  Set a new numeric digits setting                                */
/******************************************************************************/
{
    /* have a real one?                  */
    if (activation == OREF_NULL)
    {
        activation->setDigits(_digits);      /* just forward the set              */
    }
}

void RexxNativeActivation::setFuzz(
    size_t _fuzz )                     /* new NUMERIC FUZZ value            */
/******************************************************************************/
/* Function:  Set a new numeric fuzz setting                                  */
/******************************************************************************/
{
    /* have a real one?                  */
    if (activation != OREF_NULL)
    {
        activation->setFuzz(_fuzz);         /* just forward the set              */
    }
}

void RexxNativeActivation::setForm(
    bool _form )                        /* new NUMERIC FORM value            */
/******************************************************************************/
/* Function:  Set a new numeric form setting                                  */
/******************************************************************************/
{
    /* have a real one?                  */
    if (activation == OREF_NULL)
    {
        activation->setForm(_form);        /* just forward the set              */
    }
}


/**
 * Get the numeric settings for this native activation.  If we're
 * running in the direct call context from a Rexx activation, then
 * the settings are those inherited from the Rexx context.  Otherwise,
 * we just return the default numeric settings.
 *
 * @return The current Numeric settings.
 */
NumericSettings *RexxNativeActivation::getNumericSettings()
{
    if (activation == OREF_NULL)
    {
        return Numerics::getDefaultSettings();
    }
    else
    {
        return activation->getNumericSettings();
    }
}


/**
 * Indicate whether this activation represents the base of the call
 * stack.
 *
 * @return true if this is a base activation.
 */
bool RexxNativeActivation::isStackBase()
{
    return stackBase;
}


/**
 * Return the Rexx context this operates under.  Depending on the
 * context, this could be null.
 *
 * @return The parent Rexx context.
 */
RexxActivation *RexxNativeActivation::getRexxContext()
{
    return activation;    // this might be null
}


/**
 * Get the message receiver
 *
 * @return The message receiver.  Returns OREF_NULL if this is not
 *         a message activation.
 */
RexxObject *RexxNativeActivation::getReceiver()
{
    return receiver;
}


void RexxNativeActivation::guardOff()
/******************************************************************************/
/* Function:  Release a variable pool guard lock                              */
/******************************************************************************/
{
    /* currently locked?                 */
    if (this->object_scope == SCOPE_RESERVED)
    {
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
    // if there's no receiver object, then this is not a true method call.
    // there's nothing to lock
    if (receiver == OREF_NULL)
    {
        return;
    }
    /* first retrieval? */
    if (this->objectVariables == OREF_NULL)
    {
        /* grab the object variables associated with this object */
        this->objectVariables = this->receiver->getObjectVariables(this->method->getScope());
    }
    /* not currently holding the lock? */
    if (this->object_scope == SCOPE_RELEASED)
    {
        /* reserve the variable scope */
        this->objectVariables->reserve(this->activity);
        /* and remember for later */
        this->object_scope = SCOPE_RESERVED;
    }
}

void RexxNativeActivation::enableVariablepool()
/******************************************************************************/
/* Function:  Enable the variable pool                                        */
/******************************************************************************/
{
  this->resetNext();                   /* reset fetch next calls            */
  this->vpavailable = true;            /* allow the calls                   */
}

void RexxNativeActivation::disableVariablepool()
/******************************************************************************/
/* Function:  Disable the variable pool                                       */
/******************************************************************************/
{
  this->resetNext();                   /* reset fetch next calls            */
  this->vpavailable = false;           /* no more external calls            */
}

void RexxNativeActivation::resetNext()
/******************************************************************************/
/* Function: Reset the next state of the variable pool                        */
/******************************************************************************/
{
  this->nextvariable = SIZE_MAX;       /* turn off next index               */
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
    RexxStem     *stemVar;               /* a potential stem variable collection */

                                         /* starting off fresh?               */
    if (nextCurrent() == OREF_NULL)
    {
        /* grab the activation context */
        RexxActivation *act = activity->getCurrentRexxFrame();
        setNextVariable(SIZE_MAX);         /* request the first item            */
        /* Get the local variable dictionary from the context. */
        setNextCurrent(act->getLocalVariables());
        /* we are not on a stem              */
        setNextStem(OREF_NULL);
        setCompoundElement(OREF_NULL);
    }

    for (;;)                             /* loop until we get something       */
    {
        stemVar = nextStem();              /* see if we're processing a stem variable */
        if (stemVar != OREF_NULL)          /* were we in the middle of processing a stem? */
        {
            compound = stemVar->nextVariable(this);
            if (compound != OREF_NULL)     /* if we still have elements here */
            {
                /* create a full stem name           */
                *name = compound->createCompoundName(stemVar->getName());
                /* get the value                     */
                *value = compound->getVariableValue();
                return true;
            }
            else                           /* we've reached the end of the stem, reset */
            {
                /* to the main dictionary and continue */
                setNextStem(OREF_NULL);
                setCompoundElement(OREF_NULL);
            }
        }
        /* get the next variable             */
        variable = nextCurrent()->nextVariable(this);
        if (variable == OREF_NULL)         /* reached the end of the table      */
        {
            return false;
        }
        else                               /* have a real variable              */
        {
            /* get the value                     */
            RexxObject *variable_value = variable->getVariableValue();
            /* found a stem item?                */
            if (isOfClass(Stem, variable_value))
            {
                /* we are not on a stem              */
                setNextStem((RexxStem *)variable_value);
                setCompoundElement(((RexxStem *)variable_value)->first());
                /* set up an iterator for the stem   */
            }
            else                             /* found a real variable             */
            {
                *value = variable_value;       /* pass back the value (name already set) */
                *name = variable->getName();
                return true;                   /* we have another variable to return */
            }
        }
    }
}


bool RexxNativeActivation::trap(
    RexxString    * condition,         /* name of the condition             */
    RexxDirectory * exception_object)  /* exception information             */
{
  return false;                        /* this wasn't handled               */
}


/**
 * Raise a condition on behalf of a native method.  This method
 * does not return.
 *
 * @param condition  The condition type to raise.
 * @param description
 *                   The condition description string.
 * @param additional The additional information associated with this condition.
 * @param result     The result object.
 */
void RexxNativeActivation::raiseCondition(RexxString *condition, RexxString *description, RexxObject *additional, RexxObject *_result)
{
    this->result = (RexxObject *)_result; /* save the result                   */
                                         /* go raise the condition            */
    this->activity->raiseCondition(condition, OREF_NULL, description, additional, result, OREF_NULL);

    // We only return here if no activation above us has trapped this.  If we do return, then
    // we terminate the call by throw this up the stack.
    throw this;
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
    return receiver->superScope(this->method->getScope());
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


RexxObject *RexxNativeActivation::getContextStem(RexxString *name)
/******************************************************************************/
/* Function:  retrieve a stem variable stem from the current context.         */
/******************************************************************************/
{
    // if this is not a stem name, add it now
    if (name->getChar(name->getLength() - 1) != '.')
    {
        name = name->concatWithCstring(".");
    }

    RexxVariableBase *retriever = activation->getVariableRetriever(name);
    // if this didn't parse, it's an illegal name
    // it must also resolve to a stem type...this could be a compound one
    if (retriever == OREF_NULL || !isOfClass(StemVariableTerm, retriever))
    {
        return OREF_NULL;
    }
    // get the variable value
    return retriever->getValue(activation);
}


/**
 * Retrieve the value of a context variable via the API calls.
 * Returns OREF_NULL if the variable does not exist.
 *
 * @param name   The variable name.
 *
 * @return The variable value, if any.
 */
RexxObject *RexxNativeActivation::getContextVariable(const char *name)
{
    RexxVariableBase *retriever = activation->getVariableRetriever(new_string(name));
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
        return retriever->getRealValue(activation);
    }
}

/**
 * Set a context variable on behalf of an API call.
 *
 * @param name   The name of the variable.
 * @param value  The variable value.
 */
void RexxNativeActivation::setContextVariable(const char *name, RexxObject *value)
{
    // get the REXX activation for the target context
    RexxVariableBase *retriever = activation->getVariableRetriever(new_string(name));
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL || isString((RexxObject *)retriever))
    {
        return;
    }
    this->resetNext();               // all next operations must be reset

    // do the assignment
    retriever->set(activation, value);
}


/**
 * Drop a context variable for an API call.
 *
 * @param name   The target variable name.
 */
void RexxNativeActivation::dropContextVariable(const char *name)
{
    // get the REXX activation for the target context
    RexxVariableBase *retriever = activation->getVariableRetriever(new_string(name));
    // if this didn't parse, it's an illegal name
    if (retriever == OREF_NULL || isString((RexxObject *)retriever))
    {
        return;
    }
    this->resetNext();               // all next operations must be reset

    // perform the drop
    retriever->drop(activation);
}


RexxSupplier *RexxNativeActivation::getAllContextVariables()
/******************************************************************************/
/* Function:  Retriev a list of all variables in the current context.         */
/******************************************************************************/
{
    return activation->getAllLocalVariables();
}

void RexxNativeActivation::checkConditions()
/******************************************************************************/
/* Function:  check to see if a condition was raised on return from a call out*/
/******************************************************************************/
{
    // if we have a stashed condition object, we need to raise this now
    if (conditionObj != OREF_NULL)
    {
        // we're raising this in the previous stack frame.  If it doesn't exist,
        // se just leave this in place so top-level callers can check this.
        if (activation != OREF_NULL)
        {
            activity->reraiseException(conditionObj);
        }
    }
}


/**
 * Allocate a new native Activation.
 *
 * @param size   the allocation size.
 *
 * @return A pointer to the newly allocated object.
 */
void * RexxNativeActivation::operator new(size_t size)
{
                                       /* Get new object                    */
  RexxObject *newObject = new_object(size, T_NativeActivation);
  newObject->clearObject();            /* clear out at start                */
  return newObject;                    /* return the new object             */
}

REXXOBJECT REXXENTRY REXX_MSGNAME()
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
    return context.protect(context.self->getMessageName());
}


REXXOBJECT REXXENTRY REXX_RECEIVER()
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
    return context.protect(context.self->getReceiver());
}

wholenumber_t REXXENTRY REXX_INTEGER(REXXOBJECT object)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* just forward and return           */
    wholenumber_t result = 0;

    ((RexxObject *)object)->numberValue(result, context.self->digits());
    return result;                       /* return converted value            */
}

size_t REXXENTRY REXX_UNSIGNED_INTEGER(REXXOBJECT object)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
    stringsize_t result = 0;           /* returned result                   */

                                       /* First convert to numberstring     */
    ((RexxObject *)object)->unsignedNumberValue(result, context.self->digits());
    return result;                     /* return converted value            */
}

bool REXXENTRY REXX_ISINTEGER(REXXOBJECT object)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* just forward and return           */
    return context.self->isInteger((RexxObject *)object);
}

bool REXXENTRY REXX_ISSTRING(REXXOBJECT object)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
    return ((RexxObject *)object)->getObjectType() == TheStringBehaviour;
}

CSTRING REXXENTRY REXX_STRING(REXXOBJECT object)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
    return context.self->cstring((RexxObject *)object);
}


bool REXXENTRY REXX_ISINSTANCE(REXXOBJECT object, REXXOBJECT classObject)
/******************************************************************************/
/* Function:  Object ISA test                                                 */
/******************************************************************************/
{
    NativeContextBlock context;
    return ((RexxObject *)object)->isInstanceOf((RexxClass *)classObject);

}


double REXXENTRY REXX_DOUBLE(REXXOBJECT object)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
    return context.self->getDoubleValue((RexxObject *)object);
}

bool REXXENTRY REXX_ISDOUBLE(REXXOBJECT object)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* just forward and return           */
    return context.self->isDouble((RexxObject *)object);
}

REXXOBJECT REXXENTRY REXX_SEND(REXXOBJECT receiver, CSTRING msgname, REXXOBJECT arguments)
/******************************************************************************/
/* Function:  Issue a full scale send_message from native code                */
/******************************************************************************/
{
    NativeContextBlock context;
    return context.protect(((RexxObject *)receiver)->sendMessage((RexxString *)new_string(msgname), (RexxArray *)arguments));
}

REXXOBJECT REXXENTRY REXX_SUPER(CSTRING msgname, REXXOBJECT arguments)
/******************************************************************************/
/* Function:  Forward a message to a super class from native code             */
/******************************************************************************/
{
    NativeContextBlock context;
    RexxObject     *argarray[10];        /* C array of arguments              */
    RexxArray      *args = (RexxArray *)arguments;
    size_t          i;                   /* loop counter                      */

    size_t count = args->size();         /* get the argument count            */
    for (i = 1; i <= count; i++)         /* loop through the array            */
    {
                                         /* copying each OREF                 */
        argarray[i-1] = args->get(i);
    }
    ProtectedObject result;
                                       /* now send the message              */
    context.self->getReceiver()->messageSend((RexxString *)new_string(msgname), count, argarray, context.self->getReceiver()->superScope(context.self->getMethod()->getScope()), result);
                                       /* now send the message              */
    return context.protect((RexxObject *)result);
}

REXXOBJECT REXXENTRY REXX_SETVAR(CSTRING name, REXXOBJECT value)
/******************************************************************************/
/* Function:  Set the value of an object variable                             */
/******************************************************************************/
{
    NativeContextBlock context;
    RexxVariableDictionary *dictionary = context.self->methodVariables();/* get the method variables          */
    RexxString *variableName = new_string(name);    /* get a string version of this      */
                                       /* do the assignment                 */
    dictionary->set(variableName, (RexxObject *)value);
    return OREF_NULL;
}

/*******************************************************1***/
/* return the names of all public routines in the current */
/* activation context. *the caller* must free the memory! */
/**********************************************************/
REXXOBJECT REXXENTRY REXX_GETFUNCTIONNAMES(char *** names, size_t* num)
{
    NativeContextBlock context;
    RexxArray              * funcArray;
    RexxString             * name;
    size_t                   i;
    size_t                   j;

    /* pick up current activation        */

    /* get the current activation        */
    RexxActivation *activation = context.self->getRexxContext();

    *num = 0;
    RexxDirectory *routines = activation->getPublicRoutines();

    if (routines != OREF_NULL)
    {
        funcArray = routines->makeArray();
        if (funcArray != OREF_NULL)
        {
            *num = j = funcArray->numItems();
            *names = (char**) SysAllocateExternalMemory(sizeof(char*)*j);
            for (i=0; i < j; i++)
            {
                name = ((RexxString*) funcArray->get(i+1));
                (*names)[i] = (char*) SysAllocateExternalMemory(1+sizeof(char)*name->getLength());
                memcpy((*names)[i], name->getStringData(), name->getLength());
                (*names)[i][name->getLength()] = 0; // zero-terminate
            }
        }
    }

    return OREF_NULL;              /* return nothing                    */
}

REXXOBJECT REXXENTRY REXX_GETVAR(CSTRING name)
/******************************************************************************/
/* Function:  Retrieve the value of an object variable                        */
/******************************************************************************/
{
    NativeContextBlock context;

    RexxVariableDictionary *dictionary = context.self->methodVariables();/* get the method variables          */
    RexxString *variableName = new_string(name);    /* get a string version of this      */
                                       /* go get the variable               */
    // NB:  We don't call protect because this object is already anchored in the variable pool.
    return dictionary->realValue(variableName);
}

void REXXENTRY REXX_EXCEPT(int errorcode, REXXOBJECT value)
/******************************************************************************/
/* Function:  Raise an exception on behalf of native code                     */
/******************************************************************************/
{
    NativeContextBlock context;
    if (value == OREF_NULL)              /* just a NULL value?                */
    {
        reportException(errorcode);        /* no substitution parameters        */
    }
    else                                 /* use the substitution form         */
    {
        reportException(errorcode, (RexxArray *)value);
    }
}

void REXXENTRY REXX_EXCEPT1(int errorcode, REXXOBJECT value1)
/******************************************************************************/
/* Function:  Raise an exception on behalf of native code                     */
/******************************************************************************/
{
    NativeContextBlock context;
    reportException(errorcode, (RexxObject *)value1);
}

void REXXENTRY REXX_EXCEPT2(int errorcode, REXXOBJECT value1, REXXOBJECT value2)
/******************************************************************************/
/* Function:  Raise an exception on behalf of native code                     */
/******************************************************************************/
{
    NativeContextBlock context;
    reportException(errorcode, (RexxObject *)value1, (RexxObject *)value2);
}


void REXXENTRY REXX_RAISE(CSTRING condition, REXXOBJECT description, REXXOBJECT additional, REXXOBJECT result)
/******************************************************************************/
/* Function:  Raise a condition on behalf of native code                      */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* go raise the condition            */
    context.self->raiseCondition(new_string(condition), (RexxString *)description, (RexxObject *)additional, (RexxObject *)result);
}


REXXOBJECT REXXENTRY REXX_CONDITION(REXXOBJECT condition, REXXOBJECT description, REXXOBJECT additional)
/******************************************************************************/
/* Function:  Raise a condition on behalf of native code                      */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* pass on and raise the condition   */
    return (context.activity->raiseCondition((RexxString *)condition, OREF_NULL, (RexxString *)description, (RexxObject *)additional, OREF_NULL, OREF_NULL)) ? TheTrueObject : TheFalseObject;
}


int REXXENTRY REXX_VARIABLEPOOL(void *pshvblock)
/******************************************************************************/
/* Function:  If variable pool is enabled, return result from SysVariablePool */
/*             method, otherwise return RXSHV_NOAVL.                          */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* if access is enabled              */
    if (context.self->getVpavailable())
    {
                                       /* go process the requests           */
        return SysVariablePool(context.self, pshvblock, true);

    }
    else
    {
                                       /* call VP only allowing shv_exit    */
        return SysVariablePool(context.self, pshvblock, false);
    }
}


void REXXENTRY REXX_PUSH_ENVIRONMENT(REXXOBJECT environment)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* pick up current activation        */
    RexxActivation *activation = (RexxActivation *)context.self->getRexxContext();
    activation->pushEnvironment((RexxObject *)environment);
}


REXXOBJECT REXXENTRY REXX_POP_ENVIRONMENT()
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
    NativeContextBlock context;
                                       /* pick up current activation        */
    RexxActivation *activation = (RexxActivation *)context.self->getRexxContext();
    return context.protect(activation->popEnvironment());
}


bool REXXENTRY REXX_ISDIRECTORY(REXXOBJECT object)
/******************************************************************************/
/* Function:  Validate that an object is a directory                          */
/******************************************************************************/
{
                                       /* do the validation                 */
  return object != NULL && isOfClass(Directory, (RexxObject *)object);
}


REXXOBJECT REXXENTRY REXX_NIL()
/******************************************************************************/
/* Function:  Return REXX .nil object to native code                          */
/******************************************************************************/
{
  return TheNilObject;                 /* just return the object            */
}

REXXOBJECT REXXENTRY REXX_TRUE()
/******************************************************************************/
/* Function:  Return REXX TRUE object to native code                          */
/******************************************************************************/
{
  return TheTrueObject;                /* just return the object            */
}

REXXOBJECT REXXENTRY REXX_FALSE()
/******************************************************************************/
/* Function:  Return REXX FALSE object to native code                         */
/******************************************************************************/
{
  return TheFalseObject;               /* just return the object            */
}

REXXOBJECT REXXENTRY REXX_LOCAL()
/******************************************************************************/
/* Function:  Return REXX .local object to native code                        */
/******************************************************************************/
{
  return ActivityManager::localEnvironment;  /* just return the local environment */
}

REXXOBJECT REXXENTRY REXX_ENVIRONMENT()
/******************************************************************************/
/* Function:  Return REXX .environment object to native code                  */
/******************************************************************************/
{
  return TheEnvironment;               /* just return the object            */
}

int REXXENTRY REXX_STEMSORT(CSTRING stemname, int order, int type, size_t start, size_t end, size_t firstcol, size_t lastcol)
/******************************************************************************/
/* Function:  Perform a sort on stem data.  If everything works correctly,    */
/*             this returns zero, otherwise an appropriate error value.       */
/******************************************************************************/
{
    NativeContextBlock context;
    size_t  position;                    /* scan position within compound name */
    size_t  length;                      /* length of tail section            */

                                         /* if access is enabled              */
    if (!context.self->getVpavailable())
    {       /* access must be enabled for this to work */
        return false;
    }

    // NB:  The braces here are to ensure the ProtectedObjects get released before the
    // currentActivity gets zeroed out.
    {
        /* get the REXX activation */
        RexxActivation *activation = context.self->getRexxContext();

        /* get the stem name as a string */
        RexxString *variable = new_string(stemname);
        ProtectedObject p1(variable);
        /* and get a retriever for this variable */
        RexxStemVariable *retriever = (RexxStemVariable *)activation->getVariableRetriever(variable);

        /* this must be a stem variable in order for the sorting to work. */

        if ( (!isOfClass(StemVariableTerm, retriever)) && (!isOfClass(CompoundVariableTerm, retriever)) )
        {
            return false;
        }

        RexxString *tail = OREF_NULLSTRING ;
        ProtectedObject p2(tail);

        if (isOfClass(CompoundVariableTerm, retriever))
        {
            length = variable->getLength();      /* get the string length             */
            position = 0;                        /* start scanning at first character */
            /* scan to the first period          */
            while (variable->getChar(position) != '.')
            {
                position++;                        /* step to the next character        */
                length--;                          /* reduce the length also            */
            }
            position++;                          /* step past previous period         */
            length--;                            /* adjust the length                 */
            tail = variable->extract(position, length);
            tail = tail->upper();
        }

        return retriever->sort(activation, tail, order, type, start, end, firstcol, lastcol);
    }
}

void REXXENTRY REXX_GUARD_ON()
/******************************************************************************/
/* Function:  External interface to implement method guarding                 */
/******************************************************************************/
{
    NativeContextBlock context;
    context.self->guardOn();             /* turn on the guard                 */
}

void REXXENTRY REXX_GUARD_OFF()
/******************************************************************************/
/* Function:  External interface to implement method guarding                 */
/******************************************************************************/
{
    NativeContextBlock context;
    context.self->guardOff();            /* turn off the guard                 */
}
