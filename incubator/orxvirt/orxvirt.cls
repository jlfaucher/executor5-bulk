/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Manage virtual domains vi libvirt.                            */
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


/*----------------------------------------------------------------------------*/
/* Class: VirtConnx                                                           */
/*----------------------------------------------------------------------------*/

::class virtconnx public

::attribute connected

::method priv_connect EXTERNAL "LIBRARY orxvirt OrxVirt_ConnectOpen" private
::method priv_close EXTERNAL "LIBRARY orxvirt OrxVirt_ConnectClose" private
::method priv_getLibVersion EXTERNAL "LIBRARY orxvirt OrxVirt_ConnectGetLibVersion" private

::method init
expose connected
connected = .false
if arg() = 0 then do
   self~connect()
   end
else do
   use strict arg uri
   connect(uri)
   end
return

::method connect
expose connected
if self~connected then return
if arg() = 0 then do
   connected = self~priv_connect()
   end
else do
   use strict arg uri
   connected = self~priv_connect(uri)
   end
return

::method close
expose connected
if \connected then return
self~priv_close()
connected = .false
return

::method listDomains EXTERNAL "LIBRARY orxvirt OrxVirt_ConnectListDomains"
::method listDefinedDomains EXTERNAL "LIBRARY orxvirt OrxVirt_ConnectListDefinedDomains"
::method hostname EXTERNAL "LIBRARY orxvirt OrxVirt_ConnectGetHostname"

::method libversion
ver = self~getLibVersion()
if ver = -1 then return 0
major = format(ver / 1000000, , 0)
ver = ver - (major * 1000000)
minor = format(ver / 1000, , 0)
ver = ver - (minor * 1000)
rel = ver
return major'.'minor'.'rel


/*----------------------------------------------------------------------------*/
/* Class: VirtDomain                                                          */
/*----------------------------------------------------------------------------*/

::class virtdomain public

::constant VIR_DOMAIN_XML_SECURE      1  -- dump security sensitive information too
::constant VIR_DOMAIN_XML_INACTIVE    2  -- dump inactive domain information
::constant VIR_DOMAIN_XML_UPDATE_CPU  4  -- update guest CPU requirements according to host CPU

::attribute connx
::attribute id
::attribute name

::method priv_init_id EXTERNAL "LIBRARY orxvirt OrxVirt_DomainLookupByID" private
::method priv_init_name EXTERNAL "LIBRARY orxvirt OrxVirt_DomainLookupByName" private
::method priv_get_name EXTERNAL "LIBRARY orxvirt OrxVirt_DomainGetName" private
::method priv_get_id EXTERNAL "LIBRARY orxvirt OrxVirt_DomainGetID" private

::method init
expose connx id name
use strict arg connx, id
if datatype(id, 'W') then do
   self~priv_init_id(connx, id)
   name = self~priv_get_name()''
   end
else do
   name = id
   self~priv_init_name(connx, name)
   id = self~priv_get_id()
   end
return

::method startup EXTERNAL "LIBRARY orxvirt OrxVirt_DomainCreate"
::method active EXTERNAL "LIBRARY orxvirt OrxVirt_DomainIsActive"
::method reboot EXTERNAL "LIBRARY orxvirt OrxVirt_DomainReboot"
::method shutdown EXTERNAL "LIBRARY orxvirt OrxVirt_DomainShutdown"
::method suspend EXTERNAL "LIBRARY orxvirt OrxVirt_DomainSuspend"
::method resume EXTERNAL "LIBRARY orxvirt OrxVirt_DomainResume"
::method getinfo
info = .virtdomaininfo~new()
self~getinfo_private(info)
return info
::method getinfo_private EXTERNAL "LIBRARY orxvirt OrxVirt_DomainGetInfo" private
::method getxmldesc EXTERNAL "LIBRARY orxvirt OrxVirt_DomainGetXMLDesc"
::method getostype EXTERNAL "LIBRARY orxvirt OrxVirt_DomainGetOSType"


/*----------------------------------------------------------------------------*/
/* Class: VirtDomainInfo                                                      */
/*----------------------------------------------------------------------------*/

::class virtdomaininfo public

::attribute state -- possible values: NOSTATE, RUNNING, BLOCKED, PAUSED, SHUTDOWN,
                  --                  SHUTOFF, CRASHED
::attribute maxmem
::attribute memory
::attribute nrvirtcpu
::attribute cputime

