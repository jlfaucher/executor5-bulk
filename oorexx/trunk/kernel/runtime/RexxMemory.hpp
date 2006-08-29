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
/* REXX Kernel                                                RexxMemory.hpp  */
/*                                                                            */
/* Primitive Memory Class Definitions                                         */
/*                                                                            */
/******************************************************************************/

#ifndef Included_RexxMemory
#define Included_RexxMemory

#include "TableClass.hpp"
#include "StackClass.hpp"
#include "SysSemaphore.hpp"
#include "RootImage.hpp"

class WeakReference;

/* turn this on to get verbose GC output */
//#define VERBOSE_GC

/* The minimum allocation unit for an object.   */
#define ObjectGrain (sizeof(uintptr_t) * 2)
/* The unit of granularity for large allocation */
#define LargeAllocationUnit 1024
/* The unit of granularity for extremely large objects */
#define VeryLargeAllocationUnit 4096
/* Minimum size of an object.  This is not the actual minimum size, */
/* but we allocate objects with an 8-byte granularity */
/* this is the granularity for objects greater than 16Mb. */
#define VeryLargeObjectGrain    256

/* This is the smallest object we'll allocate from storage.  Our */
/* smallest possible object is smaller than this, but we get better */
/* usage by allocating with a larger grain size. */
#define MinimumObjectSize (sizeof(uintptr_t) * 6)
#define ObjectHeaderSize (sizeof(uintptr_t) * 5)
#define MaximumObjectSize (SIZE_MAX - ObjectHeaderSize)

#define ObjectIsLive(o)     (RexxMemory::memoryObject.objectIsLive((RexxObject *)(o)))
#define ObjectIsNotLive(o)  (!ObjectIsLive(o))
#define SetObjectLive(o)    (ObjectHeader(o).setObjectMark(RexxMemory::memoryObject.markWord))
#define ObjectIsMarked(o)   (ObjectHeader(o).isObjectMarked(headerMarkedValue))
#define ClearObjectMark(o)  (ObjectHeader(o).clearObjectMark())

#define IsObjectGrained(o)  ((((size_t)o)%ObjectGrain) == 0)
#define IsValidSize(s) ((s) >= MinimumObjectSize && ((s) % ObjectGrain) == 0)

/* return the size of the object */
#define ObjectSize(o)  (((RexxObject *)(o))->header.size())

#define LARGE_OBJECT_SIZE (1 << 16)
#define IsLargeObject(o)    (ObjectSize(o) >= LARGE_OBJECT_SIZE)

/* largest size we'll keep in the save stack on a mark */
#define SaveStackThreshold   4096

#define ObjectHeader(o)      (((RexxObject *)(o))->header)
#define ClearObjectHeader(o) (ObjectHeader(o).clear())
#define SetOldSpace(o)       (ObjectHeader(o).setOldSpace())

#define ClearObjectSize(o) (ObjectHeader(o).setSize(0))

#define SetObjectSize(o, s) (ObjectHeader(o).setSize(s))

#define SetNewSpace(o)               (ObjectHeader(o).clearOldSpace())
#define SetOldSpace(o)               (ObjectHeader(o).setOldSpace())

#define SetObjectHasNoReferences(o)  (ObjectHeader(o).setNoReferences())
#define SetObjectHasReferences(o)    (ObjectHeader(o).setHasReferences())
#define ObjectHasNoReferences(o)     (ObjectHeader(o).hasNoReferences())
#define ObjectHasReferences(o)       (ObjectHeader(o).hasReferences())
#define OldSpace(o)                  (ObjectHeader(o).isOldSpace())

#define NullOrOld(o)        (((RexxObject *)(o) == OREF_NULL) || OldSpace(o))

#define ObjectIsNonPrimitive(o) (ObjectHeader(o).isNonPrimitive())
#define SetNonPrimitive(o)  (ObjectHeader(o).setNonPrimitive())
#define ClearNonPrimitive(o)  (ObjectHeader(o).clearNonPrimitive())

#define SetUpNewObject(o,s) {                         \
    RexxMemory::memoryObject.setObjectSize((RexxObject *)(o), s); \
    setObjectVirtualFunctions(o);                     \
    (o)->objectVariables = NULL;                      \
    (o)->hashvalue = HASHOREF(o);                     \
    BehaviourSet(o, TheObjectBehaviour);              \
}

#define SetUpNewAllocation(o) {                       \
    setObjectVirtualFunctions(o);                     \
    (o)->objectVariables = NULL;                      \
    (o)->hashvalue = HASHOREF(o);                     \
    BehaviourSet(o, TheObjectBehaviour);              \
    SetObjectLive(o);                                 \
}

inline size_t roundObjectBoundary(size_t n) { return RXROUNDUP(n,ObjectGrain); }
inline size_t roundObjectResize(size_t n) { return RXROUNDUP(n, ObjectGrain); }

#define ObjectDataSize(o) (ObjectSize(o) - ObjectHeaderSize)
#define ObjectDataSpace(o) (&((RexxObject *)(o))->objectVariables)

/* Zero out the state data of an object */
#define ClearObject(o)          memset((void *)ObjectDataSpace(o),'\0', ObjectDataSize(o));
#define ClearObjectLength(o,l)  memset((void *)ObjectDataSpace(o),'\0', l - ObjectHeaderSize);


class RexxActivationFrameBuffer;
class MemorySegment;
class RexxVariable;
class MemorySegmentPool;
class RexxMemory;
                                       /* This class is implemented in      */
                                       /*OS2MEM.C, since the function is    */
                                       /*system dependant.                  */
typedef char MEMORY_POOL_STATE;

class MemorySegmentPool;

class MemorySegmentPoolHeader {
 friend class RexxMemory;
 friend class SysInterpreter;

 protected:
   MemorySegmentPool *next;
   MemorySegment     *spareSegment;
   char  *nextAlloc;
   char  *nextLargeAlloc;
   size_t uncommitted;
   size_t reserved;            // force aligment of the state data....
};

class MemorySegmentPool : public MemorySegmentPoolHeader {
 friend class RexxMemory;
 public:
   void *operator new(size_t size, size_t minSize);
   inline void  operator delete(void *, size_t) {;}
   inline void *operator new(size_t, void *ptr) {return ptr;}
   inline void  operator delete(void *, void *) {;}
   MemorySegmentPool();
   MemorySegment *newSegment(size_t minSize);
   MemorySegment *newLargeSegment(size_t minSize);
   bool           accessNextPool(void);
   MemorySegmentPool *freePool(void); /* CHM - defect 96: add return value */
   MemorySegmentPool *nextPool() {return this->next;}
   void               setNext( MemorySegmentPool *nextPool ); /* CHM - def.96: new function */

 private:
   char           state[8];    // must be at the end of the structure.
};

#include "MemoryStats.hpp"
#include "MemorySegment.hpp"

class RexxMemory : public RexxObject {
  friend class RexxInstructionOptions;
 public:
  inline RexxMemory();
  inline RexxMemory(RESTORETYPE restoreType) { ; };

  inline operator RexxObject*() { return (RexxObject *)this; };
  inline RexxObject *operator=(DeadObject *d) { return (RexxObject *)this; };

  void live();
  void liveGeneral();
  void flatten(RexxEnvelope *);
  static void restoreVirtualFunctions(void *a, int32_t b);

  void        init();
  MemorySegment *newSegment(size_t requestLength, size_t minLength);
  MemorySegment *newLargeSegment(size_t requestLength, size_t minLength);
  RexxObject *oldObject(size_t size);
  RexxObject *newObject(size_t size);
  RexxObject *temporaryObject(size_t size);
  RexxArray  *newObjects(size_t size, size_t count, RexxBehaviour *behaviour);
  RexxObject *clone(RexxObject *objr);
  void        reSize(RexxObject *, size_t);
  void        checkUninit(RexxObjectTable *);
  void        checkSubClasses(RexxObjectTable *);
  void        markObjects(void);
  void        markObjectsMain(RexxObject *);
  void        killOrphans(RexxObject *);
  void        mark(RexxObject *);
  void        markGeneral(RexxObject **);
  void        collect();
  inline RexxObject *saveObject(RexxInternalObject *saveObj) {this->saveTable->add((RexxObject *)saveObj, (RexxObject *)saveObj); return (RexxObject *)saveObj;}
  inline void        discardObject(RexxInternalObject *obj) {this->saveTable->remove((RexxObject *)obj);};
  inline void        removeHold(RexxInternalObject *obj) { this->saveStack->remove((RexxObject *)obj); }
  void        discardHoldObject(RexxInternalObject *obj);
  RexxObject *holdObject(RexxInternalObject *obj);
  bool        savingImage() { return saveimage; }
  bool        restoringImage() { return restoreimage; }
  RexxObject *dump();
  void        dumpObject(RexxObject *objectRef, FILE *outfile);
  void        setObjectOffset(size_t offset);
  void        setEnvelope(RexxEnvelope *);
  inline void        setOrphanCheck(bool orphancheck) {this->orphanCheck = orphancheck; };
  RexxObject *checkSetOref(RexxObject *, RexxObject **, RexxObject *, char *, size_t);
  RexxObject *setOref(RexxObject **index, RexxObject *value);
  RexxStack  *getFlattenStack();
  void        returnFlattenStack();
  RexxObject *reclaim();
  RexxObject *gutCheck();
  void        accessPools();
  void        accessPools(MemorySegmentPool *);
  void        freePools();
  MemorySegmentPool *freePools(MemorySegmentPool *);
  void        addWeakReference(WeakReference *ref);
  void        checkWeakReferences();
  void        addProxiedObject(RexxObject *object, RexxString *name);
  RexxString *getObjectProxy(RexxObject *object);
  RexxObject *getProxiedObject(RexxString *proxy);
  void        liveStackFull();
  void        dumpMemoryProfile();
  char *      allocateImageBuffer(size_t size);
  void        logVerboseOutput(char *message, void *sub1, void *sub2);
  inline void verboseMessage(char *message) {
#ifdef VERBOSE_GC
      logVerboseOutput(message, NULL, NULL);
#endif
  }

  inline void verboseMessage(char *message, size_t sub1) {
#ifdef VERBOSE_GC
      logVerboseOutput(message, (void *)sub1, NULL);
#endif
  }

  inline void verboseMessage(char *message, size_t sub1, size_t sub2) {
#ifdef VERBOSE_GC
      logVerboseOutput(message, (void *)sub1, (void *)sub2);
#endif
  }

  inline void logObjectStats(RexxObject *obj) { imageStats->logObject(obj); }
  inline void pushSaveStack(RexxObject *obj) { saveStack->push(obj); }
  inline void removeSavedObject(RexxObject *obj) { saveStack->remove(obj); }
  inline void setObjectSize(RexxObject *o, size_t s)
  {
      ObjectHeader(o).setSize(s);
  }

  inline bool objectIsLive(RexxInternalObject *obj) {return ObjectHeader(obj).isObjectLive(markWord); }
  inline bool objectIsNotLive(RexxInternalObject *obj) {return ObjectHeader(obj).isObjectDead(markWord); }
  inline void disableOrefChecks() { checkSetOK = false; }
  inline void enableOrefChecks() { checkSetOK = true; }

  void        checkAllocs();
  RexxObject *dumpImageStats();
  void        createLocks();
  void        scavengeSegmentSets(MemorySegmentSet *requester, size_t allocationLength);
  void        setUpMemoryTables(bool restoringImage);
  void        updateMemoryPool(MemorySegmentPool *newPool);

  void        saveImage();
  void        destroyImage();
  RootImage  *loadImage();
  static RootImage *createImage();
  static RootImage *restoreImage();

  uint32_t markWord;                   /* current marking counter           */
  SysMutex flattenMutex;               /* locks for various memory processes */
  SysMutex unflattenMutex;
  SysMutex envelopeMutex;
  RexxVariable *variableCache;         /* our cache of variable objects     */

  // the singleton memory object
  static RexxMemory memoryObject;

private:
  inline void checkLiveStack() { if (!liveStack->checkRoom()) liveStackFull(); }
  inline void pushLiveStack(RexxObject *obj) { checkLiveStack(); liveStack->fastPush(obj); }
  inline RexxObject * popLiveStack() { return (RexxObject *)liveStack->fastPop(); }
  inline void bumpMarkWord() { markWord ^= MarkMask; }
  inline void restoreMark(RexxObject *markObject, RexxObject **pMarkObject) {
                                       /* we update the object's location   */
      *pMarkObject = (RexxObject *)((size_t)markObject + relocation);
  }

  inline void unflattenMark(RexxObject *markObject, RexxObject **pMarkObject) {
                                       /* do the unflatten                  */
      *pMarkObject = markObject->unflatten(this->envelope);
  }

  inline void restoreObjectMark(RexxObject *markObject, RexxObject **pMarkObject) {
                                         /* update the object reference       */
      markObject = (RexxObject *)((uintptr_t)markObject + objOffset);
      SetObjectLive(markObject);         /* Then Mark this object as live.    */
      *pMarkObject = markObject;         /* now set this back again           */
  }


/* object validation method --used to find and diagnose broken object references       */
  void saveImageMark(RexxObject *markObject, RexxObject **pMarkObject);
  void orphanCheckMark(RexxObject *markObject, RexxObject **pMarkObject);

  bool inObjectStorage(RexxObject *obj);
  bool inSharedObjectStorage(RexxObject *obj);
  bool objectReferenceOK(RexxObject *o);
  void newProcess();

  RootImage  *rootImage;               // the root image object
  RexxStack  *liveStack;               // stack used for GC pushes
  RexxStack  *flattenStack;            // second stack used for object flattening operations
  RexxSaveStack    *saveStack;         // short-term memory protections
  RexxObjectTable  *saveTable;         // longer-term saved memory anchors
  RexxObjectTable  *old2new;           // table for tracking new generation objects stored in image
  MemorySegmentPool *firstPool;        /* First segmentPool block.          */
  OldSpaceSegmentSet oldSpaceSegments;
  NormalSegmentSet newSpaceNormalSegments;
  LargeSegmentSet  newSpaceLargeSegments;
  char *image_buffer;                  /* the buffer used for image save/restore operations */
  size_t image_offset;                 /* the offset information for the image */
  size_t relocation;                   /* image save/restore relocation factor */
  bool dumpEnable;                     /* enabled for dumps?                */
  bool checkSetOK;                     /* OREF checking is enabled          */
                                       /* enabled for checking for bad      */
                                       /*OREF's?                            */
  bool orphanCheck;
  size_t objOffset;                    /* offset of arriving mobile objects */
                                       /* envelope for arriving mobile      */
                                       /*objects                            */
  RexxEnvelope *envelope;
  RexxStack *originalLiveStack;        /* original live stack allocation    */
  MemoryStats *imageStats;             /* current statistics collector      */

  size_t allocations;                  /* number of allocations since last GC */
  size_t collections;                  /* number of garbage collections     */

  // static control variables
  static bool  saveimage;                /* saving the image                  */
  static bool  restoreimage;             /* restoring the image               */

  // the following two fields are used for synchronization of shared memory.
  // localMemoryPool is static, which means it resides in process-local memory.
  // globalMemoryPool is an instance variable of the memory object, which is allocated
  // in shared memory (if this is a shared memory system).  Thus this is globally
  // accessible from all processes.  Each process checks for differences between these
  // two values to determine if it is necessary to synch up the memory pools again.
  static MemorySegmentPool *localMemoryPool;
  MemorySegmentPool *globalMemoryPool;

};

/******************************************************************************/
/* Memory management macros                                                   */
/******************************************************************************/

inline void protectObject(RexxInternalObject *oref)  { RexxMemory::memoryObject.saveObject(oref); }
inline void unprotectObject(RexxInternalObject *oref) { RexxMemory::memoryObject.discardObject(oref); }
inline void holdObject(RexxInternalObject *oref) { RexxMemory::memoryObject.holdObject(oref); }

inline RexxObject *new_object(size_t s)
{
    return RexxMemory::memoryObject.newObject(s);
}

inline RexxArray *new_arrayOfObjects(size_t size, size_t count, RexxBehaviour *behaviour)
{
    return RexxMemory::memoryObject.newObjects(size, count, behaviour);
}


#define setUpMemoryMark                \
 {                                     \
   int32_t headerMarkedValue = RexxMemory::memoryObject.markWord | OldSpaceBit;

#define cleanUpMemoryMark               \
 }

#define setUpMemoryMarkGeneral       {
#define cleanUpMemoryMarkGeneral     }

#define setUpFlatten(type)        \
  {                               \
  stringsize_t  newSelf = envelope->currentOffset; \
  type *newThis = (type *)this;

#define cleanUpFlatten                    \
 }

#define ObjectNeedsMarking(oref) ((oref) != OREF_NULL && !ObjectIsMarked(oref))
#define memory_mark(oref)  if (ObjectNeedsMarking(oref)) RexxMemory::memoryObject.mark((RexxObject *)(oref))
#define memory_mark_general(oref) (RexxMemory::memoryObject.markGeneral((RexxObject **)&(oref)))

/* Following macros are for Flattening and unflattening of objects  */
#define flatten_reference(oref,envel)  if (oref) envel->flattenReference((RexxObject **)&newThis, newSelf, (RexxObject **)&(oref))

#endif
