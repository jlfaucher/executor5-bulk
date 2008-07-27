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
#include "RexxNativeActivation.hpp"

BEGIN_EXTERN_C()

RexxArrayObject RexxEntry GetMethodArguments(RexxMethodContext *c)
{
    ApiContext context(c);
    try
    {
        return context.context->getArguments();
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry GetMethodArgument(RexxMethodContext *c, RexxUnsignedNumber i)
{
    ApiContext context(c);
    try
    {
        return context.context->getArgument(i);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxMethodObject RexxEntry GetCurrentMethod(RexxMethodContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxMethodObject)context.context->getMethod();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

RexxStringPointer RexxEntry GetMessageName(RexxMethodContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxStringPointer)context.context->getMessageName();
    }
    catch (ActivityException)
    {
    }
    return NULL;
}

RexxObjectPtr RexxEntry GetSelf(RexxMethodContext *c)
{
    ApiContext context(c);
    try
    {
        return context.context->getSelf();
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxClassObject RexxEntry GetSuper(RexxMethodContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxClassObject)context.context->getSuper();
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry SetObjectVariable(RexxMethodContext *c, RexxStringPointer n, RexxObjectPtr v)
{
    ApiContext context(c);
    try
    {
        context.context->setObjectVariable((stringchar_t *)n, (RexxObject *)v);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry GetObjectVariable(RexxMethodContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        return context.context->getObjectVariable((stringchar_t *)n);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry DropObjectVariable(RexxMethodContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        context.context->dropObjectVariable((stringchar_t *)n);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry SendSuperMessage(RexxMethodContext *c, RexxStringPointer n, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        RexxObject *self = context.context->getSelf();
        RexxObject *super = context.context->getSuper();
        RexxString *message = new_string(n);
        RexxArray *args = (RexxArray *)a;
        ProtectedObject result;
        self->messageSend(context.activity, message, args->size(), args->data(), super, result);
        return context.ret((RexxObject *)result);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxObjectPtr RexxEntry SendOverrideMessage(RexxMethodContext *c, RexxStringPointer n, RexxClassObject clazz, RexxArrayObject a)
{
    ApiContext context(c);
    try
    {
        RexxObject *self = context.context->getSelf();
        RexxObject *super = (RexxObject *)clazz;
        RexxString *message = new_string(n);
        RexxArray *args = (RexxArray *)a;
        ProtectedObject result;
        self->messageSend(context.activity, message, args->size(), args->data(), super, result);
        return context.ret((RexxObject *)result);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry SetGuardOn(RexxMethodContext *c)
{
    ApiContext context(c);
    try
    {
        context.context->guardOn();
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry SetGuardOff(RexxMethodContext *c)
{
    ApiContext context(c);
    try
    {
        context.context->guardOff();
    }
    catch (ActivityException)
    {
    }
}

RexxClassObject RexxEntry FindContextClass(RexxMethodContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        // convert the name to a string instance, and check the environments.
        RexxString *name = new_upper_string(n);
        return (RexxClassObject)context.ret(context.context->getMethod()->resolveClass(context.activity, name));
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

END_EXTERN_C()


MethodContextInterface RexxActivity::methodContextFunctions =
{
    METHOD_INTERFACE_VERSION,
    GetMethodArguments,
    GetMethodArgument,
    GetMessageName,
    GetCurrentMethod,
    GetSelf,
    GetSuper,
    SetObjectVariable,
    GetObjectVariable,
    DropObjectVariable,
    SendSuperMessage,
    SendOverrideMessage,
    SetGuardOn,
    SetGuardOff,
    FindContextClass
};


