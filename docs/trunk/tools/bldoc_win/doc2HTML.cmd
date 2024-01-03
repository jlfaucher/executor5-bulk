@if not defined echo @echo off
:: -----------------------------------------------------------------------------
::
:: Copyright (c) 2020-2024, Rexx Language Association. All rights reserved.
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
:: Name: DOC2HTML.CMD
:: Type: Windows Batch script
::
rem Transform an ooRexx Docbook XML file to chunked HTML files using a modified
rem  Docbook stylesheet
if not defined docpath (
    echo You must specify the path to the ooRexx docs.
    echo Please run the DOCPATH command first.
    goto :eof
)
if [%1] NEQ [] call docprep %1
if not defined whichdoc (
    echo You must specify the name of the ooRexx document to be built.
    echo Please run the DOCPREP command first.
    goto :eof
)
call make_tmps %docpath%
setlocal
set dtd_cat=.\DocBook_Files\catalog.xml
set xsl_cat=.\DocBook_Files\docbook-xsl-nons-1.79.2\catalog.xml
set xml_catalog_files=%dtd_cat% %xsl_cat%
set inpath=%docpath%\%whichdoc%\en-US
set indoc=%inpath%\%whichdoc%.xml
if not defined HTML_folders set HTML_folders=HTML_folders
if not exist %HTML_folders% (
    echo Unable to find %HTML_folders%.
    goto :eof
)
set base_dir=%HTML_folders%\%whichdoc%
if not exist %base_dir% md %base_dir%
set x_opt1=--stringparam base.dir %base_dir%\
set x_opt2=--stringparam package ooRexx_Documentation-Open_Object_Rexx-5.0-en-US-0-0
set x_opt3=--stringparam prod.url https://www.oorexx.org/
set x_opt4=--stringparam xsltproc.version 1.1.26
set x_opts=--xinclude %x_opt1% %x_opt2% %x_opt3% %x_opt4% %xslt_opts%
echo %time:~0,-3% - Transforming %indoc% into %base_dir%\
echo This may take up to several minutes for the largest documents!
xsltproc %x_opts% html.xsl %indoc%
set xcpyf=%inpath%\Common_Content
set xcpyt=%base_dir%\Common_Content
echo Updating the files from the CSS folder if required.
xcopy %xcpyf%\css %xcpyt%\css /d /s /i /q /y
echo Updating the files from the common images folder if required.
xcopy %xcpyf%\images %xcpyt%\images /d /s /i /q /y
if exist %inpath%\images (
    echo Updating the files from the %whichdoc% images folder if required.
    xcopy %inpath%\images\* %base_dir%\images /d /s /i /q /y
)
where rexx /q
if %errorlevel% EQU 0 (
    rem Check that HTML files were created and optionally create a .zip file
    rexx chk4HTML.rex %base_dir%\
) else echo %time:~0,-3% - Transformation ended
