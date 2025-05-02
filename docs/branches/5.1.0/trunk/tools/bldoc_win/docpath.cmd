@if not defined echo @echo off
:: -----------------------------------------------------------------------------
::
:: Copyright (c) 2020 Rexx Language Association. All rights reserved.
::
:: This program and the accompanying materials are made available under
:: the terms of the Common Public License v1.0 which accompanies this
:: distribution. A copy is also available at the following address:
:: http://www.oorexx.org/license.html
::
:: Redistribution and use in source and binary forms, with or
:: without modification, are permitted provided that the following
:: conditions are met:
::
:: Redistributions of source code must retain the above copyright
:: notice, this list of conditions and the following disclaimer.
:: Redistributions in binary form must reproduce the above copyright
:: notice, this list of conditions and the following disclaimer in
:: the documentation and/or other materials provided with the distribution.
::
:: Neither the name of Rexx Language Association nor the names
:: of its contributors may be used to endorse or promote products
:: derived from this software without specific prior written permission.
::
:: THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
:: "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
:: LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
:: FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
:: OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
:: SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
:: TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
:: OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
:: OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
:: NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
:: SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
::
::------------------------------------------------------------------------------
::
:: Name: DOCPATH.CMD
:: Type: Windows Batch script
::
rem Set the env. var. docpath to the path for the working copy of the
rem  ooRexx docs
if [%1] EQU [] (
    echo You must specify the path to the ooRexx docs.
    goto :eof
)
if [%2] NEQ [] (
    echo The path specified contains blanks; please enclose it in quotes.
    goto :eof
)
if not exist %1 (
    echo Unable to find %1; please check the spelling.
    goto :eof
)
set docpath=%1
rem Get the argument w/o any quotes
set _docpath=%~1
rem %_docpath% may contain blanks so only compare the 2nd character of each
rem If they are equal, there were no quotes
if [%docpath:~1,1%] NEQ [%_docpath:~1,1%] goto quotes
rem No quotes, check for and remove a trailing \ if found
if [%docpath:~-1%] EQU [\] set docpath=%docpath:~0,-1%
goto skip
:quotes
rem Quotes - check for and remove a trailing \ if found
if [%_docpath:~-1%] EQU [\] set docpath="%_docpath:~0,-1%"
:skip
set _docpath=
echo The docpath is set to %docpath%
