@REM
@REM Copyright (c) 2012-2012 Rexx Language Association. All rights reserved.
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
REM  fixUpBotanfiles.bat
REM    This batch file determines if the build is for 64-bit or 32-bit and then
REM    copies the correct botan_all.* files to the sqlite directory for the
REM    build.
REM
REM - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


:DETERMINE_COMPILER
cl > temp.txt.okayToDelete 2>&1

for /F "tokens=1,3,6-10,11" %%i in (temp.txt.okayToDelete) do (
  if %%i == Microsoft (
    if %%j == C/C++ (
      if %%n EQU 80x86 (set CPU=X86) else (
        if %%n EQU x64 (set CPU=X64) else (
          if %%n EQU AMD64 (set CPU=X64) else (set CPU=X86)
        )
      )
    ) else (
      if %%o EQU 80x86 (set CPU=X86) else (
        if %%o EQU x64 (set CPU=X64) else (
          if %%o EQU AMD64 (set CPU=X64) else (set CPU=X86)
        )
      )
    )
  )
)

echo CPU=%CPU% > compiler.info.incl
del /F temp.txt.okayToDelete 1>nul 2>&1

if %CPU% EQU X86 (
  echo Copying x86 Botan files
  copy src\botan\botan_all_x86.cpp src\sqlite\botan_all.cpp
  copy src\botan\botan_all_x86.h src\sqlite\botan_all.h
) else (
  if %CPU% EQU X64 (
    echo Copying X64 Botan files
    copy src\botan\botan_all_amd64.cpp src\sqlite\botan_all.cpp
    copy src\botan\botan_all_amd64.h src\sqlite\botan_all.h
  ) else (
    echo Error in fixUpBotanfiles.bat
    exit /B 1
  )
)

exit /B 0
