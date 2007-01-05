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
/* REXX function activators                                                   */
/*                                                                            */
/* Classes for implementing the external function cache                       */
/*                                                                            */
/******************************************************************************/

#ifndef Included_FunctionActivator
#define Included_FunctionActivator

#include "RexxCore.h"
#include "oorexx.h"

class RexxString;

typedef RexxReturnCode RexxEntry ooRexxObjectFunctionHandler(RexxCallContext *,
    RexxNumber, RexxObjectPtr *, RexxObjectPtr *);

typedef RexxReturnCode RexxEntry ooRexxScriptFunctionHandler(RexxCallContext *,
    RexxNumber, RexxObjectPtr *, void **, RexxObjectPtr *);

class FunctionActivator : public RexxInternalObject
{
public:
    inline FunctionActivator(RESTORETYPE restoreType) { ; };
    inline FunctionActivator(RexxString *n)
    {
        name = n;
    }
    void        live();
    void        liveGeneral();

    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &) { ; }
    inline RexxString *getName() { return name; }

protected:
    RexxString *name;          // name of the function
};

// abstract class for native functions with a resolved address
class NativeActivator : public FunctionActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline NativeActivator(RESTORETYPE restoreType) : FunctionActivator(restoreType) { ; };
    inline NativeActivator(RexxString *n, PFN e) : FunctionActivator(n), entryPoint(e) { ; }
    inline void updateEntryPoint(PFN e) { entryPoint = e; }

protected:
    PFN entryPoint;            // resolved name of the entry point
};

// call a native function in the Classic Rexx style
class ClassicNativeActivator : public NativeActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline ClassicNativeActivator(RESTORETYPE restoreType) : NativeActivator(restoreType) { ; };
    inline ClassicNativeActivator(RexxString *n, PFN e) : NativeActivator(n, e) { ; }
    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &);
};

// call a native function in the Object Rexx style
class ObjectNativeActivator : public NativeActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline ObjectNativeActivator(RESTORETYPE restoreType) : NativeActivator(restoreType) { ; };
    inline ObjectNativeActivator(RexxString *n, PFN e) : NativeActivator(n, e) { ; }
    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &);
};


// call a native function in the type marshalled Object Rexx style
class TypedNativeActivator : public NativeActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline TypedNativeActivator(RESTORETYPE restoreType) : NativeActivator(restoreType) { ; };
    inline TypedNativeActivator(RexxString *n, PFN e) : NativeActivator(n, e) { ; }
    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &);
};


// call a native function registered as part of a script context
class ScriptNativeActivator : public NativeActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline ScriptNativeActivator(RESTORETYPE restoreType) : NativeActivator(restoreType) { ; };
    inline ScriptNativeActivator(RexxString *n, PFN e, void **d) : NativeActivator(n, e) , descriptor = d { ; }
    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &);

protected:
    void **descriptor;             // external environment attached opaque descriptor
};


// abstract class for functions in method form
class MethodActivator : public FunctionActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline MethodActivator(RESTORETYPE restoreType) : FunctionActivator(restoreType) { ; };
    inline MethodActivator(RexxString *n, RexxMethod *m) : FunctionActivator(n), method(m) { ; }
    void        live();
    void        liveGeneral();
    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &);

protected:
    RexxMethod *method;        // the created method name
};

// activation for processing a function written in Rexx
class RexxFunctionActivator : public MethodActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline RexxFunctionActivator(RESTORETYPE restoreType) : MethodActivator(restoreType) { ; };
    inline RexxFunctionActivator(RexxString *n, RexxMethod *m) : MethodActivator(n, m) { ; }
    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &);
    void flatten(RexxEnvelope * envelope);
};

// activation for processing a function written in Rexx
class WeakRexxActivator : public RexxFunctionActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline WeakRexxActivator(RESTORETYPE restoreType) : RexxFunctionActivator(restoreType) { ; };
    WeakRexxActivator(RexxString *n, RexxString *f, RexxMethod *m);
    void        live();
    void        liveGeneral();
    virtual void call(RexxActivation *, RexxObject **, size_t, RexxString *, ProtectedObject &);

protected:
    WeakReference *resolvedMethod;      // weakly anchored method object
    RexxString *fileName;               // the resolved file name
};

// abstract class for native functions registered with a script context
class ScriptNativeActivator : public FunctionActivator
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline NativeActivator(RESTORETYPE restoreType) : FunctionActivator(restoreType) { ; };
    inline NativeActivator(RexxString *n, PFN e) : FunctionActivator(n), entryPoint(e) { ; }
    inline void updateEntryPoint(PFN e) { entryPoint = e; }

protected:
    void **descriptor;         // registered opaque data descriptor
};

#endif

