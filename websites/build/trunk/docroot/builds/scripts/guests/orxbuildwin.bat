rem
rem Description: This is a build script step for the Windows build machine.
rem
rem Copyright (c) 2010-2011 Rexx Language Association. All rights reserved.
rem
rem This program and the accompanying materials are made available under
rem the terms of the Common Public License v1.0 which accompanies this
rem distribution. A copy is also available at the following address:
rem http://www.ibm.com/developerworks/oss/CPLv1.0.htm
rem
rem Redistribution and use in source and binary forms, with or
rem without modification, are permitted provided that the following
rem conditions are met:
rem
rem Redistributions of source code must retain the above copyright
rem notice, this list of conditions and the following disclaimer.
rem Redistributions in binary form must reproduce the above copyright
rem notice, this list of conditions and the following disclaimer in
rem the documentation and/or other materials provided with the distribution.
rem
rem Neither the name of Rexx Language Association nor the names
rem of its contributors may be used to endorse or promote products
rem derived from this software without specific prior written permission.
rem
rem THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
rem "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
rem LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
rem FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
rem OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
rem SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
rem TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
rem OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
rem OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
rem NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
rem SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
rem
rem Author: W. David Ashley
rem

rem ---------------------------------------------------------------------------
rem
rem This script should be invoked as follows:
rem      orxbuildwin.bat osname [buildtype [email]]
rem where:
rem      osname is like "win7-i386"
rem      buildtype is eother "trunk" or "branch"
rem      email is an email address
rem
rem ---------------------------------------------------------------------------

rem get the command line arguments
set osname=%1
set buildtype=%2
if %buildtype%=="" set buildtype=trunk
set email=%3

rem set up our default environment variables
c:
cd \
set builddir=\buildtemp
set targetdir=p:\www\build\docroot\builds\interpreter-main
set lockfile=c:\ooRexxBuild.lock

rem check the lockfile and loop if it exists
:existcheck
if exits %lockfile% goto startsleep
touch %lockfile%
goto endsleep
:startsleep
echo Sleeping one minute
sleep 60
goto existcheck
:endsleep

rem remove the old builddir if it still exists
rmdir /S /Q %builddir%

rem set up our build environment variables
if %osname%==win7-i386 call "C:\Tools\Visual.Studio.2008\VC\vcvarsall.bat" x86
if %osname%==win7-i386 call "C:\Tools\MsSDK.v6.0\bin\SetEnv.cmd" /Release /x86 /XP
if %osname%==win7-x86_64 call "C:\Tools\Visual.Studio2008\VC\vcvarsall.bat" amd64
if %osname%==win7-x86_64 call "C:\Tools\MsSDK.v6.0\bin\SetEnv.cmd" /Release /x64 /XP
rexx orxbuildwin.setenv.rex \orxbuildwin.setenv.bat
call \orxbuildwin.setenv.bat
del \orxbuildwin.setenv.bat
set SRC_DRV=c:
set SRC_DIR=%builddir%

rem create the builddir
md %builddir%
setlocal
cd %builddir%

rem checkout the source code
if %buildtype%==branch svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/branch/4.1.0/trunk/ %builddir%
if %buildtype%==trunk svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/ %builddir%

rem perform the build
if not exist makeorx.bat exit
call makeorx.bat BOTH PACKAGE

rem copy the output files
rexx orxbuildwin.copy.rex %email%
cd \
endlocal

rem remove build subdirectory tree
rmdir /S /Q %builddir%

rem remove the lockfile
rm %lockfile%

