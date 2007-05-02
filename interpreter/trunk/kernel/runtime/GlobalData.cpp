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
/* REXX Kernel                                                                */
/*                                                                            */
/* Global Data                                                                */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "TableClass.hpp"
#include "IntegerClass.hpp"
#include "ArrayClass.hpp"
#include "NumberStringClass.hpp"
#include "ListClass.hpp"
#include "MessageClass.hpp"
#include "PointerClass.hpp"
#include "MutableBufferClass.hpp"
#include "RelationClass.hpp"
#include "QueueClass.hpp"
#include "SupplierClass.hpp"
#include "Interpreter.hpp"
#include "RexxNativeCode.hpp"


PCPPM RexxMethod::exportedMethods[] = {            /* start of exported methods table   */
                                       /* NOTE:  getAttribute and           */
                                       /* setAttribute need to be the first */
                                       /* entries in the table, since these */
                                       /* need to be created dynamically, so*/
                                       /* they need to have a fixed,        */
                                       /* predictable location              */
CPPM(RexxObject::getAttribute),
CPPM(RexxObject::setAttribute),
CPPM(RexxObject::abstractMethod),      // also must be at this location ALWAYS

// end of the section where the indices must be fixed.
CPPM(RexxObject::objectName),
CPPM(RexxObject::objectNameEquals),
CPPM(RexxObject::run),
CPPM(RexxObject::stringRexx),
CPPM(RexxObject::notEqual),
CPPM(RexxObject::setMethod),
CPPM(RexxObject::hasMethodRexx),
CPPM(RexxObject::start),
CPPM(RexxObject::unsetMethod),
CPPM(RexxObject::requestRexx),
CPPM(RexxObject::makeStringRexx),
CPPM(RexxObject::makeArrayRexx),
CPPM(RexxInternalObject::hasUninit),
CPPM(RexxObject::classObject),
CPPM(RexxObject::equal),
CPPM(RexxObject::strictEqual),
CPPM(RexxObject::init),
CPPM(RexxObject::strictNotEqual),
CPPM(RexxObject::copyRexx),
CPPM(RexxObject::defaultNameRexx),
CPPM(RexxObject::unknownRexx),
CPPM(RexxObject::isInstanceOfRexx),
CPPM(RexxObject::instanceMethodRexx),
CPPM(RexxObject::instanceMethodsRexx),

CPPM(RexxObject::newRexx),

CPPM(RexxClass::setRexxDefined),       /* Class methods                     */
CPPM(RexxClass::queryMixinClass),
CPPM(RexxClass::getId),
CPPM(RexxClass::getBaseClass),
CPPM(RexxClass::getMetaClass),
CPPM(RexxClass::getSuperClasses),
CPPM(RexxClass::getSubClasses),
CPPM(RexxClass::defmeths),
CPPM(RexxClass::defineMethod),
CPPM(RexxClass::defineMethods),
CPPM(RexxClass::deleteMethod),
CPPM(RexxClass::method),
CPPM(RexxClass::methods),
CPPM(RexxClass::inherit),
CPPM(RexxClass::uninherit),
CPPM(RexxClass::enhanced),
CPPM(RexxClass::mixinclass),
CPPM(RexxClass::subclass),
CPPM(RexxClass::equal),
CPPM(RexxClass::strictEqual),
CPPM(RexxClass::notEqual),

CPPM(RexxClass::newRexx),

CPPM(RexxArray::sizeRexx),             /* Array methods                     */
CPPM(RexxArray::items),
CPPM(RexxArray::dimension),
CPPM(RexxArray::supplier),
CPPM(RexxArray::getRexx),
CPPM(RexxArray::putRexx),
CPPM(RexxArray::hasIndexRexx),
CPPM(RexxArray::sectionRexx),
CPPM(RexxArray::removeRexx),
CPPM(RexxArray::firstRexx),
CPPM(RexxArray::lastRexx),
CPPM(RexxArray::nextRexx),
CPPM(RexxArray::allItems),
CPPM(RexxArray::allIndexes),
CPPM(RexxArray::previousRexx),
CPPM(RexxArray::append),
CPPM(RexxArray::empty),
CPPM(RexxArray::isEmpty),
CPPM(RexxArray::index),
CPPM(RexxArray::hasItem),
CPPM(RexxArray::toString),

CPPM(RexxArray::newRexx),
CPPM(RexxArray::makeString),
CPPM(RexxArray::of),

CPPM(RexxDirectory::atRexx),           /* Directory methods                 */
CPPM(RexxDirectory::put),
CPPM(RexxDirectory::entryRexx),
CPPM(RexxDirectory::hasEntry),
CPPM(RexxDirectory::hasIndex),
CPPM(RexxDirectory::itemsRexx),
CPPM(RexxHashTableCollection::merge),
CPPM(RexxDirectory::remove),
CPPM(RexxDirectory::setEntry),
CPPM(RexxDirectory::setMethod),
CPPM(RexxDirectory::supplier),
CPPM(RexxDirectory::allItems),
CPPM(RexxDirectory::allIndexes),
CPPM(RexxDirectory::empty),
CPPM(RexxDirectory::isEmpty),
CPPM(RexxDirectory::indexRexx),
CPPM(RexxDirectory::hasItem),

CPPM(RexxDirectory::newRexx),

CPPM(RexxInteger::plus),               /* Integer methods                   */
CPPM(RexxInteger::minus),
CPPM(RexxInteger::multiply),
CPPM(RexxInteger::divide),
CPPM(RexxInteger::integerDivide),
CPPM(RexxInteger::remainder),
CPPM(RexxInteger::power),
CPPM(RexxInteger::notOp),
CPPM(RexxInteger::andOp),
CPPM(RexxInteger::orOp),
CPPM(RexxInteger::xorOp),
CPPM(RexxInteger::equal),
CPPM(RexxInteger::notEqual),
CPPM(RexxInteger::strictEqual),
CPPM(RexxInteger::strictNotEqual),
CPPM(RexxInteger::isGreaterThan),
CPPM(RexxInteger::isLessThan),
CPPM(RexxInteger::isGreaterOrEqual),
CPPM(RexxInteger::isLessOrEqual),
CPPM(RexxInteger::strictGreaterThan),
CPPM(RexxInteger::strictLessThan),
CPPM(RexxInteger::strictGreaterOrEqual),
CPPM(RexxInteger::strictLessOrEqual),
CPPM(RexxInteger::abs),
CPPM(RexxInteger::sign),
CPPM(RexxInteger::Max),
CPPM(RexxInteger::Min),
CPPM(RexxInteger::d2x),
CPPM(RexxInteger::d2c),
CPPM(RexxInteger::format),
CPPM(RexxInteger::trunc),

CPPM(RexxList::value),                 /* list methods                      */
CPPM(RexxList::remove),
CPPM(RexxList::firstRexx),
CPPM(RexxList::lastRexx),
CPPM(RexxList::next),
CPPM(RexxList::previous),
CPPM(RexxList::hasIndex),
CPPM(RexxList::supplier),
CPPM(RexxList::itemsRexx),
CPPM(RexxList::put),
CPPM(RexxList::section),
CPPM(RexxList::firstItem),
CPPM(RexxList::lastItem),
CPPM(RexxList::insertRexx),
CPPM(RexxList::allItems),
CPPM(RexxList::allIndexes),
CPPM(RexxList::append),
CPPM(RexxList::empty),
CPPM(RexxList::isEmpty),
CPPM(RexxList::index),
CPPM(RexxList::hasItem),

CPPM(RexxList::newRexx),
CPPM(RexxList::classOf),

CPPM(RexxMessage::notify),             /* Message methods                   */
CPPM(RexxMessage::result),
CPPM(RexxMessage::send),
CPPM(RexxMessage::start),
CPPM(RexxMessage::completed),
CPPM(RexxMessage::hasError),
CPPM(RexxMessage::errorCondition),
CPPMSG(RexxMessage::messageTarget),
CPPMSG(RexxMessage::messageName),
CPPMSG(RexxMessage::arguments),

CPPM(RexxMessage::newRexx),

CPPM(RexxMethod::setUnGuardedRexx),    /* Method methods                    */
CPPM(RexxMethod::setGuardedRexx),
CPPM(RexxMethod::sourceRexx),
CPPM(RexxMethod::setPrivateRexx),
CPPM(RexxMethod::setProtectedRexx),
CPPM(RexxMethod::setSecurityManager),

CPPM(RexxMethod::newFileRexx),
CPPM(RexxMethod::newRexx),

CPPM(RexxNumberString::formatRexx),    /* NumberString methods              */
CPPM(RexxNumberString::trunc),
CPPM(RexxNumberString::equal),
CPPM(RexxNumberString::notEqual),
CPPM(RexxNumberString::isLessThan),
CPPM(RexxNumberString::isGreaterThan),
CPPM(RexxNumberString::isGreaterOrEqual),
CPPM(RexxNumberString::isLessOrEqual),
CPPM(RexxNumberString::strictNotEqual),
CPPM(RexxNumberString::strictLessThan),
CPPM(RexxNumberString::strictGreaterThan),
CPPM(RexxNumberString::strictGreaterOrEqual),
CPPM(RexxNumberString::strictLessOrEqual),
CPPM(RexxNumberString::plus),
CPPM(RexxNumberString::minus),
CPPM(RexxNumberString::multiply),
CPPM(RexxNumberString::divide),
CPPM(RexxNumberString::integerDivide),
CPPM(RexxNumberString::remainder),
CPPM(RexxNumberString::power),
CPPM(RexxNumberString::abs),
CPPM(RexxNumberString::Sign),
CPPM(RexxNumberString::notOp),
CPPM(RexxNumberString::andOp),
CPPM(RexxNumberString::orOp),
CPPM(RexxNumberString::xorOp),
CPPM(RexxNumberString::Max),
CPPM(RexxNumberString::Min),
CPPM(RexxNumberString::isInteger),
CPPM(RexxNumberString::d2c),
CPPM(RexxNumberString::d2x),
CPPM(RexxNumberString::d2xD2c),
CPPM(RexxNumberString::strictEqual),

CPPM(RexxQueue::supplier),             /* Queue methods                     */
CPPM(RexxQueue::pushRexx),
CPPM(RexxQueue::queueRexx),
CPPM(RexxQueue::pullRexx),
CPPM(RexxQueue::peek),
CPPM(RexxQueue::put),
CPPM(RexxQueue::at),
CPPM(RexxQueue::hasindex),
CPPM(RexxQueue::remove),
CPPM(RexxQueue::allIndexes),
CPPM(RexxQueue::append),
CPPM(RexxQueue::ofRexx),
CPPM(RexxQueue::index),

CPPM(RexxQueue::newRexx),

CPPM(RexxStem::bracket),               /* Stem methods                      */
CPPM(RexxStem::bracketEqual),
CPPM(RexxStem::request),
CPPM(RexxStem::supplier),
CPPM(RexxStem::allItems),
CPPM(RexxStem::allIndexes),
CPPM(RexxStem::empty),
CPPM(RexxStem::isEmpty),

CPPM(RexxStem::newRexx),

CPPM(RexxString::lengthRexx),          /* String methods                    */
CPPM(RexxString::concatRexx),
CPPM(RexxString::concatBlank),
CPPM(RexxString::concatWith),
CPPM(RexxString::equal),
CPPM(RexxString::notEqual),
CPPM(RexxString::isLessThan),
CPPM(RexxString::isGreaterThan),
CPPM(RexxString::isGreaterOrEqual),
CPPM(RexxString::isLessOrEqual),
CPPM(RexxString::strictEqual),
CPPM(RexxString::strictNotEqual),
CPPM(RexxString::strictLessThan),
CPPM(RexxString::strictGreaterThan),
CPPM(RexxString::strictGreaterOrEqual),
CPPM(RexxString::strictLessOrEqual),
CPPM(RexxString::plus),
CPPM(RexxString::minus),
CPPM(RexxString::multiply),
CPPM(RexxString::divide),
CPPM(RexxString::integerDivide),
CPPM(RexxString::remainder),
CPPM(RexxString::power),
CPPM(RexxString::abs),
CPPM(RexxString::sign),
CPPM(RexxString::notOp),
CPPM(RexxString::andOp),
CPPM(RexxString::orOp),
CPPM(RexxString::xorOp),
CPPM(RexxString::Max),
CPPM(RexxString::Min),
CPPM(RexxString::isInteger),
CPPM(RexxString::equals),
CPPM(RexxString::equalsCaseless),
CPPM(RexxString::match),
CPPM(RexxString::caselessMatch),
CPPM(RexxString::matchChar),
CPPM(RexxString::caselessMatchChar),
CPPM(RexxString::caselessPos),
CPPM(RexxString::caselessLastPos),

                                          /* All BIF methods start here.  They */
                                          /*  will be arranged according to the*/
                                          /*  they are defined in.             */
CPPM(RexxString::center),
CPPM(RexxString::delstr),
CPPM(RexxString::insert),
CPPM(RexxString::left),
CPPM(RexxString::overlay),
CPPM(RexxString::reverse),
CPPM(RexxString::right),
CPPM(RexxString::strip),
CPPM(RexxString::substr),
CPPM(RexxString::subchar),

CPPM(RexxString::delWord),
CPPM(RexxString::space),
CPPM(RexxString::subWord),
CPPM(RexxString::extractWords),
CPPM(RexxString::word),
CPPM(RexxString::wordIndex),
CPPM(RexxString::wordLength),
CPPM(RexxString::wordPos),
CPPM(RexxString::words),

CPPM(RexxString::changeStr),
CPPM(RexxString::countStrRexx),
CPPM(RexxString::abbrev),
CPPM(RexxString::compare),
CPPM(RexxString::copies),
CPPM(RexxString::dataType),
CPPM(RexxString::lastPosRexx),
CPPM(RexxString::posRexx),
CPPM(RexxString::translate),
CPPM(RexxString::verify),

CPPM(RexxString::bitAnd),
CPPM(RexxString::bitOr),
CPPM(RexxString::bitXor),

CPPM(RexxString::b2x),
CPPM(RexxString::c2d),
CPPM(RexxString::c2x),
CPPM(RexxString::d2c),
CPPM(RexxString::d2x),
CPPM(RexxString::x2b),
CPPM(RexxString::x2c),
CPPM(RexxString::x2d),
CPPM(RexxString::encodeBase64),
CPPM(RexxString::decodeBase64),
CPPM(RexxString::upperRexx),
CPPM(RexxString::lowerRexx),
CPPM(RexxString::format),
CPPM(RexxString::trunc),
CPPM(RexxString::x2dC2d),
                                       /* End of BIF methods                */
CPPM(RexxString::makeArray),

CPPM(RexxString::newRexx),

CPPM(RexxMutableBuffer::newRexx),
CPPM(RexxMutableBuffer::lengthRexx),
CPPM(RexxMutableBuffer::requestString),
CPPM(RexxMutableBuffer::requestRexx),
CPPM(RexxMutableBuffer::append),
CPPM(RexxMutableBuffer::insert),
CPPM(RexxMutableBuffer::overlay),
CPPM(RexxMutableBuffer::mydelete),
CPPM(RexxMutableBuffer::substr),
CPPM(RexxMutableBuffer::subchar),
CPPM(RexxMutableBuffer::posRexx),
CPPM(RexxMutableBuffer::lastposRexx),
CPPM(RexxMutableBuffer::getBufferSize),
CPPM(RexxMutableBuffer::setBufferSize),


// the only method we expose for pointer is the new on the class object
CPPM(RexxPointer::newRexx),

CPPM(RexxSupplier::available),         /* Supplier methods                  */
CPPM(RexxSupplier::next),
CPPM(RexxSupplier::value),
CPPM(RexxSupplier::index),

CPPM(RexxSupplierClass::newRexx),

                                       /* Table methods                     */
CPPM(RexxHashTableCollection::removeRexx),
CPPM(RexxHashTableCollection::getRexx),
CPPM(RexxHashTableCollection::put),
CPPM(RexxHashTableCollection::add),
CPPM(RexxHashTableCollection::allAt),
CPPM(RexxHashTableCollection::allItems),
CPPM(RexxHashTableCollection::hasIndex),
CPPM(RexxHashTableCollection::merge),
CPPM(RexxHashTableCollection::supplier),
CPPM(RexxHashTableCollection::allItems),
CPPM(RexxHashTableCollection::allIndexes),
CPPM(RexxHashTableCollection::empty),
CPPM(RexxHashTableCollection::isEmpty),
CPPM(RexxHashTableCollection::indexRexx),
CPPM(RexxHashTableCollection::hasItem),

CPPM(RexxTable::itemsRexx),
CPPM(RexxTable::newRexx),

CPPM(RexxRelation::put),               /* Relation methods                  */
CPPM(RexxRelation::removeItemRexx),
CPPM(RexxRelation::allIndex),
CPPM(RexxRelation::itemsRexx),
CPPM(RexxRelation::supplier),

CPPM(RexxRelation::newRexx),

CPPM(RexxInterpreter::localEnvironment),  /* the .local environment methods    */

NULL                                   /* final terminating method          */
};

