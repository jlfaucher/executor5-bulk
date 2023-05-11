/*
 * Rexx/CURL sample program to mimic getright; a download program
 * with RESUME support
 * Arguments:
 *            source: protocol://location/file.ext
 *       destination: directory
 */
Call RxFuncAdd 'CurlLoadFuncs', 'rexxcurl', 'CurlLoadFuncs'
Call CurlLoadFuncs

Parse Arg url dir
If dir = '' Then
   Do
      Say 'No directory supplied'
      Exit 1
   End

pos = Lastpos( '/', url )
fname = Substr( url, pos+1 )
If Right( dir, 1 ) \= !REXXCURL.!DIRSEP Then dir = dir||!REXXCURL.!DIRSEP
dest = dir||fname
size = Stream( dest, 'C', 'QUERY SIZE' )
say Stream( dest, 'C', 'QUERY EXISTS' )

capath = ''
Parse Source os . prog
Select
   When os = 'WIN32' | os = 'WIN64' | os = 'OS2' | os = 'OS/2' Then
      Do
         capath = 'e:\etc\pki\tls\certs'
      End
   Otherwise Nop
End
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

      append = 'OVERWRITE'
      If size \= '' Then
         Do
            Call CurlSetopt curl, 'RESUMEFROM', size
            If curlerror.intcode \= 0 Then Call Abort 'Error setting RESUMEFROM option'
            append = 'APPEND'
         End

      Call CurlSetopt curl, 'OUTFILE', dest, append
      If curlerror.intcode \= 0 Then Call Abort 'Error setting OUTFILE option'

      Call CurlSetopt curl, 'PROGRESSFUNCTION', 'progresscallback'
      If curlerror.intcode \= 0 Then Call Abort 'Error setting PROGRESSFUNCTION option'

      Call CurlSetopt curl, 'SSLVERIFYPEER', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYPEER option'

      Call CurlSetopt curl, 'SSLVERIFYHOST', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting SSLVERIFYHOST option'

      Call CurlSetopt curl, 'FOLLOWLOCATION', 1
      If curlerror.intcode \= 0 Then Call Abort 'Error setting FOLLOWLOCATION option'

      If capath \= '' Then
         Do
            Call CurlSetopt curl, 'CAPATH', capath
            If curlerror.intcode \= 0 Then Call Abort 'Error setting CAPATH option'
         End

      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error getting source of' url
      /*
       * Cleanup the connection
       */
      Call CurlCLeanup curl
   End
--Call CurlDropFuncs 'UNLOAD'
Return 0

ProgressCallback:
Parse Arg dltotal, dlnow, .
dltotal = dltotal + 0
dlnow = dlnow + 0
If dltotal = 0 | dlnow = 0 Then Return 0
Say 'Downloaded:' dlnow 'of' dltotal';' dlnow / dltotal * 100'%'
Return 0

Abort: Procedure Expose curlerror. curl
Parse Arg msg
Say msg
If curlerror.curlcode \= 0 Then Say 'cURL error:' curlerror.curlcode '-' curlerror.curlerrm
Else Say 'RexxCURL error:' curlerror.intcode '-' curlerror.interrm
Call CurlCLeanup curl
--Call CurlDropFuncs 'UNLOAD'
Exit 1
