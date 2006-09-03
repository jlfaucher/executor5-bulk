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
/* Package definition for exported native methods in the "REXX" package       */
/*                                                                            */
/******************************************************************************/

#include "oorexx.h"


#define INTERNAL_METHOD(name) REXX_METHOD_PROTOTYPE(name)

#include "NativeMethods.h"             // bring in the standard list,
#include "SysNativeMethods.h"          // plus any system extensions

#undef  INTERNAL_METHOD
#define INTERNAL_METHOD(name) REXX_METHOD(name, name)

// now build the actual entry list
ooRexxMethodEntry rexx_methods[] =
{
#include "NativeMethods.h"             // bring in the standard list,
#include "SysNativeMethods.h"          // plus any system extensions
};


#define INTERNAL_FUNCTION(name) REXX_TYPED_FUNCTION_PROTOTYPE(name)
#define INTERNAL_NAMED_FUNCTION(name, entry) REXX_TYPED_FUNCTION_PROTOTYPE(entry)

#include "NativeFunctions.h"             // bring in the standard list,
#include "SysNativeFunctions.h"          // plus any system extensions

#undef  INTERNAL_FUNCTION
#define INTERNAL_FUNCTION(name) REXX_TYPED_FUNCTION(name, name)

#undef  INTERNAL_NAMED_FUNCTION
#define INTERNAL_NAMED_FUNCTION(name, entry) REXX_TYPED_FUNCTION(name, entry)

// now build the actual entry list
ooRexxFunctionEntry rexx_functions[] =
{
#include "NativeFunctions.h"             // bring in the standard list,
#include "SysNativeFunctions.h"          // plus any system extensions
};

ooRexxPackageEntry rexx_package_entry =
{
    STANDARD_PACKAGE_HEADER
    "REXX",                              // name of the package
    "4.0",                               // package information
    rexx_functions,                      // the exported
    rexx_methods
};

#undef INTERNAL_FUNCTION
#undef INTERNAL_NAMED_FUNCTION
#define INTERNAL_FUNCTION(name) REXX_TYPED_FUNCTION_PROTOTYPE(name)
#define INTERNAL_NAMED_FUNCTION(name, entry) REXX_TYPED_FUNCTION_PROTOTYPE(entry)

#include "CommonSysUtils.h"              // bring in the standard list,
#include "PlatformSysUtils.h"            // plus any system extensions

#undef INTERNAL_FUNCTION
#undef INTERNAL_NAMED_FUNCTION
#define INTERNAL_FUNCTION(name) REXX_TYPED_FUNCTION(name, name)
#define INTERNAL_NAMED_FUNCTION(name, entry) REXX_TYPED_FUNCTION(name, entry)

// now build the actual entry list
ooRexxFunctionEntry rexxutil_functions[] =
{
#include "CommonSysUtils.h"              // bring in the standard list,
#include "PlatformSysUtils.h"            // plus any system extensions
};

ooRexxPackageEntry rexxutil_package_entry =
{
    STANDARD_PACKAGE_HEADER
    "REXXUTIL",                          // name of the package
    "4.0",                               // package information
    NULL,                                // no load/unload functions
    NULL,
    rexxutil_functions,                  // the exported functions
    NULL                                 // no methods in rexxutil.
};


// and initialize the PackageManager pointers to these tables.
#include "Package.hpp"

ooRexxPackageEntry *PackageManager::rexxPackageTable = &rexx_package_entry;
ooRexxPackageEntry *PackageManager::rexxutilPackageTable = &rexxutil_package_entry;
