/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2012 Rexx Language Association. All rights reserved.    */
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
/*----------------------------------------------------------------------------*/
/* ooDialog User Guide

   Support - Model						 v00-02  09Aug12
   ----------------
   A simple superclass class for the Model-View framework.

  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

--::REQUIRES "ooDialog.cls"
--::REQUIRES "ObjectMgr.rex"

/*============================================================================*/

::CLASS 'Model' PUBLIC

  ::ATTRIBUTE myData
  ::ATTRIBUTE wantList     CLASS PUBLIC		-- for List subclasses
  --::ATTRIBUTE instanceName CLASS PUBLIC		-- for Anonymous subclasses

  /*----------------------------------------------------------------------------
    newInstance - must be invoked by subclass.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD newInstance CLASS PUBLIC
    expose noDataError				-- .true if data not found.
    use strict arg instanceName
    say ".Model-newInstance-01."
    -- Check that the model's Data object is up and running. If not, then return .false:
    if noDataError = .true then return .false

    -- Now get the name of the Data component (FredModel or FredListModel --> FredData):
    -- Get my root name (i.e. the root name of the subclass):
    className = self~objectName		-- objectName for a class Foo is "The Foo class"
    -- If there's  "LIST" in the name, then set "get all" for the file access
    --  (as opposed to the default of "get 1 record")
    getAllRecords = .false
    p = className~pos("LIST")
    if p > 0 then getAllRecords = .true
    -- if there's a "LIST" in the name, strip it out 'cos an xListModel gets data from xData
    parse var className . root1 "MODEL" .
    parse var root1 root "LIST"
    dataClassName = root||"Data"
    say ".Model-newInstance-02, dataClassName =" dataClassName
    -- Get the id of Data component:
    objMgr = .local~my.ObjectMgr
    --say ".Model-newInstance-03: objMgr =" objMgr
    myDataId = objMgr~getComponentId(dataClassName, "The")
    if myDataId = .false then do		-- if instantiation failed
      say ".Model-newInstance-04: getting ID of Data Class failed."
      noDataError = .true
      return .false
    end
    -- Got my data id, now get data for this model instance.
    -- But distinguish between Entity Models and List Models - the former needs
    -- a single record, the latter a group of records.
    say ".Model-newInstance-05: getAllRecords =" getAllRecords
    if getAllRecords then myData = myDataId~queryAll()	-- retruns an array
    else myData = myDataId~find(instanceName)		-- a directory
    if myData = .false then return .false	-- if ID (key) not found
    -- All is well, then make new instance:
    say ".Model-newInstance-06: myData =" myData
    id = self~new(myData)
    return id
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /*----------------------------------------------------------------------------
    getInstanceName - For an "anonymous" instance only (e.g. CustomerList)
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD getInstanceName CLASS PUBLIC
    expose anonInstanceName
    if anonInstanceName = "ANONINSTANCENAME" then anonInstanceName = 1
    else anonInstanceName = anonInstanceName + 1
    return anonInstanceName




/*============================================================================*/
