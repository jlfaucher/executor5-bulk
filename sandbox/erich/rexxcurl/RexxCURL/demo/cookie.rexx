/*
 * Rexx/CURL sample program to connect to a web server and save and display
 * any cookies the server sets.
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
If url = '' Then Call Abort 'No URL supplied'

curl = CurlInit()
If curl \= '' Then
   Do
      msg = 'We are running' CurlVariable('VERSION')
      Say Copies( '*', Length( msg ) )
      Say msg
      Say Copies( '*', Length( msg ) )
      Say

      r = CurlVariable('DEBUG', 0)
      Call CurlSetopt curl, 'VERBOSE', 0

      r = CurlSetOpt(Curl, 'URL', url)
      if Curlerror.intcode \= 0 Then Call Abort 'Error setting URL:' Url

      r = CurlSetOpt(Curl, 'OUTFILE', 'out.out')
      if Curlerror.intcode \= 0 Then Call Abort 'Error setting OUTFILE: out.out'

      r = CurlSetOpt(Curl, 'COOKIEFILE', '')
      if Curlerror.intcode \= 0 Then Call Abort 'Error setting COOKIEFILE to empty'

      r = CurlSetOpt(Curl, 'COOKIEJAR', 'out.cookie')
      if Curlerror.intcode \= 0 Then Call Abort 'Error setting COOKIEJAR: out.cookie'

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error getting source of' url, curl

      r = CurlGetInfo( Curl, 'COOKIELIST', 'stem.' )
      If curlerror.intcode \= 0 Then Call Abort 'Error getting COOKIELIST' url, curl

      Do i = 1 to stem.0
         Say stem.i
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
