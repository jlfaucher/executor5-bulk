/*
 * Rexx/CURL sample program to mimic getright; a download program
 * with RESUME support
 * Arguments:
 *            source: protocol://location/file.ext
 *       destination: directory
 */
Call RxFuncAdd 'CurlLoadFuncs', 'rexxcurl', 'CurlLoadFuncs'
Call CurlLoadFuncs

url = 'https://www.networking4all.com/'

curl = CurlInit()
If curl \= '' Then
   Do
      msg = 'We are running' CurlVariable('VERSION')
      Say Copies( '*', Length( msg ) )
      Say msg
      Say Copies( '*', Length( msg ) )
      Say

      Call CurlSetopt curl, 'URL', url
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option'

      Call CurlSetopt curl, 'SSLVERIFYPEER', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYPEER option'

      Call CurlSetopt curl, 'SSLVERIFYHOST', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYHOST option'

      Call CurlSetopt curl, 'CERTINFO', 1
      If curlerror.intcode \= 0 Then Call Abort 'Error setting CERTINFO option'

      Call CurlSetopt curl, 'OUTSTEM', 'junk.'
      If curlerror.intcode \= 0 Then Call Abort 'Error setting OUTSTEM option'

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error getting source of' url

      Call CurlGetInfo  curl, 'CERTINFO', 'CERT.'
      If curlerror.intcode \= 0 Then Call Abort 'Error getting CERTINFO from' url

      Do i = 1 To cert.0
         Say 'Certificate ('i'):' cert.i
      End
      /*
       * Cleanup the connection
       */
      Call CurlCLeanup curl
   End
Call CurlDropFuncs 'UNLOAD'
Return 0

WriteCallback:
Parse Arg one, two, three
say one';'two';'three
Return 0

Abort: Procedure Expose curlerror.
Parse Arg msg
Say msg
If curlerror.curlcode \= 0 Then Say 'cURL error:' curlerror.curlcode '-' curlerror.curlerrm
Else Say 'RexxCURL error:' curlerror.intcode '-' curlerror.interrm
Call CurlCLeanup curl
Call CurlDropFuncs 'UNLOAD'
Exit 1
