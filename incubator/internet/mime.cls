/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: MIME classes.                                                 */
/*                                                                            */
/* Copyright (c) 2006-2007 Rexx Language Association. All rights reserved.    */
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
/*----------------------------------------------------------------------------*/
/* Class: MIMEPART                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class mimepart public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: MIMEPART                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method mimever     attribute private -- mime-version header
::method contenttype attribute private -- content-type header
::method encoding    attribute private -- content-transfer-encoding header
::method contentid   attribute private -- content-id header
::method contentdesc attribute private -- content-description header
::method contentdisp attribute private -- content-disposition header
::method content     attribute private -- the actual content


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: MIMEPART                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 1 then raise syntax 93.902 array (1)
self~mimever = '1.0'
if arg() = 1 then self~contenttype = arg(1)
else self~contenttype = 'text/plain'
self~encoding = ''
self~contentid = ''
self~contentdesc = ''
self~contentdisp = ''
self~content = ''
return


/*----------------------------------------------------------------------------*/
/* Method: getContentType                                                     */
/* Description: get the Content-Type header                                   */
/*----------------------------------------------------------------------------*/

::method getContentType
if arg() > 0 then raise syntax 93.902 array (0)
return self~contenttype


/*----------------------------------------------------------------------------*/
/* Method: setContentType                                                     */
/* Description: set the Content-Type header                                   */
/*----------------------------------------------------------------------------*/

::method setContentType
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~contenttype = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContentEncoding                                                 */
/* Description: get the Content-Transfer-Encoding header                      */
/*----------------------------------------------------------------------------*/

::method getContentEncoding
if arg() > 0 then raise syntax 93.902 array (0)
return self~encoding


/*----------------------------------------------------------------------------*/
/* Method: setContentEncoding                                                 */
/* Description: set the Content-Transfer-Encoding header                      */
/*----------------------------------------------------------------------------*/

::method setContentEncoding
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~encoding = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContentID                                                       */
/* Description: get the Content-ID header                                     */
/*----------------------------------------------------------------------------*/

::method getContentID
if arg() > 0 then raise syntax 93.902 array (0)
return self~contentid


/*----------------------------------------------------------------------------*/
/* Method: setContentID                                                       */
/* Description: set the Content-ID header                                     */
/*----------------------------------------------------------------------------*/

::method setContentID
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~contentid = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContentDesc                                                     */
/* Description: get the Content-Description header                            */
/*----------------------------------------------------------------------------*/

::method getContentDesc
if arg() > 0 then raise syntax 93.902 array (0)
return self~contentdesc


/*----------------------------------------------------------------------------*/
/* Method: setContentDesc                                                     */
/* Description: set the Content-Description header                            */
/*----------------------------------------------------------------------------*/

::method setContentDesc
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~contentdesc = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContentDisp                                                     */
/* Description: get the Content-Disposition header                            */
/*----------------------------------------------------------------------------*/

::method getContentDisp
if arg() > 0 then raise syntax 93.902 array (0)
return self~contentdisp


/*----------------------------------------------------------------------------*/
/* Method: setContentDisp                                                     */
/* Description: set the Content-Disposition header                            */
/*----------------------------------------------------------------------------*/

::method setContentDisp
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~contentdisp = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContent                                                         */
/* Description: get the content                                               */
/*----------------------------------------------------------------------------*/

::method getContent
if arg() > 0 then raise syntax 93.902 array (0)
return self~content


/*----------------------------------------------------------------------------*/
/* Method: addContent                                                         */
/* Description: add content to the mime part                                  */
/*----------------------------------------------------------------------------*/

::method addContent
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~content = self~content || arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getMimePart                                                        */
/* Description: returns the entire formatted mime message                     */
/*----------------------------------------------------------------------------*/

::method getMimePart
if arg() > 0 then raise syntax 93.902 array (0)
crlf = '0D0A'x
headers = 'MIME-Version:' || self~mimever || crlf
if self~contenttype <> '' then do
   headers = headers || 'Content-Type:' || self~contenttype || crlf
   end
if self~encoding <> '' then do
   headers = headers || 'Content-Transfer-Encoding:' || self~encoding || crlf
   end
if self~contentid <> '' then do
   headers = headers || 'Content-ID:' || self~contentid || crlf
   end
if self~contentdesc <> '' then do
   headers = headers || 'Content-Description:' || self~contentdesc || crlf
   end
if self~contentdisp <> '' then do
   headers = headers || 'Content-Disposition:' || self~contentdisp || crlf
   end
mime = headers || crlf || self~content || crlf
return mime


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: MIMEMULTIPART                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class mimemultipart public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: MIMEMULTIPART                                                       */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method mimever     attribute private -- mime-version header
::method contenttype attribute private -- content-type header
::method encoding    attribute private -- content-transfer-encoding header
::method contentid   attribute private -- content-id header
::method contentdesc attribute private -- content-description header
::method contentdisp attribute private -- content-disposition header
::method multiparts  attribute private -- each mime part


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: MIMEMULTIPART                                                       */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 0 then raise syntax 93.902 array (0)
self~mimever = '1.0'
self~contenttype = 'multipart/mixed'
self~encoding = ''
self~contentid = ''
self~contentdesc = ''
self~contentdisp = ''
self~multiparts = .array~new
return


/*----------------------------------------------------------------------------*/
/* Method: getContentType                                                     */
/* Description: get the Content-Type header                                   */
/*----------------------------------------------------------------------------*/

::method getContentType
if arg() > 0 then raise syntax 93.902 array (0)
return self~contenttype


/*----------------------------------------------------------------------------*/
/* Method: getContentEncoding                                                 */
/* Description: get the Content-Transfer-Encoding header                      */
/*----------------------------------------------------------------------------*/

::method getContentEncoding
if arg() > 0 then raise syntax 93.902 array (0)
return self~encoding


/*----------------------------------------------------------------------------*/
/* Method: setContentEncoding                                                 */
/* Description: set the Content-Transfer-Encoding header                      */
/*----------------------------------------------------------------------------*/

::method setContentEncoding
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~encoding = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContentID                                                       */
/* Description: get the Content-ID header                                     */
/*----------------------------------------------------------------------------*/

::method getContentID
if arg() > 0 then raise syntax 93.902 array (0)
return self~contentid


/*----------------------------------------------------------------------------*/
/* Method: setContentID                                                       */
/* Description: set the Content-ID header                                     */
/*----------------------------------------------------------------------------*/

::method setContentID
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~contentid = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContentDesc                                                     */
/* Description: get the Content-Description header                            */
/*----------------------------------------------------------------------------*/

::method getContentDesc
if arg() > 0 then raise syntax 93.902 array (0)
return self~contentdesc


/*----------------------------------------------------------------------------*/
/* Method: setContentDesc                                                     */
/* Description: set the Content-Description header                            */
/*----------------------------------------------------------------------------*/

::method setContentDesc
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~contentdesc = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getContentDisp                                                     */
/* Description: get the Content-Disposition header                            */
/*----------------------------------------------------------------------------*/

::method getContentDisp
if arg() > 0 then raise syntax 93.902 array (0)
return self~contentdisp


/*----------------------------------------------------------------------------*/
/* Method: setContentDisp                                                     */
/* Description: set the Content-Disposition header                            */
/*----------------------------------------------------------------------------*/

::method setContentDisp
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~contentdisp = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: addContent                                                         */
/* Description: add a mime part                                               */
/*----------------------------------------------------------------------------*/

::method addContent
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg part
if part~defaultname~translate~pos('MIMEPART') > 0 then do
   if self~multiparts~last = .nil then self~multiparts[1] = part~getmimepart()
   else self~multiparts[self~multiparts~last + 1] = part~getmimepart()
   end
else raise syntax 93.948 array('MIMEPART')
return


/*----------------------------------------------------------------------------*/
/* Method: getContent                                                         */
/* Description: get the content                                               */
/*----------------------------------------------------------------------------*/

::method getContent
if arg() > 0 then raise syntax 93.902 array (0)
if arg() > 0 then raise syntax 93.902 array (0)
crlf = '0D0A'x
headers = 'MIME-Version:' || self~mimever || crlf
boundary = '===ooRexx=Mime=Part===' || c2x(date('b') || time('L')) || '=='
self~contenttype = 'multipart/mixed; boundary="' || boundary || '"'
headers = headers || 'Content-Type:' || self~contenttype || crlf
if self~encoding <> '' then do
   headers = headers || 'Content-Transfer-Encoding:' || self~encoding || crlf
   end
if self~contentid <> '' then do
   headers = headers || 'Content-ID:' || self~contentid || crlf
   end
if self~contentdesc <> '' then do
   headers = headers || 'Content-Description:' || self~contentdesc || crlf
   end
if self~contentdisp <> '' then do
   headers = headers || 'Content-Disposition:' || self~contentdisp || crlf
   end
mime = headers || crlf
if self~multiparts~last \= .nil then do
   do x over self~multiparts
      mime = mime || '--' || boundary || crlf
      mime = mime || x || crlf
      end
   mime = mime || '--' || boundary || '--' || crlf
   end
return mime

