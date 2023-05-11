Call RxFuncAdd 'CURLLoadFuncs', 'rexxcurl','CurlLoadFuncs'
Call CURLLoadFuncs
Call RxFuncAdd 'EECLoadFuncs', 'rexxeec','EECLoadFuncs'
Call EECLoadFuncs

-- change these as appropriate
!email = 'rexx@internode.on.net'
!dest = 'rexx.org@internode.on.net' -- email destination
!bcc = 'm.hessling@qut.edu.au'
!password = 'dinkal801' -- your application password
!subject = 'This is the subject line'
!filename = 'attachment.pdf'
!mailserver = 'mail.internode.on.net'
!mailport = 465

-- check if our version of Rexx/CURL support SMTP
havesmtp = 0
protocols = ''
Do i = 1 to !REXXCURL.!PROTOCOLS.0
   protocols = protocols !REXXCURL.!PROTOCOLS.i
   If !REXXCURL.!PROTOCOLS.i = 'smtp' Then
      Do
         havesmtp = 1
         Leave
      End
End
-- check if our version of Rexx/CURL support SMTP
If havesmtp = 0 Then
   Do
      Say 'You cannot run this demo program as your version of Rexx/CURL does not support SMTP. Supported protocols are:' protocols
      Exit 1
   End
-- check if our version of Rexx/CURL support SSL
If !REXXCURL.!SUPPORTS_SSL = 0 Then
   Do
      Say 'You cannot run this demo program as your version of Rexx/CURL does not support SSL'
      Exit 1
   End
boundary = c2x( Time( 'T' ) )
curl = CurlInit()
If curl \= '' Then
   Do
      msg = 'We are running' CurlVariable('VERSION')
      Say Copies( '*', Length( msg ) )
      Say msg
      Say Copies( '*', Length( msg ) )
      Say

      Call CurlSetopt curl, 'VERBOSE', 1
      If curlerror.intcode \= 0 Then Call Abort 'Error setting VERBOSE option', curl
      -- setup our headers and email body
      in.1 = 'From: RexxCURL <'!email'>'
      in.2 = 'To:' !dest
      in.3 = 'Subject: SMTP example from Rexx/CURL'
      in.4 = 'MIME-Version: 1.0'
--    in.5 = 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:31.0) Gecko/20100101 Thunderbird/31.2.0'
      in.5 = 'Bcc:' !bcc
      in.6 = 'Content-Type: multipart/mixed;'
      in.7 = ' boundary="'boundary'"'
      in.8 = '' -- MUST be a blank line between headers and body
      in.9 = 'This is a multi-part message in MIME format.'
      in.10 = '--'boundary
      in.11 = 'Content-Type: text/plain; charset=utf-8; format=flowed'
      in.12 = 'Content-Transfer-Encoding: 7bit'
      in.13 = '' -- MUST be a blank line between headers and body
      in.14 = 'The body of the message starts here'
      in.15 = 'and can be multiple lines.'
      in.16= '' -- MUST be a blank line between headers and body
      in.17 = '--'boundary
      in.18 = 'Content-Type: application/pdf;'
      in.19 = ' name="'!filename'"'
      in.20 = 'Content-Transfer-Encoding: base64'
      in.21 = 'Content-Disposition: attachment;'
      in.22 = ' filename="'!filename'"'
      in.23 = '' -- MUST be a blank line between headers and body
      in.0  = 23
      in.0 = EncodeAttachment( in.0, !filename )
      idx = in.0
      idx = idx + 1
      in.idx = '--'boundary'--'
      in.0 = idx

      Call CurlSetopt curl, 'INSTEM', 'in.', .endofline
      If curlerror.intcode \= 0 Then Call Abort 'Error setting INSTEM option', curl

      Call CurlSetopt curl, 'URL', 'smtps://'!mailserver':'!mailport
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option', curl

      Call CurlSetopt curl, 'MAILFROM', '<'!email'>' -- email address should be enclosed in angle brackets
      If curlerror.intcode \= 0 Then Call Abort 'Error setting MAILFROM option', curl

      Call CurlSetopt curl, 'USERNAME', !email
      If curlerror.intcode \= 0 Then Call Abort 'Error setting USERNAME option', curl
      Call CurlSetopt curl, 'PASSWORD', !password
      If curlerror.intcode \= 0 Then Call Abort 'Error setting PASSWORD option', curl

      Call CurlSetopt curl, 'USESSL', 'ALL';
      If curlerror.intcode \= 0 Then Call Abort 'Error setting USESSL option', curl

      Call CurlSetopt curl, 'SSLVERIFYHOST', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYHOST option', curl
      Call CurlSetopt curl, 'SSLVERIFYPEER', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYPEER option', curl

      rcpt.0 = 1
      rcpt.1 = '<'!dest'>' -- email address should be enclosed in angle brackets
      Call CurlSetopt curl, 'MAILRCPT', 'rcpt.'
      If curlerror.intcode \= 0 Then Call Abort 'Error setting MAILRCPT option', curl

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error getting source of' url, curl
      /*
       * Cleanup the connection
       */
      Call CurlCLeanup curl
   End
Call CurlDropFuncs 'UNLOAD'
Return 0

/* creates a date suitable for a message header */
getdate: procedure
date = Left(Date('W'),3)',' date('N') time('N')
offset = time('O') / 36000000
If offset > 0 Then sign = '+'
Return date sign || offset

EncodeAttachment: Procedure Expose in. eecerror.
Parse Arg idx, filename
cs = 72
-- check if file exists
size = Stream( filename, 'C', 'QUERY SIZE' )
blob = eecencode( Charin( filename, 1, size ), 'BASE64' )
len = Length( blob )
chunks = len % cs
rem = len // cs
start = 1
Do chunks
   line = Substr( blob, start, cs )
   start = start + cs
   idx = idx + 1
   in.idx = line
End
If rem > 0 Then
   Do
      line = Substr( blob, start )
      idx = idx + 1
      in.idx = line
   End
Return idx

Abort: Procedure Expose curlerror.
Parse Arg msg, curl
Say msg
If curlerror.curlcode \= 0 Then Say 'cURL error:' curlerror.curlcode '-' curlerror.curlerrm
Else Say 'RexxCURL error:' curlerror.intcode '-' curlerror.interrm
Call CurlCLeanup curl
Call CurlDropFuncs 'UNLOAD'
Exit 1
