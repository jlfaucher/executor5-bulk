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
#include "PackageClass.hpp"
#include "RexxBuiltinFunctions.h"


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
    return source->extractSource();
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
    size_t n = get_position(position, ARG_ONE);
    return source->get(n);
}


/**
 * Retrieve all classes defined by this package.
 *
 * @return A directory of all of the classes defined by this package.
 */
RexxDirectory *PackageClass::getClasses()
{
    // we need to return a copy.  The source might necessarily have any of these,
    // so we return an empty directory if it's not there.
    RexxDirectory *classes = source->getInstalledClasses();
    if (classes != OREF_NULL)
    {
        return (RexxDirectory *)classes->copy();
    }
    else
    {
        return new_directory();
    }
}


/**
 * Retrieve all public classes defined by this package.
 *
 * @return A directory of the public classes.
 */
RexxDirectory *PackageClass::getPublicClasses()
{
    // we need to return a copy.  The source might necessarily have any of these,
    // so we return an empty directory if it's not there.
    RexxDirectory *classes = source->getInstalledPublicClasses();
    if (classes != OREF_NULL)
    {
        return (RexxDirectory *)classes->copy();
    }
    else
    {
        return new_directory();
    }
}


/**
 * Retrieve all of the classes imported into this package from
 * other packages.
 *
 * @return A directory of the imported classes.
 */
RexxDirectory *PackageClass::getImportedClasses()
{
    // we need to return a copy.  The source might necessarily have any of these,
    // so we return an empty directory if it's not there.
    RexxDirectory *classes = source->getImportedClasses();
    if (classes != OREF_NULL)
    {
        return (RexxDirectory *)classes->copy();
    }
    else
    {
        return new_directory();
    }
}


/**
 * Get a list of all routines defined by this package.
 *
 * @return A directory of the routines.
 */
RexxDirectory *PackageClass::getRoutines()
{
    // we need to return a copy.  The source might necessarily have any of these,
    // so we return an empty directory if it's not there.
    RexxDirectory *routines = source->getInstalledRoutines();
    if (routines != OREF_NULL)
    {
        return (RexxDirectory *)routines->copy();
    }
    else
    {
        return new_directory();
    }
}


/**
 * Return a directory of the Public routines defined by this
 * package.
 *
 * @return A directory holding the public routines.
 */
RexxDirectory *PackageClass::getPublicRoutines()
{
    // we need to return a copy.  The source might necessarily have any of these,
    // so we return an empty directory if it's not there.
    RexxDirectory *routines = source->getInstalledPublicRoutines();
    if (routines != OREF_NULL)
    {
        return (RexxDirectory *)routines->copy();
    }
    else
    {
        return new_directory();
    }
}


/**
 * Get the directory of routines that have been imported into
 * to this package form other packages.
 *
 * @return A directory of the imported routines.
 */
RexxDirectory *PackageClass::getImportedRoutines()
{
    // we need to return a copy.  The source might necessarily have any of these,
    // so we return an empty directory if it's not there.
    RexxDirectory *routines = source->getImportedRoutines();
    if (routines != OREF_NULL)
    {
        return (RexxDirectory *)routines->copy();
    }
    else
    {
        return new_directory();
    }
}


/**
 * Get all of the unattached methods defined in this package.
 *
 * @return A directory of the unattached methods.
 */
RexxDirectory *PackageClass::getMethods()
{
    // we need to return a copy.  The source might necessarily have any of these,
    // so we return an empty directory if it's not there.
    RexxDirectory *methods = source->getMethods();
    if (methods != OREF_NULL)
    {
        return (RexxDirectory *)methods->copy();
    }
    else
    {
        return new_directory();
    }
}


/**
 * Get all of the packages that have been added to this package
 * context.
 *
 * @return An array of the added packages.
 */
RexxArray *PackageClass::getImportedPackages()
{
    return source->getPackages()->makeArray();
}


/**
 * Load a package in a source context.
 *
 * @param name   The target package name.
 *
 * @return The loaded package object.
 */
PackageClass *PackageClass::loadPackage(RexxString *name)
{
    // make sure we have a valid name and delegate to the source object
    name = REQUIRED_STRING(name, 1);
    return source->loadRequired(name);
}


/**
 * Load a package in a source context.
 *
 * @param name   The target package name.
 *
 * @return The loaded package object.
 */
RexxObject *PackageClass::addPackage(PackageClass *package)
{
    // this is required
    required_arg(package, ONE);

    if (!package->isInstanceOf(ThePackageClass))
    {
        reportException(Error_Invalid_argument_noclass, "package", "Package");
    }

    source->addPackage(package);

    return this;
}


/**
 * Resolve a class in the context of a package.
 *
 * @param name   The required class name.
 *
 * @return The resolved class object.
 */
RexxClass *PackageClass::resolveClass(RexxString *name)
{
    return source->resolveClass(name);
}
