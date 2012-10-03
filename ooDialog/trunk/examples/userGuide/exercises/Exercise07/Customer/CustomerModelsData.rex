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
   Exercise 07: Customer Models and Data Classes	  	  v01-01 17Aug12

   Contains: 	   classes "CustomerModel" "CustomerListModel" and "CustomerData".

   Pre-requisites: None.

   Outstanding Problems:
   None.

   Changes:
   v01-00 07Jun12: First version.
   v01-01 09Aug12: Modified to use the Model-View Framework with data read from
                   file using the GenericFile class. CustomerListModel added.
------------------------------------------------------------------------------*/

::REQUIRES "..\Support\GenericFile.rex"
::REQUIRES "..\Support\Model.rex"

/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  CustomerModel							  v01-01 09Aug12
  ------------
  The "model" part of the Customer component.

   Changes:
     v01-00 07Jun12: First version.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS CustomerModel SUBCLASS Model PUBLIC

  ::ATTRIBUTE myData

/*----------------------------------------------------------------------------
    Class Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ::METHOD newInstance CLASS PUBLIC
    expose noDataError
    use strict arg instanceName
    say ".CustomerModel-newInstance-01; class name =" self~objectName
    -- Get my data via my superclass:
    forward class (super) continue
    customerId = RESULT
    return customerId



/*----------------------------------------------------------------------------
    Instance Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ::METHOD init
    use strict arg dirData
    self~myData = dirData
    say "CustomerModel-init-01: customerId  =" dirData["CustNo"]
    say "CustomerModel-init-02: dirCustomer =" dirData "as follows:"
    do i over dirData
      say i "=" dirData[i]
    end
    return self


  ::METHOD query PUBLIC
  /*----------------------------------------------------------------------------
    query - returns Customer data.
            Standard protocol:
            Accept a .nil, directory, array, or string of names.
            if .nil then return all fields; else return values for the names in
            the directory, array, or string. String is assumed to be data
            names separated by one or more spaces.
            All returns are a Directory containing names and values.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    use arg dataNames
    say "CustomerModel-query-01: dataNames:" dataNames
    dirReturn = .Directory~new
    select
      when dataNames = .nil | dataNames = "" then return self~myData
      when dataNames = "DATANAMES"           then return self~myData

      -- Caller is requesting specific data items:
      when dataNames~isa(.Directory) then do
        do i over dataNames
      	  dirReturn[i] = self~dirData[i]
        end
      end

      when dataNames~isa(.Array) then do
        do i over dataNames
          say "CustomerModel-query-03: dataNames: '"||dataNames"'"
          dirReturn[i] = self~dirData[i]
        end
      end

      -- dataNames must be separated by a *single* space.
      when dataNames~isa(.String) then do
        dataNames = dataNames~strip
        --say "CustomerModel-query-04: dataNames: '"||dataNames"'"
        n = dataNames~countStr(" ")+1
        do i = 1 to n
          parse var dataNames name " " dataNames
          if name = " " then iterate     -- ignore extraneous leading spaces.
          --say "CustomerModel-query-05: name: '"name"'"
          dirReturn[name] = self~dirData[name]
        end
      end

      otherwise return .false
    end

    return dirReturn

/*============================================================================*/



/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  CustomerListModel						  v00-01 15Aug12
  ----------------
  The model for a list of Customers.
  Changes:
    v00-01 15Aug12: First version
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS CustomerListModel SUBCLASS Model PUBLIC

  ::ATTRIBUTE myData
  /*----------------------------------------------------------------------------
    Class Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ::METHOD newInstance CLASS PUBLIC
    use arg instanceName
    self~wantList = .true		-- set super's attribute
    forward class (super) continue
    return RESULT


  /*----------------------------------------------------------------------------
    Instance Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ::METHOD init
    expose arrData
    use arg arrData
    self~myData = arrdata
    say "CustomerListModel-init-01: myData =" self~myData
    return self

  ::METHOD query PUBLIC
  /*----------------------------------------------------------------------------
    query - returns an array of all Customer data.
            In MVF this method is invoked by the RcView (or ResView) superclass.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    say "CustomerListModel-query-01."
    return self~myData

/*============================================================================*/



/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  CustomerData							  v01-01 09Aug12
  ------------
  The "data" part of the Customer component.
   Changes:
     v01-00 07Jun12: First version.
     v01-01 09Aug12: Second version - subclasses GenericFile to read data from
                     a disk file.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS CustomerData SUBCLASS GenericFile PUBLIC

  ::ATTRIBUTE created CLASS

/*----------------------------------------------------------------------------
    Class Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD newInstance CLASS PUBLIC		-- Invoked by ObjectMgr
    --use strict arg instanceName
    if self~created = "CREATED" then do		-- If this is first time
      say ".CustomerData-newInstance-01."
      customerDataId = self~new()
      self~created = .true
      return customerDataId
    end
    else do
      say ".CustomerData-newInstance-02 - Error - Singleton component, so can't have more than one instance."
      return .false
    end


  /*----------------------------------------------------------------------------
    Instance Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD init PUBLIC
    expose fileName numRecords
    fileName = "Customer\CustomerFile.txt"
    columns = 5			-- colums in the Customer "table"
    numRecords = self~init:super(fileName, columns)
    say "CustomerData-init-01: numRecords:" numRecords
    if numRecords > 0 then do
      say "CustomerData-init-02: Array is:"
      say self~fileArray~tostring
    end
    return self


/*============================================================================*/

/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  CustomerDT - A business data type for Customer data.		  v00-01 05May12
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

::CLASS CustomerDT PUBLIC

  --		dtName		XML Name	Description
  --		---------	----------	-------------------------------
  --		ProductDT	product
  ::ATTRIBUTE	custNo		-- custNo	Customer Number
  ::ATTRIBUTE	custName	-- custName	Customer Name
  ::ATTRIBUTE	discount	-- discount	Discount code
  ::ATTRIBUTE	address		-- address	Customer's address
  ::ATTRIBUTE   zipCode		-- zip		ZipCode

  ::METHOD makeDir
    dir = .Directory~new
    dir["custNo"]     = self~custNo
    dir["custName"]   = self~custName
    dir["discount"]   = self~discount
    dir["address"]    = self~address
    dir["zipCode"]    = self~zipCode
    return dir

  ::METHOD list PUBLIC
    expose custNo custName discount address zipCode
    say "---------------"
    say "CustomerDT-List:"
    say "CustNo:  " custNo "    CustName:" custName
    say "Address: " address
    say "Zip:     " zipCode "   Discount:" discount
    say "---------------"

/*============================================================================*/
