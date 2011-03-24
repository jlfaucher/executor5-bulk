/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Manage a libvirt virtual domain.                              */
/*                                                                            */
/* Copyright (c) 2010-2011 Rexx Language Association. All rights reserved.    */
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
 * Method:  OrxVirt_ConnectOpen
 *
 * Returns a boolean. 1 = ok, 0 = error.
 *
 * @param name    URI of the hypervisor.
 *
 * @return        Boolean.
 **/
RexxMethod1(logical_t,                 // Return type
            OrxVirt_ConnectOpen,       // Object_method name
            OPTIONAL_CSTRING, uri)
{
    virConnectPtr connx;

    connx = virConnectOpen(uri);
    context->SetObjectVariable("CSELF", context->NewPointer(connx));
    if (connx == NULL) {
        context->SetObjectVariable("connected", context->False());
        return 0;
    }
    context->SetObjectVariable("connected", context->True());
    return 1;
}


/**
 * Method:  OrxVirt_ConnectOpenAuth
 *
 * Returns a boolean. 1 = ok, 0 = error.
 *
 * @param name    URI of the hypervisor.
 *
 * @return        Boolean.
 **/
RexxMethod2(logical_t,                 // Return type
            OrxVirt_ConnectOpenAuth,   // Object_method name
            CSTRING, uri,
            int, flags)
{
    virConnectPtr connx;

    connx = virConnectOpenAuth(uri, virConnectAuthPtrDefault, flags);
    context->SetObjectVariable("CSELF", context->NewPointer(connx));
    if (connx == NULL) {
        return false;
    }
    return true;
}


/**
 * Method:  OrxVirt_ConnectClose
 *
 * Returns a status code. 0 = ok, -1 = error.
 *
 * @return        Boolean.
 **/
RexxMethod1(int,                       // Return type
            OrxVirt_ConnectClose,      // Object_method name
            CSELF, self)
{
    if (self == NULL) {
        return -1;
    }
    return virConnectClose((virConnectPtr) self);
}


/**
 * Method:  OrxVirt_ConnectListDomains
 *
 * Returns an array of domain ids.
 *
 * @return        Array.
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            OrxVirt_ConnectListDomains, // Object_method name
            CSELF, self)
{
    virConnectPtr connx = (virConnectPtr) self;
    int ids[virConnectNumOfDomains(connx)];
    RexxArrayObject arr = context->NewArray(0);

    if (connx == NULL) {
        return (RexxObjectPtr) arr;
    }
    int retc = virConnectListDomains(connx, ids, sizeof(ids));
    for (int i = 0; i < retc; i++) {
        context->ArrayAppend(arr, context->Int64((int64_t)ids[i]));
    }
    return (RexxObjectPtr) arr;
}


/**
 * Method:  OrxVirt_ConnectListDefinedDomains
 *
 * Returns an array of domain names.
 *
 * @return        Array.
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            OrxVirt_ConnectListDefinedDomains, // Object_method name
            CSELF, self)
{
    virConnectPtr connx = (virConnectPtr) self;
    char * names[virConnectNumOfDefinedDomains(connx)];
    RexxArrayObject arr = context->NewArray(0);

    if (connx == NULL) {
        return (RexxObjectPtr) arr;
    }
    int retc = virConnectListDefinedDomains(connx, names, sizeof(names));
    for (int i = 0; i < retc; i++) {
        context->ArrayAppend(arr, context->String(names[i]));
    }
    return (RexxObjectPtr) arr;
}


/**
 * Method:  OrxVirt_ConnectGetHostname
 *
 * Returns the server hostname.
 *
 * @return        Hostname.
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            OrxVirt_ConnectGetHostname, // Object_method name
            CSELF, self)
{
    virConnectPtr connx = (virConnectPtr) self;

    if (connx == NULL) {
        return (RexxObjectPtr) context->String("");
    }
    return (RexxObjectPtr) context->String(virConnectGetHostname(connx));
}


/**
 * Method:  OrxVirt_ConnectGetLibVersion
 *
 * Returns the libvirt version number.
 *
 * @return        Version number.
 **/
RexxMethod1(uint64_t,                  // Return type
            OrxVirt_ConnectGetLibVersion, // Object_method name
            CSELF, self)
{
    virConnectPtr connx = (virConnectPtr) self;
    unsigned long ver;

    if (connx == NULL) {
        return -1;
    }
    virConnectGetLibVersion(connx, &ver);
    return (uint64_t) ver;
}

