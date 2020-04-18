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
:: Name: DOC2FO.CMD
:: Type: Windows Batch script
::
rem Transform an ooRexx Docbook XML file to XSL-FO using a modified
rem  Docbook stylesheet
if not defined docpath (
    echo You must specify the path to the ooRexx docs.
    echo Please run the DOCPATH command first.
    goto :eof
)
if not defined whichdoc (
    echo You must specify the name of the ooRexx document to be built.
    echo Please run the DOCPREP command first.
    goto :eof
)
setlocal
set indoc=%docpath%\%whichdoc%\en-US\%whichdoc%.xml
set x_opts=--xinclude %xslt_opts%
echo %time:~0,-3% - Transforming %indoc% to fo_files\%whichdoc%.fo
echo This may take up to 10+ minutes for the largest documents!
date /T > %docpath%\..\datepub.tmp
where svnversion /q
if %errorlevel% EQU 0 (
    svnversion %docpath% > %docpath%\..\svnrev.tmp
) else echo ?> %docpath%\..\svnrev.tmp
xsltproc %x_opts% -o fo_files\%whichdoc%.fo pdf.xsl %indoc%
echo %time:~0,-3% - Transformation complete
