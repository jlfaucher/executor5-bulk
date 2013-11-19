/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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
/* Authors;                                                                   */
/*       W. David Ashley <dashley@us.ibm.com>                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "orxvirt.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/

#define verstr "0.0.1"


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/



/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Function:  orxvirtVersion
 *
 * Return the library version number.
 *
 * @return        Version string
 */
RexxRoutine0(RexxObjectPtr, orxvirtVersion)
{
    return context->NewStringFromAsciiz(verstr);
}


// build the actual entry list
RexxRoutineEntry orxvirtobject_routines[] = {
    REXX_TYPED_ROUTINE(orxvirtVersion, orxvirtVersion),
    REXX_LAST_ROUTINE()
};


// build the actual entry list
RexxMethodEntry orxvirtobject_methods[] = {
    REXX_METHOD(orxvirtConnectOpen, orxvirtConnectOpen),
    REXX_METHOD(orxvirtConnectOpenAuth, orxvirtConnectOpenAuth),
    REXX_METHOD(orxvirtConnectGetCSELF, orxvirtConnectGetCSELF),
    REXX_METHOD(orxvirtConnectClose, orxvirtConnectClose),
    REXX_METHOD(orxvirtConnectListAllDomains, orxvirtConnectListAllDomains),
    REXX_METHOD(orxvirtDomainNew, orxvirtDomainNew),
    REXX_METHOD(orxvirtDomainGetCSELF, orxvirtDomainGetCSELF),
    REXX_METHOD(orxvirtDomainShutdown, orxvirtDomainShutdown),
    REXX_METHOD(orxvirtDomainReset, orxvirtDomainReset),
    REXX_METHOD(orxvirtDomainReboot, orxvirtDomainReboot),
    REXX_METHOD(orxvirtDomainGetState, orxvirtDomainGetState),
    REXX_METHOD(orxvirtDomainDestroy, orxvirtDomainDestroy),
    REXX_METHOD(orxvirtDomainCreate, orxvirtDomainCreate),
    REXX_METHOD(orxvirtDomainFree, orxvirtDomainFree),
    REXX_METHOD(orxvirtDomainSave, orxvirtDomainSave),
    REXX_METHOD(orxvirtDomainGetXMLDesc, orxvirtDomainGetXMLDesc),
    REXX_LAST_METHOD()
};


RexxPackageEntry orxvirtobject_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "OrxVirt",                           // name of the package
    verstr,                              // package information
    NULL,                                // no load/unload functions
    NULL,
    orxvirtobject_routines,              // the exported routines
    orxvirtobject_methods                // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(orxvirtobject);
