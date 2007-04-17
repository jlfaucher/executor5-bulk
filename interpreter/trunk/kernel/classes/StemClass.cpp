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
/* REXX Kernel                                                                */
/*                                                                            */
/* Primitive Stem variable class                                              */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "StringClass.hpp"
#include "ArrayClass.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include "RexxNativeActivation.hpp"
#include "RexxVariableDictionary.hpp"
#include "RexxVariable.hpp"
#include "StemClass.hpp"
#include "SupplierClass.hpp"

RexxClass *RexxStem::classInstance = OREF_NULL;    // the singleton class instance

RexxStem::RexxStem(
    RexxString *name)                  /* the variable name                 */
/******************************************************************************/
/* Function:  Initialize a STEM class item                                    */
/******************************************************************************/
{
  ClearObject(this);                   /* start fresh                       */
  if (name == OREF_NULL)               /* no explicit default value?        */
    name = OREF_NULLSTRING;            /* set a null string                 */
  else
                                       /* must get a string here            */
    name = REQUIRED_STRING(name, ARG_ONE);
  OrefSet(this, this->u_name, name);   /* fill in the name                  */
  OrefSet(this, this->value, name);    /* fill in the default value         */
  tails.init(this);                    /* initialize the tail table         */
                                       /* create a tails table              */
  this->dropped = true;                /* no explicit value                 */
}

RexxObject *RexxStem::copy(void)
/******************************************************************************/
/* Function:  Copy a stem collection object                                   */
/******************************************************************************/
{
  RexxStem *newObject;                 /* new stem object                   */
                                       /* make a copy of ourself (this also */
                                       /* copies the object variables       */
  newObject = (RexxStem *)this->RexxObject::copy();

  ProtectedObject p1(newObject);

  newObject->tails.copy(newObject, this);/* have the tail table copy itself   */
  return newObject;                    /* return the new object             */
}

void RexxStem::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->value);
  memory_mark(this->u_name);
  memory_mark(this->objectVariables);
  markCompoundTable();
  cleanUpMemoryMark
}

void RexxStem::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->value);
  memory_mark_general(this->u_name);
  memory_mark_general(this->objectVariables);
  markGeneralCompoundTable();
  cleanUpMemoryMarkGeneral
}

void RexxStem::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxStem)

   flatten_reference(newThis->value, envelope);
   flatten_reference(newThis->u_name, envelope);
   flatten_reference(newThis->objectVariables, envelope);
   flattenCompoundTable();

  cleanUpFlatten
}

void RexxStem::setValue(
    RexxObject *value)                 /* new variable value                */
/******************************************************************************/
/* Function:  Set a new stem default value                                    */
/******************************************************************************/
{
  OrefSet(this,this->value,value);     /* set the new value                 */
  this->dropped = false;               /* now have an explict value         */
}

void RexxStem::dropValue()
/******************************************************************************/
/* Function:  Drop a stem value -- this reinitializes it                      */
/******************************************************************************/
{
                                       /* reset to the default value        */
  OrefSet(this, this->value, this->u_name);
  this->dropped = true;                /* no explict value any more         */
}

/**
 * Retrieve the assigned stem value.
 *
 * @return The default stem value.
 */
RexxObject *RexxStem::getStemValue()
{
    return value;
}

RexxObject *RexxStem::unknown(
    RexxString *msgname,               /* unknown message name              */
    RexxArray  *arguments)             /* message arguments                 */
/******************************************************************************/
/* Function:  Forward an unknown message to the value of the stem.            */
/******************************************************************************/
{
                                       /* validate the name                 */
  msgname = REQUIRED_STRING(msgname, ARG_ONE);
  requiredArg(arguments, TWO);        /* need an argument array            */
                                       /* get this as an array              */
  arguments = arguments->requestArray();
  if (arguments == TheNilObject)       /* didn't convert?                   */
                                       /* raise an error                    */
      reportException(Error_Incorrect_method_noarray, IntegerTwo);
                                       /* just send the message on          */
  return value->sendMessage(msgname, arguments);
}

RexxObject *RexxStem::bracket(
    RexxObject **tails,                /* tail elements                     */
    size_t      argCount)              /* number of tail elements           */
/******************************************************************************/
/* Function:  Resolve the "stem.[a,b,c]" to the equivalent stem.a.b.c form,   */
/*            with all of the indices taken as constants                      */
/******************************************************************************/
{
  RexxObject   * value;                /* element value                     */

  if (argCount == 0)                   /* default value request?            */
    return this->value;                /* just return the default value     */
                                       /* create a searchable tail from the array elements */
  RexxCompoundTail resolved_tail(tails, argCount);
                                       /* now look up this element */
  value = evaluateCompoundVariableValue(OREF_NULL, &resolved_tail);
  return value;                        /* return variable value             */
}

/**
 * Evaluate an array element for an API class.
 *
 * @param tail   The direct tail value.
 *
 * @return The object value.  If the stem element does not exist or
 *         has been dropped, this returns OREF_NULL.
 */
RexxObject *RexxStem::getElement(stringsize_t tail)
{

    RexxCompoundTail resolved_tail(tail);

    return getElement(&resolved_tail);
}

/**
 * Evaluate an array element for an API class.
 *
 * @param tail   The direct tail value.
 *
 * @return The object value.  If the stem element does not exist or
 *         has been dropped, this returns OREF_NULL.
 */
RexxObject *RexxStem::getElement(stringchar_t *tail)
{

    RexxCompoundTail resolved_tail(tail);

    return getElement(&resolved_tail);
}

/**
 * Resolve a compound variable as a result of an api call.
 *
 * @param resolved_tail
 *               The resolved tail value.
 *
 * @return The variable value.  Returns OREF_NULL if not assigned or
 *         dropped.
 */
RexxObject *RexxStem::getElement(RexxCompoundTail *resolved_tail)
{
    // see if we have a variable...if we do, return its value (a dropped variable
    // has a value of OREF_NULL).  If not found, return OREF_NULL;
    RexxCompoundElement *variable = findCompoundVariable(resolved_tail);
    if (variable != OREF_NULL)
    {
        return variable->getVariableValue();
    }
    return OREF_NULL;
}


/**
 * Drop an array element for an API class.
 *
 * @param tail   The direct tail value.
 */
void RexxStem::dropElement(stringsize_t tail)
{

    RexxCompoundTail resolved_tail(tail);

    return dropElement(&resolved_tail);
}

/**
 * Drop an array element for an API class.
 *
 * @param tail   The direct tail value.
 */
void RexxStem::dropElement(stringchar_t *tail)
{

    RexxCompoundTail resolved_tail(tail);

    return dropElement(&resolved_tail);
}


/**
 * Drop an element using a resolved tail value.
 *
 * @param resolved_tail
 *               The target tail element.
 */
void RexxStem::dropElement(RexxCompoundTail *resolved_tail)
{
    // see if we have a variable...if we do, return its value (a dropped variable
    // has a value of OREF_NULL).  If not found, return OREF_NULL;
    RexxCompoundElement *variable = findCompoundVariable(resolved_tail);
    if (variable != OREF_NULL)
    {
        variable->drop();
    }
}


RexxObject *RexxStem::bracketEqual(
    RexxObject **tails,                /* tail elements                     */
    size_t      argCount)              /* number of tail elements           */
/******************************************************************************/
/* Function:  Resolve the "stem.[a,b,c]=" to the equivalent stem.a.b.c= form, */
/*            with all of the indices taken as constants                      */
/******************************************************************************/
{
  RexxVariable * variable;             /* resolved variable element         */
  RexxObject   * new_value;            /* newly assigned value              */

  if (argCount == 0)                   /* have nothing at all?              */
                                       /* this is an error                  */
    reportException(Error_Incorrect_method_noarg, IntegerOne);
  new_value = tails[0];                /* get the new value                 */
  if (new_value == OREF_NULL)          /* nothing given?                    */
                                       /* this is an error also             */
    reportException(Error_Incorrect_method_noarg, IntegerOne);

  if (argCount == 1) {                 /* just setting the default value?   */
                                       /* set the new default value         */
    OrefSet(this, this->value, new_value);
    this->tails.clear();               /* clear out the dictionary          */
    this->dropped = false;             /* now have an explicit value        */
    return this->value;                /* just return the default value     */
  }
                                       /* create a searchable tail from the array elements */
  RexxCompoundTail resolved_tail(tails + 1, argCount - 1);
  variable = getCompoundVariable(&resolved_tail);
  variable->set(new_value);            /* set the new value                 */
  return OREF_NULL;                    /* never returns anything            */
}


/**
 * Set a single stem variable object using a simple string
 * value tail as a result of an api call.
 *
 * @param tail   The index of the target value.
 * @param value  The new value to assign.
 */
void RexxStem::setElement(stringchar_t *tail, RexxObject *value)
{
    RexxCompoundTail resolved_tail(tail);
    RexxVariable *variable = getCompoundVariable(&resolved_tail);
    variable->set(value);                /* set the new value                 */
}


/**
 * Set a single stem variable object using a simple string
 * value tail as a result of an api call.
 *
 * @param tail   The index of the target value.
 * @param value  The new value to assign.
 */
void RexxStem::setElement(stringsize_t tail, RexxObject *value)
{
    RexxCompoundTail resolved_tail(tail);
    RexxVariable *variable = getCompoundVariable(&resolved_tail);
    variable->set(value);                /* set the new value                 */
}


RexxArray  *RexxStem::makeArray()
/******************************************************************************/
/* Function:  Extract as an array the tails of a stem.                        */
/******************************************************************************/
{
  return this->tailArray();            /* extract the array item            */
}


RexxArray  *RexxStem::allIndexes()
/******************************************************************************/
/* Function:  Extract as an array the tails of a stem.                        */
/******************************************************************************/
{
  return this->tailArray();            /* extract the array item            */
}

RexxString *RexxStem::stringValue()
/******************************************************************************/
/* Function:  Forward string request on to the default value                  */
/******************************************************************************/
{
                                       /* return the objects string value   */
  return (RexxString *)this->value->stringValue();
}

bool RexxStem::numberValue(wholenumber_t *result, stringsize_t precision)
/******************************************************************************/
/* Function:  Forward long request on to the default value                    */
/******************************************************************************/
{                                      /* just forward to default value     */
   return this->value->numberValue(result, precision);
}

bool RexxStem::numberValue(wholenumber_t *result)
/******************************************************************************/
/* Function:  Forward long request on to the default value                    */
/******************************************************************************/
{                                      /* just forward to default value     */
   return this->value->numberValue(result);
}

bool RexxStem::unsignedNumberValue(stringsize_t *result, stringsize_t precision)
/******************************************************************************/
/* Function:  Forward long request on to the default value                    */
/******************************************************************************/
{                                      /* just forward to default value     */
   return this->value->unsignedNumberValue(result, precision);
}

bool RexxStem::unsignedNumberValue(stringsize_t *result)
/******************************************************************************/
/* Function:  Forward long request on to the default value                    */
/******************************************************************************/
{                                      /* just forward to default value     */
   return this->value->unsignedNumberValue(result);
}

bool RexxStem::doubleValue(double *result)
/******************************************************************************/
/* Function:  Forward double request on to the default value                  */
/******************************************************************************/
{
   return this->value->doubleValue(result);  /* just forward to default value     */
}

bool RexxStem::numberValueNoNOSTRING(wholenumber_t *result, stringsize_t precision)
/******************************************************************************/
/* Function:  Forward long request on to the default value                    */
/******************************************************************************/
{                                      /* just forward to default value     */
   return this->value->numberValueNoNOSTRING(result, precision);
}

bool RexxStem::numberValueNoNOSTRING(wholenumber_t *result)
/******************************************************************************/
/* Function:  Forward long request on to the default value                    */
/******************************************************************************/
{                                      /* just forward to default value     */
   return this->value->numberValueNoNOSTRING(result);
}

bool RexxStem::doubleValueNoNOSTRING(double *result)
/******************************************************************************/
/* Function:  Forward double request on to the default value                  */
/******************************************************************************/
{
                                       /* just forward to default value     */
   return this->value->doubleValueNoNOSTRING(result);
}

RexxNumberString *RexxStem::numberString()
/******************************************************************************/
/* Function:  Forward numberstring request on to the default value            */
/******************************************************************************/
{
                                       /* just forward to default value     */
   return this->value->numberString();
}

RexxInteger *RexxStem::integerValue(stringsize_t precision)
/******************************************************************************/
/* Function:  Forward integer request on to the default value                 */
/******************************************************************************/
{
                                       /* just forward to default value     */
  return this->value->integerValue(precision);
}

RexxObject *RexxStem::request(
    RexxString *makeclass)             /* required class name               */
/******************************************************************************/
/* Function:  Forward all REQUEST messages on to the default value            */
/******************************************************************************/
{
                                       /* Verify we have a string parm      */
  makeclass = REQUIRED_STRING(makeclass, ARG_ONE)->upper();
                                       /* array request?                    */
  if (makeclass->strCompare(CHAR_ARRAY)) {
    if (isStem(this))             /* a real stem object?               */
                                       /* process here directly             */
      return (RexxObject *)this->makeArray();
    else                               /* go to the real make array method  */
      return this->sendMessage(OREF_MAKEARRAY);
  }
                                       /* just forward on                   */
  return (RexxObject *)this->value->sendMessage(OREF_REQUEST, makeclass);
}

RexxObject *RexxStem::newRexx(
    RexxObject **init_args,           /* subclass init arguments           */
    size_t       argCount)            /* the number of arguments           */
/******************************************************************************/
/* Function:  Create an instance of a stem                                    */
/******************************************************************************/
{
  RexxObject * newObject;              /* newly created queue object        */
  RexxString * name;                   /* name of the stem item             */

                                       /* break up the arguments            */
  processNewArgs(init_args, argCount, &init_args, &argCount, 1, (RexxObject **)&name, NULL);
  newObject = new RexxStem (name);     /* get a new relation                */
  BehaviourSet(newObject, ((RexxClass *)this)->instanceBehaviour);
                                       /* does object have an UNINT method  */
  if (((RexxClass *)this)->uninitDefined())
     newObject->hasUninit();           /* Make sure everyone is notified.   */
                                       /* Initialize the new instance       */
  newObject->sendMessage(OREF_INIT, init_args, argCount);
  return newObject;                    /* return the new object             */
}

void *RexxStem::operator new(size_t size)
/******************************************************************************/
/* Function:  Create a new stem object                                        */
/******************************************************************************/
{
    return new_object(size, TheStemBehaviour);
}


RexxCompoundElement *RexxStem::getCompoundVariable(
    RexxCompoundTail *name)             /* tail name                         */
/******************************************************************************/
/* Function:  Get an item from the variable dictionary, adding a new empty    */
/*             variable entry if it wasn't found.                             */
/******************************************************************************/
{
                                        /* get/create an entry in the table */
    return tails.findEntry(name, true)->realVariable();
}


RexxCompoundElement *RexxStem::exposeCompoundVariable(
    RexxCompoundTail *name)             /* tail name                         */
/******************************************************************************/
/* Function:  Get an item from the variable dictionary, adding a new empty    */
/*             variable entry if it wasn't found.                             */
/******************************************************************************/
{
    // first see if the compound variable already exists.  If it does, then
    // it might represent an explicitly dropped varaible.  We leave this along
    // in that case.
    RexxCompoundElement *variable = tails.findEntry(name, false);
    // ok, it's already there, return the real variable value.
    if (variable != OREF_NULL)
    {
        return variable->realVariable();
    }

    // we're creating a real variable.  If the stem has an explicit value,
    // then we need to set that now.
    variable = tails.findEntry(name, true)->realVariable();
    /* if this variable does not have a value, we need to check the */
    /* stem for a default value and assign it. */
    if (variable->getVariableValue() == OREF_NULL) {
        if (!dropped) {                 /* stem have a default value?        */
            variable->set(value);       /* get the stem's value explicitly   */
        }
    }
    return variable;
}


RexxCompoundElement *RexxStem::findCompoundVariable(
    RexxCompoundTail *name)             /* tail name                         */
/******************************************************************************/
/* Function:  Get an item from the variable dictionary, without creating a    */
/*            new variable if it doesn't exist.                               */
/******************************************************************************/
{
    RexxCompoundElement *variable;
                                        /* get, but don't create an entry in the table */
    variable = tails.findEntry(name);
    if (variable != OREF_NULL) {
        return variable->realVariable();/* return the real target */
    }
    return OREF_NULL;
}


void RexxStem::dropCompoundVariable(
    RexxCompoundTail *name)             /* tail name                         */
/******************************************************************************/
/* Function:  Mark a variable as dropped.                                     */
/******************************************************************************/
{
  RexxVariable *variable;              /* located variable reference        */

  variable = getCompoundVariable(name);/* look up this tail element         */
  variable->drop();                    /* drop the variable entry           */
}


RexxCompoundElement *RexxStem::nextVariable(
  RexxNativeActivation *activation)    /* Hosting Native Act.               */
/******************************************************************************/
/* Function:  Return the "next" variable of a variable traversal              */
/******************************************************************************/
{
  RexxCompoundElement *variable;       /* variable entry                    */
  RexxObject *value;                   /* variable value                    */

                                       /* get the last saved stem           */
  variable = activation->compoundElement();

  while (variable != OREF_NULL) {      /* while we have more to process     */
                                       /* get the value                     */
    value = variable->getVariableValue();
    if (value != OREF_NULL) {          /* not a dropped variable?           */
      activation->setCompoundElement(tails.next(variable));
      return variable;                 /* give this one back                */
    }
    variable = tails.next(variable);   /* step to the next table item       */
  }
                                       /* clear out the saved element       */
  activation->setCompoundElement(OREF_NULL);
  activation->setNextStem(OREF_NULL);

  return OREF_NULL;                    /* return end of table indicator     */
}

void RexxStem::setCompoundVariable(
    RexxCompoundTail *name,            /* tail name                         */
    RexxObject *value)                 /* value to assign to variable name  */
/******************************************************************************/
/* Function:  Set a new variable value                                        */
/******************************************************************************/
{
  RexxCompoundElement *variable;       /* retrieved variable item           */

                                       /* see if we have an entry for this  */
  variable = getCompoundVariable(name);
  variable->set(value);                /* and perform the set               */
}


/**
 * Return all items in the stem.
 *
 * @return An array of all items in the stem.
 */
RexxArray *RexxStem::allItems()
{
    // now we know how big the return result will be, get an array and
    // populate it, using the same traversal logic as before
    RexxArray *array = new_array(items());
    // we index the array with a origin-one index, so we start with one this time
    arraysize_t count = 1;

    variable = tails.first();
    while (variable != OREF_NULL)
    {
        // only add the real values
        if (variable->getVariableValue() != OREF_NULL)
        {
            array->put(variable->getVariableValue(), count++);
        }
        variable = tails.next(variable);
    }
    return array;    // tada, finished
}


/**
 * Create an array of all indexes of the stem.
 *
 * @return An array of all tail names used in the stem.
 */
RexxArray  *RexxStem::allIndexes()
{
  return this->tailArray();            /* extract the array item            */
}


/**
 * Get the count of non-dropped items in the stem.
 *
 * @return The number of non-dropped items.
 */
arraysize_t RexxStem::items()
{
    arraysize_t count = 0;

    RexxCompoundElement *variable = tails.first();
   while (variable != OREF_NULL)
   {
        // we only want to include the non-dropped compounds, so we only count
        // elements with real values.
       if (variable->getVariableValue() != OREF_NULL)
       {
            count++;
       }
        // and keep iterating
       variable = tails.next(variable);
   }
    return count;
}


/**
 * Empty the stem.  This also clears dropped and exposed tails,
 *
 * @return Nothing.
 */
RexxObject *RexxStem::empty()
{
    tails.clear();      // just clear the tails.
    return OREF_NULL;
}


/**
 * Test if the stem is empty.
 *
 * @return True if the stem is empty, false otherwise.
 */
RexxObject *RexxStem::isEmpty()
{
    return (items() == 0) ? TheTrueObject : TheFalseObject;
}



/**
 * Create a supplier for the stem, returning the tail names as
 * the indexes and the values as the items.
 *
 * @return A supplier instance.
 */
RexxSupplier *RexxStem::supplier()
{
    // essentially the same logic as allItems(), but both the item and the
    // tail value are accumulated.
    arraysize_t count = 0;
    RexxCompoundElement *variable = tails.first();
    while (variable != OREF_NULL)
    {
        // again, get the variable count
        if (variable->getVariableValue() != OREF_NULL)
        {
            count++;                     /* count this variable               */
        }
        variable = tails.next(variable);
    }

    // to create the supplier, we need 2 arrays
    RexxArray *tailValues = new_array(count);
    RexxArray *values = new_array(count);
    count = 1;                           // we fill in using 1-based indexes

    variable = tails.first();
    while (variable != OREF_NULL)
    {
        // now grab both the tail and value and put them in the respective arrays
        if (variable->getVariableValue() != OREF_NULL)
        {
            tailValues->put(variable->getName(), count);
            values->put(variable->getVariableValue(), count++);
        }
        variable = tails.next(variable);
    }
    // two arrays become one supplier
    return new_supplier(values, tailValues);
}


RexxArray *RexxStem::tailArray()
/******************************************************************************/
/* Function:  Return all indices as an array                                  */
/******************************************************************************/
{
  RexxCompoundElement *variable;       /* table variable entry              */
  RexxArray  *array;                   /* returned array                    */
  arraysize_t   count;                 /* count of variables                */
                                       /* traverse through all of the items */
                                       /* in the stem variable dictionary,  */
                                       /* counting each real variable       */
  count = 0;                           /* start with zero                   */
  variable = tails.first();            /* get the first variable            */
  while (variable != OREF_NULL) {      /* while more values to process      */
                                       /* this a real variable?             */
      if (variable->getVariableValue() != OREF_NULL)
          count++;                     /* count this variable               */
      variable = tails.next(variable); /* go get the next one               */
  }
  array = new_array(count);            /* get the array                     */
  count = 1;                           /* start at the beginning again      */

  variable = tails.first();            /* get the first variable            */
  while (variable != OREF_NULL) {      /* while more values to process      */
                                       /* this a real variable?             */
      if (variable->getVariableValue() != OREF_NULL)
          /* add to our array                  */
          array->put(variable->getName(), count++);
      variable = tails.next(variable); /* go get the next one               */
  }
  return array;                        /* return the array item             */
}

RexxObject *RexxStem::evaluateCompoundVariableValue(
     RexxActivation *context,           /* the evaluation context (can be NULL) */
     RexxCompoundTail *resolved_tail)   /* the search tail                   */
/******************************************************************************/
/* Function:  Retrieve a compound variable, returning the default value if the*/
/*            variable does not exist.  This includes NOVALUE handling.       */
/******************************************************************************/
{
  RexxCompoundElement *variable;      /* the real variable                 */
  RexxObject   *value;                 /* final variable value              */
  RexxString   *tail_name;             /* formatted tail name               */

                                       /* get the compound variable         */
  variable = findCompoundVariable(resolved_tail);
  if (variable == OREF_NULL) {         /* variable does not exist?          */
    if (!dropped)                      /* stem have a default value?        */
      value = this->value;             /* get the stems value               */
    else {                             /* need to use name                  */
                                       /* create a string version of the name */
      tail_name = resolved_tail->createCompoundName(u_name);
                                       /* take care of any novalue situations */
      value = handleNovalue(tail_name, context);
    }
  }
  else {
                                       /* get the variable value            */
    value = variable->getVariableValue();
    if (value == OREF_NULL) {          /* explicitly dropped variable?      */
                                       /* create a string version of the name */
      tail_name = resolved_tail->createCompoundName(u_name);
                                       /* take care of any novalue situations */
      value = handleNovalue(tail_name, context);
    }
  }
  return value;                        /* and finally return the value */
}

/**
 * Evaluate the real value of a compound variable.  The real
 * value is either its explicitly assigned value or a stem
 * assigned value.  This returns OREF_NULL if neither is
 * available.  This does not raise NOVALUE conditions.
 *
 * @param resolved_tail
 *               The target tail value.
 *
 * @return The variable value, or OREF_NULL if the variable does not
 *         have an explicit value.
 */
RexxObject *RexxStem::getCompoundVariableRealValue(RexxCompoundTail *resolved_tail)
{
    // first resolve the compound variable.
    RexxVariable *variable = findCompoundVariable(resolved_tail);
    // if the variable is not found, return the stem's default value if it has one.
    // If there is no default value, return OREF_NULL.
    if (variable == OREF_NULL)           /* variable does not exist?          */
    {
        if (!dropped)
        {
            return value;
        }
        return OREF_NULL;
    }
    else
    {
        // just return the variable value (which may be OREF_NULL if explicitly dropped)
        return variable->getVariableValue();
    }
}

RexxObject *RexxStem::getCompoundVariableValue(
     RexxCompoundTail *resolved_tail)   /* the search tail                   */
/******************************************************************************/
/* Function:  Retrieve a compound variable, returning the default value if the*/
/*            variable does not exist.  This does NOT raise NOVALUE conditions*/
/******************************************************************************/
{
  RexxCompoundElement *variable;       /* the real variable                 */
  RexxObject   *value;                 /* final variable value              */

                                       /* get the compound variable         */
  variable = findCompoundVariable(resolved_tail);
  if (variable == OREF_NULL) {         /* variable does not exist?          */
    if (!dropped)                      /* stem have a default value?        */
      value = this->value;             /* get the stems value               */
    else {                             /* need to use name                  */
                                       /* create a string version of the name */
      value = (RexxObject *)resolved_tail->createCompoundName(u_name);
    }
  }
  else {
                                       /* get the variable value            */
    value = variable->getVariableValue();
    if (value == OREF_NULL) {          /* explicitly dropped variable?      */
                                       /* create a string version of the name */
      value = (RexxObject *)resolved_tail->createCompoundName(u_name);
    }
  }
  return value;                        /* and finally return the value */
}


RexxObject *RexxStem::realCompoundVariableValue(
     RexxCompoundTail *resolved_tail)   /* the search tail                   */
/******************************************************************************/
/* Function:  Retrieve a compound variable, returning the default value if the*/
/*            variable does not exist.  This does not handle NOVALUEs.        */
/******************************************************************************/
{
  RexxCompoundElement *variable;      /* the real variable                 */
  RexxObject   *value;                 /* final variable value              */

                                       /* get the compound variable         */
  variable = findCompoundVariable(resolved_tail);
  if (variable == OREF_NULL) {         /* variable does not exist?          */
    if (!dropped)                      /* stem have a default value?        */
      value = this->value;             /* get the stems value               */
    else {                             /* need to use name                  */
      value = OREF_NULL;               /* nothing to return                 */
    }
  }
  else {
                                       /* get the variable value            */
    value = variable->getVariableValue();
  }
  return value;                        /* and finally return the value */
}


RexxObject *RexxStem::handleNovalue(
    RexxString *name,                  /* the fully resolved compound name */
    RexxActivation *context)           /* the execution context for the request */
/******************************************************************************/
/* Function:  Process a nonvalue condition for a stem variable retrieval.     */
/*            If a context is provided, the novalue is handled in its         */
/*            context.                                                        */
/******************************************************************************/
{
    RexxObject *value;

    /* are we doing this directly for method execution? */
    if (context != OREF_NULL) {
        /* the context may need to do additional work */
        value = context->handleNoValueEvent(name);
    }
    else {
        value = name;                /* just use the name                 */
    }
    return value;
}


void RexxStem::expose(
    RexxCompoundElement *old_variable) /* the parent compound variable     */
/******************************************************************************/
/* Function:  Retrieve a compound variable, returning the default value if the*/
/*            variable does not exist.  This includes NOVALUE handling.       */
/******************************************************************************/
{
                                       /* create the equivalent in this stem */
  RexxCompoundElement *new_variable = tails.findEntry(old_variable->getName(), true);
  new_variable->expose(old_variable);  /* make the association between the two */
}


/******************************************************************************/
/* Function:  Below are a series of comparison routines used by the qsort()   */
/*            library function when sorting stems.                            */
/******************************************************************************/
int compare_asc_i(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return arg1->sortCaselessCompare(arg2);
}

int compare_asc_i_cols(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return arg1->sortCaselessCompare(arg2, sd->startColumn, sd->columnLength);
}

int compare_asc(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return arg1->sortCompare(arg2);
}

int compare_asc_cols(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return arg1->sortCompare(arg2, sd->startColumn, sd->columnLength);
}

int compare_desc(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return -arg1->sortCompare(arg2);
}

int compare_desc_cols(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return -arg1->sortCompare(arg2, sd->startColumn, sd->columnLength);
}

int compare_desc_i(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return -arg1->sortCaselessCompare(arg2);
}

int compare_desc_i_cols(SortData *sd, RexxString *arg1, RexxString *arg2)
{
    return -arg1->sortCaselessCompare(arg2, sd->startColumn, sd->columnLength);
}


void RexxStem::quickSort(SortData *sd, int (*comparator)(SortData *, RexxString *, RexxString *), RexxString **strings, size_t left, size_t right)
{
    size_t old_left = left;
    size_t old_right = right;

    RexxString *pivot = strings[left];     // get the pivot value

    // now find the new partitioning
    while (left < right)
    {
        // fix the right end
        while (comparator(sd, strings[right], pivot) >= 0 && (left < right))
        {
            right--;
        }
        // did we find a mismatch while testing?  then pull things in from the left too
        if (left != right)
        {
            // swap these and pull the left in
            strings[left] = strings[right];
            left++;
        }
        // now compare from the left
        while (comparator(sd, strings[left], pivot) <= 0 && (left < right))
        {
            left++;
        }
        // still not done?
        if (left != right)
        {
            // swap these two and continue
            strings[right] = strings[left];
            right--;
        }
    }

    // store the pivot value in the current left position
    strings[left] = pivot;
    // this is the new pivot point
    size_t pivotPoint = left;
    // restore the old end points
    left = old_left;
    right = old_right;
    // something to the left of the pivot?
    if (left < pivotPoint)
    {
        // sort the left partition
        quickSort(sd, comparator, strings, left, pivotPoint - 1);
    }
    // and also the right partition if we have one
    if (right > pivotPoint)
    {
        quickSort(sd, comparator, strings, pivotPoint + 1, right);
    }
}


bool RexxStem::sort(RexxString *prefix, int order, int type, size_t first, size_t last, size_t firstcol, size_t lastcol)
/******************************************************************************/
/* Function:  Sort elements of a stem variable as if it was an array.  This   */
/*            routine assumes that element ".0" of the stem contains a size   */
/*            value for the array portion of the elements, and that tail      */
/*            indices ".start" to ".end", inclusive all exist in the tail.    */
/*            Sorting will be performed on the string values of all elements, */
/*            and in the final result, all values will be replaced by the     */
/*            string values.                                                  */
/******************************************************************************/
{
    SortData sd;

    sd.startColumn = 0;
    sd.columnLength = 0;

    RexxCompoundTail stem_size(prefix, (size_t)0);
    RexxCompoundElement *size_element = findCompoundVariable(&stem_size);
    if (size_element == OREF_NULL) {
        return false;
    }
    RexxObject *size_value = size_element->getVariableValue();
    if (size_value == OREF_NULL) {
        return false;
    }

    stringsize_t count;
    /* get the integer value of this.  It must be a valid numeric */
    /* value. */
    if (!size_value->unsignedNumberValue(&count, Numerics::DEFAULT_DIGITS))
    {
        return false;
    }
    if (count == 0)         // if the count is zero, sorting is easy!
    {
        return true;
    }

    /* if this is not specified, sort to the end */
    if (last == SIZE_MAX)
    {
        last = count;
    }

    /* verify we're fully within the bounds */
    if (first > count || last > count) {
        return false;
    }
    size_t bounds = last - first + 1;

    /* get an array item and protect it.  We need to have space for both the variable anchors, and the variable values. */
    RexxArray *array = new_array(bounds * 2);
    ProtectedObject p1(array);

    size_t i;
    size_t j;
    for (j = 1, i = first; i <= last; i++, j++)
    {
        RexxCompoundTail nextStem(prefix, (size_t)i);
        RexxCompoundElement *next_element = findCompoundVariable(&nextStem);

        if (next_element == OREF_NULL) {
            return false;
        }

        RexxObject *nextValue = next_element->getVariableValue();
        if (nextValue == OREF_NULL) {
            return false;
        }
        /* force this to a string value. */
        nextValue = REQUEST_STRING(nextValue);
        /* now anchor both in the sorting array */
        array->put((RexxObject *)next_element, j);
        array->put(nextValue, j + bounds);
    }

    RexxString **aData = (RexxString **)array->data(bounds + 1);

    {
        // we're releasing kernel access during the process.  The sort is being
        // done on a locally allocated array, so this will not be accessed by another thread.
        // All the rest of the operations are thread safe.
        UnsafeBlock block;

        if ((firstcol == 0) && (lastcol == SIZE_MAX)) {
          /* no special columns to check */
          switch (type) {

              case SORT_CASESENSITIVE:
                  quickSort(&sd, order == SORT_ASCENDING ? compare_asc : compare_desc,
                      aData, 0, bounds - 1);
                  break;
              case SORT_CASEIGNORE:
                  quickSort(&sd, order == SORT_ASCENDING ? compare_asc_i : compare_desc_i,
                    aData, 0, bounds - 1);
                break;
          }
        }
        else
        {
          /* set columns to sort */
          sd.startColumn = firstcol;
          sd.columnLength = lastcol - firstcol + 1;

          switch (type)
          {
            case SORT_CASESENSITIVE:
                quickSort(&sd, order == SORT_ASCENDING ? compare_asc_cols : compare_desc_cols,
                    aData, 0, bounds - 1);
                break;
            case SORT_CASEIGNORE:
                quickSort(&sd, order == SORT_ASCENDING ? compare_asc_i_cols : compare_desc_i_cols,
                    aData, 0, bounds - 1);
                break;
          }
        }
    }


    /* The values have now been sorted.  We now need to set each */
    /* each variable back to its new value. */
    for (i = 1; i <= bounds; i++) {
        RexxCompoundElement *element = (RexxCompoundElement *)array->get(i);
        RexxObject *value = array->get(i + bounds);
        element->set(value);
    }
    return true;
}
