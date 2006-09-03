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
#include "ooRexxScrptError.hpp"


ooRexxScriptError::ooRexxScriptError(FILE *Stream, ConditionData *info, bool *Exists)  :
ulRefCount(1),
    logfile(Stream)
{
    if (info)
        memcpy(&RexxErrorInfo,info,sizeof(ConditionData));
    else memset(&RexxErrorInfo,0,sizeof(ConditionData));
    RunDestructor = Exists;
    if (RunDestructor) *RunDestructor = true;
}



ooRexxScriptError::~ooRexxScriptError()
{
    if (RunDestructor) *RunDestructor = false;
}




/****************************************/
/* implementation of IUnknown interface */
/*                           complete + */
/****************************************/

/*************************************************************/
/* ooRexxScriptError::QueryInterface                                 */
/*                                                           */
/* return an interface pointer of the requested IID.         */
/* this means simply casting "this" to the desired interface */
/* pointer, if the interface is supported.                   */
/*************************************************************/
STDMETHODIMP ooRexxScriptError::QueryInterface(REFIID riid, void **ppvObj)
{
    HRESULT hResult = E_NOINTERFACE;
    char    cIID[100],*IIDName,TrulyUnknown[]="??????";


    //  We should look this riid up in HKClass_Root\Interface\.... to print what it represents.  <----

    // a pointer to result storage must be supplied
    if (!ppvObj)
        return ResultFromScode(E_INVALIDARG);
    // set to NULL initiallly
    *ppvObj = NULL;


    // need to supply an IUnknown pointer?
    if (IsEqualIID(riid, IID_IUnknown))
    {
        *ppvObj = (LPVOID)(IUnknown *)(IActiveScript *) this;
    }
    // need to supply an IActiveScriptError pointer?
    else if (IsEqualIID(riid, IID_IActiveScriptError))
    {
        *ppvObj = (LPVOID)(IActiveScriptError *) this;
    }
    else
    {
        if (!(IIDName = NameThatInterface((OLECHAR *)&cIID[0]))) IIDName = &TrulyUnknown[0];
        if (IIDName != &TrulyUnknown[0]) free(IIDName);
    }

    // on success, call AddRef()
    if (*ppvObj != NULL)
    {
        AddRef();
        hResult = NOERROR;
    }

    return hResult;
}

/**********************************/
/* ooRexxScriptError::AddRef              */
/*                                */
/* increment the reference count. */
/**********************************/
STDMETHODIMP_(ULONG) ooRexxScriptError::AddRef()
{
    //  Returns > 0 if ulRefCount > 0, but is not guaranteed
    // to return the actual value of ulRefCount.
    InterlockedIncrement((long *)&ulRefCount);
    return ulRefCount;
}

/***********************************************************/
/* ooRexxScriptError::Release                                      */
/*                                                         */
/* decrement the reference count, if zero, destroy object. */
/***********************************************************/
STDMETHODIMP_(ULONG) ooRexxScriptError::Release()
{

    InterlockedDecrement((long *)&ulRefCount);
    if (ulRefCount)
        return ulRefCount;

    delete this;
    return 0;
}


STDMETHODIMP_(ULONG) ooRexxScriptError::UDRelease()
{

    InterlockedDecrement((long *)&ulRefCount);

    if (RunDestructor) *RunDestructor = false;  //  The creator's pointer to us is no longer valid.
    RunDestructor = NULL;                      //  The ceator of us has gone away.
    if (ulRefCount)
        return ulRefCount;

    delete this;
    return 0;
}


/********************************************************************/
/*                                                                  */
/* IActiveScriptError interface                                     */
/* Fill in error information from ConditionData, gets called in     */
/* return to a call to IActiveScriptSite->OnScriptError             */
/*                                                                  */
/********************************************************************/
HRESULT ooRexxScriptError::GetExceptionInfo(EXCEPINFO *pInfo)
{
    OLECHAR *temp;
    char msg[512];
    short major,minor;

    if (!pInfo) return E_POINTER;

    major = RexxErrorInfo.code/1000;
    minor = RexxErrorInfo.code%1000;
    if (minor)
        sprintf(msg,"[%d.%d] ",major,minor);
    else
        sprintf(msg,"[%d] ",major);

    ooRexxStrcat(msg, sizeof(msg), RexxErrorInfo.errortext.strptr);
    if (RexxErrorInfo.message.strlength > 0 && RexxErrorInfo.message.strlength < 1024)
    {
        ooRexxStrcat(msg, sizeof(msg), " / ");
        ooRexxStrcat(msg, sizeof(msg), RexxErrorInfo.message.strptr);
    }

    // clear info block
    memset(pInfo,0,sizeof(EXCEPINFO));

    temp = (OLECHAR*) malloc(sizeof(OLECHAR)*(strlen(msg)+1));
    //  Should use the C2W() macro to generate this call.
    MultiByteToWideChar( CP_ACP, 0, msg, -1, temp, strlen(msg)+1 );

    pInfo->wCode = major;
    pInfo->bstrDescription = SysAllocString(temp);

    free(temp);

    return S_OK;
}




STDMETHODIMP ooRexxScriptError::GetSourcePosition(DWORD *pdwSourceContext, ULONG *LineNumber, LONG *CharPos)
{
    if (pdwSourceContext) *pdwSourceContext = 0;
    if (LineNumber) *LineNumber = RexxErrorInfo.position;
    // if(LineNumber) *LineNumber = 666;
    if (CharPos) *CharPos = -1;
    return S_OK;
}




STDMETHODIMP ooRexxScriptError::GetSourceLineText(BSTR *Text)
{
    return E_NOTIMPL;
}
