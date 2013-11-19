/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Classes for the ooRexx interface to libvirt.                  */
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
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


/*============================================================================*/
/* Class: virErrorLevel                                                       */
/* Note: This class should never be instatiated. It is here just as a         */
/*       container for a host of virErrorLevel constants.                     */
/*============================================================================*/

::class virErrorLevel public

::constant VIR_ERR_NONE 0
::constant VIR_ERR_WARNING 1    /* A simple warning */
::constant VIR_ERR_ERROR 2      /* An error */


/*============================================================================*/
/* Class: virErrorDomain                                                      */
/* Note: This class should never be instatiated. It is here just as a         */
/*       container for a host of virErrorDomain constants.                    */
/*============================================================================*/

::class virErrorDomain public

::constant VIR_FROM_NONE 0
::constant VIR_FROM_XEN 1           /* Error at Xen hypervisor layer */
::constant VIR_FROM_XEND 2          /* Error at connection with xend daemon */
::constant VIR_FROM_XENSTORE 3      /* Error at connection with xen store */
::constant VIR_FROM_SEXPR 4         /* Error in the S-Expression code */
::constant VIR_FROM_XML 5           /* Error in the XML code */
::constant VIR_FROM_DOM 6           /* Error when operating on a domain */
::constant VIR_FROM_RPC 7           /* Error in the XML-RPC code */
::constant VIR_FROM_PROXY 8         /* Error in the proxy code; unused since*/
::constant VIR_FROM_CONF 9          /* Error in the configuration file handling */
::constant VIR_FROM_QEMU 10         /* Error at the QEMU daemon */
::constant VIR_FROM_NET 11          /* Error when operating on a network */
::constant VIR_FROM_TEST 12         /* Error from test driver */
::constant VIR_FROM_REMOTE 13       /* Error from remote driver */
::constant VIR_FROM_OPENVZ 14       /* Error from OpenVZ driver */
::constant VIR_FROM_XENXM 15        /* Error at Xen XM layer */
::constant VIR_FROM_STATS_LINUX 16  /* Error in the Linux Stats code */
::constant VIR_FROM_LXC 17          /* Error from Linux Container driver */
::constant VIR_FROM_STORAGE 18      /* Error from storage driver */
::constant VIR_FROM_NETWORK 19      /* Error from network config */
::constant VIR_FROM_DOMAIN 20       /* Error from domain config */
::constant VIR_FROM_UML 21          /* Error at the UML driver */
::constant VIR_FROM_NODEDEV 22      /* Error from node device monitor */
::constant VIR_FROM_XEN_INOTIFY 23  /* Error from xen inotify layer */
::constant VIR_FROM_SECURITY 24     /* Error from security framework */
::constant VIR_FROM_VBOX 25         /* Error from VirtualBox driver */
::constant VIR_FROM_INTERFACE 26    /* Error when operating on an interface */
::constant VIR_FROM_ONE 27          /* The OpenNebula driver no longer exists. */
::constant VIR_FROM_ESX 28          /* Error from ESX driver */
::constant VIR_FROM_PHYP 29         /* Error from IBM power hypervisor */
::constant VIR_FROM_SECRET 30       /* Error from secret storage */
::constant VIR_FROM_CPU 31          /* Error from CPU driver */
::constant VIR_FROM_XENAPI 32       /* Error from XenAPI */
::constant VIR_FROM_NWFILTER 33     /* Error from network filter driver */
::constant VIR_FROM_HOOK 34         /* Error from Synchronous hooks */
::constant VIR_FROM_DOMAIN_SNAPSHOT 35 /* Error from domain snapshot */
::constant VIR_FROM_AUDIT 36        /* Error from auditing subsystem */
::constant VIR_FROM_SYSINFO 37      /* Error from sysinfo/SMBIOS */
::constant VIR_FROM_STREAMS 38      /* Error from I/O streams */
::constant VIR_FROM_VMWARE 39       /* Error from VMware driver */
::constant VIR_FROM_EVENT 40        /* Error from event loop impl */
::constant VIR_FROM_LIBXL 41        /* Error from libxenlight driver */
::constant VIR_FROM_LOCKING 42      /* Error from lock manager */
::constant VIR_FROM_HYPERV 43       /* Error from Hyper-V driver */
::constant VIR_FROM_CAPABILITIES 44 /* Error from capabilities */
::constant VIR_FROM_URI 45          /* Error from URI handling */
::constant VIR_FROM_AUTH 46         /* Error from auth handling */
::constant VIR_FROM_DBUS 47         /* Error from DBus */
::constant VIR_FROM_PARALLELS 48    /* Error from Parallels */
::constant VIR_FROM_DEVICE 49       /* Error from Device */
::constant VIR_FROM_SSH 50          /* Error from libssh2 connection transport */
::constant VIR_FROM_LOCKSPACE 51    /* Error from lockspace */
::constant VIR_FROM_INITCTL 52      /* Error from initctl device communication */
::constant VIR_FROM_IDENTITY 53     /* Error from identity code */
::constant VIR_FROM_CGROUP 54       /* Error from cgroups */


/*============================================================================*/
/* Class: virErrorNumber                                                      */
/* Note: This class should never be instatiated. It is here just as a         */
/*       container for a host of virErrorNumber constants.                    */
/*============================================================================*/

::class virErrorNumber public

::constant VIR_ERR_OK 0
::constant VIR_ERR_INTERNAL_ERROR 1           /* internal error */
::constant VIR_ERR_NO_MEMORY 2                /* memory allocation failure */
::constant VIR_ERR_NO_SUPPORT 3               /* no support for this function */
::constant VIR_ERR_UNKNOWN_HOST 4             /* could not resolve hostname */
::constant VIR_ERR_NO_CONNECT 5               /* can't connect to hypervisor */
::constant VIR_ERR_INVALID_CONN 6             /* invalid connection object */
::constant VIR_ERR_INVALID_DOMAIN 7           /* invalid domain object */
::constant VIR_ERR_INVALID_ARG 8              /* invalid function argument */
::constant VIR_ERR_OPERATION_FAILED 9         /* a command to hypervisor failed */
::constant VIR_ERR_GET_FAILED 10              /* a HTTP GET command to failed */
::constant VIR_ERR_POST_FAILED 11             /* a HTTP POST command to failed */
::constant VIR_ERR_HTTP_ERROR 12              /* unexpected HTTP error code */
::constant VIR_ERR_SEXPR_SERIAL 13            /* failure to serialize an S-Expr */
::constant VIR_ERR_NO_XEN 14                  /* could not open Xen hypervisor control */
::constant VIR_ERR_XEN_CALL 15                /* failure doing an hypervisor call */
::constant VIR_ERR_OS_TYPE 16                 /* unknown OS type */
::constant VIR_ERR_NO_KERNEL 17               /* missing kernel information */
::constant VIR_ERR_NO_ROOT 18                 /* missing root device information */
::constant VIR_ERR_NO_SOURCE 19               /* missing source device information */
::constant VIR_ERR_NO_TARGET 20               /* missing target device information */
::constant VIR_ERR_NO_NAME 21                 /* missing domain name information */
::constant VIR_ERR_NO_OS 22                   /* missing domain OS information */
::constant VIR_ERR_NO_DEVICE 23               /* missing domain devices information */
::constant VIR_ERR_NO_XENSTORE 24             /* could not open Xen Store control */
::constant VIR_ERR_DRIVER_FULL 25             /* too many drivers registered */
::constant VIR_ERR_CALL_FAILED 26             /* not supported by the drivers (DEPRECATED) */
::constant VIR_ERR_XML_ERROR 27               /* an XML description is not well formed or broken */
::constant VIR_ERR_DOM_EXIST 28               /* the domain already exist */
::constant VIR_ERR_OPERATION_DENIED 29        /* operation forbidden on read-only connections */
::constant VIR_ERR_OPEN_FAILED 30             /* failed to open a conf file */
::constant VIR_ERR_READ_FAILED 31             /* failed to read a conf file */
::constant VIR_ERR_PARSE_FAILED 32            /* failed to parse a conf file */
::constant VIR_ERR_CONF_SYNTAX 33             /* failed to parse the syntax of a conf file */
::constant VIR_ERR_WRITE_FAILED 34            /* failed to write a conf file */
::constant VIR_ERR_XML_DETAIL 35              /* detail of an XML error */
::constant VIR_ERR_INVALID_NETWORK 36         /* invalid network object */
::constant VIR_ERR_NETWORK_EXIST 37           /* the network already exist */
::constant VIR_ERR_SYSTEM_ERROR 38            /* general system call failure */
::constant VIR_ERR_RPC 39                     /* some sort of RPC error */
::constant VIR_ERR_GNUTLS_ERROR 40            /* error from a GNUTLS call */
::constant VIR_WAR_NO_NETWORK 41              /* failed to start network */
::constant VIR_ERR_NO_DOMAIN 42               /* domain not found or unexpectedly disappeared */
::constant VIR_ERR_NO_NETWORK 43              /* network not found */
::constant VIR_ERR_INVALID_MAC 44             /* invalid MAC address */
::constant VIR_ERR_AUTH_FAILED 45             /* authentication failed */
::constant VIR_ERR_INVALID_STORAGE_POOL 46    /* invalid storage pool object */
::constant VIR_ERR_INVALID_STORAGE_VOL 47     /* invalid storage vol object */
::constant VIR_WAR_NO_STORAGE 48              /* failed to start storage */
::constant VIR_ERR_NO_STORAGE_POOL 49         /* storage pool not found */
::constant VIR_ERR_NO_STORAGE_VOL 50          /* storage volume not found */
::constant VIR_WAR_NO_NODE 51                 /* failed to start node driver */
::constant VIR_ERR_INVALID_NODE_DEVICE 52     /* invalid node device object */
::constant VIR_ERR_NO_NODE_DEVICE 53          /* node device not found */
::constant VIR_ERR_NO_SECURITY_MODEL 54       /* security model not found */
::constant VIR_ERR_OPERATION_INVALID 55       /* operation is not applicable at this time */
::constant VIR_WAR_NO_INTERFACE 56            /* failed to start interface driver */
::constant VIR_ERR_NO_INTERFACE 57            /* interface driver not running */
::constant VIR_ERR_INVALID_INTERFACE 58       /* invalid interface object */
::constant VIR_ERR_MULTIPLE_INTERFACES 59     /* more than one matching interface found */
::constant VIR_WAR_NO_NWFILTER 60             /* failed to start nwfilter driver */
::constant VIR_ERR_INVALID_NWFILTER 61        /* invalid nwfilter object */
::constant VIR_ERR_NO_NWFILTER 62             /* nw filter pool not found */
::constant VIR_ERR_BUILD_FIREWALL 63          /* nw filter pool not found */
::constant VIR_WAR_NO_SECRET 64               /* failed to start secret storage */
::constant VIR_ERR_INVALID_SECRET 65          /* invalid secret */
::constant VIR_ERR_NO_SECRET 66               /* secret not found */
::constant VIR_ERR_CONFIG_UNSUPPORTED 67      /* unsupported configuration construct */
::constant VIR_ERR_OPERATION_TIMEOUT 68       /* timeout occurred during operation */
::constant VIR_ERR_MIGRATE_PERSIST_FAILED 69  /* a migration worked, but making the VM persist on the dest host failed */
::constant VIR_ERR_HOOK_SCRIPT_FAILED 70      /* a synchronous hook script failed */
::constant VIR_ERR_INVALID_DOMAIN_SNAPSHOT 71 /* invalid domain snapshot */
::constant VIR_ERR_NO_DOMAIN_SNAPSHOT 72      /* domain snapshot not found */
::constant VIR_ERR_INVALID_STREAM 73          /* stream pointer not valid */
::constant VIR_ERR_ARGUMENT_UNSUPPORTED 74    /* valid API use but unsupported by the given driver */
::constant VIR_ERR_STORAGE_PROBE_FAILED 75    /* storage pool probe failed */
::constant VIR_ERR_STORAGE_POOL_BUILT 76      /* storage pool already built */
::constant VIR_ERR_SNAPSHOT_REVERT_RISKY 77   /* force was not requested for a risky domain snapshot revert */
::constant VIR_ERR_OPERATION_ABORTED 78       /* operation on a domain was canceled/aborted by user */
::constant VIR_ERR_AUTH_CANCELLED 79          /* authentication cancelled */
::constant VIR_ERR_NO_DOMAIN_METADATA 80      /* The metadata is not present */
::constant VIR_ERR_MIGRATE_UNSAFE 81          /* Migration is not safe */
::constant VIR_ERR_OVERFLOW 82                /* integer overflow */
::constant VIR_ERR_BLOCK_COPY_ACTIVE 83       /* action prevented by block copy job */
::constant VIR_ERR_OPERATION_UNSUPPORTED 84   /* The requested operation is not supported */
::constant VIR_ERR_SSH 85                     /* error in ssh transport driver */
::constant VIR_ERR_AGENT_UNRESPONSIVE 86      /* guest agent is unresponsive, not running or not usable */
::constant VIR_ERR_RESOURCE_BUSY 87           /* resource is already in use */


/*============================================================================*/
/* Class: virDomainState                                                      */
/* Note: This class should never be instatiated. It is here just as a         */
/*       container for a host of virDomainState constants.                    */
/*============================================================================*/

::class virDomainState public

::constant VIR_DOMAIN_NOSTATE 0 
::constant VIR_DOMAIN_RUNNING 1 
::constant VIR_DOMAIN_BLOCKED 2 
::constant VIR_DOMAIN_PAUSED  3 
::constant VIR_DOMAIN_SHUTDOWN 4    
::constant VIR_DOMAIN_SHUTOFF 5 
::constant VIR_DOMAIN_CRASHED 6 
::constant VIR_DOMAIN_PMSUSPENDED 7 


/*============================================================================*/
/* Class: virDomainXMLFlags                                                   */
/* Note: This class should never be instatiated. It is here just as a         */
/*       container for a host of virDomainXMLFlags constants.                 */
/*============================================================================*/

::class virDomainXMLFlags public

::constant VIR_DOMAIN_XML_SECURE 1  
::constant VIR_DOMAIN_XML_INACTIVE 2    
::constant VIR_DOMAIN_XML_UPDATE_CPU 4  
::constant VIR_DOMAIN_XML_MIGRATABLE 8  


/*============================================================================*/
/* Routine: orxvirtVersion                                                    */
/*============================================================================*/

::ROUTINE orxvirtVersion EXTERNAL "LIBRARY orxvirt orxvirtVersion"


/*============================================================================*/
/* Class: orxvirtConnect                                                      */
/*============================================================================*/

::class orxvirtConnect public

::METHOD open EXTERNAL "LIBRARY orxvirt orxvirtConnectOpen"
::METHOD openauth EXTERNAL "LIBRARY orxvirt orxvirtConnectOpenAuth"
::METHOD getCSELF EXTERNAL "LIBRARY orxvirt orxvirtConnectGetCSELF"
::METHOD close EXTERNAL "LIBRARY orxvirt orxvirtConnectClose"
::METHOD listalldomains EXTERNAL "LIBRARY orxvirt orxvirtConnectListAllDomains"


/*============================================================================*/
/* Class: orxvirtDomain                                                       */
/*============================================================================*/

::class orxvirtDomain public

::ATTRIBUTE connectptr


::METHOD init EXTERNAL "LIBRARY orxvirt orxvirtDomainNew"
::METHOD getCSELF EXTERNAL "LIBRARY orxvirt orxvirtDomainGetCSELF"
::METHOD shutdown EXTERNAL "LIBRARY orxvirt orxvirtDomainShutdown"
::METHOD reset EXTERNAL "LIBRARY orxvirt orxvirtDomainReset"
::METHOD reboot EXTERNAL "LIBRARY orxvirt orxvirtDomainReboot"
::METHOD getstate_internal EXTERNAL "LIBRARY orxvirt orxvirtDomainGetState" private
::METHOD stop EXTERNAL "LIBRARY orxvirt orxvirtDomainDestroy"
::METHOD destroy EXTERNAL "LIBRARY orxvirt orxvirtDomainDestroy"
::METHOD start EXTERNAL "LIBRARY orxvirt orxvirtDomainCreate"
::METHOD free EXTERNAL "LIBRARY orxvirt orxvirtDomainFree"
::METHOD save EXTERNAL "LIBRARY orxvirt orxvirtDomainSave"
::METHOD getXMLDesc_internal EXTERNAL "LIBRARY orxvirt orxvirtDomainGetXMLDesc" private

::METHOD getstate
-- get the state of a domain as a string
use strict arg
state = self~getstate_internal()
select
   when state = -1 then return 'error'
   when state = .virDomainState~VIR_DOMAIN_NOSTATE() then return 'nostate'
   when state = .virDomainState~VIR_DOMAIN_RUNNING() then return 'running'
   when state = .virDomainState~VIR_DOMAIN_BLOCKED() then return 'blocked'
   when state = .virDomainState~VIR_DOMAIN_PAUSED() then return 'paused'
   when state = .virDomainState~VIR_DOMAIN_SHUTDOWN() then return 'shutdown'
   when state = .virDomainState~VIR_DOMAIN_SHUTOFF() then return 'shutoff'
   when state = .virDomainState~VIR_DOMAIN_CRASHED() then return 'crashed'
   when state = .virDomainState~VIR_DOMAIN_PMSUSPENDED() then return 'pmsuspended'
   otherwise nop
   end
return 'unknown'

::METHOD getXMLDesc
-- get the xml description and put it into a file
-- note that the output file will be empty on an error
use strict arg filename, flags = 0
xml = self~getXMLDesc_internal(flags)
ofile = .stream~new(filename)
ofile~charout(xml)
ofile~close()
return

