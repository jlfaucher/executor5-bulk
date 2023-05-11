/**/
Parse arg ver .
If ver = '' Then
 Do
    Say 'Must supply version: eg 20'
    Exit
 End
SRC = Value('REXXCURL_SRCDIR',,'ENVIRONMENT')
'mkdir dist'
Call Chdir 'dist'
'del /Y *.*'
'copy ..\rexxcurl.exe'
'copy ..\rexxcurl.dll'
'copy c:\bin\rexxtrans.dll'
'copy' SRC || '\TODO'
'copy' SRC || '\INSTALL'
'copy' SRC || '\CPLv1.0.txt'
'copy' SRC || '\README'
'mkdir demo'
'copy' SRC || '\demo\*.rexx demo'
'mkdir extradlls'
'copy c:\bin\libcurl.dll extradlls'
'copy c:\bin\libeay32.dll extradlls'
'copy c:\bin\libssl32.dll extradlls'
'mkdir doc'
'copy ..\doc\*.html doc'
'copy ..\doc\*.png doc'
'del rexxcurl??_w32.zip'
'zip -r rexxcurl'ver'_w32 *'
Return

