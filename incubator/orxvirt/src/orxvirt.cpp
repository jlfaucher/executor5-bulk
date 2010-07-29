/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Manage a KVM virtual domain.                                  */
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
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
/* Author: W. David Ashley                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/



/* include the standard C headers */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/* include the libvirt headers */
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
/* include the ooRexx headers */
#include <oorexxapi.h>
/* application headers */
#include "orxvirt.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Local Definitions/variables                                                */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Functions                                                          */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Functions                                                           */
/*----------------------------------------------------------------------------*/

/**
 * Function:  OrxVirInitialize
 *
 * Initialize the library.
 *
 * @return        Zero.
 */
RexxRoutine0(int, OrxVirtInitialize)
{
    return virInitialize();
}


/*----------------------------------------------------------------------------*/
/* Private Methods                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* ooRexx Package Stubs                                                       */
/*----------------------------------------------------------------------------*/

// build the actual function entry list
RexxRoutineEntry orxvirt_routines[] = {
    REXX_TYPED_ROUTINE(OrxVirtInitialize, OrxVirtInitialize),
    REXX_LAST_ROUTINE()
};


// build the actual method entry list
RexxMethodEntry orxvirt_methods[] = {
    REXX_METHOD(OrxVirt_ConnectOpen, OrxVirt_ConnectOpen),
    REXX_METHOD(OrxVirt_ConnectClose, OrxVirt_ConnectClose),
    REXX_METHOD(OrxVirt_ConnectGetHostname, OrxVirt_ConnectGetHostname),
    REXX_METHOD(OrxVirt_ConnectGetLibVersion, OrxVirt_ConnectGetLibVersion),
    REXX_METHOD(OrxVirt_ConnectListDomains, OrxVirt_ConnectListDomains),
    REXX_METHOD(OrxVirt_ConnectListDefinedDomains, OrxVirt_ConnectListDefinedDomains),
    REXX_METHOD(OrxVirt_DomainLookupByID, OrxVirt_DomainLookupByID),
    REXX_METHOD(OrxVirt_DomainLookupByName, OrxVirt_DomainLookupByName),
    REXX_METHOD(OrxVirt_DomainCreate, OrxVirt_DomainCreate),
    REXX_METHOD(OrxVirt_DomainIsActive, OrxVirt_DomainIsActive),
    REXX_METHOD(OrxVirt_DomainReboot, OrxVirt_DomainReboot),
    REXX_METHOD(OrxVirt_DomainShutdown, OrxVirt_DomainShutdown),
    REXX_METHOD(OrxVirt_DomainGetName, OrxVirt_DomainGetName),
    REXX_METHOD(OrxVirt_DomainGetID, OrxVirt_DomainGetID),
    REXX_LAST_METHOD()
};


RexxPackageEntry orxvirt_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "OrxVirt",                           // name of the package
    "4.1.0",                             // package information
    NULL,                                // no load functions
    NULL,                                // no unload functions
    orxvirt_routines,                    // the exported routines
    orxvirt_methods                      // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(orxvirt);

