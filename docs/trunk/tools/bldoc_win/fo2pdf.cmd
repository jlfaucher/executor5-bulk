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
:: Name: FO2PDF.CMD
:: Type: Windows Batch script
::
Rem Create a PDF from an XSL-FO file using Apache FOP
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
Rem fop_opts cannot be local as it is referenced in fop.bat
if not defined fop_opts (
    set fop_opts=-Xmx1280M
    set nofopts=1
)
if not defined fo_files set fo_files=fo_files
set in_file=%fo_files%\%whichdoc%.fo
if not exist %in_file% (
    echo Unable to find %in_file%.
    echo Make sure the DOC2FO command has run successfully.
    goto :eof
)
if not defined fopver for /f %%v in ('dir fop-* /ad /b /od') do set fopver=%%v
if not defined fopver (
    echo Unable to find Apache FOP.
    echo Make sure the SETUP command has run successfully.
    goto :eof
)
setlocal
if not defined log_files set log_files=log_files
if not exist %log_files% (
    echo Unable to find %log_files%.
    goto :eof
)
set log_file=%log_files%\%whichdoc%.log
if not defined PDF_files set PDF_files=PDF_files
if not exist %PDF_files% (
    echo Unable to find %PDF_files%.
    goto :eof
)
set out_file=%PDF_files%\%whichdoc%.pdf
set fopcfg=%docpath%\%whichdoc%\en-US\Common_Content\fop.cfg
set foplvl=%fopver:~4%
echo %time:~0,-3% - Creating %out_file% from %in_file% using Apache FOP %foplvl%
call %fopver%\fop\fop.bat -c %fopcfg% %in_file% %out_file% 2>%log_file%
where rexx /q
if %errorlevel% EQU 0 (
    rexx chk4pdf.rex %out_file%
) else echo %time:~0,-3% - FOP processing ended
endlocal
Rem Clear temporary variables
if defined nofopts (
    set nofopts=
    set fop_opts=
)
exit /b
