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


#ifndef ORXVIRT_H
#define ORXVIRT_H


/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <oorexxapi.h>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method Prototypes                                                          */
/*----------------------------------------------------------------------------*/

REXX_METHOD_PROTOTYPE(GrxWidgetNew)


REXX_METHOD_PROTOTYPE(orxvirtConnectOpen)
REXX_METHOD_PROTOTYPE(orxvirtConnectOpenAuth)
REXX_METHOD_PROTOTYPE(orxvirtConnectGetCSELF)
REXX_METHOD_PROTOTYPE(orxvirtConnectClose)
REXX_METHOD_PROTOTYPE(orxvirtConnectListAllDomains)
REXX_METHOD_PROTOTYPE(orxvirtDomainNew)
REXX_METHOD_PROTOTYPE(orxvirtDomainGetCSELF)
REXX_METHOD_PROTOTYPE(orxvirtDomainShutdown)
REXX_METHOD_PROTOTYPE(orxvirtDomainReset)
REXX_METHOD_PROTOTYPE(orxvirtDomainReboot)
REXX_METHOD_PROTOTYPE(orxvirtDomainGetState)
REXX_METHOD_PROTOTYPE(orxvirtDomainDestroy)
REXX_METHOD_PROTOTYPE(orxvirtDomainCreate)
REXX_METHOD_PROTOTYPE(orxvirtDomainFree)
REXX_METHOD_PROTOTYPE(orxvirtDomainSave)
REXX_METHOD_PROTOTYPE(orxvirtDomainGetXMLDesc)

 
 #endif /* ORXVIRT_H */
 
