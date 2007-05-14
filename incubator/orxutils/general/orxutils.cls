/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: ooRexx utility class.                                         */
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
/*----------------------------------------------------------------------------*/
/* Class: ORXUTILS                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class orxutils public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ORXUTILS                                                            */
/*        Public class methods                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: buffer2array                                                       */
/* Description: Convert a string buffer containing LFs to an array of lines.  */
/*----------------------------------------------------------------------------*/

::method buffer2array class
if arg() > 1 then raise syntax 93.902 array (1)
instr = arg(1)
buflines = .array~new
/* try to be intelligent about what is being used as an EOL marker */
if instr~pos('0D0A'x) > 0 then nl = '0D0A'x
else nl = '0A'x
do while instr~length > 0
   parse var instr nextline (nl) instr
   buflines[buflines~items + 1] = nextline
   end
return buflines


/*----------------------------------------------------------------------------*/
/* Method: printf                                                             */
/* Description: Similar to the C printf function except that it returns the   */
/*              formatted string. The formatted string is NOT sent to stdout. */
/*----------------------------------------------------------------------------*/

::method printf class
numargs = arg()
if numargs < 1 then raise syntax 93.901 array (1)
fmtstr = escape_seq(arg(1))
outstr = ''
argctr = 1
x = pos('%', fmtstr)
do while x > 0
   /* get the prefix string to the format escape char */
   if x > 1 then outstr = outstr || substr(fmtstr, 1, x - 1)
   fmtstr = substr(fmtstr, x + 1)
   /* get the before and after lengths */
   before = ''
   do while pos(left(fmtstr, 1), '0123456789.') > 0
      before = before || left(fmtstr, 1)
      fmtstr = substr(fmtstr, 2)
      end
   parse var before before '.' after
   /* now get the format specifier */
   specifier = left(fmtstr, 1)
   fmtstr = substr(fmtstr, 2)
   /* now we can format the next argument */
   select
      when specifier = '%' then do
         if before <> '' then raise syntax 40.29 array ('PRINTF', '%' || before'%')
         outstr = outstr || '%'
         end
      when specifier = 's' then do
         argctr = argctr + 1
         if argctr > numargs then raise syntax 40.3 array ('PRINTF', argctr)
         str = arg(argctr)
         if before = '' then before = length(str)
         if after = '' then outstr = outstr || left(arg(argctr), before)
         else do
            if after < before then ,
             raise syntax 40.29 array ('PRINTF', '%' || before'.'after's')
            outstr = outstr || ,
             left(str, min(max(before, length(str)), after))
            end
         end
      when specifier = 'd' then do
         argctr = argctr + 1
         if argctr > numargs then raise syntax 40.3 array ('PRINTF', argctr)
         str = arg(argctr)
         if before <> '' & after <> '' then,
          outstr = outstr || format(str, before, after)
         else if before <> '' then,
          outstr = outstr || format(str, before)
         else outstr = outstr || format(str)
         end
      when specifier = 'x' then do
         argctr = argctr + 1
         if argctr > numargs then raise syntax 40.3 array ('PRINTF', argctr)
         str = arg(argctr)
         if before <> '' then raise syntax 40.29 array ('PRINTF', '%' || before'x')
         outstr = outstr || c2x(str)
         end
      otherwise do
         raise syntax 40.29 array ('PRINTF', specifier)
         end
      end
   x = pos('%', fmtstr)
   end
outstr = outstr || fmtstr
return outstr


escape_seq: procedure
numargs = arg()
if numargs < 1 then raise syntax 93.901 array (1)
fmtstr = arg(1)
outstr = ''
parse source os .
if pos('Windows', os) > 0 | os = 'OS2' then nl = '0D0A'x
else nl = '0A'x
x = pos('\', fmtstr)
do while x > 0
   /* get the prefix string to the escape char */
   if x > 1 then outstr = outstr || substr(fmtstr, 1, x - 1)
   fmtstr = substr(fmtstr, x + 1)
   /* get the escape character */
   specifier = left(fmtstr, 1)
   fmtstr = substr(fmtstr, 2)
   /* now we can interpret the escape character */
   select
      when specifier = '\' then outstr = outstr || '\'
      when specifier = 'a' then outstr = outstr || '07'x
      when specifier = 'f' then outstr = outstr || '0C'x
      when specifier = 'n' then outstr = outstr || nl
      when specifier = 'r' then outstr = outstr || '0D'x
      when specifier = 't' then outstr = outstr || '09'x
      when specifier = 'v' then outstr = outstr || '0B'x
      otherwise do
         raise syntax 40.29 array ('ESCAPE_SEQ', specifier)
         end
      end
   x = pos('\', fmtstr)
   end
outstr = outstr || fmtstr
return outstr


/*----------------------------------------------------------------------------*/
/* Method: httpget                                                            */
/* Description: Returns a buffer containing the response to the input URI.    */
/*----------------------------------------------------------------------------*/


::method httpget class
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 3 then raise syntax 93.902 array (3)
use arg uri, userid, password
if uri = '' then raise syntax 93.911 array(1)
if userid <> '' & password = '' then raise syntax 93.911 array (3)
if substr(uri, 1, 6) = 'https:' then raise syntax 93.914 array (2, 'http://', 'https')
if substr(uri, 1, 7) <> 'http://' then raise syntax 93.914 array (2, 'http://', uri)
uri = substr(uri, 8)
x = pos(':', uri)
y = pos('/', uri)
if x > 0 then do
   server = substr(uri, 1, x - 1)
   port = substr(uri, x + 1, y - x - 1)
   end
else do
   server = substr(uri, 1, y - 1)
   port = 80
   end
url = substr(uri, y)
if port = 443 then return 'Error: port 443 not supported.'
/* load the ooRexx socket library */
if rxfuncquery('SockDropFuncs') then do
   call rxfuncadd 'SockLoadFuncs', 'rxsock', 'SockLoadFuncs'
   call SockLoadFuncs
   end

/* perform the transaction */
response = httpget(server, port, url)

/* check to see if we got a challenge response */
if substr(response, 9, 5) = ' 401 ' & userid <> '' &,
 pos('Authenticate: Basic', response) > 0 then do
   /* perform the transaction with authentication */
   response = httpget(server, port, url, userid, password)
   end

return response


httpget: procedure

if arg() > 3 then do
   use arg server, port, url, username, password
   auth = .orxutils~encodebase64(username || ':' || password)
   end
else do
   use arg server, port, url
   auth = ''
   end

/* create a TCP socket */
socket = SockSocket('AF_INET', 'SOCK_STREAM', '0')
if socket < 0 then return 'Error: Unable to create socket'

/* resolve server name alias to dotted IP address */
retc = SockGetHostByName(server, 'host.!')
if retc = 0 then return 'Error: Unable to resolve server:' server

/* connect the new socket to the specified server */
host.!family = "AF_INET"
host.!port = port
retc = SockConnect(socket, 'host.!')
if retc < 0 then do
   call SockShutDown socket, 2
   call SockClose socket
   return 'Error: Unable to connect to server:' server
   end

/* create the HTTP command to send */
command = 'GET' url 'HTTP/1.1' || '0D0A'x
command = command || 'Host:' server || '0D0A'x
if auth <> '' then do
   command = command || 'Authorization: Basic' auth || '0D0A'x
   end
command = command || 'User-Agent: httpget.rex/1.0' || '0D0A'x
command = command || 'Connection: close' || '0D0A'x || '0D0A'x

/* send/receive HTTP transaction */
response = transactsocket(socket, command)

/* we are done with the socket */
call SockShutDown socket, 2
call SockClose socket

return response


transactsocket: procedure

parse arg socket, command

/* send the command */
bytessent = SockSend(socket, command)

/* get the response */
response = ''
do forever
   bytesrcvd = SockRecv(socket, 'rcvdata', 4096)
   if bytesrcvd <= 0 then leave
   response = response || rcvdata
   end
return response

