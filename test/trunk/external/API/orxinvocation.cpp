/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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
/* THIS SOFTWARE IS PROVIDED BY THE COPYright HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYright   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <string.h>
#include "oorexxapi.h"
#include "orxexits.hpp"

RexxMethod0(int, init)
{
    RexxBufferObject buffer = context->NewBuffer(sizeof(InstanceInfo));
    InstanceInfo *instanceInfo = new(context->BufferData(buffer)) InstanceInfo();
    instanceInfo->exitType = InstanceInfo::NONE;
    instanceInfo->programName = "";
    instanceInfo->initialAddress = "";
    context->SetObjectVariable("CSELF", buffer);
    return 0;
}


RexxMethod2(int, setExitType,
            CSELF, self,
            CSTRING, type)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;

    if (strcmp(type, "NONE") == 0)
    {
        instanceInfo->exitType = InstanceInfo::NONE;
    }
    else if (strcmp(type, "CONTEXT") == 0)
    {
        instanceInfo->exitType = InstanceInfo::CONTEXT;
    }
    else if (strcmp(type, "DLL") == 0)
    {
        instanceInfo->exitType = InstanceInfo::REGISTERED_DLL;
    }
    else if (strcmp(type, "EXE") == 0)
    {
        instanceInfo->exitType = InstanceInfo::REGISTERED_EXE;
    }
    else
    {
    instanceInfo->exitType = InstanceInfo::NONE;
    }
    return 0;
}


RexxMethod1(int, setProgramName,
            CSELF, self,
            CSTRING, name)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instanceInfo->programName = name;
    return 0;
}


RexxMethod1(int, setInitialAddress,
            CSELF, self,
            CSTRING, name)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instanceInfo->initialAddress = name;
    return 0;
}


RexxMethod1(int, setSearchPath,
            CSELF, self,
            CSTRING, path)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instanceInfo->extensionPath = path;
    return 0;
}


RexxMethod1(int, setExtensions,
            CSELF, self,
            CSTRING, extensions)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instanceInfo->extensions = extensions;
    return 0;
}


RexxMethod1(int, setLoadLibrary,
            CSELF, self,
            CSTRING, lib)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instanceInfo->loadLibrary = lib;
    return 0;
}



RexxMethod1(CSTRING, getExitType,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    switch (instanceInfo->exitType)
    {
        case InstanceInfo::NONE:
            return "NONE";
        case InstanceInfo::CONTEXT:
            return "CONTEXT";
        case InstanceInfo::REGISTERED_DLL:
            return "DLL";
        case InstanceInfo::REGISTERED_EXE:
            return "EXE";

    }
    return "NONE";
}


RexxMethod2(int, setFunctionExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->fnc = setting;
    return 0;
}

RexxMethod2(int, setCommandExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->cmd = setting;
    return 0;
}

RexxMethod2(int, setQueueExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->msq = setting;
    return 0;
}

RexxMethod2(int, setIOExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->sio = setting;
    return 0;
}

RexxMethod2(int, setTraceExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->trc = setting;
    return 0;
}

RexxMethod2(int, setHaltExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->hlt = setting;
    return 0;
}

RexxMethod2(int, setInitExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->ini = setting;
    return 0;
}

RexxMethod2(int, setTermExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->ter = setting;
    return 0;
}

RexxMethod2(int, setScriptingExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->exf = setting;
    return 0;
}

RexxMethod2(int, setObjectFunctionExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->ofnc = setting;
    return 0;
}

RexxMethod2(int, setNovalueExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->var = setting;
    return 0;
}

RexxMethod2(int, setValueExit,
            CSELF, self,
            CSTRING, setting)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instaneInfo->val = setting;
    return 0;
}

RexxMethod1(CSTRING, getFunctionExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->fnc;
}

RexxMethod1(CSTRING, getCommandExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->cmd;
}

RexxMethod1(CSTRING, getQueueExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->msq;
}

RexxMethod1(CSTRING, getIOExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->sio;
}

RexxMethod1(CSTRING, getTraceExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->trc;
}

RexxMethod1(CSTRING, getHaltExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->hlt;
}

RexxMethod1(CSTRING, getInitExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->ini;
}

RexxMethod1(CSTRING, getTermExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->ter;
}

RexxMethod1(CSTRING, getScriptingExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->exf;
}

RexxMethod1(CSTRING, getObjectFunctionExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->ofnc;
}

RexxMethod1(CSTRING, getNovalueExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->var;
}

RexxMethod1(CSTRING, getValueExit,
            CSELF, self)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    return (const char *)instaneInfo->val;
}


RexxMethod1(wholenumber_t, getRC,
            CSELF, self)
{
     InstanceInfo *instanceInfo = (InstanceInfo *)self;
     return instanceInfo->rc;
}


RexxMethod1(wholenumber_t, getCode,
            CSELF, self)
{
     InstanceInfo *instanceInfo = (InstanceInfo *)self;
     return instanceInfo->code;
}


RexxMethod3(RexxObjectPtr, callInstanceProgram,
            CSELF, self,
            CSTRING, program,
            RexxArrayObject, args)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)self;
    instanceInfo->programName = program;
    instanceInfo->argCount = context->ArraySize(args);
    for (size_t i = 0; i < instanceInfo->argcount; i++)
    {
        RexxObjectPtr arg = context->ArrayAt(i + 1);
        if (arg != NULLOBJECT)
        {
            instanceInfo->arguments[i] = context->String(arg);
        }
        else
        {
            instanceInfo->arguments[i] = NULL;
        }
    }

    invokeProgram(instanceInfo);
    if (instanceInfo->rc != 0)
    {
        return context->Nil();
    }
    else
    {
        return context->String(instanceInfo->returnResult);
    }
}


RexxMethodEntry orxtest_methods[] = {
    REXX_METHOD(init,                    init),
    REXX_METHOD(setFunctionExit,         setFunctionExit),
    REXX_METHOD(setCommandExit,          setCommandExit),
    REXX_METHOD(setQueueExit,            setQueueExit),
    REXX_METHOD(setIOExit,               setIOExit),
    REXX_METHOD(setTraceExit,            setTraceExit),
    REXX_METHOD(setHaltExit,             setHaltExit),
    REXX_METHOD(setInitExit,             setInitExit),
    REXX_METHOD(setTermExit,             setTermExit),
    REXX_METHOD(setScriptingExit,        setScriptingExit),
    REXX_METHOD(setObjectFunctionExit,   setObjectFunctionExit),
    REXX_METHOD(setObjectFunctionExit,   setObjectFunctionExit),
    REXX_METHOD(setNovalueExit,          setNovalueExit),
    REXX_METHOD(setValueExit,            setValueExit),
    REXX_METHOD(getFunctionExit,         getFunctionExit),
    REXX_METHOD(getCommandExit,          getCommandExit),
    REXX_METHOD(getQueueExit,            getQueueExit),
    REXX_METHOD(getIOExit,               getIOExit),
    REXX_METHOD(getTraceExit,            getTraceExit),
    REXX_METHOD(getHaltExit,             getHaltExit),
    REXX_METHOD(getInitExit,             getInitExit),
    REXX_METHOD(getTermExit,             getTermExit),
    REXX_METHOD(getScriptingExit,        getScriptingExit),
    REXX_METHOD(getObjectFunctionExit,   getObjectFunctionExit),
    REXX_METHOD(getObjectFunctionExit,   getObjectFunctionExit),
    REXX_METHOD(getNovalueExit,          getNovalueExit),
    REXX_METHOD(getValueExit,            getValueExit),
    REXX_LAST_METHOD()
};


RexxPackageEntry UnitTest_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "InvocationTest",                    // name of the package
    "1.0.0",                             // package information
    NULL,                                // no load/unload functions
    NULL,
    NULL,                                // the exported routines
    orxtest_methods                      // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(UnitTest);
