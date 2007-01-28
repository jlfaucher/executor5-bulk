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
/*****************************************************************************/
/* REXX Interpreter                                                          */
/*                                                                           */
/*****************************************************************************/

#include "RexxCore.h"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"
#include "InterpreterInstance.hpp"
#include "SystemException.hpp"
#include "ExitHandler.hpp"
#include "RexxNativeCode.hpp"
#include "RootImage.hpp"
#include "Package.hpp"
#include "DirectoryClass.hpp"
#include "FunctionActivator.hpp"
#include <time.h>

                                       /* generic default settings structure*/
ACTIVATION_SETTINGS RexxInterpreter::defaultSettings;
                                       /* default active settings           */
ACTIVATION_SETTINGS *RexxInterpreter::currentSettings = &defaultSettings;
bool RexxInterpreter::initialized = false;             // the interpreter is initialized
RexxDirectory *RexxInterpreter::theEnvironment = OREF_NULL;        // the global environment object
RexxDirectory *RexxInterpreter::theKernel = OREF_NULL;             // the global kernel environment

RexxDirectory *RexxInterpreter::publicRoutines = OREF_NULL;        // the public routines directory
RexxDirectory *RexxInterpreter::staticRequires = OREF_NULL;        // the statically loaded requires directory
RexxDirectory *RexxInterpreter::commonRetrievers = OREF_NULL;      // the predefined rexx variable objects

RexxActivity *RexxInterpreter::currentActivity = OREF_NULL;        // expose current activity object

SysMutex RexxInterpreter::executionLock;               // the exclusive execution lock
SysMutex RexxInterpreter::resourceLock;                // global control block mutex
SysMutex RexxInterpreter::initializationLock;          // semaphore to protect startup/shutdown phases
SysSemaphore RexxInterpreter::terminationSem;          // shutdown completion semaphore
SysCriticalSection RexxInterpreter::criticalSection;   // critical section for thread sync
bool RexxInterpreter::DBCS_codepage = false;           // DBCS code page possible
uint32_t RexxInterpreter::codepage = 0;                // current activity codepage
stringchar_t RexxInterpreter::DBCS_table[256];         // current DBCS vector table


RexxInterpreter *RexxInterpreter::singleInstance = OREF_NULL; // the single instance for this process

RexxObject *RexxInterpreter::processID = OREF_NULL;    // the process identifier...also the process anchor point
RootImage  *RexxInterpreter::rootImage = OREF_NULL;    // single cross process anchor point
int32_t     RexxInterpreter::randomSeed = 0;           // the random number randomizer

/**
 * Create a new RexxInterpreter object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *RexxInterpreter::operator new(size_t size)
{
    return new_object(size, TheInterpreterBehaviour);
}

/**
 * Initialize an interpeter process instance.
 */
RexxInterpreter::RexxInterpreter()
{
    versionID = OREF_NULL;
    globalReferences = new_object_table();
    instanceObjects = new_object_table();
    activityPool = new_object_table();
    activityPoolSize = 0;
    uninitTable = new_object_table();
    pendingUninits = 0;
    processingUninits = false;
}

/**
 * Normal live marking.
 */
void RexxInterpreter::live()
{
  setUpMemoryMark
      memory_mark(versionID);
      memory_mark(packageManager);
      memory_mark(cachedFunctions);
      memory_mark(globalReferences);
      memory_mark(instanceObjects);
      memory_mark(activityPool);
      //NB:  The uninit table is not marked...that is handled as a separate
      // step during GC processing.
  cleanUpMemoryMark
}

/**
 * Generalized live marking.
 */
void RexxInterpreter::liveGeneral()
{
  setUpMemoryMarkGeneral
      memory_mark_general(versionID);
      memory_mark_general(packageManager);
      memory_mark_general(cachedFunctions);
      memory_mark_general(globalReferences);
      memory_mark_general(instanceObjects);
      memory_mark_general(activityPool);
      //NB:  The uninit table is not marked...that is handled as a separate
      // step during GC processing.
  cleanUpMemoryMarkGeneral
}

/**
 * Do one-time process specific intialization of the environment.
 *
 */
void RexxInterpreter::initializeProcessEnvironment()
{
    executionLock.create();
    resourceLock.create();
    initializationLock.create();
    terminationSem.create();
}


/**
 * Do process-termination processing to make sure process-specific
 * resources are cleaned up properly.
 */
void RexxInterpreter::terminateProcessEnvironment()
{
    executionLock.close();
    resourceLock.close();
    initializationLock.close();
    terminationSem.close();
}


/******************************************************************************/
/* Process a call to the RexxTerminate() API                                  */
/******************************************************************************/
void RexxInterpreter::terminateInstance(InterpreterInstance *instance)
{
    InitializationSection lock;            // we lock on initializations while doing this

    instance->terminate();                 // have the instance shut everything down
    // remove the instance and see if this was the last instance
    bool lastInstance = singleInstance->removeInstance(instance);
    delete instance;                       // delete the instance block
    // was this the last instance?
    if (lastInstance)
    {
        // clear the free activity pool
        singleInstance->terminateActivityPool();
        // remove this from the process table.
        singleInstance->rootImage->removeProcess(processID);
        // remove the instance....we'll reinitialize next time through.
        singleInstance = OREF_NULL;
    }
}

bool RexxInterpreter::removeInstance(InterpreterInstance *instance)
{
    // remove the anchor object
    InterpreterInstanceObject *anchor = instance->instanceObject;
    singleInstance->instanceObjects->remove((RexxObject *)anchor);
    return singleInstance->instanceObjects->items() == 0;
}


/******************************************************************************/
/* Perform process-global initialization of the interpreter environment       */
/******************************************************************************/
void RexxInterpreter::initializeInterpreter(InterpreterInstance *instance, InitializationReason reason)
{
    // if the global environment is already initialized, this is easy!
    if (singleInstance != OREF_NULL)
    {
        return;
    }
    //TODO:  recheck startup process in original to make sure I haven't forgotten an
    // important step

    SysInterpreter::initializeInterpreter(); // perform other system init work

    if (reason == CREATE_IMAGE)              // need to create the image during the build process?
    {
        createImage(instance);               // go create the image
    }
    else
    {
        restoreImage();                      // make sure we have a restored image
    }
                                             // get the DBCS information
    DBCS_codepage = SysInterpreter::getDBCSInformation(&codepage, DBCS_table);
    initialized = true;                     // we're now initialized
}

/**
 * Save the initial interpreter image.
 *
 * @return A succsess/failure return code.
 */
RexxReturnCode RexxInterpreter::saveImage()
{
    RexxInstance *instance;
    RexxThreadContext *context;

    // NB:  createInstance() with CREATE_IMAGE specified terminates the process directly
    // when the image save is complete, so control generally doesn't return to here.
    return RexxInterpreter::createInstance(&instance, &context, NULL, NULL, CREATE_IMAGE) ? RXAPI_OK : RXAPI_MEMFAIL;
}


/******************************************************************************/
/* Create a runnable instance of the interpreter.                             */
/******************************************************************************/
bool RexxInterpreter::createInstance(RexxInstance **instance, RexxThreadContext **context, ExitHandler *handlers, RexxOption *options, InitializationReason reason)
{
    // grab the init/term semaphore so we don't trip over each over.
    // note that if we're running in shared mode, this will be a cross-process
    // lock.
    InitializationSection init;
    try
    {
        // create an interpreter instance.  NOTE:  this is allocated from
        // C++ object storage, not from Object Rexx storage.  The Rexx heap is not
        // necessarily initialized yet.
        // TODO:  add a system specific initialization step to the constructor
        InterpreterInstance *newInstance = new InterpreterInstance(handlers);

        // now that we have an instance to run under, see if we need to
        // initialize the interpreter environment

        // make sure all global process initialization is completed.
        initializeInterpreter(newInstance, reason);

        // now attach this to the process and get the Rexx objects backing the instance
        // created.  We need to make sure the interpreter instance is created
        // before we do this, to ensure we can create Rexx objects ok.
        newInstance->attachToProcess();
        // now that we have a running interpreter environment, process the specified
        // options...if this fails now, we return a failure value.
        if (!newInstance->processOptions(options))
        {
            return false;
        }
        *instance = (RexxInstance *)newInstance;

        return true;
    }
    catch (SystemException *)
    {
        return false;
    }
}


void RexxInterpreter::shutdownEnvironment()
/******************************************************************************/
/* Shutdown the entire Rexx environment                                       */
/******************************************************************************/
{
    // do any system-specific shutdowns
    SysInterpreter::shutdownEnvironment();
    // make sure we unload any libraries we've loaded.
    singleInstance->packageManager->unload();
    // we're in serious shutdown mode now...destroy the memory pool...which
    // will invalidate all memory objects
    RexxMemory::memoryObject.destroyImage();
    initialized = false;
}


void RexxInterpreter::restoreImage()
/******************************************************************************/
/* Main startup routine for OREXX                                             */
/******************************************************************************/
{
    // lock the kernel...it wouldn't be good to do this on multiple threads
    lockInterpreter();
                                     /* go build the VFT Array            */

    // build the virtual function table before restoring the image.  This is
    // needed to restore the VFT pointer in the unflattened image objects.
    RexxBehaviour::buildVirtualFunctionTable();
    // do the base reinitialization of the memory image
    RootImage *rootImage = RexxMemory::restoreImage();

    // have the image class restore its global objects
    RexxIntegerClass::restoreInstance(rootImage);
                                       // restore the global string table
    GlobalNames::restoreNames(rootImage->globalNames);

    PackageManager::restoreInstance(rootImage); /* fix up native methods             */
    RexxActivityManager::restoreInstance(rootImage);        /* do activity restores              */
    // initialize the process specific tables
    initializeProcess(rootImage);
    RexxMemory::memoryObject.enableOrefChecks(); /* enable setCheckOrefs...           */
    // initialied the external interface vectors.
    RexxActivity::initializeInterfaces();
    unlockInterpreter();        // done with exclusive access
}


void RexxInterpreter::initializeProcess(RootImage *rootImage)
/******************************************************************************/
/* Function:  Do process-specific interpreter initialization                  */
/******************************************************************************/
{
    // create a new object table for tracking the active processes
    rootImage->activeProcesses = new_object_table();
    // get a process identifier
    processID = SysInterpreter::getProcessName();
    // create a process-specific anchor object we can add to the global process table
    singleInstance = new RexxInterpreter();
    // now add a process instance and add it to the global table
    rootImage->addProcess(processID, singleInstance);
}


void RexxInterpreter::createImage(InterpreterInstance *instance)
/******************************************************************************/
/* Function:  Build and save the REXX image                                   */
/******************************************************************************/
{
    // build the virtual function table array */
    RexxBehaviour::buildVirtualFunctionTable();
    // go create the base memory image
    RootImage *rootImage = RexxMemory::createImage();

    bootstrapClasses(rootImage);         /* define the initial class set      */
    // initialized the process specific tables
    initializeProcess(rootImage);

    // initialied the external interface vectors.
    RexxActivity::initializeInterfaces();

    // create an activity associated with this thread.  Since this will be the
    // first activity associated with the instance, this will create the associated
    // object instances that are anchored in the process instance.
    instance->enterOnCurrentThread();
    // now that we have an activity, we can finish setup, which requires the
    // ability to run Rexx code.
    if (exportBaseClasses())
    {
        RexxMemory::memoryObject.saveImage();   // ok, tell the memory object to save what we have in-memory
        // we call exit directly...we don't allow anything else to run after an image
        // creation operation.
        exit(RC_OK);
    }
    else
    {
        fatalError((stringchar_t *)"Error building kernel image.  Image not saved.");
    }
    // and go shutdown the instance.
    instance->exitCurrentThread();
}

RexxString *RexxInterpreter::addGlobalName(
    stringchar_t* value)               /* ASCII-Z string value              */
/******************************************************************************/
/* Function:  Create a common string value during image build and save        */
/******************************************************************************/
{
  RexxString * stringValue;            /* string value                      */
  RexxString * result;                 /* result value                      */

  RexxDirectory *stringTable = rootImage->globalStringTable;

  stringValue = new_string((char *)value);  /* get a string object               */
  if (stringTable == OREF_NULL)        /* no longer collecting strings?     */
    return stringValue;                /* just return the string            */
                                       /* check the global table first      */
  result = (RexxString *)stringTable->at(stringValue);
  if (result == OREF_NULL) {           /* not in the table                  */
                                       /* add this to the table             */
    stringTable->put((RexxObject *)stringValue, stringValue);
    result = stringValue;              /* also the final value              */
  }
  return result;                       /* return the string                 */
}

RexxDirectory *RexxInterpreter::getSourceStringTable()
/******************************************************************************/
/* Function:  Get a table for collection strings.  If this is an image build  */
/* we return the common table.  All other cases get a unique directory        */
/******************************************************************************/
{
    RexxDirectory *stringTable = rootImage->globalStringTable;
    if (stringTable == OREF_NULL)        /* no longer collecting strings?     */
    {
        stringTable = new_directory();   // just return a new directory
    }
    return stringTable;
}

void RexxInterpreter::createInterpreterTables()
/******************************************************************************/
/* Function:  Create interpreter state data during startup                    */
/******************************************************************************/
{
}

void RexxInterpreter::addPublicObject(
    stringchar_t* name,                // ASCII-Z string name
    RexxObject  * object,              // the object we're exposing
    RexxDirectory *dir)                // the published directory
/******************************************************************************/
/* Function:  Create a common string value during image build and save        */
/******************************************************************************/
{

    dir->setEntry(addGlobalName(name), object);
}

/**
 * Run a Rexx program as a result of an API call.
 *
 * @param activity  The activity we're running on.
 * @param filename  The name of the source file.
 * @param argCount  The count of arguments.
 * @param arguments The string arguments.
 * @param callType  The type of call used with this program.
 * @param instoreSource
 *                  Potential instorage source.
 * @param instoreImage
 *                  Potential instorage image.
 * @param result    Pointer to a location for a returned result object.
 * @param retCode   A pointer for a returned numeric return code, if convertable.
 */
void RexxInterpreter::runRexxProgram(RexxActivity *activity, stringchar_t *filename, size_t argCount, RxString *arguments,
    wholenumber_t callType, stringchar_t *initialAddress, RxString *instoreSource, RxString *instoreImage, RxString *resultPtr, RexxNumber *retCode)
{
    ProtectedObject name;
    if (filename != NULL)              /* have an actual name?              */
    {
        // create a string version of the name and protect it
        name = new_string(filename);
    }
    else
    {
        name = OREF_NULLSTRING;            /* use an "unlocatable" name         */
    }

    // now an argument array, also protected
    RexxArray *new_arglist = new_array(argCount);
    ProtectedObject args(new_arglist);

    // now convert all of the arguments
    for (size_t i = 0; i < argCount; i++)
    {
        // convert to string form, and add to the arg array if real
        RexxString *arg = new_string(arguments[i]);
        if (arg != OREF_NULL)
        {
            new_arglist->put(arg, i + 1);

        }
    }

    //TODO:  Make sure command line stuff removes leading blank on arg string
    // turn the calltype into a string value
    RexxString *source_calltype;
    switch (callType)
    {
        default:
        case RXCOMMAND:
            source_calltype = OREF_COMMAND;  // this is the 'COMMAND' string
            break;

        case RXFUNCTION:
            // 'FUNCTION' string
            source_calltype = OREF_FUNCTIONNAME;
            break;

        case RXSUBROUTINE:
            source_calltype = OREF_SUBROUTINE;
            break;

        case RXMETHOD:
            source_calltype = OREF_METHOD;
            break;

        case RXSCRIPT:
            source_calltype = OREF_SCRIPT;
            break;
    }

    RexxMethod *method;
    ProtectedObject fullname;
    ProtectedObject pMethod;
    // if no instore image was specified, we're working from a file source...load and
    // turn into a method.
    if (instoreSource == NULL && instoreImage == NULL)
    {
        /* go resolve the name               */
        fullname = SysInterpreter::resolveProgramName(name, OREF_NULL);
        if ((RexxObject *)fullname == OREF_NULL)         /* not found?                        */
        {
            /* got an error here                 */
            reportException(Error_Program_unreadable_notfound, (RexxString *)name);
        }
        /* try to restore saved image        */
        RexxMethod::restoreProgram((RexxString *)fullname, pMethod);
        method = (RexxMethod *)pMethod;
        if (method == OREF_NULL)
        {         /* unable to restore?                */
                  /* go translate the image            */
            pMethod = RexxMethod::newFile(fullname);
            method = (RexxMethod *)pMethod;
            method->saveProgram((RexxString *)fullname);   /* go save this method               */
        }
    }
    else
    {                               /* have an instore program           */
        RexxUnsignedNumber temp;

        // all nulls here means try to execute this from the macro space, so
        // just try to retrieve the code from there.
        if (instoreSource->strptr == NULL && instoreImage->strptr == NULL)
        {
            /* see if this exists                */
            if (!ooRexxQueryMacro((RexxStringPointer)((RexxString *)name)->getStringData(), &temp))
            {
                /* go convert into a method          */
                pMethod = RexxMethod::getMacroCode(name);
            }
        }
        else
        {
            /* go handle instore parms           */
            pMethod = RexxMethod::processInstoreExecution(instoreSource, instoreImage, name);

        }

        method = (RexxMethod *)pMethod;
        if (method == OREF_NULL)           /* couldn't get it?                  */
        {
            /* got an error here                 */
            reportException(Error_Program_unreadable_name, (RexxString *)name);
        }
        // the fully resolved name is just the one handed to us
        fullname = name;
    }

    ProtectedObject address;
    if (initialAddress != NULL)    /* have an address override?         */
    {
        /* use the provided one              */
        address = new_string(initialAddress);
    }
    else
    {
        address = SysInterpreter::getInitialAddressForFile((RexxString *)fullname);
        if ((RexxObject *)address == OREF_NULL)      /* have a real one?                  */
        {
                                           /* use system defined default        */
            address = SysInterpreter::getDefaultCommandEnvironment();
        }
    }
    /* actually need to run this?        */
    if (method != OREF_NULL)
    {
        ProtectedObject result;
        /* Check to see if halt or trace sys */
        activity->queryTrcHlt();           /* were set                          */
                                           /* run and get the result            */
        activity->runProgram(method, name, source_calltype, address, new_arglist->data(), new_arglist->size(), result);

        // if requested, return the result as both a string value and
        // converted into numeric form.
        RexxObject *program_result = (RexxObject *)result;
        if (resultPtr != NULL)
        {        /* if return provided for            */
            copyObjectToRxString(program_result, resultPtr);
        }

        wholenumber_t returnCode;
        if (program_result->numberValue(&returnCode))
        {
            *retCode = returnCode;
        }
    }
}

void RexxInterpreter::copyObjectToRxString(RexxObject *value, RxString *target)
{
    if (value == OREF_NULL)
    {
        MakeRxString(*target, NULL, 0);
        return;
    }

    // get the string version of this
    RexxString *stringValue = value->stringValue();
    // copy to the RxString
    stringValue->copyIntoRxString(target);
}


RexxDirectory *RexxInterpreter::localEnvironment()
/******************************************************************************/
/* Function:  Return the current activation's local environment pointer       */
/******************************************************************************/
{
                                       /* just return the current local     */
                                       /* environment                       */
  return contextActivity()->getLocalEnvironment();
}

bool RexxInterpreter::isPendingUninit(RexxObject *obj)
/******************************************************************************/
/* Function:  Test if an object is going to require its uninit method run.    */
/******************************************************************************/
{
    /* uninit table for this process     */
    RexxObjectTable * uninitTable = singleInstance->uninitTable;
    /* do we have an uninit table? */
    if (uninitTable != OREF_NULL)
    {
        /* is object in the table?           */
        return uninitTable->get(obj) != OREF_NULL;
    }
    return false;
}

void  RexxInterpreter::runUninits()
/******************************************************************************/
/* Function:  Run any UNINIT methods for this activity                        */
/******************************************************************************/
/* NOTE: The routine to iterate across uninit Table isn't quite right, since  */
/*  the removal of zombieObj may move another zombieObj and then doing        */
/*  the next will skip this zombie, we should however catch it next time      */
/*  through.                                                                  */
/*                                                                            */
/******************************************************************************/
{
    /* if we're already processing this, don't try to do this */
    /* recursively. */
    if (singleInstance->processingUninits)
    {
        return;
    }

    /* turn on the recursion flag, and also zero out the count of */
    /* pending uninits to run */
    singleInstance->processingUninits = true;
    singleInstance->pendingUninits = 0;

    RexxObjectTable *table = singleInstance->uninitTable;
    /* retrive the UNINIT table for      */
    /*  process, if it exists            */
    if (table != OREF_NULL)
    {
        // get the activity context.
        RexxActivity *activity = contextActivity();
        // add a native activation as the error termination point.
        activity->pushNewContext();

        RexxObject * zombieObj;              /* obj that needs uninit run.        */
        HashLink iterTable;                  /* iterator for table.               */
        /* uninitTabe exists, run UNINIT     */
        for (iterTable = table->first(); (zombieObj = table->index(iterTable)) != OREF_NULL; iterTable = table->next(iterTable))
        {
            /* is this object readyfor UNINIT?   */
            while (table->value(iterTable) == TheTrueObject)
            {
                /* make sure we don't recurse        */
                table->put(TheFalseObject, zombieObj);
                try
                {
                    zombieObj->uninit();           /* run the UNINIT method           */
                }
                catch (ActivityException)
                {
                    // errors are ignored...
                    activity->clearException();
                }
                                               /* remove zombie from uninit table   */
                table->remove(zombieObj);
                // see if something moved back into this slot
                zombieObj = table->index(iterTable);
                if (zombieObj == OREF_NULL)
                {
                    break;
                }
            }
        }                                  /* now go check next object in table */
        activity->popContext();
    }
    singleInstance->processingUninits = false;
}

void RexxInterpreter::addUninitObject(
    RexxObject *obj)                   /* object to add                     */
/******************************************************************************/
/* Function:  Add an object with an uninit method to the uninit table for     */
/*            a process                                                       */
/******************************************************************************/
{
   RexxObjectTable *uninitTable = singleInstance->uninitTable;
                                       /* is object already in table?       */
   if (uninitTable->get(obj) == OREF_NULL)
   {
       // add this to the UNIT table using a value of .nil.  When it is recognized
       // as unreferenced, it will become .true, and then .false when it the unit
       // method is being run.
       uninitTable->put(TheNilObject, obj);
   }
}

void  RexxInterpreter::removeUninitObject(
    RexxObject *obj)                   /* object to remove                  */
/******************************************************************************/
/* Function:  Remove an object from the uninit tables                         */
/******************************************************************************/
{
    RexxObjectTable *uninitTable = singleInstance->uninitTable;
    // remove the object
    uninitTable->remove(obj);
}


void  RexxInterpreter::uninitObject(RexxObject *dropObj)
/******************************************************************************/
/* Function:  Run UNINIT method for a specific object in this activity        */
/******************************************************************************/
/* NOTE: This method will only be called from the drop function in order to   */
/*  run the uninit for the object that has just been dropped. The object will */
/*  then be removed from the uninit table so that uninit will not be called   */
/*  again.                                                                    */
/*                                                                            */
/******************************************************************************/
{
  RexxObjectTable *table = singleInstance->uninitTable;
  // TRUE means this is ready to run
  if (table->get(dropObj) == TheTrueObject)
  {
                                       /* make sure we don't recurse        */
      table->put(TheFalseObject, dropObj);
      dropObj->uninit();               /* yes, indicate run the UNINIT      */
                                       /* remove zombie from uninit table   */
      table->remove(dropObj);
  }
}

bool RexxInterpreter::requestInterpreter(RexxActivity *activity)
/******************************************************************************/
/* Function:  Request access to the kernel, returning if not obtained         */
/******************************************************************************/
{
    // try to grab this immediately
    if (executionLock.requestImmediate())
    {
#ifdef SHARED
        // have the memory object sync pools if we're going cross process.
        RexxMemory::memoryObject.syncMemoryPools();
#endif
        // set the current global state
        currentActivity = activity;
        currentSettings = activity->getSettings();
        return true;    // flag that we got this ok
    }
    return false;       // requester is going to have to wait
}

void RexxInterpreter::lockInterpreter()
/******************************************************************************/
/* Function:  Request access to the kernel                                    */
/******************************************************************************/
{
  executionLock.request();             /* just request the semaphore        */

                                       /* have more pools been added since  */
                                       /* we left the kernel ?              */

#ifdef SHARED
  // have the memory object sync pools if we're going cross process.
  RexxMemory::memoryObject.syncMemoryPools();
#endif
}

void RexxInterpreter::unlockInterpreter()
/******************************************************************************/
/* Function:  Release the kernel access                                       */
/******************************************************************************/
{
  RexxActivityManager::currentActivity = OREF_NULL;         /* no current activation             */
  currentSettings = &defaultSettings;   /* use default settings              */
  executionLock.release();             /* release the kernel semaphore      */
}


/******************************************************************************/
/* FUNCTION: attach an interpreter instance to a new process.  We need to     */
/*  allocate an instance object for it, but we don so with out an activity    */
/*  yet, because we haven't obtained one yet.  We do need to allocate a       */
/*  Rexx object at this point to finish the process, so we need exclusive     */
/*  access.                                                                   */
/******************************************************************************/
InterpreterInstanceObject *RexxInterpreter::attachInstance(InterpreterInstance *instance)
{
    ExecutionSection lock;      // make sure we have the execution lock

    // create the instance object
    InterpreterInstanceObject *instanceObj = new InterpreterInstanceObject(instance, singleInstance);
    // attach this to the process instance
    singleInstance->addInstance(instanceObj);

    return instanceObj;
}

void RexxInterpreter::addInstance(InterpreterInstanceObject *instance)
{
    // add this to the instance objects table
    instanceObjects->put((RexxObject *)instance, (RexxObject *)instance);
}


RexxActivity *RexxInterpreter::newActivity(bool createThread, SysThread::ThreadPriority priority, InterpreterInstance *instance)
/******************************************************************************/
/* Function:  Create or reuse an activity object                              */
/******************************************************************************/
{
  ResourceSection lock;

  // we only pool activities that are created on new threads.  If we've been
  // asked to create one, then we might be able to reuse a "resting" one created
  // in this process.
  if (createThread)
  {
      // see if we we have an activity in the pool now
      HashLink index = activityPool->first();
      RexxActivity *activity = (RexxActivity *)activityPool->value(index);
      if (activity != OREF_NULL)
      {
          // attach this to the current instance
          activity->attachToInstance(priority, instance);
          return activity;
      }
  }
  // return a new activity object
  return new RexxActivity(createThread, priority, instance);
}

void RexxInterpreter::terminateActivityPool()
/******************************************************************************/
/* Function:   see if there are any Uninit messages need to be send before    */
/*             the process goes away.                                         */
/******************************************************************************/
{
    {
        ResourceSection lock;          // get the resource lock
#ifdef SHARED
        // have the memory object sync pools if we're going cross process.
        RexxMemory::memoryObject.syncMemoryPools();
#endif
        // we're shutting down, create a termination that the shutting down
        // activities will post when they finish.

        // get the counter of activities we need to wait on
        activityPoolSize = activityPool->items();
        // if there are none in the pool, we're cool and get out of here.
        if (activityPoolSize == 0)
        {
            return;
        }

        // we're shutting down, create a termination that the shutting down
        // activities will post when they finish.
        terminationSem.create();
        terminationSem.reset();

        for (HashLink index = activityPool->first(); activityPool->value(index) != OREF_NULL; activityPool->next(index))
        {
            // get the next activity, and tell it to terminate
            RexxActivity *activity = (RexxActivity *)activityPool->value(index);
            activity->terminatePoolActivity();
        }
    }

    // now wait for everything to shut down
    terminationSem.wait();
}


void RexxInterpreter::activityTerminated(RexxActivity *activity)
{
    ResourceSection lock;          // get the resource lock

    // remove from the activity table
    singleInstance->activityPool->remove((RexxObject *)activity);
    singleInstance->activityPoolSize--;
    // was this the last one to shutdown?
    if (singleInstance->activityPoolSize == 0)
    {
        // alert the main waiting thread
        singleInstance->terminationSem.post();
    }
}

bool RexxInterpreter::addToActivityPool(RexxActivity *activity)
/******************************************************************************/
/* Function:  add an activity to the process-specific free pool               */
/******************************************************************************/
{
    ResourceSection lock;          // get the resource lock
    // only add this if we haven't reached our maximum number of retained
    // activities.
    // detach this from the instance
    activity->detachInstance();
    if (singleInstance->activityPoolSize < MAX_ACTIVITY_POOL_SIZE)
    {
        singleInstance->activityPoolSize++;
        singleInstance->activityPool->put((RexxObject *)activity, (RexxObject *)activity);
        return true;
    }
    else
    {
        // just tell this one to go away, we're not keeping it around
        return false;
    }
}

/******************************************************************************/
/* Function:  resolve a proxied object to a named instance.                   */
/******************************************************************************/
RexxObject *RexxInterpreter::resolveProxy(RexxString *name)
{
    // first check the registered proxy table
    RexxObject *result = RexxMemory::memoryObject.getProxiedObject(name);
    if (result == OREF_NULL)
    {
        // if we can't find a registered one, just grab an environment entry
        result = getEnvironment(name);
    }
    return result;
}


int32_t RexxInterpreter::generateRandomNumberSeed()
/******************************************************************************/
/* Function:  Generate a fresh random number seed.                            */
/******************************************************************************/
{
  int           i;                     /* loop counter                      */
                                       /* take the seed from the time       */
  // create a new seed from the time stamp...we use the time function to get
  // a numeric time value, but also add in the last generated seed value
  // so calls close in time sequence do not generate the same random seed.
  randomSeed = randomSeed + (int32_t)time(NULL);
  for (i = 0; i < 13; i++) {           /* randomize the seed number a bit   */
                                       /* scramble the seed a bit           */
      randomSeed = RANDOMIZE(randomSeed);
  }
  return randomSeed;
}

FunctionActivator *RexxInterpreter::findCachedExternalFunction(RexxString *target)
/******************************************************************************/
/* Function:  Search the external function cache.                             */
/******************************************************************************/
{
    return (FunctionActivator *)singleInstance->cachedFunctions->get(target);
}

void RexxInterpreter::addCachedExternalFunction(RexxString *target, FunctionActivator *activator)
/******************************************************************************/
/* Function:  Search the external function cache.                             */
/******************************************************************************/
{
    singleInstance->cachedFunctions->put((RexxObject *)activator, target);
}

/**
 * Locate the activity associated with the current
 * thread and return a RexxCallContext pointer associated
 * with this activity.  A call context is only available when
 * the top-level API context of the activity is either an
 * external function call or an exit call.
 *
 * @return A pointer to the current call context structure, or NULL
 *         if this was an invalid calling condtion.
 */
RexxCallContext *RexxInterpreter::getCurrentCallContext()
{
    // locate an activity associated with this thread
    RexxActivity *activity = RexxActivityManager::findActivityForCurrentThread();
    // and delegate this to the activity
    return activity->getCurrentCallContext();
}

/**
 * Process an external halt event, signaling all interpeter
 * instances to halt.
 */
void RexxInterpreter::processExternalHalt()
{
    ResourceSection lock;              // lock activity changes
    HashLink index;

    RexxObjectTable * table = singleInstance->instanceObjects;

    // signal each of the active instances for this process.
    for (index = table->first(); table->index(index) != OREF_NULL; index = table->next(index))
    {
        InterpreterInstanceObject *instanceObject = (InterpreterInstanceObject *)table->value(index);
        InterpreterInstance *instance = instanceObject->getInstance();
        instance->processExternalHalt();
    }
}
