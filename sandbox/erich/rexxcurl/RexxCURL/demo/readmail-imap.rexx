Call RxFuncAdd 'CURLLoadFuncs', 'rexxcurl','CurlLoadFuncs'
Call CURLLoadFuncs

-- change these as appropriate
url = 'imaps://imap.gmail.com:993/INBOX'
email = 'grfb.pages@gmail.com'

Say 'Please enter password for' email':'
Parse Pull password
-- check if our version of Rexx/CURL supports IMAP
haveimap = 0
protocols = ''
Do i = 1 to !REXXCURL.!PROTOCOLS.0
   protocols = protocols !REXXCURL.!PROTOCOLS.i
   If !REXXCURL.!PROTOCOLS.i = 'imap' Then
      Do
         haveimap = 1
         Leave
      End
End
-- check if our version of Rexx/CURL support pop3
If haveimap = 0 Then
   Do
      Say 'You cannot run this demo program as your version of Rexx/CURL does not support IMAP. Supported protocols are:' protocols
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

      Call CurlSetopt curl, 'VERBOSE', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting VERBOSE option', curl

      Call CurlSetopt curl, 'URL', url
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option', curl

      Call CurlSetopt curl, 'USERNAME', email
      If curlerror.intcode \= 0 Then Call Abort 'Error setting USERNAME option', curl

      Call CurlSetopt curl, 'PASSWORD', password
      If curlerror.intcode \= 0 Then Call Abort 'Error setting PASSWORD option', curl
      -- save the list of emails into a stem
      Call CurlSetopt curl, 'OUTSTEM', 'mail.', '0D0A'x
      If curlerror.intcode \= 0 Then Call Abort 'Error setting OUTSTEM option', curl
      -- get em
      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error getting source of' url, curl
      Do i = 1 To 6 -- mail.0
         Drop item.
         in_header = 1
         fn = 'mail.'i'.txt'
         Call Stream fn, 'C', 'OPEN WRITE REPLACE'
         Say
         Say Copies( '*', 30 )
         Call CurlSetopt curl, 'URL', url'/'i
         If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option', curl
         Call CurlSetopt curl, 'OUTSTEM', 'item.', '0D0A'x
         If curlerror.intcode \= 0 Then Call Abort 'Error setting OUTSTEM option', curl
         -- get the contents of our email
         Call CurlPerform curl
         If curlerror.intcode \= 0 Then Call Abort 'Error getting source of' url, curl
         Do j = 1 To item.0
            -- first blank line is end of headers
            If Strip( item.j ) = '' Then in_header = 0
            -- save the line contents to our file
            Call Lineout fn, item.j
            -- for each line in the email message, check for special headers and display them
            If in_header & ( Left( item.j, 5 ) = 'From:' | Left( item.j, 8 ) = 'Subject:' | Left( item.j, 5 ) = 'Date:' ) Then Say item.j
         End
         Call Stream fn, 'C', 'CLOSE'
      End
      /*
       * Cleanup the connection
       */
      Call CurlCLeanup curl
   End
Call CurlDropFuncs 'UNLOAD'
Return 0

Abort: Procedure Expose curlerror.
Parse Arg msg, curl
Say msg
If curlerror.curlcode \= 0 Then Say 'cURL error:' curlerror.curlcode '-' curlerror.curlerrm
Else Say 'RexxCURL error:' curlerror.intcode '-' curlerror.interrm
Call CurlCLeanup curl
Call CurlDropFuncs 'UNLOAD'
Exit 1
