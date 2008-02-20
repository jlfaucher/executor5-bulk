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
/* REXX API support                                                           */
/*                                                                            */
/* Stub functions for all APIs accessed via the NativeMethodContext           */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#include "ContextApi.hpp"
#include "RexxActivity.hpp"
#include "StringClass.hpp"
#include "IntegerClass.hpp"
#include "BufferClass.hpp"
#include "SupplierClass.hpp"
#include "ArrayClass.hpp"
#include "DirectoryClass.hpp"
#include "TableClass.hpp"
#include "MethodClass.hpp"
#include "RoutineClass.hpp"
#include "PackageClass.hpp"
#include "Interpreter.hpp"
#include "InterpreterInstance.hpp"
#include "SystemInterpreter.hpp"
#include "PointerClass.hpp"
#include "ActivityManager.hpp"
#include "RexxStartDispatcher.hpp"
#include "PackageManager.hpp"

BEGIN_EXTERN_C()

void RexxEntry DetachThread(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        context.activity->detachInstance();
    }
    catch (ActivityException)
    {
    }
}


/**
 * API stub for raising a halt condition on a thread.
 */
void RexxEntry HaltThread(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        context.activity->halt(OREF_NULL);
    }
    catch (ActivityException)
    {
    }
}


void RexxEntry SetThreadTrace(RexxThreadContext *c, logical_t setting)
{
    ApiContext context(c);
    try
    {
        context.activity->setTrace(setting == TRUE);
    }
    catch (ActivityException)
    {
    }
}


RexxObjectPtr RexxEntry RequestGlobalReference(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        context.activity->getInstance()->addGlobalReference((RexxObject *)o);
        return o;
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


void RexxEntry ReleaseGlobalReference(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        context.activity->getInstance()->removeGlobalReference((RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
}


void RexxEntry ReleaseLocalReference(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        context.context->removeLocalReference((RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
}

//NB:  The name "SendMessage" has a conflict with a Windows API, so this name differs from
// the call vector version.
RexxObjectPtr RexxEntry SendMessageArray(RexxThreadContext *c, RexxObjectPtr o, CSTRING m, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxObject *)o)->sendMessage(new_upper_string(m), (RexxArray *)a));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxObjectPtr RexxEntry SendMessage0(RexxThreadContext *c, RexxObjectPtr o, CSTRING m)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxObject *)o)->sendMessage(new_upper_string(m)));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxObjectPtr RexxEntry SendMessage1(RexxThreadContext *c, RexxObjectPtr o, CSTRING m, RexxObjectPtr a1)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxObject *)o)->sendMessage(new_upper_string(m), (RexxObject *)a1));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry SendMessage2(RexxThreadContext *c, RexxObjectPtr o, CSTRING m, RexxObjectPtr a1, RexxObjectPtr a2)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxObject *)o)->sendMessage(new_upper_string(m), (RexxObject *)a1, (RexxObject *)a2));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxDirectoryObject RexxEntry GetLocalEnvironment(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)ActivityManager::localEnvironment;
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxDirectoryObject RexxEntry GetGlobalEnvironment(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)TheEnvironment;
    }
    catch (ActivityException)
    {

    }
    return NULLOBJECT;
}


logical_t RexxEntry IsSameType(RexxThreadContext *c, RexxObjectPtr o1, RexxObjectPtr o2)
{
    ApiContext context(c);
    try
    {
        return ((RexxObject *)o1)->isSameType((RexxObject *)o2);
    }
    catch (ActivityException)
    {
    }
    return 0;
}


logical_t RexxEntry IsInstanceOf(RexxThreadContext *c, RexxObjectPtr o, RexxClassObject cl)
{
    ApiContext context(c);
    try
    {
        return ((RexxObject *)o)->isInstanceOf((RexxClass *)cl);
    }
    catch (ActivityException)
    {
    }
    return 0;
}


logical_t RexxEntry HasMethod(RexxThreadContext *c, RexxObjectPtr o, CSTRING n)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        return ((RexxObject *)o)->hasMethod(new_string(n)) == TheTrueObject;

    }
    catch (ActivityException)
    {
    }
    return 0;
}


RexxPackageObject RexxEntry LoadPackage(RexxThreadContext *c, CSTRING n)
{
    ApiContext context(c);
    try
    {
        ProtectedObject package;
        RexxString *name = new_string(n);
        RexxString *resolvedName = context.activity->getInstance()->resolveProgramName(name, OREF_NULL, OREF_NULL);

        // convert the name to a string instance, and check the environments.
        return (RexxPackageObject)context.ret(PackageManager::loadRequires(context.activity, name, resolvedName, package));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxPackageObject RexxEntry LoadPackageFromData(RexxThreadContext *c, CSTRING n, CSTRING d, size_t l)
{
    ApiContext context(c);
    try
    {
        ProtectedObject package;
        return (RexxPackageObject)PackageManager::loadRequires(context.activity, new_string(n), d, l, package);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxClassObject RexxEntry FindClass(RexxThreadContext *c, CSTRING n)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        RexxString *name = new_upper_string(n);
        return (RexxClassObject)context.ret(context.context->resolveClass(name));

    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxClassObject RexxEntry FindClassFromPackage(RexxThreadContext *c, RexxPackageObject m, CSTRING n)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        RexxString *name = new_upper_string(n);
        return (RexxClassObject)context.ret(((PackageClass *)m)->resolveClass(name));

    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxDirectoryObject RexxEntry GetPackageRoutines(RexxThreadContext *c, RexxPackageObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.ret(((PackageClass *)m)->getRoutines());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxDirectoryObject RexxEntry GetPackageClasses(RexxThreadContext *c, RexxPackageObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.ret(((PackageClass *)m)->getMethods());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxDirectoryObject RexxEntry GetPackageMethods(RexxThreadContext *c, RexxPackageObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.ret(((PackageClass *)m)->getClasses());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxObjectPtr RexxEntry CallRoutine(RexxThreadContext *c, RexxRoutineObject r, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        CallRoutineDispatcher dispatcher((RoutineClass *)r, (RexxArray *)a);
        context.activity->run(dispatcher);
        return (RexxObjectPtr)dispatcher.result;
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxObjectPtr RexxEntry CallProgram(RexxThreadContext *c, const char * p, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        CallProgramDispatcher dispatcher(p, (RexxArray *)a);
        context.activity->run(dispatcher);
        return (RexxObjectPtr)dispatcher.result;
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxMethodObject RexxEntry NewMethod(RexxThreadContext *c, CSTRING name, CSTRING source, stringsize_t length)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        return (RexxMethodObject)context.ret(RexxMethod::newRexxBuffer(name, source, length));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxRoutineObject RexxEntry NewRoutine(RexxThreadContext *c, CSTRING name, CSTRING source, stringsize_t length)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        return (RexxRoutineObject)context.ret(RoutineClass::newRexxBuffer(new_string(name), source, length));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxBufferObject RexxEntry SaveRoutine(RexxThreadContext *c, RexxRoutineObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxBufferObject)context.ret(((RoutineClass *)m)->save());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry NewObject(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        return context.ret(new (TheObjectClass) RexxObject());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry NumberToObject(RexxThreadContext *c, wholenumber_t n)
{
    ApiContext context(c);
    try
    {
        return context.ret(Numerics::toObject((wholenumber_t)n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry UintptrToObject(RexxThreadContext *c, uintptr_t n)
{
    ApiContext context(c);
    try
    {
        return context.ret(Numerics::ptrToObject(n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxObjectPtr RexxEntry ValueToObject(RexxThreadContext *c, ValueDescriptor *d)
{
    ApiContext context(c);
    try
    {
        return (RexxObjectPtr)context.ret(context.context->valueToObject(d));
    }
    catch (ActivityException)
    {
        context.context->setConditionInfo(OREF_NULL);
    }
    return NULLOBJECT;
}


RexxArrayObject RexxEntry ValuesToObject(RexxThreadContext *c, ValueDescriptor *d, size_t count)
{
    ApiContext context(c);
    try
    {
        return (RexxArrayObject)context.ret(context.context->valuesToObject(d, count));
    }
    catch (ActivityException)
    {
        context.context->setConditionInfo(OREF_NULL);
    }
    return NULLOBJECT;
}


logical_t RexxEntry ObjectToValue(RexxThreadContext *c, RexxObjectPtr o, ValueDescriptor *d)
{
    ApiContext context(c);
    try
    {
        return context.context->objectToValue((RexxObject *)o, d);
    }
    catch (ActivityException)
    {
        // some conversion failures result in an exception...cancel that, and
        // just return FALSE;
        context.context->setConditionInfo(OREF_NULL);
    }
    return FALSE;
}

RexxObjectPtr RexxEntry UnsignedNumberToObject(RexxThreadContext *c, stringsize_t n)
{
    ApiContext context(c);
    try
    {
        return context.ret(Numerics::toObject((stringsize_t)n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


logical_t RexxEntry ObjectToNumber(RexxThreadContext *c, RexxObjectPtr o, wholenumber_t *n)
{
    ApiContext context(c);
    try
    {
        wholenumber_t temp;
        // this uses the entire value range
        if (Numerics::objectToWholeNumber((RexxObject *)o, temp, SSIZE_MAX, SSIZE_MIN))
        {
            *n = (wholenumber_t)temp;
            return true;
        }
        return false;
    }
    catch (ActivityException)
    {
    }
    return 0;
}


logical_t RexxEntry ObjectToUnsignedNumber(RexxThreadContext * c, RexxObjectPtr o, stringsize_t * n)
{
    ApiContext context(c);
    try
    {
        stringsize_t temp;
        // this uses the entire value range
        if (Numerics::objectToStringSize((RexxObject *)o, temp, SIZE_MAX))
        {
            *n = (stringsize_t)temp;
            return true;
        }
        return false;
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxObjectPtr RexxEntry Int64ToObject(RexxThreadContext *c, int64_t n)
{
    ApiContext context(c);
    try
    {
        return context.ret(Numerics::toObject(n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry UnsignedInt64ToObject(RexxThreadContext * c, uint64_t n)
{
    ApiContext context(c);
    try
    {
        return context.ret(Numerics::toObject(n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry ObjectToInt64(RexxThreadContext *c, RexxObjectPtr o, int64_t * n)
{
    ApiContext context(c);
    try
    {
        // this uses the entire value range
        return Numerics::objectToInt64((RexxObject *)o, *n);
    }
    catch (ActivityException)
    {
    }
    return 0;
}

logical_t RexxEntry ObjectToUnsignedInt64(RexxThreadContext *c, RexxObjectPtr o, uint64_t *n)
{
    ApiContext context(c);
    try
    {
        // this uses the entire value range
        return Numerics::objectToUnsignedInt64((RexxObject *)o, *n);
    }
    catch (ActivityException)
    {
    }
    return 0;
}

logical_t RexxEntry ObjectToUintptr(RexxThreadContext * c, RexxObjectPtr o, uintptr_t * n)
{
    ApiContext context(c);
    try
    {
        // this uses the entire value range
        return Numerics::objectToUintptr((RexxObject *)o, *n);
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxObjectPtr RexxEntry DoubleToObject(RexxThreadContext *c, double n)
{
    ApiContext context(c);
    try
    {
        return context.ret(new_numberstring(n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry DoubleToObjectWithPrecision(RexxThreadContext *c, double n, size_t precision)
{
    ApiContext context(c);
    try
    {
        return context.ret(new_string(n, precision));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry ObjectToDouble(RexxThreadContext *c, RexxObjectPtr o, double *n)
{
    ApiContext context(c);
    try
    {
        return ((RexxObject *)o)->doubleValue(*n);
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxStringObject RexxEntry ObjectToString(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return (RexxStringObject)context.ret(REQUEST_STRING((RexxObject *)o));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

CSTRING RexxEntry ObjectToStringValue(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = REQUEST_STRING((RexxObject *)o);
        context.ret(temp);
        return (CSTRING)temp->getStringData();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

size_t RexxEntry StringGet(RexxThreadContext *c, RexxStringObject s, size_t o, POINTER r, size_t l)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return temp->get(o, (char *)r, l);
    }
    catch (ActivityException)
    {
    }
    return 0;
}

size_t RexxEntry StringLength(RexxThreadContext *c, RexxStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return temp->getLength();
    }
    catch (ActivityException)
    {
    }
    return 0;
}

CSTRING RexxEntry StringData(RexxThreadContext *c, RexxStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return (CSTRING)temp->getStringData();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

RexxStringObject RexxEntry NewString(RexxThreadContext *c, CSTRING s, size_t l)
{
    ApiContext context(c);
    try
    {
        return (RexxStringObject)context.ret(new_string(s, (stringsize_t)l));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxStringObject RexxEntry NewStringFromAsciiz(RexxThreadContext *c, CSTRING s)
{
    ApiContext context(c);
    try
    {
        return (RexxStringObject)context.ret(new_string((char *)s));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxStringObject RexxEntry StringUpper(RexxThreadContext *c, RexxStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return (RexxStringObject)context.ret(temp->upper());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxStringObject RexxEntry StringLower(RexxThreadContext *c, RexxStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return (RexxStringObject)context.ret(temp->lower());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry IsString(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isString((RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxBufferStringObject RexxEntry NewBufferString(RexxThreadContext * c, size_t l)
{
    ApiContext context(c);
    try
    {
        return (RexxBufferStringObject)context.ret(raw_string((stringsize_t)l));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

size_t  RexxEntry BufferStringLength(RexxThreadContext *c, RexxBufferStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return temp->getLength();
    }
    catch (ActivityException)
    {
    }
    return 0;
}

POINTER RexxEntry BufferStringData(RexxThreadContext *c, RexxBufferStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return (POINTER)temp->getWritableData();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

RexxStringObject RexxEntry FinishBufferString(RexxThreadContext *c, RexxBufferStringObject s, size_t l)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        temp->finish(l);
        return s;
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

void  RexxEntry TablePut(RexxThreadContext *c, RexxTableObject t, RexxObjectPtr o, RexxObjectPtr i)
{
    ApiContext context(c);
    try
    {
        ((RexxTable *)t)->put((RexxObject *)o, (RexxObject *)i);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry TableGet(RexxThreadContext *c, RexxTableObject t, RexxObjectPtr i)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxTable *)t)->get((RexxObject *)i));
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

RexxObjectPtr RexxEntry TableRemove(RexxThreadContext *c, RexxTableObject t, RexxObjectPtr i)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxTable *)t)->remove((RexxObject *)i));
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

RexxTableObject RexxEntry NewTable(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxTableObject)context.ret(new_table());
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

logical_t RexxEntry IsTable(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isOfClass(Table, (RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

void  RexxEntry DirectoryPut(RexxThreadContext *c, RexxDirectoryObject t, RexxObjectPtr o, CSTRING i)
{
    ApiContext context(c);
    try
    {
        ((RexxDirectory *)t)->put((RexxObject *)o, new_string(i));
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry DirectoryAt(RexxThreadContext *c, RexxDirectoryObject t, CSTRING i)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxDirectory *)t)->at(new_string(i)));
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

RexxObjectPtr RexxEntry DirectoryRemove(RexxThreadContext *c, RexxDirectoryObject t, CSTRING i)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxDirectory *)t)->remove(new_string(i)));
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

RexxDirectoryObject RexxEntry NewDirectory(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.ret(new_table());
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

logical_t RexxEntry IsDirectory(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isOfClass(Directory, (RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

RexxObjectPtr RexxEntry ArrayAt(RexxThreadContext *c, RexxArrayObject a, size_t i)
{
    ApiContext context(c);
    try
    {
        return (RexxObjectPtr)context.ret(((RexxArray *)a)->getApi(i));
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

logical_t RexxEntry ArrayHasIndex(RexxThreadContext *c, RexxArrayObject a, size_t i)
{
    ApiContext context(c);
    try
    {
        return ((RexxArray *)a)->hasIndexApi(i);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

void RexxEntry ArrayPut(RexxThreadContext *c, RexxArrayObject a, RexxObjectPtr o, size_t i)
{
    ApiContext context(c);
    try
    {
        ((RexxArray *)a)->putApi((RexxObject *)o, i);
    }
    catch (ActivityException)
    {
    }
}

size_t RexxEntry ArraySize(RexxThreadContext *c, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        return ((RexxArray *)a)->size();
    }
    catch (ActivityException)
    {
    }
    return 0;
}

wholenumber_t RexxEntry ArrayDimension(RexxThreadContext *c, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        return ((RexxArray *)a)->getDimension();
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxArrayObject RexxEntry NewArray(RexxThreadContext *c, size_t s)
{
    ApiContext context(c);
    try
    {
        return (RexxArrayObject)context.ret(new_array(s));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxArrayObject RexxEntry ArrayOfOne(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return (RexxArrayObject)context.ret(new_array((RexxObject *)o));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxArrayObject RexxEntry ArrayOfTwo(RexxThreadContext *c, RexxObjectPtr o1, RexxObjectPtr o2)
{
    ApiContext context(c);
    try
    {
        return (RexxArrayObject)context.ret(new_array((RexxObject *)o1, (RexxObject *)o2));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


logical_t RexxEntry IsArray(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isArray((RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

CSTRING RexxEntry BufferData(RexxThreadContext *c, RexxBufferObject b)
{
    ApiContext context(c);
    try
    {
        return (CSTRING)((RexxBuffer *)b)->getData();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

size_t RexxEntry BufferLength(RexxThreadContext *c, RexxBufferObject b)
{
    ApiContext context(c);
    try
    {
        return ((RexxBuffer *)b)->getLength();
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxBufferObject RexxEntry NewBuffer(RexxThreadContext *c, size_t l)
{
    ApiContext context(c);
    try
    {
        return (RexxBufferObject)context.ret((RexxObject *)new_buffer(l));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry IsBuffer(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isOfClass(Buffer, (RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

wholenumber_t RexxEntry IntegerValue(RexxThreadContext *c, RexxIntegerObject o)
{
    ApiContext context(c);
    try
    {
        return ((RexxInteger *)o)->wholeNumber();
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxIntegerObject RexxEntry NewInteger(RexxThreadContext *c, wholenumber_t n)
{
    ApiContext context(c);
    try
    {
        return (RexxIntegerObject)context.ret(new_integer(n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry IsInteger(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isInteger((RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

POINTER RexxEntry PointerValue(RexxThreadContext *c, RexxPointerObject o)
{
    ApiContext context(c);
    try
    {
        return (POINTER)((RexxPointer *)o)->pointer();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

RexxPointerObject RexxEntry NewPointer(RexxThreadContext *c, POINTER p)
{
    ApiContext context(c);
    try
    {
        return (RexxPointerObject)context.ret(new_pointer(p));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry IsPointer(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isOfClass(Pointer, (RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

RexxObjectPtr RexxEntry SupplierValue(RexxThreadContext *c, RexxSupplierObject o)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxSupplier *)o)->value());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry SupplierIndex(RexxThreadContext *c, RexxSupplierObject o)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxSupplier *)o)->index());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry SupplierAvailable(RexxThreadContext *c, RexxSupplierObject o)
{
    ApiContext context(c);
    try
    {
        return ((RexxSupplier *)o)->available() == TheTrueObject;
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

void RexxEntry SupplierNext(RexxThreadContext *c, RexxSupplierObject o)
{
    ApiContext context(c);
    try
    {
        ((RexxSupplier *)o)->next();
    }
    catch (ActivityException)
    {
    }
}

RexxSupplierObject RexxEntry NewSupplier(RexxThreadContext *c, RexxArrayObject values, RexxArrayObject names)
{
    ApiContext context(c);
    try
    {
        return (RexxSupplierObject)context.ret(new_supplier((RexxArray *)values, (RexxArray *)names));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry SetStemElement(RexxThreadContext *c, RexxStemObject s, CSTRING n, RexxObjectPtr v)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->setElement(n, (RexxObject *)v);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry GetStemElement(RexxThreadContext *c, RexxStemObject s, CSTRING n)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxStem *)s)->getElement(n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry DropStemElement(RexxThreadContext *c, RexxStemObject s, CSTRING n)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->dropElement(n);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry SetStemArrayElement(RexxThreadContext *c, RexxStemObject s, size_t i, RexxObjectPtr v)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->setElement((size_t )i, (RexxObject *)v);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry GetStemArrayElement(RexxThreadContext *c, RexxStemObject s, size_t i)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxStem *)s)->getElement((size_t)i));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry DropStemArrayElement(RexxThreadContext *c, RexxStemObject s, size_t i)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->dropElement((size_t)i);
    }
    catch (ActivityException)
    {
    }
}

RexxSupplierObject RexxEntry GetAllStemElements(RexxThreadContext *c, RexxStemObject s)
{
    ApiContext context(c);
    try
    {
        return (RexxSupplierObject)context.ret(((RexxStem *)s)->supplier());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry GetStemValue(RexxThreadContext *c, RexxStemObject s)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxStem *)s)->getStemValue());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

logical_t RexxEntry IsStem(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        return isOfClass(Stem, (RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

void RexxEntry RaiseException0(RexxThreadContext *c, size_t n)
{
    ApiContext context(c);
    try
    {
        reportException((wholenumber_t)n);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry RaiseException1(RexxThreadContext *c, size_t n, RexxObjectPtr o1)
{
    ApiContext context(c);
    try
    {
        reportException((wholenumber_t)n, (RexxObject *)o1);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry RaiseException2(RexxThreadContext *c, size_t n, RexxObjectPtr o1, RexxObjectPtr o2)
{
    ApiContext context(c);
    try
    {
        reportException((wholenumber_t)n, (RexxObject *)o1, (RexxObject *)o2);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry RaiseExceptionArray(RexxThreadContext *c, size_t n, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        reportException((wholenumber_t)n, (RexxArray *)a);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry RaiseCondition(RexxThreadContext *c, CSTRING name, CSTRING desc, RexxArrayObject add, RexxObjectPtr result)
{
    ApiContext context(c);
    try
    {
        context.activity->raiseCondition(new_upper_string(name), OREF_NULL, new_string(desc),
            (RexxArray *)add, (RexxObject *)result, OREF_NULL);
    }
    catch (ActivityException)
    {
    }
}

logical_t RexxEntry CheckCondition(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        return context.context->getConditionInfo() != OREF_NULL;
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

RexxDirectoryObject RexxEntry GetConditionInfo(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.context->getConditionInfo();
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}


void RexxEntry DecodeConditionInfo(RexxThreadContext *c, RexxDirectoryObject d, RexxCondition *cd)
{
    ApiContext context(c);
    try
    {
        Interpreter::decodeConditionData((RexxDirectory *)d, cd);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry ClearCondition(RexxThreadContext *c)
{
    ApiContext context(c);
    try
    {
        context.context->setConditionInfo(OREF_NULL);
    }
    catch (ActivityException)
    {
    }
}

END_EXTERN_C()

RexxThreadInterface RexxActivity::threadContextFunctions =
{
    THREAD_INTERFACE_VERSION,
    DetachThread,
    HaltThread,
    SetThreadTrace,
    RequestGlobalReference,
    ReleaseGlobalReference,
    ReleaseLocalReference,

    SendMessageArray,
    SendMessage0,
    SendMessage1,
    SendMessage2,

    GetLocalEnvironment,
    GetGlobalEnvironment,

    IsSameType,
    IsInstanceOf,
    HasMethod,
    LoadPackage,
    LoadPackageFromData,
    FindClass,
    FindClassFromPackage,
    GetPackageRoutines,
    GetPackageClasses,
    GetPackageMethods,
    CallRoutine,
    CallProgram,

    NewMethod,
    NewRoutine,
    SaveRoutine,

    NewObject,
    NumberToObject,
    UintptrToObject,
    ValueToObject,
    ValuesToObject,
    ObjectToValue,
    UnsignedNumberToObject,
    ObjectToNumber,
    ObjectToUnsignedNumber,
    Int64ToObject,
    UnsignedInt64ToObject,
    ObjectToInt64,
    ObjectToUnsignedInt64,
    ObjectToUintptr,
    DoubleToObject,
    DoubleToObjectWithPrecision,
    ObjectToDouble,

    ObjectToString,
    ObjectToStringValue,
    StringGet,
    StringLength,
    StringData,
    NewString,
    NewStringFromAsciiz,
    StringUpper,
    StringLower,
    IsString,

    NewBufferString,
    BufferStringLength,
    BufferStringData,
    FinishBufferString,

    TablePut,
    TableGet,
    TableRemove,
    NewTable,
    IsTable,

    DirectoryPut,
    DirectoryAt,
    DirectoryRemove,
    NewDirectory,
    IsDirectory,

    ArrayAt,
    ArrayHasIndex,
    ArrayPut,
    ArraySize,
    ArrayDimension,
    NewArray,
    ArrayOfOne,
    ArrayOfTwo,
    IsArray,

    BufferData,
    BufferLength,
    NewBuffer,
    IsBuffer,

    IntegerValue,
    NewInteger,
    IsInteger,

    PointerValue,
    NewPointer,
    IsPointer,

    SupplierValue,
    SupplierIndex,
    SupplierAvailable,
    SupplierNext,
    NewSupplier,

    SetStemElement,
    GetStemElement,
    DropStemElement,
    SetStemArrayElement,
    GetStemArrayElement,
    DropStemArrayElement,
    GetAllStemElements,
    GetStemValue,
    IsStem,

    RaiseException0,
    RaiseException1,
    RaiseException2,
    RaiseExceptionArray,
    RaiseCondition,
    CheckCondition,
    GetConditionInfo,
    DecodeConditionInfo,
    ClearCondition,

    OREF_NULL,
    OREF_NULL,
    OREF_NULL,
    OREF_NULL
};
