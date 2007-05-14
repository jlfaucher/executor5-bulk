/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Test the XML parser class.                                    */
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
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
/*                                                                            */
/* This test converts an XML file to a CSV file.                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


parser = .myparser~new()
errortxt = parser~parse_file('testxml2.xml')

return


::requires 'xmlparser.cls'

::class myparser subclass xmlparser

::method mystream attribute
::method recname attribute
::method recstreet attribute
::method reccity attribute
::method recstate attribute
::method reczip attribute
::method curtag attribute

::method start_element
use arg chunk
self~curtag = chunk~tag
select
   when chunk~tag = 'addressbook' then do
      -- open our output file
      self~mystream = .stream~new('testxml2.csv')
      status = self~mystream~open('write replace')
      end
   when chunk~tag = 'record' then do
      -- create a new record
      self~recname = ''
      self~recstreet = ''
      self~reccity = ''
      self~recstate = ''
      self~reczip = ''
      end
   otherwise nop
   end
return

::method end_element
use arg chunk
select
   when chunk~tag = 'addressbook' then do
      -- close our output file
      status = self~mystream~close()
      end
   when chunk~tag = 'record' then do
      -- create the record
      record = '"' || self~recname~changestr('"', '""') ||'"'
      record = record || ',"' || self~recstreet~changestr('"', '""') ||'"'
      record = record || ',"' || self~reccity~changestr('"', '""') ||'"'
      record = record || ',"' || self~recstate~changestr('"', '""') ||'"'
      record = record || ',"' || self~reczip~changestr('"', '""') ||'"'
      -- write the record
      self~mystream~lineout(record)
      end
   otherwise nop
   end
return

::method passthrough
use arg chunk
-- nop
return

::method text
use arg chunk
select
   when self~curtag = 'name' then self~recname = chunk~text~strip()
   when self~curtag = 'street' then self~recstreet = chunk~text~strip()
   when self~curtag = 'city' then self~reccity = chunk~text~strip()
   when self~curtag = 'state' then self~recstate = chunk~text~strip()
   when self~curtag = 'zip' then self~reczip = chunk~text~strip()
   otherwise nop
   end
return

::method error
use arg err
say err~text
return

