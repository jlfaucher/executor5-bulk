/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (C) W. David Ashley 2004-2008. All Rights Reserved.              */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
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
/* The Request_rec class                                                      */
/*----------------------------------------------------------------------------*/

::class Request_rec public

::METHOD init EXTERNAL "LIBRARY mod_oorexx WWWNewReqRec"
::METHOD connection EXTERNAL "LIBRARY mod_oorexx WWWReqRecConnection"
::METHOD server EXTERNAL "LIBRARY mod_oorexx WWWReqRecServer"
::METHOD next EXTERNAL "LIBRARY mod_oorexx WWWReqRecNext"
::METHOD prev EXTERNAL "LIBRARY mod_oorexx WWWReqRecPrev"
::METHOD main EXTERNAL "LIBRARY mod_oorexx WWWReqRecMain"
::METHOD ismain EXTERNAL "LIBRARY mod_oorexx WWWReqRecIsMain"
::METHOD the_request EXTERNAL "LIBRARY mod_oorexx WWWReqRecThe_request"
::METHOD proxyreq EXTERNAL "LIBRARY mod_oorexx WWWReqRecProxyreq"
::METHOD header_only EXTERNAL "LIBRARY mod_oorexx WWWReqRecHeader_only"
::METHOD protocol EXTERNAL "LIBRARY mod_oorexx WWWReqRecProtocol"
::METHOD status_line EXTERNAL "LIBRARY mod_oorexx WWWReqRecStatus_line"
::METHOD status EXTERNAL "LIBRARY mod_oorexx WWWReqRecStatus"
::METHOD method EXTERNAL "LIBRARY mod_oorexx WWWReqRecMethod"
::METHOD method_number EXTERNAL "LIBRARY mod_oorexx WWWReqRecMethod_number"
::METHOD allowed EXTERNAL "LIBRARY mod_oorexx WWWReqRecAllowed"
::METHOD bytes_sent EXTERNAL "LIBRARY mod_oorexx WWWReqRecBytes_sent"
::METHOD header_in EXTERNAL "LIBRARY mod_oorexx WWWReqRecHeader_in"
::METHOD header_out EXTERNAL "LIBRARY mod_oorexx WWWReqRecHeader_out"
::METHOD err_header_out EXTERNAL "LIBRARY mod_oorexx WWWReqRecErr_header_out"
::METHOD subprocess_env EXTERNAL "LIBRARY mod_oorexx WWWReqRecSubprocess_env"
::METHOD notes EXTERNAL "LIBRARY mod_oorexx WWWReqRecNotes"
::METHOD content_type EXTERNAL "LIBRARY mod_oorexx WWWReqRecContent_type"
::METHOD content_encoding EXTERNAL "LIBRARY mod_oorexx WWWReqRecContent_encoding"
::METHOD handler EXTERNAL "LIBRARY mod_oorexx WWWReqRecHandler"
::METHOD content_languages EXTERNAL "LIBRARY mod_oorexx WWWReqRecContent_languages"
::METHOD no_cache EXTERNAL "LIBRARY mod_oorexx WWWReqRecNo_cache"
::METHOD uri EXTERNAL "LIBRARY mod_oorexx WWWReqRecUri"
::METHOD filename EXTERNAL "LIBRARY mod_oorexx WWWReqRecFilename"
::METHOD path_info EXTERNAL "LIBRARY mod_oorexx WWWReqRecPath_info"
::METHOD args EXTERNAL "LIBRARY mod_oorexx WWWReqRecArgs"
::METHOD finfo_stmode EXTERNAL "LIBRARY mod_oorexx WWWReqRecFinfo_stmode"
::METHOD user EXTERNAL "LIBRARY mod_oorexx WWWReqRecUser"
::METHOD auth_type EXTERNAL "LIBRARY mod_oorexx WWWReqRecAuth_type"


/*----------------------------------------------------------------------------*/
/* The Server_rec class                                                       */
/*----------------------------------------------------------------------------*/

::class Server_rec public

::METHOD init EXTERNAL "LIBRARY mod_oorexx WWWNewSrvRec"
::METHOD server_admin EXTERNAL "LIBRARY mod_oorexx WWWSrvRecServer_admin"
::METHOD server_hostname EXTERNAL "LIBRARY mod_oorexx WWWSrvRecServer_hostname"
::METHOD port EXTERNAL "LIBRARY mod_oorexx WWWSrvRecPort"
::METHOD is_virtual EXTERNAL "LIBRARY mod_oorexx WWWSrvRecIs_virtual"


/*----------------------------------------------------------------------------*/
/* The Connection_rec class                                                   */
/*----------------------------------------------------------------------------*/

::class Connection_rec public

::METHOD init EXTERNAL "LIBRARY mod_oorexx WWWNewCnxRec"
::METHOD aborted EXTERNAL "LIBRARY mod_oorexx WWWCnxRecAborted"

