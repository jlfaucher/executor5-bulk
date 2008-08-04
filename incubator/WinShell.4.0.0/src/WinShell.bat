
@ECHO OFF

REM  First parse winshell.ver to get the existing version numbers.
for /F "eol=# delims== tokens=1,2,3*" %%i in (winshell.ver) do (
 if %%i == ORX_MAJOR set MAJOR_NUM=%%j
 if %%i == ORX_MINOR set MINOR_NUM=%%j
 if %%i == ORX_MOD_LVL set LVL_NUM=%%j
 if %%i == ORX_BLD_LVL set BLD_NUM=%%j
)

if not exist .svn\nul goto NOSVN

for /F "usebackq tokens=1,2,3,4*" %%i in (`svn info`) do if (%%i) == (Revision:) set SVN_REV=%%j

if %SVN_REV%x == x (
  echo Executing in a svn working copy, but could not determine the svn revision
  echo number.
  echo Do NOT use this environment for a release build.
  echo.
  goto NOSVN
)

REM Now write out winshell.ver.incl
if exist winshell.ver.incl del /F /Q winshell.ver.incl
for /F "delims== tokens=1,2,3*" %%i in (winshell.ver) do (
 if %%i == ORX_BLD_LVL (
   echo %%i=%SVN_REV%>> winshell.ver.incl
   set BLD_NUM=%SVN_REV%
 ) else (
   if %%i == ORX_VER_STR (
     echo %%i="%MAJOR_NUM%.%MINOR_NUM%.%LVL_NUM%.%SVN_REV%">> winshell.ver.incl
   ) else (
     if %%jx == x (
       echo %%i>> winshell.ver.incl
     ) else (
       echo %%i=%%j>> winshell.ver.incl
     )
   )
 )
)
echo SVN_REVSION=%SVN_REV%>> winshell.ver.incl
goto DONE

:NOSVN
if exist winshell.ver.incl (
   for /F "eol=# delims== tokens=1,2,3*" %%i in (winshell.ver.incl) do (
    if %%i == ORX_BLD_LVL set BLD_NUM=%%j
    if %%i == SVN_REVISION set SVN_REV=%%j
   )
) else (
  copy winshell.ver winshell.ver.incl 1>nul 2>&1
  echo SVN_REVSION=0>> winshell.ver.incl
)

:DONE
