/*
 * Rexx/CURL sample program to connect to a web server and display
 * the source of the page specified by the supplied URL.
 * Only argument is a URL as in http://rexxcurl.sourceforge.net
 */
Call RxFuncAdd 'CurlLoadFuncs', 'rexxcurl', 'CurlLoadFuncs'
If result > 10 Then
   Do
      Parse Version ver
      If Left( ver, 11 ) = 'REXX-Regina' Then extra = RxFuncErrMsg()
      Else extra = ''
      Say 'Unable to load CurlLoadFuncs, result:' result extra
      Exit 1
   End
Call CurlLoadFuncs

Parse Arg url
If url = '' Then
   Do
      Say 'No URL supplied'
      Exit 1
   End
protocols = 'Protocols:'
Do i = 1 To !REXXCURL.!PROTOCOLS.0
   protocols = protocols !REXXCURL.!PROTOCOLS.i
End

curl = CurlInit()
If curl \= '' Then
   Do
      msg = 'We are running' CurlVariable('VERSION')
      len = Max( Length( msg ), Length( protocols ) )
      Say Copies( '*', len )
      Say msg
      Say protocols
      Say Copies( '*', len )
      Say

      Call CurlSetopt curl, 'URL', url
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option', curl

      Call CurlSetopt curl, 'FOLLOWLOCATION', 1
      If curlerror.intcode \= 0 Then Call Abort 'Error setting FOLLOWLOCATION option', curl

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error getting source of' url, curl
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
