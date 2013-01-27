/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2013 Rexx Language Association. All rights reserved.    */
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
   Exercise07 Part 2: The WowModel component.			  v02-00 06Sep12

   Contains:       Classes: WowPicker.

   Pre-requisites: The Model-View Framework (MVF)

   Description:    The model component of the "Words of Wisdom" app.

   Changes:
     v01-00 31May12: First version.
     v02-00 06Sep12: Second version - uses the MVF.
            09Jan13: Comment-out 'say's.
------------------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  WowPicker							  v01-00 31may12
  ---------
  A class that returns a Words of Wisdom string, selected randomly from a set
  of such Words of Wisdom.
  (Potential enhancements: Request a different set from the WowData class;
  			   Have the size of the set configurable.)
  Changes:
    v01-00 31may12: First version.
    v02-00 06Sep12: Seciond version - uses the MVF.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::REQUIRES "..\Support\Model.rex"					-- MVF

-- ::CLASS WowModel Public						-- v01-00
::CLASS WowModel SUBCLASS Model PUBLIC					-- MVF

  /*----------------------------------------------------------------------------
    newInstance - class method required for MVF. 			-- MVF
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD newInstance CLASS PUBLIC
    use strict arg instanceName
    forward class (super) continue		-- MVF: super does the ~new and
                                                --   passes data as a param on the init.
    modelId = RESULT				-- MVF (or just 'return RESULT')
    return modelId				-- MVF

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*----------------------------------------------------------------------------
    init - Since there's not no divided setup as in Views,		-- MVF
           'activate' is same as 'init' so with MVF we just use 'init'.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD init								-- MVF
    expose arrWowSet
    use strict arg dirWowSet		-- MVF: data for model provided by super
    arrWowSet = dirWowSet[Records]	-- MVF: pre-defines format of data.
    return self				-- MVF.

  /*----------------------------------------------------------------------------
    activate - Not used by Model components when using MVF.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*::METHOD activate
    expose arrWowSet
    dataSource = .local~my.idWowData
    arrWowSet = dataSource~readWowSet
    return */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /*----------------------------------------------------------------------------
    pickWow - picks a Word of Wisdom from the current wowSet and returns it.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD pickWow				-- MVF uses 'query' method.
    expose arrWowSet
    i = random(1,25)
    --say "WowModel-query: arrWowSet[3,1] =" arrWowSet[3,1]
    return arrWowSet[i,1]

  /*----------------------------------------------------------------------------
    query - Required by MVF but not used by the View - because does not display
            data when dialog opened for first time.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD query PUBLIC							-- MVF
    return "Wow does not support this method."
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*============================================================================*/