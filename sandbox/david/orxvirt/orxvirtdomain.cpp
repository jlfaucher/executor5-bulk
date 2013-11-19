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


/* include the linux headers and our local stuff */
#include "orxvirt.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/

#define INVALIDDOMAINPTR "The internal pointer to the Domain instance is not valid."


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/


/*============================================================================*/
/* Public Methods                                                             */
/*============================================================================*/

/**
 * Method:  init
 *
 * Initialize the domain instance.
 *
 * @return        Zero.
 **/
RexxMethod3(int,                         // Return type
            orxvirtDomainNew,            // Object_method name
            RexxObjectPtr, myConnect,    // Connection object
            CSTRING, domainName,         // Domain name
            OPTIONAL_CSTRING, filename)  // restore file name
{
    virConnectPtr myConnectPtr = (virConnectPtr) context->ObjectToCSelf(myConnect);
    if (strlen(domainName) == 0) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = virDomainLookupByName(myConnectPtr, domainName);
    context->SetObjectVariable("CSELF", context->NewPointer(myDomain));

    if (myDomain == NULL) {
        return 0;
    }

    if (argumentExists(3)) {
        int retc = virDomainRestore(myConnectPtr, filename);
    }
    return 0;
}

/**
 * Method:  getCSELF
 *
 * Get the CSELF pointer.
 *
 * @return        RexxPointerObject.
 **/
RexxMethod1(RexxObjectPtr,               // Return type
            orxvirtDomainGetCSELF,       // Object_method name
            CSELF, self)                 // Self
{
    return context->NewPointer(self);
}

/**
 * Method:  shutdown
 *
 * Normal OS shutdown.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                         // Return type
            orxvirtDomainShutdown,       // Object_method name
            CSELF, self)                 // Self
{
    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    return virDomainShutdown(myDomain);
}

/**
 * Method:  reset
 *
 * Emulates hitting the power button (forced power off).
 *
 * @return        Zero.
 **/
RexxMethod1(int,                         // Return type
            orxvirtDomainReset,          // Object_method name
            CSELF, self)                 // Self
{
    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    return virDomainReset(myDomain, 0);
}

/**
 * Method:  reboot
 *
 * Reboot the domain.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                         // Return type
            orxvirtDomainReboot,         // Object_method name
            CSELF, self)                 // Self
{
    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    return virDomainReboot(myDomain, 0);
}

/**
 * Method:  getstate
 *
 * Get the state of the domain.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                         // Return type
            orxvirtDomainGetState,       // Object_method name
//            int, flags,                // Flags
            CSELF, self)                 // Self
{
    // Todo: add the flags as a argument so we can pass it to
    // virDomainGetState()

    int state, reason, retc;

    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;
    retc = virDomainGetState(myDomain, &state, &reason, 0);

    if (retc == 0) {
        return state;
    }
    return retc;
}

/**
 * Method:  destroy
 *
 * Destroy (stop) the domain.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                         // Return type
            orxvirtDomainDestroy,        // Object_method name
            CSELF, self)                 // Self
{
    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    return virDomainDestroy(myDomain);
}

/**
 * Method:  create
 *
 * Run (start/boot) the domain.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                         // Return type
            orxvirtDomainCreate,         // Object_method name
            CSELF, self)                 // Self
{
    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    return virDomainCreate(myDomain);
}

/**
 * Method:  free
 *
 * Free the domain.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                         // Return type
            orxvirtDomainFree,           // Object_method name
            CSELF, self)                 // Self
{
    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    int retc = virDomainFree(myDomain);
    if (retc == 0) {
        context->SetObjectVariable("CSELF", context->NewPointer(NULL));
    }
    return retc;
}

/**
 * Method:  save
 *
 * Save the running domain.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                         // Return type
            orxvirtDomainSave,           // Object_method name
            CSTRING, filename,           // File name for the save domain
            CSELF, self)                 // Self
{
    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    return virDomainSave(myDomain, filename);
}

/**
 * Method:  getXMLDesc
 *
 * Get the domain's XML description.
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,               // Return type
            orxvirtDomainGetXMLDesc,     // Object_method name
            int, flags,                  // Flags
            CSELF, self)                 // Self
{
    RexxObjectPtr xmlstr;

    if (self == NULL) {
        context->RaiseException1(Rexx_Error_Interpretation,
                                 context->NewStringFromAsciiz(INVALIDDOMAINPTR));
        return 0;
    }
    virDomainPtr myDomain = (virDomainPtr) self;

    char *xml = virDomainGetXMLDesc(myDomain, (unsigned int)flags);
    if (xml == NULL) {
        RexxObjectPtr xmlstr = (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    else {
        RexxObjectPtr xmlstr = (RexxObjectPtr)context->NewStringFromAsciiz(xml);
        free(xml);
    }
    return xmlstr;
}

