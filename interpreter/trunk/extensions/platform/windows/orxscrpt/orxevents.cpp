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
/******************************************************************************
*
*
*
*
*
******************************************************************************/
#include "orxEvents.hpp"

#define EVENTNAMECONVERT(Item,SubItem,OurName) \
  sprintf(OurName,"%S<-Item (AddScriptlet Event) SubItem->%S",Item,SubItem);

//  The values for these FL (Flag List) arrays are defined in ooRexxIDispatch.cpp
extern FL DispatchInvoke[];


extern FL DispExGetDispID[];

FL ImplementedInterface[] = {           // H - orizontal flags
    {IMPLTYPEFLAG_FDEFAULT,"Default"},
    {IMPLTYPEFLAG_FSOURCE,"Source"},
    {IMPLTYPEFLAG_FRESTRICTED,"Restricted"},
    {IMPLTYPEFLAG_FDEFAULTVTABLE,"Default VTable"},
    {(DWORD)0,(char *)NULL}
};






/*
 *    This creates an empty event chain, and binds it to the engine that created it.
 *  The pointer to the logfile of the engine is also passed.  In this manner the
 *  events can add to the log without requiring access to private properties of the
 *  engine.
 */
ooRexxEvent::ooRexxEvent(ooRexxScript *Script, FILE *pLogFile) : logfile(pLogFile),
    Engine(Script)
{


    EventSourceChain = new LooseLinkedList;
}


ooRexxEvent::~ooRexxEvent()
{
    ListItem    *Current;
    ESource     *Content;

    DisconnectEvents();

    Current = EventSourceChain->FindItem(0);
    while (Current)
    {
        Content = (ESource *)Current->GetContent();
        if (Content->EventType == ESource::AddScriptlet)
        {
            Content->Release();
        }
        Current = EventSourceChain->FindItem();
    }
    delete EventSourceChain;

}



/*
 */
STDMETHODIMP ooRexxEvent::CheckEvent(LPCOLESTR Code, IDispatch **pbIDispatch)
{
    ListItem    *Current;
    ESource     *Content;
    HRESULT      RetCode=DISP_E_UNKNOWNNAME;
    int          i=0,CodeLen;
    RetCode = DISP_E_UNKNOWNNAME;


    if (!Code) RetCode = E_POINTER;
    else CodeLen = wcslen(Code);
    if (!pbIDispatch) RetCode = E_POINTER;
    else *pbIDispatch = (IDispatch *)NULL;
    if (RetCode == DISP_E_UNKNOWNNAME) while (Current = EventSourceChain->FindItem(i++))
        {
            Content = (ESource *)Current->GetContent();
            if (Content->EventType == ESource::ParseProcedure) if (CodeLen == Content->CodeLen)
                    if (Content->Code) if (wcsicmp(Code,Content->Code) == 0)
                        {
                            *pbIDispatch = (IDispatch *)Content;
                            RetCode = S_OK;
                            ((IDispatch*) Content)->AddRef();
                            break;
                        }
        }
    return RetCode;
}



/*
 *    This is the AddEvent() for ParseProcedure().  ParseProcedure() events always get called
 *  with a DispID of 0, therefore, these events are always treated as an individual container.
 */
STDMETHODIMP ooRexxEvent::AddEvent(OLECHAR *pName, LPCOLESTR Code, DISPID SinkDispID,DWORD Flags, IDispatch **pbIDispatch)
{
    ESource  *Current;
    ListItem *Next;
    HRESULT  RetCode=S_OK;
    char     Name[MAX_PATH];


    *pbIDispatch = (IDispatch *)NULL;
    Current = new ESource;
    if (Current == NULL) RetCode = E_OUTOFMEMORY;
    else
    {
        RetCode = Current->InitEvent(pName,Code,SinkDispID,Engine,logfile);
        if (SUCCEEDED(RetCode))
        {
            W2C(&Name[0], pName, wcslen(pName)+1);
            Next = EventSourceChain->AddItem(Name,LinkedList::End,(void *)Current);
            //  Make sure the Event can tell people if it goes away prematurely.
            Current->SetDestructor(EventSourceChain, (void *)Current);
        }
        else Next = NULL;
        if (Next) *pbIDispatch = (IDispatch *)Current;
        else
        {
            delete Current;
            RetCode = SUCCEEDED(RetCode) ? E_OUTOFMEMORY : RetCode;
        }
    }
    return RetCode;
}




STDMETHODIMP ESource::InitEvent(OLECHAR *Name,
    LPCOLESTR  pCode,
    DISPID     SinkDispID,
    ooRexxScript *ORexxScript,
    FILE      *LogFile)
{
    PEMAP      NewMap;
    HRESULT    RetCode;


    EventType = ParseProcedure;
    EventName = SysAllocString(Name);
    Engine = ORexxScript;
    Connected = true;
    ConnectionPoint = NULL;
    Container = NULL;
    logfile = LogFile;
    CodeLen = wcslen(pCode);
    Code = (OLECHAR *) GlobalAlloc(GMEM_FIXED,(CodeLen+1)*sizeof(OLECHAR));
    if (!Code)
    {
        return E_OUTOFMEMORY;
    }
    memcpy(Code,pCode,(CodeLen+1)*sizeof(OLECHAR));

    RetCode = AddMap("0",&NewMap);
    if (FAILED(RetCode))
    {
        GlobalFree(Code);
        Code = NULL;
        return RetCode;
    }
    NewMap->SourceEventName = SysAllocString(Name);
    NewMap->Sink = SinkDispID;

    return S_OK;
}


STDMETHODIMP ooRexxEvent::AddEvent(LPCOLESTR  ItemName,
    LPCOLESTR  SubItemName,
    LPCOLESTR  EventName,
    DISPID     SinkDispID)
{
    ESource   *Current;
    IDispatch *Source;
    char       Name[2*MAX_PATH];
    HRESULT    RetCode=S_OK;


    sprintf(Name,"%S<-Item (AddScriptlet Event) SubItem->%S",ItemName,SubItemName);
    Current = (ESource *)EventSourceChain->FindContent(Name);
    if (!Current)
    {
        RetCode = Engine->GetSourceIDispatch(ItemName,SubItemName,&Source);
        if (RetCode == S_OK)
        {
            Current = new ESource;
            if (Current == NULL) RetCode = E_OUTOFMEMORY;
            else
            {
                RetCode = Current->InitEvent(Source,Engine,logfile);
                EventSourceChain->AddItem(Name,LinkedList::End,(void *)Current);
                //  Make sure the Event can tell people if it goes away prematurely.
                Current->SetDestructor(EventSourceChain, (void *)Current);
            }
        }
    }
    // The InitEvent() uses the MS terminology "function" for event name.
    //  Find the function that is identified by EventFunction, and map it to SinkDispID
    if (Current) RetCode = Current->SetMap(EventName, SinkDispID);


    return RetCode;
}



STDMETHODIMP ESource::InitEvent(IDispatch *SourceDispatch,
    ooRexxScript *ORexxScript,
    FILE *LogFile)
{
    ITypeInfo *SourceType;
    TYPEATTR  *TypeAttributes;
    OLECHAR    lGUID[50];
    BSTR       SourceName;
    unsigned int NameCount;
    int        i;
    FUNCDESC  *FuncDesc;
    char       DispIDName[29];
    PEMAP      NewMap;
    HRESULT    RetCode=S_OK;
    int        EMCount;


    EventType = AddScriptlet;
    Source = SourceDispatch;   // Mimick the ParseProcedures "THIS" parameter by returning this pointer.
    Engine = ORexxScript;
    Connected = false;
    ConnectionPoint = NULL;
    Container = NULL;
    logfile = LogFile;

    RetCode = GetTypeInfo(&SourceType);
    if (SUCCEEDED(RetCode))
    {
        RetCode = SourceType->GetTypeAttr(&TypeAttributes);
        memcpy(&SourceGUID,&TypeAttributes->guid,sizeof(GUID));
        EMCount = TypeAttributes->cFuncs;
        SourceType->ReleaseTypeAttr(TypeAttributes);

        /*    For each entry in the type library, create an entry on the Event Map chain.
         *  This is a many to one relation.  Each of the different Source Disp ID's
         *  will translate to the same Sink Disp ID.  There is only one chunk of code
         *  being bound to this Event that the Type Library is describing.  So every
         *  Source call must map to the same Sink.
         */
        for (i=0; i<EMCount; i++)
        {
            SourceType->GetFuncDesc(i, &FuncDesc);
            //  Despite what the documentation says, this returns Max Names, not Max Names - 1.
            // The first name is the function name, the remainder if they exist are parameters.
            SourceType->GetNames(FuncDesc->memid, &SourceName, 1, &NameCount);
            sprintf(DispIDName,"%d",FuncDesc->memid);
            //  This creates the entry for the function with an invalid DispID to call.
            RetCode = AddMap(DispIDName,&NewMap);
            if (FAILED(RetCode)) return RetCode;
            NewMap->SourceEventName = SourceName;
            SourceType->ReleaseFuncDesc(FuncDesc);
        }

        SourceType->Release();
    }
    return RetCode;
}


/******************************************************************************
*
* returns
*             RetCode           SinkDispID     Meaning
*           ---------           ----------
*  DISP_E_UNKNOWNNAME                   -1     Events for this object have not been
*                                              defined before.
*                S_OK                   -1     This object has events, just not this one.
*                S_OK                   >0     This object has this event.  The Container
*                                              will call us using the value in SinkDispID
*                                              as the identifier.
*
******************************************************************************/
STDMETHODIMP ooRexxEvent::FindEvent(LPCOLESTR  ItemName,
    LPCOLESTR  SubItemName,
    LPCOLESTR  EventName,
    DISPID     *SinkDispID)
{
    ESource   *Current;
    char       Name[2*MAX_PATH];
    HRESULT    RetCode=DISP_E_UNKNOWNNAME;


    *SinkDispID = (DISPID) -1;
    EVENTNAMECONVERT(ItemName,SubItemName,Name)
        // sprintf(Name,"%S<-Item (AddScriptlet Event) SubItem->%S",ItemName,SubItemName);
    Current = (ESource *)EventSourceChain->FindContent(Name);
    if (Current) RetCode = Current->FindMap(EventName, SinkDispID);

    return RetCode;
}



STDMETHODIMP ooRexxEvent::ConnectEvents()
{
    ESource  *Current;
    HRESULT  RetCode=S_OK;


    Current = (ESource *)EventSourceChain->FindContent(0);
    while (Current != NULL)
    {
        /*  Check the connection flag.  DON'T try to connect something that is already connected.*/
        if (Current->EventType == ESource::AddScriptlet && Current->ConnectionPoint == NULL)
        {
            RetCode = Current->Source->QueryInterface( IID_IConnectionPointContainer,
                (void**)&(Current->Container) );


            if (SUCCEEDED(RetCode))
            {
                //Use the interface to get the connection point we want
                RetCode = Current->Container->FindConnectionPoint( Current->SourceGUID,
                    &Current->ConnectionPoint );
                Current->Container->Release();
            }
        }

        if (Current->EventType == ESource::AddScriptlet && SUCCEEDED(RetCode) && !Current->Connected)
        {
            //    Use the connection point to hook up to the event source.
            //  And just to be dogmatic about things, AddRef() the ESource.
            //  We are handing out the ptr to a COM object, to be used as such.
            //  The Unadvise() will Release() it.
            Current->AddRef();
            Current->ConnectionPoint->Advise( (IUnknown*)Current, &Current->Cookie );

            //  The Event is FINALLY connected!
            Current->Connected = true;
        }
        Current = (ESource *)EventSourceChain->FindContent();
    }
    return RetCode;
}



STDMETHODIMP ooRexxEvent::DisconnectEvents()
{
    ESource  *Current;
    HRESULT   RetCode;

    Current = (ESource *)EventSourceChain->FindContent(0);
    while (Current != NULL)
    {
        if (Current->Connected && Current->ConnectionPoint != NULL && Current->EventType == ESource::AddScriptlet)
        {
            RetCode = Current->ConnectionPoint->Unadvise(Current->Cookie);
            Current->Release();
        }
        Current->Connected = false;
        Current = (ESource *)EventSourceChain->FindContent();
    }

    return S_OK;
}










ESource::ESource()
: ulRefCount(1),
    Source(NULL),
    Engine(NULL),
    Connected(false),
    ConnectionPoint(NULL),
    Container(NULL),
    InvokeCount(0),
    EventName(NULL),
    Code(NULL),
    CodeLen(0),
    logfile(NULL)
{
    ;
}




// DTOR
ESource::~ESource()
{
    PEMAP      EventMap;


    if (EventName) SysFreeString(EventName);
    //  Disconnect the old-style of interrupts.
    if (Connected && ConnectionPoint != NULL && EventType == AddScriptlet)
    {
        ConnectionPoint->Unadvise(Cookie);
        ConnectionPoint->Release();
        Release();                         // Decrement our reference to match the unadvise.
    }
    if (EventType == ParseProcedure)
    {
        if (Code) GlobalFree(Code);
    }
    Connected = false;
    // Release the IDispatch pointer used to find the ConnectionPoint.
    if (Source) Source->Release();
    // Release all of the Event Maps for this Source.
    EventFunctionMap.DeleteList();

    EventMap = (PEMAP)EventFunctionMap.FindContent(0);

}


/****************************************/
/* implementation of IUnknown interface */
/*                           complete + */
/****************************************/

/*************************************************************/
/* ESource::QueryInterface                                   */
/*                                                           */
/* return an interface pointer of the requested IID.         */
/* this means simply casting "this" to the desired interface */
/* pointer, if the interface is supported.                   */
/*************************************************************/
STDMETHODIMP ESource::QueryInterface(REFIID riid, void **ppvObj)
{
    HRESULT hResult = E_NOINTERFACE;
    char    cIID[100],*IIDName,TrulyUnknown[]="??????";


    StringFromGUID2(riid,(unsigned short *)cIID,sizeof(cIID)/2);

    // a pointer to result storage must be supplied
    if (!ppvObj)   return E_INVALIDARG;
    // set to NULL initiallly
    *ppvObj = NULL;


    // need to supply an IUnknown pointer?
    if (IsEqualIID(riid, IID_IUnknown))
    {
        *ppvObj = (LPVOID)(IUnknown *)(IActiveScript *) this;
    }
    else if (riid == IID_IDispatchEx)
    {
        *ppvObj = static_cast<IDispatch *>(this);
    }
    else if (riid == IID_IDispatch)
    {
        *ppvObj = static_cast<IDispatch*>(this);
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
/* ESource::AddRef                */
/*                                */
/* increment the reference count. */
/**********************************/
STDMETHODIMP_(ULONG) ESource::AddRef()
{
    //        InterlockedIncrement()
    //  Returns > 0 if ulRefCount > 0, but is not guaranteed
    // to return the actual value of ulRefCount.
    InterlockedIncrement((long *)&ulRefCount);
    return ulRefCount;
}

/***********************************************************/
/* ESource::Release                                        */
/*                                                         */
/* decrement the reference count, if zero, destroy object. */
/***********************************************************/
STDMETHODIMP_(ULONG) ESource::Release()
{
    InterlockedDecrement((long *)&ulRefCount);
    if (ulRefCount)
        return ulRefCount;

    delete this;
    return 0;
}




/******************************************************************************
*   IDispatch Interface -- This interface allows this class to be used as an
*   automation server, allowing its functions to be called by other COM
*   objects
******************************************************************************/

/******************************************************************************
*   GetTypeInfoCount -- This function determines if the class supports type
*   information interfaces or not.  It places 1 in iTInfo if the class supports
*   type information and 0 if it doesn't.
******************************************************************************/
STDMETHODIMP ESource::GetTypeInfoCount(UINT *pTInfo)
{

    //This object doesn't support type information
    *pTInfo = 0;
    return E_NOTIMPL;
}

/******************************************************************************
*   GetTypeInfo -- Returns the type information for the class.  For classes
*   that don't support type information, this function returns DISP_E_BADINDEX;
*   It would be preferable to return E_NOTIMPL, but that is not one of our options.
******************************************************************************/
STDMETHODIMP ESource::GetTypeInfo(UINT pTInfo, LCID plcid,
    ITypeInfo **pTypeInfo)
{

    //This object doesn't support type information
    *pTypeInfo = NULL;
    return DISP_E_BADINDEX;
}


/******************************************************************************
*   GetIDsOfNames -- Takes an array of strings and returns an array of DISPID's
*   which correspond to the methods or properties indicated.  In real life,
*   If the name is not recognized, then DISP_E_UNKNOWNNAME is returned.
*   This should never be called.  Because of the IAdvise, our caller should know
*   all of the DispID's that we will support.
******************************************************************************/
STDMETHODIMP ESource::GetIDsOfNames(REFIID riid,
    OLECHAR **pNames,
    UINT pNamesCount,  LCID plcid,
    DISPID *pbDispID)
{
    HRESULT RetCode = S_OK;
    char    lIID[100];


    StringFromGUID2(riid,(unsigned short *)lIID,sizeof(lIID)/2);

    //check parameters
    if (riid != IID_NULL) RetCode = E_INVALIDARG;

    else
    {
        RetCode = E_NOTIMPL;

    }

    return RetCode;
}

/******************************************************************************
*  Invoke -- Takes a dispid and uses it to call a method or property defined
*  in the script code in the context of this ESource.
******************************************************************************/
STDMETHODIMP ESource::Invoke(DISPID pDispID, REFIID riid, LCID plcid,
    WORD pFlags, DISPPARAMS* pDispParams,
    VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
    UINT* pArgErr)
{

    //  check parameters
    //  According to the Doc, this should always be IID_NULL.
    if (riid != IID_NULL) return E_INVALIDARG;

    return CommonInvoke(pDispID, plcid, pFlags, pDispParams, pVarResult, pExcepInfo);
}




/***** IDispatchEx Methods *****/
/******************************************************************************
*                 GetDispID
******************************************************************************/
STDMETHODIMP ESource::GetDispID(
/* [in] */ BSTR pName,
/* [in] */ DWORD pFlags,               // Derived from fdexName... defines.
/* [out] */ DISPID __RPC_FAR *pbDispID)
{
    HRESULT RetCode= E_NOTIMPL;

    return RetCode;

}


/******************************************************************************
*                 InvokeEx
******************************************************************************/
STDMETHODIMP ESource::InvokeEx(
/* [in] */ DISPID pDispID,
/* [in] */ LCID lcid,
/* [in] */ WORD pFlags,                // Derived from ... defines.
/* [in] */ DISPPARAMS __RPC_FAR *pArgs,
/* [out] */ VARIANT __RPC_FAR *pbResults,
/* [out] */ EXCEPINFO __RPC_FAR *pbErrInfo,
/* [unique][in] */ IServiceProvider __RPC_FAR *pCaller)
{

    return CommonInvoke(pDispID, lcid, pFlags, pArgs, pbResults, pbErrInfo);
}





/******************************************************************************
*                 DeleteMemberByName
******************************************************************************/
STDMETHODIMP ESource::DeleteMemberByName(
/* [in] */ BSTR pName,
/* [in] */ DWORD pFlags)
{               // Derived from fdexName... defines.
    return E_NOTIMPL;
}



/******************************************************************************
*                 DeleteMemberByDispID
******************************************************************************/
STDMETHODIMP ESource::DeleteMemberByDispID(
/* [in] */ DISPID pDispID)
{
    return E_NOTIMPL;
}



/******************************************************************************
*                 GetMemberProperties
******************************************************************************/
STDMETHODIMP ESource::GetMemberProperties(
/* [in] */ DISPID pDispID,
/* [in] */ DWORD pFetchFlag,           // Derived from ???... defines.
/* [out] */ DWORD __RPC_FAR *pbProperties)
{ // Derived from fdexProp... defines.
    return E_NOTIMPL;
}



/******************************************************************************
*                 GetMemberName
******************************************************************************/
STDMETHODIMP ESource::GetMemberName(
/* [in] */ DISPID pDispID,
/* [out] */ BSTR __RPC_FAR *pbName)
{
    HRESULT RetCode=E_NOTIMPL;

    *pbName = SysAllocString(L"");
    return RetCode;
}



/******************************************************************************
*                 GetNextDispID
******************************************************************************/
STDMETHODIMP ESource::GetNextDispID(
/* [in] */ DWORD pFlags,               // Derived from fdexEnum... defines.
/* [in] */ DISPID pDispID,             // Previous DispID returned.
/* [out] */ DISPID __RPC_FAR *pbDispID)
{
    return E_NOTIMPL;
}



/******************************************************************************
*                 GetNameSpaceParent
******************************************************************************/
STDMETHODIMP ESource::GetNameSpaceParent(
/* [out] */ IUnknown __RPC_FAR *__RPC_FAR *pbIUnknown)
{
    return E_NOTIMPL;
}







STDMETHODIMP ESource::AddMap(char *Name, PEMAP *pbNewMap)
{
    PEMAP     Next;
    ListItem *Map;
    HRESULT   RetCode=S_OK;


    *pbNewMap = NULL;
    Next = (PEMAP)GlobalAlloc(GMEM_FIXED,sizeof(EMAP));
    if (Next == NULL) RetCode = E_OUTOFMEMORY;
    else
    {
        memset(Next,0,sizeof(EMAP));
        Map = EventFunctionMap.AddItem(Name,LinkedList::End,(void *)Next);
        if (Next)
        {
            *pbNewMap = Next;
            Next->Sink = -1;
        }
        else
        {
            RetCode = E_OUTOFMEMORY;
            GlobalFree((HGLOBAL)Next);
        }
    }
    return RetCode;
}






STDMETHODIMP ESource::SetMap(LPCOLESTR pName, DISPID SinkDispID)
{
    PEMAP     Map;
    HRESULT   RetCode=S_OK;
    int       tab;

    Map = (PEMAP)EventFunctionMap.FindContent(0);
    /*    For each entry in the type library, create an entry on the Event Map chain.
     *  This is a many to one relation.  Each of the different Source Disp ID's
     *  will translate to the same Sink Disp ID.  There is only one chunk of code
     *  being bound to this Event that the Type Library is describing.  So every
     *  Source call must map to the same Sink.
     */
    while (Map)
    {
        if (wcsicmp(pName,Map->SourceEventName) == 0) break;
        Map = (PEMAP)EventFunctionMap.FindContent();
    }

    if (!Map) RetCode = DISP_E_UNKNOWNNAME;
    else
        Map->Sink = SinkDispID;
    return RetCode;
}






STDMETHODIMP ESource::FindMap(LPCOLESTR Event, DISPID *SinkDispID)
{
    PEMAP     Map;
    HRESULT   RetCode=S_OK;


    *SinkDispID = (DISPID) -1;
    Map = (PEMAP)EventFunctionMap.FindContent(0);
    /*    For each entry in the type library, create an entry on the Event Map chain.
     *  This is a many to one relation.  Each of the different Source Disp ID's
     *  will translate to the same Sink Disp ID.  There is only one chunk of code
     *  being bound to this Event that the Type Library is describing.  So every
     *  Source call must map to the same Sink.
     */
    while (Map)
    {
        if (wcsicmp(Event,Map->SourceEventName) == 0) break;
        Map = (PEMAP)EventFunctionMap.FindContent();
    }

    if (!Map) RetCode = DISP_E_UNKNOWNNAME;
    else *SinkDispID = Map->Sink;
    return RetCode;
}




STDMETHODIMP ESource::GetTypeInfo(ITypeInfo **pbTypeInfo)
{
    HRESULT    RetCode;
    IProvideClassInfo* ClassInfo = NULL;
    TYPEATTR  *TypeAttributes = NULL;
    ITypeInfo *CoClassTypeInfo = NULL,*TypeInfo;
    int        NumInterfaces;
    int        InterfaceFlags;
    int        i;
    HREFTYPE   refType;


    OLECHAR    lGUID[50];
    FUNCDESC  *FuncDesc;
    unsigned int j,k,NameCount,FCount;
    BSTR       SourceEventName[20];
    char       *IIDName,TrulyUnknown[]="??????";

    //make sure we have an source IDispatch to work with.
    if (Source)
    {
        //Get the TypeInfo from the source IDispatch
        RetCode = Source->QueryInterface( IID_IProvideClassInfo,
            (void**)&ClassInfo);
        if (FAILED(RetCode)) return RetCode;

        RetCode = ClassInfo->GetClassInfo( &CoClassTypeInfo );
        ClassInfo->Release();

        if (FAILED(RetCode)) return RetCode;

        //Check the type attributes of the ITypeInfo to make sure it's a coclass
        RetCode = CoClassTypeInfo->GetTypeAttr( &TypeAttributes );
        if (SUCCEEDED(RetCode))
        {
            if (TypeAttributes->typekind != TKIND_COCLASS) return E_NOTIMPL;
        }
        else
            return RetCode;

        //How many interfaces does this coclass implement?
        NumInterfaces = TypeAttributes->cImplTypes;
        //Release the type attributes
        CoClassTypeInfo->ReleaseTypeAttr( TypeAttributes );

        //Now, search through the set of interfaces looking for one that is marked
        //default, source, and not restricted.
        for (i=0; i<NumInterfaces; i++)
        {
            RetCode = CoClassTypeInfo->GetImplTypeFlags( i, &InterfaceFlags );
            if (FAILED(RetCode)) return RetCode;

            RetCode = CoClassTypeInfo->GetRefTypeOfImplType( i, &refType );
            RetCode = CoClassTypeInfo->GetRefTypeInfo( refType, &TypeInfo );
            RetCode = TypeInfo->GetTypeAttr(&TypeAttributes);
            FCount = TypeAttributes->cFuncs;
            StringFromGUID2(TypeAttributes->guid,lGUID,sizeof(lGUID)/sizeof(lGUID[0]));
            if (!(IIDName = NameThatInterface((OLECHAR *)&lGUID[0]))) IIDName = &TrulyUnknown[0];
            if (IIDName != &TrulyUnknown[0]) free(IIDName);
            TypeInfo->ReleaseTypeAttr(TypeAttributes);


            /*    For each entry in the type library, create an entry on the Event Map chain.
             *  This is a many to one relation.  Each of the different Source Disp ID's
             *  will translate to the same Sink Disp ID.  There is only one chunk of code
             *  being bound to this Event that the Type Library is describing.  So every
             *  Source call must map to the same Sink.
             */
            for (k=0; k<FCount; k++)
            {
                TypeInfo->GetFuncDesc(k, &FuncDesc);
                TypeInfo->GetNames(FuncDesc->memid, &(SourceEventName[0]),
                    20,(unsigned int *)&NameCount);
                for (j=0; j<NameCount; j++)
                {
                    SysFreeString(SourceEventName[j]);
                }
                TypeInfo->ReleaseFuncDesc(FuncDesc);
            }

            if ((InterfaceFlags & (IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE |
                IMPLTYPEFLAG_FRESTRICTED)) == (IMPLTYPEFLAG_FDEFAULT |
                IMPLTYPEFLAG_FSOURCE))
            {
                *pbTypeInfo = TypeInfo;
            }
            else TypeInfo->Release();


        }  // for(i=0...
    }
    return S_OK;
    // return E_INVALIDARG;
}




/******************************************************************************
*                 CommonInvoke
*
*   Called by both Invoke() and InvokeEx so that behavior will be identical,
*  and support is centrally located (on place to maintain).
******************************************************************************/
STDMETHODIMP ESource::CommonInvoke(
/* [in] */ DISPID pDispID,
/* [in] */ LCID lcid,
/* [in] */ WORD pFlags,                // Derived from ... defines.
/* [in] */ DISPPARAMS __RPC_FAR *pArgs,
/* [out] */ VARIANT __RPC_FAR *pbResults,
/* [out] */ EXCEPINFO __RPC_FAR *pbErrInfo)
{
    HRESULT  RetCode=S_OK;
    PEMAP   Map;
    char    DispIDName[MAX_PATH];
    DISPPARAMS ASDispParms,*EDispParms;
    VARIANT    Mutant;
    DISPID     NamedArg;

    //  There may be more than one function using the same DispID.
    // Make sure it is one of the functions we are implementing.
    sprintf(DispIDName,"%d",(int)pDispID);
    Map = (PEMAP)EventFunctionMap.FindContent(DispIDName);
    if (Map)
    {
        if ((int)Map->Sink == -1)
        {
            RetCode = DISP_E_UNKNOWNNAME;
        }
    }
    else
    {
        RetCode = DISP_E_UNKNOWNNAME;
    }


    /*   >>>>???<<<<
      Need to add code to examine the pExcepInfo and pArgErr parameters.
    */


    //    Override the parameters to pass the named THIS, if this is AddScriptlet.
    //  Then AddScriplet will emulate the behavior of ParseProcedure.
    if (EventType == AddScriptlet)
    {
        VariantInit(&Mutant);
        V_VT(&Mutant) = VT_DISPATCH;
        V_DISPATCH(&Mutant) = Source;

        NamedArg = DISPID_THIS;

        ASDispParms.rgvarg = &Mutant;
        ASDispParms.rgdispidNamedArgs = &NamedArg;
        ASDispParms.cArgs = 1;
        ASDispParms.cNamedArgs = 1;
        EDispParms = &ASDispParms;
    }
    else EDispParms = pArgs;        // Set the Engine DispParms.




    if (SUCCEEDED(RetCode))
    {
        switch (pFlags)
        {
            case DISPATCH_METHOD:
                RetCode = Engine->InvokeEx(Map->Sink, lcid, pFlags,  EDispParms,
                    pbResults, pbErrInfo, NULL);
                break;
            case DISPATCH_PROPERTYGET:
                if (pbResults)
                {
                    V_VT(pbResults) = VT_DISPATCH;
                    pbResults->pdispVal = this;
                    this->AddRef();
                }
                else
                {
                    RetCode = E_POINTER;   // Let the caller know we did not do anything.
                }
                break;
            case DISPATCH_PROPERTYPUT:
            case DISPATCH_PROPERTYPUTREF:
            case DISPATCH_CONSTRUCT:
            default:
                RetCode = E_NOTIMPL;   // Let the caller know we did not do anything.
        }

    }  // if(SUCCEEDED(RetCode))

    return RetCode;
}



