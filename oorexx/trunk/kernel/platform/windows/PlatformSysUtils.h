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
/* list of REXX platform specific REXXUTIL functions.                         */
/*                                                                            */
/******************************************************************************/

//NOTE:  This file gets included multiple times to define different tables, so
// it does not have standard #ifndef multiple include protections.

   INTERNAL_FUNCTION(SysCls)
   INTERNAL_FUNCTION(SysCurPos)
   INTERNAL_FUNCTION(SysCurState)
   INTERNAL_FUNCTION(SysDriveInfo)
   INTERNAL_FUNCTION(SysDriveMap)
   INTERNAL_FUNCTION(SysDropFuncs)
   INTERNAL_FUNCTION(SysFileDelete)
   INTERNAL_FUNCTION(SysFileSearch)
   INTERNAL_FUNCTION(SysFileTree)
   INTERNAL_FUNCTION(SysGetKey)
   INTERNAL_FUNCTION(SysIni)
   INTERNAL_FUNCTION(SysMkDir)
   INTERNAL_FUNCTION(SysWinVer)
   INTERNAL_FUNCTION(SysRmDir)
   INTERNAL_FUNCTION(SysSearchPath)
   INTERNAL_FUNCTION(SysSleep)
   INTERNAL_FUNCTION(SysTempFileName)
   INTERNAL_FUNCTION(SysTextScreenRead)
   INTERNAL_FUNCTION(SysTextScreenSize)
   INTERNAL_FUNCTION(SysBootDrive)
   INTERNAL_FUNCTION(SysSystemDirectory)
   INTERNAL_FUNCTION(SysFileSystemType)
   INTERNAL_FUNCTION(SysVolumeLabel)
   INTERNAL_FUNCTION(SysCreateMutexSem)
   INTERNAL_FUNCTION(SysOpenMutexSem)
   INTERNAL_FUNCTION(SysCloseMutexSem)
   INTERNAL_FUNCTION(SysRequestMutexSem)
   INTERNAL_FUNCTION(SysReleaseMutexSem)
   INTERNAL_FUNCTION(SysCreateEventSem)
   INTERNAL_FUNCTION(SysOpenEventSem)
   INTERNAL_FUNCTION(SysCloseEventSem)
   INTERNAL_FUNCTION(SysResetEventSem)
   INTERNAL_FUNCTION(SysPostEventSem)
   INTERNAL_FUNCTION(SysPulseEventSem)
   INTERNAL_FUNCTION(SysWaitEventSem)
   INTERNAL_FUNCTION(SysSetPriority)
   INTERNAL_FUNCTION(SysShutDownSystem)
   INTERNAL_FUNCTION(SysSwitchSession)
   INTERNAL_FUNCTION(SysWaitNamedPipe)
   INTERNAL_FUNCTION(SysQueryProcess)
   INTERNAL_FUNCTION(SysSetFileDateTime)
   INTERNAL_FUNCTION(SysGetFileDateTime)
   INTERNAL_FUNCTION(SysVersion)
   INTERNAL_FUNCTION(SysWinVer)
   INTERNAL_FUNCTION(RxWinExec)
   INTERNAL_FUNCTION(SysWinEncryptFile)
   INTERNAL_FUNCTION(SysWinDecryptFile)
   INTERNAL_FUNCTION(SysGetErrortext)
   INTERNAL_FUNCTION(SysFromUniCode)
   INTERNAL_FUNCTION(SysToUniCode)
   INTERNAL_FUNCTION(SysWinGetPrinters)
   INTERNAL_FUNCTION(SysWinGetDefaultPrinter)
   INTERNAL_FUNCTION(SysWinSetDefaultPrinter)
   INTERNAL_FUNCTION(SysFileCopy)
   INTERNAL_FUNCTION(SysFileMove)
   INTERNAL_FUNCTION(SysIsFile)
   INTERNAL_FUNCTION(SysIsFileDirectory)
   INTERNAL_FUNCTION(SysIsFileLink)
   INTERNAL_FUNCTION(SysIsFileCompressed)
   INTERNAL_FUNCTION(SysIsFileEncrypted)
   INTERNAL_FUNCTION(SysIsFileNotContentIndexed)
   INTERNAL_FUNCTION(SysIsFileOffline)
   INTERNAL_FUNCTION(SysIsFileSparse)
   INTERNAL_FUNCTION(SysIsFileTemporary)
