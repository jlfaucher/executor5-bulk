/*
 * Rexx/CURL sample program to execute a HTTP POST to a web server.
 * This example, subscribes the user to the rexxcurl-users mailing
 * list at SourceForge.
 * Pass the email address and password on the command line.
 */

Call RxFuncAdd 'CurlLoadFuncs', 'rexxcurl', 'CurlLoadFuncs'
Call CurlLoadFuncs

Parse Arg cmd
If cmd = '' Then
   Do
      Say 'No BIF supplied'
      Exit 1
   End

site = 'http://www.rexxla.org/cgi/ANSI/ansibifs.rex'
curl = CurlInit()
If curl \= '' Then
   Do
      Call CurlSetopt curl, 'HTTPPOST', 1
      field.1 = 'digits='9
      field.2 = 'fuzz='0
      field.3 = 'numform=SCIENTIFIC'
      field.4 = 'cmd='cmd
      field.0 = 4

      Call CurlSetopt curl, 'HTTPPOSTFIELDS', 'field.'
      If curlerror.intcode \= 0 Then Call Abort 'Error setting HTTPPOSTFIELDS option'
      Call CurlSetopt curl, 'OUTSTEM', 'response.', '0A'x

      Call CurlSetopt curl, 'URL', site
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option'

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error POSTing to' site
      /*
       * Check that the submission worked
       */
      dump = 0
      Say 'Results of "'cmd'" are:'
      Do i = 1 To response.0
         Select
            When dump = 0 & Left( response.i, 5 ) = '<PRE>' Then dump = 1
            When dump = 1 & Left( response.i, 6 ) = '</PRE>' Then Leave i
            When dump = 1 Then Say response.i
            Otherwise Nop
         End
      End
      Say '(ignore dotted lines and leading/trailing square brackets around result)'
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
