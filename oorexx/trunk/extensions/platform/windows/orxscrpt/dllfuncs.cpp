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
#define  DLLFUNCS_CPP
#include "dllfuncs.hpp"
#include "eng2rexx.hpp"


CRITICAL_SECTION EngineSection = { 0};
HANDLE           semaphore;
HANDLE           mutex;
Index  *thread2EngineList = NULL;  // needed to find the engine that is using the
RexxInstance *interpreter = NULL;  // our interpreter instance
// thread that the REXX code is running in

bool APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{

    ListItem    *Current;
    ooRexxScript   *Content;

    RexxThreadContext *context;


    if ( ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        if (!EngineChain) EngineChain = new LooseLinkedList;
        InitializeCriticalSection(&EngineSection);
        // list to associate engines with the threads it owns
        thread2EngineList = new Index;

        // init REXX for this process, to keep directory entries
        // alive as long as this process is up

        // set up an exit list to use for all of our invocations.
        RexxExitCallback exitsList[] =
        {
            { RexxCatchExternalFunc, RXEXF},         // last ditch function hook
            { RexxNovalueExit, RXNOVAL},             // unknown value hook
            { RexxValueExit, RXVALUE},               // hook to enable the WSHPROPERTY selector on Value()
            { NULL, RXENDLST}
        }

        RexxEngineInfo globalState;
        ooRexxCreateInterpreter(&interpreter, &context, exitList, &globalState);
        // turn off dispatch messages, otherwise the WSH will interfere
        // with the execution of the engine code and will cause everything
        // to hang...
        ooRexxSetProcessMessages(false);

        mutex = CreateMutex(NULL, false, NULL); // we're in deep trouble if this is zero after the call!

    }
    else if ( ul_reason_for_call == DLL_PROCESS_DETACH)
    {

        //  If we are going away before all of the engines are gone,
        // then close them, so they will release their resources.
        if (EngineChain)
        {
            Current = EngineChain->FindItem(0);
            while (Current)
            {
                Content = (ooRexxScript *)Current->GetContent();
                delete Content;
                Current = EngineChain->FindItem();
            }
            delete EngineChain;
        }

        //TODO:  Create "handle based" environment creation".
        // shut down REXX
        interpreter->Terminate();
        interpreter = NULL;

        if (mutex) CloseHandle(mutex);

        // remove this list
        if (thread2EngineList) delete thread2EngineList;
        // remove critical section
        DeleteCriticalSection(&EngineSection);
    }

    return true;
}

STDAPI DllCanUnloadNow(void)
{
    HRESULT RetCode;

    /* The DLL can be unloaded when no OLE object is in existance and no */
    /* lock has been issued against this DLL (for performance reasons).  */
    if ( (ulDllReferences == 0) && (ulDllLocks == 0) )
        RetCode = S_OK;
    else
        RetCode = S_false;
    return RetCode;
}


STDAPI DllGetClassObject(REFCLSID rclsid,
    REFIID riid,
    LPVOID *ppv)
{
    /* This call returns a pointer to the IUnknown interface of the OLE */
    /* class implemented by this DLL.                                   */
    ooRexxClassFactory *pClassFac = NULL;
    HRESULT         hr;

    /* check input arguments */
    if (ppv == NULL)
        return ResultFromScode(E_INVALIDARG);

    if ( (riid != IID_IUnknown) && (riid != IID_IClassFactory) )
        return ResultFromScode(E_INVALIDARG);

    /* create class factory object for this DLL */
    /*  This is what creates the engine object. */
    pClassFac = CreateClassFactory();
    if (pClassFac == NULL)
        return ResultFromScode(E_OUTOFMEMORY);

    /* check that correct classid is requested */
    if (rclsid != pClassFac->GetClassID())
    {
        pClassFac->Release();
        return ResultFromScode(E_INVALIDARG);
    }

    /* query requested interface from class object */
    /*  If that interface is supported, then the object
     * RefCount is incremented, and the Release() does
     * nothing, otherwise the object removes itself.
     */
    hr = pClassFac->QueryInterface(riid, ppv);
    pClassFac->Release();
    return hr;
}


STDAPI DllRegisterServer(void)
{
    /* This call registers the OLE class in the Windows registry. */
    ooRexxClassFactory *pClassFac = NULL;
    HRESULT         hr;

    /* create class factory object */
    pClassFac = CreateClassFactory();
    if (pClassFac == NULL)
        return ResultFromScode(E_UNEXPECTED);

    /* register server and return result to caller */
    hr = pClassFac->RegisterServer();
    pClassFac->Release();
    return hr;
}


STDAPI DllUnregisterServer(void)
{
    /* This call deregisters the OLE class from the Windows registry. */
    ooRexxClassFactory *pClassFac = NULL;
    HRESULT         hr;

    /* create class factory object */
    pClassFac = CreateClassFactory();
    if (pClassFac == NULL)
        return ResultFromScode(E_UNEXPECTED);

    /* unregister server and return result to caller */
    hr = pClassFac->UnregisterServer();
    pClassFac->Release();
    return hr;
}
