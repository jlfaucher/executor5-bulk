/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Memcached access class.                                       */
/*              The memcache daemon stores keyed data in a memory hash table  */
/*              for later retrieval. This class provides the methods for      */
/*              accessing a memcache daemon running on a server on the        */
/*              network.                                                      */
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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


::requires './strmsock.cls'

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: memcache                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class memcache public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: memcache                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::method s attribute private


/*----------------------------------------------------------------------------*/
/* Method: cmd                                                                */
/* Description: the set, add and replace methods all use the same interface   */
/*----------------------------------------------------------------------------*/

::method cmd private
use strict arg cmd, key, flags, exptime, data
if data~length = 0 then return .true
self~s~lineout(cmd key flags exptime data~length)
self~s~charout(data || '0D0A'x)
self~status = self~s~linein()
return self~status

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: memcache                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::attribute status


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
use strict arg hostname, port
self~s = .socket~new(hostname, port)
if self~s = -1 then self~status = 'OPEN FAILED'
else self~status = ''
return


/*----------------------------------------------------------------------------*/
/* Method: get                                                                */
/* Description: retrieve a single piece of keyed data from the cache          */
/*----------------------------------------------------------------------------*/

::method get
use strict arg key
parse var key key . -- make sure we only use one key
self~status = ''
self~s~lineout('get' key)
desc = self~s~linein()
parse var desc rowtype key flags bytes
-- was there data?
if rowtype = 'END' then return '' -- the data for the key was not found
-- get the data
data = self~s~charin(, bytes) -- get the data
crlf = self~s~charin(, 2) -- get the ending CRLF
desc = self~s~linein() -- get the END line
return data


/*----------------------------------------------------------------------------*/
/* Method: set                                                                */
/* Description: modify a single piece of keyed data in the cache              */
/*----------------------------------------------------------------------------*/

::method set
use strict arg key, flags = 0, exptime = 0, data
return self~cmd('set', key, flags, exptime, data)


/*----------------------------------------------------------------------------*/
/* Method: add                                                                */
/* Description: add a new piece of keyed data to the cache                    */
/*----------------------------------------------------------------------------*/

::method add
use strict arg key, flags = 0, exptime = 0, data
return self~cmd('add', key, flags, exptime, data)


/*----------------------------------------------------------------------------*/
/* Method: replace                                                            */
/* Description: replace a piece of keyed data in the cache                    */
/*----------------------------------------------------------------------------*/

::method replace
use strict arg key, flags = 0, exptime = 0, data
return self~cmd('replace', key, flags, exptime, data)


/*----------------------------------------------------------------------------*/
/* Method: delete                                                             */
/* Description: delete a piece of keyed data in the cache                     */
/*----------------------------------------------------------------------------*/

::method delete
use strict arg key
exptime = 0
self~s~lineout('delete' key '0')
self~status = self~s~linein()
return self~status


/*----------------------------------------------------------------------------*/
/* Method: version                                                            */
/* Description: retrieve the memcache daemon version                          */
/*----------------------------------------------------------------------------*/

::method version
use strict arg
self~s~lineout('version')
self~status = self~s~linein()
return self~status


/*----------------------------------------------------------------------------*/
/* Method: close                                                              */
/* Description: close the connection to the cache                             */
/*----------------------------------------------------------------------------*/

::method close
use strict arg
self~s~lineout('quit')
self~s~close()
return


