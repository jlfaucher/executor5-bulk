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

#include "oorexx.h"
#include "ContextApi.hpp"
#include "RexxActivity.hpp"
#include "StringClass.hpp"
#include "IntegerClass.hpp"
#include "RexxBuffer.hpp"
#include "SupplierClass.hpp"
#include "ArrayClass.hpp"
#include "DirectoryClass.hpp"
#include "TableClass.hpp"
#include "MethodClass.hpp"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"
#include "PointerClass.hpp"

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


void RexxEntry SetThreadTrace(RexxThreadContext *c, RexxBoolean setting)
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
        context.activity->addGlobalReference((RexxObject *)o);
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
        context.activity->removeGlobalReference((RexxObject *)o);
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
        context.activity->removeLocalReference((RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
}

//NB:  The name "SendMessage" has a conflict with a Windows API, so this name differs from
// the call vector version.
RexxObjectPtr RexxEntry SendMessageArray(RexxThreadContext *c, RexxObjectPtr o, RexxStringPointer m, RexxArrayObject a)
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


RexxObjectPtr RexxEntry SendMessage0(RexxThreadContext *c, RexxObjectPtr o, RexxStringPointer m)
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


RexxObjectPtr RexxEntry SendMessage1(RexxThreadContext *c, RexxObjectPtr o, RexxStringPointer m, RexxObjectPtr a1)
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

RexxObjectPtr RexxEntry SendMessage2(RexxThreadContext *c, RexxObjectPtr o, RexxStringPointer m, RexxObjectPtr a1, RexxObjectPtr a2)
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
        context.activity->getLocalEnvironment();
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
        RexxInterpreter::theEnvironment;
    }
    catch (ActivityException)
    {

    }
    return NULLOBJECT;
}


RexxBoolean RexxEntry IsSameType(RexxThreadContext *c, RexxObjectPtr o1, RexxObjectPtr o2)
{
    ApiContext context(c);
    try
    {
        return isSameType((RexxObject *)o1, (RexxObject *)o2);
    }
    catch (ActivityException)
    {
    }
    return 0;
}


RexxBoolean RexxEntry IsInstanceOf(RexxThreadContext *c, RexxObjectPtr o, RexxClassObject cl)
{
    ApiContext context(c);
    try
    {
        return ((RexxClass *)cl)->instanceOf((RexxObject *)o);
    }
    catch (ActivityException)
    {
    }
    return 0;
}


RexxClassObject RexxEntry FindClass(RexxThreadContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        RexxString *name = new_upper_string(n);
        return (RexxClassObject)context.ret(context.activity->resolveClass(name));

    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxClassObject RexxEntry FindClassFromMethod(RexxThreadContext *c, RexxMethodObject m, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        RexxString *name = new_upper_string(n);
        return (RexxClassObject)context.ret(((RexxMethod *)m)->resolveClass(context.activity, name));

    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxBoolean RexxEntry HasMethod(RexxThreadContext *c, RexxObjectPtr o, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        return ((RexxObject *)o)->hasMethod((stringchar_t *)n);

    }
    catch (ActivityException)
    {
    }
    return 0;
}


RexxMethodObject RexxEntry NewMethod(RexxThreadContext *c, RexxStringPointer source, RexxUnsignedNumber length)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        return (RexxMethodObject)context.ret(RexxMethod::newMethod((stringchar_t *)source, (stringsize_t)length));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxDirectoryObject RexxEntry GetMethodRoutines(RexxThreadContext *c, RexxMethodObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.ret(((RexxMethod *)m)->getRoutines());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxDirectoryObject RexxEntry GetMethodClasses(RexxThreadContext *c, RexxMethodObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.ret(((RexxMethod *)m)->getMethods());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxDirectoryObject RexxEntry GetMethodMethods(RexxThreadContext *c, RexxMethodObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxDirectoryObject)context.ret(((RexxMethod *)m)->getClasses());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry CallMethod(RexxThreadContext *c, RexxMethodObject m, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        ProtectedObject result;
        RexxArray *args = (RexxArray *)a;
        context.activity->runProgram((RexxMethod *)m, OREF_NULLSTRING, OREF_SCRIPT, OREF_NULL, args->data(), args->size(), result);
        return context.ret((RexxObject *)result);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}


RexxBufferObject RexxEntry SaveMethod(RexxThreadContext *c, RexxMethodObject m)
{
    ApiContext context(c);
    try
    {
        return (RexxBufferObject)context.ret((RexxObject *)((RexxMethod *)m)->save());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxMethodObject RexxEntry LoadMethod(RexxThreadContext *c, RexxStringPointer d, size_t l)
{
    ApiContext context(c);
    try
    {
        RexxBuffer *buf = new_buffer((stringchar_t *)d, (size_t)l);
        return (RexxMethodObject)context.ret(RexxMethod::restore(buf, buf->address()));
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
        return context.ret(context.context->valueToObject(d));
    }
    catch (ActivityException)
    {
        context.context->setConditionInfo(OREF_NULL);
    }
    return NULLOBJECT;
}

RexxBoolean RexxEntry ObjectToValue(RexxThreadContext *c, RexxObjectPtr o, ValueDescriptor *d)
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

RexxObjectPtr RexxEntry UnsignedNumberToObject(RexxThreadContext *c, RexxUnsignedNumber n)
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


RexxBoolean RexxEntry ObjectToNumber(RexxThreadContext *c, RexxObjectPtr o, wholenumber_t *n)
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


RexxBoolean RexxEntry ObjectToUnsignedNumber(RexxThreadContext * c, RexxObjectPtr o, RexxUnsignedNumber * n)
{
    ApiContext context(c);
    try
    {
        stringsize_t temp;
        // this uses the entire value range
        if (Numerics::objectToStringSize((RexxObject *)o, temp, SIZE_MAX))
        {
            *n = (RexxUnsignedNumber)temp;
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

RexxBoolean RexxEntry ObjectToInt64(RexxThreadContext *c, RexxObjectPtr o, int64_t * n)
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

RexxBoolean RexxEntry ObjectToUnsignedInt64(RexxThreadContext *c, RexxObjectPtr o, uint64_t *n)
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

RexxBoolean RexxEntry ObjectToUintptr(RexxThreadContext * c, RexxObjectPtr o, uintptr_t * n)
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

RexxObjectPtr RexxEntry DoubleToObjectWithPrecision(RexxThreadContext *c, double n, wholenumber_t precision)
{
    ApiContext context(c);
    try
    {
        return context.ret(new_string(n, (stringsize_t)precision));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxBoolean RexxEntry ObjectToDouble(RexxThreadContext *c, RexxObjectPtr o, double *n)
{
    ApiContext context(c);
    try
    {
        return ((RexxObject *)o)->doubleValue(n);
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
        return context.ret(REQUEST_STRING(o));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxStringPointer RexxEntry ObjectToStringValue(RexxThreadContext *c, RexxObjectPtr o)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = REQUEST_STRING(o);
        context.ret(temp);
        return (RexxStringPointer)temp->getStringData();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

size_t RexxEntry StringGet(RexxThreadContext *c, RexxStringObject s, size_t o, RexxStringPointer r, size_t l)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return temp->get(o, (stringchar_t *)r, l);
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

RexxStringPointer RexxEntry StringData(RexxThreadContext *c, RexxStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return (RexxStringPointer)temp->getStringData();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

RexxStringObject RexxEntry NewString(RexxThreadContext *c, RexxStringPointer s, size_t l)
{
    ApiContext context(c);
    try
    {
        return (RexxStringObject)context.ret(new_string((stringchar_t *)s, (stringsize_t)l));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxStringObject RexxEntry NewStringFromAsciiz(RexxThreadContext *c, RexxStringPointer s)
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
        return context.ret(temp->upper());
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
        return context.ret(temp->lower());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxBoolean RexxEntry IsString(RexxThreadContext *c, RexxObjectPtr o)
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
        return (RexxStringObject)context.ret(raw_string((stringsize_t)l));
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

RexxStringPointer RexxEntry BufferStringData(RexxThreadContext *c, RexxBufferStringObject s)
{
    ApiContext context(c);
    try
    {
        RexxString *temp = (RexxString *)s;
        return (RexxStringPointer)temp->getStringData();
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
        return context.ret(new_table());
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

RexxBoolean RexxEntry IsTable(RexxThreadContext *c, RexxObjectPtr o)
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

void  RexxEntry DirectoryPut(RexxThreadContext *c, RexxDirectoryObject t, RexxObjectPtr o, RexxStringPointer i)
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

RexxObjectPtr RexxEntry DirectoryAt(RexxThreadContext *c, RexxDirectoryObject t, RexxStringPointer i)
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

RexxObjectPtr RexxEntry DirectoryRemove(RexxThreadContext *c, RexxDirectoryObject t, RexxStringPointer i)
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
        return context.ret(new_table());
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

RexxBoolean RexxEntry IsDirectory(RexxThreadContext *c, RexxObjectPtr o)
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

RexxObjectPtr RexxEntry ArrayAt(RexxThreadContext *c, RexxArrayObject a, RexxUnsignedNumber i)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxArray *)a)->getApi((arraysize_t)i));
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}

RexxBoolean RexxEntry ArrayHasIndex(RexxThreadContext *c, RexxArrayObject a, RexxUnsignedNumber i)
{
    ApiContext context(c);
    try
    {
        return ((RexxArray *)a)->hasIndexApi((arraysize_t)i);
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

void RexxEntry ArrayPut(RexxThreadContext *c, RexxArrayObject a, RexxObjectPtr o, RexxUnsignedNumber i)
{
    ApiContext context(c);
    try
    {
        ((RexxArray *)a)->putApi((RexxObject *)o, (arraysize_t)i);
    }
    catch (ActivityException)
    {
    }
}

RexxUnsignedNumber RexxEntry ArraySize(RexxThreadContext *c, RexxArrayObject a)
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

RexxArrayObject RexxEntry NewArray(RexxThreadContext *c, RexxUnsignedNumber s)
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


RexxBoolean RexxEntry IsArray(RexxThreadContext *c, RexxObjectPtr o)
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

RexxStringPointer RexxEntry BufferData(RexxThreadContext *c, RexxBufferObject b)
{
    ApiContext context(c);
    try
    {
        return (RexxStringPointer)((RexxBuffer *)b)->address();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

wholenumber_t RexxEntry BufferLength(RexxThreadContext *c, RexxBufferObject b)
{
    ApiContext context(c);
    try
    {
        return ((RexxBuffer *)b)->length();
    }
    catch (ActivityException)
    {
    }
    return 0;
}

RexxBufferObject RexxEntry NewBuffer(RexxThreadContext *c, RexxUnsignedNumber l)
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

RexxBoolean RexxEntry IsBuffer(RexxThreadContext *c, RexxObjectPtr o)
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

RexxBoolean RexxEntry IsInteger(RexxThreadContext *c, RexxObjectPtr o)
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

RexxBoolean RexxEntry IsPointer(RexxThreadContext *c, RexxObjectPtr o)
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

RexxBoolean RexxEntry SupplierAvailable(RexxThreadContext *c, RexxSupplierObject o)
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
        return context.ret(new_supplier((RexxArray *)values, (RexxArray *)names));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry SetStemElement(RexxThreadContext *c, RexxStemObject s, RexxStringPointer n, RexxObjectPtr v)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->setElement((stringchar_t *)n, (RexxObject *)v);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry GetStemElement(RexxThreadContext *c, RexxStemObject s, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxStem *)s)->getElement((stringchar_t *)n));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry DropStemElement(RexxThreadContext *c, RexxStemObject s, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->dropElement((stringchar_t *)n);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry SetStemArrayElement(RexxThreadContext *c, RexxStemObject s, RexxUnsignedNumber i, RexxObjectPtr v)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->setElement((stringsize_t )i, (RexxObject *)v);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry GetStemArrayElement(RexxThreadContext *c, RexxStemObject s, RexxUnsignedNumber i)
{
    ApiContext context(c);
    try
    {
        return context.ret(((RexxStem *)s)->getElement((stringsize_t)i));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry DropStemArrayElement(RexxThreadContext *c, RexxStemObject s, RexxUnsignedNumber i)
{
    ApiContext context(c);
    try
    {
        ((RexxStem *)s)->dropElement((stringsize_t)i);
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

RexxBoolean RexxEntry IsStem(RexxThreadContext *c, RexxObjectPtr o)
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

void RexxEntry RaiseException0(RexxThreadContext *c, RexxUnsignedNumber n)
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

void RexxEntry RaiseException1(RexxThreadContext *c, RexxUnsignedNumber n, RexxObjectPtr o1)
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

void RexxEntry RaiseException2(RexxThreadContext *c, RexxUnsignedNumber n, RexxObjectPtr o1, RexxObjectPtr o2)
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

void RexxEntry RaiseExceptionArray(RexxThreadContext *c, RexxUnsignedNumber n, RexxArrayObject a)
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

void RexxEntry RaiseCondition(RexxThreadContext *c, RexxStringPointer name, RexxStringPointer desc, RexxArrayObject add, RexxObjectPtr result)
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

RexxBoolean RexxEntry CheckCondition(RexxThreadContext *c)
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
        return context.context->getConditionInfo();
    }
    catch (ActivityException)
    {
    }
    return OREF_NULL;
}


void RexxEntry DecodeConditionInfo(RexxThreadContext *c, RexxDirectoryObject d, ConditionData *cd)
{
    ApiContext context(c);
    try
    {
        RexxInterpreter::decodeConditionData((RexxDirectory *)d, cd);
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
    FindClass,
    FindClassFromMethod,
    HasMethod,

    NewMethod,
    GetMethodRoutines,
    GetMethodClasses,
    GetMethodMethods,
    CallMethod,
    SaveMethod,
    LoadMethod,

    NewObject,
    NumberToObject,
    UintptrToObject,
    ValueToObject,
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
