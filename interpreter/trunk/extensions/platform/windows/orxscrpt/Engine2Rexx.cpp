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

#include <process.h>
#include "Engine2Rexx.hpp"
#include "ScriptUtilities.hpp"

#undef DEBUGC
#undef DEBUGZ

// these are definitions from urlmon.h - needed for security manager of IE
// instead of having to use the extra package of IE they are defined here
// (see INETSDK and it's successors for details!)
// note: it may be that Microsoft decides to include these into the SDK
//       at some later point - it's mixed up already (IObjectSafety, for example) -
//       these lines may result in compiler errors and would have to be removed
// MHES 29122004 - commented out ENG001A
//#define CONFIRMSAFETYACTION_LOADOBJECT  0x00000001
//static const GUID GUID_CUSTOM_CONFIRMOBJECTSAFETY = { 0x10200490, 0xfa38, 0x11d0, { 0xac, 0xe, 0x0, 0xa0, 0xc9, 0xf, 0xff, 0xc0 } };
//#ifdef _OLDWINDOWS
//struct CONFIRMSAFETY
//    {
//    CLSID clsid;
//    IUnknown __RPC_FAR *pUnk;
//    DWORD dwFlags;
//    };
//#endif

// include struct definition if CONFIRMSAFETYACTION_LOADOBJECT not defined - more portable
#ifndef CONFIRMSAFETYACTION_LOADOBJECT
    #define CONFIRMSAFETYACTION_LOADOBJECT  0x00000001
struct CONFIRMSAFETY
{
    CLSID clsid;
    IUnknown __RPC_FAR *pUnk;
    DWORD dwFlags;
};
#endif
static const GUID GUID_CUSTOM_CONFIRMOBJECTSAFETY = { 0x10200490, 0xfa38, 0x11d0, { 0xac, 0xe, 0x0, 0xa0, 0xc9, 0xf, 0xff, 0xc0}};

ooRexxScript* findEngineForThread(DWORD);

ooRexxScript *CurrentEngine;

RexxObjectPtr DispParms2RexxArray(RexxThreadContext *context, void *arguments)
{
    RexxObjectPtr result = NULLOBJECT;
    DISPPARAMS *dp = (DISPPARAMS*) arguments;
    int j;

    //   Thanks to the wonderful way that Windows passes variants,
    // this routine must reverse the args....
    if (dp)
    {
        j = dp->cArgs;
        result = context->NewArray(j);
        for (int i=0; i<j; i++)
        {
            RexxObject Ptr temp = Variant2Rexx(context, &dp->rgvarg[j-i-1]);
            context->ArrayPut(result, temp, i+1);
        }

    }
    // no arguments? set in default empty string
    else
    {
        result = context->ArrayOfOne(context->NullString());
    }

    return result;
}



/* Exit handler to find out the values of variables of "immediate code" */
RexxReturnCode RexxEntry RexxRetrieveVariables(RexxExitContext *context, RexxNumber exitNumber, RexxNumber subfunction, RexxExitParm *parmblock)
{
    ooRexxScript     *engine    = findEngineForThread(GetCurrentThreadId());

    if (engine)
    {
        // we need to pass a function, a member of an object doesn't work...
        // therefore a global variable that is holding the pointer to the
        // current engine. beware: this doesn't work in multi-threads...
        // >>> ??? <<< Can we put a mutex check on this?
        // get the set of context variables
        RexxSupplierObject vars = context->GetAllContextVariables();

        while (context->SupplierAvailable(vars))
        {
            RexxObjectPtr name = context->SupplierIndex(vars);
            RexxObjectPtr value = context->SupplierValue(vars);
            engine->insertVariable(name, value);
        }

        // always return with NOT_HANDLED...
        return RXEXIT_NOT_HANDLED;
    }
}


/* Exit handler for external function calls */
RexxReturnCode RexxEntry RexxCatchExternalFunc(RexxExitContext *context, RexxNumber exitNumber, RexxNumber subfunction, RexxExitParm *pblock)
{
    RxFunctionExitParm *parmblock = (RxFunctionExitParm *)pblock;
    RexxObjectPtr  result    = context->NilObject();
    char          *fncname   = context->StringData(parmblock->function_name);
    ooRexxScript  *engine    = findEngineForThread(GetCurrentThreadId());
    PRCB           pImage    = NULL;
    RexxReturnCode state = RXEXIT_NOT_HANDLED;
    BOOL           function = !parmblock->rxfnc_flags.rxffsub;  // called as function?

    HRESULT        hResult;
    DISPID         dispID;
    DWORD          flags;
    IDispatch     *pDispatch = NULL;
    ITypeInfo     *pTypeInfo = NULL;
    DISPPARAMS     dp;
    VARIANT        sResult;
    VARIANT       *pResult;
    EXCEPINFO      sExc;
    unsigned int   uArgErr;
    BOOL           test = FALSE;

    // have an engine to work with?
    if (engine)
    {
        // is the function we're looking for a REXX method that was defined for this
        // script engine earlier?
        pImage = (RCB*) engine->findRexxFunction(fncname);
        if (pImage)
        {
            // the function that needs to be called is a REXX method NOT defined
            // in the current REXX script scope

            // argument array for runMethod
            LPVOID    arguments[8];
            char      invString[256];
            char      buffer[8];
            ConditionData cd;

            // build argument array of REXX objects...
            REXXOBJECT args = RexxArray(1+parmblock->rxfnc_argc);
            REXXOBJECT temp;

            // ...and invocation string
            if (function)
                sprintf(invString,"return %s(", parmblock->rxfnc_name);
            else
                sprintf(invString,"call %s ", parmblock->rxfnc_name);
            for (int i=0;i<parmblock->rxfnc_argc;i++)
            {
                sscanf(parmblock->rxfnc_argv[i].strptr,"%p",&temp);
                array_put(args,temp,i+2); // does this change current activity?? seems to be NULL when we run into okarray.c
                sprintf(buffer,"arg(%d)",i+2); // +2,because 1st argument is "CALL %s..." string
                strcat(invString,buffer);
                // not the last argument?
                if (i<parmblock->rxfnc_argc-1)
                    strcat(invString,",");
                // if there are too many arguments, raise error (maybe we need a bigger
                // string, or a different way of passing the args?)
                if (strlen(invString) > 240)
                {
                    sprintf(invString,"RAISE SYNTAX 5");
                    break;
                }
            }
            if (function)
                strcat(invString,")");
            else
                strcat(invString,"; exit");

            // invocation statement
            array_put(args, RexxString(invString), 1);

            arguments[0] = (void*) engine; // engine that is running this code
            arguments[1] = (void*) pImage; // method to run
            arguments[2] = NULL;           // no COM arguments
            arguments[3] = (void*) args;   // we have REXX arguments
            arguments[4] = (void*) &result;// result object
            arguments[5] = (void*) &cd;    // condition information
            arguments[6] = (void*) false;  // don't end this thread
            arguments[7] = (void*) false;  // don't care about variables at the end
            runMethod(arguments);
            state = RXEXIT_HANDLED;
            sprintf(parmblock->rxfnc_retc.strptr,"%p",result);
            parmblock->rxfnc_retc.strlength = 8;
            // if something went wrong, indicate an error to REXX
            if (cd.rc != 0)
            {
                // indicate error in parmblock...
                parmblock->rxfnc_flags.rxfferr = 1;
                // we don't need to notify the engine of the error
                // here, because this was already done by the part
                // of the code that invoked the REXX codeblock
            }
        }
        else
        {
            // ask the named items if one of them knows this function
            hResult = engine->getNamedItems()->WhoKnows((char*) parmblock->rxfnc_name,engine->Lang,&dispID,&flags,&pDispatch,&pTypeInfo);
            if (hResult == S_OK)
            {
                // found something...
                // build DISPPARAMS structure for the invoke
                dp.cNamedArgs = 0;
                dp.cArgs = parmblock->rxfnc_argc;
                VariantInit(&sResult);
                pResult = &sResult;

                // do we have to build an argument array?
                if (parmblock->rxfnc_argc)
                {
                    REXXOBJECT temp;
                    VARIANTARG *pVarArgs = (VARIANTARG*) GlobalAlloc(GMEM_FIXED,(sizeof(VARIANTARG) * dp.cArgs));

                    dp.rgvarg = pVarArgs;
                    for (int i = 0; i < dp.cArgs; i++)
                    {
                        // get the REXX object
                        sscanf(parmblock->rxfnc_argv[i].strptr,"%p",&temp);

                        // arguments must be filled in from the end of the array...
                        VariantInit(&(pVarArgs[dp.cArgs - i - 1]));
                        Rexx2Variant(temp, &(pVarArgs[dp.cArgs - i - 1]), VT_EMPTY /*(try your best)*/, 0 /*dummy argument?!*/);
                    }
                }
                else
                    dp.rgvarg = NULL; // no argument array needed

                if (dispID != -1)
                {
                    hResult = pDispatch->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, pResult, &sExc, &uArgErr);
                }
                else
                {
                    // call the default method of a named item object
                    unsigned short wFlags = DISPATCH_METHOD;
                    DISPID         PropPutDispId = DISPID_PROPERTYPUT;

                    pTypeInfo = NULL;
                    hResult = pDispatch->GetTypeInfo(0, LOCALE_USER_DEFAULT, &pTypeInfo);

                    if (SUCCEEDED(hResult))
                    {
                        TYPEATTR *pTypeAttr = NULL;
                        FUNCDESC *pFuncDesc = NULL;
                        bool      found = false;

                        hResult = pTypeInfo->GetTypeAttr(&pTypeAttr);
                        if (SUCCEEDED(hResult))
                        {
                            for (int l = 0; (l < pTypeAttr->cFuncs) && !found; l++)
                            {
                                hResult = pTypeInfo->GetFuncDesc(l, &pFuncDesc);
                                if (SUCCEEDED(hResult))
                                {
                                    if ((pFuncDesc->cParams + pFuncDesc->cParamsOpt >= parmblock->rxfnc_argc) &&
                                        pFuncDesc->memid == 0)
                                    {
                                        wFlags = pFuncDesc->invkind;
                                        found = true;
                                    }
                                    pTypeInfo->ReleaseFuncDesc(pFuncDesc);
                                }
                            }

                            pTypeInfo->ReleaseTypeAttr(pTypeAttr);
                            pTypeInfo->Release();
                        }
                        if (wFlags == DISPATCH_PROPERTYPUT)
                        {
                            dp.cNamedArgs = 1;
                            dp.rgdispidNamedArgs = &PropPutDispId;
                        }
                    }

                    hResult = pDispatch->Invoke((DISPID) 0, IID_NULL, LOCALE_USER_DEFAULT, wFlags, &dp, pResult, &sExc, &uArgErr);
                }

                if (SUCCEEDED(hResult))
                {
                    // success, make REXX object from VARIANT
                    result = Variant2Rexx(&sResult);
                    state = RXEXIT_HANDLED;
                    sprintf(parmblock->rxfnc_retc.strptr,"%p",result);
                    parmblock->rxfnc_retc.strlength = 8;
                }
                else
                {
                    result = NULL; // this is an error. behave accordingly
                    parmblock->rxfnc_flags.rxfferr = 1;
                }

                // clear argument array, free memory:
                if (dp.rgvarg)
                {
                    for (int i = 0; i < dp.cArgs; i++)
                        VariantClear(&(dp.rgvarg[i]));
                    GlobalFree(dp.rgvarg);
                }

                // free result
                VariantClear(&sResult);
            }
        }
    }

    return state;
}

// insert engine pointer into thread list so that
// the string unknown can find out which engine to use
// this may be additional work if REXX code is only run
// from one thread. if we have to go to "multithread"
// (again) this will be usefull, there we just leave it
// in here...
void registerEngineForCallback(ooRexxScript *engine)
{
    char       string[9];

    sprintf(string,"%08x",GetCurrentThreadId());
    // exclusive access to this region
    WaitForSingleObject(mutex,INFINITE);
    thread2EngineList->AddItem(string,LinkedList::Beginning,engine);
    ReleaseMutex(mutex);
}

// remove engine pointer from thread list
void deregisterEngineForCallback()
{
    char       string[9];
    sprintf(string,"%08x",GetCurrentThreadId());
    // exclusive access to this region
    WaitForSingleObject(mutex,INFINITE);

    thread2EngineList->DropItem(thread2EngineList->FindItem(string));
    ReleaseMutex(mutex);
}


/* this function looks for the engine that is associated with the */
/* given thread id. on failure it returns NULL.                   */
/* the list used to find this information is process-global.      */
ooRexxScript* findEngineForThread(DWORD id)
{
    char       string[9];
    ListItem  *result = NULL;
    ooRexxScript *engine = NULL;

    sprintf(string,"%08x",id);

    // exclusive access to this region
    WaitForSingleObject(mutex,INFINITE);
    result = thread2EngineList->FindItem(string);

    if (result)
        engine = (ooRexxScript*) result->GetContent();

    ReleaseMutex(mutex);

    return engine;
}

int __stdcall scriptSecurity(CLSID clsid, IUnknown *pObject)
{
    int result = 1; // assume OK
    ooRexxScript  *engine = findEngineForThread(GetCurrentThreadId());

    // have an engine?
    if (engine)
    {
        HRESULT hResult;
        DWORD dwPolicy;
        IInternetHostSecurityManager *pSecurityManager;

        // script host object should be considered safe at all time (acc. to Joel Alley, MS)
        // this is achieved by turning of the security check for objects that
        // have been added by the script host and will be instantiated by the
        // script at the time of need. a flag is used to find out if a check
        // has to be made
        if (engine->checkObjectCreation() == false) return result;

        pSecurityManager = engine->getIESecurityManager();

        // found a security manager?
        if (pSecurityManager)
        {
            // only clsid given? check if we're allowed to create this
            if (pObject == NULL)
            {
                hResult = pSecurityManager->ProcessUrlAction(URLACTION_ACTIVEX_RUN,
                    (BYTE*) &dwPolicy,
                    sizeof(dwPolicy),
                    (BYTE*) &clsid,
                    sizeof(clsid),
                    PUAF_DEFAULT,
                    0);
                if (SUCCEEDED(hResult))
                {
                    if (dwPolicy != URLPOLICY_ALLOW)
                        result = 0; // not allowed to create
                }
                else
                    result = 0;   // not allowed to create
            }
            else
            {
                // an object was given, check if it is safe to run it
                DWORD        *pdwPolicy;
                DWORD         cbPolicy;
                CONFIRMSAFETY csafe;

                csafe.pUnk  = pObject;
                csafe.clsid = clsid;
                csafe.dwFlags = CONFIRMSAFETYACTION_LOADOBJECT;

                hResult = pSecurityManager->QueryCustomPolicy(GUID_CUSTOM_CONFIRMOBJECTSAFETY,
                    (BYTE**) &pdwPolicy,
                    &cbPolicy,
                    (BYTE*) &csafe,
                    sizeof(csafe),
                    0);
                if (SUCCEEDED(hResult))
                {
                    dwPolicy = URLPOLICY_DISALLOW;
                    if (pdwPolicy)
                    {
                        if (sizeof(DWORD) <= cbPolicy)
                            dwPolicy = *pdwPolicy;
                        CoTaskMemFree(pdwPolicy);
                    }
                    if (dwPolicy != URLPOLICY_ALLOW)
                        result = 0; // not allowed to run
                }
                else
                    result = 0;   // not allowed to run
            }
        }
    }
    return result;
}

/******************************************************************************
*
*  callback for VALUE
*  this is used set and retrieve engine properties with the VALUE func.
*
*  Note that we go through WhoKnows() to find/set these, and do not
*  check the PropertyList ourselves.  By going through WhoKnows(),
*  we let the WSH name precedence resolve it.  If it is one of ours
*  it will be handled by ooRexxIDispatch, just as if JScript called.
*
******************************************************************************/
REXXOBJECT RexxEntry propertyChange(REXXOBJECT name, REXXOBJECT newvalue,int SelectorType,int *RetCode)
{
    REXXOBJECT  result = NULLOBJECT;
    ooRexxScript  *engine = findEngineForThread(GetCurrentThreadId());
    char       *PropName=NULL;
    char        tBuffer[1024];
    HRESULT     hResult;
    DISPID      dispID;
    DWORD       flags;
    IDispatch  *pDispatch = NULL;
    ITypeInfo  *pTypeInfo = NULL;
    VARIANT    *Property;

    *RetCode = -1;        // No name found.
    //  First, validate parameters.  Second, copy the name into a useable area.
    // _asm int 3
    if (!name)
    {                     // If there is no name, error out.
        *RetCode = 1;
        return result;
    }

    size_t length = string_length(name);
    if (length == 0)
    {
        *RetCode = 2;
        return result;
    }

    if ((length + 1) <= sizeof(tBuffer)) PropName = &tBuffer[0];
    else if (!(PropName = (char *)malloc(length+1)))
    {
        *RetCode = 3;
        return result;
    }
    memcpy(PropName, string_data(name), length);
    PropName[length] = '\0';

    // if we set a new value, we must return the old one...
    // Either way, first find the old one.

    // do we have an engine to work with?
    if (engine)
    {
        switch (SelectorType)
        {
            case 1:
                Property = engine->GetExternalProperty(PropName);
                if (PropName != &tBuffer[0]) free(PropName);
                if (Property)
                {
                    // GET
                    result = Variant2Rexx(Property);
                    // PUT
                    if (newvalue)
                    {
                        VariantClear(Property);
                        //  >>> ??? <<< Does this return something that I can check for failure?
                        Rexx2Variant(newvalue, Property, VT_EMPTY /*(try your best)*/, 0 /*dummy argument?!*/);
                    }
                    *RetCode = 0;
                }
                else
                {
                    *RetCode = 0;
                    result = RexxString("");
                }
                break;
            case 2:
                hResult = engine->getNamedItems()->WhoKnows(PropName,engine->Lang,&dispID,&flags,&pDispatch,&pTypeInfo);
                if (PropName != &tBuffer[0]) free(PropName);
                if (SUCCEEDED(hResult))
                {
                    // do we have a real named item?
                    if (dispID == -1)
                    {
                        //  >>> ??? <<<   This is nasty.
                        //  >>> ??? <<<   error condition 1.
                        *RetCode = 0;
                    }
                    else
                    {
                        // we have a property

                        if (pDispatch == NULL)
                        {
                            // This property is part of a Typelib.
                            TYPEATTR *pTypeAttr;
                            VARDESC  *pVarDesc;
                            bool      found = false;


                            if (newvalue)
                            {
                                *RetCode = 4;        // For now, Typelib's are immutable.
                                return NULL;
                            }
                            hResult = pTypeInfo->GetTypeAttr(&pTypeAttr);
                            if (SUCCEEDED(hResult))
                            {
                                for (int i=0; i<pTypeAttr->cVars && !found; i++)
                                {
                                    hResult = pTypeInfo->GetVarDesc(i,&pVarDesc);
                                    if (SUCCEEDED(hResult))
                                    {
                                        // found the variable?
                                        if (pVarDesc->memid == dispID)
                                        {
                                            found = true;
                                            *RetCode = 6;      // Got a variant, just not a supported one.
                                            switch (pVarDesc->varkind)
                                            {
                                                case VAR_STATIC:
                                                    break;   // can this be treated like VAR_CONST, too?case VAR_CONST:
                                                    result = Variant2Rexx(pVarDesc->lpvarValue);
                                                    *RetCode = 0;
                                                    break;
                                                    // don't know what to do with these two:case VAR_PERINSTANCE:case VAR_DISPATCH:
                                                    break;
                                            }
                                        }

                                        pTypeInfo->ReleaseVarDesc(pVarDesc);
                                    }
                                }

                                pTypeInfo->ReleaseTypeAttr(pTypeAttr);
                            }
                        }
                    }  //  if (dispID == -1)
                }  //  if (SUCCEEDED(hResult))

                if (result == NULL)
                {
                    result = RexxString("");
                }
                break;
            case 3:
                result = RexxNil;
                // get the named items of the engine
                // a newvalue must not exist since this is read-only!
                if (newvalue == NULL)
                {
                    // a name must be given
                    if (name)
                    {
                        // it must be "NAMEDITEMS"
                        if (!strcmp(string_data(name), "NAMEDITEMS"))
                        {
                            // retrieve the NamedItem list of the engine
                            ooRexxNamedItem* pItemList = engine->getNamedItems();
                            if (pItemList)
                            {
                                int num = 0;
                                // return a char* array with the names
                                // num contains the number of strings
                                char **names = pItemList->getNamedItems(&num);
                                SAFEARRAY      *pSafeArray = NULL;
                                SAFEARRAYBOUND  ArrayBound;
                                VARIANT        *pVarArray = (VARIANT*) malloc(sizeof(VARIANT));
                                VARIANT         sVariant;
                                OLECHAR         uniBuffer[128];

                                // we cannot directly use REXX to create objects
                                // so we have to first create a OLE Array Variant
                                // and let this be converted by the OREXXOLE.C functions
                                if (num > 0)
                                {
                                    ArrayBound.cElements = num;
                                    ArrayBound.lLbound = 0;   // zero-based
                                    pSafeArray = SafeArrayCreate(VT_VARIANT,(UINT) 1, &ArrayBound);
                                    VariantInit(pVarArray);
                                    V_VT(pVarArray) = VT_ARRAY | VT_VARIANT;
                                    V_ARRAY(pVarArray) = pSafeArray;

                                    for (long i = 0; i < num; i++)
                                    {
                                        // convert to unicode
                                        C2W(uniBuffer, names[i], 128);
                                        // create a BSTR variant
                                        VariantInit(&sVariant);
                                        V_VT(&sVariant) = VT_BSTR;
                                        V_BSTR(&sVariant) = SysAllocString(uniBuffer);
                                        SafeArrayPutElement(pSafeArray, &i, &sVariant);
                                        // release the char* from the string array
                                        free(names[i]);
                                    }
                                }
                                // release the string array
                                free(names);
                                result = Variant2Rexx(pVarArray);
                                *RetCode = 0;
                            }
                        }
                    }
                }
                else
                {
                    *RetCode = 4; // can only read the info!
                }

                break;
            default:
                *RetCode = -2;    // Bad SelectorType
                break;
        }   //  switch(SelectorType)
    }   //  if (engine)
    else ;  // >>> ??? <<< Good question, what do we do? Error, or return NULL string?
    // result = RexxString("");

    return result;
}

/* unknown callback                                                     */
/* this will deal with objects that REXX is unaware of, but the engine  */
/* is...                                                                */
REXXOBJECT REXXENTRY engineDispatch(REXXOBJECT arguments)
{
    char       *objname = string_data(arguments);
    RexxObject *result = NULL; // NULL indicates error
    ooRexxScript  *engine = findEngineForThread(GetCurrentThreadId());
    HRESULT    hResult;
    DISPID     dispID;
    DWORD      flags;
    IDispatch *pDispatch = NULL;
    ITypeInfo *pTypeInfo = NULL;
    VARIANT    temp;

    // do we have an engine to work with?
    if (engine)
    {
        engine->setObjectCreation(false); // turn off IE security manager checking
        engine->PreventCallBack();        // Don't pick up properties.  This stops our GetDispID() from returning DispID's for properties.
        hResult = engine->getNamedItems()->WhoKnows(objname,engine->Lang,&dispID,&flags,&pDispatch,&pTypeInfo);
        engine->ReleaseCallBack();
        if (SUCCEEDED(hResult))
        {
            VariantInit(&temp);
            // do we have a real named item?
            if (dispID == -1)
            {

                V_VT(&temp) = VT_DISPATCH;
                temp.pdispVal = pDispatch;
                // convert it to an OLEObject
                // because the top activation on the activity might not be
                // "native", we have to make sure that it is
                // (Win...Kernel() must never be executed concurrently!)
                result = Variant2Rexx(&temp);
                // this must be a variable / constant
            }
            else
            {
                if (pDispatch == engine)
                {
                    hResult = engine->LocalParseProcedureText(objname,0,&pDispatch);
                    V_VT(&temp) = VT_DISPATCH;
                    temp.pdispVal = pDispatch;
                }
                else hResult = GetProperty(NULL,pDispatch,engine->Lang,&temp,dispID);
                if (SUCCEEDED(hResult))
                {
                    result = Variant2Rexx(&temp);
                }
                VariantClear(&temp);
            }
        }
        engine->setObjectCreation(true); // turn on possible IE security manager checking


    } /* endif engine to work with */
    return result;
}

/* here we "parse" the text and retrieve the names of all */
/* (public) routines mentioned in there.                  */
/* the name retrieval is done with the exit handler that  */
/* will call back into the engine to add the names...     */
/* parameters: 0 - script text in wide charachters (in)   */
/*             1 - pointer to script engine (in)          */
/*             2 - return code (out)                      */
// this is running in a thread of its own...
int parseText(RexxThreadContext *context, LPCOLESTR pStrCode, LinkedList *pList)
{
    char *script = NULL;

    // convert the unicode source into straight 8-bit code for interpretation.
    script = (char*) malloc(sizeof(char)*(1+wcslen(pStrCode)));
    sprintf(script,"%S",pStrCode);

    RexxMethodObject method = context->createMethod(script, strlen(script));

    free(script);

    count = 0;
    // had an error
    if (method != NULLOBJECT)
    {
        RexxSupplierObject list = context->GetMethodRoutines(method);
        while (context->SupplierAvailable(list))
        {
            RexxObjectPtr name = context->SupplierIndex(list);
            RexxMethodObject value = (RexxMethodObject)context->SupplierValue(list);
            // give this long term GC protection.
            context->RequestGlobalReference(value);

            char *funcname = context->ObjectToStringValue(name);
            // TODO:  need to sort out this list
            myList->AddItem(names[iCount], LinkedList::Beginning,NULL);
            count++;
            context->SupplierNext(vars);
        }
    }
    return count;
}

/* this creates a (flattened) method from the source    */
/* that is passed to us.                                */
/* a "handler" is prepended to allow for execution of   */
/* the immediate code (once) and calls for functions    */
/* inside this method (many times). this is done with   */
/* an interpret statement.                              */
/* parameters: 0 - script text in wide charachters (in) */
/*             1 - pointer to script engine (in)        */
/*             2 - pointer to RXSTRING for result image */
/*                 (out)                                */
RexxMethodObject ooRexxScript::createCode(RexxThreadContext *context, LPCOLESTR pStrCode, RexxDirectoryObject *conditionInfo)
{
    // "normal" local variables
    char        funcHandler[128];
    char       *script = NULL;
    RxString    source;
    RexxReturnCode rc;

    // build "our" version of the script text that does allows us "dispatch" function
    // calls and to run "immediate" code
    sprintf(funcHandler,"if arg(1) \\='' then interpret arg(1)\n");
    script = (char*) malloc(sizeof(char)*(1+wcslen(pStrCode)+strlen(funcHandler)));
    sprintf(script,"%s%S",funcHandler,pStrCode);

    // convert this into a runnable method
    RexxMethodObject method = context->createMethod(script, strlen(script));
    // nothing returned, then we have a condition problem.  Return the
    // condition object describing the problem.
    if (method == NULLOBJECT )
    {
        *conditionInfo = context->GetConditionInfo();
        return NULLOBJECT;
    }

    // keep this globally safe.
    method = context->RequestGlobalReference(method);

    return method;
}


RexxObject *Create_securityObject(RexxThreadContext *context, ooRexxScript *pEngine)
{
    RexxObjectPtr securityObject = NULL;

    /* create an instance of a security manager object */
    if (pEngine->getSafetyOptions())
    {
        DISPPARAMS dp;
        VARIANT temp;
        OLECHAR invokeString[32];
        char args[32];
        ConditionData condData; // condition info
        ooRexxScriptError *ErrObj;
        bool        ErrObj_Exists;
        APIRET      rc;
        HRESULT     hResult=S_OK;

        sprintf(args,"FLAGS=%d",pEngine->getSafetyOptions());
        C2W(invokeString,args,32);
        VariantInit(&temp);
        V_VT(&temp) = VT_BSTR;
        V_BSTR(&temp) = SysAllocString(invokeString);
        dp.cArgs = 1;
        dp.cNamedArgs = 0;
        dp.rgdispidNamedArgs = NULL;
        dp.rgvarg = &temp;

        //TODO:  redo this....

        rc = ooRexxRunMethod(pEngine->getEngineName(), pEngine->getSecurityManager(),&dp,DispParms2RexxArray,NULL,&securityObject,NULL,&condData);
        VariantClear(&temp);

        //TODO:  redo the condition handling.
        if (condData.rc)
        {
            // an error occured: init excep info
            ErrObj = new ooRexxScriptError(NULL,&condData,&ErrObj_Exists);
            hResult = pEngine->getScriptSitePtr()->OnScriptError((IActiveScriptError*) ErrObj);
            // init to empty again....
            if (ErrObj_Exists) ErrObj->UDRelease();
        }  // if(condData->rc)
    }

    return securityObject;
}



/* this runs a previously created method       */
/* parameters: 0 - pointer to engine           */
/*             1 - ptr to image of meth. block */
/*             2 - COM parameters, if needed   */
/*             2 - REXX parameters, if needed  */
/*             3 - REXX result object          */
/*             4 - VARIANT result              */
/*             5 - ConditionData pointer       */
/*             6 - end thread?                 */
/*             7 - get variables?              */
//TODO:  Store a thread context in the engine?
//TODO:  Split this into two different methods, one for Rexx arguments/value, one for OLE version.

void ooRexxScript::runMethod(RexxCallContext *context, DISPPARMS *parms,
    Variant **vResult, RexxDirectoryObject *condData, bool getVariables);{
    // convert the
    args = DispParms2RexxArray(parms);
    RexxObjectPtr result = NULLOBJECT;
    runMethod(context, args, &result, condData, getVariables);
    Rexx2Variant(context, result, *vResult, VT_EMPTY /*(try your best)*/, 0 /*dummy argument?!*/);
    if (V_VT(*vResult) == VT_ERROR)
    {
        (*vResult)->vt = VT_EMPTY;
    }
}

void ooRexxScript::runMethod(RexxCallContext *context, RexxMethodObject method,
    RexxArrayObject args, RexxObjectPtr *result, RexxDirectoryObject *condData, bool getVariables);{
    // thread dependend registration of the engine
    registerEngineForCallback(pEngine);
    // flag the termination exit that we need to get the variables.
    exitGetVariables = getVariables;

    *result = context->RunMethod(method, args);

    checkRexxException(context, condData);

    // remove engine from thread list
    deregisterEngineForCallback();
}

