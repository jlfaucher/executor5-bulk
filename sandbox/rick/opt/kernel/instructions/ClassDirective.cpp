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
/* REXX Kernel                                           ClassDirective.cpp   */
/*                                                                            */
/* Primitive Translator Abstract Directive Code                               */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include "RexxCore.h"
#include "ClassDirective.hpp"
#include "Clause.hpp"



/**
 * Construct a ClassDirective.
 *
 * @param n      The name of the requires target.
 * @param p      The public name of the requires target.
 * @param clause The source file clause containing the directive.
 */
ClassDirective::ClassDirective(RexxString *n, RexxString *p, RexxClause *clause) : RexxDirective(clause, KEYWORD_CLASS)
{
    name = n;
    publicName = p;
}

/**
 * Normal garbage collecting live mark.
 *
 * @param liveMark The current live object mark.
 */
void ClassDirective::live(size_t liveMark)
{
    memory_mark(this->nextInstruction);  // must be first one marked (though normally null)
    memory_mark(this->publicName);
    memory_mark(this->idName);
    memory_mark(this->metaclassName);
    memory_mark(this->subclassName);
    memory_mark(this->dependencies);
}


/**
 * The generalized object marking routine.
 *
 * @param reason The processing faze we're running the mark on.
 */
void ClassDirective::liveGeneral(int reason)
{
    memory_mark_general(this->nextInstruction);  // must be first one marked (though normally null)
    memory_mark_general(this->publicName);
    memory_mark_general(this->idName);
    memory_mark_general(this->metaclassName);
    memory_mark_general(this->subclassName);
    memory_mark_general(this->dependencies);
}


/**
 * Flatten the directive instance.
 *
 * @param envelope The envelope we're flattening into.
 */
void ClassDirective::flatten(RexxEnvelope *envelope)
{
    setUpFlatten(ClassDirective)

        flatten_reference(newThis->nextInstruction, envelope);
        flatten_reference(newThis->publicName, envelope);
        flatten_reference(newThis->idName, envelope);
        flatten_reference(newThis->metaclassName, envelope);
        flatten_reference(newThis->subclassName, envelope);
        // by this time, we should be finished with this, and it should
        // already be null.  Make sure this is the case.
        newThis->dependencies = OREF_NULL;

    cleanUpFlatten
}


/**
 * Allocate a new requires directive.
 *
 * @param size   The size of the object.
 *
 * @return The memory for the new object.
 */
void *ClassDirective::operator new(size_t size)
{
    return new_object(size, T_ClassDirective); /* Get new object                    */
}


/**
 * Do install-time processing of the ::requires directive.  This
 * will resolve the directive and merge all of the public information
 * from the resolved file into this program context.
 *
 * @param activation The activation we're running under for the install.
 */
void ClassDirective::install(RexxSource *source, RexxActivation *activation)
{
    RexxClass *metaclass = OREF_NULL;
    RexxClass *subclass = (RexxClass *)TheEnvironment->fastAt(OREF_OBJECTSYM);

    // make this the current line for the error context
    activation->setCurrent(this);

    if (metaclassName != OREF_NULL)
    {
        /* resolve the class                 */
        metaclass = source->resolveClass(metaclassName, activation);
        if (metaclass == OREF_NULL)    /* nothing found?                    */
        {
            /* not found in environment, error!  */
            reportException(Error_Execution_nometaclass, metaclassName);
        }
    }

    if (subclassName != OREF_NULL)  /* no subclass?                      */
    {
        /* resolve the class                 */
        subclass = source->resolveClass(subclassName, activation);
        if (subclass == OREF_NULL)     /* nothing found?                    */
        {
            /* not found in environment, error!  */
            reportException(Error_Execution_noclass, subclassName);
        }
    }

    RexxClass *classObject;       // the class object we're creating

    // create the class object using the appropriate mechanism
    if (mixinClass)
    {
        classObject = subclass->mixinclass(idName, metaclass, classMethods);
    }
    else
    {
        /* doing a subclassing               */
        classObject = subclass->subclass(idName, metaclass, class_methods);
    }
    /* add the class to the directory    */
    source->addInstalledClass(name, classObject, publicClass);
    if (inherits != OREF_NULL)       /* have inherits to process?         */
    {
        /* establish remainder of inheritance*/
        for (j = 1; j <= inherits->size(); j++)
        {
            /* get the next inherits name        */
            RexxString *inheritsName = (RexxString *)inherits->get(j);
            /* go resolve the entry              */
            subclass = source->resolveClass(inheritsName, activation);
            if (subclass == OREF_NULL)   /* not found?                        */
            {
                /* not found in environment, error!  */
                reportException(Error_Execution_noclass, inheritsName);
            }
            /* do the actual inheritance         */
            classObject->sendMessage(OREF_INHERIT, subclass);
        }
    }
    if (instanceMethods != OREF_NULL) /* have instance methods to add?     */
    {
        /* define them to the class object   */
        classObject->defineMethods(instanceMethods);
    }
}


/**
 * Check if a dependency this class has is based off of a
 * class co-located in the same class package.
 *
 * @param name   The class name.
 * @param class_directives
 *               The global local classes list.
 */
void ClassDireective::checkDepdendency(RexxString *name, RexxDirectory *class_directives)
{
    if (name != OREF_NULL)
    {
        // if this is in  install?           */
        if (class_dependencies->entry(name) != OREF_NULL)
        {
            if (dependencies == OREF_NULL)
            {
                OrefSet(this, this->dependencies, new_directory());
            }
            /* add to our pending list           */
            dependencies->setEntry(name, name);
        }
    }
}


/**
 * Check our class dependencies against the locally defined class
 * list to develop a cross dependency list.
 *
 * @param class_directives
 *               The global set of defined classes in this package.
 */
void ClassDirective::addDependencies(RexxDirectory *class_directives)
{
    // When the class was created, a dependency directory was added as a marker to
    // prevent duplicate classes from getting created.  We now
    RexxDirectory *dependencies = (RexxDirectory *)(class_dependencies->fastAt(publicName));

    // now for each of our dependent classes, if this is defined locally, we
    // an entry to our dependency list to aid the class ordering

    checkDependency(metaClassName, class_directives);
    checkDependency(subclassName, class_directives);
    // process each inherits item the same way
    if (inherits != OREF_NULL)
    {
        for (size_t i = 1; i <= inherits->size(); i++)
        {
            checkDependency((RexxString *)(inherits->get(i)));
        }
    }
}


/**
 * Check if this class has any additional in-package dependencies.
 *
 * @return true if all in-package dependencies have been resolved already.
 */
bool ClassDirective::dependenciesResolved()
{
    return dependencies == OREF_NULL;
}


/**
 * Remove a class from the dependency list.
 *
 * @param name   The name of the class that's next in the ordering.
 */
void ClassDirective::removeDependency(RexxString *name)
{
    // if we have a dependency list, remove this name from the
    // list.  If this is our last dependency item, we can junk
    // the list entirely.
    if (dependencies != OREF_NULL)
    {
        dependencies->remove(name);
        if (dependencies->items() == 0)
        {
            OrefSet(this, this->dependencies, OREF_NULL);
        }
    }
}


/**
 * Add an inherits class to the class definition.
 *
 * @param name   The name of the inherited class.
 */
void ClassDirective::addInherits(RexxString *name)
{
    if (inheritsClasses == OREF_NULL)
    {
        OrefSet(this, this->inheritsClasses, new_list());
    }
    inheritsClasses->append(name);
}


/**
 * Retrieve the class methods directory for this class.
 *
 * @return The class methods directory.
 */
RexxDirectory *ClassDirective::getClassMethods()
{
    if (classMethods == OREF_NULL)
    {
        OrefSet(this, this->classMethods, new_directory());
    }
    return classMethods;
}


/**
 * Retrieve the instance methods directory for this class.
 *
 * @return The instance methods directory.
 */
RexxDirectory *ClassDirective::getClassMethods()
{
    if (instanceMethods == OREF_NULL)
    {
        OrefSet(this, this->instanceMethods, new_directory());
    }
    return instanceMethods;
}

