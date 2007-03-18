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
/* REXX Kernel                                                  oksetup.c     */
/*                                                                            */
/* Setup initial class definitions during an image build                      */
/*                                                                            */
/******************************************************************************/
#include <string.h>
#include "RexxCore.h"
 #include "TableClass.hpp"
 #include "RexxMemory.hpp"
 #include "RexxBehaviour.hpp"
 #include "ClassClass.hpp"
 #include "NumberStringClass.hpp"
 #include "IntegerClass.hpp"
 #include "StringClass.hpp"
 #include "MutableBufferClass.hpp"
 #include "ArrayClass.hpp"
 #include "DirectoryClass.hpp"
 #include "RelationClass.hpp"
 #include "ListClass.hpp"
 #include "QueueClass.hpp"
 #include "SupplierClass.hpp"
 #include "MethodClass.hpp"
 #include "RexxEnvelope.hpp"
 #include "MessageClass.hpp"
 #include "StemClass.hpp"
 #include "RexxNativeCode.hpp"
 #include "RexxActivity.hpp"
 #include "RexxNativeActivation.hpp"
 #include "RexxVariableDictionary.hpp"
 #include "ExpressionVariable.hpp"
 #include "RexxLocalVariables.hpp"
 #include "PointerClass.hpp"
 #include "SysInterpreter.hpp"
 #include "Interpreter.hpp"
 #include "RexxMemory.hpp"
 #include "Package.hpp"


void RexxInterpreter::bootstrapClasses(RootImage *rootImage)
/******************************************************************************/
/* Function:  Bootstrap the initial class environment                         */
/******************************************************************************/
{
    // build the virtual function table array */
    RexxBehaviour::buildVirtualFunctionTable();

    // now create the globally exported class object.  Some of these require
    // specialied processing, some just require creating the class object.
    CreateClassObject(NumberString, RexxClass);

    // create the Integer class instance and global objects
    RexxArray::createInstance(rootImage);

    // create the global names table.  This is done VERY early so
    // things dependent on the names can use them.
    GlobalNames::createNames(rootImage);

    // Rexx library manager needs to initialize
    PackageManager::createInstance(rootImage);

    CreateClassObject(Directory, RexxClass);

    // one more step in the bootstrapping process.  We can create the
    // environment object that is the root of most objects.
    rootImage->globalEnvironment = theEnvironment = new_directory();

    rootImage->interpreterAnchor = theKernel = new_directory();
    theKernel->header.makeProxiedObject();

    // now the public routines and requires sets
    rootImage->publicRoutines = publicRoutines = new_directory();


    // now that we have names, mark these objects for special proxy treatment.
    rootImage->addProxiedObject(theEnvironment, OREF_ENVIRONMENT);
    rootImage->addProxiedObject(theKernel, OREF_KERNEL);

    // pointer needs to created early because following classes need to create
    // pointer object
    RexxPointer::createInstance(rootImage);

    // now some more classes.
    CreateClassObject(Method, RexxClass);
    CreateClassObject(Queue, RexxClass);      /* RexxQueue                         */

    CreateClassObject(List, RexxClass);       /* RexxList                          */
    CreateClassObject(Stem, RexxClass);       /* RexxStem                          */

    // Activity is a big one...needs quit a few classes created at this point
    RexxActivityManager::createInstance(rootImage);  /* RexxActivity                      */

    CreateClassObject(Supplier, RexxClass);   /* RexxSupplier                      */
    CreateClassObject(Message, RexxClass);    /* RexxMessage                       */
    CreateClassObject(MutableBuffer, RexxClass);

                                         /* build the common retriever tables */
    commonRetrievers = (RexxDirectory *)new_directory();
                                         /* add all of the special variables  */
    commonRetrievers->put((RexxObject *)new RexxParseVariable(OREF_SELF, VARIABLE_SELF), OREF_SELF);
    commonRetrievers->put((RexxObject *)new RexxParseVariable(OREF_SUPER, VARIABLE_SUPER), OREF_SUPER);
    commonRetrievers->put((RexxObject *)new RexxParseVariable(OREF_SIGL, VARIABLE_SIGL), OREF_SIGL);
    commonRetrievers->put((RexxObject *)new RexxParseVariable(OREF_RC, VARIABLE_RC), OREF_RC);
    commonRetrievers->put((RexxObject *)new RexxParseVariable(OREF_RESULT, VARIABLE_RESULT), OREF_RESULT);

    // this is added to the saved image too
    rootImage->commonRetrievers = commonRetrievers;
    RexxMemory::memoryObject.enableOrefChecks();     /* enable setCheckOrefs...           */
}


void RexxInterpreter::defineKernelMethod(
    stringchar_t  * name,              /* ASCII-Z name for the method       */
    RexxBehaviour * behaviour,         /* behaviour to use                  */
    PCPPM           entryPoint,        /* method's entry point              */
    size_t          arguments )        /* count of arguments                */
/******************************************************************************/
/* Function:  Add a C++ method to an object's behaviour                       */
/******************************************************************************/
{
  behaviour->define(addGlobalName(name), RexxMethod::createKernelMethod(entryPoint, arguments));
}

void RexxInterpreter::defineProtectedKernelMethod(
    stringchar_t  * name,              /* ASCII-Z name for the method       */
    RexxBehaviour * behaviour,         /* behaviour to use                  */
    PCPPM           entryPoint,        /* method's entry point              */
    size_t          arguments )        /* count of arguments                */
/******************************************************************************/
/* Function:  Add a C++ method to an object's behaviour                       */
/******************************************************************************/
{
  behaviour->define(addGlobalName(name), RexxMethod::createProtectedKernelMethod(entryPoint, arguments));
}

void RexxInterpreter::definePrivateKernelMethod(
    stringchar_t  * name,              /* ASCII-Z name for the method       */
    RexxBehaviour * behaviour,         /* behaviour to use                  */
    PCPPM           entryPoint,        /* method's entry point              */
    size_t          arguments )        /* count of arguments                */
/******************************************************************************/
/* Function:  Add a C++ method to an object's behaviour                       */
/******************************************************************************/
{
  behaviour->define(addGlobalName(name), RexxMethod::createPrivateKernelMethod(entryPoint, arguments));
}

bool RexxInterpreter::exportBaseClasses()
/******************************************************************************/
/* Function:  Build the Object REXX image, including hand building the        */
/*            base set of REXX classes.                                       */
/******************************************************************************/
{
  RexxString *symb;                    /* symbolic name for added methods   */
  RexxString *programName;             /* name of the image file            */
  RexxMethod *meth;                    /* added method object               */
  RexxDirectory *kernel_methods;       /* table of exported kernel_methods  */
/******************************************************************************/
/* The following Rexx classes that are exposed to the users are set up as    */
/* subclassable classes.                                                     */
/*****************************************************************************/

     /* The NEW method is exposed for the CLASS class behaviour.             */
     /* The CLASS class needs the methods of the CLASS instance behaviour    */
     /* so the instance behaviour methods are also in the CLASS class mdict. */
     /*                                                                      */
     /* Also Since the CLASS class needs OBJECT instance methods the         */
     /* OBJECT class is setup. Then the class method SUBCLASSABLE can be     */
     /* invoked on OBJECT then CLASS and then all the subclassable classes.  */

                                       /* add the Rexx class NEW method     */
  defineKernelMethod(CHAR_NEW, TheClassClassBehaviour, CPPM(RexxClass::newRexx), A_COUNT);

                                       /* set the scope of the method to    */
                                       /* the CLASS scope                   */
  TheClassClassBehaviour->setMethodDictionaryScope(TheClassClass);

                                       /* add the instance methods to the   */
                                       /* class's instance mdict            */
  defineProtectedKernelMethod(CHAR_BASECLASS       ,TheClassBehaviour, CPPM(RexxClass::getBaseClass), 0);
  defineProtectedKernelMethod(CHAR_DEFINE          ,TheClassBehaviour, CPPM(RexxClass::defineMethod), 2);
  defineProtectedKernelMethod(CHAR_DEFINE_METHODS  ,TheClassBehaviour, CPPM(RexxClass::defineMethods), 1);
  defineProtectedKernelMethod(CHAR_DELETE ,TheClassBehaviour, CPPM(RexxClass::deleteMethod), 1);
  defineKernelMethod(CHAR_ENHANCED        ,TheClassBehaviour, CPPM(RexxClass::enhanced), A_COUNT);
  defineKernelMethod(CHAR_ID              ,TheClassBehaviour, CPPM(RexxClass::getId), 0);
  defineKernelMethod(CHAR_INHERIT         ,TheClassBehaviour, CPPM(RexxClass::inherit), 2);
  defineProtectedKernelMethod(CHAR_METACLASS       ,TheClassBehaviour, CPPM(RexxClass::getMetaClass), 0);
  defineKernelMethod(CHAR_METHOD          ,TheClassBehaviour, CPPM(RexxClass::method), 1);
  defineKernelMethod(CHAR_METHODS         ,TheClassBehaviour, CPPM(RexxClass::methods), 1);
  defineKernelMethod(CHAR_MIXINCLASS      ,TheClassBehaviour, CPPM(RexxClass::mixinclass), 3);
  defineKernelMethod(CHAR_QUERYMIXINCLASS ,TheClassBehaviour, CPPM(RexxClass::queryMixinClass), 0);
  defineKernelMethod(CHAR_SUBCLASS        ,TheClassBehaviour, CPPM(RexxClass::subclass), 3);
  defineProtectedKernelMethod(CHAR_SUBCLASSES      ,TheClassBehaviour, CPPM(RexxClass::getSubClasses), 0);
  defineProtectedKernelMethod(CHAR_SUPERCLASSES    ,TheClassBehaviour, CPPM(RexxClass::getSuperClasses), 0);
  defineProtectedKernelMethod(CHAR_UNINHERIT       ,TheClassBehaviour, CPPM(RexxClass::uninherit), 1);
                                       /* Class operator methods....        */
  defineKernelMethod(CHAR_EQUAL                  ,TheClassBehaviour, CPPM(RexxClass::equal), 1);
  defineKernelMethod(CHAR_STRICT_EQUAL           ,TheClassBehaviour, CPPM(RexxClass::strictEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_EQUAL        ,TheClassBehaviour, CPPM(RexxClass::notEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_GREATERTHAN   ,TheClassBehaviour, CPPM(RexxClass::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN_LESSTHAN   ,TheClassBehaviour, CPPM(RexxClass::notEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_EQUAL ,TheClassBehaviour, CPPM(RexxClass::notEqual), 1);
                                       /* and the private class methods     */
  defineProtectedKernelMethod(CHAR_SHRIEKREXXDEFINED,TheClassBehaviour, CPPM(RexxClass::setRexxDefined), 0);

                                       /* set the scope of the methods to   */
                                       /* the CLASS scope                   */
  TheClassBehaviour->setMethodDictionaryScope(TheClassClass);

     /************************************************************************/
     /*                                                                      */
     /* The OBJECT class and instance behaviour mdict's are needed next      */
     /*                                                                      */
     /************************************************************************/

                                       /* add the NEW method to the OBJECT  */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW, TheObjectClassBehaviour, CPPM(RexxObject::newRexx), A_COUNT);

                                       /* set the scope of the method to    */
                                       /* the OBJECT scope                  */
  TheObjectClassBehaviour->setMethodDictionaryScope(TheObjectClass);

                                       /* now set up the instance behaviour */
                                       /* mdict                             */
  defineKernelMethod(CHAR_INIT                   ,TheObjectBehaviour, CPPM(RexxObject::init), 0);
  defineKernelMethod(CHAR_EQUAL                  ,TheObjectBehaviour, CPPM(RexxObject::equal), 1);
  defineKernelMethod(CHAR_STRICT_EQUAL           ,TheObjectBehaviour, CPPM(RexxObject::strictEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_EQUAL        ,TheObjectBehaviour, CPPM(RexxObject::notEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_GREATERTHAN   ,TheObjectBehaviour, CPPM(RexxObject::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN_LESSTHAN   ,TheObjectBehaviour, CPPM(RexxObject::notEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_EQUAL ,TheObjectBehaviour, CPPM(RexxObject::strictNotEqual), 1);
  defineKernelMethod(CHAR_COPY                   ,TheObjectBehaviour, CPPM(RexxObject::copyRexx), 0);
  defineKernelMethod(CHAR_CLASS                  ,TheObjectBehaviour, CPPM(RexxObject::classObject), 0);
  defineKernelMethod(CHAR_HASMETHOD              ,TheObjectBehaviour, CPPM(RexxObject::hasMethodRexx), 1);
  defineKernelMethod(CHAR_DEFAULTNAME            ,TheObjectBehaviour, CPPM(RexxObject::defaultNameRexx), 0);
  defineKernelMethod(CHAR_OBJECTNAME             ,TheObjectBehaviour, CPPM(RexxObject::objectName), 0);
  defineKernelMethod(CHAR_OBJECTNAMEEQUALS       ,TheObjectBehaviour, CPPM(RexxObject::objectNameEquals), 1);
  defineKernelMethod(CHAR_REQUEST                ,TheObjectBehaviour, CPPM(RexxObject::requestRexx), 1);
  defineKernelMethod(CHAR_START                  ,TheObjectBehaviour, CPPM(RexxObject::start), A_COUNT);
  defineKernelMethod(CHAR_STRING                 ,TheObjectBehaviour, CPPM(RexxObject::stringRexx), 0);
  defineKernelMethod(CHAR_ISINSTANCEOF           ,TheObjectBehaviour, CPPM(RexxObject::isInstanceOfRexx), 1);
  definePrivateKernelMethod(CHAR_RUN             ,TheObjectBehaviour, CPPM(RexxObject::run), A_COUNT);
  definePrivateKernelMethod(CHAR_SETMETHOD       ,TheObjectBehaviour, CPPM(RexxObject::setMethod), 3);
  definePrivateKernelMethod(CHAR_UNSETMETHOD     ,TheObjectBehaviour, CPPM(RexxObject::unsetMethod), 1);
                                       /* set the scope of the methods to   */
                                       /* the OBJECT scope                  */
  TheObjectBehaviour->setMethodDictionaryScope(TheObjectClass);
                                       /* Now call the class subclassable   */
                                       /* method for OBJECT then CLASS      */
  TheObjectClass->subClassable((stringchar_t *)"Object");
  TheClassClass->subClassable((stringchar_t *)"Class");

  /************************************** The rest of the classes can now be */
  /************************************** set up.                            */

  /***************************************************************************/
  /*           ARRAY                                                         */
  /***************************************************************************/

  defineKernelMethod(CHAR_NEW, TheArrayClassBehaviour, CPPM(RexxArray::newRexx), A_COUNT);
  defineKernelMethod(CHAR_OF,  TheArrayClassBehaviour, CPPM(RexxArray::of), A_COUNT);
                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheArrayClassBehaviour->setMethodDictionaryScope(TheArrayClass);

  defineKernelMethod(CHAR_BRACKETS     ,TheArrayBehaviour, CPPM(RexxArray::getRexx), A_COUNT);
  defineKernelMethod(CHAR_BRACKETSEQUAL,TheArrayBehaviour, CPPM(RexxArray::putRexx), A_COUNT);
  defineKernelMethod(CHAR_AT           ,TheArrayBehaviour, CPPM(RexxArray::getRexx), A_COUNT);
  defineKernelMethod(CHAR_DIMENSION    ,TheArrayBehaviour, CPPM(RexxArray::dimension), 1);
  defineKernelMethod(CHAR_HASINDEX     ,TheArrayBehaviour, CPPM(RexxArray::hasIndexRexx), A_COUNT);
  defineKernelMethod(CHAR_ITEMS        ,TheArrayBehaviour, CPPM(RexxArray::items), 0);
  defineKernelMethod(CHAR_MAKEARRAY    ,TheArrayBehaviour, CPPM(RexxObject::makeArrayRexx), 0);
  defineKernelMethod(CHAR_ALLITEMS     ,TheArrayBehaviour, CPPM(RexxArray::allItems), 0);
  defineKernelMethod(CHAR_ALLINDEXES   ,TheArrayBehaviour, CPPM(RexxArray::allIndexes), 0);
  defineKernelMethod(CHAR_PUT          ,TheArrayBehaviour, CPPM(RexxArray::putRexx), A_COUNT);
  defineKernelMethod(CHAR_REMOVE       ,TheArrayBehaviour, CPPM(RexxArray::removeRexx), A_COUNT);
  defineKernelMethod(CHAR_SECTION      ,TheArrayBehaviour, CPPM(RexxArray::sectionRexx), 2);
  defineKernelMethod(CHAR_SIZE         ,TheArrayBehaviour, CPPM(RexxArray::sizeRexx), 0);
  defineKernelMethod(CHAR_SUPPLIER     ,TheArrayBehaviour, CPPM(RexxArray::supplier), 0);
  defineKernelMethod(CHAR_FIRST        ,TheArrayBehaviour, CPPM(RexxArray::firstRexx), 0);
  defineKernelMethod(CHAR_LAST         ,TheArrayBehaviour, CPPM(RexxArray::lastRexx), 0);
  defineKernelMethod(CHAR_NEXT         ,TheArrayBehaviour, CPPM(RexxArray::nextRexx), 1);
  defineKernelMethod(CHAR_PREVIOUS     ,TheArrayBehaviour, CPPM(RexxArray::previousRexx), 1);
  defineKernelMethod(CHAR_MAKESTRING   ,TheArrayBehaviour, CPPM(RexxArray::makeString), 1);   /*THU006*/
                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheArrayBehaviour->setMethodDictionaryScope(TheArrayClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheArrayClass->subClassable((stringchar_t *)"Array");

  /***************************************************************************/
  /*           DIRECTORY                                                     */
  /***************************************************************************/

  defineKernelMethod(CHAR_NEW           , TheDirectoryClassBehaviour, CPPM(RexxDirectory::newRexx), A_COUNT);

                                       /* set the scope of the method to    */
                                       /* this classes oref                 */
  TheDirectoryClassBehaviour->setMethodDictionaryScope(TheDirectoryClass);

                                       /* add the instance methods          */
  defineKernelMethod(CHAR_BRACKETS      , TheDirectoryBehaviour, CPPM(RexxDirectory::atRexx), 1);
  defineKernelMethod(CHAR_BRACKETSEQUAL , TheDirectoryBehaviour, CPPM(RexxDirectory::put), 2);
  defineKernelMethod(CHAR_AT            , TheDirectoryBehaviour, CPPM(RexxDirectory::atRexx), 1);
  defineKernelMethod(CHAR_ENTRY         , TheDirectoryBehaviour, CPPM(RexxDirectory::entryRexx), 1);
  defineKernelMethod(CHAR_HASENTRY      , TheDirectoryBehaviour, CPPM(RexxDirectory::hasEntry), 1);
  defineKernelMethod(CHAR_HASINDEX      , TheDirectoryBehaviour, CPPM(RexxDirectory::hasIndex), 1);
  defineKernelMethod(CHAR_ITEMS         , TheDirectoryBehaviour, CPPM(RexxDirectory::itemsRexx), 0);
  defineKernelMethod(CHAR_MAKEARRAY     , TheDirectoryBehaviour, CPPM(RexxObject::makeArrayRexx), 0);
  defineKernelMethod(CHAR_ALLITEMS      , TheDirectoryBehaviour, CPPM(RexxDirectory::values), 0);
  defineKernelMethod(CHAR_ALLINDEXES    , TheDirectoryBehaviour, CPPM(RexxDirectory::allIndexes), 0);
  defineKernelMethod(CHAR_PUT           , TheDirectoryBehaviour, CPPM(RexxDirectory::put), 2);
  defineKernelMethod(CHAR_REMOVE        , TheDirectoryBehaviour, CPPM(RexxDirectory::remove), 1);
  defineKernelMethod(CHAR_SETENTRY      , TheDirectoryBehaviour, CPPM(RexxDirectory::setEntry), 2);
  defineProtectedKernelMethod(CHAR_SETMETHOD   , TheDirectoryBehaviour, CPPM(RexxDirectory::setMethod), 2);
  defineKernelMethod(CHAR_SUPPLIER      , TheDirectoryBehaviour, CPPM(RexxDirectory::supplier), 0);
  defineKernelMethod(CHAR_UNKNOWN       , TheDirectoryBehaviour, CPPM(RexxObject::unknownRexx), 2);
  defineProtectedKernelMethod(CHAR_UNSETMETHOD   , TheDirectoryBehaviour, CPPM(RexxDirectory::remove), 1);  // ENG004M

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheDirectoryBehaviour->setMethodDictionaryScope(TheDirectoryClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheDirectoryClass->subClassable((stringchar_t *)"Directory");


  /***************************************************************************/
  /*           LIST                                                          */
  /***************************************************************************/

                                       /* add the class behaviour methods   */
  defineKernelMethod(CHAR_NEW           , TheListClassBehaviour, CPPM(RexxList::newRexx), A_COUNT);
  defineKernelMethod(CHAR_OF            , TheListClassBehaviour, CPPM(RexxList::classOf), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheListClassBehaviour->setMethodDictionaryScope(TheListClass);

                                       /* add the instance behaviour methods*/
  defineKernelMethod(CHAR_BRACKETS     ,TheListBehaviour, CPPM(RexxList::value), 1);
  defineKernelMethod(CHAR_BRACKETSEQUAL,TheListBehaviour, CPPM(RexxList::put), 2);
  defineKernelMethod(CHAR_MAKEARRAY    ,TheListBehaviour, CPPM(RexxObject::makeArrayRexx), 0);
  defineKernelMethod(CHAR_ALLITEMS     ,TheListBehaviour, CPPM(RexxList::allItems), 0);
  defineKernelMethod(CHAR_ALLINDEXES   ,TheListBehaviour, CPPM(RexxList::allIndexes), 0);
  defineKernelMethod(CHAR_AT           ,TheListBehaviour, CPPM(RexxList::value), 1);
  defineKernelMethod(CHAR_FIRST        ,TheListBehaviour, CPPM(RexxList::firstRexx), 0);
  defineKernelMethod(CHAR_FIRSTITEM    ,TheListBehaviour, CPPM(RexxList::firstItem), 0);
  defineKernelMethod(CHAR_HASINDEX     ,TheListBehaviour, CPPM(RexxList::hasIndex), 1);
  defineKernelMethod(CHAR_INSERT       ,TheListBehaviour, CPPM(RexxList::insertRexx), 2);
  defineKernelMethod(CHAR_ITEMS        ,TheListBehaviour, CPPM(RexxList::itemsRexx), 0);
  defineKernelMethod(CHAR_LAST         ,TheListBehaviour, CPPM(RexxList::lastRexx), 0);
  defineKernelMethod(CHAR_LASTITEM     ,TheListBehaviour, CPPM(RexxList::lastItem), 0);
  defineKernelMethod(CHAR_NEXT         ,TheListBehaviour, CPPM(RexxList::next), 1);
  defineKernelMethod(CHAR_PREVIOUS     ,TheListBehaviour, CPPM(RexxList::previous), 1);
  defineKernelMethod(CHAR_PUT          ,TheListBehaviour, CPPM(RexxList::put), 2);
  defineKernelMethod(CHAR_REMOVE       ,TheListBehaviour, CPPM(RexxList::remove), 1);
  defineKernelMethod(CHAR_SECTION      ,TheListBehaviour, CPPM(RexxList::section), 2);
  defineKernelMethod(CHAR_SUPPLIER     ,TheListBehaviour, CPPM(RexxList::supplier), 0);
                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheListBehaviour->setMethodDictionaryScope(TheListClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheListClass->subClassable((stringchar_t *)"List");

  /***************************************************************************/
  /*           MESSAGE                                                       */
  /***************************************************************************/

                                       /* Define the NEW method in the      */
                                       /* class behaviour mdict             */
  defineKernelMethod(CHAR_NEW      , TheMessageClassBehaviour, CPPM(RexxMessage::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheMessageClassBehaviour->setMethodDictionaryScope(TheMessageClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */

  defineKernelMethod(CHAR_COMPLETED, TheMessageBehaviour, CPPM(RexxMessage::completed), 0);
  defineKernelMethod(CHAR_HASERROR,  TheMessageBehaviour, CPPMSG(RexxMessage::hasError), 0);
  defineKernelMethod(CHAR_NOTIFY   , TheMessageBehaviour, CPPM(RexxMessage::notify), 1);
  defineKernelMethod(CHAR_RESULT   , TheMessageBehaviour, CPPM(RexxMessage::result), 0);
  defineKernelMethod(CHAR_ERRORCONDITION , TheMessageBehaviour, CPPMSG(RexxMessage::errorCondition), 0);
  defineKernelMethod(CHAR_SEND     , TheMessageBehaviour, CPPM(RexxMessage::send), 1);
  defineKernelMethod(CHAR_START    , TheMessageBehaviour, CPPM(RexxMessage::start), 1);
  defineKernelMethod(CHAR_TARGET   , TheMessageBehaviour, CPPMSG(RexxMessage::messageTarget), 0);
  defineKernelMethod(CHAR_MESSAGENAME  , TheMessageBehaviour, CPPMSG(RexxMessage::messageName), 0);
  defineKernelMethod(CHAR_ARGUMENTS  , TheMessageBehaviour, CPPMSG(RexxMessage::arguments), 0);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheMessageBehaviour->setMethodDictionaryScope(TheMessageClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheMessageClass->subClassable((stringchar_t *)"Message");

  /***************************************************************************/
  /*           METHOD                                                        */
  /***************************************************************************/

                                       /* Add the NEW methods to the        */
                                       /* class behaviour                   */
  defineKernelMethod(CHAR_NEW     , TheMethodClassBehaviour, CPPM(RexxMethod::newRexx), A_COUNT);
  defineKernelMethod(CHAR_NEWFILE , TheMethodClassBehaviour, CPPM(RexxMethod::newFileRexx), 1);
                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheMethodClassBehaviour->setMethodDictionaryScope(TheMethodClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_SETUNGUARDED ,TheMethodBehaviour, CPPM(RexxMethod::setUnGuardedRexx), 0);
  defineKernelMethod(CHAR_SETGUARDED   ,TheMethodBehaviour, CPPM(RexxMethod::setGuardedRexx), 0);
  defineKernelMethod(CHAR_SETPRIVATE   ,TheMethodBehaviour, CPPM(RexxMethod::setPrivateRexx), 0);
  defineProtectedKernelMethod(CHAR_SETPROTECTED ,TheMethodBehaviour, CPPM(RexxMethod::setProtectedRexx), 0);
  defineProtectedKernelMethod(CHAR_SETSECURITYMANAGER ,TheMethodBehaviour, CPPM(RexxMethod::setSecurityManager), 1);
  defineKernelMethod(CHAR_SOURCE       ,TheMethodBehaviour, CPPM(RexxMethod::sourceRexx), 0);
                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheMethodBehaviour->setMethodDictionaryScope(TheMethodClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheMethodClass->subClassable((stringchar_t *)"Method");

  /***************************************************************************/
  /*           QUEUE                                                         */
  /***************************************************************************/

                                       /* Add the NEW method to the class   */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW, TheQueueClassBehaviour, CPPM(RexxQueue::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheQueueClassBehaviour->setMethodDictionaryScope(TheQueueClass);

                                       /* Add the instance methods to the   */
                                       /* instance method mdict             */

  defineKernelMethod(CHAR_MAKEARRAY     ,TheQueueBehaviour, CPPM(RexxObject::makeArrayRexx), 0);
  defineKernelMethod(CHAR_ALLITEMS      ,TheQueueBehaviour, CPPM(RexxList::allItems), 0);
  defineKernelMethod(CHAR_ALLINDEXES    ,TheQueueBehaviour, CPPM(RexxQueue::allIndexes), 0);
  defineKernelMethod(CHAR_ITEMS         ,TheQueueBehaviour, CPPM(RexxList::itemsRexx), 0);
  defineKernelMethod(CHAR_SUPPLIER      ,TheQueueBehaviour, CPPM(RexxQueue::supplier), 0);
  defineKernelMethod(CHAR_PUSH          ,TheQueueBehaviour, CPPM(RexxQueue::pushRexx), 1);
  defineKernelMethod(CHAR_PEEK          ,TheQueueBehaviour, CPPM(RexxQueue::peek), 0);
  defineKernelMethod(CHAR_PULL          ,TheQueueBehaviour, CPPM(RexxQueue::pullRexx), 0);
  defineKernelMethod(CHAR_QUEUE         ,TheQueueBehaviour, CPPM(RexxQueue::queueRexx), 1);
  defineKernelMethod(CHAR_BRACKETS      ,TheQueueBehaviour, CPPM(RexxQueue::at), 1);
  defineKernelMethod(CHAR_BRACKETSEQUAL ,TheQueueBehaviour, CPPM(RexxQueue::put), 2);
  defineKernelMethod(CHAR_AT            ,TheQueueBehaviour, CPPM(RexxQueue::at), 1);
  defineKernelMethod(CHAR_HASINDEX      ,TheQueueBehaviour, CPPM(RexxQueue::hasindex), 1);
  defineKernelMethod(CHAR_PUT           ,TheQueueBehaviour, CPPM(RexxQueue::put), 2);
  defineKernelMethod(CHAR_REMOVE        ,TheQueueBehaviour, CPPM(RexxQueue::remove), 1);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheQueueBehaviour->setMethodDictionaryScope(TheQueueClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheQueueClass->subClassable((stringchar_t *)"Queue");

  /***************************************************************************/
  /*           RELATION                                                      */
  /***************************************************************************/

                                       /* Add the NEW method to the         */
                                       /* class behaviour mdict             */
  defineKernelMethod(CHAR_NEW          , TheRelationClassBehaviour, CPPM(RexxRelation::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheRelationClassBehaviour->setMethodDictionaryScope(TheRelationClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_BRACKETS     , TheRelationBehaviour, CPPM(RexxHashTableCollection::getRexx), 1);
  defineKernelMethod(CHAR_BRACKETSEQUAL, TheRelationBehaviour, CPPM(RexxRelation::put), 2);
  defineKernelMethod(CHAR_ALLAT        , TheRelationBehaviour, CPPM(RexxHashTableCollection::allAt), 1);
  defineKernelMethod(CHAR_ALLINDEX     , TheRelationBehaviour, CPPM(RexxRelation::allIndex), 1);
  defineKernelMethod(CHAR_ALLITEMS     , TheRelationBehaviour, CPPM(RexxHashTableCollection::allItems), 0);
  defineKernelMethod(CHAR_ALLINDEXES   , TheRelationBehaviour, CPPM(RexxHashTableCollection::allIndexes), 0);
  defineKernelMethod(CHAR_MAKEARRAY    , TheRelationBehaviour, CPPM(RexxObject::makeArrayRexx), 0);
  defineKernelMethod(CHAR_AT           , TheRelationBehaviour, CPPM(RexxHashTableCollection::getRexx), 1);
  defineKernelMethod(CHAR_HASINDEX     , TheRelationBehaviour, CPPM(RexxHashTableCollection::hasIndex), 1);
  defineKernelMethod(CHAR_HASITEM      , TheRelationBehaviour, CPPM(RexxRelation::hasItem), 2);
  defineKernelMethod(CHAR_INDEX        , TheRelationBehaviour, CPPM(RexxRelation::getIndex), 1);
  defineKernelMethod(CHAR_ITEMS        , TheRelationBehaviour, CPPM(RexxRelation::itemsRexx), 1);
  defineKernelMethod(CHAR_PUT          , TheRelationBehaviour, CPPM(RexxRelation::put), 2);
  defineKernelMethod(CHAR_REMOVE       , TheRelationBehaviour, CPPM(RexxHashTableCollection::removeRexx), 1);
  defineKernelMethod(CHAR_REMOVEITEM   , TheRelationBehaviour, CPPM(RexxRelation::removeItemRexx), 2);
  defineKernelMethod(CHAR_SUPPLIER     , TheRelationBehaviour, CPPM(RexxRelation::supplier), 1);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheRelationBehaviour->setMethodDictionaryScope(TheRelationClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheRelationClass->subClassable((stringchar_t *)"Relation");

  /***************************************************************************/
  /*           STEM                                                          */
  /***************************************************************************/

                                       /* Add the NEW method to the class   */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW, TheStemClassBehaviour, CPPM(RexxStem::newRexx), A_COUNT);
                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheStemClassBehaviour->setMethodDictionaryScope(TheStemClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_BRACKETS      ,TheStemBehaviour, CPPM(RexxStem::bracket), A_COUNT);
  defineKernelMethod(CHAR_BRACKETSEQUAL ,TheStemBehaviour, CPPM(RexxStem::bracketEqual), A_COUNT);
  defineKernelMethod(CHAR_MAKEARRAY     ,TheStemBehaviour, CPPM(RexxObject::makeArrayRexx), 0);
  defineKernelMethod(CHAR_ALLITEMS      ,TheStemBehaviour, CPPM(RexxStem::allItems), 0);
  defineKernelMethod(CHAR_ALLINDEXES    ,TheStemBehaviour, CPPM(RexxStem::allIndexes), 0);
  defineKernelMethod(CHAR_REQUEST       ,TheStemBehaviour, CPPM(RexxStem::request), 1);
  defineKernelMethod(CHAR_UNKNOWN       ,TheStemBehaviour, CPPM(RexxObject::unknownRexx), 2);
  defineKernelMethod(CHAR_SUPPLIER      ,TheStemBehaviour, CPPM(RexxStem::supplier), 0);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheStemBehaviour->setMethodDictionaryScope(TheStemClass);

                                       /* delete these methods from stems by*/
                                       /* using .nil as the methobj         */
  TheStemBehaviour->define(addGlobalName(CHAR_STRICT_EQUAL)          , (RexxMethod *)TheNilObject);
  TheStemBehaviour->define(addGlobalName(CHAR_EQUAL)                 , (RexxMethod *)TheNilObject);
  TheStemBehaviour->define(addGlobalName(CHAR_STRICT_BACKSLASH_EQUAL), (RexxMethod *)TheNilObject);
  TheStemBehaviour->define(addGlobalName(CHAR_BACKSLASH_EQUAL)       , (RexxMethod *)TheNilObject);
  TheStemBehaviour->define(addGlobalName(CHAR_LESSTHAN_GREATERTHAN)  , (RexxMethod *)TheNilObject);
  TheStemBehaviour->define(addGlobalName(CHAR_GREATERTHAN_LESSTHAN)  , (RexxMethod *)TheNilObject);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheStemClass->subClassable((stringchar_t *)"Stem");

  /***************************************************************************/
  /*           STRING                                                        */
  /***************************************************************************/

                                       /* Add the NEW method to the class   */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW, TheStringClassBehaviour, CPPM(RexxClass::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheStringClassBehaviour->setMethodDictionaryScope(TheStringClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_NULLSTRING                   ,TheStringBehaviour, CPPM(RexxString::concatRexx), 1);
  defineKernelMethod(CHAR_BLANK                        ,TheStringBehaviour, CPPM(RexxString::concatBlank), 1);
  defineKernelMethod(CHAR_LENGTH                       ,TheStringBehaviour, CPPM(RexxString::lengthRexx), 0);
  defineKernelMethod(CHAR_CONCATENATE                  ,TheStringBehaviour, CPPM(RexxString::concatRexx), 1);
  defineKernelMethod(CHAR_CENTER                       ,TheStringBehaviour, CPPM(RexxString::center), 2);
  defineKernelMethod(CHAR_CENTRE                       ,TheStringBehaviour, CPPM(RexxString::center), 2);
  defineKernelMethod(CHAR_DATATYPE                     ,TheStringBehaviour, CPPM(RexxString::dataType), 1);
  defineKernelMethod(CHAR_DELSTR                       ,TheStringBehaviour, CPPM(RexxString::delstr), 2);
  defineKernelMethod(CHAR_FORMAT                       ,TheStringBehaviour, CPPM(RexxString::format), 4);
  defineKernelMethod(CHAR_INSERT                       ,TheStringBehaviour, CPPM(RexxString::insert), 4);
  defineKernelMethod(CHAR_LEFT                         ,TheStringBehaviour, CPPM(RexxString::left), 2);
  defineKernelMethod(CHAR_OVERLAY                      ,TheStringBehaviour, CPPM(RexxString::overlay), 4);
  defineKernelMethod(CHAR_REVERSE                      ,TheStringBehaviour, CPPM(RexxString::reverse), 0);
  defineKernelMethod(CHAR_RIGHT                        ,TheStringBehaviour, CPPM(RexxString::right), 2);
  defineKernelMethod(CHAR_STRIP                        ,TheStringBehaviour, CPPM(RexxString::strip), 2);
  defineKernelMethod(CHAR_SUBSTR                       ,TheStringBehaviour, CPPM(RexxString::substr), 3);
  defineKernelMethod(CHAR_SUBCHAR                      ,TheStringBehaviour, CPPM(RexxString::subchar), 1);
  defineKernelMethod(CHAR_DELWORD                      ,TheStringBehaviour, CPPM(RexxString::delWord), 2);
  defineKernelMethod(CHAR_SPACE                        ,TheStringBehaviour, CPPM(RexxString::space), 2);
  defineKernelMethod(CHAR_EXTRACTWORDS                 ,TheStringBehaviour, CPPM(RexxString::extractWords), 2);
  defineKernelMethod(CHAR_SUBWORD                      ,TheStringBehaviour, CPPM(RexxString::subWord), 2);
  defineKernelMethod(CHAR_TRUNC                        ,TheStringBehaviour, CPPM(RexxString::trunc), 1);
  defineKernelMethod(CHAR_WORD                         ,TheStringBehaviour, CPPM(RexxString::word), 1);
  defineKernelMethod(CHAR_WORDINDEX                    ,TheStringBehaviour, CPPM(RexxString::wordIndex), 1);
  defineKernelMethod(CHAR_WORDLENGTH                   ,TheStringBehaviour, CPPM(RexxString::wordLength), 1);
  defineKernelMethod(CHAR_WORDPOS                      ,TheStringBehaviour, CPPM(RexxString::wordPos), 2);
  defineKernelMethod(CHAR_WORDS                        ,TheStringBehaviour, CPPM(RexxString::words), 0);
  defineKernelMethod(CHAR_ABBREV                       ,TheStringBehaviour, CPPM(RexxString::abbrev), 2);
  defineKernelMethod(CHAR_CHANGESTR                    ,TheStringBehaviour, CPPM(RexxString::changeStr), 2);
  defineKernelMethod(CHAR_COMPARE                      ,TheStringBehaviour, CPPM(RexxString::compare), 2);
  defineKernelMethod(CHAR_COPIES                       ,TheStringBehaviour, CPPM(RexxString::copies), 1);
  defineKernelMethod(CHAR_COUNTSTR                     ,TheStringBehaviour, CPPM(RexxString::countStrRexx), 1);
  defineKernelMethod(CHAR_LASTPOS                      ,TheStringBehaviour, CPPM(RexxString::lastPosRexx), 2);
  defineKernelMethod(CHAR_POS                          ,TheStringBehaviour, CPPM(RexxString::posRexx), 2);
  defineKernelMethod(CHAR_TRANSLATE                    ,TheStringBehaviour, CPPM(RexxString::translate), 3);
  defineKernelMethod(CHAR_VERIFY                       ,TheStringBehaviour, CPPM(RexxString::verify), 3);
  defineKernelMethod(CHAR_BITAND                       ,TheStringBehaviour, CPPM(RexxString::bitAnd), 2);
  defineKernelMethod(CHAR_BITOR                        ,TheStringBehaviour, CPPM(RexxString::bitOr), 2);
  defineKernelMethod(CHAR_BITXOR                       ,TheStringBehaviour, CPPM(RexxString::bitXor), 2);
  defineKernelMethod(CHAR_B2X                          ,TheStringBehaviour, CPPM(RexxString::b2x), 0);
  defineKernelMethod(CHAR_C2D                          ,TheStringBehaviour, CPPM(RexxString::c2d), 1);
  defineKernelMethod(CHAR_C2X                          ,TheStringBehaviour, CPPM(RexxString::c2x), 0);
  defineKernelMethod(CHAR_D2C                          ,TheStringBehaviour, CPPM(RexxString::d2c), 1);
  defineKernelMethod(CHAR_D2X                          ,TheStringBehaviour, CPPM(RexxString::d2x), 1);
  defineKernelMethod(CHAR_X2B                          ,TheStringBehaviour, CPPM(RexxString::x2b), 0);
  defineKernelMethod(CHAR_X2C                          ,TheStringBehaviour, CPPM(RexxString::x2c), 0);
  defineKernelMethod(CHAR_X2D                          ,TheStringBehaviour, CPPM(RexxString::x2d), 1);
  defineKernelMethod(CHAR_ENCODEBASE64                 ,TheStringBehaviour, CPPM(RexxString::encodeBase64), 0);
  defineKernelMethod(CHAR_DECODEBASE64                 ,TheStringBehaviour, CPPM(RexxString::decodeBase64), 0);
  defineKernelMethod(CHAR_LOWER                        ,TheStringBehaviour, CPPM(RexxString::lowerRexx), 0);
  defineKernelMethod(CHAR_UPPER                        ,TheStringBehaviour, CPPM(RexxString::upperRexx), 0);
  defineKernelMethod(CHAR_MAKESTRING                   ,TheStringBehaviour, CPPM(RexxObject::makeStringRexx), 0);
  defineKernelMethod(CHAR_ABS                          ,TheStringBehaviour, CPPM(RexxString::abs), 0);
  defineKernelMethod(CHAR_MAX                          ,TheStringBehaviour, CPPM(RexxString::Max), A_COUNT);
  defineKernelMethod(CHAR_MIN                          ,TheStringBehaviour, CPPM(RexxString::Min), A_COUNT);
  defineKernelMethod(CHAR_SIGN                         ,TheStringBehaviour, CPPM(RexxString::sign), 0);
  defineKernelMethod(CHAR_EQUAL                        ,TheStringBehaviour, CPPM(RexxString::equal), 1);
  defineKernelMethod(CHAR_BACKSLASH_EQUAL              ,TheStringBehaviour, CPPM(RexxString::notEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_GREATERTHAN         ,TheStringBehaviour, CPPM(RexxString::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN_LESSTHAN         ,TheStringBehaviour, CPPM(RexxString::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN                  ,TheStringBehaviour, CPPM(RexxString::isGreaterThan), 1);
  defineKernelMethod(CHAR_LESSTHAN                     ,TheStringBehaviour, CPPM(RexxString::isLessThan), 1);
  defineKernelMethod(CHAR_GREATERTHAN_EQUAL            ,TheStringBehaviour, CPPM(RexxString::isGreaterOrEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_LESSTHAN           ,TheStringBehaviour, CPPM(RexxString::isGreaterOrEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_EQUAL               ,TheStringBehaviour, CPPM(RexxString::isLessOrEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_GREATERTHAN        ,TheStringBehaviour, CPPM(RexxString::isLessOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_EQUAL                 ,TheStringBehaviour, CPPM(RexxString::strictEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_EQUAL       ,TheStringBehaviour, CPPM(RexxString::strictNotEqual), 1);
  defineKernelMethod(CHAR_STRICT_GREATERTHAN           ,TheStringBehaviour, CPPM(RexxString::strictGreaterThan), 1);
  defineKernelMethod(CHAR_STRICT_LESSTHAN              ,TheStringBehaviour, CPPM(RexxString::strictLessThan), 1);
  defineKernelMethod(CHAR_STRICT_GREATERTHAN_EQUAL     ,TheStringBehaviour, CPPM(RexxString::strictGreaterOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_LESSTHAN    ,TheStringBehaviour, CPPM(RexxString::strictGreaterOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_LESSTHAN_EQUAL        ,TheStringBehaviour, CPPM(RexxString::strictLessOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_GREATERTHAN ,TheStringBehaviour, CPPM(RexxString::strictLessOrEqual), 1);
  defineKernelMethod(CHAR_PLUS                         ,TheStringBehaviour, CPPM(RexxString::plus), 1);
  defineKernelMethod(CHAR_SUBTRACT                     ,TheStringBehaviour, CPPM(RexxString::minus), 1);
  defineKernelMethod(CHAR_MULTIPLY                     ,TheStringBehaviour, CPPM(RexxString::multiply), 1);
  defineKernelMethod(CHAR_POWER                        ,TheStringBehaviour, CPPM(RexxString::power), 1);
  defineKernelMethod(CHAR_DIVIDE                       ,TheStringBehaviour, CPPM(RexxString::divide), 1);
  defineKernelMethod(CHAR_INTDIV                       ,TheStringBehaviour, CPPM(RexxString::integerDivide), 1);
  defineKernelMethod(CHAR_REMAINDER                    ,TheStringBehaviour, CPPM(RexxString::remainder), 1);
  defineKernelMethod(CHAR_BACKSLASH                    ,TheStringBehaviour, CPPM(RexxString::notOp), 0);
  defineKernelMethod(CHAR_AND                          ,TheStringBehaviour, CPPM(RexxString::andOp), 1);
  defineKernelMethod(CHAR_OR                           ,TheStringBehaviour, CPPM(RexxString::orOp), 1);
  defineKernelMethod(CHAR_XOR                          ,TheStringBehaviour, CPPM(RexxString::xorOp), 1);
  defineKernelMethod(CHAR_MAKEARRAY                    ,TheStringBehaviour, CPPM(RexxString::makeArray), 1);
  defineKernelMethod(CHAR_EQUALS                       ,TheStringBehaviour, CPPM(RexxString::equals), 1);
  defineKernelMethod(CHAR_EQUALSCASELESS               ,TheStringBehaviour, CPPM(RexxString::equalsCaseless), 1);
                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheStringBehaviour->setMethodDictionaryScope(TheStringClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheStringClass->subClassable((stringchar_t *)"String");


  /***************************************************************************/
  /*           MUTABLEBUFFER                                                 */
  /***************************************************************************/

                                       /* Add the NEW method to the class   */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW, TheMutableBufferClassBehaviour, CPPM(RexxMutableBuffer::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheMutableBufferClassBehaviour->setMethodDictionaryScope(TheMutableBufferClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_APPEND                       ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::append), 1);
  defineKernelMethod(CHAR_INSERT                       ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::insert), 4);
  defineKernelMethod(CHAR_OVERLAY                      ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::overlay), 4);
  defineKernelMethod(CHAR_DELETE                       ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::mydelete), 2);
  defineKernelMethod(CHAR_SUBSTR                       ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::substr), 3);
  defineKernelMethod(CHAR_SUBCHAR                      ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::subchar), 1);
  defineKernelMethod(CHAR_POS                          ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::posRexx), 2);
  defineKernelMethod(CHAR_LASTPOS                      ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::lastPosRexx), 2);
  defineKernelMethod(CHAR_GETBUFFERSIZE                , TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::getBufferSize), 0);
  defineKernelMethod(CHAR_SETBUFFERSIZE                , TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::setBufferSize), 1);

  defineKernelMethod(CHAR_LENGTH                       ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::lengthRexx), 0);
  defineKernelMethod(CHAR_STRING                       ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::requestString), 0);
  defineKernelMethod(CHAR_MAKESTRING                   ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::makeString), 1);
  defineKernelMethod(CHAR_MAKEARRAY                    ,TheMutableBufferBehaviour, CPPM(RexxMutableBuffer::makeArray), 0);


  /***************************************************************************/
  /*           POINTER                                                       */
  /***************************************************************************/

                                       /* Add the NEW method to the class   */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW, ThePointerClassBehaviour, CPPM(RexxPointer::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  ThePointerClassBehaviour->setMethodDictionaryScope(ThePointerClass);


  // pointer doesn't have any additional instance methods

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  ThePointerBehaviour->setMethodDictionaryScope(ThePointerClass);
                                       /* Now call the class subclassable   */
                                       /* method                            */
  ThePointerClass->subClassable((stringchar_t *)"Pointer");

  /***************************************************************************/
  /*             INTEGER                                                     */
  /***************************************************************************/

    /* If the integer class was set up correctly it would have the           */
    /* class_id method in its own class but instead it points to the one     */
    /* in the string class.                                                 .*/

  defineKernelMethod(CHAR_NEW, TheIntegerClassBehaviour, CPPM(RexxClass::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheIntegerClassBehaviour->setMethodDictionaryScope(TheIntegerClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_PLUS                         ,TheIntegerBehaviour, CPPM(RexxInteger::plus), 1);
  defineKernelMethod(CHAR_SUBTRACT                     ,TheIntegerBehaviour, CPPM(RexxInteger::minus), 1);
  defineKernelMethod(CHAR_MULTIPLY                     ,TheIntegerBehaviour, CPPM(RexxInteger::multiply), 1);
  defineKernelMethod(CHAR_POWER                        ,TheIntegerBehaviour, CPPM(RexxInteger::power), 1);
  defineKernelMethod(CHAR_DIVIDE                       ,TheIntegerBehaviour, CPPM(RexxInteger::divide), 1);
  defineKernelMethod(CHAR_INTDIV                       ,TheIntegerBehaviour, CPPM(RexxInteger::integerDivide), 1);
  defineKernelMethod(CHAR_REMAINDER                    ,TheIntegerBehaviour, CPPM(RexxInteger::remainder), 1);
  defineKernelMethod(CHAR_BACKSLASH                    ,TheIntegerBehaviour, CPPM(RexxInteger::notOp), 0);
  defineKernelMethod(CHAR_AND                          ,TheIntegerBehaviour, CPPM(RexxInteger::andOp), 1);
  defineKernelMethod(CHAR_OR                           ,TheIntegerBehaviour, CPPM(RexxInteger::orOp), 1);
  defineKernelMethod(CHAR_XOR                          ,TheIntegerBehaviour, CPPM(RexxInteger::xorOp), 1);
  defineKernelMethod(CHAR_UNKNOWN                      ,TheIntegerBehaviour, CPPM(RexxObject::unknownRexx), 2);
  defineKernelMethod(CHAR_D2C                          ,TheIntegerBehaviour, CPPM(RexxInteger::d2c), 1);
  defineKernelMethod(CHAR_D2X                          ,TheIntegerBehaviour, CPPM(RexxInteger::d2x), 1);
  defineKernelMethod(CHAR_ABS                          ,TheIntegerBehaviour, CPPM(RexxInteger::abs), 0);
  defineKernelMethod(CHAR_MAX                          ,TheIntegerBehaviour, CPPM(RexxInteger::Max), A_COUNT);
  defineKernelMethod(CHAR_MIN                          ,TheIntegerBehaviour, CPPM(RexxInteger::Min), A_COUNT);
  defineKernelMethod(CHAR_SIGN                         ,TheIntegerBehaviour, CPPM(RexxInteger::sign), 0);
  defineKernelMethod(CHAR_EQUAL                        ,TheIntegerBehaviour, CPPM(RexxInteger::equal), 1);
  defineKernelMethod(CHAR_BACKSLASH_EQUAL              ,TheIntegerBehaviour, CPPM(RexxInteger::notEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_GREATERTHAN         ,TheIntegerBehaviour, CPPM(RexxInteger::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN_LESSTHAN         ,TheIntegerBehaviour, CPPM(RexxInteger::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN                  ,TheIntegerBehaviour, CPPM(RexxInteger::isGreaterThan), 1);
  defineKernelMethod(CHAR_LESSTHAN                     ,TheIntegerBehaviour, CPPM(RexxInteger::isLessThan), 1);
  defineKernelMethod(CHAR_GREATERTHAN_EQUAL            ,TheIntegerBehaviour, CPPM(RexxInteger::isGreaterOrEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_LESSTHAN           ,TheIntegerBehaviour, CPPM(RexxInteger::isGreaterOrEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_EQUAL               ,TheIntegerBehaviour, CPPM(RexxInteger::isLessOrEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_GREATERTHAN        ,TheIntegerBehaviour, CPPM(RexxInteger::isLessOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_EQUAL                 ,TheIntegerBehaviour, CPPM(RexxInteger::strictEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_EQUAL       ,TheIntegerBehaviour, CPPM(RexxInteger::strictNotEqual), 1);
  defineKernelMethod(CHAR_STRICT_GREATERTHAN           ,TheIntegerBehaviour, CPPM(RexxInteger::strictGreaterThan), 1);
  defineKernelMethod(CHAR_STRICT_LESSTHAN              ,TheIntegerBehaviour, CPPM(RexxInteger::strictLessThan), 1);
  defineKernelMethod(CHAR_STRICT_GREATERTHAN_EQUAL     ,TheIntegerBehaviour, CPPM(RexxInteger::strictGreaterOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_LESSTHAN    ,TheIntegerBehaviour, CPPM(RexxInteger::strictGreaterOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_LESSTHAN_EQUAL        ,TheIntegerBehaviour, CPPM(RexxInteger::strictLessOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_GREATERTHAN ,TheIntegerBehaviour, CPPM(RexxInteger::strictLessOrEqual), 1);
  defineKernelMethod(CHAR_MAKESTRING                   ,TheIntegerBehaviour, CPPM(RexxObject::makeStringRexx), 0);
  defineKernelMethod(CHAR_FORMAT                       ,TheIntegerBehaviour, CPPM(RexxInteger::format), 4);
  defineKernelMethod(CHAR_TRUNC                        ,TheIntegerBehaviour, CPPM(RexxInteger::trunc), 1);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheIntegerBehaviour->setMethodDictionaryScope(TheIntegerClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheIntegerClass->subClassable((stringchar_t *)"String");

  /***************************************************************************/
  /*             NUMBERSTRING                                                */
  /***************************************************************************/

     /* If the numberstring class was set up correctly it should have the    */
     /* class_id method in its own class but instead it points to the one    */
     /* in the string class.                                                 */

  defineKernelMethod(CHAR_NEW, TheNumberStringClassBehaviour, CPPM(RexxClass::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheNumberStringClassBehaviour->setMethodDictionaryScope(TheNumberStringClass);

                                       /* Add the instance methods to this  */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_UNKNOWN                      ,TheNumberStringBehaviour, CPPM(RexxObject::unknownRexx), 2);
  defineKernelMethod(CHAR_ABS                          ,TheNumberStringBehaviour, CPPM(RexxNumberString::abs), 1);
  defineKernelMethod(CHAR_MAX                          ,TheNumberStringBehaviour, CPPM(RexxNumberString::Max), A_COUNT);
  defineKernelMethod(CHAR_MIN                          ,TheNumberStringBehaviour, CPPM(RexxNumberString::Min), A_COUNT);
  defineKernelMethod(CHAR_SIGN                         ,TheNumberStringBehaviour, CPPM(RexxNumberString::Sign), 0);
  defineKernelMethod(CHAR_D2C                          ,TheNumberStringBehaviour, CPPM(RexxNumberString::d2c), 1);
  defineKernelMethod(CHAR_D2X                          ,TheNumberStringBehaviour, CPPM(RexxNumberString::d2x), 1);
  defineKernelMethod(CHAR_EQUAL                        ,TheNumberStringBehaviour, CPPM(RexxNumberString::equal), 1);
  defineKernelMethod(CHAR_BACKSLASH_EQUAL              ,TheNumberStringBehaviour, CPPM(RexxNumberString::notEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_GREATERTHAN         ,TheNumberStringBehaviour, CPPM(RexxNumberString::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN_LESSTHAN         ,TheNumberStringBehaviour, CPPM(RexxNumberString::notEqual), 1);
  defineKernelMethod(CHAR_GREATERTHAN                  ,TheNumberStringBehaviour, CPPM(RexxNumberString::isGreaterThan), 1);
  defineKernelMethod(CHAR_LESSTHAN                     ,TheNumberStringBehaviour, CPPM(RexxNumberString::isLessThan), 1);
  defineKernelMethod(CHAR_GREATERTHAN_EQUAL            ,TheNumberStringBehaviour, CPPM(RexxNumberString::isGreaterOrEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_LESSTHAN           ,TheNumberStringBehaviour, CPPM(RexxNumberString::isGreaterOrEqual), 1);
  defineKernelMethod(CHAR_LESSTHAN_EQUAL               ,TheNumberStringBehaviour, CPPM(RexxNumberString::isLessOrEqual), 1);
  defineKernelMethod(CHAR_BACKSLASH_GREATERTHAN        ,TheNumberStringBehaviour, CPPM(RexxNumberString::isLessOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_EQUAL                 ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_EQUAL       ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictNotEqual), 1);
  defineKernelMethod(CHAR_STRICT_GREATERTHAN           ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictGreaterThan), 1);
  defineKernelMethod(CHAR_STRICT_LESSTHAN              ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictLessThan), 1);
  defineKernelMethod(CHAR_STRICT_GREATERTHAN_EQUAL     ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictGreaterOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_LESSTHAN    ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictGreaterOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_LESSTHAN_EQUAL        ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictLessOrEqual), 1);
  defineKernelMethod(CHAR_STRICT_BACKSLASH_GREATERTHAN ,TheNumberStringBehaviour, CPPM(RexxNumberString::strictLessOrEqual), 1);
  defineKernelMethod(CHAR_PLUS                         ,TheNumberStringBehaviour, CPPM(RexxNumberString::plus), 1);
  defineKernelMethod(CHAR_SUBTRACT                     ,TheNumberStringBehaviour, CPPM(RexxNumberString::minus), 1);
  defineKernelMethod(CHAR_MULTIPLY                     ,TheNumberStringBehaviour, CPPM(RexxNumberString::multiply), 1);
  defineKernelMethod(CHAR_POWER                        ,TheNumberStringBehaviour, CPPM(RexxNumberString::power), 1);
  defineKernelMethod(CHAR_DIVIDE                       ,TheNumberStringBehaviour, CPPM(RexxNumberString::divide), 1);
  defineKernelMethod(CHAR_INTDIV                       ,TheNumberStringBehaviour, CPPM(RexxNumberString::integerDivide), 1);
  defineKernelMethod(CHAR_REMAINDER                    ,TheNumberStringBehaviour, CPPM(RexxNumberString::remainder), 1);
  defineKernelMethod(CHAR_BACKSLASH                    ,TheNumberStringBehaviour, CPPM(RexxNumberString::notOp), 0);
  defineKernelMethod(CHAR_AND                          ,TheNumberStringBehaviour, CPPM(RexxNumberString::andOp), 1);
  defineKernelMethod(CHAR_OR                           ,TheNumberStringBehaviour, CPPM(RexxNumberString::orOp), 1);
  defineKernelMethod(CHAR_XOR                          ,TheNumberStringBehaviour, CPPM(RexxNumberString::xorOp), 1);
  defineKernelMethod(CHAR_MAKESTRING                   ,TheNumberStringBehaviour, CPPM(RexxObject::makeStringRexx), 0);
  defineKernelMethod(CHAR_FORMAT                       ,TheNumberStringBehaviour, CPPM(RexxNumberString::formatRexx), 4);
  defineKernelMethod(CHAR_TRUNC                        ,TheNumberStringBehaviour, CPPM(RexxNumberString::trunc), 1);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheNumberStringBehaviour->setMethodDictionaryScope(TheNumberStringClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheNumberStringClass->subClassable((stringchar_t *)"String");

  /***************************************************************************/
  /*           SUPPLIER                                                      */
  /***************************************************************************/
                                       /* Add the NEW methods to the class  */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW, TheSupplierClassBehaviour, CPPM(RexxSupplierClass::newRexx), A_COUNT);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */

  defineKernelMethod(CHAR_AVAILABLE ,TheSupplierBehaviour, CPPM(RexxSupplier::available), 0);
  defineKernelMethod(CHAR_INDEX     ,TheSupplierBehaviour, CPPM(RexxSupplier::index), 0);
  defineKernelMethod(CHAR_NEXT      ,TheSupplierBehaviour, CPPM(RexxSupplier::next), 0);
  defineKernelMethod(CHAR_ITEM      ,TheSupplierBehaviour, CPPM(RexxSupplier::value), 0);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheSupplierBehaviour->setMethodDictionaryScope(TheSupplierClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheSupplierClass->subClassable((stringchar_t *)"Supplier");

  /***************************************************************************/
  /*           TABLE                                                         */
  /***************************************************************************/

                                       /* Add the NEW methods to the class  */
                                       /* behaviour mdict                   */
  defineKernelMethod(CHAR_NEW          , TheTableClassBehaviour, CPPM(RexxTable::newRexx), A_COUNT);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheTableClassBehaviour->setMethodDictionaryScope(TheTableClass);

                                       /* Add the instance methods to the   */
                                       /* instance behaviour mdict          */
  defineKernelMethod(CHAR_BRACKETS     , TheTableBehaviour, CPPM(RexxHashTableCollection::getRexx), 1);
  defineKernelMethod(CHAR_BRACKETSEQUAL, TheTableBehaviour, CPPM(RexxHashTableCollection::put), 2);
  defineKernelMethod(CHAR_MAKEARRAY    , TheTableBehaviour, CPPM(RexxObject::makeArrayRexx), 0);
  defineKernelMethod(CHAR_ALLITEMS     , TheTableBehaviour, CPPM(RexxHashTableCollection::allItems), 0);
  defineKernelMethod(CHAR_ALLINDEXES   , TheTableBehaviour, CPPM(RexxHashTableCollection::allIndexes), 0);
  defineKernelMethod(CHAR_AT           , TheTableBehaviour, CPPM(RexxHashTableCollection::getRexx), 1);
  defineKernelMethod(CHAR_HASINDEX     , TheTableBehaviour, CPPM(RexxHashTableCollection::hasIndex), 1);
  defineKernelMethod(CHAR_ITEMS        , TheTableBehaviour, CPPM(RexxTable::itemsRexx), 0);
  defineKernelMethod(CHAR_PUT          , TheTableBehaviour, CPPM(RexxHashTableCollection::put), 2);
  defineKernelMethod(CHAR_REMOVE       , TheTableBehaviour, CPPM(RexxHashTableCollection::removeRexx), 1);
  defineKernelMethod(CHAR_SUPPLIER     , TheTableBehaviour, CPPM(RexxHashTableCollection::supplier), 0);

                                       /* set the scope of the methods to   */
                                       /* this classes oref                 */
  TheTableBehaviour->setMethodDictionaryScope(TheTableClass);

                                       /* Now call the class subclassable   */
                                       /* method                            */
  TheTableClass->subClassable((stringchar_t *)"Table");

  /***************************************************************************/
  /***************************************************************************/
  /***************************************************************************/
    /* These classes don't have any class methods                            */
    /*  and are not subclassed from object                                   */

  /* put the kernel-provided public objects in the environment directory */
  addPublicObject(CHAR_ARRAY            ,TheArrayClass   ,TheEnvironment);
  addPublicObject(CHAR_CLASS            ,TheClassClass   ,TheEnvironment);
  addPublicObject(CHAR_DIRECTORY        ,TheDirectoryClass ,TheEnvironment);
  addPublicObject(CHAR_ENVIRONMENT      ,TheEnvironment  ,TheEnvironment);
  addPublicObject(CHAR_FALSE            ,TheFalseObject  ,TheEnvironment);
  addPublicObject(CHAR_KERNEL           ,TheKernel       ,TheEnvironment);
  addPublicObject(CHAR_LIST             ,TheListClass    ,TheEnvironment);
  addPublicObject(CHAR_MESSAGE          ,TheMessageClass ,TheEnvironment);
  addPublicObject(CHAR_METHOD           ,TheMethodClass  ,TheEnvironment);
  addPublicObject(CHAR_NIL              ,TheNilObject    ,TheEnvironment);
  addPublicObject(CHAR_OBJECT           ,TheObjectClass  ,TheEnvironment);
  addPublicObject(CHAR_QUEUE            ,TheQueueClass   ,TheEnvironment);
  addPublicObject(CHAR_RELATION         ,TheRelationClass,TheEnvironment);
  addPublicObject(CHAR_STRING           ,TheStringClass  ,TheEnvironment);
  addPublicObject(CHAR_MUTABLEBUFFER    ,TheMutableBufferClass  ,TheEnvironment);
  addPublicObject(CHAR_STEM             ,TheStemClass    ,TheEnvironment);
  addPublicObject(CHAR_SUPPLIER         ,TheSupplierClass,TheEnvironment);

  // add a system subdirectory...this hold system information.
  RexxDirectory *theSystem = new_directory();
  addPublicObject(CHAR_SYSTEM           ,theSystem       ,TheEnvironment);
  // mark this as proxied too
  RexxMemory::memoryObject.addProxiedObject(theSystem, OREF_SYSTEM);
  // also proxy the nil object
  RexxMemory::memoryObject.addProxiedObject(TheNilObject, OREF_NIL);

  theSystem->header.makeProxiedObject();

  addPublicObject(CHAR_TABLE            ,TheTableClass   ,TheEnvironment);
  addPublicObject(CHAR_TRUE             ,TheTrueObject   ,TheEnvironment);

  addPublicObject(CHAR_PUBLIC_ROUTINES  ,publicRoutines,    TheEnvironment);
  addPublicObject(CHAR_STATIC_REQUIRES  ,staticRequires,    TheEnvironment);

  // this anchors many non-public objects
  addPublicObject(CHAR_INTEGER          ,TheIntegerClass     , TheKernel);

  /* make the memory and environment objects public */
  addPublicObject(CHAR_MEMORY ,&RexxMemory::memoryObject, TheKernel);

                                       /* set Oryx version                  */
  addPublicObject(CHAR_VERSION, versionString(), TheKernel);
                                       /* set the system name               */
  addPublicObject(CHAR_NAME, SysInterpreter::getSystemName(), theSystem);
                                       /* and the system version info       */
  addPublicObject(CHAR_VERSION, SysInterpreter::getVersion(), theSystem);

/******************************************************************************/
/*      Complete the image build process, calling BaseClasses to establish    */
/*      the rest of the REXX image.                                           */
/******************************************************************************/

  /* set up the kernel methods that will be defined on OBJECT classes in  */
  /*  BaseClasses.ORX and ORYXJ.ORX.                                            */
                                       /* create a kernel methods directory */
  kernel_methods = (RexxDirectory *)new_directory();
  ProtectedObject p1(kernel_methods);
  kernel_methods->put(RexxMethod::createKernelMethod(CPPM(RexxInterpreter::localEnvironment), 0), addGlobalName(CHAR_LOCAL));

                                       /* create the BaseClasses method and run it*/
  symb = addGlobalName((stringchar_t *)CORECLASSES);    /* get a name version of the string  */
                                       /* go resolve the program name       */
  programName = SysInterpreter::resolveProgramName(symb, OREF_NULL);
  try
  {
                                           /* create a method object out of this*/
      meth = RexxMethod::newFile(programName);
      ProtectedObject result;
                                           /* now call BaseClasses to finish the image*/
      RexxActivityManager::currentActivity->runProgram(meth, OREF_SCRIPT, OREF_SCRIPT, OREF_NULL, (RexxObject **)&kernel_methods, 1, result);
                                           /* remove kernel from .environment   */
      TheEnvironment->remove(addGlobalName(CHAR_KERNEL));

      /* define and suppress methods in the nil object */
      TheNilObject->defMethod(addGlobalName(CHAR_COPY), (RexxMethod *)TheNilObject);
      TheNilObject->defMethod(addGlobalName(CHAR_START), (RexxMethod *)TheNilObject);
      TheNilObject->defMethod(addGlobalName(CHAR_OBJECTNAMEEQUALS), (RexxMethod *)TheNilObject);
      return true;
  }
  catch (ActivityException)
  {
      RexxActivityManager::currentActivity->error();         /* do error cleanup                  */
      return false;
  }
}

