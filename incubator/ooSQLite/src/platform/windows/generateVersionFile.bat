@REM
@REM Copyright (c) 2012-2008 Rexx Language Association. All rights reserved.
@REM
@REM This program and the accompanying materials are made available under
@REM the terms of the Common Public License v1.0 which accompanies this
@REM distribution. A copy is also available at the following address:
@REM http://www.oorexx.org/license.html
@REM
@REM Redistribution and use in source and binary forms, with or
@REM without modification, are permitted provided that the following
@REM conditions are met:
@REM
@REM Redistributions of source code must retain the above copyright
@REM notice, this list of conditions and the following disclaimer.
@REM Redistributions in binary form must reproduce the above copyright
@REM notice, this list of conditions and the following disclaimer in
@REM the documentation and/or other materials provided with the distribution.
@REM
@REM Neither the name of Rexx Language Association nor the names
@REM of its contributors may be used to endorse or promote products
@REM derived from this software without specific prior written permission.
@REM
@REM THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
@REM "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
@REM LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
@REM FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
@REM OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
@REM SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
@REM TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
@REM OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
@REM OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
@REM NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
@REM SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@REM
@ECHO OFF

REM - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
REM  generateVersionFile.bat
REM    This batch file generates, (or uses an existing,) ooSQLite.ver.incl file.
REM    If executing in a svn 'working copy' directory, it determines the current
REM    revision number and includes that information in the generated file.  If
REM    not a working directory, it checks for an existing ooSQLite.ver.incl
REM    file, which may have been included in a source file package when the
REM    package was created.  If not a svn directory, and no ooSQLite.ver.incl
REM    file, simply copy ooSQLite.ver to ooSQLite.ver.incl.
REM - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

pushd src
cd

REM  First parse ooSQLite.ver to get the existing version numbers.
for /F "eol=# delims== tokens=1,2,3*" %%i in (ooSQLite.ver) do (
 if %%i == OOSQLITE_MAJOR set MAJOR_NUM=%%j
 if %%i == OOSQLITE_MINOR set MINOR_NUM=%%j
 if %%i == OOSQLITE_MOD_LVL set LVL_NUM=%%j
 if %%i == OOSQLITE_BLD_LVL set BLD_NUM=%%j
)

set SVN_REV=NONE

for /F "usebackq tokens=1,2,3,4*" %%i in (`svn info`) do if (%%i) == (Revision:) set SVN_REV=%%j

REM If we didn't get a svn revision number, go to NOSVN and fake it.
if %SVN_REV% == NONE goto NOSVN

REM  Now write out ooSQLite.ver.incl
if exist ooSQLite.ver.incl del /F /Q ooSQLite.ver.incl
for /F "delims== tokens=1,2,3*" %%i in (ooSQLite.ver) do (
 if %%i == OOSQLITE_BLD_LVL (
   echo %%i=%SVN_REV%>> ooSQLite.ver.incl
   set BLD_NUM=%SVN_REV%
 ) else (
   if %%i == OOSQLITE_VER_STR (
     echo %%i="%MAJOR_NUM%.%MINOR_NUM%.%LVL_NUM%.%SVN_REV%">> ooSQLite.ver.incl
   ) else (
     if %%jx == x (
       echo %%i>> ooSQLite.ver.incl
     ) else (
       echo %%i=%%j>> ooSQLite.ver.incl
     )
   )
 )
)
echo SVN_REVSION=%SVN_REV%>> ooSQLite.ver.incl
goto DONE_OK

:NOSVN
if NOT exist ooSQLite.ver.incl (
  for /F "delims== tokens=1,2,3*" %%i in (ooSQLite.ver) do (
    if %%i == OOSQLITE_BLD_LVL (
      echo %%i=%%j>> ooSQLite.ver.incl
    ) else (
      if %%i == OOSQLITE_VER_STR (
        echo %%i="%MAJOR_NUM%.%MINOR_NUM%.%LVL_NUM%.%BLD_NUM%">> ooSQLite.ver.incl
      ) else (
        if %%jx == x (
          echo %%i>> ooSQLite.ver.incl
        ) else (
          echo %%i=%%j>> ooSQLite.ver.incl
        )
      )
    )
  )
  echo SVN_REVSION=0 >> ooSQLite.ver.incl
)

goto DONE_OK

:DONE_ERR
popd
set MAJOR_NUM=
set MINOR_NUM=
set LVL_NUM=
set BLD_NUM=
set SVN_REV=

exit /b 1

:DONE_OK
popd
set MAJOR_NUM=
set MINOR_NUM=
set LVL_NUM=
set BLD_NUM=
set SVN_REV=
exit /b 0


