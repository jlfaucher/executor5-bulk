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
/* Primitive Package management                                               */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#include "oorexx.h"
#include "Package.hpp"
#include "Interpreter.hpp"
#include "RexxNativeCode.hpp"
#include "FunctionActivator.hpp"
#include "InternalAPI.hpp"


PackageManager *PackageManager::singleInstance = OREF_NULL;  // singleton package manager instance

/**
 * Create a new Package object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *Package::operator new(size_t size)
{
    return new_object(size, ThePackageBehaviour);
}

/**
 * Create a new Package object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *PackageManager::operator new(size_t size)
{
    return new_object(size, ThePackageManagerBehaviour);
}

/**
 * Constructor for a loaded package.
 *
 * @param n      Name of the library associated with this package.  This is
 *               also the name used to load the library when requested.
 */
Package::Package(RexxString *n)
{
    ClearObject(this);
    OrefSet(this, libraryName, n);
}

/**
 * Constructor for a loaded package.
 *
 * @param n      Name of the library associated with this package.  This is
 *               also the name used to load the library when requested.
 * @param m      The package manager that orchestrates the loading operations.
 * @param p      The packag table attached to this package name.
 */
Package::Package(RexxString *n, PackageManager *m, ooRexxPackageEntry *p)
{
    ClearObject(this);
    OrefSet(this, libraryName, n);
    // this is an internal package.
    internal = true;
    loadPackage(m, p);
}

/**
 * Normal live marking.
 */
void Package::live()
{
  setUpMemoryMark
      memory_mark(libraryName);
      memory_mark(functions);
      memory_mark(methods);
  cleanUpMemoryMark
}

/**
 * Generalized live marking.
 */
void Package::liveGeneral()
{
  setUpMemoryMarkGeneral
      memory_mark_general(libraryName);
      memory_mark_general(functions);
      memory_mark_general(methods);
  cleanUpMemoryMarkGeneral
}


/**
 * Perform the initial loading of a package.  The loading
 * process involves resolving the external library and
 * attempting to resolve a Rexx package exporter routine
 * in the library.  If the library loads, but does not have
 * the package exporter function, this is a classic library.
 *
 * If we do find a package exporter, then we can load all of
 * the functions immediately.  Method loads are deferred
 * until the first request.
 *
 * @param manager The package manager we're attached to.
 *
 * @return True if we were able to load this as proper ooRexx package
 *         file, false if either step failed.  We do not throw
 *         exceptions here, since these are usually loaded in the
 *         context of operations that return an error result instead
 *         of an exception.
 */
bool Package::load(PackageManager *manager)
{
    // try to load the package table.
    ooRexxPackageEntry *table = getPackageTable();
    // if this is NULL, return false to the manager
    if (table == NULL)
    {
        return false;
    }
    // call the loader to get the package tables and set them up.
    loadPackage(manager, table);
    return true;
}

/**
 * Unload a package library.
 */
void Package::unload()
{
    // call an unloader, if we have one.
    if (p->unloader != NULL)
    {
        ActivityContext *context = currentActivity()->getActivityContext();
        // unload the package
        p->unloader(context);
        currentActivity()->releaseActivityContext(context);
    }
    lib.unload();
}

/**
 * Load a library and see if it is possible to retrieve
 * a package entry from the library.
 *
 * @return A package table entry, if possible.  A load failure or
 *         no package loading functions returns NULL.
 */
ooRexxPackageEntry *Package::getPackageTable()
{
    // first try to load the libary
    PACKAGE_LOADER loader;
    // reset the library handle that was saved in the image.
    lib.reset();

    if (!lib.load((char *)libraryName->getStringData()))
    {
        // we don't report an exception here.  This may have
        // just been a probe attempt to see if we're real.  We'll
        // leave the exception decisions up to the package manager.
        return NULL;
    }

    // we're loaded now, vs. just a package fronting a name.
    loaded = true;
    // the try to resolve a package getting structure
    // resolve the function address
    PFN entry = (PFN)lib.getProcedure("ooRexxGetPackage");
    if (entry == NULL)
    {
        // again, this is not an exception...this could just be
        // a classic style function registration.
        return NULL;
    }
    loader = (PACKAGE_LOADER)entry;
    // call the loader to get the package tables and set them up.
    return (*loader)();
}


/**
 * Load a package with a provided package definition.
 *
 * @param manager The package manager instance we're attached to.
 * @param p       The package table entry.
 */
void Package::loadPackage(PackageManager *manager, ooRexxPackageEntry *p)
{
    ispackage = true;
    package = p;       //NB:  this is NOT an object, so OrefSet is not needed.
    // load the function table
    loadFunctions(manager, package->functions);
    // if this is the initial image load, force loading of the methods also
    if (manager->isMaster())
    {
        loadMethods(manager, package->methods);
    }

    // call a loader, if we have one.
    if (p->loader != NULL)
    {
        ActivityContext *context = currentActivity()->getActivityContext();
        // load the package
        p->loader(context);
        currentActivity()->releaseActivityContext(context);
    }
}

/**
 * Load all of the functions in a package, registering them
 * with the package manager.
 *
 * @param manager The package manager we're associated with.
 * @param table   The package table describing this package.
 */
void Package::loadFunctions(PackageManager *manager, ooRexxFunctionEntry *table)
{
    // no functions exported by this package?  Just return without
    // doing anything.
    if (table == NULL)
    {
        return;
    }

    // create a directory of loaded functions.
    OrefSet(this, functions, new_directory());

    while (table->style != 0)
    {
        // table names tend to be specified in friendly form, we need to
        // convert them to uppercase because "normal" Rexx function names
        // tend to be uppercase.
        RexxString *target = new_upper_string(table->name)->upper();
        FunctionActivator *activator = createActivator(target, (PFN)table->entryPoint, table->style);

        // able to create an activator?
        if (activator != NULL)
        {
            manager->addFunction(target, activator);
        }
        // step to the next table entry
        table++;
    }
}

/**
 * Load all of the methods in a package.
 *
 * @param manager The package manager we're associated with.
 * @param table   The package table describing this package.
 */
void Package::loadMethods(PackageManager *manager, ooRexxMethodEntry *table)
{
    // no methods exported by this package?  Just return without
    // doing anything.
    if (table == NULL)
    {
        return;
    }

    // create a directory of loaded methods.
    OrefSet(this, methods, new_directory());

    while (table->style != 0)
    {
        // table names tend to be specified in friendly form, we need to
        // convert them to uppercase because "normal" Rexx function names
        // tend to be uppercase.
        RexxString *target = new_upper_string(table->name)->upper();
        RexxNativeCode *code = new RexxNativeCode(target, NULL, libraryName, (PFN)table->entryPoint);
        methods->put((RexxObject *)code, target);
        // step to the next table entry
        table++;
    }
}


/**
 * Resolve a named function from this library.  If the function
 * was part of a loaded package, it will already be in our
 * functions table (but should also be in the package manager's
 * merged table, so in theory, we'll never get called for that).
 *
 * If not in the table, then we assume this is a non-package
 * function, either registered with ooRexxRegisterLibraryFunction(),
 * already, or directly resolveable using the procedure
 * address.  If we resolve from the procedure address, we'll
 * also register the function in the global table.
 *
 * @param name      Name of the function.
 * @param procedure Identified procedure for this function on RxFuncAdd.
 *
 * @return Returns a FunctionActivator appropriate for the function
 *         type.  Returns NULL if this was not resolveable.
 */
FunctionActivator *Package::resolveFunction(RexxString *name, RexxString *procedure)
{
    // not created a function table yet?  This can happen if this
    // is an old-type function library and we haven't been asked to
    // resolve any functions yet.
    if (functions == OREF_NULL)
    {
        // create a directory of loaded functions, and one for loaded methods
        OrefSet(this, functions, new_directory());
    }

    // see if we already know about this one
    FunctionActivator *activator = (FunctionActivator *)functions->at(name);
    if (activator != NULL)
    {
        return activator;
    }

    // ok, we probably have a classic-style function registraton going on.  We
    // need to handle this in a couple of steps:
    //
    // Step 1)  Check the global registration to see if this has already been
    // registered.  It may have been done already, and registered with an
    // explicitly specificed call style.
    //
    // Step 2)  We try to resolve the procedure address directly from the library.
    // If we find one, add the global registration using the classic call style.

    PFN entryPoint;
    RexxNumber style;

    // see if it is registered
    if (ooRexxResolveFunction((RexxStringPointer)name->getStringData(),
        (RexxStringPointer)libraryName->getStringData(), (RexxCallback *)&entryPoint, &style) == 0)
    {
        activator = createActivator(name, entryPoint, (int)style);

        // able to create an activator?
        if (activator != NULL)
        {
            // hand this back to the package manager.
            return activator;
        }
    }

    // the try to resolve the target procedure name directly.
    PFN entry = (PFN)lib.getProcedure((char *)procedure->getStringData());
    // can't resolve the procedure?  This is a failure.
    if (entry == NULL)
    {
        return NULL;
    }
    // we have a resolveable entry point, so register this in the global
    // table, but as a classic function.
    RexxReturnCode rc = ooRexxRegisterLibraryFunction((RexxStringPointer)name->getStringData(),
        (RexxStringPointer)libraryName->getStringData(),
        (RexxStringPointer)procedure->getStringData(), FUNCTION_CLASSIC_STYLE);
    // we'll assume this works ok if we get a collision...all other non-zero
    // conditions are a failure.
    if (rc != RXFUNC_OK && rc != RXFUNC_DEFINED)
    {
        // failed for some reason...this bad, definitely very bad.
        return NULL;
    }

    // return a resolved activator for this
    activator = new ClassicNativeActivator(name, entry);
    // add this to our resolved table, and go tell the
    // package manager this exists.
    functions->put((RexxObject *)activator, name);
    return activator;
}


/**
 * Create a function activator for the given function style.
 *
 * @param name   Name of the function.
 * @param entry  The resolved function entry point.
 * @param style  The call style of the function.
 *
 * @return A FunctionActivator appropriate to the type.
 */
FunctionActivator *Package::createActivator(RexxString *name, PFN entry, int style)
{
    FunctionActivator *activator = NULL;

    // now create the appropriate activator for this call type
    switch (style)
    {
        // classic string-oriented function call
        case FUNCTION_CLASSIC_STYLE:
        {
            activator = new ClassicNativeActivator(name, entry);
            break;
        }

        // arguments/results handled as objects, with a context
        case FUNCTION_OBJECT_STYLE:
        {
            activator = new ObjectNativeActivator(name, entry);
            break;
        }

        case FUNCTION_TYPED_STYLE:
        {
            activator = new TypedNativeActivator(name, entry);
            break;
        }
        // unknown type, just ignore this
        default:
            return NULL;
    }

    // add this to our resolved table, and go tell the
    // package manager this exists.
    functions->put((RexxObject *)activator, name);
    return activator;
}

/**
 * Locate a named method entry from the package registration
 * table.
 *
 * @param name   The target name.
 *
 * @return The entry associated with the target entry, if it exists.
 *         NULL indicates a not found condition.
 */
ooRexxMethodEntry *Package::locateMethodEntry(RexxString *name)
{
    ooRexxMethodEntry *entry = package->methods;

    // scan the exported method table for the required method
    while (entry->style != 0)
    {
        // is this one a name match?  Make a method, add it to
        // the table, and return.
        if (name->strICompare((stringchar_t *)entry->name))
        {
            return entry;
        }
        entry++;
    }
    return NULL;
}

/**
 * Locate a named function entry from the package registration
 * table.
 *
 * @param name   The target name.
 *
 * @return A pointer to the located function structure.  Returns NULL
 *         if the package doesn't exist.
 */
ooRexxFunctionEntry *Package::locateFunctionEntry(RexxString *name)
{
    ooRexxFunctionEntry *entry = package->functions;

    // scan the exported method table for the required method
    while (entry->style != 0)
    {
        // is this one a name match?  Make a method, add it to
        // the table, and return.
        if (name->strICompare((stringchar_t *)entry->name))
        {
            return entry;
        }
        entry++;
    }
    return NULL;
}

/**
 * Get a NativeCode object for a method associated with a
 * package.
 *
 * @param name   Name of the target method.
 *
 * @return A RexxNativeCode object for this method, if located.
 */
RexxNativeCode *Package::resolveMethod(RexxString *name)
{
    // create our methods table if not yet created.
    if (methods == OREF_NULL)
    {
        OrefSet(this, methods, new_directory());
    }

    // see if this is in the table yet.
    RexxNativeCode *code = (RexxNativeCode *)methods->at(name);
    if (code == OREF_NULL)
    {
        // find the package definition
        ooRexxMethodEntry *entry = locateMethodEntry(name);
        // if we found one with this name, create a native method out of it.
        if (entry != NULL)
        {
            code = new RexxNativeCode(name, NULL, libraryName, (PFN)entry->entryPoint);
            methods->put((RexxObject *)code, name);
            return code;
        }
        // This, we know from nothing....
        return OREF_NULL;
    }
    // had this cached already.
    return code;
}

/**
 * Get an entry pointer for a method associated with a
 * package.
 *
 * @param name   Name of the target method.
 *
 * @return The entry point address of the method, if found.
 */
PFN Package::resolveMethodEntry(RexxString *name)
{
    // find the package definition
    ooRexxMethodEntry *entry = locateMethodEntry(name);
    // if we found one with this name, create a native method out of it.
    if (entry != NULL)
    {
        return (PFN)entry->entryPoint;
    }
    // This, we know from nothing....
    return NULL;
}


/**
 * Refresh a non-internal package after an image restore.
 *
 * @param manager The package manager instance.
 */
void Package::refreshPackage(PackageManager *manager)
{
    ooRexxPackageEntry *table = getPackageTable();
    if (table == OREF_NULL)
    {
        RexxInterpreter::fatalError((stringchar_t *)"Failure loading required base library");
    }
    // refresh all of the package entries.
    refreshPackage(manager, table);
}


/**
 * Refresh a package on an image restore.  This will update
 * all of the entry point addresses based on the new
 * package table.
 *
 * @param manager The parent package manager.
 * @param p       The new package table.
 */
void Package::refreshPackage(PackageManager *manager, ooRexxPackageEntry *p)
{
    // set the new package pointer.
    package = p;
    HashLink        i;                   /* table traversal index             */

    // if we have functions
    if (functions != OREF_NULL)
    {
                                             /* now traverse the entire table     */
        for (i = functions->first(); functions->available(i); i = functions->next(i))
        {
            // get the next procedure
            RexxString *name = (RexxString *)functions->index(i);
            // get the matching method
            NativeActivator *activator = (NativeActivator *)functions->value(i);

            // find the entry structure for this
            ooRexxFunctionEntry *entry = locateFunctionEntry(name);
            // NB:  This should never happen.
            if (entry != NULL)
            {
                activator->updateEntryPoint((PFN)entry->entryPoint);
            }
        }
    }
                                         /* now traverse the entire table     */
    for (i = functions->first(); functions->available(i); i = functions->next(i))
    {
        // get the next procedure
        RexxString *name = (RexxString *)functions->index(i);
        // get the matching method
        NativeActivator *activator = (NativeActivator *)functions->value(i);

        // find the entry structure for this
        ooRexxFunctionEntry *entry = locateFunctionEntry(name);
        // NB:  This should never happen.
        if (entry != NULL)
        {
            activator->updateEntryPoint((PFN)entry->entryPoint);
        }
    }
    // if we have methods in this package
    if (methods != OREF_NULL)
    {
        for (i = methods->first(); methods->available(i); i = methods->next(i))
        {
            // get the next procedure
            RexxString *name = (RexxString *)methods->index(i);
            // get the matching method
            RexxNativeCode *code = (RexxNativeCode *)methods->value(i);

            // find the entry structure for this and update with
            // the new information.
            ooRexxMethodEntry *entry = locateMethodEntry(name);
            // NB:  This should never happen.
            if (entry != NULL)
            {
                code->setEntry((PFN)entry->entryPoint);
            }
        }
    }
}

/**
 * Constructor for a PackageManager instance.
 */
PackageManager::PackageManager()
{
    ClearObject(this);
    // we defer creating these until we do an actual create/restore operation.
    OrefSet(this, packages, new_directory());
    OrefSet(this, functions, new_directory());
    // and this is not the master
    masterPackage = false;
}


/**
 * Normal live marking.
 */
void PackageManager::live()
{
  setUpMemoryMark
      memory_mark(packages);
      memory_mark(functions);
      memory_mark(rexxPackage);
      memory_mark(rexxutilPackage);
  cleanUpMemoryMark
}

/**
 * Generalized live marking.
 */
void PackageManager::liveGeneral()
{
  setUpMemoryMarkGeneral
      memory_mark_general(packages);
      memory_mark_general(functions);
      memory_mark_general(rexxPackage);
      memory_mark_general(rexxutilPackage);
  cleanUpMemoryMarkGeneral
}


/**
 * Resolve a named package, dynamically loading the package
 * if not already in the table.
 *
 * @param name   The name of the library associated with this package.
 *
 * @return A resolved package...throws an exception if the package
 *         is not loadable.
 */
Package *PackageManager::getPackage(RexxString *name)
{
    // have we already loaded this package?
    // may need to bootstrap it up first.
    Package *package = (Package *)packages->at(name);
    if (package == NULL)
    {
        package = new Package(name);
        // add this to our package list.
        packages->put(packages, name);
        // now force the package to load.
        if (!package->load(this))
        {
            // methods are only resolved from loadable packages.  If
            // this didn't load ok, we can't handle this.
            packages->remove(name);
            reportException(Error_Execution_package, name);
        }
    }
    return package;
}

/**
 * Attempt to load a library without raising an error.  Returns
 * a Package object for the library if the load was successful.
 *
 * @param name   The target library name.
 *
 * @return A Package object for the library, or OREF_NULL if was
 *         not resolvable.
 */
Package *PackageManager::loadPackage(RexxString *name)
{
    // have we already loaded this package?
    // may need to bootstrap it up first.
    Package *package = (Package *)packages->at(name);
    if (package == NULL)
    {
        package = new Package(name);
        // add this to our package list.
        packages->put((RexxObject *)packages, name);
        // now force the package to load.
        if (!package->load(this))
        {
            // this could be a failure because of a load error, or because it's
            // the wrong package type.  If the library was resolveable, we're ok
            // with that.
            if (!package->isLoaded())
            {
                // unable to load the library, so remove this and return NULL.
                packages->remove(name);
                return OREF_NULL;
            }
        }
    }
    return package;
}

/**
 * Create a Native method from a registered package.
 *
 * @param packageName
 *                   The name of the package the library is loaded from.
 *
 * @param methodName The name of the procedure to resolve from the package.
 *
 * @return A Native method that represents this package entry.  Returns
 *         NULL if not found.
 */
RexxNativeCode *PackageManager::resolveMethod(RexxString *packageName, RexxString *methodName)
{
    // have we already loaded this package?
    // may need to bootstrap it up first.
    Package *package = getPackage(packageName);

    // now see if this can be resolved.
    RexxNativeCode *code = package->resolveMethod(methodName);
    // raise an exception if this entry point is not found.
    if (code == OREF_NULL)
    {
         reportException(Error_External_name_not_found_method, methodName);
    }
    return code;
}

/**
 * Resolve the entry point for a native method from a registered package.
 *
 * @param packageName
 *                   The name of the package the library is loaded from.
 *
 * @param methodName The name of the procedure to resolve from the package.
 *
 * @return A Native method that represents this package entry.  Returns
 *         NULL if not found.
 */
PFN PackageManager::resolveMethodEntry(RexxString *packageName, RexxString *methodName)
{
    // have we already loaded this package?
    // may need to bootstrap it up first.
    Package *package = getPackage(packageName);

    // now see if this can be resolved.
    PFN code = package->resolveMethodEntry(methodName);
    // raise an exception if this entry point is not found.
    if (code == NULL)
    {
         reportException(Error_External_name_not_found_method, methodName);
    }
    return code;
}

/**
 * Resolve a package function activator.
 *
 * @param function  The function name.
 * @param package   The package/library name.
 * @param procedure Procedure name.  Only used if directly loaded from a library
 *                  file.
 *
 * @return A function activator for this function, if it can be
 *         resolved.
 */
FunctionActivator *PackageManager::resolveFunction(RexxString *function, RexxString *packageName, RexxString *procedure)
{
    // have we already loaded this package?
    // may need to bootstrap it up first.
    Package *package = getPackage(packageName);

    // now see if this can be resolved.
    FunctionActivator *activator = package->resolveFunction(function, procedure);
    // raise an exception if this entry point is not found.
    if (activator == OREF_NULL)
    {
         reportException(Error_External_name_not_found_routine, function);
    }

    return activator;
}

/**
 * Resolve a package-based function and return a NativeCode
 * wrapper for invoking the function.
 *
 * @param function  Name of the target function.
 * @param packageName
 *                  Name of the function package.
 * @param procedure A procedure name for the function.
 *
 * @return A NativeCode wrapper usable for invoking this function.
 */
RexxNativeCode *PackageManager::resolveNativeFunction(RexxString *function, RexxString *packageName, RexxString *procedure)
{
    return new RexxNativeCode(function, packageName, resolveFunction(function, packageName, procedure));
}

/**
 * Load an internal package into our list.  This does not
 * load a library, but links package routines ones already
 * contained inside the interpreter.
 *
 * @param name   Name of the package...this is probably "REXX" or "REXXUTIL".
 * @param p      The package descriptor with the method and function list.
 */
void PackageManager::loadInternalPackage(RexxString *name, ooRexxPackageEntry *p)
{
    // load up the package and add it to our cache
    Package *package = new Package(name, this, p);
    // have we already loaded this package?
    packages->put(packages, name);
}

/**
 * Create the initial root image package entry for all packages
 * contained within the main Rexx DLL.
 */
void PackageManager::createRootPackages()
{
    OrefSet(this, packages, new_directory());
    OrefSet(this, functions, new_directory());
    // mark this as special.  All packages loaded into this package manager will
    // not use lazy, on-demand loading.
    setMaster();
    // create our initial packages and store them in the table.
    rexxPackage = new Package(OREF_REXX, this, rexxPackageTable);
    packages->put((RexxObject *)rexxPackage, OREF_REXX);
    rexxPackage = new Package(OREF_REXXUTIL, this, rexxutilPackageTable);
    packages->put((RexxObject *)rexxPackage, OREF_REXXUTIL);
}

/**
 * Restore any entry point address in the packages saved in
 * the original base image.
 *
 * @param master The master PackageManager saved in the image file.
 */
void PackageManager::restoreRootPackages(PackageManager *master)
{
    // get the references to the saved internal packages.
    rexxPackage = master->rexxPackage;
    rexxutilPackage = master->rexxutilPackage;

    // copy the saved directory caches in the package manager. We need a copy
    // of these so we don't end up pushing a bunch of old-to-new references.
    packages = (RexxDirectory *)master->packages->copy();
    functions = (RexxDirectory *)master->functions->copy();

    // fix up the entry point addresses in the internal packages.
    rexxPackage->refreshPackage(this, rexxPackageTable);
    rexxutilPackage->refreshPackage(this, rexxutilPackageTable);

    HashLink i;
    // traverse the package table, and force a refresh for any additional
    // packages that got saved in the main image.  These will need to load
    // additional DLLs.
    for (i = packages->first(); packages->available(i); i = packages->next(i))
    {
        // get the next package
        Package *package = (Package *)packages->value(i);
        // not one of the internal packages, so reload.
        if (!package->isInternal())
        {
            package->refreshPackage(this);
        }
    }
}


/**
 * Create the initial instance of the PackageManager during
 * the image creation process.
 *
 * @param rootImage The rootImage object used to anchor global storage.
 */
void PackageManager::createInstance(RootImage *rootImage)
{
    // create a new libary manager instance
    singleInstance = new PackageManager();
    // mark this as special.  All packages loaded into this package manager will
    // not use lazy, on-demand loading.
    singleInstance->setMaster();
    // and set this into the interpreter single instance to anchor in this
    // process.
    RexxInterpreter::setPackageManager(singleInstance);
    // go initialize the root package set.
    singleInstance->createRootPackages();
    // anchor this in the root image.  This will cause all of the packages
    // loaded during image save processing to be saved with the image.
    rootImage->packageManager = singleInstance;
}

/**
 * Restore the PackageManager information during the image restore
 * process.
 *
 * @param rootImage The rootImage object used to anchor global storage.
 */
void PackageManager::restoreInstance(RootImage *rootImage)
{
    // create a fresh package manager instance.
    singleInstance = new PackageManager();
    // and set this into the interpreter single instance to anchor in this
    // process again.
    RexxInterpreter::setPackageManager(singleInstance);
    // this needs to copy the information from the root set.
    singleInstance->restoreRootPackages(rootImage->packageManager);
}

/**
 * Check the loaded package table for a named function.  Returns
 * OREF_NULL if the function does not exist
 *
 * @param name   The String name of the target function.
 *
 * @return Either a function activator appropriate to the package
 *         function, or OREF_NULL if it is not in the table.
 */
FunctionActivator *PackageManager::findFunction(RexxString *name)
{
    return (FunctionActivator *)functions->at(name);
}

/**
 * Insert a function into the global package-managed functions
 * table.
 *
 * @param name   Name of the added function.
 * @param func   The activator object for this function.
 */
void PackageManager::addFunction(RexxString *name, FunctionActivator *func)
{
    functions->put((RexxObject *)func, name);
}

/**
 * Process the basics of RxFuncAdd().  This will return true
 * if the function can be resolved and is callable, false
 * otherwise.  If the target function is not in a loadable
 * package file, this will also do the global registration.
 *
 * @param name   The name of the registered function.
 * @param module The name of the library containing the function.
 * @param proc   The target procedure name (ignored if the target library
 *               is a self-loading one).
 *
 * @return True if the function registration worked and the function
 *         is callable.  False otherwise.
 */
RexxObject *PackageManager::addRegisteredFunction(RexxString *name, RexxString *module, RexxString *proc)
{
    // make sure we're using uppercase name versions here.
    name = name->upper();
    ProtectedObject p1(name);

    // see if this package is resolveable/loadable.
    Package *package = loadPackage(module);
    if (package == OREF_NULL)
    {
        // See if this is resolvable in this context.  If we got it,
        // return True.
        FunctionActivator *activator = package->resolveFunction(name, proc);
        return activator != OREF_NULL ? TheTrueObject : TheFalseObject;
    }
    // this was a load failure...let the caller know about it.
    return TheFalseObject;
}


/**
 * Drop a registered function.
 *
 * @param name   Name of the registered function.
 *
 * @return True if this was deregistered from the global table, false
 *         otherwise.
 */
RexxObject *PackageManager::dropRegisteredFunction(RexxString *name)
{
    // just allow this to pass through to Rexxapi.  If this was truely registered
    // instead of loaded implicitly, this will remove the entry.  Otherwise, it will
    // return false.  Regardless, we leave it in our internal tables until we exit.
    return ooRexxQueryFunction((RexxStringPointer)name->getStringData()) ? TheTrueObject : TheFalseObject;
}


/**
 * The query method backing RxFuncQuery().  This checks both
 * our local tables and the global tables to see if something
 * has been registered.
 *
 * @param name   Target name.
 *
 * @return True if the external function exists in our local tables or
 *         in the global registry.
 */
RexxObject *PackageManager::queryRegisteredFunction(RexxString *name)
{
    // does this name exist in our table?
    if (findFunction(name) != OREF_NULL)
    {
        return TheTrueObject;
    }
    // just allow this to pass through to Rexxapi.  If this was truely registered
    // instead of loaded implicitly, it will find it.
    return ooRexxDeregisterFunction((RexxStringPointer)name->getStringData()) ? TheTrueObject : TheFalseObject;
}


/**
 * Unload all of the libraries loaded in this Rexx process.
 */
void PackageManager::unload()
{
    HashLink i;
    // traverse the package table, and force a refresh for any additional
    // packages that got saved in the main image.  These will need to load
    // additional DLLs.
    for (i = packages->first(); packages->available(i); i = packages->next(i))
    {
        // get the next package
        Package *package = (Package *)packages->value(i);
        // not one of the internal packages, so reload.
        if (!package->isInternal())
        {
            package->unload();
        }
    }

}
