/*
 * Rexx/CURL sample program to execute a HTTP POST to a web server.
 * This example, subscribes the user to the rexxcurl-users mailing
 * list at SourceForge.
 * Pass the email address and password on the command line.
 */

Call RxFuncAdd 'CurlLoadFuncs', 'rexxcurl', 'CurlLoadFuncs'
Call CurlLoadFuncs

site = 'http://192.168.178.78/bin/response.scgi'

curl = CurlInit()
If curl \= '' Then
   Do
      Call CurlSetopt curl, 'HTTPPOST', 1
      field.1 = 'tid=0'
      field.2 = 'id=02'
      field.3 = 'user=999999'
      field.4 = 'password=998grfb998'
      field.0 = 4

      Call CurlSetopt curl, 'VERBOSE', 0
      Call CurlSetopt curl, 'HTTPPOSTFIELDS', 'field.'
      Call CurlSetopt curl, 'OUTSTEM', 'response.', x'0D0A'

      Call CurlSetopt curl, 'URL', site
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option'

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error POSTing to' site
      /*
       * Check that the submission worked
       */
      Do i = 1 To response.0
         say response.i
      End
      -- try another
      field.2 = 'id=01'
      field.5 = 'eta=2303'
      field.0 = 5
      Call CurlSetopt curl, 'HTTPPOSTFIELDS', 'field.'
      Call CurlSetopt curl, 'OUTSTEM', 'response.', x'0D0A'
      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error POSTing to' site
      Do i = 1 To response.0
         say response.i
      End
      /*
       * Cleanup the connection
       */
      Call CurlCLeanup curl
   End
Call CurlDropFuncs 'UNLOAD'
Return 0

Abort: Procedure Expose curlerror.
Parse Arg msg
Say msg
If curlerror.curlcode \= 0 Then Say 'cURL error:' curlerror.curlcode '-' curlerror.curlerrm
Else Say 'RexxCURL error:' curlerror.intcode '-' curlerror.interrm
Call CurlCLeanup curl
Call CurlDropFuncs 'UNLOAD'
Exit 1
