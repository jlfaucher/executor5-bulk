@REM
@REM Copyright (c) 2012-2013 Rexx Language Association. All rights reserved.
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
REM  determineCompiler.bat
REM    This batch file sets CPU and MSVCVER as determined by executing cl.  It
REM    also sets OOCON_ROOT_DIR because this is easy to do in a batch file, but
REM    difficult to do in a nMake make file.
REM
REM    It is simply a copy of the stuff from the ooDialog build.
REM
REM    There may be a better way to do this, especially since VC++ 6.0 is not
REM    supported anymore.  Later versions of either the SDK or VC++ set
REM    TARGET_CPU and MSVCVER, so we could just pull the values from the
REM    enviroment.  But, I'm not sure when those started getting set.  It would
REM    take a lot of testing to determine what compiler / SDK combinations would
REM    be supported if we did away with this.  It is simplier to just keep it.
REM - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set OOCON_ROOT_DIR=%cd%

pushd src

:DETERMINE_COMPILER
cl > temp.txt.okayToDelete 2>&1

for /F "tokens=1,3,6-10,11" %%i in (temp.txt.okayToDelete) do (
  if %%i == Microsoft (
    if %%j == C/C++ (
      if %%l GTR 16.0 set MSVCVER=10.0
      if %%l GTR 15.0 (if %%l LSS 16 set MSVCVER=9.0)
      if %%l GTR 14.0 (if %%l LSS 15 set MSVCVER=8.0)
      if %%l GTR 13.0 (if %%l LSS 14 set MSVCVER=7.0)
      if %%l GTR 12.0 (if %%l LSS 13 set MSVCVER=6.0)

      if %%n EQU 80x86 (set CPU=X86) else (
        if %%n EQU x64 (set CPU=X64) else (
          if %%n EQU AMD64 (set CPU=X64) else (set CPU=X86)
        )
      )
    ) else (
      if %%m GTR 16.0 set MSVCVER=10.0
      if %%m GTR 15.0 (if %%m LSS 16 set MSVCVER=9.0)
      if %%m GTR 14.0 (if %%m LSS 15 set MSVCVER=8.0)
      if %%m GTR 13.0 (if %%m LSS 14 set MSVCVER=7.0)
      if %%m GTR 12.0 (if %%m LSS 13 set MSVCVER=6.0)

      if %%o EQU 80x86 (set CPU=X86) else (
        if %%o EQU x64 (set CPU=X64) else (
          if %%o EQU AMD64 (set CPU=X64) else (set CPU=X86)
        )
      )
    )
  )
)

echo CPU=%CPU% > compiler.info.incl
echo MSVCVER=%MSVCVER% >> compiler.info.incl
echo OOCON_ROOT_DIR=%OOCON_ROOT_DIR% >> compiler.info.incl

del /F temp.txt.okayToDelete 1>nul 2>&1

popd
