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

   Samples:  Person Model and Data Classes	  	  	  v01-00 01Oct12

   Contains: classes "PersonModel" and "PersonData".

   Pre-requisites: Model-View Framework.

   Outstanding Problems:
   None.

   Changes:
   v01-00 01Oct2: First version.

------------------------------------------------------------------------------*/


::REQUIRES "..\Support\GenericFile.rex"
::REQUIRES "..\Support\Model.rex"


/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  PersonModel							  v01-00 01Oct12
  ------------

  The "model" part of the Person component - a simple "Model" class that
  illustrates use of the Model-View framework.
  See comments that include the string 'MFV'.

  Changes:
    v01-00 01Oct12: First version.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS 'PersonModel' SUBCLASS Model PUBLIC

  ::METHOD newInstance CLASS PUBLIC
    use strict arg instanceName		-- Instance name of required instance is
    					-- provided by MVF as result of sending
    					-- a 'showModel' message to the Object Manager.
    forward class (super) continue	-- As part of MVF, super does the ~new
    					-- and provide's the model's instance
    					-- data as a param on the init.
    modelId = RESULT			-- MVF requires the model instance
    return modelId			-- ('modelId') to be returned.



  /*----------------------------------------------------------------------------
    init
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD init
    expose dirPerson
    use strict arg dirPerson	-- Super gets data from PersonData and passes
    				-- that data when it creates this instance with
    				-- '~new'.
    				-- The data is in a directory:
    say "PersonModel-init-01: personNumber =" dirPerson["number"]
    say "PersonModel--init-02: dirPerson =" dirPerson "as follows:"
    do i over dirPerson
      say "  "||i "=" dirPerson[i]
    end
    --return self
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /*----------------------------------------------------------------------------
    query - returns Person data.
            Standard protocol:
            Accept a .nil, directory, array, or string of names.
            if .nil then return all; else return values for the names in
            the directory, array, or string. String is assumed to be data
            names separated by one or more spaces.
            All returns are a Directory containing names and values.

    NOTE: This should be implemented by the Model superclass - so this method
          will be removed later!
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD query PUBLIC
    expose dirPerson
    use arg dataNames
    --say "PersonModelData-query-01: familyName =" dirPerson["familyName"]
    say "PersonModel-query-02: dataNames:" dataNames
    dirReturn = .Directory~new
    select
      when dataNames = .nil | dataNames = "" then return dirPerson
      when dataNames = "DATANAMES" then return dirPerson

      when dataNames~isa(.Directory) then do
        do i over dataNames
      	  dirReturn[i] = dirPerson[i]
        end
      end

      when dataNames~isa(.Array) then do
        do i over dataNames
          say "PersonModel-query-03: dataNames: '"||dataNames"'"
          dirReturn[i] = dirPerson[i]
        end
      end

      -- dataNames must be separated by a *single* space.
      when dataNames~isa(.String) then do
        dataNames = dataNames~strip
        --say "PersonModel-query-04: dataNames: '"||dataNames"'"
        n = dataNames~countStr(" ")+1
        do i = 1 to n
          parse var dataNames name " " dataNames
          if name = " " then iterate     -- ignore extraneous leading spaces.
          --say "PersonModel-query-05: name: '"name"'"
          dirReturn[name] = dirPerson[name]
        end
      end

      otherwise return .false
    end

    return dirReturn

/*============================================================================*/



/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  PersonData- The data resource for Persons. 			  v01-00 23Sep12

  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

::CLASS 'PersonData' SUBCLASS GenericFile PUBLIC

  ::ATTRIBUTE created CLASS

  ::METHOD newInstance CLASS PUBLIC		-- Invoked by ObjectMgr
    --use strict arg instanceName
    if self~created = "CREATED" then do		-- If this is first time
      say ".PersonData-newInstance-01."
      personDataId = self~new()			-- the object id of the PersonData component.
      self~created = .true
      return personDataId
    end
    else do
      say ".PersonData-newInstance-02 - Error - Singleton component, so can't have more than one instance."
      return .false
    end


  ::METHOD init PUBLIC
    expose fileName records instanceName
    fileName = "..\samples\person\PersonFile.txt"
    columns = 6					-- colums in the Persons "table"
    records = self~init:super(fileName, columns)
    say "PersonData-init-01: records:" records
    return self					-- MVF


/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  PersonDT - A business data type for Person data.		  v00-01 05May12
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

::CLASS PersonDT PUBLIC

  --		dtName		XML Name	Description
  --		---------	----------	-------------------------------
  --		ProductDT	product
  ::ATTRIBUTE	number		-- number	Person Number
  ::ATTRIBUTE	familyName	-- familyName	Person Family Name
  ::ATTRIBUTE	firstName	-- firstName	Person First Name
  ::ATTRIBUTE	dob		-- dob		Date of Birth (format: yymmdd)
  ::ATTRIBUTE   jobDescr	-- jobDescrip	Job Description
  ::ATTRIBUTE   baseSalary	-- baseSalary	Basic Salary

  ::METHOD makeDir
    dir = .Directory~new
    dir["number"]     = self~number
    dir["familyName"] = self~familyName
    dir["firstName"]  = self~firstName
    dir["dob"]        = self~dob
    dir["jobDescr"]   = self~jobDescr
    dir["baseSalary"] = self~baseSalary

    return dir

  ::METHOD list PUBLIC
    expose number name price uom description size
    say "---------------"
    say "PersonDT-List:"
    say "Number: " number "    Family Name:" familyName
    say "DOB:" dob "   First Name:" firstName
    say "Base Salary:" baseSalary
    say "Job:" jobDescr
    say "---------------"

/*============================================================================*/
