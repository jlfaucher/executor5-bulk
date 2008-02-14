/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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
/* REXX Kernel                                             PackageClass.cpp   */
/*                                                                            */
/* Primitive Package class                                                    */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "ArrayClass.hpp"
#include "SourceFile.hpp"
#include "DirectoryClass.hpp"
#include "ProtectedObject.hpp"


// singleton class instance
RexxClass *PackageClass::classInstance = OREF_NULL;


PackageClass::PackageClass(RexxSource *s)
/******************************************************************************/
/* Function:  Initialize a method object                                      */
/******************************************************************************/
{
    this->clearObject();                 /* start out fresh                   */
    OrefSet(this, this->source, s);      /* store the code                    */
}


void PackageClass::live(size_t liveMark)
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
    memory_mark(this->source);
}

void PackageClass::liveGeneral(int reason)
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
    memory_mark_general(this->source);
}

void PackageClass::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(PackageClass)

   flatten_reference(newThis->source, envelope);

  cleanUpFlatten
}


/**
 * Get the program name of the package
 *
 * @return The name of the original source package.
 */
RexxString *PackageClass::getName()
{
    return source->getProgramName();
}


/**
 * Return all of the source lines for the package, as an array.
 *
 * @return The entire array of source lines.
 */
RexxArray *PackageClass::getSource()
{
    return source->extraceSource();
}


/**
 * Extract a specific line from the program source.
 *
 * @param n      The line position.
 *
 * @return The extracted line.
 */
RexxString *PackageClass::getSourceLine(size_t n)
{
    return source->get(n);
}


/**
 * Extract a specific line from the program source.
 *
 * @param n      The line position.
 *
 * @return The extracted line.
 */
RexxString *PackageClass::getSourceLineRexx(RexxObject *position)
{
    // the starting position isn't optional
    size_t b = get_position(position, ARG_ONE);
    return source->get(n);
}


RexxDirectory *PackageClass::getClasses()
{
    return (RexxDirectory *)source->getInstalledClasses()->copy();
}


RexxDirectory *PackageClass::getPublicClasses()
{
    return (RexxDirectory *)source->getInstalledPublicClasses()->copy();
}


RexxDirectory *PackageClass::getImportedClasses()
{
    return (RexxDirectory *)source->getImportedClasses()->copy();
}


RexxDirectory *PackageClass::getRoutines()
{
    return (RexxDirectory *)source->getInstalledRoutines()->copy();
}


RexxDirectory *PackageClass::getPublicRoutines()
{
    return (RexxDirectory *)source->getInstalledPublicRoutines()->copy();
}


RexxDirectory *PackageClass::getImportedRoutines()
{
    return (RexxDirectory *)source->getImportedRoutines()->copy();
}


RexxDirectory *PackageClass::getMethods()
{
    return (RexxDirectory *)source->getMethods()->copy();
}
