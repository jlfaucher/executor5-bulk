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
/* Object REXX Kernel                                           ObjectClass.c    */
/*                                                                            */
/* The main REXX object definitions                                           */
/*                                                                            */
/******************************************************************************/
#include <ctype.h>
#include <string.h>
#include "RexxCore.h"
#include "ObjectClass.hpp"
#include "StringClass.hpp"
#include "RexxBuffer.hpp"
#include "RexxSmartBuffer.hpp"
#include "DirectoryClass.hpp"
#include "RexxVariableDictionary.hpp"
#include "ArrayClass.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include "MessageClass.hpp"
#include "MethodClass.hpp"
#include "ExpressionBaseVariable.hpp"
#include "SourceFile.hpp"
#include "Interpreter.hpp"

RexxObject *RexxObject::nilObject = OREF_NULL;      // the nil object singleton
RexxClass  *RexxObject::classInstance = OREF_NULL;  // singleton class instance

void RexxObject::createInstance(RootImage *rootImage)
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
    // create the .nil singleto object
    TheNilObject = new (TheObjectClass) RexxObject;
                                         /* We don't move the NIL object, we  */
                                         /*will use the remote systems NIL    */
                                         /*object.                            */
    TheNilObject->header.makeProxiedObject();
    rootImage->nilObject = TheNilObject; // make sure this is anchored in the global object
}


void RexxObject::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->objectVariables);
  cleanUpMemoryMark
}

void RexxObject::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->objectVariables);
  cleanUpMemoryMarkGeneral
}

void RexxObject::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxObject)

  flatten_reference(newThis->objectVariables, envelope);

  cleanUpFlatten
}

RexxObject * RexxInternalObject::makeProxy(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Create a proxy object for a "special" REXX object               */
/******************************************************************************/
{
    // see if there is a registered proxy for this object...
    RexxString *proxy = RexxMemory::memoryObject.getObjectProxy((RexxObject *)this);
    if (proxy != OREF_NULL)
    {
        return proxy;
    }
    // if not registered, the proxy value is itself
    return (RexxObject *)this;
}


/**
 * Test if an internal object is an instance of another class.
 *
 * @param other  The test class.
 *
 * @return Always returns false.
 */
bool RexxInternalObject::isInstanceOf(RexxClass *other)
{
    // internal classes always fail this
    return false;
}


/**
 * Test if a Rexx object is an instance of a given class.
 *
 * @param other  The other test class.
 *
 * @return True if this object is an instance of the target class, false otherwise.
 */
bool RexxObject::isInstanceOf(RexxClass *other)
{
    return classObject()->isCompatibleWith(other);
}


/**
 * The Rexx external version of the instance of.
 *
 * @param other  The other test class.
 *
 * @return .true if this class is an instance of the target class. .false
 *         otherwise.
 */
RexxObject *RexxObject::isInstanceOfRexx(RexxClass *other)
{
    required_arg(other, ONE);
    return isInstanceOf(other) ? TheTrueObject : TheFalseObject;
}

bool RexxInternalObject::isEqual(
    RexxObject *other )                /* other object for comparison       */
/******************************************************************************/
/* Function:  primitive level equality method used by the hash collection     */
/*            classes for determining equality.                               */
/******************************************************************************/
{
  return ((RexxObject *)this) == other;/* simple identity equality          */
}

bool RexxObject::isEqual(
    RexxObject *other )                /* other object for comparison       */
/******************************************************************************/
/* Function:  primitive level equality method used by the hash collection     */
/*            classes for determining equality.                               */
/******************************************************************************/
{
  if (isPrimitive(this))               /* primitive object?                 */
                                       /* simple identity equality          */
    return ((RexxObject *)this) == other;
  else                                 /* return truth value of a compare   */
    return this->sendMessage(OREF_STRICT_EQUAL, other)->truthValue(Error_Logical_value_method);
}

RexxObject * RexxObject::strictEqual(
    RexxObject * other)                /* other object for comparison       */
/******************************************************************************/
/* Function:  Process the default "==" strict comparison operator             */
/******************************************************************************/
{
  HashCode hash;                       /* retrieved hash                    */

  if (other == OREF_NULL) {            /* asking for the hash value?        */
    hash = this->getHashValue();       /* get the hash value                */
                                       /* create a string value             */
    return (RexxObject *)new_string((stringchar_t *)&hash, sizeof(HashCode));
  }
  else
                                       /* this is direct object equality    */
    return (RexxObject *)((this == other)? TheTrueObject: TheFalseObject);
}

RexxObject * RexxObject::equal(RexxObject * other)
/******************************************************************************/
/* Function:  Normal "=" type comparison.  This only returns true if the      */
/*            two objects are the same object                                 */
/******************************************************************************/
{
  requiredArg(other, ONE);            /* must have the other argument      */
                                       /* this is direct object equality    */
  return (RexxObject *)((this == other)? TheTrueObject: TheFalseObject);
}

RexxInteger *RexxObject::strictNotEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Return the strict inequality of two objects                     */
/******************************************************************************/
{
   requiredArg(other, ONE);           /* first argument is required        */
   return this != other ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxObject::notEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Return the inequality of two objects                            */
/******************************************************************************/
{
   requiredArg(other, ONE);           /* first argument is required        */
   return this != other ? TheTrueObject : TheFalseObject;
}


/**
 * Test if the object is a value logical value as defined
 * for IF, WHEN, etc.
 *
 * @return Always returns false for internal objects.
 */
bool RexxInternalObject::isLogical()
{
    return false;
}


/**
 * Test if the object is a value logical value as defined
 * for IF, WHEN, etc.
 *
 * @return Returns of the test on the string value of the object.
 */
bool RexxObject::isLogical()
{
    return REQUEST_STRING(this)->isLogical();
}

bool RexxInternalObject::truthValue(
    wholenumber_t   errorCode )        /* error to issue for bad conversion */
/******************************************************************************/
/* Function:  test the truth value of a primitive object                      */
/******************************************************************************/
{
                                       /* report the error                  */
  reportException(errorCode, OREF_NULLSTRING);
  return false;                        /* need a return value               */
}

bool RexxObject::truthValue(
    wholenumber_t  errorCode )         /* error to issue for bad conversion */
/******************************************************************************/
/* Function:  test the truth value of a primitive object                      */
/******************************************************************************/
{
                                       /* just return string truth value    */
   return REQUEST_STRING(this)->truthValue(errorCode);
}

RexxObject * RexxInternalObject::copy()
/******************************************************************************/
/* Function:  First level primitive copy of an object.  This just copies      */
/*            the object storage, and nothing else.                           */
/******************************************************************************/
{
  /* Instead of calling new_object and memcpy, ask the memory object to make  */
  /* a copy of ourself.  This way, any header information can be correctly    */
  /* initialized by memory.                                                   */
  return (RexxObject *)RexxMemory::memoryObject.clone((RexxObject *)this);
}

void *RexxInternalObject::operator new(size_t size,
    RexxClass *classObject)            /* class of the object               */
/******************************************************************************/
/* Function:  Create a new primitive object                                   */
/******************************************************************************/
{
    return new_object(size, classObject->instanceBehaviour);
}

void *RexxInternalObject::operator new(size_t size,
    RexxClass * classObject,           /* class of the object               */
    RexxObject **arguments,            /* arguments to the new method       */
    size_t       argCount)             /* the count of arguments            */
/******************************************************************************/
/* Function:  Create a new instance of object (with arguments)                */
/******************************************************************************/
{
    return new_object(size, classObject->instanceBehaviour);
}

RexxObject * RexxObject::copy()
/******************************************************************************/
/* Function:  Copy an object that has an Object Variable Dictionary (OVD)     */
/******************************************************************************/
{
  /* Instead of calling new_object and memcpy, ask the memory object to make  */
  /* a copy of ourself.  This way, any header information can be correctly    */
  /* initialized by memory.                                                   */
  RexxObject       *newObject;         /* copied object                     */

                                       /* first copy the object             */
  newObject = (RexxObject *)RexxMemory::memoryObject.clone(this);
  ProtectedObject p1(newObject);
                                       /* have object variables?            */
  if (this->objectVariables != OREF_NULL) {
    copyObjectVariables(newObject);    /* copy the object variables into the new object */
  }
                                       /* have instance methods?            */
  if (this->behaviour->instanceMethodDictionary != OREF_NULL)
                                       /* need to copy the behaviour        */
    BehaviourSet(newObject, (RexxBehaviour *)newObject->behaviour->copy());
  return newObject;                    /* return the copied version         */
}

void RexxObject::copyObjectVariables(RexxObject *newObject)
/******************************************************************************/
/* Function:  Copy an object's object variable dictionaries into another obj. */
/******************************************************************************/
{
    RexxVariableDictionary *dictionary = objectVariables;
    /* clear out the existing object variable pointer */
    newObject->objectVariables = OREF_NULL;

    while (dictionary != OREF_NULL) {
        /* copy the dictionary */
        RexxVariableDictionary *newDictionary = (RexxVariableDictionary *)dictionary->copy();
        /* add this to the variable set */
        newObject->addObjectVariables(newDictionary);
        /* now that the dictionary is anchored in the new object, */
        /* copy the variable objects inside. */
        newDictionary->copyValues();
        /* and repeat for each one */
        dictionary = dictionary->getNextDictionary();
    }
}

RexxMethod * RexxObject::checkPrivate(
    RexxActivity     * activity,       // the activity context
    RexxMethod       * method )        /* method to check                   */
/******************************************************************************/
/* Function:  Check a private method for accessibility.                       */
/******************************************************************************/
{
                                       /* get the top activation            */
  RexxActivation *activation = activity->getCurrentContext();
  if (activation == OREF_NULL)
  {
      return OREF_NULL;
  }
  if (activation->getReceiver() == this)  /* the same receiver?                */
  {
      return method;                   /* just return the same method       */
  }
  return OREF_NULL;                    /* return a failure indicator        */
}

/******************************************************************************/
/* Function:  Stub methods for issuing messages without an activity.          */
/******************************************************************************/
RexxObject  *RexxObject::sendMessage(RexxString *m, RexxArray *a)
{
    return this->sendMessage(contextActivity(), m, a);
}

RexxObject  *RexxObject::sendMessage(RexxString *m)
{
    return this->sendMessage(contextActivity(), m);
}

RexxObject  *RexxObject::sendMessage(RexxString *m, RexxObject **a, size_t c)
{
    return this->sendMessage(contextActivity(), m, a, c);
}

RexxObject  *RexxObject::sendMessage(RexxString *m, RexxObject *a1)
{
    return this->sendMessage(contextActivity(), m, a1);
}

RexxObject  *RexxObject::sendMessage(RexxString *m, RexxObject *a1, RexxObject *a2)
{
    return this->sendMessage(contextActivity(), m, a1, a2);
}

RexxObject  *RexxObject::sendMessage(RexxString *m, RexxObject *a1, RexxObject *a2, RexxObject *a3)
{
    return this->sendMessage(contextActivity(), m, a1, a2, a3);
}

RexxObject  *RexxObject::sendMessage(RexxString *m, RexxObject *a1, RexxObject *a2, RexxObject *a3, RexxObject *a4)
{
    return this->sendMessage(contextActivity(), m, a1, a2, a3, a4);
}

RexxObject  *RexxObject::sendMessage(RexxString *m, RexxObject *a1, RexxObject *a2, RexxObject *a3, RexxObject *a4, RexxObject *a5)
{
    return this->sendMessage(contextActivity(), m, a1, a2, a3, a4, a5);
}

RexxObject * RexxObject::sendMessage(
    RexxActivity     * activity,       // the activity context
    RexxString      *message,          /* name of the message to process    */
    RexxArray  *arguments )            /* array of arguments                */
{
    ProtectedObject result;
    messageSend(activity, message, arguments->size(), arguments->data(), result);
    return (RexxObject *)result;
}

RexxObject *RexxObject::sendMessage(RexxActivity *a, RexxString *message)
{
    ProtectedObject result;
    messageSend(a, message, 0, OREF_NULL, result);
    return (RexxObject *)result;
}

RexxObject *RexxObject::sendMessage(RexxActivity *a, RexxString *message, RexxObject **args, size_t argCount)
{
    ProtectedObject result;
    this->messageSend(a, message, argCount, args, result);
    return (RexxObject *)result;
}

RexxObject *RexxObject::sendMessage(RexxActivity *a, RexxString *message, RexxObject *argument1)
{
    ProtectedObject result;
    this->messageSend(a, message, 1, &argument1, result);
    return (RexxObject *)result;
}

RexxObject *RexxObject::sendMessage(
    RexxActivity     * activity,       // the activity context
    RexxString *message,               /* name of the message to process    */
    RexxObject *argument1,             /* first argument                    */
    RexxObject *argument2 )            /* second argument                   */
/******************************************************************************/
/* Function:  Send a message with two arguments                               */
/******************************************************************************/
{
    ProtectedObject result;
    RexxObject *arguments[2];            /* argument array                    */

    arguments[0] = argument1;            /* set each argument                 */
    arguments[1] = argument2;
                                         /* just pass on to message send      */
    messageSend(activity, message, 2, arguments, result);
    return (RexxObject *)result;
}

RexxObject * RexxObject::sendMessage(
    RexxActivity     * activity,       // the activity context
    RexxString *message,               /* name of the message to process    */
    RexxObject *argument1,             /* first argument                    */
    RexxObject *argument2,             /* second argument                   */
    RexxObject *argument3 )            /* third argument                    */
/******************************************************************************/
/* Function:  Send a message with three arguments                             */
/******************************************************************************/
{
    ProtectedObject result;
    RexxObject *arguments[3];            /* argument array                    */

    arguments[0] = argument1;            /* set each argument                 */
    arguments[1] = argument2;
    arguments[2] = argument3;
                                         /* just pass on to message send      */
    messageSend(activity, message, 3, arguments, result);
    return (RexxObject *)result;
}

RexxObject * RexxObject::sendMessage(
    RexxActivity     * activity,       // the activity context
    RexxString *message,               /* name of the message to process    */
    RexxObject *argument1,             /* first argument                    */
    RexxObject *argument2,             /* second argument                   */
    RexxObject *argument3,             /* third argument                    */
    RexxObject *argument4 )            /* fourth argument                   */
/******************************************************************************/
/* Function:  Send a message with four arguments                              */
/******************************************************************************/
{
    ProtectedObject result;
    RexxObject *arguments[4];            /* argument array                    */

    arguments[0] = argument1;            /* set each argument                 */
    arguments[1] = argument2;
    arguments[2] = argument3;
    arguments[3] = argument4;
                                         /* just pass on to message send      */
    messageSend(activity, message, 4, arguments, result);
    return (RexxObject *)result;
}

RexxObject * RexxObject::sendMessage(
    RexxActivity     * activity,       // the activity context
    RexxString *message,               /* name of the message to process    */
    RexxObject *argument1,             /* first argument                    */
    RexxObject *argument2,             /* second argument                   */
    RexxObject *argument3,             /* third argument                    */
    RexxObject *argument4,             /* fourth argument                   */
    RexxObject *argument5 )            /* fifth argument                    */
/******************************************************************************/
/* Function:  Send a message with five arguments                              */
/******************************************************************************/
{
    ProtectedObject result;
    RexxObject *arguments[5];            /* argument array                    */

    arguments[0] = argument1;            /* set each argument                 */
    arguments[1] = argument2;
    arguments[2] = argument3;
    arguments[3] = argument4;
    arguments[4] = argument5;
                                         /* just pass on to message send      */
    messageSend(activity, message, 5, arguments, result);
    return (RexxObject *)result;
}

void RexxObject::messageSend(
    RexxActivity     * activity,       // the activity context
    RexxString      *msgname,          /* name of the message to process    */
    size_t           count,            /* count of arguments                */
    RexxObject     **arguments,        /* array of arguments                */
    ProtectedObject &result)           // returned result
/******************************************************************************/
/* Function:    send a message (with message lookup) to an object.            */
/*              All types of methods are handled and dispatched               */
/******************************************************************************/
{
  activity->setLastMessageName(msgname);  /* have enough stack space?          */
  activity->stackSpace();              /* have enough stack space?          */
                                       /* grab the method from this level   */
  RexxMethod *method_save = this->behaviour->methodLookup(msgname);
                                       /* method exists...special processing*/
  if (method_save != OREF_NULL && method_save->isSpecial())
  {
    if (method_save->isPrivate())      /* actually private method?          */
    {
                                           /* go validate a private method      */
        method_save = this->checkPrivate(activity, method_save);
    }
                                       /* now process protected methods     */
    if (method_save != OREF_NULL && method_save->isProtected())
    {
                                       // save this method in the activity
        activity->setLastMethod(method_save);

                                       /* really a protected method         */
        this->processProtectedMethod(activity, msgname, count, arguments, result);
        return;
    }
  }
                                       /* have a method                     */
  if (method_save != OREF_NULL) {
      activity->setLastMethod(method_save);
                                       /* run the method                    */
      method_save->run(activity, this, msgname, count, arguments, result);
      return;
  }
                                       /* go process an unknown method      */
  this->processUnknown(activity, msgname, count, arguments, result);
}

void RexxObject::messageSend(
    RexxActivity    *activity,         // activiy we're running on
    RexxString      *msgname,          /* name of the message to process    */
    size_t           count,            /* count of arguments                */
    RexxObject     **arguments,        /* array of arguments                */
    RexxObject      *startscope,       /* starting superclass scope         */
    ProtectedObject &result)           // the returned result
/******************************************************************************/
/* Function:    send a message (with message lookup) to an object.            */
/*              All types of methods are handled and dispatched               */
/******************************************************************************/
{
  activity->setLastMessageName(msgname);  /* have enough stack space?          */
  activity->stackSpace();       /* have enough stack space?          */
                                       /* go to the higher level            */
  RexxMethod *method_save = this->superMethod(msgname, startscope);
  if (method_save != OREF_NULL && method_save->isProtected()) {
    if (method_save->isPrivate())      /* actually private method?          */
                                       /* go validate a private method      */
      method_save = this->checkPrivate(activity, method_save);
    else                               /* really a protected method         */
    {
                                       // save this method in the activity
        activity->setLastMethod(method_save);
        this->processProtectedMethod(activity, msgname, count, arguments, result);
        return;
    }
  }
                                       /* have a method                     */
  if (method_save != OREF_NULL)
  {
                                       // save this method in the activity
      activity->setLastMethod(method_save);
                                         /* run the method                    */
      method_save->run(activity, this, msgname, count, arguments, result);
      return;
  }
                                       /* go process an unknown method      */
  this->processUnknown(activity, msgname, count, arguments, result);
}

void RexxObject::processProtectedMethod(
    RexxActivity * activity,
    RexxString   * messageName,        /* message to issue                  */
    size_t         count,              /* count of arguments                */
    RexxObject  ** arguments,          /* actual message arguments          */
    ProtectedObject &result)           // the returned result
/******************************************************************************/
/* Function:  Process an unknown message, uncluding looking for an UNKNOWN    */
/*            method and raising a NOMETHOD condition                         */
/******************************************************************************/
{
    RexxArray          *argumentArray;   /* unknown method argument array     */
    RexxDirectory      *securityArgs;    /* security arguments                */
    RexxMethod         *thisMethod;      /* saved method                      */
    RexxActivationBase *activation;      /* current activation                */

    /* get the top activation            */
    activation = activity->getCurrentContext();
    /* have an activation?               */
    if (activation != OREF_NULL)
    {
        /* have a security manager?          */
        if (activation->hasSecurityManager())
        {
            // get the current method
            thisMethod = activity->getLastMethod();
            securityArgs = new_directory();  /* get the security args             */
                                             /* stuff in the name                 */
            securityArgs->put(messageName, OREF_NAME);
            /* add in the actual object          */
            securityArgs->put(this, OREF_OBJECTSYM);
            /* get an array for the arguments */
            argumentArray = new (count, arguments) RexxArray;
            /* add in the arguments              */
            securityArgs->put(argumentArray, OREF_ARGUMENTS);
            /* now go ask permission             */
            if (((RexxActivation *)activation)->callSecurityManager(OREF_METHODNAME, securityArgs))
            {
                /* handled, just return the result   */
                result = securityArgs->fastAt(OREF_RESULT);
                return;
            }
            // make sure we save the message information
            activity->setLastMethod(messageName, thisMethod);
        }
    }
    /* run the method                    */
    activity->getLastMethod()->run(activity, this, messageName, count, arguments, result);
}

void RexxObject::processUnknown(
    RexxActivity * activity,           // the execution activity
    RexxString   * messageName,        /* message to issue                  */
    size_t         count,              /* count of arguments                */
    RexxObject  ** arguments,          /* actual message arguments          */
    ProtectedObject &result)           // returned result
/******************************************************************************/
/* Function:  Process an unknown message, uncluding looking for an UNKNOWN    */
/*            method and raising a NOMETHOD condition                         */
/******************************************************************************/
{
  RexxObject     *unknown_arguments[2];/* arguments to the unknown method   */
  size_t          i;                   /* loop counter                      */
  RexxArray      *argumentArray;       /* unknown method argument array     */

                                       /* no method for this msgname        */
                                       /* find the unknown method           */
  RexxMethod *method_save = this->behaviour->methodLookup(OREF_UNKNOWN);
  if (method_save == OREF_NULL)        /* "unknown" method exists?          */
                                       /* no unknown method - try to raise  */
                                       /* a NOMETHOD condition, and if that */
      activity->reportNoMethod(messageName, this);
  argumentArray = new_array(count);    /* get an array for the arguments    */
  for (i = 1; i <= count; i++)         /* copy the arguments into an array  */
    argumentArray->put(arguments[i - 1], i);
  unknown_arguments[0] = messageName;  /* method name is first argument     */
                                       /* second argument is array of       */
  unknown_arguments[1] = argumentArray;/* arguments for the original call   */
                                       /* run the unknown method            */
  activity->setLastMethod(method_save);
  method_save->run(activity, this, OREF_UNKNOWN, 2, unknown_arguments, result);
}

RexxMethod * RexxObject::methodLookup(
    RexxString *msgname)               /* name of the target message        */
/******************************************************************************/
/* Function:  Return the method object associated with a message name         */
/******************************************************************************/
{
  return this->behaviour->methodLookup(msgname);
}

RexxMethod * RexxObject::methodObject(
    RexxString *msgname)               /* name of the target message        */
/******************************************************************************/
/* Function:  Return the method object associated with a message name         */
/******************************************************************************/
{
  RexxObject *startScope;              /* message target scope              */
  RexxString *message;                 /* string message name               */
  RexxArray  *arrayMessage;            /* message as an array version       */

  requiredArg(msgname, ONE);          /* make sure we have the requireds   */

                                       /* if start scope passed, do a       */
                                       /* superclass search                 */
  arrayMessage = msgname->requestArray();
                                       /* convert ok?                       */
  if (arrayMessage != TheNilObject && arrayMessage->getDimension() == 1) {
    startScope = arrayMessage->get(2); /* get the scope                     */
                                       /* and the message name              */
    message = (RexxString *)arrayMessage->get(1);
                                       /* and do the lookup                 */
    return this->superMethod(message, startScope);
  }
  else {
                                       /* just do the simple method lookup  */
                                       /* behaviour methodObject does all   */
                                       /* needed error checks               */
    return this->behaviour->methodLookup(msgname);
  }
}

bool RexxInternalObject::unsignedNumberValue(stringsize_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert a primitive internal object to a long value             */
/******************************************************************************/
{
  return false;                        /* give a "safe" default here        */
}

bool RexxInternalObject::unsignedNumberValue(stringsize_t *result)
/******************************************************************************/
/* Function:  Convert a primitive internal object to a long value             */
/******************************************************************************/
{
  return false;                        /* give a "safe" default here        */
}

bool RexxInternalObject::numberValue(wholenumber_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert a primitive internal object to a long value             */
/******************************************************************************/
{
  return false;                        /* give a "safe" default here        */
}

bool RexxInternalObject::numberValue(wholenumber_t *result)
/******************************************************************************/
/* Function:  Convert a primitive internal object to a long value             */
/******************************************************************************/
{
  return false;                        /* give a "safe" default here        */
}

RexxInteger * RexxInternalObject::integerValue(
    stringsize_t precision)                  /* precision to use                  */
/******************************************************************************/
/* Function:  Convert a primitive internal object to an integer value         */
/******************************************************************************/
{
  return (RexxInteger *)TheNilObject;  /* give a "safe" default here        */
}

bool RexxInternalObject::doubleValue(double *result)
/******************************************************************************/
/* Function:  Convert a primitive internal object to a double value           */
/******************************************************************************/
{
  return false;                        /* give a "safe" default here        */
}

RexxNumberString * RexxInternalObject::numberString()
/******************************************************************************/
/* Function:  convert an internal object to a numberstring representation     */
/******************************************************************************/
{
  return OREF_NULL;                    /* this never converts               */
}

bool RexxObject::numberValue(wholenumber_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert a REXX object to a long value                           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return REQUEST_STRING(this)->numberValue(result, digits);
}

bool RexxObject::numberValue(wholenumber_t *result)
/******************************************************************************/
/* Function:  Convert a REXX object to a long value                           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return REQUEST_STRING(this)->numberValue(result);
}

bool RexxObject::unsignedNumberValue(stringsize_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert a REXX object to a long value                           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return REQUEST_STRING(this)->unsignedNumberValue(result, digits);
}

bool RexxObject::unsignedNumberValue(stringsize_t *result)
/******************************************************************************/
/* Function:  Convert a REXX object to a long value                           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return REQUEST_STRING(this)->unsignedNumberValue(result);
}

bool RexxObject::numberValueNoNOSTRING(
    wholenumber_t *result, stringsize_t precision)
/******************************************************************************/
/* Function:  Convert a REXX object to a long value                           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return this->requestStringNoNOSTRING()->numberValue(result, precision);
}

bool RexxObject::numberValueNoNOSTRING(
    wholenumber_t *result)
/******************************************************************************/
/* Function:  Convert a REXX object to a long value                           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return this->requestStringNoNOSTRING()->numberValue(result);
}

RexxInteger * RexxObject::integerValue(
    stringsize_t precision)                  /* precision to use                  */
/******************************************************************************/
/* Function:  Convert a primitive internal object to an integer value         */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return REQUEST_STRING(this)->integerValue(precision);
}

bool RexxObject::doubleValue(double *result)
/******************************************************************************/
/* Function:  Convert a primitive internal object to a double value           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return this->requestString()->doubleValue(result);
}

bool RexxObject::doubleValueNoNOSTRING(double *result)
/******************************************************************************/
/* Function:  Convert a primitive internal object to a double value           */
/******************************************************************************/
{
                                       /* get a string and convert          */
  return this->requestStringNoNOSTRING()->doubleValue(result);
}

RexxNumberString * RexxObject::numberString()
/******************************************************************************/
/* Function:  convert a standard object to a numberstring representation      */
/******************************************************************************/
{
                                       /* get the string representation,    */
                                       /* return the numberstring form      */
  return this->requestString()->numberString();
}

RexxString *RexxInternalObject::stringValue()
/******************************************************************************/
/* Function:  Convert a primitive internal object to a string value           */
/******************************************************************************/
{
  return OREF_NULLSTRING;              /* give a "safe" default here        */
}

RexxString *RexxObject::stringValue()
/******************************************************************************/
/* Function:  Convert a primitive object to a string value                    */
/******************************************************************************/
{
                                       /* issue the object name message     */
  return (RexxString *)this->sendMessage(OREF_OBJECTNAME);
}

RexxString *RexxInternalObject::makeString()
/******************************************************************************/
/* Function:  Handle a string conversion REQUEST for an internal object       */
/******************************************************************************/
{
  return (RexxString *)TheNilObject;   /* should never occur                */
}


void RexxInternalObject::copyIntoTail(RexxCompoundTail *tail)
/******************************************************************************/
/* Function:  Handle a tail construction request for an internal object       */
/******************************************************************************/
{
  return;                              /* should never occur                */
}

RexxString *RexxInternalObject::primitiveMakeString()
/******************************************************************************/
/* Function:  Handle a string conversion REQUEST for an internal object       */
/******************************************************************************/
{
  return (RexxString *)TheNilObject;   /* should never occur                */
}

RexxString *RexxObject::makeString()
/******************************************************************************/
/* Function:  Handle a string conversion REQUEST for a REXX object            */
/******************************************************************************/
{
  if (isPrimitive(this))               /* primitive object?                 */
    return (RexxString *)TheNilObject; /* this never converts               */
  else                                 /* process as a string request       */
    return (RexxString *)this->sendMessage(OREF_REQUEST, OREF_STRINGSYM);
}


void RexxObject::copyIntoTail(RexxCompoundTail *tail)
/******************************************************************************/
/* Function:  Handle a tail construction request for an internal object       */
/******************************************************************************/
{
                                       /* get our string value              */
    RexxString *value = REQUEST_STRING(this);
    value->copyIntoTail(tail);         /* pass this on to the string value  */
}


RexxString *RexxObject::primitiveMakeString()
/******************************************************************************/
/* Function:  Handle a string conversion REQUEST for a REXX object            */
/******************************************************************************/
{
  return (RexxString *)TheNilObject;   /* this never converts               */
}

RexxArray *RexxInternalObject::makeArray()
/******************************************************************************/
/* Function:  Handle an array conversion REQUEST for an internal object       */
/******************************************************************************/
{
  return (RexxArray *)TheNilObject;    /* should never occur                */
}

RexxArray *RexxObject::makeArray()
/******************************************************************************/
/* Function:  Handle a string conversion REQUEST for a REXX object            */
/******************************************************************************/
{
  if (isPrimitive(this))               /* primitive object?                 */
    return (RexxArray *)TheNilObject;  /* this never converts               */
  else                                 /* process as a string request       */
    return (RexxArray *)this->sendMessage(OREF_REQUEST, OREF_ARRAYSYM);
}

RexxString *RexxObject::requestString()
/******************************************************************************/
/* Function:  Handle a string request for a REXX object.  This will go        */
/*            through the whole search order to do the conversion.            */
/******************************************************************************/
{
  RexxString *string_value;            /* converted object                  */

                                       /* primitive object?                 */
  if (isPrimitive((RexxObject *)this)) {
                                       /* get the string representation     */
    string_value = this->primitiveMakeString();
    if (string_value == TheNilObject) {/* didn't convert?                   */
                                       /* get the final string value        */
      string_value = this->stringValue();
                                       /* raise a NOSTRING condition        */
      RexxActivityManager::currentActivity->raiseCondition(OREF_NOSTRING, OREF_NULL, string_value, (RexxObject *)this, OREF_NULL, OREF_NULL);
    }
  }
  else {                               /* do a real request for this        */
    string_value = (RexxString *)this->sendMessage(OREF_REQUEST, OREF_STRINGSYM);
    if (string_value == TheNilObject) {/* didn't convert?                   */
                                       /* get the final string value        */
      string_value = (RexxString *)this->sendMessage(OREF_STRINGSYM);
                                       /* raise a NOSTRING condition        */
      RexxActivityManager::currentActivity->raiseCondition(OREF_NOSTRING, OREF_NULL, string_value, this, OREF_NULL, OREF_NULL);
    }
  }
  return string_value;                 /* return the converted form         */
}

RexxString *RexxObject::requestStringNoNOSTRING()
/******************************************************************************/
/* Function:  Handle a string request for a REXX object.  This will go        */
/*            through the whole search order to do the conversion.            */
/******************************************************************************/
{
  RexxString *string_value;            /* converted object                  */

                                       /* primitive object?                 */
  if (isPrimitive((RexxObject *)this)) {
                                       /* get the string representation     */
    string_value = this->primitiveMakeString();
    if (string_value == TheNilObject) {/* didn't convert?                   */
                                       /* get the final string value        */
      string_value = this->stringValue();
    }
  }
  else {                               /* do a real request for this        */
    string_value = (RexxString *)this->sendMessage(OREF_REQUEST, OREF_STRINGSYM);
    if (string_value == TheNilObject) {/* didn't convert?                   */
                                       /* get the final string value        */
      string_value = (RexxString *)this->sendMessage(OREF_STRINGSYM);
    }
  }
  return string_value;                 /* return the converted form         */
}

RexxString *RexxObject::requiredString(
    size_t position)                   /* required argument position        */
/******************************************************************************/
/* Function:  Handle a string request for a REXX object in a context where    */
/*            the object MUST have a string value.                            */
/******************************************************************************/
{
  RexxString *string_value;            /* converted object                  */

  if (isPrimitive(this))               /* primitive object?                 */
    string_value = this->makeString(); /* get the string representation     */
  else                                 /* do a full request for this        */
    string_value = (RexxString *)this->sendMessage(OREF_REQUEST, OREF_STRINGSYM);
                                       /* didn't convert?                   */
  if (string_value == (RexxString *)TheNilObject)
                                       /* this is an error                  */
    reportException(Error_Incorrect_method_nostring, position);
  return string_value;                 /* return the converted form         */
}

RexxInteger *RexxObject::requestInteger(
    stringsize_t precision )           /* precision to use                  */
/******************************************************************************/
/* Function:  Request an integer value from an object.  If this is not a      */
/*            primitive object, the object will be converted to a string,     */
/*            and then the string integer value will be returned.             */
/******************************************************************************/
{
  if (isPrimitive(this))               /* primitive object?                 */
                                       /* return the integer value          */
    return this->integerValue(precision);
  else                                 /* return integer value of string    */
    return this->requestString()->integerValue(precision);
}

RexxInteger *RexxObject::requiredInteger(
    size_t position,                   /* precision to use                  */
    stringsize_t precision)                  /* argument position for errors      */
/******************************************************************************/
/* Function:  Request an integer value from an object.  If this is not a      */
/*            primitive object, a REQUEST('STRING') will be performeding,     */
/*            and then the string integer value will be returned.             */
/******************************************************************************/
{
  RexxInteger *result;                 /* returned result                   */

  if (isPrimitive(this))               /* primitive object?                 */
                                       /* return the integer value          */
    result = this->integerValue(precision);
  else                                 /* return integer value of string    */
    result = this->requiredString(position)->integerValue(precision);
                                       /* didn't convert well?              */
  if (result == (RexxInteger *)TheNilObject)
                                       /* raise an error                    */
    reportException(Error_Incorrect_method_whole, position, this);
  return result;                       /* return the new integer            */
}

bool RexxObject::requestNumber(wholenumber_t *result, stringsize_t precision)
/******************************************************************************/
/* Function:  Request a long value from an object.  If this is not a          */
/*            primitive object, the object will be converted to a string,     */
/*            and then the string long value will be returned.                */
/******************************************************************************/
{
  if (isPrimitive(this))               /* primitive object?                 */
    return this->numberValue(result, precision); /* return the long value             */
  else                                 /* return integer value of string    */
    return this->requestString()->numberValue(result, precision);
}

bool RexxObject::requestNumber(wholenumber_t *result)
/******************************************************************************/
/* Function:  Request a long value from an object.  If this is not a          */
/*            primitive object, the object will be converted to a string,     */
/*            and then the string long value will be returned.                */
/******************************************************************************/
{
  if (isPrimitive(this))               /* primitive object?                 */
    return this->numberValue(result); /* return the long value             */
  else                                 /* return integer value of string    */
    return this->requestString()->numberValue(result);
}

wholenumber_t RexxObject::requiredNumber(
    size_t  position ,                       /* precision to use                  */
    stringsize_t precision)                  /* argument position for errors      */
/******************************************************************************/
/* Function:  Request a long value from an object.  If this is not a          */
/*            primitive object, a REQUEST('STRING') will be performed         */
/*            and then the string long value will be returned.                */
/******************************************************************************/
{
  wholenumber_t  value;               /* returned result                   */
  bool result;

                                       /* primitive object?                 */
  if (isPrimitive(this) && !isOfClass(Object, this))
                                       /* return the integer value          */
    result = this->numberValue(&value, precision);
  else                                 /* return integer value of string    */
    result = this->requiredString(position)->numberValue(&value, precision);
  if (!result)                         /* didn't convert well?              */
                                       /* raise an error                    */
    reportException(Error_Incorrect_method_whole, position, this);
  return value;                        /* return the result                 */
}

stringsize_t  RexxObject::requiredPositive(
    size_t       position,                   /* precision to use                  */
    stringsize_t precision)                  /* argument position for errors      */
/******************************************************************************/
/* Function:  Request a a positive long value from an object.  If this is not */
/*            positive, it will raise an error.                               */
/******************************************************************************/
{
  wholenumber_t  result;               /* returned result                   */

                                       /* get the value first               */
  result = this->requiredNumber(position, precision);
  if (result <= 0)                     /* non-positive number?              */
                                       /* raise the error                   */
    reportException(Error_Incorrect_method_positive, position, this);
  return (stringsize_t)result;         /* return the result                 */
}

stringsize_t RexxObject::requiredNonNegative(
    size_t position ,                        /* precision to use                  */
    stringsize_t precision)                  /* argument position for errors      */
/******************************************************************************/
/* Function:  Request a non-negative long value from an object.  If this is   */
/*            less than zero, it will raise an error                          */
/******************************************************************************/
{
  wholenumber_t result;                /* returned result                   */

                                       /* get the value first               */
  result = this->requiredNumber(position, precision);
  if (result < 0)                      /* negative number?                  */
                                       /* raise the error                   */
    reportException(Error_Incorrect_method_nonnegative, position, this);
  return (stringsize_t)result;         /* return the result                 */
}

RexxArray *RexxObject::requestArray()
/******************************************************************************/
/* Function:  Request an array value from an object.                          */
/******************************************************************************/
{
  if (isPrimitive(this)) {             /* primitive object?                 */
    if (isArray(this))            /* already an array?                 */
      return (RexxArray *)this;        /* return directly, don't makearray  */
    else
      return this->makeArray();        /* return the array value            */
  }
  else                                 /* return integer value of string    */
    return (RexxArray *)this->sendMessage(OREF_REQUEST, OREF_ARRAYSYM);
}

RexxString *RexxObject::objectName()
/******************************************************************************/
/* Function:  Retrieve the object name for a primitive object                 */
/******************************************************************************/
{
  RexxObject *scope;                   /* method's variable scope           */
  RexxString *string_value;            /* returned string value             */
  RexxActivity *activity = RexxActivityManager::currentActivity;

  scope = activity->getLastMethod()->getScope();  /* get the method's scope            */
                                       /* get the object name variable      */
  string_value = (RexxString *)this->getObjectVariable(OREF_NAME, scope);
  if (string_value == OREF_NULL) {     /* no name?                          */
    if (isPrimitive(this))             /* primitive object?                 */
                                       /* use fast path to default name     */
      string_value = this->defaultName();
    else                               /* go through the full search        */
      string_value = (RexxString *)this->sendMessage(activity, OREF_DEFAULTNAME);
  }
  return string_value;                 /* return the string value           */
}

RexxObject  *RexxObject::objectNameEquals(RexxObject *name)
/******************************************************************************/
/* Function:  retrieve an objects default name value                          */
/******************************************************************************/
{
  RexxObject *scope;                   /* scope of the object               */
  RexxActivity *activity = RexxActivityManager::currentActivity;

  requiredArg(name, ONE);             /* must have a name                  */
  scope = activity->getLastMethod()->getScope();        /* get the method's scope            */
                                       /* get this as a string              */
  name = (RexxObject *)REQUIRED_STRING(name, ARG_ONE);
                                       /* set the name                      */
  this->setObjectVariable(OREF_NAME, name, scope);
  return OREF_NULL;                    /* no return value                   */
}

RexxObject  *RexxObject::setAttribute(RexxObject *value)
/******************************************************************************/
/* Function:  set an object's named attribute to a value                  */
/******************************************************************************/
{
  requiredArg(value, ONE);            /* must have a value                 */
  RexxMethod *method = RexxActivityManager::currentActivity->getLastMethod();
                                       /* just directly set the value       */
  method->getAttribute()->set(this->getObjectVariables(method->getScope()), value);
  return OREF_NULL;                    /* no return value                   */
}

RexxObject  *RexxObject::getAttribute()
/******************************************************************************/
/* Function:  retrieve an object's named attribute                            */
/******************************************************************************/
{
    RexxMethod *method = RexxActivityManager::currentActivity->getLastMethod();
                                       /* just directly retrieve the value  */
    return method->getAttribute()->getValue(this->getObjectVariables(method->getScope()));
}

RexxString  *RexxObject::defaultName()
/******************************************************************************/
/* Function:  Handle "final" string coercion level                            */
/******************************************************************************/
{
  RexxString* defaultname;             /* returned default name             */

                                       /* use the class id as the default   */
                                       /* name                              */
  defaultname = this->behaviour->getCreateClass()->id;
                                       /* check if it is from an enhanced   */
  if (this->behaviour->isEnhanced()) { /* class                             */
                                       /* return the 'enhanced' id          */
   return defaultname->concatToCstring("enhanced ");
  }
  switch (defaultname->getChar(0)) {   /* process based on first character*/
    case 'a':                          /* vowels                          */
    case 'A':
    case 'e':
    case 'E':
    case 'i':
    case 'I':
    case 'o':
    case 'O':
    case 'u':
    case 'U':
                                       /* prefix with "an"                  */
      defaultname = defaultname->concatToCstring("an ");
      break;

    default:                           /* consonants                        */
                                       /* prefix with "a"                   */
      defaultname = defaultname->concatToCstring("a ");
      break;
  }
  return defaultname;                  /* return that value                 */
}

RexxInteger *RexxObject::hasMethod(RexxString *msgname)
/******************************************************************************/
/* Function:  Check for the presense of a method on the object                */
/******************************************************************************/
{
                                       /* check the behaviour for the method*/
  return (this->behaviour->methodObject(msgname) != TheNilObject) ? TheTrueObject : TheFalseObject;
}

/**
 * Check for the presense of a method on an object.
 *
 * @param msgname The method name of interest.
 *
 * @return true if the object has the required method, false otherwise.
 */
bool RexxObject::hasMethod(stringchar_t *msgname)
{
    return hasMethod(new_string(msgname)) == TheTrueObject;
}

RexxClass   *RexxObject::classObject()
/******************************************************************************/
/* Function:  Return the class object associated with an object               */
/******************************************************************************/
{
                                       /* just return class from behaviour  */
  return this->behaviour->getCreateClass();
}

RexxObject  *RexxObject::setMethod(
    RexxString *msgname,               /* name of the new method            */
    RexxMethod *methobj,               /* associated method object/code     */
    RexxString *option)
/******************************************************************************/
/* Function:  Add a new method to an object instance                          */
/******************************************************************************/
{
                                       /* get the message name as a string  */
  msgname = REQUIRED_STRING(msgname, ARG_ONE)->upper();
  if (option) {
    option = REQUIRED_STRING(option, ARG_THREE);
    if (!stricmp("OBJECT", (char *)option->getStringData())) {
      // do nothing if OBJECT
    } else if (!stricmp("FLOAT", (char *)option->getStringData()))
      // "FLOAT" makes option a NULL pointer, causing the old default behaviour on setMethod...
      option = OREF_NULL;
    else
      reportException(Error_Incorrect_call_list, new_string((char *)CHAR_SETMETHOD), IntegerThree, new_string("\"FLOAT\", \"OBJECT\""), option);
  }

  if (methobj == OREF_NULL)            /* we weren't passed a method,       */
                                       /* add a dummy method                */
    methobj = (RexxMethod *)TheNilObject;
  else if (!isMethod(methobj))    /* not a method type already?        */
                                       /* make one from a string or array   */
    methobj = RexxMethod::newRexxCode(msgname, (RexxObject *)methobj, IntegerTwo, OREF_NULL);
  this->defMethod(msgname, methobj, option);   /* defMethod handles all the details */
  return OREF_NULL;                    /* no return value                   */
}

RexxObject  *RexxObject::unsetMethod(
    RexxString *msgname)               /* target message name               */
/******************************************************************************/
/* Function:  Remove a method from an object instance                         */
/******************************************************************************/
{
                                       /* get the message name as a string  */
  msgname = REQUIRED_STRING(msgname, ARG_ONE)->upper();
                                       /* now just go remove this           */
  this->behaviour->removeMethod(msgname);
  return OREF_NULL;                    /* no return value                   */
}

RexxObject  *RexxObject::requestRexx(
    RexxString *className)             /* target name of the class          */
/******************************************************************************/
/* Function:  Externalized version of the REQUEST method.  This tries to      */
/*            convert one class of object into another.                       */
/******************************************************************************/
{
  RexxString *  make_method;           /* needed make method                */
  RexxMethod *  method;                /* MAKExxxxx method pointer          */
  RexxString *  class_id;              /* class of this object              */

                                       /* Verify we have a string parm      */
  className = REQUIRED_STRING(className, ARG_ONE)->upper();
  class_id = this->id()->upper();      /* get the class name in uppercase   */
                                       /* of the same class?                */
  if (className->strictEqual(class_id) == TheTrueObject) {
    return this;                       /* already converted                 */
  }
                                       /* Get "MAKE"||class methodname      */
  make_method = className->concatToCstring((char *)CHAR_MAKE);
                                       /* find the MAKExxxx method          */
  method = this->behaviour->methodLookup(make_method);
                                       /* have this method?                 */
  if (method != OREF_NULL)
                                       /* Return its results                */
    return (RexxObject *)this->sendMessage(make_method);
  else                                 /* No makeclass method               */
    return TheNilObject;               /* Let user handle it                */
}

RexxMessage *RexxObject::start(
    RexxObject **arguments,            /* message arguments                 */
    size_t       argCount)             /* the number of arguments           */
/******************************************************************************/
/* Function:  Spin a message off on a seperate activity                       */
/******************************************************************************/
{
  RexxMessage *newMessage;             /* new message object                */
  RexxObject  *message;                /* message to be sent to receiver.   */
                                       /* message to be sent to receiver.   */
  RexxArray   *messageArray = (RexxArray*) TheNilObject;
  RexxActivationBase *activation;      /* current activation                */
  RexxObject  *sender;                 /* sending object                    */
  RexxString  *newMsgName;             /* msgname to be sent                */

#ifdef NOTHREADSUPPORT
   reportException(Execution_no_concurrency,
                  new_string("Concurrency not supported"));
#else

  if (argCount < 1 )                   /* no arguments?                     */
    missingArgument(ARG_ONE);          /* Yes, this is an error.            */
                                       /* Get the message name.             */
  message = arguments[0];              /* get the message .                 */
                                       /* Did we receive a message name     */
  if (message == OREF_NULL)
    missingArgument(ARG_ONE);          /* Yes, this is an error.            */

                                       /* if 1st arg is a string, we can do */
                                       /* this quickly                      */
  if (!isString(message)) {
                                       /* is this an array?                 */
    if (isArray(message)) {
      messageArray = (RexxArray*) message;
    } else {
      RexxClass *theClass = message->classObject();
      RexxArray *classes = theClass->classSuperClasses;
      size_t i = classes->numItems();
      for (; i != 0; i--) {
        if (classes->get(i) == TheStringClass)
          break;
      }
      if (i == 0) {                    /* not subclassed from string?       */
                                       /* see if this is an array item      */
        messageArray = message->requestArray();
      }
    }
  }
  if (messageArray != TheNilObject) {  /* is message specified as an array? */
                                       /* didn't get two arguments?         */
    if (messageArray->getDimension() != 1 || messageArray->size() != 2)
                                       /* raise an error                    */
      reportException(Error_Incorrect_method_message);
                                       /* get the message as a string       */
    newMsgName = REQUIRED_STRING(messageArray->get(1), ARG_ONE);
                                       /* Was starting scope omitted ?      */
    if (OREF_NULL == messageArray->get(2))
                                       /* Yes, this is an error, report it. */
      reportException(Error_Incorrect_method_noarg, IntegerTwo);
                                       /* get the top activation            */
    activation = RexxActivityManager::currentActivity->getCurrentContext();
                                       /* have an activation?               */
    if (activation != OREF_NULL) {
                                       /* get the receiving object          */
      sender = (RexxObject *)activation->u_receiver;
      if (sender != this)              /* not the same receiver?            */
                                       /* this is an error                  */
         reportException(Error_Execution_super);
    }
    else
                                       /* this is an error                  */
      reportException(Error_Execution_super);
  }
  else {                               /* not an array as message.          */
                                       /* force to a string value           */
    message = REQUIRED_STRING(message, ARG_ONE);
  }
                                       /* Create the new message object.    */
  newMessage = new RexxMessage(this, message, new (argCount - 1, arguments + 1) RexxArray);
  ProtectedObject p1(newMessage);
  newMessage->start(OREF_NULL);        /* Tell the message object to start  */
#endif                                 // end of NOTHREADSUPPORT
  return newMessage;                   /* return the new message object     */
}


RexxString  *RexxObject::objectReference()
/****************************************************************************/
/* Function:  Return the objects address as a HEX string (debugging only)   */
/****************************************************************************/
{
  char buffer[20];                     /* buffered address                  */

  sprintf(buffer, "%p", this);         /* format this                       */
                                       /* and return a string               */
  return (RexxString *)new_string(buffer);
}

RexxObject  *RexxInternalObject::hasUninit()
/****************************************************************************/
/* Function:  Tag an object as having an UNINIT method                      */
/****************************************************************************/
{
                                       /* tell the activity about this      */
   RexxInterpreter::addUninitObject((RexxObject *)this);
   return OREF_NULL;
}

RexxObject  *RexxObject::run(
    RexxObject **arguments,            /* method arguments                  */
    size_t       argCount)             /* the number of arguments           */
/****************************************************************************/
/* Function:  Run a method on an object as if it was part of the object's   */
/*            behaviour.                                                    */
/****************************************************************************/
{
  RexxMethod *methobj;                 /* the object method                 */
  RexxString *option;                  /* run option string                 */
  RexxArray  *arglist;                 /* forwarded option string           */
  RexxObject **argumentPtr = NULL;     /* default to no arguments passed along */
  size_t argcount = 0;

                                       /* get the method object             */
  methobj = (RexxMethod *)arguments[0];
  requiredArg(methobj, ONE);          /* make sure we have a method        */
  if (!isMethod(methobj)) {           /* this a method object?             */
                                      /* create a method object            */
    methobj = RexxMethod::newRexxCode(OREF_RUN, (RexxObject *)methobj, IntegerOne, OREF_NULL);
                                       /* set the correct scope             */
    methobj->setScope((RexxClass *)TheNilObject);
  }
  else
                                       /* ensure correct scope on method    */
    methobj = methobj->newScope((RexxClass *)TheNilObject);

  ProtectedObject p1(methobj);
  ProtectedObject p2;

  if (argCount > 1) {                  /* if any arguments passed           */
                                       /* get the 1st one, its the option   */
    option = (RexxString *)arguments[1];
                                       /* this is now required              */
    option = REQUIRED_STRING(option, ARG_TWO);
                                       /* process the different options     */
    switch (toupper(option->getChar(0))) {
      case 'A':                        /* args are an array                 */
                                       /* so they say, make sure we have an */
                                       /* array and we were only passed 3   */
                                       /*args                               */
        if (argCount < 3)              /* not enough arguments?             */
          missingArgument(ARG_THREE);  /* this is an error                  */
        if (argCount > 3)              /* too many arguments?               */
         reportException(Error_Incorrect_method_maxarg, IntegerThree);
                                       /* now get the array                 */
        arglist = (RexxArray *)arguments[2];
                                       /* force to array form               */
        arglist = arglist->requestArray();

        p2 = arglist;   // protect this
                                       /* not an array?                     */
        if (arglist == TheNilObject || arglist->getDimension() != 1)
                                       /* raise an error                    */
          reportException(Error_Incorrect_method_noarray, arguments[2]);
        /* grab the argument information */
        argumentPtr = arglist->data();
        argcount = arglist->size();
        break;

      case 'I':                        /* args are "strung out"             */
        /* point to the array data for the second value */
        argumentPtr = arguments + 2;
        argcount = argCount - 2;
        break;

      default:
                                       /* invalid option                    */
        reportException(Error_Incorrect_method_option, (stringchar_t *)"AI", option);
        break;
    }
  }
  ProtectedObject result;
                                       /* now just run the method....       */
  methobj->run(contextActivity(), this, OREF_NONE, argcount, argumentPtr, result);
  return (RexxObject *)result;
}

RexxObject  *RexxObject::defMethods(
    RexxDirectory *methods)            /* new table of methods              */
/****************************************************************************/
/* Function:  Add a table of methods to an object's behaviour               */
/****************************************************************************/
{
  HashLink i;                          /* loop counter                      */
  RexxMethod *method;
  RexxString *name;

                                       /* make a copy of the behaviour      */
  OrefSet(this, this->behaviour, (RexxBehaviour *)this->behaviour->copy());
                                       /* loop through the list of methods  */
  for (i = methods->first(); methods->available(i); i = methods->next(i)) {
                                       /* Get the methjod Object            */
    method = (RexxMethod *)methods->value(i);
    if (method != TheNilObject)        /* not a removal?                    */
                                       /* set a new scope on this           */
      method = method->newScope((RexxClass *)this);
    else
                                       /* no real method added              */
      method = (RexxMethod *)TheNilObject;
                                       /* Get the name for this method      */
    name = (RexxString *)methods->index(i);
    name = name->upper();              /* make sure the name is upperCase.  */
                                       /* add this method to the object's   */
                                       /* behaviour                         */
    this->behaviour->define(name, method);
  }
  return OREF_NULL;
}

RexxObject  *RexxObject::defMethod(
    RexxString *msgname,               /* new method name                   */
    RexxMethod *methobj,               /* associated method object          */
    RexxString *option)
/****************************************************************************/
/* Function:  Add a method to an object's behaviour                         */
/****************************************************************************/
{
  RexxMethod *methcopy;                /* copy of the original method       */
                                       /* default scope "FLOAT"             */
  RexxClass  *targetClass = (RexxClass*) TheNilObject;

  msgname = msgname->upper();          /* add this as an uppercase name     */
  if (methobj != TheNilObject) {       /* not a removal?                    */
                                       /* got an option? */
    if (option) {
      if (!stricmp("OBJECT", (char *)option->getStringData()))
        targetClass = this->behaviour->getCreateClass();
      else
        reportException(Error_Incorrect_call_list, new_string((char *)CHAR_SETMETHOD), IntegerThree, new_string("\"FLOAT\", \"OBJECT\""), option);
    }
                                       /* set a new scope on this           */
    methcopy = methobj->newScope(targetClass);
  }
  else
                                       /* no real method added              */
    methcopy = (RexxMethod *)TheNilObject;
                                       /* is this the first added method?   */
  if (this->behaviour->instanceMethodDictionary == OREF_NULL) {

/* copy primitive behaviour object and define the method, a copy is made to */
/* ensure that we don't update the behaviour of any other object, since they*/
/* may have been sharing the mvd.                                           */
    OrefSet(this, this->behaviour, (RexxBehaviour *)this->behaviour->copy());
  }
                                       /* now add this to the behaviour     */
  this->behaviour->addMethod(msgname, methcopy);
                                       /* adding an UNINIT method to obj?   */
  if (methobj != TheNilObject && msgname->strCompare(CHAR_UNINIT)) {
    this->hasUninit();                 /* yes, mark it as such              */
  }
  return OREF_NULL;
}

short RexxObject::ptype()
/******************************************************************************/
/* Function:  Return the object's primitive type number                       */
/******************************************************************************/
{
  return this->behaviour->typenum();
}

void RexxInternalObject::removedUninit()
/******************************************************************************/
/* Function:  Remove an UNINIT method from an object                          */
/******************************************************************************/
{
  RexxInterpreter::removeUninitObject((RexxObject *)this);
}

RexxObject * RexxObject::getObjectVariable(
  RexxString * name,                   /* variable name (name object)       */
  RexxObject * scope)                  /* target variable scope             */
/******************************************************************************/
/* Function:   retrieve the value of an object variable.  This name           */
/*             must be a name object, and only simple variables are supported.*/
/*             If the variable has not been assigned a value, then OREF_NULL  */
/*             is returned.                                                   */
/******************************************************************************/
{
  RexxVariableDictionary *ovd;         /* ovd for this object at base scope */

  if (OREF_NULL == scope)              /* were we passed a scope for lookup */
    scope = this;                      /* no, we use our own.               */
                                       /* get the ovd for our scope level   */
  ovd = this->getObjectVariables(scope);
  return ovd->realValue(name);         /* get the real variable value       */
}

void RexxObject::setObjectVariable(
  RexxString *name,                    /* variable name (name object)     */
  RexxObject *value,                   /* new variable value              */
  RexxObject *scope)                   /* target variable scope           */
/******************************************************************************/
/* Function:   assign a new value to a object variable.  This name            */
/*             must be a name object, and only simple variables are supported.*/
/******************************************************************************/
{
  RexxVariableDictionary *ovd;         /* target OVD for this scope         */

  if (OREF_NULL == scope)              /* were we passed a scope for lookup */
    scope = this;                      /* no, we use our own.               */
                                       /* get the ovd for our scope level   */
  ovd = this->getObjectVariables(scope);
  ovd->set(name, value);               /* do the variable assignment      */
}

void RexxObject::addObjectVariables(
    RexxVariableDictionary *dictionary)/* new variable set                  */
/******************************************************************************/
/* Function:  Add a new variable dictionary to an object with a given scope   */
/******************************************************************************/
{
    /* chain any existing dictionaries off of the new one */
    dictionary->setNextDictionary(objectVariables);
    /* make this the new head of the chain */
    OrefSet(this, objectVariables, dictionary);
}

RexxObject * RexxObject::superScope(
  RexxObject *startScope)              /* target scope                    */
/******************************************************************************/
/* Function:  Find the scope of a method's super class                        */
/******************************************************************************/
{
  return this->behaviour->superScope(startScope);
}

RexxMethod * RexxObject::superMethod(
  RexxString *msgName,                 /* target message name             */
  RexxObject *startScope)              /* starting lookup scope           */
/******************************************************************************/
/* Function:   Find a method using the given starting scope information       */
/******************************************************************************/
{
  return this->behaviour->superMethod(msgName, startScope);
}

RexxVariableDictionary * RexxObject::getObjectVariables(
    RexxObject *scope)                 /* required dictionary scope         */
/******************************************************************************/
/* Function:   Retrieve an object dictionary for a given scope                */
/******************************************************************************/
{
  RexxVariableDictionary *dictionary;  /* retrieved dictionary              */

  dictionary = objectVariables;        /* get the head of the chain         */
  while (dictionary != OREF_NULL) {    /* search for a scope match          */
      /* if we've found a match, return it */
      if (dictionary->isScope(scope)) {
          return dictionary;
      }
      dictionary = dictionary->getNextDictionary();
  }

                                       /* just create a new vdict           */
  dictionary = RexxVariableDictionary::newInstance(scope);
  /* chain any existing dictionaries off of the new one */
  dictionary->setNextDictionary(objectVariables);
  /* make this the new head of the chain */
  OrefSet(this, objectVariables, dictionary);
  SetObjectHasReferences(this);        /* we now have references            */
  return dictionary;                   /* return the correct ovd            */
}

stringchar_t *RexxObject::idString(void)
/******************************************************************************/
/* Function:  Return a pointer to the object's string value                   */
/******************************************************************************/
{
  RexxString *classId;                 /* return class id string            */

  classId = this->id();                /* get the id string                 */
  if (classId == OREF_NULL)            /* internal class?                   */
    return (stringchar_t *)"unknown Class";  /* return an unknown identifier      */
  else
    return classId->getStringData();        /* return the actual class ID        */
}

RexxString *RexxObject::id(void)
/******************************************************************************/
/* Function:  Get the class string name                                       */
/******************************************************************************/
{
  RexxClass *createClass;              /* class associated with the object  */

                                       /* get the class                     */
  createClass = this->behaviourObject()->getCreateClass();
  if (createClass == OREF_NULL)        /* no class object?                  */
    return OREF_NULL;                  /* return nothing                    */
  else
    return createClass->id;            /* return the class id string        */
}

RexxObject *RexxObject::init(void)
/******************************************************************************/
/* Function:  Exported Object INIT method                                     */
/******************************************************************************/
{
  return OREF_NULL;                    /* this is basically a no-op         */
}

void        RexxObject::uninit(void)
/******************************************************************************/
/* Function:  Exported Object INIT method                                     */
/******************************************************************************/
{
  if (TheTrueObject == this->hasMethod(OREF_UNINIT))
    this->sendMessage(OREF_UNINIT);

}

bool RexxObject::hasUninitMethod()
/******************************************************************************/
/* Function:  Check to see if an object has an UNINIT method.                 */
/******************************************************************************/
{
  return TheTrueObject == this->hasMethod(OREF_UNINIT);
}

RexxObject *RexxObject::newRexx(RexxObject **arguments, size_t argCount)
/******************************************************************************/
/* Function:  Exposed REXX NEW method                                         */
/******************************************************************************/
{
  return new ((RexxClass *)this, arguments, argCount) RexxObject;
}

#undef operatorMethod
#define operatorMethod(name, message) RexxObject * RexxObject::name(RexxObject *operand) \
{\
  RexxObject *result;                  /* returned result                   */\
                                       /* do a real message send            */\
  result = (RexxObject *)this->sendMessage(OREF_##message, &operand, 1); \
  if (result == OREF_NULL)             /* in an expression and need a result*/ \
                                       /* need to raise an exception        */ \
    reportException(Error_No_result_object_message, OREF_##message); \
  return result;                       /* return the final result           */ \
}\


operatorMethod(operator_plus                      , PLUS)
operatorMethod(operator_minus                     , SUBTRACT)
operatorMethod(operator_multiply                  , MULTIPLY)
operatorMethod(operator_divide                    , DIVIDE)
operatorMethod(operator_integerDivide             , INTDIV)
operatorMethod(operator_remainder                 , REMAINDER)
operatorMethod(operator_power                     , POWER)
operatorMethod(operator_abuttal                   , NULLSTRING)
operatorMethod(operator_concat                    , CONCATENATE)
operatorMethod(operator_concatBlank               , BLANK)
operatorMethod(operator_equal                     , EQUAL)
operatorMethod(operator_notEqual                  , BACKSLASH_EQUAL)
operatorMethod(operator_isGreaterThan             , GREATERTHAN)
operatorMethod(operator_isBackslashGreaterThan    , BACKSLASH_GREATERTHAN)
operatorMethod(operator_isLessThan                , LESSTHAN)
operatorMethod(operator_isBackslashLessThan       , BACKSLASH_LESSTHAN)
operatorMethod(operator_isGreaterOrEqual          , GREATERTHAN_EQUAL)
operatorMethod(operator_isLessOrEqual             , LESSTHAN_EQUAL)
operatorMethod(operator_strictEqual               , STRICT_EQUAL)
operatorMethod(operator_strictNotEqual            , STRICT_BACKSLASH_EQUAL)
operatorMethod(operator_strictGreaterThan         , STRICT_GREATERTHAN)
operatorMethod(operator_strictBackslashGreaterThan, STRICT_BACKSLASH_GREATERTHAN)
operatorMethod(operator_strictLessThan            , STRICT_LESSTHAN)
operatorMethod(operator_strictBackslashLessThan   , STRICT_BACKSLASH_LESSTHAN)
operatorMethod(operator_strictGreaterOrEqual      , STRICT_GREATERTHAN_EQUAL)
operatorMethod(operator_strictLessOrEqual         , STRICT_LESSTHAN_EQUAL)
operatorMethod(operator_lessThanGreaterThan       , LESSTHAN_GREATERTHAN)
operatorMethod(operator_greaterThanLessThan       , GREATERTHAN_LESSTHAN)
operatorMethod(operator_and                       , AND)
operatorMethod(operator_or                        , OR)
operatorMethod(operator_xor                       , XOR)
operatorMethod(operator_not                       , BACKSLASH)

void *RexxObject::operator new(size_t size, RexxClass *classObject)
/******************************************************************************/
/* Function:  Create a new translator object                                  */
/******************************************************************************/
{
  RexxObject *newObject;               /* created new object                */

                                       /* get storage for new object        */
  newObject = (RexxObject *)new_object(size);
                                       /* use the class instance behaviour  */
  BehaviourSet(newObject, classObject->instanceBehaviour);
                                       /* use the default hash value        */
  newObject->hashvalue = HASHOREF(newObject);
                                       /* clear the object variable oref    */
  OrefSet(newObject, newObject->objectVariables, OREF_NULL);


  if (classObject->uninitDefined() || classObject->parentUninitDefined()) {  /* or parent has one */
     newObject->hasUninit();
   }

  return (void *)newObject;            /* Initialize the new object         */
}

void *RexxObject::operator new(size_t size, RexxClass *classObject, RexxObject **args, size_t argCount)
/******************************************************************************/
/* Function:  Create a new instance of object                                  */
/******************************************************************************/
{
  RexxObject *newObject;               /* created new object                */

                                       /* create a new object               */
  newObject = new (classObject) RexxObject;
  ProtectedObject p1(newObject);
                                       /* now drive the user INIT           */
  newObject->sendMessage(OREF_INIT, args, argCount);
  return newObject;                    /* and returnthe new object          */
}

RexxString *RexxObject::stringRexx()
/******************************************************************************/
/* Function:  Exported access to an object virtual function                   */
/******************************************************************************/
{
  return this->stringValue();          /* forward to the virtual function   */
}

RexxObject *RexxObject::makeStringRexx()
/******************************************************************************/
/* Function:  Exported access to an object virtual function                   */
/******************************************************************************/
{
  return this->makeString();           /* forward to the virtual function   */
}

RexxObject *RexxObject::makeArrayRexx()
/******************************************************************************/
/* Function:  Exported access to an object virtual function                   */
/******************************************************************************/
{
  return this->makeArray();            /* forward to the virtual function   */
}

RexxString *RexxObject::defaultNameRexx()
/******************************************************************************/
/* Function:  Exported access to an object virtual function                   */
/******************************************************************************/
{
  return this->defaultName();          /* forward to the virtual function   */
}

RexxObject *RexxObject::copyRexx()
/******************************************************************************/
/* Function:  Exported access to an object virtual function                   */
/******************************************************************************/
{
  return this->copy();                 /* forward to the virtual function   */
}

RexxObject *RexxObject::unknownRexx(
    RexxString *message,               /* unknown message                   */
    RexxArray  *arguments )            /* message arguments                 */
/******************************************************************************/
/* Function:  Exported access to an object virtual function                   */
/******************************************************************************/
{
                                       /* forward to the virtual function   */
  return this->unknown(message, arguments);
}

RexxObject *RexxObject::hasMethodRexx(
    RexxString *message )              /* method name                       */
/******************************************************************************/
/* Function:  Exported access to an object virtual function                   */
/******************************************************************************/
{
  return this->hasMethod(message);     /* forward to the virtual function   */
}

void RexxInternalObject::printObject()
/******************************************************************************/
/* Function:  give a formatted print of object information.                   */
/******************************************************************************/
{
    printf("Object at %p, of type %d\n",
        this, objectTypeNumber(this));
}

void RexxObject::processNewArgs(
    RexxObject **arg_array,            /* source argument array             */
    size_t       argCount,             /* size of the argument array        */
    RexxObject***init_args,            /* remainder arguments               */
    size_t      *remainderSize,        /* remaining count of arguments      */
    size_t       required,             /* number of arguments we require    */
    RexxObject **argument1,            /* first returned argument           */
    RexxObject **argument2 )           /* second return argument            */
/******************************************************************************/
/* Function:  Divide up a class new arglist into new arguments and init args  */
/******************************************************************************/
{
  *argument1 = OREF_NULL;              /* clear the first argument          */
  if (argCount >= 1)                   /* have at least one argument?       */
    *argument1 = arg_array[0];         /* get the first argument            */
  if (required == 2) {                 /* processing two arguments?         */
    if (argCount >= 2)                 /* get at least 2?                   */
      *argument2 = arg_array[1];       /* get the second argument           */
    else
      *argument2 = OREF_NULL;          /* clear the second argument         */
  }
                                       /* get the init args part            */
  *init_args = arg_array + required;
  /* if we have at least the required arguments, reduce the count. */
  /* Otherwise, set this to zero. */
  if (argCount >= required) {
      *remainderSize = argCount - required;
  }
  else {
      *remainderSize = 0;
  }
}


PCPPM RexxObject::operatorMethods[] = {   /* object operator methods           */
   NULL,                               /* first entry not used              */
   (PCPPM)&RexxObject::operator_plus,
   (PCPPM)&RexxObject::operator_minus,
   (PCPPM)&RexxObject::operator_multiply,
   (PCPPM)&RexxObject::operator_divide,
   (PCPPM)&RexxObject::operator_integerDivide,
   (PCPPM)&RexxObject::operator_remainder,
   (PCPPM)&RexxObject::operator_power,
   (PCPPM)&RexxObject::operator_abuttal,
   (PCPPM)&RexxObject::operator_concat,
   (PCPPM)&RexxObject::operator_concatBlank,
   (PCPPM)&RexxObject::operator_equal,
   (PCPPM)&RexxObject::operator_notEqual,
   (PCPPM)&RexxObject::operator_isGreaterThan,
   (PCPPM)&RexxObject::operator_isBackslashGreaterThan,
   (PCPPM)&RexxObject::operator_isLessThan,
   (PCPPM)&RexxObject::operator_isBackslashLessThan,
   (PCPPM)&RexxObject::operator_isGreaterOrEqual,
   (PCPPM)&RexxObject::operator_isLessOrEqual,
   (PCPPM)&RexxObject::operator_strictEqual,
   (PCPPM)&RexxObject::operator_strictNotEqual,
   (PCPPM)&RexxObject::operator_strictGreaterThan,
   (PCPPM)&RexxObject::operator_strictBackslashGreaterThan,
   (PCPPM)&RexxObject::operator_strictLessThan,
   (PCPPM)&RexxObject::operator_strictBackslashLessThan,
   (PCPPM)&RexxObject::operator_strictGreaterOrEqual,
   (PCPPM)&RexxObject::operator_strictLessOrEqual,
   (PCPPM)&RexxObject::operator_lessThanGreaterThan,
   (PCPPM)&RexxObject::operator_greaterThanLessThan,
   (PCPPM)&RexxObject::operator_and,
   (PCPPM)&RexxObject::operator_or,
   (PCPPM)&RexxObject::operator_xor,
   (PCPPM)&RexxObject::operator_not,
};
