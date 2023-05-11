Call RxFuncAdd 'CURLLoadFuncs', 'rexxcurl','CurlLoadFuncs'
Call CURLLoadFuncs

-- change these as appropriate
email = 'your.gmail.account@gmail.com'
dest = 'yourdest@dest.com' -- email destination
password = 'hdtycvsrfsrsyegf' -- your gmail application password
server = 'smtp://smtp.gmail.com:587'

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
      in.1 = 'Subject: SMTP example from Rexx/CURL'
      in.2 = '' -- MUST be a blank line between headers and body
      in.3 = 'The body of the message starts here'
      in.4 = 'and can be multiple lines.'
      in.0  = 4
      Call CurlSetopt curl, 'INSTEM', 'in.', .endofline
      If curlerror.intcode \= 0 Then Call Abort 'Error setting INSTEM option', curl

      Call CurlSetopt curl, 'URL', server
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option', curl

      Call CurlSetopt curl, 'MAILFROM', '<'email'>' -- email address should be enclosed in angle brackets
      If curlerror.intcode \= 0 Then Call Abort 'Error setting MAILFROM option', curl

      Call CurlSetopt curl, 'USERNAME', email
      If curlerror.intcode \= 0 Then Call Abort 'Error setting USERNAME option', curl
      Call CurlSetopt curl, 'PASSWORD', password
      If curlerror.intcode \= 0 Then Call Abort 'Error setting PASSWORD option', curl

      Call CurlSetopt curl, 'USESSL', 'ALL';
      If curlerror.intcode \= 0 Then Call Abort 'Error setting USESSL option', curl

      Call CurlSetopt curl, 'SSLVERIFYHOST', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYHOST option', curl
      Call CurlSetopt curl, 'SSLVERIFYPEER', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYPEER option', curl

      rcpt.0 = 1
      rcpt.1 = '<'dest'>' -- email address should be enclosed in angle brackets
      Call CurlSetopt curl, 'MAILRCPT', 'rcpt.'
      If curlerror.intcode \= 0 Then Call Abort 'Error setting MAILRCPT option', curl
      Call CurlSetopt curl, 'UPLOAD', 1
      If curlerror.intcode \= 0 Then Call Abort 'Error setting MAILRCPT UPLOAD', curl

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

Abort: Procedure Expose curlerror.
Parse Arg msg, curl
Say msg
If curlerror.curlcode \= 0 Then Say 'cURL error:' curlerror.curlcode '-' curlerror.curlerrm
Else Say 'RexxCURL error:' curlerror.intcode '-' curlerror.interrm
Call CurlCLeanup curl
Call CurlDropFuncs 'UNLOAD'
Exit 1
