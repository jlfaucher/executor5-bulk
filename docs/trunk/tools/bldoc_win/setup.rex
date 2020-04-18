/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2020 Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* Name: SETUP.REX                                                            */
/* Type: Object REXX Script                                                   */
/*                                                                            */
/* This program is designed to be run once and will retrieve the other parts
   needed for the BLDOC package and install them in the correct locations. If it
   is run again, it will verify that all the parts are installed correctly and,
   if not, retrieve them if required and then install them.  It does not install
   the fonts as they are optional but does make them available.

   Each package is retrieved from the Internet using a PowerShell command and
   placed in a local subfolder.  Then a PowerShell command is used to extract
   the files from the compressed folder into the proper folder. Finally, for the
   four packages needed for xsltproc, the executable files are copied into the
   main folder using another PowerShell command so that the path does not need
   to be modified in order to be able to use them.
*/
    -- Initialization
    zips_dir = ".\packages"
    zips = .file~new(zips_dir)  -- folder to hold the downloaded files
    if \zips~isDirectory then
        zips~makeDir

    -- First we handle the Liberation Fonts
    src_url = "https://www.fontsquirrel.com/fonts/download/"
    fonts = .file~new(".\Liberation Fonts")
    if \fonts~isDirectory then
        fonts~makeDir
    /* Each font family has 4 font files plus a license file; assume that if we
        have 8 or more files, then we are OK */
    if fonts~list~items < 8 then do -- missing fonts
        do aFont over "sans", "mono"
            font = "liberation-"aFont
            font_zip_name = zips_dir"\"font".zip"
            font_zip = .file~new(font_zip_name)
            if \font_zip~exists then
                call retrieve src_url||font, font_zip_name
            if font_zip~exists then
                -- need single quotes since folder name has a blank
                call unzip font_zip_name, "'"fonts~string"'"
            else
                say "Unable to retrieve font" font"."
        end
    end

    -- Next are the four packages for XSLTPROC
    src_url = "https://www.zlatkovic.com/pub/libxml/"
    pkgs = "libxslt-1.1.26.win32", "libxml2-2.7.8.win32", "iconv-1.9.2.win32", -
        "zlib-1.2.5.win32"
    /* The executables - the *.exe and *.dll files - from each package are
        copied to the current directory so the path does not need to be changed.
        So we check to see if the expected number of those files are present and
        assume that if they are, then we are OK */
    call SysFileTree ".\*.exe", exes., FO
    call SysFileTree ".\*.dll", dlls., FO
    if (exes.0 \= 5) | (dlls.0 \= 5) then do    -- missing executables
        do aPkg over pkgs
            pkg_zip_name = zips~string"\"aPkg".zip"
            pkg_zip = .file~new(pkg_zip_name)
            if \pkg_zip~exists then
                call retrieve src_url||aPkg".zip", pkg_zip_name
            if pkg_zip~exists then do
                if \.file~new(aPkg)~exists then
                    call unzip pkg_zip_name, .
                if aPkg~left(4) = "zlib" then
                    -- this one doesn't follow naming conventions, folder name
                    --  is missing .win32
                    parse var aPkg aPkg ".win32"
                if .file~new(aPkg"\bin")~exists then
                    "powershell copy-item" aPkg"\bin\* -Include *.exe, *.dll"
                else
                    say "Unable to locate" aPkg"\bin."
            end
            else
                say "Unable to retrieve" aPkg"."
        end
    end

    -- Finally the Apache FOP package
    src_url = "http://us.mirrors.quenda.co/apache/xmlgraphics/fop/binaries/"
    fop_pkg = "fop-2.4"
    if \.file~new(".\"fop_pkg)~isDirectory then do
        fop_zip_name = zips~string"\"fop_pkg"-bin.zip"
        fop_zip = .file~new(fop_zip_name)
        if \fop_zip~exists then
            call retrieve src_url||fop_pkg"-bin.zip", fop_zip_name
        if fop_zip~exists then
            call unzip fop_zip_name, .
    end

/* check that all the packages have been installed and inform the user */
    call SysFileTree ".\*.exe", exes., FO
    call SysFileTree ".\*.dll", dlls., FO
    if exes.0 = 5 & dlls.0 = 5 then
        say "XSLTPROC executables are now available."
    if .file~new(".\"fop_pkg)~isDirectory then
        say "Apache FOP package is now available."
    if fonts~list~items >= 8 then
        say "Liberation Fonts are available for installation."
    exit

retrieve: procedure -- get the package from the Internet
    parse arg url, zipfile
    "powershell start-bitstransfer """url""" -Destination" zipfile
    return

unzip: procedure    -- extract the package files to the appropriate location
    parse arg zipfile, outdir
    "powershell expand-archive -Path" zipfile "-DestinationPath" outdir "-Force"
    return
