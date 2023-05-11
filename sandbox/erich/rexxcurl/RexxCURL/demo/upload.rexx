/*
 * Rexx/CURL sample program to upload files to the /incoming directory
 * on an anonymous ftp site and change their permissions so they can be
 * downloaded.
 * Once all the files have been uploaded, list the directory.
 * Pass the files to upload on the command line.
 */
Call RxFuncAdd 'CurlLoadFuncs', 'rexxcurl', 'CurlLoadFuncs'
Call CurlLoadFuncs

Parse Arg filenames
site = 'ftp://upload.sourceforge.net/incoming/' /* specify your site */
user = 'anonymous:mark@rexx.org' /* specify your email address */
site = 'ftp://192.168.0.3/upload/'
user = 'Mark:mark'

curl = CurlInit()
If curl \= '' Then
   Do
      Say 'We are running' CurlVariable('VERSION')

      Call CurlSetopt curl, 'UPLOAD', 1
      If curlerror.intcode \= 0 Then Call Abort 'Error setting UPLOAD option'
      Call CurlSetopt curl, 'USERPWD', user
      If curlerror.intcode \= 0 Then Call Abort 'Error setting USERPWD option'
/*
Call CurlSetopt curl, 'URL', site
If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option'
*/
      Do i = 1 To Words( filenames )
         upload_file = Word( filenames, i )
         Call CurlSetopt curl, 'INFILE', upload_file
         If curlerror.intcode \= 0 Then Call Abort 'Error setting INFILE option'
         pos = Lastpos( '/', upload_file )
         remote_file = Substr( upload_file, pos+1 )
if i = 1 Then do
--         Call CurlSetopt curl, 'URL', site || remote_file
         Call CurlSetopt curl, 'URL', site
         If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option'
end
         Call CurlSetopt curl, 'PROGRESSFUNCTION', 'ProgressCallback'
         If curlerror.intcode \= 0 Then Call Abort 'Error setting PROGRESSFUNCTION option'
         Call Charout ,'Uploading' upload_file'...'
         /*
          * Upload the file...
          */
         Call CurlPerform curl
         If curlerror.intcode \= 0 Then Call Abort 'Error performing upload of' upload_file
         Say 'Done!'
         /*
          * Get some stats...
          */
         speed_upload = CurlGetinfo( curl, 'SPEED_UPLOAD' )
         total_time = CurlGetinfo( curl, 'TOTAL_TIME' )
         Say 'Speed:' speed_upload 'bytes/sec. Time:' total_time 'seconds'
      End
      /*
       * Now the uploads are finished, list the new directory to see our new
       * file(s). Of cource for the SF uploads directory we won't see anything!
      Call CurlSetopt curl, 'INFILE', ''
      If curlerror.intcode \= 0 Then Call Abort 'Error setting INFILE option off'
      Call CurlSetopt curl, 'URL', site
      If curlerror.intcode \= 0 Then Call Abort 'Error setting URL option'
      Call CurlSetopt curl, 'UPLOAD', 0
      If curlerror.intcode \= 0 Then Call Abort 'Error setting UPLOAD option off'
      Call CurlPerform curl
      If curlerror.intcode \= 0 Then Call Abort 'Error performing list of files in' site
       */
      /*
       * Cleanup the connection
       */
      Call CurlCLeanup curl
   End
Call CurlDropFuncs 'UNLOAD'
Return 0

ProgressCallback: Procedure
Call Charout ,'.'
Return 0

Abort: Procedure Expose curlerror.
Parse Arg msg
Say msg
If curlerror.curlcode \= 0 Then Say 'cURL error:' curlerror.curlcode '-' curlerror.curlerrm
Else Say 'RexxCURL error:' curlerror.intcode '-' curlerror.interrm
Call CurlCLeanup curl
Call CurlDropFuncs 'UNLOAD'
Exit 1
