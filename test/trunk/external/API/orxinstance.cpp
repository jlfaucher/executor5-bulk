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

#include "oorexxapi.h"
#include "orxexits.hpp"

RexxObjectPtr TestGetContextVariable(RexxExitContext *context, size_t argc, RexxObjectPtr *argv)
{
    return context->GetContextVariable(context->ObjectToStringValue(argv[0]));
}

RexxObjectPtr TestSetContextVariable(RexxExitContext *context, size_t argc, RexxObjectPtr *argv)
{
    context->SetContextVariable(context->ObjectToStringValue(argv[0]), argv[1]);
    return context->True();
}

RexxObjectPtr TestDropContextVariable(RexxExitContext *context, size_t argc, RexxObjectPtr *argv)
{
    context->DropContextVariable(context->ObjectToStringValue(argv[0]));
    return context->True();
}

RexxObjectPtr TestGetAllContextVariables(RexxExitContext *context, size_t argc, RexxObjectPtr *argv)
{
    return (RexxObjectPtr)context->GetAllContextVariables();
}

RexxObjectPtr TestGetCallerContext(RexxExitContext *context, size_t argc, RexxObjectPtr *argv)
{
    return context->GetCallerContext();
}


int invokeExitFunction(RexxExitContext *context, const char *name, PEXIT exitInfo)
{
    RXOFNCCAL_PARM *parms = (RXOFNCCAL_PARM *)exitInfo;
    const char *functionName = parms->rxfnc_name.strptr;

    if (strcmp(functionName, "TESTGETCONTEXTVARIABLE") == 0)
    {
        parms->rxfnc_retc = TestGetContextVariable(context, parms->rxfnc_argc, parms->rxfnc_argv);
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTSETCONTEXTVARIABLE") == 0)
    {
        parms->rxfnc_retc = TestSetContextVariable(context, parms->rxfnc_argc, parms->rxfnc_argv);
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTDROPCONTEXTVARIABLE") == 0)
    {
        parms->rxfnc_retc = TestDropContextVariable(context, parms->rxfnc_argc, parms->rxfnc_argv);
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTGETALLCONTEXTVARIABLES") == 0)
    {
        parms->rxfnc_retc = TestGetAllContextVariables(context, parms->rxfnc_argc, parms->rxfnc_argv);
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTGETCALLERCONTEXT") == 0)
    {
        parms->rxfnc_retc = TestGetCallerContext(context, parms->rxfnc_argc, parms->rxfnc_argv);
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTSUBCALL") == 0)
    {
        // return the status of the flag
        parms->rxfnc_retc = parms->rxfnc_flags.rxffsub ? context->NewStringFromAsciiz("SUBROUTINE") : context->NewStringFromAsciiz("FUNCTION");
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTERROR") == 0)
    {
        // this should raise an error
        parms->rxfnc_flags.rxfferr = 1;
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTNOTOUND") == 0)
    {
        // this should raise an error
        parms->rxfnc_flags.rxffnfnd = 1;
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTARGUMENTS") == 0)
    {
        RexxArrayObject args = context->NewArray(parms->rxfnc_argc);
        for (int i = 0; i < parms->rxfnc_argc; i++)
        {
            if (parms->rxfnc_argv[i] != NULLOBJECT)
            {
                context->ArrayPut(args, parms->rxfnc_argv[i], i + 1);
            }
        }
        parms->rxfnc_retc = (RexxObjectPtr)args;
        return RXEXIT_HANDLED;
    }
    else
    {
        // pass on this
        return RXEXIT_NOT_HANDLED;
    }
}

int RexxEntry TestContextFunctionExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Script Function Exit"));
        return RXEXIT_NOT_HANDLED;
    }

    RXFNCCAL_PARM *parms = (RXFNCCAL_PARM *)exitInfo;
    const char *functionName = parms->rxfnc_name;
    if (strcmp(functionName, "TESTSUBCALL") == 0)
    {
        // return the status of the flag
        if (parms->rxfnc_flags.rxffsub)
        {
            strcpy(parms->rxfnc_retc.strptr, "SUBROUTINE");
        }
        else
        {
            strcpy(parms->rxfnc_retc.strptr, "FUNCTION");
        }
        parms->rxfnc_retc.strlength = strlen(parms->rxfnc_retc.strptr);
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTERROR") == 0)
    {
        // this should raise an error
        parms->rxfnc_flags.rxfferr = 1;
        return RXEXIT_HANDLED;
    }
    else if (strcmp(functionName, "TESTNOTOUND") == 0)
    {
        // this should raise an error
        parms->rxfnc_flags.rxffnfnd = 1;
        return RXEXIT_HANDLED;
    }
    else
    {
        // pass on this
        return RXEXIT_NOT_HANDLED;
    }
}

int RexxEntry TestContextCommandExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    RXCMDHST_PARM *parms = (RXCMDHST_PARM *)exitInfo;
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Command Exit"));
        return RXEXIT_NOT_HANDLED;
    }

    // handle commands here...we need to process both the address environment and the commands
    if (strcmp(parms->rxcmd_address, "FOOBAR") == 0)
    {
        strcpy(parms->rxcmd_retc.strptr, "-1");
        parms->rxcmd_retc.strlength = 2;
        return RXEXIT_HANDLED;
    }
    // ok, a good address...now do the different commands
    if (strcmp(parms->rxcmd_command.strptr, "GOOD") == 0)
    {
        strcpy(parms->rxcmd_retc.strptr, "0");
        parms->rxcmd_retc.strlength = 1;
        return RXEXIT_HANDLED;
    }
    else if (strcmp(parms->rxcmd_command.strptr, "ERROR") == 0)
    {
        strcpy(parms->rxcmd_retc.strptr, "1");
        parms->rxcmd_retc.strlength = 1;
        parms->rxcmd_flags.rxfcerr = 1;
        return RXEXIT_HANDLED;
    }
    // ok, a good address...now do the different commands
    else if (strcmp(parms->rxcmd_command.strptr, "TRACEON") == 0)
    {
        instanceInfo->msq = "ON";
        strcpy(parms->rxcmd_retc.strptr, "0");
        parms->rxcmd_retc.strlength = 1;
        return RXEXIT_HANDLED;
    }
    // ok, a good address...now do the different commands
    else if (strcmp(parms->rxcmd_command.strptr, "TRACEOFF") == 0)
    {
        instanceInfo->msq = "OFF";
        strcpy(parms->rxcmd_retc.strptr, "0");
        parms->rxcmd_retc.strlength = 1;
        return RXEXIT_HANDLED;
    }
    // ok, a good address...now do the different commands
    else if (strcmp(parms->rxcmd_command.strptr, "HALT") == 0)
    {
        instanceInfo->hlt = "HALT";
        strcpy(parms->rxcmd_retc.strptr, "0");
        parms->rxcmd_retc.strlength = 1;
        return RXEXIT_HANDLED;
    }
    else
    {
        // unknown command
        strcpy(parms->rxcmd_retc.strptr, "-2");
        parms->rxcmd_retc.strlength = 2;
        parms->rxcmd_flags.rxfcfail = 1;
        return RXEXIT_HANDLED;
    }
}

int RexxEntry TestContextQueueExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->msq, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->msq, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->msq, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Queue Exit"));
        return RXEXIT_NOT_HANDLED;
    }
    switch (subcode)
    {
        case RXMSQPLL:
        {
            RXMSQPLL_PARM *parms = (RXMSQPLL_PARM *)exitInfo;
            strcpy(parms->rxmsq_retc.strptr, "Hello World");
            parms->rxmsq_retc.strlength = strlen("Hello World");
            return RXEXIT_HANDLED;
        }
        case RXMSQPSH:
        {
            RXMSQPSH_PARM *parms = (RXMSQPSH_PARM *)exitInfo;
            if (parms->rxmsq_flags.rxfmlifo)
            {
                if (strcmp(parms->rxmsq_value.strptr, "LIFO") == 0)
                {
                    return RXEXIT_HANDLED;
                }
                else
                {
                    return RXEXIT_RAISE_ERROR;
                }
            }
            else
            {
                if (strcmp(parms->rxmsq_value.strptr, "FIFO") == 0)
                {
                    return RXEXIT_HANDLED;
                }
                else
                {
                    return RXEXIT_RAISE_ERROR;
                }
            }
        }
        case RXMSQSIZ:
        {
            RXMSQSIZ_PARM *parms = (RXMSQSIZ_PARM *)exitInfo;
            // this always returns a large, distinctive number
            parms->rxmsq_size = 999999;
            return RXEXIT_HANDLED;
        }
        case RXMSQNAM:
        {
            RXMSQPLL_PARM *parms = (RXMSQPLL_PARM *)exitInfo;
            strcpy(parms->rxmsq_retc.strptr, "FOOBAR");
            parms->rxmsq_retc.strlength = strlen("FOOBAR");
            return RXEXIT_HANDLED;
        }
    }
    return RXEXIT_RAISE_ERROR;
}

int RexxEntry TestContextSessionIOExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->sio, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->sio, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->sio, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("IO Exit"));
        return RXEXIT_NOT_HANDLED;
    }
    switch (subcode)
    {
        case RXSIOTRD:
        {
            if (strcmp(instanceInfo->sio, "ALL") == 0 || strcmp(instanceInfo->sio, "CONSOLE") == 0)
            {
                RXSIOTRD_PARM *parms = (RXSIOTRD_PARM *)exitInfo;
                strcpy(parms->rxsiotrd_retc.strptr, "Hello World");
                parms->rxsiotrd_retc.strlength = strlen("Hello World");
                return RXEXIT_HANDLED;
            }
            return RXEXIT_NOT_HANDLED;
        }
        case RXSIODTR:
        {
            if (strcmp(instanceInfo->sio, "ALL") == 0 || strcmp(instanceInfo->sio, "TRACE") == 0)
            {
                RXSIODTR_PARM *parms = (RXSIODTR_PARM *)exitInfo;
                strcpy(parms->rxsiodtr_retc.strptr, "trace off");
                parms->rxsiodtr_retc.strlength = strlen("trace off");
                return RXEXIT_HANDLED;
            }
            return RXEXIT_NOT_HANDLED;
        }
        case RXSIOSAY:
        {
            if (strcmp(instanceInfo->sio, "ALL") == 0 || strcmp(instanceInfo->sio, "CONSOLE") == 0)
            {
                RXSIOSAY_PARM *parms = (RXSIOSAY_PARM *)exitInfo;
                if (strcmp(parms->rxsio_string.strptr, "HELLO") == 0)
                {
                    return RXEXIT_HANDLED;
                }
                else
                {
                    return RXEXIT_RAISE_ERROR;
                }
            }
            return RXEXIT_NOT_HANDLED;
        }
        case RXSIOTRC:
        {
            if (strcmp(instanceInfo->sio, "ALL") == 0 || strcmp(instanceInfo->sio, "TRACE") == 0)
            {
                RXMSQSIZ_PARM *parms = (RXMSQSIZ_PARM *)exitInfo;
                // this one is really hard to test, so it's sufficient that we got here.
                return RXEXIT_HANDLED;
            }
            return RXEXIT_NOT_HANDLED;
        }
    }
    return RXEXIT_RAISE_ERROR;
}

int RexxEntry TestContextHaltExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Halt Exit"));
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "HALT") == 0)
    {
        RXHLTTST_PARM *parms = (RXHLTTST_PARM *)exitInfo;
        parms->rxhlt_flags.rxfhhalt = 1;
        return RXEXIT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "NOHALT") == 0)
    {
        RXHLTTST_PARM *parms = (RXHLTTST_PARM *)exitInfo;
        parms->rxhlt_flags.rxfhhalt = 0;
        return RXEXIT_HANDLED;
    }
    return RXEXIT_NOT_HANDLED;
}

int RexxEntry TestContextTraceExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Trace Exit"));
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ON") == 0)
    {
        RXHLTTST_PARM *parms = (RXHLTTST_PARM *)exitInfo;
        parms->rxhlt_flags.rxfhhalt = 1;
        return RXEXIT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "OFF") == 0)
    {
        RXHLTTST_PARM *parms = (RXHLTTST_PARM *)exitInfo;
        parms->rxhlt_flags.rxfhhalt = 0;
        return RXEXIT_HANDLED;
    }
    return RXEXIT_NOT_HANDLED;
}

int RexxEntry TestContextInitExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Initialization Exit"));
        return RXEXIT_NOT_HANDLED;
    }

    context->SetContextVariable("test1", context->NewStringFromAsciiz("Hello World"));
    return RXEXIT_HANDLED;
}

int RexxEntry TestContextTerminationExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Termination Exit"));
        return RXEXIT_NOT_HANDLED;
    }

    RexxObjectPtr value = context->GetContextVariable("test1");
    if (strcmp(context->ObjectToStringValue(value), "Hello World") != 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Termination Exit"));
    }
    return RXEXIT_NOT_HANDLED;
}

int RexxEntry TestContextScriptFunctionExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Script Function Exit"));
        return RXEXIT_NOT_HANDLED;
    }
    return invokeExitFunction(context, "Script", exitInfo);
}

int RexxEntry TestContextObjectFunctionExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Object Function Exit"));
        return RXEXIT_NOT_HANDLED;
    }
    return invokeExitFunction(context, "Object", exitInfo);
}

int RexxEntry TestContextNovalueExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("Novalue Exit"));
        return RXEXIT_NOT_HANDLED;
    }

    RXVARNOVALUE_PARM *parms = (RXVARNOVALUE_PARM *)exitInfo;

    if (strcmp(parms->variable_name.strptr, "FOO") == 0)
    {
        parms->value = context->ArrayOfOne(context->NewStringFromAsciiz("BAR"));
        return RXEXIT_HANDLED;
    }
    return RXEXIT_NOT_HANDLED;
}

int RexxEntry TestContextValueExit(RexxExitContext *context, int code, int subcode, PEXIT exitInfo)
{
    InstanceInfo *instanceInfo = (InstanceInfo *)context->GetApplicationData();
    if (strcmp(instanceInfo->cmd, "SKIP") == 0)
    {
        return RXEXIT_NOT_HANDLED;
    }
    else if (strcmp(instanceInfo->cmd, "ERROR") == 0)
    {
        return RXEXIT_RAISE_ERROR;
    }
    else if (strcmp(instanceInfo->cmd, "RAISE") == 0)
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz("VALUE Exit"));
        return RXEXIT_NOT_HANDLED;
    }

    RXVALCALL_PARM *parms = (RXVALCALL_PARM *)exitInfo;

    if (strcmp(parms->selector.strptr, "TEST") == 0)
    {
        if (strcmp(parms->variable_name.strptr, "FOO") == 0)
        {
            parms->value = context->ArrayOfOne(context->NewStringFromAsciiz("BAR"));
            return RXEXIT_HANDLED;
        }
    }
    return RXEXIT_NOT_HANDLED;
}

