/*
 * Rexx/CURL sample program to execute a HTTP POST to a web server.
 * This example, subscribes the user to the rexxcurl-users mailing
 * list at SourceForge.
 * Pass the email address and password on the command line.
 */

Call RxFuncAdd 'CurlLoadFuncs', 'rexxcurl', 'CurlLoadFuncs'
Call CurlLoadFuncs

site = 'http://centos-65-amd64/paddleed/test.php'
username = 'markhessling'
password = 'pfmoe09'

curl = CurlInit()
If curl \= '' Then
   Do
      Say 'We are running' CurlVariable('VERSION')

      Call CurlSetopt curl, 'HTTPPOST', 1
      field.1 = 'username='username
      field.2 = 'password='password
      field.3 = 'version=PaddleEd-MacOS-1.1.3'
      field.4 = 'data=[{"acId":0,"firstName":"Fred","surname":"Bloggs","qualId":153},{"acId":"undefined","firstName":"Mary","surname":"Bloggs","qualId":153},{"acId":0,"firstName":"Donald","surname":"Bloggs","qualId":153}]'
      field.0 = 4
      Call CurlSetopt curl, 'HTTPPOSTFIELDS', 'field.'
      Call CurlSetopt curl, 'OUTSTEM', 'response.', '0A'x

      Call CurlSetopt curl, 'URL', site
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option'

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error POSTing to' site
      /*
       * Check that the submission worked
       */
      ok = 0
      Do i = 1 To response.0
         Say response.i
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
