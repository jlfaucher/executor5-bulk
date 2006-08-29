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
/* Kernel                                                       RexxMemory.c    */
/*                                                                            */
/* Memory Object                                                              */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "RexxCore.h"
#include "RexxMemory.hpp"
#include "StackClass.hpp"
#include "StringClass.hpp"
#include "MutableBufferClass.hpp"
#include "DirectoryClass.hpp"
#include "RexxActivity.hpp"
#include "RexxNativeCode.hpp"
#include "IntegerClass.hpp"
#include "ArrayClass.hpp"
#include "TableClass.hpp"
#include "Interpreter.hpp"
#include "WeakReference.hpp"
#include "SysInterpreter.hpp"
#include "SysFileSystem.hpp"
#include "PointerClass.hpp"
#include "RelationClass.hpp"
#include "QueueClass.hpp"
#include "MessageClass.hpp"
#include "SupplierClass.hpp"

/* NOTE:  There is just a single memory object in global storage.  We'll define      */
/* memobj to be the direct address of this memory object.                            */
RexxMemory RexxMemory::memoryObject;

#define LiveStackSize  16370         /* live stack size                   */

#define SaveStackSize 20             /* newly created objects to save */
#define SaveStackAllocSize 500       /* pre-allocation for save stack  */

#define MaxImageSize 500000          /* maximum startup image size */

bool  RexxMemory::saveimage = false;       /* saving the image                  */
bool  RexxMemory::restoreimage = false;    /* restoring the image               */

MemorySegmentPool *RexxMemory::localMemoryPool = NULL;

void RexxMemory::restoreVirtualFunctions(void *a, int32_t b)
{
  setVirtualFunctions(a, b);
}

void logMemoryCheck(FILE *outfile, char *message, ...)
{
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    if (outfile != NULL) {
        vfprintf(outfile, message, args);
    }
    va_end(args);
}


RexxMemory::RexxMemory()
/******************************************************************************/
/* Function: Main Constructor for Rexxmemory, called once during main         */
/*  initialization.  Will create the initial memory Pool(s), etc.             */
/******************************************************************************/
{
  /* we need to set a valid size for this object.  We round it up */
  /* to the minimum allocation boundary, even though that might be */
  /* a lie.  Since this never participates in a sweep operation, */
  /* this works ok in the end. */
  SetObjectSize(this, roundObjectBoundary(sizeof(RexxMemory)));
  globalMemoryPool = this->firstPool; /* indicate current pool addrs       */
  localMemoryPool = this->firstPool;

                                       /* make sure we hand craft memory's  */
                                       /*  header information.              */

                                       /* See if we want this debug info    */
                                       /*or not by default, its always      */
                                       /*setable from OREXX code.           */
#ifdef CHECKOREFS
  this->orphanCheck = true;            /* default value for OREF checking   */
#else
  this->orphanCheck = false;           /* default value for OREF checking   */
#endif

  this->hashvalue = HASHOREF(this);
                                       /* OR'ed into object headers to      */
                                       /*mark                               */
  this->markWord = 1;
                                       /* objects as being alive in mark    */
                                       /*phase                              */
  this->saveStack = NULL;
  this->saveTable = NULL;
  this->dumpEnable = false;
  this->objOffset = 0;
  this->envelope = NULL;
  this->rootImage = NULL;             // no root object yet

  this->envelopeMutex.create();        // create the mutexs
  this->flattenMutex.create();
  this->unflattenMutex.create();
}

void RexxMemory::init()
/******************************************************************************/
/* Function:  Initialize the memory and restore the image                     */
/******************************************************************************/
{
  disableOrefChecks();                 /* Make sure we don't try to validate*/
  // make sure the memory object is properly initialized
  BehaviourSet(&memoryObject, TheMemoryBehaviour);
                                       /*  OrefSets just yet.               */
  // initialize our memory pools again...this will give us memory to work with
  new (&newSpaceNormalSegments) NormalSegmentSet(this);
  new (&newSpaceLargeSegments) LargeSegmentSet(this);

  // and the new/old space segment set
  new (&oldSpaceSegments) OldSpaceSegmentSet(this);

  collections = 0;
  allocations = 0;
  variableCache = OREF_NULL;

                                       /* NOTE: we don't set livestack      */
                                       /*via the  OrefSet macro, since we   */
                                       /*are so early on in the system,     */
                                       /*that we can't use it right now,    */
                                       /*we will fix this all               */
  liveStack = (RexxStack *)oldSpaceSegments.allocateObject(SegmentDeadSpace);
                                       /* remember the original one         */
  originalLiveStack = liveStack;
  /* make sure we have an inital segment set to allocate from. */
  newSpaceNormalSegments.getInitialSet();
}


void RexxMemory::destroyImage()
/******************************************************************************/
/* Function:  Shutdown the Rexx image                                         */
/******************************************************************************/
{
   freePools();                   // just free the memory pools
}

RootImage *RexxMemory::createImage()
/******************************************************************************/
/* Function:  Initial memory setup during image build                         */
/******************************************************************************/
{
  // set up the initial memory object
  memoryObject.init();
  memoryObject.rootImage = new RootImage();         // create the root image object
  // create the base class image set
  RexxClass::createInstance(memoryObject.rootImage);/* get the CLASS class created       */
  RexxIntegerClass::createInstance(memoryObject.rootImage);

  // create non root class objects
  CreateClassObject(String, RexxClass);
  CreateClassObject(Object, RexxClass);
  CreateClassObject(Table, RexxClass);
  CreateClassObject(Relation, RexxClass);

  // set up the singleton objects
  RexxObject::createInstance(memoryObject.rootImage);

  /* Now get our savestack and         */
  /*savetable                          */
  memoryObject.setUpMemoryTables(false);

  // return the root image object so subsequent initialization stages
  // can anchor root image objects in it.
  return memoryObject.rootImage;
}

void RexxMemory::logVerboseOutput(char *message, void *sub1, void *sub2)
/******************************************************************************/
/* Function:  Log verbose output events                                       */
/******************************************************************************/
{
    logMemoryCheck(NULL, message, sub1, sub2);
}

void RexxMemory::dumpMemoryProfile()
{
  FILE *outFile;                       /* memory stat output file           */

  outFile = fopen("memory.prf","wb");  /* open the file                     */
  /* have each of the memory segments dump their own profile information */
  newSpaceNormalSegments.dumpMemoryProfile(outFile);
  newSpaceLargeSegments.dumpMemoryProfile(outFile);
  fclose(outFile);                     /* close the file                    */
}


void RexxMemory::dumpObject(RexxObject *objectRef, FILE *outfile)
/******************************************************************************/
/* Arguments:  none,                                                          */
/*                                                                            */
/*  Function: Dump out the contents of an object, to screen and optionally    */
/*       a file.                                                              */
/******************************************************************************/
{
 uintptr_t *dmpPtr;
 uintptr_t *ObjEnd;

 ObjEnd = (uintptr_t *)((uintptr_t)objectRef + ObjectSize(objectRef));
 for (dmpPtr = (uintptr_t *)objectRef; dmpPtr <= ObjEnd ; dmpPtr += 4 ) {
    logMemoryCheck(outfile, "  >Parent Dump -->%p   %p   %p   %p \n", *dmpPtr, *(dmpPtr+1), *(dmpPtr+2), *(dmpPtr+3));
 }
}

bool RexxMemory::inSharedObjectStorage(RexxObject *object)
/******************************************************************************/
/* Arguments:  Any OREF                                                       */
/*                                                                            */
/*  Returned:  true if object is in object storage, else false                */
/******************************************************************************/
{
  /* 1st Check Old Space. */
  if (oldSpaceSegments.isInSegmentSet(object))
      return true;

  /* Now Check New Space.              */
  if (newSpaceNormalSegments.isInSegmentSet(object))
      return true;

  /* Now Check New Space large segments*/
  if (newSpaceLargeSegments.isInSegmentSet(object))
      return true;

  /* this is bad, definitely very bad... */
  return false;
}


bool RexxMemory::inObjectStorage(RexxObject *object)
/******************************************************************************/
/* Arguments:  Any OREF                                                       */
/*                                                                            */
/*  Returned:  true if object is in object storage, else false                */
/******************************************************************************/
{
  /* check for a few valid locations in 'C' storage                     */
  if ((object >= (RexxObject *)RexxBehaviour::primitiveBehaviours && object <= (RexxObject *)(RexxBehaviour::primitiveBehaviours + T_Last_Primitive_Class))    ||
      (object == (RexxObject *)this)) {
    return true;
  }

  return inSharedObjectStorage(object);
}


/* object validation method --used to find and diagnose broken object references       */
bool RexxMemory::objectReferenceOK(RexxObject *o)
/******************************************************************************/
/* Function:  Object validation...used to find and diagnose broken object     */
/* references.                                                                */
/******************************************************************************/
{
    if (!inObjectStorage(o)) {
        return false;
    }
    RexxBehaviour *type = objectType(o);
    if (inObjectStorage((RexxObject *)type) && objectType(type) == TheBehaviourBehaviour) {
        return true;
    }
    /* these last two checks are for very early checking...we can */
    /* possibly be testing this before TheBehaviourBehaviour is */
    /* set up, so we have two additional means of verifying the */
    /* behaviour object. */
    return(objectTypeNumber(type) == T_Behaviour) ||
        type == ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Behaviour]));
}


void RexxMemory::markObjectsMain(RexxObject *rootObject)
/******************************************************************************/
/* Function:  Main memory_mark driving loop                                   */
/******************************************************************************/
{
  RexxObject *markObject;
  setUpMemoryMark

  allocations = 0;
  pushLiveStack(OREF_NULL);            /* push a unique terminator          */
  mark(rootObject);                    /* OREF_ENV or old2new               */
  for (markObject = popLiveStack();
       markObject != OREF_NULL;        /* test for unique terminator        */
       markObject = popLiveStack()) {
                                       /* mark behaviour live               */
    memory_mark((RexxObject *)markObject->behaviour);
    /* Mark other referenced obj.  We can do this without checking */
    /* the references flag because we only push the object on to */
    /* the stack if it has references. */
    allocations++;
    markObject->live();
  }

  cleanUpMemoryMark
}


void  RexxMemory::killOrphans(RexxObject *rootObject)
/******************************************************************************/
/* Function:  Garbage collection validity check routine                       */
/******************************************************************************/
{
  RexxObject *mref;

  setUpMemoryMarkGeneral

  /* This is the debugging mark code that traverses the object tree from          */
  /* OREF_ENV, verifying that the objects and their OREFs look OK, specifically   */
  /* that each one is in object storage, and has a behaviour whose behaviour      */
  /* is OREF_BEHAV_B, behaviour's behaviour.                                      */

  /* push a unique terminator          */
  pushLiveStack(OREF_NULL);
  // push the root marking object
  pushLiveStack(rootObject);
  memory_mark_general(rootObject);     /* start from the very tip-top       */
  memory_mark_general(TheNilObject);   /* use .nil to mark the stack        */
                                       /* mark .nil behaviour live          */
  memory_mark_general(TheNilObject->behaviour);
                                       /* mark .nil ovds      live          */
  memory_mark_general(TheNilObject->objectVariables);
  for (mref = popLiveStack();
       mref != OREF_NULL;              /* test for unique terminator        */
                                       /* get the next marked item          */
       mref = popLiveStack()) {

    /* OREF_NIL is used as a marker on the stack.  These markers will be used to  */
    /* recreate the ancestry of any broken object found.  At this point though,   */
    /* finding one indicates that we've already marked all of the descendants of  */
    /* the object just below the OREF_NIL, so we'll leave the OREF_NIL popped,    */
    /* and pop off the parent object which is no longer of any interest.          */
    if (mref == TheNilObject) {        /* found a marker                    */
                                       /* pop off the completed parent      */
       popLiveStack();
       continue;                       /* and back to top of loop           */
    }
                                       /* mark behaviour live               */
    memory_mark_general(mref->behaviour);
                                       /* object have any references?       */
    if (ObjectHasReferences(mref)){
       /* Though this guy is already marked, and as far as gc, we're done with him,    */
       /* we'll now push him back onto the livestack followed by an OREF_NIL marker.   */
       /* If we find an invalid object later, we'll be able to find all the ancestors  */
       /* by looking for the OREF_NIL objects on the livestack.                        */
                                       /* push the ancestor object          */
       pushLiveStack(mref);
                                       /* push an ancestor marker           */
       pushLiveStack(TheNilObject);
       mref->liveGeneral();            /* tell the ancestor to mark his kids*/
    }                                  /* object have any references?      */
  }

  cleanUpMemoryMark
}

void RexxMemory::checkUninit(RexxObjectTable *processTable)
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
{
    // iterate across all of the processes, checking each uninit table
    for (HashLink index = processTable->first();
        processTable->index(index) != OREF_NULL;
        index = processTable->next(index))
    {
        /* retrieve the next process uninit*/
        /*  table.                         */
        RexxInterpreter *process = (RexxInterpreter *)processTable->value(index);
        // get the process uninit table
        RexxObjectTable *uninitTable = process->uninitTable;
        /* now iterate through the uninit  */

        /* table and any object is isn't   */
        /* alive, we indicate it should be */
        /* sent unInit.  We indiacte this  */
        /* by setting the value to 1,      */
        /* instead of NIL (the default)    */
        RexxObject *obj;
        for (HashLink uninitIndex = uninitTable->first();
            (obj = uninitTable->index(uninitIndex)) != OREF_NULL;
            uninitIndex = uninitTable->next(uninitIndex))
        {

            /* is this object now dead?        */
            if (!objectIsLive(obj))
            {
                /* yes, indicate object is to be   */
                /*  sent uninit.                   */
                uninitTable->replace(TheTrueObject, uninitIndex);
                RexxInterpreter::addPendingUninit();
            }
        }
    }

    // now go back through the processed tables again, marking each of
    // the uninit tables in turn.

    for (HashLink index = processTable->first();
        processTable->index(index) != OREF_NULL;
        index = processTable->next(index))
    {
        /* retrieve the next process uninit*/
        /*  table.                         */
        RexxInterpreter *process = (RexxInterpreter *)processTable->value(index);
        // get the process uninit table
        RexxObjectTable *uninitTable = process->uninitTable;

        if (orphanCheck)
        {
            killOrphans(uninitTable);    // mark this uninit table now
        }
        else
        {
            markObjectsMain(uninitTable);    // mark this uninit table now
        }
    }                                    /* now go check next object in tabl*/
}

void RexxMemory::checkSubClasses
   (RexxObjectTable *subClasses)       /* table of subclasses               */
/******************************************************************************/
/* Function:  Scan the table of subclasses for now dead class objects         */
/******************************************************************************/
{
    HashLink    position;                /* position within the table         */
    RexxObject *classObject;             /* object to check                   */

                                         /* check each VALUE position, not the*/
                                         /* index positions for dead objects  */
                                         /* the values are the subclasses, and*/
                                         /* are the ones we're trying to clean*/
                                         /* out of memory                     */
    for (position = subClasses->first(); (classObject = subClasses->value(position)) != OREF_NULL; position = subClasses->next(position))
    {
        /* is this object now dead?          */
        while (objectIsNotLive(classObject) && !OldSpace(classObject))
        {
            /* remove from the table             */
            subClasses->removeItem(classObject, subClasses->index(position));
            /* see of something moved into this  */
            /* slot                              */
            classObject = subClasses->value(position);
            if (classObject == OREF_NULL)    /* cleared the chain for this?       */
                break;                         /* done clearing                     */
        }
    }
}

void RexxMemory::markObjects(void)
/******************************************************************************/
/* Function:   Main mark routine for garbage collection.  This reoutine       */
/*  Determines which mark routine to call and does all additional processing  */
/******************************************************************************/
{
    verboseMessage("Beginning mark operation\n");

    if (this->orphanCheck)
    {             /* debugging bad OREF's?             */
                  /* yup, call debugging mark          */
        this->killOrphans(this);
        // now check for unreferenced objects in the uninit table.
        this->checkUninit(rootImage->activeProcesses);
    }
    else
    {
        /* call normal,speedy,efficient mark */
        this->markObjectsMain(this);
        // now check for unreferenced objects in the uninit table.
        this->checkUninit(rootImage->activeProcesses);
    }

    // now process the weak reference queue...
    checkWeakReferences();
    /* have to expand the live stack?    */
    if (this->liveStack != this->originalLiveStack)
    {
        free((void *)this->liveStack);     /* release the old one               */
                                           /* and set back to the original      */
        this->liveStack = this->originalLiveStack;
    }
    verboseMessage("Mark operation completed\n");
}


/******************************************************************************/
/* Function:  Scan the weak reference queue looking for either dead weak      */
/* objects or weak references that refer to objects that have gone out of     */
/* scope.  Objects with registered notification objects (that are still in    */
/* scope) will be moved to a notification queue, which is processed after     */
/* everything is scanned.                                                     */
/******************************************************************************/
void RexxMemory::checkWeakReferences()
{
    WeakReference *current = rootImage->weakReferenceList;
    WeakReference *previous = OREF_NULL;
    // list of "live" weak references...built while scanning
    WeakReference *newList = OREF_NULL;

    // loop through the list
    while (current != OREF_NULL)
    {
        // this reference still in scope?
        if (objectIsLive(current))
        {
            // have a reference?
            if (current->referentObject != OREF_NULL)
            {
                // if the object is still alive, keep it in the list
                if (objectIsLive(current->referentObject))
                {
                    // keep this one in the list
                    current->nextReferenceList = newList;
                    newList = current;
                }
                else
                {
                    // this is now cleared
                    current->referentObject = OREF_NULL;
                    // cleared objects get removed from the list
                    current->nextReferenceList = OREF_NULL;
                }
            }
            else
            {
                // cleared objects get removed from the list
                current->nextReferenceList = OREF_NULL;
            }
        }
    }

    // update the list
    rootImage->weakReferenceList = newList;
}



MemorySegment *RexxMemory::newSegment(size_t requestedBytes, size_t minBytes)
{
  MemorySegment *segment;

#ifdef MEMPROFILE
  printf("Allocating a new segment of %d bytes\n", requestedBytes);
#endif
  /* first make sure we've got enough space for the control */
  /* information, and round this to a proper boundary */
  requestedBytes = roundSegmentBoundary(requestedBytes + MemorySegmentOverhead);
#ifdef MEMPROFILE
  printf("Allocating boundary a new segment of %d bytes\n", requestedBytes);
#endif
                                       /*Get a new segment                  */
  segment = localMemoryPool->newSegment(requestedBytes);
                                       /* Did we get a segment              */
  if (segment == NULL) {
      /* Segmentsize is the minimum size request we handle.  If */
      /* minbytes is small, then we're just adding a segment to the */
      /* small pool.  Reduce the request to SegmentSize and try again. */
      /* For all other requests, try once more with the minimum. */
      minBytes = roundSegmentBoundary(minBytes + MemorySegmentOverhead);
      /* try to allocate once more...if this fails, the caller will */
      /* have to handle it. */
      segment = localMemoryPool->newSegment(minBytes);
  }
  return segment;                      /* return the allocated segment      */
}


MemorySegment *RexxMemory::newLargeSegment(size_t requestedBytes, size_t minBytes)
/******************************************************************************/
/* Function:  Allocate a segment of the requested size.  The requested size   */
/* is the desired size, while the minimum is the absolute minimum we can      */
/* handle.  This takes care of the overhead accounting and additional         */
/* rounding.  The requested size is assumed to have been rounded up to the    */
/* next "appropriate" boundary already, and the segment overhead will be      */
/* allocated from that part, if possible.  Otherwise, and additional page is  */
/* added.                                                                     */
/******************************************************************************/
{
  MemorySegment *segment;

#ifdef MEMPROFILE
  printf("Allocating large new segment of %d bytes\n", requestedBytes);
#endif
  /* first make sure we've got enough space for the control */
  /* information, and round this to a proper boundary */
  requestedBytes = roundSegmentBoundary(requestedBytes + MemorySegmentOverhead);
#ifdef MEMPROFILE
  printf("Allocating large boundary new segment of %d bytes\n", requestedBytes);
#endif
                                       /*Get a new segment                  */
  segment = localMemoryPool->newLargeSegment(requestedBytes);
                                       /* Did we get a segment              */
  if (segment == NULL) {
      /* Segmentsize is the minimum size request we handle.  If */
      /* minbytes is small, then we're just adding a segment to the */
      /* small pool.  Reduce the request to SegmentSize and try again. */
      /* For all other requests, try once more with the minimum. */
      minBytes = roundSegmentBoundary(minBytes + MemorySegmentOverhead);
      /* try to allocate once more...if this fails, the caller will */
      /* have to handle it. */
      segment = localMemoryPool->newLargeSegment(minBytes);
  }
  return segment;                      /* return the allocated segment      */
}


RootImage *RexxMemory::loadImage()
/******************************************************************************/
/* Function:  Restore a saved image to usefulness.                            */
/******************************************************************************/
{
  size_t imagesize;                      /* size of the image                 */
  RexxBehaviour *imageBehav;           /*behaviour of OP object in image    */

  // go load the image into a buffer
  SysInterpreter::loadImage(BASEIMAGE, image_buffer, imagesize);
                                     /* get the relocation factor         */
  relocation = (size_t)image_buffer - sizeof(size_t);
  char *objectPointer = image_buffer; /* address the start of the image    */
                                     /* and the ending location           */
  char *endPointer = image_buffer + imagesize;
                                     /* the saved context is the 1st object */
  rootImage = (RootImage *)image_buffer;
  restoreimage = true;               /* we are now restoring              */
                                     /* loop through the image buffer     */
  while (objectPointer < endPointer)
  {
                                     /*Fixup Behaviour pointer for obj.   */
                                     /* We don't do an OrefSet here, since*/
                                     /* too early, and we don't need to be*/
                                     /* concerned about object in pbehav  */

                                     /* Retrieve the behaviour obj        */
                                     /* or a copy?                        */
    if (ObjectIsNonPrimitive(objectPointer)) {
                                     /* Working with a copy, don't use    */
                                     /* PBEHAV version.                   */
                                     /* Make sure static behaviour inf    */
                                     /* is resolved before using the      */
                                     /* Behaviour.                        */
      imageBehav = (RexxBehaviour *)(relocation + (intptr_t)((RexxObject *)objectPointer)->behaviour);
                                     /* give this object it behaviour.    */
      ((RexxObject *)objectPointer)->behaviour = (RexxBehaviour *)imageBehav;
    }
    else {
                                     /* originally a primitive;  the      */
                                     /* type number is the behaviour      */
      ((RexxObject *)objectPointer)->behaviour = RexxBehaviour::restoreSavedPrimitiveBehaviour(((RexxObject *)objectPointer)->behaviour);
    }
    /* This will be an OldSpace object.  We delay setting this */
    /* until now, because the oldspace bit is overloaded with the */
    /* NonPrimitive bit.  Since the we are done with the */
    /* non-primitive bit now, we can use this for the oldspace */
    /* flag too. */
    SetOldSpace(objectPointer);
                                     /* Force fix-up of                   */
                                     /*VirtualFunctionTable,              */
    setVirtualFunctions(objectPointer, ((RexxObject *)objectPointer)->behaviour->typenum());

                                     /* Does this object have any         */
                                     /*references?                        */
    if (ObjectHasReferences(objectPointer))
                                     /*  Yes, mark other referenced objs  */
      ((RexxObject *)objectPointer)->liveGeneral();
                                     /* Point to next object in image..   */
    objectPointer += ObjectSize(objectPointer);

  }

  restoreimage = false;
                                     /* restore all of the saved primitive*/
  for (size_t i = 0; i <= T_Last_Exported_Class; i++)
  {
                                     /* behaviours into this array        */
      RexxBehaviour::primitiveBehaviours[i].restore((RexxBehaviour *)rootImage->primitiveBehaviours->get(i + 1));
  }

  // reset the memory object behaviour...this sets it to the behaviour
  // that was just reloaded from the image.
  BehaviourSet(this, TheMemoryBehaviour);

  /* Retrieve special saved objects    */
  /* OREF_ENV and primitive behaviours */
  /* are already restored              */

  // restore the various saved global objects

  TheKernel      = rootImage->interpreterAnchor;

  TheTrueObject  = rootImage->trueObject;
  TheFalseObject = rootImage->falseObject;

  TheNilObject   = rootImage->nilObject;
  TheNullArray   = rootImage->nullArray;
  TheNullPointer = rootImage->nullPointer;
  TheClassClass  = rootImage->classClass;
  TheActivityManager = rootImage->activityManager;
  RexxInterpreter::commonRetrievers = rootImage->commonRetrievers;
  RexxInterpreter::staticRequires = rootImage->staticRequires;
  RexxInterpreter::publicRoutines = rootImage->publicRoutines;

  // Now restore each of the class objects
  RestoreClassObject(Object, RexxClass);
  RestoreClassObject(Class, RexxClass);
                                       /* (CLASS is already restored)       */
  RestoreClassObject(String, RexxClass);
  RestoreClassObject(Array, RexxClass);
  RestoreClassObject(Directory, RexxClass);
  RestoreClassObject(Integer, RexxIntegerClass);
  RestoreClassObject(List, RexxClass);
  RestoreClassObject(Message, RexxClass);
  RestoreClassObject(Method, RexxClass);
  RestoreClassObject(NumberString, RexxClass);
  RestoreClassObject(Queue, RexxClass);
  RestoreClassObject(Stem, RexxClass);
  RestoreClassObject(Supplier, RexxClass);
  RestoreClassObject(Table, RexxClass);
  RestoreClassObject(Relation, RexxClass);
  RestoreClassObject(MutableBuffer, RexxClass);
  RestoreClassObject(Pointer, RexxClass);

  SetOldSpace(this);          /* Mark Memory Object as OldSpace    */

  /* initialize the tables used for garbage collection. */
  setUpMemoryTables(true);
  return rootImage;
}

void RexxMemory::saveImage(void)
/******************************************************************************/
/* Function:  Save the memory image as part of interpreter build              */
/******************************************************************************/
{
  FILE *image;
  RexxObject *markObject;
  MemoryStats imageStats;
                                       /* array of objects needed at front  */
                                       /*of image for faster restore.       */
  size_t i;                            /* loop counter                      */

  this->imageStats = &imageStats;      /* set the pointer to the current collector */
                                       /* of image for faster restore.      */
  imageStats.clear();                  /* clear out image counters          */

                                       /* create the behaviour array        */
  rootImage->primitiveBehaviours = new_array(T_Last_Exported_Class + 1);
                                       /* copy all of the primitive         */
  for (i = 0; i <= T_Last_Exported_Class; i++)
  {
                                       /* behaviours into this array        */
      rootImage->primitiveBehaviours->put((RexxObject *)&RexxBehaviour::primitiveBehaviours[i], i+1);
  }

  // make sure the non-saved elements are removed.  We don't need them any more.
  rootImage->prepareForImageSave();

  // ok, we've zeroed out all of the non-saved image elements.  Now force a GC
  // cycle to make sure we've cleared out any weak references that are still
  // hanging around.

  collect();

  // now remove the weak reference pointer.  Any existing weak references will
  // just become normal objects after a restore image because the old space objects
  // they reference will never be garbage collected.
  rootImage->weakReferenceList = OREF_NULL;

  image_buffer = (char *)malloc(MaxImageSize);
  image_offset = sizeof(size_t);
  saveimage = true;
  disableOrefChecks();                 /* Don't try to check OrefSets now.  */
  bumpMarkWord();

  setUpMemoryMarkGeneral

  pushLiveStack(OREF_NULL);            /* push a unique terminator          */
  memory_mark_general(rootImage);      /* push for marking and go!          */

  for (markObject = popLiveStack();
       markObject != OREF_NULL;        /*   test for unique terminator      */
       markObject = popLiveStack()) {
                                       /* The mark of this object moved it  */
                                       /*to the image, its behaviour now    */
                                       /*contains its offset in the image   */
                                       /* so point to the object in the     */
                                       /*image.                             */
                                       /* the buffer copy                   */
    RexxObject *copyObject = (RexxObject *)(image_buffer+(uintptr_t)markObject->behaviour);

    copyObject->liveGeneral();         /* mark other referenced objs        */
                                       /* non-primitive behaviour?          */
    if (ObjectIsNonPrimitive(copyObject))
                                       /* mark/move behaviour live      */
      memory_mark_general(copyObject->behaviour);
  }
  cleanUpMemoryMarkGeneral

  image = fopen(BASEIMAGE, "wb");
                                       /* PLace actual size at beginning of buffer*/
  memcpy(image_buffer, &image_offset, sizeof(size_t));
                                       /* Write out buffer (image)      */
  fwrite(image_buffer, 1, image_offset, image);
  fclose(image);
  free(image_buffer);

#ifdef PRINT_IMAGE_STATS
  printf("Object stats for this image save are \n");
  imageStats.printSavedImageStats();
  printf("\n\n Total bytes for this image %d bytes \n", image_offset);
#endif
  // we call exit directly...we don't allow anything else to run after an image
  // creation operation.
  exit(RC_OK);
}

void RexxMemory::live(void)
/******************************************************************************/
/* Arguments:  None                                                           */
/*                                                                            */
/*  Returned:  Nothing                                                        */
/*                                                                            */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->saveStack);
  memory_mark(this->saveTable);
  memory_mark(this->old2new);
  memory_mark(this->envelope);
  memory_mark(this->variableCache);
  memory_mark(this->rootImage);
  cleanUpMemoryMark
}

void RexxMemory::liveGeneral(void)
/******************************************************************************/
/* Arguments:  None                                                           */
/*                                                                            */
/*  Returned:  Nothing                                                        */
/*                                                                            */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->saveStack);
  memory_mark_general(this->saveTable);
  memory_mark_general(this->old2new);
  memory_mark_general(this->envelope);
  memory_mark_general(this->variableCache);
  memory_mark_general(this->rootImage);
  cleanUpMemoryMarkGeneral

}

void RexxMemory::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Arguments:  None                                                           */
/*                                                                            */
/*  Returned:  OREF NULL,                                                     */
/******************************************************************************/
{
  /* Nothing to flatten */ this;
}


RexxObject* RexxMemory::reclaim()
/******************************************************************************/
/* Function:  Interface method for external control of garbage collection.    */
/* This forces a GC cycle.                                                    */
/******************************************************************************/
{
    /* just drive the GC operation */
    collect();
    return OREF_NULL;
}


void RexxMemory::collect()
/******************************************************************************/
/* Function:  Collect all dead memory in the Rexx object space.  The          */
/* collection process performs a mark operation to mark all of the live       */
/* objects, followed by sweep of each of the segment sets.                    */
/******************************************************************************/
{
                                       /* can't be interupted for this      */
  CriticalBlock block;

  collections++;
  verboseMessage("Begin collecting memory, cycle #%d after %d allocations.\n", collections, allocations);
  allocations = 0;

  /* change our marker to the next value so we can distinguish */
  /* between objects marked on this cycle from the objects marked */
  /* in the pervious cycles. */
  bumpMarkWord();

  /* do the object marking now...followed by a sweep of all of the */
  /* segments. */
  markObjects();
  newSpaceNormalSegments.sweep();
  newSpaceLargeSegments.sweep();

  /* The space segments are now in a known, completely clean state. */
  /* Now based on the context that caused garbage collection to be */
  /* initiated, the segment sets may be expanded to add additional */
  /* free memory.  The decision to expand the object space requires */
  /* the usage statistics collected by the mark-and-sweep */
  /* operation. */

  verboseMessage("End collecting memory\n");
  verboseMessage("Object save table contains %d objects\n", this->saveTable->items());
}

RexxObject *RexxMemory::oldObject(size_t requestLength)
/******************************************************************************/
/* Arguments:  Requested length                                               */
/*                                                                            */
/*  Returned:  New object, or OREF_NULL if requested length was too large     */
/******************************************************************************/
{
  RexxObject *newObject;

  /* Compute size of new object and determine where we should */
  /* allocate from */
  requestLength = roundObjectBoundary(requestLength);
  newObject = oldSpaceSegments.allocateObject(requestLength);

  /* if we got a new object, then perform the final setup steps. */
  /* Since the oldspace objects are special, we don't push them on */
  /* to the save stack.  Also, we don't set the oldspace flag, as */
  /* those are a separate category of object. */
  if (newObject != OREF_NULL) {
      /* setup the new object header for use */
      SetUpNewObject(newObject, requestLength);
  }

  /* return the newly allocated object to our caller */
  return newObject;
}

char *RexxMemory::allocateImageBuffer(size_t imageSize)
/******************************************************************************/
/* Function:  Allocate an image buffer for the system code.  The image buffer */
/* is allocated in the oldspace segment set.  We create an object from that   */
/* space, then return this object as a character pointer.  We eventually will */
/* get that pointer passed back to us as the image address.                   */
/******************************************************************************/
{
    return (char *)oldObject(imageSize);
}


RexxObject *RexxMemory::newObject(size_t requestLength)
/******************************************************************************/
/* Arguments:  Requested length                                               */
/*                                                                            */
/*  Returned:  New object, or OREF_NULL if requested length was too large     */
/******************************************************************************/
{
  RexxObject *newObject;

  allocations++;

  /* Compute size of new object and determine where we should */
  /* allocate from */
  requestLength = roundObjectBoundary(requestLength);

  /* is this a typical small object? */
  if (requestLength <= LargeBlockThreshold) {
      /* make sure we don't go below our minimum size. */
      if (requestLength < MinimumObjectSize) {
          requestLength = MinimumObjectSize;
      }
      newObject = newSpaceNormalSegments.allocateObject(requestLength);
      /* feat. 1061 moves the handleAllocationFailure code into the initial */
      /* allocation parts; this way an "if" instruction can be saved.       */
      if (newObject == NULL) {
        newObject = newSpaceNormalSegments.handleAllocationFailure(requestLength);
      }
  }
  /* we keep the big objects in a separate cage */
  else {
      newObject = newSpaceLargeSegments.allocateObject(requestLength);
      if (newObject == NULL) {
          newObject = newSpaceLargeSegments.handleAllocationFailure(requestLength);
      }
  }

  /* complete the construction of this */
  SetUpNewAllocation(newObject);

  if (this->saveStack != OREF_NULL) {
                                       /* saveobj doesn't get turned on     */
                                       /*until the system is initialized    */
                                       /*far enough but once its on, push   */
                                       /*this new obj on the save stack to  */
                                       /*keep him from being garbage        */
                                       /*collected, before it can be used   */
                                       /*and safely anchored by caller.     */
      pushSaveStack(newObject);
  }
  /* return the newly allocated object to our caller */
  return newObject;
}


RexxObject *RexxMemory::clone(RexxObject *obj)
/******************************************************************************/
/* Arguments:  Clone an object, and set up its header.  This method should    */
/*             be called by other _copy methods instead of using new_object   */
/*             and memcpy, so that memory can properly initialize the new     */
/*             object's header to avoid early gc.                             */
/*                                                                            */
/*  Returned:  A new object copied from objr, but set to be live to avoid     */
/*             being garbage collected on a pending sweep.                    */
/******************************************************************************/
{
  RexxObject *clone;
  ObjectHeader  newHeader;
  size_t size = ObjectSize(obj);
                                       /* get new object same size as this  */
  clone = newObject(size);
  /* copy the header information, and adjust the state bits. */
  newHeader = ObjectHeader(clone);
                                       /* copy all data from obj to clone   */
  memcpy((char *)clone, (char *)obj, size);
  ObjectHeader(clone) = newHeader;     /* restore the header information    */
  return clone;
}

RexxArray  *RexxMemory::newObjects(
                size_t         size,
                size_t         count,
                RexxBehaviour *behaviour)
/******************************************************************************/
/* Arguments:  size of individual objects,                                    */
/*             number of objects to get                                       */
/*             behaviour of the new objects.                                  */
/*                                                                            */
/* Function : Return an  Array of count objects of size size, with the given  */
/*             behaviour.  Each objects will be cleared                       */
/*  Returned: Array object                                                    */
/*                                                                            */
/******************************************************************************/
{
  size_t i;
  size_t objSize = roundObjectBoundary(size);
  size_t totalSize;                      /* total size allocated              */
  RexxObject *prototype;                 /* our first prototype object        */

  RexxArray  *arrayOfObjects;
  RexxObject *largeObject;

  /* Get array object to contain all the objects.. */
  arrayOfObjects = (RexxArray *)new_array(count);

  /* Get one LARGE object, that we will parcel up into the smaller */
  /* objects over allocate by the size of one minimum object so we */
  /* can handle any potential overallocations */
  totalSize = objSize * count;         /* first get the total object size   */
  /* We make the decision on which heap this should be allocated */
  /* from based on the size of the object request rather than the */
  /* total size of the aggregate object.  Since our normal usage of */
  /* this is for allocating large collections of small objects, we */
  /* don't want those objects coming from the large block heap, */
  /* even if the aggregate size would suggest this should happen. */
  if (objSize <= LargeBlockThreshold) {
    largeObject = newSpaceNormalSegments.allocateObject(totalSize);
    if (largeObject == OREF_NULL) {
        largeObject = newSpaceNormalSegments.handleAllocationFailure(totalSize);
    }
  }
  /* we keep the big objects in a separate cage */
  else {
    largeObject = newSpaceLargeSegments.allocateObject(totalSize);
    if (largeObject == OREF_NULL) {
        largeObject = newSpaceLargeSegments.handleAllocationFailure(totalSize);
    }
  }

  /* complete the construction of this */
  SetUpNewAllocation(largeObject);

  if (this->saveStack != OREF_NULL) {
                                       /* saveobj doesn't get turned on     */
                                       /*until the system is initialized    */
                                       /*far enough but once its on, push   */
                                       /*this new obj on the save stack to  */
                                       /*keep him from being garbage        */
                                       /*collected, before it can be used   */
                                       /*and safely anchored by caller.     */
      pushSaveStack(largeObject);
  }

  /* Description of defect 318:  IH:

  The problem is caused by the constructor of RexxClause which is calling newObjects.
  NewObjects allocates one large Object. Immediately after this large Object is allocated,
  an array is allocated as well. It then can happen that while allocating the array
  the largeObject shall be marked. This causes a trap when objectVariables is != NULL.

  Solution: Set objectVariables to NULL before allocating the array. In order to make
  OrefOK (called if CHECKOREF is defined) work properly, the largeObject has to be
  set to a valid object state before calling new_array. Therefore the behaviour assignement
  and the virtual functions assignement has to be done in advance. */

  /* get the remainder object size...this is used to manage the */
  /* dangling piece on the end of the allocation. */
  totalSize = ObjectSize(largeObject) - totalSize;

  /* Clear out the entire object... */
  ClearObject(largeObject);

  prototype = largeObject;

  /* IH: Object gets a valid state for the mark and sweep process. */
  /* Otherwise OrefOK (CHECKOREFS) will fail */

  /* set the length of the first object. */
  SetUpNewObject(largeObject, objSize);
  largeObject->behaviour = behaviour;
  setVirtualFunctions(largeObject, behaviour->typenum());

  for (i=1 ;i < count ; i++ ) {
    /* IH: Loop one time less than before because first object is initialized
       outside of the loop. I had to move the following 2 statements
       in front of the object initialization */
    /* add this object to the array of objs */
    arrayOfObjects->put(largeObject, i);
    /* point to the next object space. */
    largeObject = (RexxObject *)((char *)largeObject + objSize);
    /* copy the information from the prototype */
    memcpy(largeObject, prototype, sizeof(RexxInternalObject));
  }
  arrayOfObjects->put(largeObject, i);  /* put the last Object */

  /* adjust the size of the last one to account for any remainder */
  SetObjectSize(largeObject, (totalSize + objSize));

  return arrayOfObjects;               /* Return our array of objects.      */
}


void RexxMemory::reSize(RexxObject *shrinkObj, size_t requestSize)
/******************************************************************************/
/* Function:  The object shrinkObj only needs to be the size of newSize       */
/*             If the left over space is big enough to become a dead object   */
/*             we will shrink the object to the specified size.               */
/*            NOTE: Since memory knows nothing about any objects that are     */
/*             in the extra space, it cannot do anything about them if this   */
/*             is an OldSpace objetc, therefore the caller must have already  */
/*             take care of this.                                             */
/*                                                                            */
/******************************************************************************/
{
  DeadObject *newDeadObj;

  size_t newSize = roundObjectResize(requestSize);

  /* is the rounded size smaller and is remainder at least the size */
  /* of the smallest OBJ MINOBJSIZE */
  if (newSize < requestSize && (ObjectSize(shrinkObj) - newSize) >= MinimumObjectSize) {
      size_t deadObjectSize = ObjectSize(shrinkObj) - newSize;
      /* Yes, then we can shrink the object.  Get starting point of */
      /* the extra, this will be the new Dead obj */
      newDeadObj = new ((void *)((uintptr_t)shrinkObj + newSize)) DeadObject (deadObjectSize);
      /* if an object is larger than 16 MB, the last 8 bits (256) are */
      /* truncated and therefore the object must have a size */
      /* dividable by 256 and the rest must be put to the dead chain. */
      /* If the resulting dead object is smaller than the size we */
      /* gave, then we've got a truncated remainder we need to turn */
      /* into a dead object. */
      deadObjectSize -= ObjectSize(newDeadObj);
      if (deadObjectSize != 0)
      {
          /* create difference object.  Note:  We don't bother */
          /* putting this back on the dead chain.  It will be */
          /* picked up during the next GC cycle. */
          DeadObject *remainderObj = new ((char *)newDeadObj + ObjectSize(newDeadObj)) DeadObject (deadObjectSize);
      }
      /* Adjust size of original object */
      SetObjectSize(shrinkObj, newSize);
  }
}


void RexxMemory::scavengeSegmentSets(
    MemorySegmentSet *requestor,           /* the requesting segment set */
    size_t allocationLength)               /* the size required          */
/******************************************************************************/
/* Function:  Orchestrate sharing of sharing of storage between the segment   */
/* sets in low storage conditions.  We do this only as a last ditch, as we'll */
/* end up fragmenting the large heap with small blocks, messing up the        */
/* benefits of keeping the heaps separate.  We first look a set to donate a   */
/* segment to the requesting set.  If that doesn't work, we'll borrow a block */
/* of storage from the other set so that we can satisfy this request.         */
/******************************************************************************/
{
    MemorySegmentSet *donor;

    /* first determine the donor/requester relationships. */
    if (requestor->is(MemorySegmentSet::SET_NORMAL)) {
        donor = &newSpaceLargeSegments;
    }
    else {
        donor = &newSpaceNormalSegments;
    }

    /* first look for an unused segment.  We might be able to steal */
    /* one and just move it over. */
    MemorySegment *newSegment = donor->donateSegment(allocationLength);
    if (newSegment != NULL) {
        requestor->addSegment(newSegment);
        return;
    }

    /* we can't just move a segment over.  Find the smallest block */
    /* we can find that will satisfy this allocation.  If found, we */
    /* can insert it into the normal deadchains. */
    DeadObject *largeObject = donor->donateObject(allocationLength);
    if (largeObject != NULL) {
        /* we need to insert this into the normal dead chain */
        /* locations. */
        requestor->addDeadObject(largeObject);
    }
}


void RexxMemory::liveStackFull()
/******************************************************************************/
/* Function:  Process a live-stack overflow situation                         */
/******************************************************************************/
{
  RexxStack *newLiveStack;             /* new temporary live stack          */

                                       /* create a temporary stack          */
  newLiveStack = new (this->liveStack->u_size * 2, true) RexxStack (this->liveStack->u_size * 2);
                                       /* copy the live stack entries       */
  newLiveStack->copyEntries(this->liveStack);
                                       /* has this already been expanded?   */
  if (this->liveStack != this->originalLiveStack)
    free((void *)this->liveStack);      /* release the old one               */
  this->liveStack = newLiveStack;      /* and set the new stack             */
}

void RexxMemory::mark(RexxObject *markObject)
/******************************************************************************/
/* Function:  Perform a memory management mark operation                      */
/******************************************************************************/
{
  setUpMemoryMark

  SetObjectLive(markObject);           /* Then Mark this object as live.    */
                                       /* object have any references?       */
                                       /* if there are no references, we don't */
                                       /* need to push this on the stack, but */
                                       /* we might need to push the behavior */
                                       /* on the stack.  Since behaviors are */
                                       /* we can frequently skip that step as well */
  if (!ObjectHasReferences(markObject)) {
      if (ObjectNeedsMarking((RexxObject *)markObject->behaviour)) {
          /* mark the behaviour now to keep us from processing this */
          /* more than once. */
          SetObjectLive((RexxObject *)markObject->behaviour);
                                       /* push him to livestack, to mark    */
                                       /* later.                            */
          pushLiveStack((RexxObject *)markObject->behaviour);
      }
  }
  else {
                                       /* push him to livestack, to mark    */
                                       /* later.                            */
      pushLiveStack(markObject);
  }

  cleanUpMemoryMark
}

RexxObject *RexxMemory::temporaryObject(size_t requestLength)
/******************************************************************************/
/* Function:  Allocate and setup a temporary object obtained via malloc       */
/*            storage.  This is used currently only by the mark routine to    */
/*            expand the size of the live stack during a garbage collection.  */
/******************************************************************************/
{
  RexxObject *newObject;               /* the new object                    */
  size_t allocationLength;             /* length to allocate                */

                                       /* get the rounded size of the object*/
  allocationLength = roundObjectBoundary(requestLength);
                                       /* allocate a new object             */
  newObject = (RexxObject *)malloc(allocationLength);
  if (newObject == OREF_NULL)          /* unable to allocate a new one?     */
                                       /* can't allocate, report resource   */
                                       /* error.                            */
    reportException(Error_System_resources);
                                       /* setup the new object header for   */
                                       /*use                                */
  SetUpNewObject(newObject, allocationLength);
  setObjectVirtualFunctions(newObject);/* give it the default VFT           */
  return newObject;                    /* and return it                     */
}

void RexxMemory::markGeneral(RexxObject **pMarkObject)
/******************************************************************************/
/* Function:  Perform various general marking functions such as image save,   */
/*            image restore, object unflatten, and debug garbage collection   */
/******************************************************************************/
{
  RexxObject *markObject = *pMarkObject;


  if (markObject == OREF_NULL)         /* is this a null reference?         */
    return;                            /* always return immediately         */

  /* If its a restore image mark       */
  if (restoreimage) {
      /* we update the object's location   */
      restoreMark(markObject, pMarkObject);
      return;                            /* finished                          */
  }

  if (objOffset != 0) {                /* restoring an object?              */
      /* we update the object's location   */
      restoreObjectMark(markObject, pMarkObject);
      return;                            /* finished                          */
  }

  if (this->envelope) {                /* processing an envelope?           */
      /* do the unflatten */
      unflattenMark(markObject, pMarkObject);
      return;                          /* and out                           */
  }


  /* The OrphanCheck code is complete mark from OREF_ENV, which checks the           */
  /* validity of each object in the entire object tree.  Any invalid objects will be */
  /* cause the mark to terminate and output information about the invalid object and */
  /* its ancestry.                                                                   */
  if ((this->orphanCheck) && !saveimage) {
      /* do the special validity check mark. */
      orphanCheckMark(markObject, pMarkObject);
      /*  all done here, so return         */
      return;
  }


  if (!saveimage)
  {
      RexxInterpreter::fatalError((stringchar_t *)"Wrong mark routine called");
  }

  /* we're doing the image construction work */
  saveImageMark(markObject, pMarkObject);
}


void RexxMemory::saveImageMark(RexxObject *markObject, RexxObject **pMarkObject)
/******************************************************************************/
/* Function:  Perform marking during a save image operation                   */
/******************************************************************************/
{
    RexxObject *bufferReference;
    size_t size;
    /* Regular GC or Saveimage processing. if a REAL object and not */
    /* already marked and not in Old Space. */
    if (markObject != OREF_NULL && objectIsNotLive(markObject) && !OldSpace(markObject)) {
        /* Then Mark this object as live.    */
        SetObjectLive(markObject);
        /* push him to livestack, so we can mark (at a later time) his */
        /* references. */
        pushLiveStack(markObject);
        /* Get size of this object.          */
        size = ObjectSize(markObject);
        /* add this to our image statistics  */
        logObjectStats(markObject);

        /* address the copy in the image */
        bufferReference = (RexxObject *)(image_buffer + image_offset);
        /* Copy object to image buffer. */
        memcpy(bufferReference, markObject, size);
        /* clear the mark in the copy        */
        ClearObjectMark(bufferReference);
        /* Retrieve the behaviour obj */
        behaviour = bufferReference->behaviour;
        /* Working with a primitive behaviour or a copy? */
        if (behaviour->isNonPrimitiveBehaviour())
            /* tag this as a non-primitive behav */
            SetNonPrimitive(bufferReference);
        else {
            /* clear this out, as this is overloaded with the oldspace */
            /* flag. */
            ClearNonPrimitive(bufferReference);
            /* replace behaviour with typenumber */
            bufferReference->behaviour = behaviour->getSavedPrimitiveBehaviour();
        }

        /* we are saving image at this point, no need to keep track of */
        /* Remembered set so don't use OrefSet here. */
        markObject->behaviour = (RexxBehaviour *)image_offset;
        /* update image_offset to point to where next object is to be */
        /* placed in image */
        image_offset += size;
    }

    /* If its a saveimage, modify object referencing this "moved" obj */
    /* with it location within the image Buffer */
    *pMarkObject = (RexxObject *)markObject->behaviour;
}


void RexxMemory::orphanCheckMark(RexxObject *markObject, RexxObject **pMarkObject)
/******************************************************************************/
/* Function:  Perform orphan check marking on an object                       */
/******************************************************************************/
{
    char *outFileName;
    FILE *outfile;
    bool firstnode;
    RexxString *className;
    stringchar_t *objectClassName;

    /* check for invalid objects         */
    if (!objectReferenceOK(markObject)) {
        /* Get a temporary file name for out */
        outFileName = SysFileSystem::getTempFileName();
        /* Open the temp file for the dump. */
        outfile = fopen(outFileName,"wb");
        logMemoryCheck(outfile, "Found non Object at %p, being marked from %p\n",markObject, pMarkObject);
        /* Let the traversal know we want    */
        /*  more info about first guy...     */
        firstnode = true;
        /* If the object is in object storage*/
        if (inObjectStorage(markObject)) {
            /* DIsplay a few words of the object's storage. */
            logMemoryCheck(outfile, " non-Object dump -->%8.8X   %8.8X   %8.8X   %8.8X \n", *(uint32_t *)markObject,      *((uint32_t *)markObject+1) , *((uint32_t *)markObject+2) , *((uint32_t *)markObject+3));
            logMemoryCheck(outfile, " non-Object dump -->%8.8X   %8.8X   %8.8X   %8.8X \n", *((uint32_t *)markObject+4) , *((uint32_t *)markObject+5) , *((uint32_t *)markObject+6) , *((uint32_t *)markObject+7));
            logMemoryCheck(outfile, " non-Object dump -->%8.8X   %8.8X   %8.8X   %8.8X \n", *((uint32_t *)markObject+8) , *((uint32_t *)markObject+9) , *((uint32_t *)markObject+10), *((uint32_t *)markObject+11));
            logMemoryCheck(outfile, " non-Object dump -->%8.8X   %8.8X   %8.8X   %8.8X \n", *((uint32_t *)markObject+12), *((uint32_t *)markObject+13), *((uint32_t *)markObject+14), *((uint32_t *)markObject+15));

        }
        /* Time to traverse the livestack and get this guy's ancestry--very useful    */
        /* information for debugging this type of problem.                            */
        for (markObject = popLiveStack();
            markObject != OREF_NULL;     /* test for unique terminator        */
            markObject = popLiveStack()) {

            /* OREF_NIL marks an ancestor        */
            if (markObject == TheNilObject) {
                /* pop the ancestor                  */
                markObject  = popLiveStack();
                className = markObject->id();
                if (className == OREF_NULL)
                    objectClassName = (stringchar_t *)"";
                else
                    objectClassName = className->getStringData();
                if (firstnode) {             /* is this the first node??          */
                    printf("-->Parent node was marking offset '%p'x \n",(uintptr_t)pMarkObject - (uintptr_t)markObject);
                    dumpObject(markObject, outfile);
                    firstnode = false;
                    logMemoryCheck(outfile, "Parent node is at %p, of type %s(%d) \n",
                        markObject, objectClassName, markObject->behaviour->typenum());
                }
                else {
                    logMemoryCheck(outfile, "Next node is at %p, of type %s(%d) \n",
                        markObject, objectClassName, markObject->behaviour->typenum());
                }
            }
        }
        /* reached the OREF_NULL sentinel    */
        logMemoryCheck(outfile, "Finished popping stack !!\n");
        printf("All data has been captured in file %s \n", outFileName);
        fclose(outfile);                /* Close the file, all with it.      */
        /* we would have crashed soon anyway!*/
        RexxInterpreter::fatalError((stringchar_t *)"Bad Object found during GC !\n");
    }

    if (objectIsNotLive(markObject) && !OldSpace(markObject)) {
        SetObjectLive(markObject);       /* Mark this object as live          */
                                         /* push him to livestack, so we can  */
                                         /*mark (at a later time) his         */
                                         /*references.                        */
        pushLiveStack(markObject);
    }
}


void RexxMemory::discardHoldObject(RexxInternalObject *obj)
{
   /* Remove object form savetable */
   saveTable->remove((RexxObject *)obj);
   /* no memory section needed */
   saveStack->push((RexxObject *)obj);
}


RexxObject *RexxMemory::holdObject(RexxInternalObject *obj)
/******************************************************************************/
/* Function:  Place an object on the hold stack                               */
/******************************************************************************/
{
   /* Push object onto saveStack */
   saveStack->push((RexxObject *)obj);
   /* return held  object               */
   return (RexxObject *)obj;
}


RexxObject *RexxMemory::dump(void)
/******************************************************************************/
/* Function:  Dump the memory tables                                          */
/******************************************************************************/
{
#ifdef _DEBUG
  FILE *dumpfile;
  FILE *keyfile;
  MemorySegmentPool *currentPool = this->firstPool;
  int   i;

  if (dumpEnable) {
                                       /* don't allow another dump unless   */
                                       /*reset by user                      */
    RexxMemory::memoryObject.dumpEnable = false;
                                       /* first generate the dump by writing*/
                                       /*  each segment to the dumpfile     */
    printf("Dumping object memory to orexdump.dmp\n");
                                       /* open for binary write             */
    dumpfile = fopen("orexdump.dmp","wb");
                                       /* now generate the key file which   */
                                       /*is a series of REXX statements     */
    printf("Creating dump key file in orexdump.key\n");
                                       /* open for text write               */
    keyfile = fopen("orexdump.key","w");
    fprintf(keyfile, "/* Object REXX dump key file */\n");
    fprintf(keyfile, "memoryaddr = %p\n", this);
    fprintf(keyfile, "marker = %d\n", markWord);
    /* dump the oldspace segments */
    oldSpaceSegments.dumpSegments(keyfile, dumpfile);
    /* followed by the newer allocations */
    newSpaceNormalSegments.dumpSegments(keyfile, dumpfile);
    newSpaceLargeSegments.dumpSegments(keyfile, dumpfile);

    i = 0;
    while (currentPool) {
      i++;
      fprintf(keyfile, "Pool addr.%d = %p\n", i, currentPool);
      fprintf(keyfile, "Pool size.%d = %d\n", i, currentPool->reserved);
      currentPool = currentPool->nextPool();
    }

    for (i = 0; i <= T_Last_Primitive_Class; i++) {
      fprintf(keyfile, "Behaviour type %d = %p\n", i, &RexxBehaviour::primitiveBehaviours[i]);
    }

                                       /* now close actual dump and key file*/
    fclose(dumpfile);
    fclose(keyfile);
  }
#endif
  return OREF_NULL;
}


RexxObject *RexxMemory::gutCheck(void)
/******************************************************************************/
/* Arguments:  None                                                           */
/*                                                                            */
/*  Returned:  Nothing                                                        */
/******************************************************************************/
{
  size_t j, count, testcount;
  bool restoreimagesave;
  RexxInteger *value;
  RexxInteger *testValue;
  RexxObject *index;
                                       /* temp OREF used to hold original   */
                                       /*and test versions                  */
  RexxObjectTable *tempold2new;

  printf("Comparing old2new with the current system.\n");

                                       /* build a test remembered set       */
  tempold2new = new_object_table();
  restoreimagesave = restoreimage;
  restoreimage = true;                 /* setting both of these to true will*/
                                       /* traverse System OldSpace and live */
                                       /*mark all objects                   */
  oldSpaceSegments.markOldSpaceObjects();

  restoreimage = restoreimagesave;     /* all done building remembered set  */
                                       /* For all object in old2new table   */
  for (j = old2new->first();
       (index = (RexxObject *)old2new->index(j)) != OREF_NULL;
       j = old2new->next(j)) {
                                       /* Get refCount for this obj old2new */
    value = (RexxInteger *)this->old2new->get(index);
                                       /* new refcount for obj in newly     */
                                       /*build old2new table                */
    testValue = (RexxInteger *)tempold2new->get(index);
                                       /* Was object found in new table?    */
    if (testValue == OREF_NULL) {
                                       /* nope, extra stuff in orig.        */
       printf("object:  %p,  type:  %d, is extra in old2new.\n\n",
               index, index->behaviour->typenum());
    } else {
                                       /* now make sure both refCounts are  */
                                       /* the same.                         */
       count = value->stringSize(); ;
       testcount = testValue->stringSize();
       if (count != testcount) {
         printf("object:  %p,  type:  %d, has an incorrect refcount.\n",
                 index, index->behaviour->typenum());
         printf("Refcount for object is %d, should be %d.\n\n", count, testcount);
       }
                                       /* now remove object from new table  */
       tempold2new->remove(index);
    }
  }
                                       /* Anything left in new table was    */
                                       /* missing from original old2new     */
                                       /* so iterate through it and report  */
                                       /* the missing objects               */
  for (j = tempold2new->first();
       (index = (RexxObject *)tempold2new->index(j)) != OREF_NULL;
       j = tempold2new->next(j)) {
    printf("object:  %p,  type:  %d, is missing from old2new.\n\n",
            index,index->behaviour->typenum());
  }

                                       /* now take a dump so that we can    */
                                       /*  diagnose any problems we just    */
                                       /*uncovered                          */
  printf("Dumping object memory.\n");  /* tell the user what's happening    */
  this->dumpEnable = true;             /* turn dumps on                     */
  this->dump();                        /* take the dump                     */

  return OREF_NULL;
}


void RexxMemory::setObjectOffset(size_t offset)
/******************************************************************************/
/* Arguments:  offset, the length to add to references within arriving objects*/
/*     since only one unflattem can happen at a time, we need to ensure       */
/*     serialization, so if already have an objOffset, wait unto done.        */
/*                                                                            */
/*  Returned:  Nothing                                                        */
/******************************************************************************/
{
  /* Starting or ending? */
  if (offset != 0) {

   /* have a value, starting unflatten.  See if we can get MUTEX */
   /* immed */
   if (this->unflattenMutex.requestImmediate())
   {
       // No kernel access in this block
       UnsafeBlock ublock;
       /* wait for current unflatten to end */
       this->unflattenMutex.request();
   }
  }
  else {
     /* no value, ending an unflatten */
      /* Release the MUTEX.                */
     this->unflattenMutex.release();
  }

  /* setup offSet value.               */
  this->objOffset = offset;
}

void      RexxMemory::setEnvelope(RexxEnvelope *envelope)
/******************************************************************************/
/* Arguments:  envelope object,                                               */
/*     since only one unflattem can happen at a time, we need to ensure       */
/*     serialization, so if already have an envelope,  wait unto done.        */
/*                                                                            */
/*  Returned:  Nothing                                                        */
/******************************************************************************/
{
                                       /* Starting or ending?               */
  if (envelope != OREF_NULL) {
                                       /* have a value, starting unflatt    */
                                       /* See if we can get MUTEX immed     */
   if (this->envelopeMutex.requestImmediate()) {
       // No kernel access in this block
       UnsafeBlock ublock;
       this->envelopeMutex.request();     /* wait for current unflat to end    */
                                          /* get kernel access back.           */
   }
  }
  else {
                                       /* no value, ending an unflatten     */
     this->envelopeMutex.release();    /* Release the MUTEX.                */
  }

  this->envelope = envelope;           /* set the envelope object           */
}


RexxObject *RexxMemory::setOref(RexxObject **index, RexxObject *value)
/******************************************************************************/
/* Arguments:  index-- OREF to set;  value--OREF to which objr is set         */
/*                                                                            */
/*  Returned:  nothing                                                        */
/*                                                                            */
/******************************************************************************/
{
  RexxInteger *refcount;
  if (old2new != OREF_NULL) {
    if (!NullOrOld(*index)) {
                                       /* decrement reference count for     */
                                       /**index                             */
      refcount = (RexxInteger *)this->old2new->get(*index);
      if (refcount != OREF_NULL) {
                                       /* found a value for refcount, now   */
                                       /*decrement the count                */
        refcount->decrementValue();
                                       /* if the new value is 0, *index is  */
                                       /*no longer ref'ed from oldspace     */
        if (refcount->stringSize() == 0) {
                                       /* delete the entry for *index       */
          this->old2new->remove(*index);
        }
      } else {
        /* naughty, naughty, someone didn't use SetOref */
        printf("******** error in memory_setoref, unable to decrement refcount\n");
        printf("Naughty object reference is from:  %p\n", index);
        printf("Naughty object reference is at:  %p\n", *index);
        printf("Naughty object reference type is:  %d\n", (*index)->behaviour->typenum());
      }
    }
    if (!NullOrOld(value)) {
                                       /* increment reference count for     */
                                       /*value                              */
      refcount = (RexxInteger *)this->old2new->get(value);
                                       /* was there a reference here        */
                                       /*already?                           */
      if (refcount) {
                                       /* yep, found one, so just increment */
                                       /*it                                 */
        refcount->incrementValue();
      } else {
                                       /* nope, this is the first, so set   */
                                       /*the refcount to 1                  */
        this->old2new->put(new_counter(1), value);
      }
    }
  }
                                       /* now make the assignment, just     */
                                       /*like all this stuff never happened!*/
  return *index = value;
}


RexxObject *RexxMemory::checkSetOref(
                RexxObject  *setter,
                RexxObject **index,
                RexxObject  *value,
                char        *fileName,
                size_t       lineNumber)
/******************************************************************************/
/* Arguments:  index-- OREF to set;  value--OREF to which objr is set         */
/*                                                                            */
/*  Returned:  nothing                                                        */
/*                                                                            */
/******************************************************************************/
{
  bool allOK = true;
  char *outFileName;
  FILE *outfile;
                                       /* Skip all checks during saveimage  */
 if (checkSetOK) {                     /* and initial part of restore Image */
  if (!inObjectStorage(setter)) {      /* Is the setter object invalid      */
    allOK = false;                     /* No, just put out setters addr.    */
    outFileName = SysFileSystem::getTempFileName();/* Get a temporary file name for out */
    outfile = fopen(outFileName,"wb");
    logMemoryCheck(outfile, "The Setter object at %p is invalid...\n");

                                       /* Is the new value a real object?   */
  }
  else if (value && value != (RexxObject *)TheBehaviourBehaviour && value != (RexxObject *)&RexxBehaviour::primitiveBehaviours[T_Behaviour] && !objectReferenceOK(value)) {
    allOK = false;                     /* No, put out the info              */
    outFileName = SysFileSystem::getTempFileName();/* Get a temporary file name for out */
    outfile = fopen(outFileName,"wb");
    logMemoryCheck(outfile, "The Setter object at %p attempted to put a non object %p, at offset %p\n",setter, value, (uintptr_t)index - (uintptr_t)setter);
    logMemoryCheck(outfile, " A dump of the Setting object follows: \n");
    dumpObject(setter, outfile);

  }
  else if (index >= (RexxObject **)((uintptr_t)setter + ObjectSize(setter))) {
    allOK = false;                     /* Yes, let them know                */
    outFileName = SysFileSystem::getTempFileName();/* Get a temporary file name for out */
    outfile = fopen(outFileName,"wb");
    logMemoryCheck(outfile, "The Setter object at %p has tried to store at offset, which is  outside his object range\n",setter, (uintptr_t)index - (uintptr_t)setter);
    logMemoryCheck(outfile, " A dump of the Setting object follows: \n");
    dumpObject(setter, outfile);
  }


 }

  if (!allOK) {
    logMemoryCheck(outfile, " The error occurred in line %u of file %s\n", lineNumber, fileName);
    printf("The dump data has been written to file %s \n",outFileName);
    fclose(outfile);
    RexxInterpreter::fatalError((stringchar_t *)"Something went really wrong in SetOref ...\n");
  }
                                       /* now do the normal SetOref() stuff */
  return (OldSpace(setter) ? (this->setOref(index, value)) : (*index = value));

}

RexxStack *RexxMemory::getFlattenStack(void)
/******************************************************************************/
/* Function:  Allocate and lock the flatten stack capability.                 */
/******************************************************************************/
{
                                       /* See if we can get MUTEX immed     */
    if (this->flattenMutex.requestImmediate())
    {
       // No kernel access in this block
        UnsafeBlock ublock;
        this->flattenMutex.release();   /* wait for current flattento end    */
    }
                                       /* create a temporary stack          */
    rootImage->flattenStack = flattenStack = new (LiveStackSize, true) RexxStack (LiveStackSize);
    return this->flattenStack;           /* return flatten Stack              */
}

void RexxMemory::returnFlattenStack(void)
/******************************************************************************/
/* Function:  Release the flatten stack                                       */
/******************************************************************************/
{
   free((void *)this->flattenStack);   /* release the flatten stack         */
   this->flattenMutex.release();       /* and release the semaphore         */
}

RexxObject *RexxMemory::dumpImageStats(void)
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
{
  MemoryStats imageStats;

  /* clear all of the statistics */
  imageStats.clear();
  /* gather a fresh set of stats for all of the segments */
  newSpaceNormalSegments.gatherStats(&imageStats, &imageStats.normalStats);
  newSpaceLargeSegments.gatherStats(&imageStats, &imageStats.largeStats);

  /* print out the memory statistics */
  imageStats.printMemoryStats();
  return TheNilObject;
}

void RexxMemory::accessPools()
/******************************************************************************/
/* Function:  Gain access to all Pools                                        */
/******************************************************************************/
{
                                       /* access 1st pool directly. SysCall */
                                       /* Did the pool exist?               */

  if (SysInterpreter::accessMemoryPool(this->firstPool))
  {
                                       /* Yes, then see if others to access */
    this->accessPools(this->firstPool);/* Now gain access to all the rest   */
                                       /* now have access to all pools on   */
                                       /* this process.                     */
    localMemoryPool = globalMemoryPool;
  }
}

/**
 * Update the memory pool for a new segment pool added to the set.
 *
 * @param newPool The new head pool for the list.
 */
void RexxMemory::updateMemoryPool(MemorySegmentPool *newPool)
{
    localMemoryPool = newPool;
    globalMemoryPool = newPool;
}

void RexxMemory::accessPools(MemorySegmentPool *pool)
/******************************************************************************/
/* Function:  Gain access to all Pools after pool                             */
/******************************************************************************/
{
  MemorySegmentPool  *lastPool;        /* CHM - defect 29 */

  while (pool) {                       /* for all pools.                    */
   lastPool = pool;                    /* CHM def29: save last valid pool   */
   pool->accessNextPool();             /* access next pool.                 */
   pool = pool->nextPool();            /* get next Pool                     */
  }

  /* CHM - defect 29: if GlobalCurrentPool is not set, then set it to the   */
  /* last valid pool                                                        */
  if (globalMemoryPool == NULL)
  {
     globalMemoryPool = lastPool;
  } /* endif */
}


void RexxMemory::freePools()
/******************************************************************************/
/* Function:  Free all the memory pools currently accessed by this process    */
/*    If not already released.  Then set process Pool to NULL, indicate       */
/*    pools have been released.                                               */
/******************************************************************************/
{
                                       /* Released pool yet?                */
   if (localMemoryPool != NULL) {
                                       /* nope release them now.            */
     this->freePools(this->firstPool);

   }
   localMemoryPool = NULL;          /* no pools available to this process*/
}

MemorySegmentPool *RexxMemory::freePools(MemorySegmentPool *pool)
/******************************************************************************/
/* Function:  Free all the memory pools currently accessed by this process    */
/*    This is called recursivly, until we reach the end of the list           */
/******************************************************************************/
{
  if (pool != localMemoryPool) {    /* At the end of the list?           */
                                       /* nope, free next one first.        */
    pool->setNext(this->freePools(pool->nextPool()));    /* CHM - defect 96 */

    if ( !pool->nextPool() )           /* CHM - def96: if no next pool then */
    {                                  /* move GlobalCurrentPool one pool   */
       globalMemoryPool = pool;       /* backwards                         */
    } /* endif */
  }
  return pool->freePool();             /* CHM - def.96: now free this pool. */
}


void RexxMemory::setUpMemoryTables(bool restoringImage)
/******************************************************************************/
/* Function:  Set up the initial memory table.                                */
/******************************************************************************/
{
  /* fix up the previously allocated live stack to have the correct */
  /* characteristics...we're almost ready to go on the air. */
  BehaviourSet(liveStack, TheStackBehaviour);
  liveStack->init(LiveStackSize);

  // we publish these in the root object and also keep a local copy
  saveStack = rootImage->saveStack = new (SaveStackAllocSize) RexxSaveStack(SaveStackSize, SaveStackAllocSize);

  // from this point on, we push things on to the save stack
  saveTable = rootImage->saveTable = new_object_table();
  // create the rest of the process specific information
  rootImage->createProcessTables(restoringImage);

  // make sure we get a copy of the mapping table
  old2new = rootImage->oldToNewTable;
}

void RexxMemory::createLocks()
/******************************************************************************/
/* Function:  Do the initial lock creation for memory setup.                  */
/******************************************************************************/
{
                                       /* Create/Open Shared MUTEX      */
                                       /* Semophores used to serialize  */
                                       /* the flatten/unflatten process */
  flattenMutex.create();
  unflattenMutex.create();
  envelopeMutex.create();
}


RootImage *RexxMemory::restoreImage()
{
  /* Make sure memory is cleared! */
  memoryObject.init();
  // load the memory image, and return the loaded root image
  return memoryObject.loadImage();
}

void RexxMemory::addWeakReference(WeakReference *ref)
/******************************************************************************/
/* Function:  Add a new weak reference to the tracking table                  */
/******************************************************************************/
{
    // just add this to the front of the list
    ref->nextReferenceList = rootImage->weakReferenceList;
    rootImage->weakReferenceList = ref;
}

void RexxMemory::addProxiedObject(RexxObject *object, RexxString *name)
/******************************************************************************/
/* Function:  Add a new proxied object to the tracking table                  */
/******************************************************************************/
{
    rootImage->addProxiedObject(object, name);
}

RexxString *RexxMemory::getObjectProxy(RexxObject *object)
/******************************************************************************/
/* Function:  Map an object to its proxy.                                     */
/******************************************************************************/
{
    return rootImage->getObjectProxy(object);
}

RexxObject *RexxMemory::getProxiedObject(RexxString *proxy)
/******************************************************************************/
/* Function:  Map a proxy back to its object                                  */
/******************************************************************************/
{
    return rootImage->getProxiedObject(proxy);
}
