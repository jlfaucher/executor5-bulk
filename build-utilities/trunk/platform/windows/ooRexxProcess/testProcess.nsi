OutFile "testProcess.exe"

!include LogicLib.nsh

# default section
Section

   StrCpy $0 "Mark Miesfeld"
   ooRexxProcess::findProcess "rxapi"
   Pop $R0
   MessageBox MB_OK "findProcess rxapi ret = $R0"

   ooRexxProcess::findProcess "rxapi.exe"
   Pop $R0
   MessageBox MB_OK "findProcess rxapi.exe ret = $R0"

   ${if} $R0 == 0  ; 0 equals success, equals found.
     ooRexxProcess::killProcess "rxapi.exe"
     Pop $R0
     MessageBox MB_OK "killProcess rxapi.exe ret = $R0"
   ${endif}


SectionEnd


