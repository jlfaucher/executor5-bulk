/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Classes to support SMTP.                                      */
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


::requires 'mime.cls'
::requires 'strmsock.cls'


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: SMTPMSG                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class smtpmsg public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: SMTPMSG                                                             */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method content     attribute private -- content of the message
::method from        attribute private -- from header
::method recipients  attribute private -- recipients header
::method subject     attribute private -- subject header


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: SMTPMSG                                                             */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 0 then raise syntax 93.902 array (0)
self~content = ''
self~from = ''
self~recipients = .array~new
self~subject = ''
return


/*----------------------------------------------------------------------------*/
/* Method: getContent                                                         */
/* Description: get the content of the message                                */
/*----------------------------------------------------------------------------*/

::method getContent
if arg() > 0 then raise syntax 93.902 array (0)
return self~content


/*----------------------------------------------------------------------------*/
/* Method: addContent                                                         */
/* Description: add content to the message                                    */
/*----------------------------------------------------------------------------*/

::method addContent
if arg() < 1 then raise syntax 93.902 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg part
self~content = part~getcontent
return


/*----------------------------------------------------------------------------*/
/* Method: getFrom                                                            */
/* Description: get the from header                                           */
/*----------------------------------------------------------------------------*/

::method getFrom
if arg() > 0 then raise syntax 93.902 array (0)
return self~from

/*----------------------------------------------------------------------------*/
/* Method: setFrom                                                            */
/* Description: set the from header                                           */
/*----------------------------------------------------------------------------*/

::method setFrom
if arg() < 1 then raise syntax 93.902 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~from = arg(1)
return

/*----------------------------------------------------------------------------*/
/* Method: CntRecipients                                                      */
/* Description: get the number of recipients                                  */
/*----------------------------------------------------------------------------*/

::method CntRecipients
cnt_rcpt = self~recipients~items
return cnt_rcpt

/*----------------------------------------------------------------------------*/
/* Method: getRcptRecipients                                                  */
/* Description: get the list of recipients                                    */
/*----------------------------------------------------------------------------*/

::method getRcptRecipients
use arg rcpt_seq
max_cnt = self~recipients~items
if rcpt_seq > maxcnt then return
rcpt_data = self~recipients[rcpt_seq]
return rcpt_data


/*----------------------------------------------------------------------------*/
/* Method: getRecipients                                                      */
/* Description: get the list of recipients                                    */
/*----------------------------------------------------------------------------*/

::method getRecipients
if arg() > 0 then raise syntax 93.902 array (0)
recplist = ''
linelen = 3 -- add space for the 'To:' prefix
pad = '   '
do recp over self~recipients
   if recplist = '' then do
      recplist = recp
      linelen = linelen + recp~length
      end
   else do
      recplist = recplist',' recp
      linelen = linelen + recp~length + 2
      end
   -- the smtp protocol limits header lines to 1000 chars, we will use less
   if linelen > 80 then do
      recplist = recplist || '0D0A'x || pad
      linelen = pad~length
      end
   end
return recplist


/*----------------------------------------------------------------------------*/
/* Method: addRecipient                                                       */
/* Description: add a new reciepient                                          */
/*----------------------------------------------------------------------------*/

::method addRecipient
if arg() < 1 then raise syntax 93.902 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~recipients[self~recipients~items + 1] = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: getSubject                                                         */
/* Description: get the subject header                                        */
/*----------------------------------------------------------------------------*/

::method getSubject
if arg() > 0 then raise syntax 93.902 array (0)
return self~subject


/*----------------------------------------------------------------------------*/
/* Method: setSubject                                                         */
/* Description: set the subject header                                        */
/*----------------------------------------------------------------------------*/

::method setSubject
if arg() < 1 then raise syntax 93.902 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~subject = arg(1)
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: SMTP                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class smtp public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: SMTP                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method authtypes  attribute private -- authorization types, if any
::method strmsock   attribute private -- stream socket class instance
::method data       attribute private -- unparsed received data
::method lhost      attribute private -- local host ip address
::method rcport     attribute private -- remote SMTP port
::method resp       attribute private -- raw unparsed command response buffer
::method rhost      attribute private -- remote SMTP host name/ip address


/*----------------------------------------------------------------------------*/
/* Method: transactsock                                                       */
/* Description: perform a single socket transaction & check return code       */
/*----------------------------------------------------------------------------*/

::method strmtransact private
use arg cmd, respcode
self~cmdresponse[self~cmdresponse~items + 1] = cmd
retc = self~strmsock~lineout(cmd)
if retc = 1 then return .true
/* get the response, possibly multiple lines */
self~cmdresponse[self~cmdresponse~items + 1] = self~strmsock~linein()
startresp = self~cmdresponse~items
do while self~cmdresponse[self~cmdresponse~items]~substr(4, 1) = '-'
   self~cmdresponse[self~cmdresponse~items + 1] = self~strmsock~linein()
   end
/* check the response & return .false if we find an expected return code */
self~smtperrno = self~cmdresponse[self~cmdresponse~items]~substr(1, 3)
if respcode = '*' then return .false
parse var respcode r1 respcode
do while r1 <> ''
   /* per the RFC 959 only check the first digit of the return code */
   if r1~substr(1, 1) = self~smtperrno~substr(1, 1) then return .false
   parse var respcode r1 respcode
   end
/* we found an unexpected return code */
do i = startresp to self~cmdresponse~items
   self~debugsay('StrmSock: unexpected response:' self~cmdresponse[i])
   end
return .true


/*----------------------------------------------------------------------------*/
/* Method: auth_login                                                         */
/* Description: login using plain text login authentication                   */
/*----------------------------------------------------------------------------*/

::method auth_login private
retc = self~strmtransact('AUTH LOGIN', '3')
if retc = .true then do
   self~debugsay('Error: AUTH LOGIN command to' self~rhost 'failed.')
   return -1
   end
retc = self~strmtransact(self~authid~encodebase64, '3')
if retc = .true then do
   self~debugsay('Error: AUTH LOGIN command to' self~rhost 'failed.')
   return -1
   end
retc = self~strmtransact(self~password~encodebase64, '2')
if retc = .true then do
   self~debugsay('Error: AUTH LOGIN command to' self~rhost 'failed.')
   return -1
   end
return 0


/*----------------------------------------------------------------------------*/
/* Method: debugsay                                                           */
/* Description: say the message if debug = .true                              */
/*----------------------------------------------------------------------------*/

::method debugsay private unguarded
/* any value other than .false activates debug messages */
if self~debug <> .false then say arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: setdefaults                                                        */
/* Description: setup/reset all the defaults for the class                    */
/*----------------------------------------------------------------------------*/

::method setdefaults private
self~authid = ''                 -- authentication id
self~authtypes = ''              -- authorization types
self~cmdresponse = .array~new()  -- all command/status responses from the server
self~debug = .false              -- .false suppresses debugging information
self~password = ''               -- account password on the smtp server
self~rcport = 25                 -- default remote command port
self~resp = ''                   -- raw unparsed command socket response buffer
self~response = .nil             -- parsed last command response from the server
self~rhost = ''                  -- host name or ip address of the smtp server
self~smtperrno = ''              -- last smtp response code
self~strmsock = .socket~new()    -- get a stream socket instance
self~localhost = self~strmsock~gethostbyaddr('127.0.0.1') -- get our hostname
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: SMTP                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method authid      attribute -- authentication id (username@xxx.com)
::method cmdresponse attribute -- all parsed commands and responses
::method debug       attribute -- .false suppresses debug messages
::method localhost   attribute -- our local host name
::method password    attribute -- remote SMTP password
::method response    attribute -- parsed command response from last command
::method smtperrno   attribute -- last smtp response code

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 0 then raise syntax 93.902 array (0)
self~setdefaults
self~strmsock = .socket~new
return


/*----------------------------------------------------------------------------*/
/* Method: setLocalhost                                                       */
/* Description: set the local host name                                       */
/*----------------------------------------------------------------------------*/

::method setLocalHost
if arg() < 1 then raise syntax 93.902 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
self~localhost = arg(1)
return


/*----------------------------------------------------------------------------*/
/* Method: Connect                                                            */
/* Description: set up the primary connection to the smtp server and logon    */
/* Arguments:                                                                 */
/*      remote_host                                                           */
/*      authid      (optional)                                                */
/*      password    (optional)                                                */
/*----------------------------------------------------------------------------*/

::method Connect
self~response = .array~new()
self~smtperrno = ''
/* check the arguments */
if arg() > 3 then raise syntax 93.902 array (3)
/* a authid without a password is useless */
if arg() = 2 then raise syntax 93.903 array (4)
if arg() < 1 then raise syntax 93.901 array (1)
select
   when arg() = 1 then do
      use arg thost
      tport = 25
      tauth = ''
      tpassword = ''
      end
   when arg() = 3 then do
      use arg thost, tauth, tpassword
      end
   otherwise nop
   end
self~rhost = thost
self~authid = tauth
self~password = tpassword
/* check to see if an alternate port was specified */
hostname = self~rhost
parse var hostname hostname ':' tport
if tport <> '' then do
   self~rhost = hostname
   if tport~datatype('W') = 0 then do
      return -1
      end
   self~rcport = tport
   end
retc = self~strmsock~open(self~rhost, self~rcport)
if retc = .true then return -1
/* get the header message from the server */
self~debugsay('waiting on the header from' self~rhost)
/* get the response, possibly multiple lines */
self~cmdresponse[self~cmdresponse~items + 1] = self~strmsock~linein()
startresp = self~cmdresponse~items
do while self~cmdresponse[self~cmdresponse~items]~substr(4, 1) = '-'
   self~cmdresponse[self~cmdresponse~items + 1] = self~strmsock~linein()
   end
/* get the capabilities of the server */
retc = self~strmtransact('EHLO' self~localhost, '2')
if retc = .true then return -1
/* get the authorization capability */
do i = 1 to self~cmdresponse~items
   self~debugsay(self~cmdresponse[i])
   if self~cmdresponse[i]~substr(5, 4) = 'AUTH' then do
      self~authtypes = self~cmdresponse[i]~substr(5)
      leave
      end
   end
/* if necessary, login */
select
   /* select the authorization method to use to login */
   when self~authtypes~pos('LOGIN') > 0 then retc = self~auth_login()
   otherwise retc = 0
   end
return retc


/*----------------------------------------------------------------------------*/
/* Method: Logoff                                                             */
/* Description: logoff the smtp server                                        */
/*----------------------------------------------------------------------------*/

::method Logoff
self~response = .array~new()
/* check/get args */
if arg() > 0 then raise syntax 93.902 array (0)
/* log the user off */
retc = self~strmsock~lineout('QUIT')
/* shutdown the socket */
self~strmsock~close
/* reset all the defaults */
self~setdefaults
return 0


/*----------------------------------------------------------------------------*/
/* Method: Send                                                               */
/* Description: Send the mail message                                         */
/*----------------------------------------------------------------------------*/

::method Send
if arg() < 1 then raise syntax 93.902 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg msg
if msg~defaultname~translate~pos('SMTPMSG') = 0 then ,
 raise syntax 93.948 array (1, 'SMTPMsg')
self~response = .array~new()
/* send the 'To:' header */
retc = self~strmtransact('MAIL FROM:'msg~getFrom, '2')
if retc = .true then do
   self~debugsay('Error: MAIL FROM command to' self~rhost 'failed.')
   return -1
   end
/* send the 'From:' header */
rcpt_count = msg~CntRecipients
do cnt2 = 1 to rcpt_count
   torcpt = msg~getRcptRecipients(cnt2)
   tofield = 'RCPT TO: ' || torcpt || '  '
   retc = self~strmtransact(tofield, '2')
   if retc = .true then do
      self~debugsay('Error: RCPT TO command to' self~rhost 'failed.')
      return -1
      end
   end
/* send the message body */
retc = self~strmtransact('DATA', '3')
if retc = .true then do
   self~debugsay('Error: DATA command to' self~rhost 'failed.')
   return -1
   end
retc = self~strmsock~lineout('From:'msg~getFrom)
retc = self~strmsock~lineout('To:'msg~getRecipients)
retc = self~strmsock~lineout('Subject:'msg~getSubject)
retc = self~strmsock~lineout(msg~getContent)
retc = self~strmsock~lineout('.')
/* get and check the response */
self~cmdresponse[self~cmdresponse~items + 1] = self~strmsock~linein()
startresp = self~cmdresponse~items
do while self~cmdresponse[self~cmdresponse~items]~substr(4, 1) = '-'
   self~cmdresponse[self~cmdresponse~items + 1] = self~strmsock~linein()
   end
if self~cmdresponse[self~cmdresponse~items]~substr(1, 1) \= '2' then do
   self~debugsay('Error: DATA command to' self~rhost 'failed.')
   return -1
   end
return 0

