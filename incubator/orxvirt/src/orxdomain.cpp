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


/*----------------------------------------------------------------------------*/
/* Private Methods                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/**
 * Method:  OrxVirt_DomainLookupByID
 *
 * Return true if a domain is found.
 *
 * @param pconnx  Connection object.
 *
 * @param id      The id to look up.
 *
 * @return        Zero.
 **/
RexxMethod2(logical_t,                 // Return type
            OrxVirt_DomainLookupByID,  // Object_method name
            RexxObjectPtr, pconnx,
            int, id)
{
    virConnectPtr connx = (virConnectPtr)context->ObjectToCSelf(pconnx);
    virDomainPtr domain;

    domain = virDomainLookupByID(connx, id);
    context->SetObjectVariable("CSELF", context->NewPointer(domain));
    if (domain == NULL) {
        return 0;
    }
    return 1;
}


/**
 * Method:  OrxVirt_DomainLookupByName
 *
 * Return true if a domain is found.
 *
 * @param pconnx  Connection object.
 *
 * @param name    The domain name to look up.
 *
 * @return        Zero.
 **/
RexxMethod2(logical_t,                 // Return type
            OrxVirt_DomainLookupByName,  // Object_method name
            RexxObjectPtr, pconnx,
            CSTRING, name)
{
    virConnectPtr connx = (virConnectPtr)context->ObjectToCSelf(pconnx);
    virDomainPtr domain;

    domain = virDomainLookupByName(connx, name);
    context->SetObjectVariable("CSELF", context->NewPointer(domain));
    if (domain == NULL) {
        return 0;
    }
    return 1;
}


/**
 * Method:  OrxVirt_DomainCreate
 *
 * Start a domain.
 *
 * @return        Status code (-1, 0, 1)
 **/
RexxMethod1(int,                       // Return type
            OrxVirt_DomainCreate,      // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    return virDomainCreate(domain);
}


/**
 * Method:  OrxVirt_DomainIsActive
 *
 * Is a domain active?
 *
 * @return        Status code (-1, 0, 1)
 **/
RexxMethod1(int,                       // Return type
            OrxVirt_DomainIsActive,    // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    return virDomainIsActive(domain);
}


/**
 * Method:  OrxVirt_DomainReboot
 *
 * Reboot a domain.
 *
 * @return        Status code (-1, 0, 1)
 **/
RexxMethod1(int,                       // Return type
            OrxVirt_DomainReboot,      // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    return virDomainReboot(domain, 0);
}


/**
 * Method:  OrxVirt_DomainShutdown
 *
 * Shutdown a domain.
 *
 * @return        Status code (-1, 0, 1)
 **/
RexxMethod1(int,                       // Return type
            OrxVirt_DomainShutdown,    // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    return virDomainShutdown(domain);
}


/**
 * Method:  OrxVirt_DomainGetName
 *
 * Return the domain name.
 *
 * @return        Domain name.
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            OrxVirt_DomainGetName,     // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    const char *name = virDomainGetName(domain);
    return (RexxObjectPtr) context->String(name);
}


/**
 * Method:  OrxVirt_DomainGetID
 *
 * Return the domain id.
 *
 * @return        Domain id.
 **/
RexxMethod1(uint64_t,                  // Return type
            OrxVirt_DomainGetID,       // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    return (uint64_t) virDomainGetID(domain);
}


/**
 * Method:  OrxVirt_DomainSuspend
 *
 * Suspend a domain.
 *
 * @return        Status code (-1, 0)
 **/
RexxMethod1(uint64_t,                  // Return type
            OrxVirt_DomainSuspend,     // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    return (uint64_t) virDomainSuspend(domain);
}


/**
 * Method:  OrxVirt_DomainResume
 *
 * Resume a domain.
 *
 * @return        Status code (-1, 0)
 **/
RexxMethod1(uint64_t,                  // Return type
            OrxVirt_DomainResume,      // Object_method name
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    return (uint64_t) virDomainResume(domain);
}


/**
 * Method:  OrxVirt_DomainGetInfo
 *
 * Get info about a domain.
 *
 * @return        The domain info object
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            OrxVirt_DomainGetInfo,     // Object_method name
            RexxObjectPtr, rxinfo,
            CSELF, self)
{
    virDomainPtr domain = (virDomainPtr) self;
    virDomainInfo info;
    if (rxinfo == NULL) {
        return context->Nil();
    }
    uint64_t retc = (uint64_t) virDomainGetInfo(domain, &info);
    switch (info.state) {
         VIR_DOMAIN_RUNNING:
             context->SendMessage1(rxinfo, "state=", context->String("RUNNING"));
             break;
         VIR_DOMAIN_BLOCKED:
             context->SendMessage1(rxinfo, "state=", context->String("BLOCKED"));
             break;
         VIR_DOMAIN_PAUSED:
             context->SendMessage1(rxinfo, "state=", context->String("PAUSED"));
             break;
         VIR_DOMAIN_SHUTDOWN:
             context->SendMessage1(rxinfo, "state=", context->String("SHUTDOWN"));
             break;
         VIR_DOMAIN_SHUTOFF:
             context->SendMessage1(rxinfo, "state=", context->String("SHUTOFF"));
             break;
         VIR_DOMAIN_CRASHED:
             context->SendMessage1(rxinfo, "state=", context->String("CRASHED"));
             break;
         default:
             context->SendMessage1(rxinfo, "state=", context->String("NOSTATE"));
    }
    context->SendMessage1(rxinfo, "maxmem=",
                          context->WholeNumber((wholenumber_t)info.maxMem));
    context->SendMessage1(rxinfo, "memory=",
                          context->WholeNumber((wholenumber_t)info.memory));
    context->SendMessage1(rxinfo, "nrvirtcpu=",
                          context->WholeNumber((wholenumber_t)info.nrVirtCpu));
    context->SendMessage1(rxinfo, "cpuTime=",
                          context->UnsignedInt64((uint64_t)info.nrVirtCpu));
    return rxinfo;
}

