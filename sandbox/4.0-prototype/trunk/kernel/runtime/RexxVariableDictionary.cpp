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
/* REXX Kernel                                                  RexxVariableDictionary.c     */
/*                                                                            */
/* Primitive Variable Dictionary Class                                        */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "RexxNativeActivation.hpp"
#include "RexxActivation.hpp"
#include "RexxActivity.hpp"
#include "ArrayClass.hpp"
#include "ListClass.hpp"
#include "RexxVariableDictionary.hpp"
#include "StemClass.hpp"
#include "ExpressionStem.hpp"
#include "ExpressionVariable.hpp"
#include "ExpressionCompoundVariable.hpp"
#include "SupplierClass.hpp"

RexxObject  *RexxVariableDictionary::copy()
/******************************************************************************/
/* Function:  Copy a variable dictionary                                      */
/******************************************************************************/
{
  RexxVariableDictionary *copy;        /* copied object                     */

  /* create a new object               */
  copy = RexxVariableDictionary::newInstance(contents->mainSlotsSize());
  ProtectedObject p1(copy);
  ClearObject(copy);                   /* clear this out                    */
                                       /* copy the behaviour pointer        */
  OrefSet(copy, copy->behaviour, this->behaviour);
                                       /* copy the hash table               */
  OrefSet(copy, copy->contents, (RexxHashTable *)this->contents->copy());
  /* make sure we copy the scope too */
  OrefSet(copy, copy->scope, this->scope);
  copy->copyValues();                  /* copy all of the variables         */
  return (RexxObject *)copy;           /* return the new vdict              */
}

void RexxVariableDictionary::copyValues()
/******************************************************************************/
/* Function:  Copy all of the values in a vdict                               */
/******************************************************************************/
{
  size_t      i;                       /* loop counter                      */
  RexxObject *value;                   /* hash table value                  */
  RexxObject *copy;                    /* copied value                      */

                                       /* loop through the hash table       */
  for (i = this->contents->first();
       i < this->contents->totalSlotsSize();
       i = this->contents->next(i)) {
    value = this->contents->value(i);  /* get the next value                */
    copy = value->copy();              /* copy the value                    */
    this->contents->replace(copy, i);  /* replace with the copied value     */
  }
}


/**
 * Get all of the variables in the local context.  This returns
 * just the top-level variables (i.e., simple variables and stems).
 *
 * @return A supplier for iterating the variable sset.
 */
RexxSupplier *RexxVariableDictionary::getAllVariables()
{
    HashLink    i;                       /* loop counter                      */
    RexxVariable *variable;              /* hash table value                  */
    arraysize_t count = 0;               // the size of the supplier arrays

                                         /* loop through the hash table       */
    for (i = this->contents->first();
         i < this->contents->totalSlotsSize();
         i = this->contents->next(i))
    {
        // get the next variable from the dictionary
        variable = (RexxVariable *)this->contents->value(i);
        // if this variable has a value, bump the count
        if (variable->getVariableValue() != OREF_NULL)
        {
            count ++;
        }
    }

    RexxArray *names = new_array(count);
    ProtectedObject p1(names);
    RexxArray *values = new_array(count);
    ProtectedObject p2(values);

    count = 1;
    // now loop again populating the supplier arrays
    for (i = this->contents->first();
         i < this->contents->totalSlotsSize();
         i = this->contents->next(i))
    {
        // get the next variable from the dictionary
        variable = (RexxVariable *)this->contents->value(i);
        // if this variable has a value, bump the count
        if (variable->getVariableValue() != OREF_NULL)
        {
            // only add the real values to the list
            names->put(variable->getName(), count);
            values->put(variable->getVariableValue(), count);
            count++;
        }
    }

    return new_supplier(values, names);
}


/**
 * Bind a complete variable dictionary to the initial local
 * variable set of an activation.
 *
 * @param context The activation context.
 */
void RexxVariableDictionary::bindScriptVariables(RexxActivation *context)
{
    HashLink    i;                       /* loop counter                      */
    RexxVariable *variable;              /* hash table value                  */

                                         /* loop through the hash table       */
    for (i = this->contents->first();
         i < this->contents->totalSlotsSize();
         i = this->contents->next(i))
    {
        // get the next variable from the dictionary
        variable = (RexxVariable *)this->contents->value(i);
        // set this variable in the local context so it has an initial value.
        context->updateLocalVariable(variable);
    }
}


/**
 * Merge the variables of a local activation back into the
 * script context.  This only updates the local script context
 * with the non-bound variables in the list.
 *
 * @param contextVars
 *                  The script context's context variable list.
 * @param boundVars The script context's bound variable list, used for merge
 *                  filtering.
 */
void RexxVariableDictionary::mergeScriptVariables(RexxVariableDictionary *contextVars, RexxVariableDictionary *boundVars)
{
    HashLink    i;                       /* loop counter                      */

                                         /* loop through the hash table       */
    for (i = this->contents->first();
         i < this->contents->totalSlotsSize();
         i = this->contents->next(i))
    {
        // get the next variable from the dictionary
        RexxVariable *variable = (RexxVariable *)this->contents->value(i);
        RexxString *name = variable->getName();
        // if this is not in the bound variable list, we need to push it into the
        // context list
        if (!boundVars->hasVariable(name))
        {
            contextVars->put(variable, name);
        }
    }
}




RexxObject  *RexxVariableDictionary::realValue(
     RexxString *name)                 /* name of variable to retrieve      */
/******************************************************************************/
/* Function:  Retrieve a variable's value WITHOUT returning the default       */
/*            variable name if it doesn't exist.                              */
/******************************************************************************/
{
  RexxVariable *variable;              /* retrieved variable                */
  variable = resolveVariable(name);    /* look up the name                  */
  if (variable == OREF_NULL)           /* not found?                        */
    return OREF_NULL;                  /* say so                            */
  return variable->getVariableValue(); /* use the variable value            */
}


/**
 * Drop the value of a named variable in the method dictionary.
 *
 * @param name   The string name of the variable.
 */
void RexxVariableDictionary::drop(RexxString *name)
{
    // if the variable exists, drop the value
    RexxVariable *variable = resolveVariable(name);
    if (variable != OREF_NULL)
    {
        variable->drop();
    }
}


/**
 * Remove a variable from a variable dictionary.  This completely
 * removes the variable, so this is more than a drop operation.
 * The variable is handled as a drop (since it might be in use
 * in the current context), but additionally, the variable object
 * is removed from this dictionary.
 *
 * @param name   The target variable name.
 */
void RexxVariableDictionary::remove(RexxString *name)
{
    drop(name);
    removeVariable(name);

}



RexxCompoundElement *RexxVariableDictionary::getCompoundVariable(
     RexxString *stem,                 /* name of stem for compound         */
     RexxObject **tail,                /* tail of the compound element      */
     size_t      tailCount)            /* number of tail pieces             */
/******************************************************************************/
/* Function:  Retrieve a compound variable, returning OREF_NULL if the        */
/*            variable does not exist.                                        */
/******************************************************************************/
{
  RexxStem     *stem_table;            /* retrieved stem table              */
                                       /* new tail for compound             */
  RexxCompoundTail resolved_tail(this, tail, tailCount);

  stem_table = getStem(stem);          /* get the stem entry from this dictionary */
                                       /* get the compound variable         */
  return stem_table->getCompoundVariable(&resolved_tail);
}


RexxObject *RexxVariableDictionary::getCompoundVariableValue(
     RexxString *stem,                 /* name of stem for compound         */
     RexxObject **tail,                /* tail of the compound element      */
     size_t      tailCount)            /* number of tail pieces             */
/******************************************************************************/
/* Function:  Retrieve a compound variable, returning default value if the    */
/*            variable does not exist.  This does not raise NOVALUE.          */
/******************************************************************************/
{
  RexxStem     *stem_table;            /* retrieved stem table              */
                                       /* new tail for compound             */
  RexxCompoundTail resolved_tail(this, tail, tailCount);

  stem_table = getStem(stem);          /* get the stem entry from this dictionary */
                                       /* get the value from the stem...we pass OREF_NULL */
                                       /* for the dictionary to bypass NOVALUE handling */
  return stem_table->evaluateCompoundVariableValue(OREF_NULL, &resolved_tail);
}


/**
 * Retrieve the "real" value of a compound variable.  This
 * return OREF_NULL for any situation where the compound variable
 * name would be returned.
 *
 * @param stem      The name of the stem.
 * @param tail      The set of tails used for the lookup.
 * @param tailCount The number of tail elements.
 *
 * @return Either the variable value, or OREF_NULL for unassigned
 *         variables.
 */
RexxObject *RexxVariableDictionary::getCompoundVariableRealValue(RexxString *stem,
     RexxObject **tail, size_t tailCount)
{
  RexxStem     *stem_table;            /* retrieved stem table              */
                                       /* new tail for compound             */
  RexxCompoundTail resolved_tail(this, tail, tailCount);

  stem_table = getStem(stem);          /* get the stem entry from this dictionary */
                                       /* get the value from the stem...we pass OREF_NULL */
                                       /* for the dictionary to bypass NOVALUE handling */
  return stem_table->getCompoundVariableRealValue(&resolved_tail);
}


RexxObject  *RexxVariableDictionary::realStemValue(
     RexxString *stem)                 /* name of stem for compound         */
/******************************************************************************/
/* Function:  Retrieve the "real" value of a stem variable.  OREF_NULL is     */
/*            returned if the stem does not exist.                            */
/******************************************************************************/
{
                                       /* look up the name                  */
  return this->getStem(stem);          /* find and return the stem directly */
}


void RexxVariableDictionary::add(
     RexxVariable *variable,           /* new variable entry                */
     RexxString   *name)               /* variable name                     */
/******************************************************************************/
/* Function:  Insert an item into the variable dictionary hash table,         */
/*            updating the lookaside array if the index is available.         */
/******************************************************************************/
{
  RexxHashTable *new_hash;             /* new hash table                    */
                                       /* try to place in existing hashtab  */
  new_hash = this->contents->stringAdd((RexxObject *)variable, name);
  if (new_hash != OREF_NULL)           /* have a reallocation occur?        */
                                       /* hook on the new hash table        */
    OrefSet(this, this->contents, new_hash);
}

void RexxVariableDictionary::put(
     RexxVariable *variable,           /* new variable entry                */
     RexxString   *name)               /* variable name                     */
/******************************************************************************/
/* Function:  Insert an item into the variable dictionary hash table,         */
/*            updating the lookaside array if the index is available.         */
/******************************************************************************/
{
  RexxHashTable *new_hash;             /* new hash table                    */
                                       /* try to place in existing hashtab  */
  new_hash = this->contents->stringPut((RexxObject *)variable, name);
  if (new_hash != OREF_NULL)           /* have a reallocation occur?        */
                                       /* hook on the new hash table        */
    OrefSet(this, this->contents, new_hash);
}


RexxVariable *RexxVariableDictionary::createStemVariable(
     RexxString *stem)                 /* name of target stem               */
/******************************************************************************/
/* Function:  Lookup and retrieve a STEM variable item (not the stem table)   */
/*            level)                                                          */
/******************************************************************************/
{
  RexxStem     *stemtable;             /* retrieved stem dictionary         */
  RexxVariable *variable;              /* resolved variable item            */
  RexxHashTable *new_hash;             /* reallocated hash table            */

  variable =  RexxVariable::newInstance(stem);    /* make a new variable entry         */
  stemtable = new RexxStem (stem);   /* create a stem object as value     */
                                     /* the stem object is the value of   */
                                     /* stem variable                     */
  variable->set((RexxObject *)stemtable);
                                     /* try to place in existing hashtab  */
  new_hash = this->contents->stringAdd((RexxObject *)variable, stem);
  if (new_hash != OREF_NULL)         /* have a reallocation occur?        */
                                     /* hook on the new hash table        */
    OrefSet(this, this->contents, new_hash);
  return variable;                     /* return the stem                   */
}


RexxVariable  *RexxVariableDictionary::createVariable(
     RexxString *name)                 /* name of target variable           */
/******************************************************************************/
/* Function:  Create a new variable item and add it to the dictionary.        */
/******************************************************************************/
{
  RexxVariable *variable;              /* resolved variable item            */
  RexxHashTable *new_hash;             /* reallocated hash table            */

  variable =  RexxVariable::newInstance(name);  /* make a new variable entry         */
                                       /* try to place in existing hashtab  */
  new_hash = this->contents->stringAdd((RexxObject *)variable, name);
  if (new_hash != OREF_NULL)           /* have a reallocation occur?        */
                                       /* hook on the new hash table        */
    OrefSet(this, this->contents, new_hash);
  return variable;                     /* return the stem                   */
}

RexxVariable *RexxVariableDictionary::nextVariable(
  RexxNativeActivation *activation)    /* Hosting Native Act.               */
/******************************************************************************/
/* Function:  Return the "next" variable of a variable traversal              */
/******************************************************************************/
{
  RexxVariable *variable;              /* variable entry                    */
  RexxObject *value;                   /* variable value                    */

  if (activation->nextVariable() == -1)/* first time through?               */
                                       /* get the first item                */
    activation->setNextVariable(this->contents->first());
  else                                 /* step to the next index item       */
    activation->setNextVariable(this->contents->next(activation->nextVariable()));
                                       /* while more directory entries      */
  while (this->contents->index(activation->nextVariable()) != OREF_NULL) {
                                       /* get the variable object           */
    variable = (RexxVariable *)this->contents->value(activation->nextVariable());
                                       /* get the value                     */
    value = variable->getVariableValue();
    if (value != OREF_NULL) {          /* not a dropped variable?           */
        return variable;               /* got what we need                  */
    }
                                       /* step to the next index item       */
    activation->setNextVariable(this->contents->next(activation->nextVariable()));
  }
  activation->setNextVariable(-1);     /* reset the index for the end       */
  return OREF_NULL;
}

void RexxVariableDictionary::set(
     RexxString *name,                 /* name to set                       */
     RexxObject *value)                /* value to assign to variable name  */
/******************************************************************************/
/* Function:  Set a new variable value                                        */
/******************************************************************************/
{
  RexxVariable *variable;              /* retrieved variable item           */

  variable = getVariable(name);        /* look up the name                  */
  variable->set(value);                /* and perform the set               */
}

void RexxVariableDictionary::reserve(
  RexxActivity *activity)              /* reserving activity                */
/******************************************************************************/
/* Function:  Reserve a scope on an object, waiting for completion if this    */
/*            is already reserved by another activity                         */
/******************************************************************************/
{
                                       /* currently unlocked?               */
  if (this->reservingActivity == OREF_NULL) {
                                       /* set the locker                    */
    OrefSet(this, this->reservingActivity, activity);
    this->reserveCount = 1;            /* we're reserved once               */
  }
                                       /* doing again on the same stack?    */
  else if (this->reservingActivity == activity)
    this->reserveCount++;              /* bump the nesting count            */
  else {                               /* need to wait on this              */
                                       /* go perform dead lock checks       */
    this->reservingActivity->checkDeadLock(activity);
                                       /* no list here?                     */
    if (this->waitingActivities == OREF_NULL)
                                       /* get a waiting queue               */
      OrefSet(this, this->waitingActivities, new_list());
                                       /* add to the wait queue             */
    this->waitingActivities->addLast((RexxObject *)activity);
                                       /* ok, now we wait                   */
    activity->waitReserve((RexxObject *)this);
  }
}

void RexxVariableDictionary::release(
  RexxActivity *activity)              /* reserving activity                */
/******************************************************************************/
/* Function:  Release the lock on an object's ovd                             */
/******************************************************************************/
{
  RexxActivity *newActivity;           /* new reserving activity            */

  this->reserveCount--;                /* decrement the reserving count     */
  if (this->reserveCount == 0) {       /* last one for this activity?       */
                                       /* remove the current reserver       */
    OrefSet(this, this->reservingActivity, OREF_NULL);
                                       /* have things waiting?              */
    if (this->waitingActivities != OREF_NULL) {
                                       /* get the next one                  */
      newActivity = (RexxActivity *)this->waitingActivities->removeFirst();
                                       /* have a real one here?             */
      if (newActivity != (RexxActivity *)TheNilObject) {
                                       /* this is the new owner             */
        OrefSet(this, this->reservingActivity, newActivity);
        this->reserveCount = 1;        /* back to one lock again            */
                                       /* wake up the waiting activity      */
        newActivity->postRelease();
      }
    }
  }
}


bool RexxVariableDictionary::transfer(
  RexxActivity *activity)              /* new reserving activity            */
/******************************************************************************/
/* Function:  Transfer a vdict lock to another activity                       */
/******************************************************************************/
{
  if (this->reserveCount == 1) {       /* only one level of nesting?        */
                                       /* easy, just switch the owner       */
    OrefSet(this, this->reservingActivity, activity);
    return true;                       /* say this worked                   */
  }
  else {                               /* multiple nesting levels           */
    this->release(activity);           /* release this lock                 */
    return false;                      /* can't do this                     */
  }
}


void RexxVariableDictionary::setNextDictionary(RexxVariableDictionary *next)
/******************************************************************************/
/* Function:  Chain up a dictionary associated with an object                 */
/******************************************************************************/
{
    OrefSet(this, this->next, next);
}

void RexxVariableDictionary::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->contents);
  memory_mark(this->reservingActivity);
  memory_mark(this->waitingActivities);
  memory_mark(this->next);
  memory_mark(this->scope);
  cleanUpMemoryMark
}

void RexxVariableDictionary::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->contents);
  memory_mark_general(this->reservingActivity);
  memory_mark_general(this->waitingActivities);
  memory_mark_general(this->next);
  memory_mark_general(this->scope);
  cleanUpMemoryMarkGeneral
}

void RexxVariableDictionary::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxVariableDictionary)

   flatten_reference(newThis->contents, envelope);
   flatten_reference(newThis->reservingActivity, envelope);
   flatten_reference(newThis->waitingActivities, envelope);
   flatten_reference(newThis->next, envelope);
   flatten_reference(newThis->scope, envelope);
  cleanUpFlatten
}


RexxVariableDictionary *RexxVariableDictionary::newInstance(
     size_t looksize)                  /* expected size of the vdict        */
/******************************************************************************/
/* Function:  Create a new translator object                                  */
/******************************************************************************/
{
  RexxVariableDictionary *newObject;   /* newly created object              */
  size_t hashTabSize;                  /* size of hash table to allocate    */

  hashTabSize = looksize * 2;          /* create entries for twice size     */
                                       /* Get new object                    */
                                       /* NOTE:  there is one extra         */
                                       /* lookaside element allocated,      */
                                       /* which is used for non-lookaside   */
                                       /* lookups.  Using this extra element*/
                                       /* (which is always NULL), allows    */
                                       /* some special optimization of the  */
                                       /* look ups                          */
                                       /* get a new object and hash         */
  newObject = (RexxVariableDictionary *)newHashCollection(hashTabSize, sizeof(RexxVariableDictionary));
                                       /* Give new object its behaviour     */
  BehaviourSet(newObject, TheVariableDictionaryBehaviour);
                                       /* set the virtual function table    */
  setVirtualFunctions(newObject, T_VariableDictionary);
  return newObject;                    /* return the new vdict              */
}


RexxVariableDictionary *RexxVariableDictionary::newInstance(
     RexxObject *scope)                /* expected size of the vdict        */
/******************************************************************************/
/* Function:  Create a new translator object                                  */
/******************************************************************************/
{
  RexxVariableDictionary *newObject;   /* newly created object              */
  size_t hashTabSize;                  /* size of hash table to allocate    */

  /* create entries for twice size     */
  hashTabSize = DEFAULT_OBJECT_DICTIONARY_SIZE * 2;
                                       /* Get new object                    */
                                       /* NOTE:  there is one extra         */
                                       /* lookaside element allocated,      */
                                       /* which is used for non-lookaside   */
                                       /* lookups.  Using this extra element*/
                                       /* (which is always NULL), allows    */
                                       /* some special optimization of the  */
                                       /* look ups                          */
                                       /* get a new object and hash         */
  newObject = (RexxVariableDictionary *)newHashCollection(hashTabSize, sizeof(RexxVariableDictionary));
  newObject->scope = scope;            /* fill in the scope */
                                       /* Give new object its behaviour     */
  BehaviourSet(newObject, TheVariableDictionaryBehaviour);
                                       /* set the virtual function table    */
  setVirtualFunctions(newObject, T_VariableDictionary);
  return newObject;                    /* return the new vdict              */
}
