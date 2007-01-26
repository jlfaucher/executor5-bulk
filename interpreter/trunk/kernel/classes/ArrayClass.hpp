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
/* REXX Kernel                                               ArrayClass.hpp   */
/*                                                                            */
/* Primitive Array Class Definitions                                          */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxArray
#define Included_RexxArray

#define MAX_FIXEDARRAY_SIZE 100000000
#define ARRAY_EXTEND_EXTRA_SIZE  10
#define ARRAY_EXTEND_EXTRA_LARGE_SIZE  100
#define ARRAY_MIN_SIZE          4

#define RaiseBoundsNone     0x00000000
#define RaiseBoundsUpper    0x00000001
#define RaiseBoundsInvalid  0x00000002
#define RaiseBoundsTooMany  0x00000004
#define RaiseBoundsAll      0x0000000F
#define ExtendUpper         0x00000010


typedef struct copyElelmentParm {
   arraysize_t firstChangedDimension;
   RexxArray *newArray;
   RexxArray *newDimArray;
   RexxArray *oldDimArray;
   arraysize_t deltaDimSize;
   arraysize_t copyElements;
   arraysize_t skipElements;
   RexxObject **startNew;
   RexxObject **startOld;
} COPYELEMENTPARM;


 class RexxArray : public RexxObject {
  public:

   inline void  operator delete(void *) {;}
   inline void *operator new(size_t, void *ptr) {return ptr;}
   inline void  operator delete(void *, void *) {;}
   void * operator new(size_t, RexxObject **, arraysize_t, RexxClass *);
   inline void operator delete(void *, RexxObject **, arraysize_t, RexxClass *) {;}
   void * operator new(size_t, RexxObject *);
   inline void operator delete(void *, RexxObject *) {;}
   void * operator new(size_t, RexxObject *, RexxObject *);
   inline void operator delete(void *, RexxObject *, RexxObject *) {;}
   void * operator new(size_t, RexxObject *, RexxObject *, RexxObject *);
   inline void operator delete(void *, RexxObject *, RexxObject *, RexxObject *) {;}
   void * operator new(size_t, RexxObject *, RexxObject *, RexxObject *, RexxObject *);
   inline void operator delete(void *, RexxObject *, RexxObject *, RexxObject *, RexxObject *) {;}
   void * operator new(size_t, arraysize_t, RexxObject **);
   inline void operator delete(void *, arraysize_t, RexxObject **) {;}
   void * operator new(size_t, size_t, RexxClass *cls = TheArrayClass);
   inline void operator delete(void *, arraysize_t, RexxClass *cls) {;}
   inline void operator delete(void *, RexxObject **) { ; }
   inline RexxArray(RESTORETYPE restoreType) { ; };
   inline RexxArray() { ; };
   inline ~RexxArray() { ; };

   void         init(arraysize_t, arraysize_t);
   void         live();
   void         liveGeneral();
   void         flatten(RexxEnvelope *);
   RexxObject  *copy();
   RexxArray   *makeArray();
   RexxArray   *allItems();
   RexxArray   *allIndices();
   RexxString  *makeString(RexxString *);
   RexxString  *primitiveMakeString();
   RexxObject  *getRexx(RexxObject **, arraysize_t);
   void         put(RexxObject * eref, arraysize_t pos);
   void         putApi(RexxObject * eref, arraysize_t pos);
   RexxObject  *putRexx(RexxObject **, arraysize_t);
   RexxObject  *remove(size_t);
   RexxObject  *removeRexx(RexxObject **, arraysize_t);
   void         setExpansion(RexxObject * expansion);
   RexxInteger *available(size_t position);
   bool         validateIndex(RexxObject **, arraysize_t, arraysize_t, arraysize_t *, arraysize_t);
   RexxInteger *sizeRexx();
   RexxObject  *firstRexx();
   RexxObject  *lastRexx();
   RexxObject  *nextRexx(RexxObject *);
   RexxObject  *previousRexx(RexxObject *);
   RexxArray   *section(arraysize_t, arraysize_t);
   RexxObject  *sectionRexx(RexxObject *, RexxObject *);
   RexxObject  *sectionSubclass(arraysize_t, arraysize_t);
   RexxInteger *hasIndex(RexxObject *);
   bool         hasIndexApi(arraysize_t);
   RexxObject  *hasIndexRexx(RexxObject **, size_t);
   arraysize_t  numItems();
   RexxObject  *items();
   RexxObject  *dimension(RexxObject *);
   arraysize_t  getDimension();
   RexxObject  *supplier();
   RexxObject  *join(RexxArray *);
   RexxObject  *insert(RexxObject *, arraysize_t);
   RexxArray   *extend(arraysize_t);
   void         shrink(arraysize_t);
   arraysize_t  indexOf(RexxObject *);
   void         deleteItem(arraysize_t);
   void         insertItem(RexxObject *, arraysize_t);
   RexxArray   *extendMulti(RexxObject **, arraysize_t, arraysize_t);
   void         resize();
   RexxObject  *newRexx(RexxObject **, arraysize_t);
   RexxObject  *of(RexxObject **, arraysize_t);

   inline void         addLast(RexxObject *item) { this->insertItem(item, this->size() + 1); }
   inline void         addFirst(RexxObject *item) { this->insertItem(item, 1); }
   inline RexxArray   *array() { return this->makeArray(); }
   inline arraysize_t  size() { return this->expansionArray->arraySize; }
   inline RexxObject  *get(arraysize_t pos) { return (this->data())[pos-1];}
          RexxObject  *getApi(arraysize_t pos);
   inline RexxObject **data() { return this->expansionArray->objects; }
   inline RexxObject **data(arraysize_t pos) { return &((this->data())[pos-1]);}
   inline RexxArray   *getExpansion() { return this->expansionArray; }
   inline RexxString  *indexToStringRep(arraysize_t, char*, arraysize_t*);  // def. 1048
   static void createInstance(RootImage *rootImage);

   arraysize_t arraySize;              /* current size of array         */
   arraysize_t maximumSize;            /* Maximum size array can grow   */
   RexxArray *dimensions;              /* Array containing dimensions - null if 1-dimensional */
   RexxArray *expansionArray;          /* actual array containing data  */
   RexxObject  *objects[1];            /* Data.                         */


   static RexxClass *classInstance;    // the array class object
   static RexxArray *nullArray;        // singleton null array instance
 };

inline RexxArray *new_externalArray(arraysize_t s, RexxClass *c)
{
    return new (s, c) RexxArray;
}

inline RexxArray *new_array(arraysize_t s)
{
    return new (s) RexxArray;
}

inline RexxArray *new_array(RexxObject *o1)
{
    return new (o1) RexxArray;
}

inline RexxArray *new_array(RexxObject *o1, RexxObject *o2)
{
    return new (o1, o2) RexxArray;
}

inline RexxArray *new_array(RexxObject *o1, RexxObject *o2, RexxObject *o3)
{
    return new (o1, o2, o3) RexxArray;
}

inline RexxArray *new_array(RexxObject *o1, RexxObject *o2, RexxObject *o3, RexxObject *o4)
{
    return new (o1, o2, o3, o4) RexxArray;
}

#endif
