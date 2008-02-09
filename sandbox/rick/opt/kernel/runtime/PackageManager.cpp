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
#include "PackageManager.hpp"
#include "Package.hpp"
#include "Interpreter.hpp"
#include "RexxNativeCode.hpp"


RexxDirectory *PackageManager::packages = OREF_NULL;        // our loaded packages
RexxDirectory *PackageManager::packageFunctions = OREF_NULL;     // table of functions loaded from packages
RexxDirectory *PackageManager::registeredFunction = OREF_NULL;

/**
 * Initialize the package manager global state.
 */
void PackageManager::initialize()
{
    packages = new_directory();               // create the tables for the manager
    packageFunctions = new_directory();
    registeredFunctions = new_directory();
}


/**
 * Return the information that needs to be saved in the saved
 * image.
 *
 * @return An array of the items added to the saved image.
 */
RexxArray *PackageManager::getImageData()
{

    RexxArray *imageArray = new_array(IMAGE_ARRAY_SIZE);
    imageArray->put(packages, IMAGE_PACKAGES);
    imageArray->put(packageFunctions, IMAGE_PACKAGE_FUNCTIONS);
    imageArray->put(registeredFunctions, IMAGE_REGISTERED_FUNCTIONS);

    return imageArray;
}


/**
 * Restore the saved image data.
 *
 * @param imageArray The array we placed in the save image originally.
 */
void PackageManager::restore(RexxArray *imageArray)
{
    // we use copies of these directories to avoid old-to-new image problems.
    packages = (RexxDirectory *)imageArray->get(IMAGE_PACKAGES)->copy();
    packageFunctions = (RexxDirectory *)imageArray->get(IMAGE_PACKAGE_FUNCTIONS)->copy();
    registeredFunctions = (RexxDirectory *)imageArray->get(IMAGE_REGISTERED_FUNCTIONS)->copy();

    for (HashLink i = packages->first(); packages->available(i); i = packages->next(i))
    {
        // get the next package
        Package *package = (Package *)packages->value(i);
        // not one of the internal packages, so reload.
        if (!package->isInternal())
        {
            package->reload();
        }
        else
        {
            // the only internal package is the Rexx one
            package->reload(rexxPackage);
        }
    }
}


/**
 * Normal live marking.
 */
void PackageManager::live(size_t liveMark)
{
    memory_mark(packages);
    memory_mark(packageFunctions);
    memory_mark(registeredFunctions);
}

/**
 * Generalized live marking.
 */
void PackageManager::liveGeneral(int reason)
{
    memory_mark_general(packages);
    memory_mark_general(packageFunctions);
    memory_mark_general(registeredFunctions);
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
    Package *package = loadPackage(name);
    if (package == NULL)
    {
        // this is an error
        reportException(Error_Execution_package, name);
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
             // unable to load the library, so remove this and return NULL.
             packages->remove(name);
             return OREF_NULL;
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
RexxNativeMethod *PackageManager::resolveMethod(RexxString *packageName, RexxString *methodName)
{
    // have we already loaded this package?
    // may need to bootstrap it up first.
    Package *package = getPackage(packageName);

    // now see if this can be resolved.
    RexxNativeMethod *code = package->resolveMethod(methodName);
    // raise an exception if this entry point is not found.
    if (code == OREF_NULL)
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
RoutineClass *PackageManager::resolveFunction(RexxString *function, RexxString *packageName, RexxString *procedure)
{
    // see if we have this one already
    RoutineClass *func = (RoutineClass *)registeredFunctions->at(function);

    // if we have this, then we can return it directly.
    if (func != OREF_NULL)
    {
        return func;
    }

    // go register this (unconditionally....at this point, we don't care if this fails)
    RexxRegisterFunctionDll(function->getStringData(), packageName->getStringData(), procedure->getStringData());

    // resolve a registered entry, if we can and add it to the cache
    return createRegisteredFunction(function);
}



/**
 * Resolve a package function activator.
 *
 * @param function  The function name.
 *
 * @return A function activator for this function, if it can be
 *         resolved.
 */
RoutineClass *PackageManager::resolveFunction(RexxString *function)
{
    // see if we have this one already as a package function
    RoutineClass *func = (RoutineClass *)packageFunctions->at(func);

    // if we have this, then we can return it directly.
    if (func != OREF_NULL)
    {
        return func;
    }

    // see if we have this one already as a registered function
    func = (RoutineClass *)registeredFunctions->at(func);

    // if we have this, then we can return it directly.
    if (func != OREF_NULL)
    {
        return func;
    }

    // resolve a registered entry, if we can and add it to the cache
    return createRegisteredFunction(function);
}


/**
 * Create a new registered function entry and add to the
 * function cache.
 *
 * @param function
 *
 * @return
 */
RoutineClass *PackageManager::createRegisteredFunction(RexxString *function)
{
    REXXPFN entry = NULL;

    // now go resolve this entry pointer
    RexxResolveFunction(function->getStringData(), *entry);

    // this is a failure
    if (entry == NULL)
    {
        return OREF_NULL;
    }

    // create a code handler and add to the cache
    RexxFunction *func = new RoutineClass(new RegisteredFunction((RexxFunctionHandler *)REXXPFN));
    registeredFunctions->put(func, function);
    // we got this
    return func;
}



/**
 * Load an internal package into our list.  This does not
 * load a library, but links package routines ones already
 * contained inside the interpreter.
 *
 * @param name   Name of the package...this is probably "REXX" or "REXXUTIL".
 * @param p      The package descriptor with the method and function list.
 */
void PackageManager::loadInternalPackage(RexxString *name, RexxPackageEntry *p)
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
    // create our initial packages and store them in the table.
    rexxPackage = new Package(OREF_REXX, this, rexxPackageTable);
    packages->put((RexxObject *)rexxPackage, OREF_REXX);
}



/**
 * Restore any entry point address in the packages saved in
 * the original base image.
 *
 * @param master The master PackageManager saved in the image file.
 */
void PackageManager::restoreRootPackages(RexxArray *savedPackages)
{
    // get the references to the saved internal packages.
    rexxPackage = master->rexxPackage;

    // copy the saved directory caches in the package manager. We need a copy
    // of these so we don't end up pushing a bunch of old-to-new references.
    packages = (RexxDirectory *)savedPackages->copy();

    // This is all we need to do.  The saved methods cleared out their entry points
    // on a save and will do a lazy restore on first invocation.
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
        return package->resolveFunction(name) != OREF_NULL ? TheTrueObject : TheFalseObject;
    }

    // ok, this is not a converted new-style package.  Now try registering the function and
    // resolving it in this process.  This will also add this to the local cache
    return resolveFunction(name, module, proc) != OREF_NULL ? TheTrueObject : TheFalseObject;
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
    // remove this from the local cache, then remove it from the global function
    // registration.
    registeredFunctions->remove(name);

    // just allow this to pass through to Rexxapi.  If this was truely registered
    // instead of loaded implicitly, this will remove the entry.  Otherwise, it will
    // return false.  Regardless, we leave it in our internal tables until we exit.
    return RexxDeregisterFunction(name->getStringData()) ? TheTrueObject : TheFalseObject;
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
    // just allow this to pass through to Rexxapi.  If this was truly registered
    // instead of loaded implicitly, it will find it.
    return RexxQueryFunction(name->getStringData()) ? TheTrueObject : TheFalseObject;
}


/**
 * Unload all of the libraries loaded in this Rexx process.
 */
void PackageManager::unload()
{
    // traverse the package table, and force an unload for each library we've loaded up.
    for (HashLink i = packages->first(); packages->available(i); i = packages->next(i))
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


/**
 * Attempt to call a native code function.  This will call either
 * new-style package functions or old-style registered functions.
 *
 * @param activity  The activity we're running under.
 * @param name      The target name of the routine.
 * @param arguments The function arguments.
 * @param argcount  The argument count.
 * @param result    The return result.
 *
 * @return true if we located and successfully called this function.  false
 *         means the function is not located as a native function.
 */
bool PackageManager::callNativeFunction(RexxActivity *activity, RexxString *name,
    RexxObject **arguments, size_t argcount, ProtectedObject &result)
{
    // package functions come first
    RexxFunction *function = (RexxFunction *)packageFunctions->at(name);
    if (function != OREF_NULL)
    {
        function->call(activity, name, arguments, argcount, result);
        return true;
    }
    // now check for registered functions.  This will either return a cached value,
    // or resolve a routine for the first time and return the cached value.
    function = resolveFunction(name);
    if (function != OREF_NULL)
    {
        function->call(activity, name, arguments, argcount, result);
        return true;
    }

    // not one of these
    return false;
}


RoutineClass *PackageManager::getRequires(RexxActivity *activity, RexxString *shortName, RexxString *resolvedName, ProtectedObject &result)
{
    result = OREF_NULL;

    SecurityManager *manager = activity->getSecurityManager();
    RexxObject *securityManager = OREF_NULL;

    name = manager->checkRequires(name, securityManager);
    // no return means forbidden access to this name.  Just return
    // nothing
    if (name == OREF_NULL)
    {
        return OREF_NULL;
    }

    // first check this using the specified name.  Since we need to perform checks in the
    // macro space, it's possible this will be loaded under the simple name.  We'll need to check
    // table again using the fully resolved name afterward.
    WeakReference *requiresRef = (WeakReference *)loadedRequires->get(name);
    if (requiresRef != OREF_NULL)
    {
        RexxCode *resolved = (RexxCode *)requiresRef->getValue();
        if (resolved != OREF_NULL)
        {
            result = resolved;
            return resolved;
        }
        // this was garbage collected, remove it from the table
        loadedRequires->remove(name);
    }

    unsigned short macroPosition;    // a macrospace position marker

    // we need to look in the macrospace before we try checking for a file-based
    // requires.  The macrospace version uses the original name for all checks.  Once we
    // get to the file-based version, we switch to the full resolved name.
    bool checkMacroSpace = RexxQueryMacro(name->getStringData(), &macroPosition) == 0;
    if (checkMacroSpace && (macroPosition == RXMACRO_SEARCH_BEFORE))
    {
        getMacroSpaceRequires(activity, name, result, securityManager);
        return (RexxCode *)(RexxObject *)result;
    }

    // it's possible we don't have a file version of this
    if (resolvedName != OREF_NULL)
    {
        resolvedName = manager->checkRequires(resolvedName);
        // no return means forbidden access to this name.  Just return
        // nothing
        if (resolvedName == OREF_NULL)
        {
            return OREF_NULL;
        }


        // first check this using the specified name.  Since we need to perform checks in the
        // macro space, it's possible this will be loaded under the simple name.  We'll need to check
        // table again using the fully resolved name afterward.
        requiresRef = (WeakReference *)loadedRequires->get(resolvedName);
        if (requiresRef != OREF_NULL)
        {
            RexxCode *resolved = (RexxCode *)requiresRef->getValue();
            if (resolved != OREF_NULL)
            {
                result = resolved;
                return resolved;
            }
            // this was garbage collected, remove it from the table
            loadedRequires->remove(resolvedName);
        }
        getRequiresFile(activity, resolvedName, result, securityManager);
        return (RexxCode *)(RexxObject *)result;
    }

    // do the macrospace after checks
    if (checkMacroSpace)
    {
        getMacroSpaceRequires(activity, name, result, securityManager);
        return (RexxCode *)(RexxObject *)result;
        return;
    }

    // nothing to return
    return OREF_NULL;
}


void PackageManager::getMacroSpaceRequires(RexxActivity *activity, RexxString *name, ProtectedObject &result, RexxObject *securityManager)
{
    // make sure we're not stuck in a circular reference
    activity->checkRequires(name);
    // unflatten the method and protect it
    RexxCode *code = RexxActivation::getMacroCode(name);
    result = code;

    if (securityManager == OREF_NULL)
    {
        code->setSecurityManager(securityManager);
    }
    runRequires(activity, name, code);
    WeakReference *ref = new WeakReference(code);
    loadedRequires->put(ref, name);
}


void PackageManager::getRequiresFile(RexxActivity *activity, RexxString *name, ProtectedObject &result)
{
    // make sure we're not stuck in a circular reference
    activity->checkRequires(name);
    // try to load this from a previously compiled source file
    RexxCode *code = SysRestoreProgram(name);
    if (code == OREF_NULL)             /* unable to restore?              */
    {
                                         /* go translate the image          */
         code = RexxSource::generateCodeFromFile(fullname);
         SysSaveProgram(name, code);     /* go save this method             */
    }

    result = code;
    if (securityManager == OREF_NULL)
    {
        code->setSecurityManager(securityManager);
    }
    runRequires(activity, name, code);
    WeakReference *ref = new WeakReference(code);
    loadedRequires->put(ref, name);
}


void PackageManager::runRequires(RexxActivity *activity, RexxString *name, RoutineClass *code)
{
    ProtectedObject dummy;

    // make sure we reference the circular reference stack
    activity->addRunningRequires(name);
    code->call(activity, name, NULL, 0, OREF_ROUTINENAME, OREF_NULL, EXTERNALCALL, dummy);
                                         /* No longer installing routine.     */
    this->activity->removeRunningRequires(name);
}
