/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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
/* THIS SOFTWARE IS PROVIDED BY THE COPYright HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYright   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/


::class 'INVOCATIONtester' public

::method init EXTERNAL 'LIBRARY orxinvocation init'
::method setExitType EXTERNAL 'LIBRARY orxinvocation setExitType'
::method getExitType EXTERNAL 'LIBRARY orxinvocation getExitType'
::method !setProgramName PRIVATE EXTERNAL 'LIBRARY orxinvocation setProgramName'
::attribute programName GET
::attribute programName SET
  expose programName
  use arg programName
  self~!setProgramName(programName)

::method !setInitialAddress PRIVATE EXTERNAL 'LIBRARY orxinvocation setInitialAddress'
::attribute initialAddress GET
::attribute initialAddress SET
  expose initialAddress
  use arg initialAddress
  self~!setInitialAddress(initialAddress)

::method !setSearchPath PRIVATE EXTERNAL 'LIBRARY orxinvocation setSearchPath'
::attribute SearchPath GET
::attribute SearchPath SET
  expose SearchPath
  use arg SearchPath
  self~!setSearchPath(SearchPath)


::method !setExtensions PRIVATE EXTERNAL 'LIBRARY orxinvocation setExtensions'
::attribute Extensions GET
::attribute Extensions SET
  expose Extensions
  use arg Extensions
  self~!setExtensions(Extensions)

::method !setLoadLibrary PRIVATE EXTERNAL 'LIBRARY orxinvocation setLoadLibrary'
::attribute LoadLibrary GET
::attribute LoadLibrary SET
  expose LoadLibrary
  use arg LoadLibrary
  self~!setLoadLibrary(LoadLibrary)

::attribute FunctionExit SET EXTERNAL 'LIBRARY orxinvocation setFunctionExit'
::attribute CommandExit SET EXTERNAL 'LIBRARY orxinvocation setCommandExit'
::attribute QueueExit SET EXTERNAL 'LIBRARY orxinvocation setQueueExit'
::attribute IOExit SET EXTERNAL 'LIBRARY orxinvocation setIOExit'
::attribute TraceExit SET EXTERNAL 'LIBRARY orxinvocation setTraceExit'
::attribute HaltExit SET EXTERNAL 'LIBRARY orxinvocation setHaltExit'
::attribute InitExit SET EXTERNAL 'LIBRARY orxinvocation setInitExit'
::attribute TermExit SET EXTERNAL 'LIBRARY orxinvocation setTermExit'
::attribute ScriptingExit SET EXTERNAL 'LIBRARY orxinvocation setScriptingExit'
::attribute ScriptingExit SET EXTERNAL 'LIBRARY orxinvocation setScriptingExit'
::attribute ObjectFunctionExit SET EXTERNAL 'LIBRARY orxinvocation setObjectFunctionExit'
::attribute NovalueExit SET EXTERNAL 'LIBRARY orxinvocation setNovalueExit'
::attribute ValueExit SET EXTERNAL 'LIBRARY orxinvocation setValueExit'

::attribute FunctionExit GET EXTERNAL 'LIBRARY orxinvocation getFunctionExit'
::attribute CommandExit GET EXTERNAL 'LIBRARY orxinvocation getCommandExit'
::attribute QueueExit GET EXTERNAL 'LIBRARY orxinvocation getQueueExit'
::attribute IOExit GET EXTERNAL 'LIBRARY orxinvocation getIOExit'
::attribute TraceExit GET EXTERNAL 'LIBRARY orxinvocation getTraceExit'
::attribute HaltExit GET EXTERNAL 'LIBRARY orxinvocation getHaltExit'
::attribute InitExit GET EXTERNAL 'LIBRARY orxinvocation getInitExit'
::attribute TermExit GET EXTERNAL 'LIBRARY orxinvocation getTermExit'
::attribute ScriptingExit GET EXTERNAL 'LIBRARY orxinvocation getScriptingExit'
::attribute ScriptingExit GET EXTERNAL 'LIBRARY orxinvocation getScriptingExit'
::attribute ObjectFunctionExit GET EXTERNAL 'LIBRARY orxinvocation getObjectFunctionExit'
::attribute NovalueExit GET EXTERNAL 'LIBRARY orxinvocation getNovalueExit'
::attribute ValueExit GET EXTERNAL 'LIBRARY orxinvocation getValueExit'

::method getRC EXTERNAL 'LIBRARY orxinvocation getRC'
::method getCode EXTERNAL 'LIBRARY orxinvocation getCode'
