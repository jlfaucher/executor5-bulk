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
#include "SupplierClass.hpp"
#include "Interpreter.hpp"

BEGIN_EXTERN_C()

RexxArrayObject RexxEntry GetCallArguments(RexxCallContext *c)
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

RexxObjectPtr RexxEntry GetCallArgument(RexxCallContext *c, RexxUnsignedNumber i)
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

void RexxEntry SetContextVariable(RexxCallContext *c, RexxStringPointer n, RexxObjectPtr v)
{
    ApiContext context(c);
    try
    {
        context.context->setContextVariable((stringchar_t *)n, (RexxObject *)v);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry GetContextVariable(RexxCallContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        return context.context->getContextVariable((stringchar_t *)n);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry DropContextVariable(RexxCallContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        context.context->dropContextVariable((stringchar_t *)n);
    }
    catch (ActivityException)
    {
    }
}

RexxSupplierObject RexxEntry GetAllContextVariables(RexxCallContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxSupplierObject)context.ret(context.context->getAllContextVariables());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

RexxStemObject RexxEntry ResolveStemVariable(RexxCallContext *c, RexxObjectPtr s)
{
    ApiContext context(c);
    try
    {
        return (RexxStemObject)context.context->resolveStemVariable((RexxObject *)s);
    }
    catch (ActivityException)
    {
        // this may throw an exception, so clear it out.  The null return is the
        // failure indication.
        context.context->setConditionInfo(OREF_NULL);
    }
    return NULLOBJECT;
}

void RexxEntry InvalidRoutine(RexxCallContext *c)
{
    ApiContext context(c);
    try
    {
        // raise an exception which will be reraised when the caller returns.
        reportException(Error_Incorrect_call);
    }
    catch (ActivityException)
    {
    }
}

void RexxEntry SetExitContextVariable(RexxExitContext *c, RexxStringPointer n, RexxObjectPtr v)
{
    ApiContext context(c);
    try
    {
        context.context->setContextVariable((stringchar_t *)n, (RexxObject *)v);
    }
    catch (ActivityException)
    {
    }
}

RexxObjectPtr RexxEntry GetExitContextVariable(RexxExitContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        return context.context->getContextVariable((stringchar_t *)n);
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

void RexxEntry DropExitContextVariable(RexxExitContext *c, RexxStringPointer n)
{
    ApiContext context(c);
    try
    {
        context.context->dropContextVariable((stringchar_t *)n);
    }
    catch (ActivityException)
    {
    }
}

RexxSupplierObject RexxEntry GetAllExitContextVariables(RexxExitContext *c)
{
    ApiContext context(c);
    try
    {
        return (RexxSupplierObject)context.ret(context.context->getAllContextVariables());
    }
    catch (ActivityException)
    {
    }
    return NULLOBJECT;
}

wholenumber_t RexxEntry GetContextDigits(RexxCallContext *c)
{
    ApiContext context(c);
    try
    {
        return numberDigits();
    }
    catch (ActivityException)
    {
    }
    return 0;
}
wholenumber_t RexxEntry GetContextFuzz(RexxCallContext *c)
{
    ApiContext context(c);
    try
    {
        return numberFuzz();
    }
    catch (ActivityException)
    {
    }
    return 0;

}
RexxBoolean RexxEntry GetContextForm(RexxCallContext *c)
{
    ApiContext context(c);
    try
    {
        return numberForm() ? TRUE : FALSE;
    }
    catch (ActivityException)
    {
    }
    return FALSE;
}

END_EXTERN_C()

CallContextInterface RexxActivity::callContextFunctions =
{
    CALL_INTERFACE_VERSION,
    GetCallArguments,
    GetCallArgument,
    SetContextVariable,
    GetContextVariable,
    DropContextVariable,
    GetAllContextVariables,
    ResolveStemVariable,
    InvalidRoutine,
    GetContextDigits,
    GetContextFuzz,
    GetContextForm
};

ExitContextInterface RexxActivity::exitContextFunctions =
{
    EXIT_INTERFACE_VERSION,
    SetExitContextVariable,
    GetExitContextVariable,
    DropExitContextVariable,
    GetAllExitContextVariables,
};

