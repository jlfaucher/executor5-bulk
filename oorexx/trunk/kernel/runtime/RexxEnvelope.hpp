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
/* REXX Kernel                                                  RexxEnvelope.hpp   */
/*                                                                            */
/* Primitive Envelope Class Definitions                                       */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxEnvelope
#define Included_RexxEnvelope

#include "RexxSmartBuffer.hpp"

/* The following bits define the type of envelope we are creating */
#define MOBILE_ENVELOPE 0x00000001     /* This envelope will be mobile  a mobi*/
#define METHOD_ENVELOPE 0x00000010     /* This envelope used to preserve      */
                                       /* a translated method in EA's         */

#define BEHAVIOUR_NON_PRIMITIVE (1 << ((sizeof(uintptr_t) * 8) - 1))
inline bool ObjectHasNonPrimitiveBehaviour(RexxObject *o) { return (((uintptr_t)((o)->behaviour)) & BEHAVIOUR_NON_PRIMITIVE) != 0; }

 class RexxEnvelope : public RexxObject {
  public:
   void *operator new(size_t);
   inline void  operator delete(void *) {;}
   inline void *operator new(size_t, void *ptr) {return ptr;}
   inline void  operator delete(void *, void *) {;}

   RexxEnvelope();
   inline RexxEnvelope(RESTORETYPE restoreType) { ; };
   void live();
   void liveGeneral();
   void flatten(RexxEnvelope*);
   RexxObject *unflatten(RexxEnvelope *);
   void flattenReference(RexxObject **, size_t, RexxObject **);
   RexxEnvelope *pack(RexxString *, RexxObject *, RexxString *, RexxArray *);
   void        puff(RexxBuffer *, stringchar_t *);
   RexxObject *queryObj(RexxObject *);
   RexxObject *queryProxy(RexxObject *);
   size_t      copyBuffer(RexxObject *);
   void rehash();
   int  queryType();
   stringchar_t *  bufferStart();
   void    associateProxy(RexxObject *o ,RexxObject *p);
   void    addTable(RexxObject *obj);
   void    addProxy(RexxObject *o, RexxObject *p);

   inline RexxSmartBuffer *getBuffer() {return this->buffer;}
   inline RexxObject *getReceiver() {return this->receiver;}
   inline size_t      getCurrentOffset() { return this->currentOffset; }
   inline RexxObjectTable *getDuptable() {return this->duptable;}
   inline RexxObjectTable *getRehashtable() {return this->rehashtable;}
   inline stringchar_t *offsetToAddress(uintptr_t offset)
   {
       return buffer->address() + offset;
   }

   RexxObject *home;
   RexxString *destination;            /* mailing destination               */
   RexxObject *receiver;               /* object to receive the message     */
   RexxString *message;                /* message to issue                  */
   RexxArray  *arguments;              /* array of arguments                */
   RexxObject *result;                 /* returned result                   */
   RexxObjectTable  *duptable;         /* table of duplicates               */
   RexxObjectTable  *savetable;        /* table of protected objects created during flattening */
   RexxSmartBuffer *buffer;            /* smart buffer wrapper              */
   RexxObjectTable  *rehashtable;      /* table to rehash                   */
   RexxStack  *flattenStack;           /* the flattening stack              */
   size_t      currentOffset;          /* current flattening offset         */

   static RexxClass *classInstance;    // the singleton class instance
 };
#endif
