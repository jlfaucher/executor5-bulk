/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2020-2022 Rexx Language Association. All rights reserved.    */
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
   needed for the BLDOC package and install them in the correct locations. If
   it is run again, it will verify that all the parts are installed correctly
   and, if not, retrieve them if required and then install them.  It does not
   install the fonts as they are optional but does make them available.

   Each package is retrieved from the Internet using a PowerShell command and
   placed in a local subfolder.  Then a PowerShell command is used to extract
   the files from the compressed folder into the proper folder. Finally, for
   the four packages needed for xsltproc, the executable files are copied
   into the main folder using another PowerShell command so that the path
   does not need to be modified in order to be able to use them.
*/
    -- Verify the platform (only runs on Windows)
    if .rexxinfo~platform~left(1) \= 'W' then do
        say 'This program is only designed to run on Windows.'
        exit
    end
    -- Initialization
    _ = '09'x   -- Tab char.
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
        say "Making the Liberation Fonts available for installation. (1/5)"
        do aFont over "sans", "mono"
            font = "liberation-"aFont
            font_zip_name = zips_dir"\"font".zip"
            font_zip = .file~new(font_zip_name)
            if \font_zip~exists then do
                say _"Downloading the" font "fonts."
                call retrieve src_url||font, font_zip_name
                if result \= 0 then
                    say _"Error retrieving" font".zip; RC was" result"."
            end
            if font_zip~exists then do
                say _"Unzipping the" font "fonts."
                -- need single quotes since folder name has a blank
                call unzip font_zip_name, "'"fonts~string"'"
                if result \= 0 then
                    say _"Error unzipping" font_zip_name"; RC was" result"."
            end
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
        say "Making XSLTPROC executables available. (2/5)"
        do aPkg over pkgs
            pkg_zip_name = zips_dir"\"aPkg".zip"
            pkg_zip = .file~new(pkg_zip_name)
            if \pkg_zip~exists then do
                say _"Downloading" aPkg".zip."
                call retrieve src_url||aPkg".zip", pkg_zip_name
                if result \= 0 then
                    say _"Error retrieving" aPkg".zip; RC was" result"."
            end
            if pkg_zip~exists then do
                say _"Unzipping the" aPkg "package."
                if \.file~new(aPkg)~exists then do
                    call unzip pkg_zip_name, .
                    if result \= 0 then
                        say _"Error unzipping" pkg_zip_name"; RC was" result"."
                end
                if aPkg~left(4) = "zlib" then
                    -- this one doesn't follow naming conventions, folder name
                    --  is missing .win32
                    parse var aPkg aPkg ".win32"
                if .file~new(aPkg"\bin")~exists then do
                    say _"Copying the *.exe and *.dll files."
                    address path -
                    "powershell Copy-Item" aPkg"\bin\* -Include *.exe, *.dll"
                    if RC \= 0 then
                        say _"RC" RC "copying the *.exe/*.dll files from" aPkg"."
                end
            end
        end
    end

    -- Create a folder for the DocBook files
    DBfolderName = ".\DocBook_Files"
    dtd_xsl = .file~new(DBfolderName)
    if \dtd_xsl~isDirectory then
        dtd_xsl~makeDir

    -- Get the DTD files to improve performance
    --  http://www.oasis-open.org/docbook/xml/4.5/docbook-xml-4.5.zip
    src_url = "http://www.oasis-open.org/docbook/xml/4.5/"
    main_dtd = "docbookx.dtd"
    if \.file~new(DBfolderName"\"main_dtd)~exists then do
        say "Making DocBook DTD files available. (3/5)"
        dtds_zip_name = "docbook-xml-4.5.zip"
        full_zip_name = zips_dir"\"dtds_zip_name
        dtds_zip = .file~new(full_zip_name)
        if \dtds_zip~exists then do
            say _"Downloading" dtds_zip_name"."
            call retrieve src_url||dtds_zip_name, full_zip_name
            if result \= 0 then
                say _"Error retrieving" dtds_zip_name"; RC was" result"."
        end
        if dtds_zip~exists then do
            say _"Unzipping" dtds_zip_name"."
            call unzip full_zip_name, DBfolderName
            if result \= 0 then
                say _"Error unzipping" dtds_zip_name"; RC was" result"."
        end
    end

    -- Get the XSL files to improve performance
    --  https://github.com/docbook/xslt10-stylesheets/releases/
    --  download/release/1.79.2/docbook-xsl-nons-1.79.2.zip
    src_url = "https://github.com/docbook/xslt10-stylesheets/releases/" || -
            "download/release/1.79.2/"
    xsl_pkg = "docbook-xsl-nons-1.79.2"
    if \.file~new(DBfolderName"\"xsl_pkg)~exists then do
        say "Making DocBook style sheets available. (4/5)"
        xsl_zip_name = zips_dir"\"xsl_pkg".zip"
        xsl_zip = .file~new(xsl_zip_name)
        -- Due to an issue with using BITS with GitHub to retrieve files, (see
        -- https://powershell.org/forums/topic/bits-transfer-with-github/), we
        -- use Invoke-WebRequest instead
        if \xsl_zip~exists then do
            say _"Downloading" xsl_pkg".zip."
            call retrieve src_url||xsl_pkg".zip", xsl_zip_name, "use_IWR"
            if result \= 0 then
                say _"Error retrieving" xsl_pkg".zip; RC was" result"."
        end
        if xsl_zip~exists then do
            say _"Unzipping" xsl_pkg".zip."
            call unzip xsl_zip_name, DBfolderName
            if result \= 0 then
                say _"Error unzipping" xsl_pkg".zip; RC was" result"."
        end
    end

    -- Finally the Apache FOP package
    -- https://xmlgraphics.apache.org/fop/download.html
    -- Use the above site to find an alternate mirror site if needed.
    src_url = "https://mirror.nodesdirect.com/apache/xmlgraphics/fop/binaries/"
    fop_lvl = getfoplvl()   -- determine the latest version from the Web page
    if fop_lvl = "??" then
        say "Unable to determine the latest version of Apache FOP."
    else do
        fop_pkg = "fop-"fop_lvl
        if \.file~new(".\"fop_pkg)~isDirectory then do
            say "Making Apache FOP package available. (5/5)"
            fop_zip_name = zips_dir"\"fop_pkg"-bin.zip"
            fop_zip = .file~new(fop_zip_name)
            if \fop_zip~exists then do
                say _"Downloading" fop_pkg"-bin.zip"
                call retrieve src_url||fop_pkg"-bin.zip", fop_zip_name
                if result \= 0 then
                    say _"Error retrieving" fop_pkg"-bin.zip; RC was" result"."
            end
            if fop_zip~exists then do
                say _"Unzipping" fop_pkg"-bin.zip"
                call unzip fop_zip_name, .
                if result \= 0 then
                    say _"Error unzipping" fop_pkg"-bin.zip; RC was" result"."
            end
        end
    end

/* check that all the packages have been installed and inform the user */
    if fonts~list~items >= 8 then
        say "Liberation Fonts are available for installation. (1/5)"
    call SysFileTree ".\*.exe", exes., FO
    call SysFileTree ".\*.dll", dlls., FO
    if exes.0 = 5 & dlls.0 = 5 then
        say "XSLTPROC executables are now available. (2/5)"
    if .file~new(DBfolderName"\"main_dtd)~exists then
        say "DocBook DTD files are now available. (3/5)"
    if .file~new(DBfolderName"\"xsl_pkg)~isDirectory then
        say "DocBook style sheets are now available. (4/5)"
    if .file~new(".\"fop_pkg)~isDirectory then
        say "Apache FOP package is now available. (5/5)"
    exit

retrieve: procedure -- get the package from the Internet
    parse arg url, outfile
    if arg(3, O) then   -- use the newer BITS method
        address path -
            "powershell Start-BitsTransfer """url""" -Destination" outfile
    else                -- use older API
        address path -
            "powershell Invoke-WebRequest" url "-OutFile" outfile
    return RC

unzip: procedure    -- extract the package files to the appropriate location
    parse arg zipfile, outdir
    address path -
    "powershell Expand-Archive -Path" zipfile "-DestinationPath" outdir "-Force"
    return RC

getfoplvl: procedure    -- determine the current level of FOP
    fopHomePage = "https://xmlgraphics.apache.org/fop/"
    htmlFile = .file~new("fop_page.htm")
    call retrieve fopHomePage, htmlFile~name
    fopPage = .stream~new(htmlFile)
    fopLines = fopPage~arrayin
    fopPage~close
    htmlFile~delete
    lvl = '??'  -- default if unable to find the line with the latest level
    do i=1 to fopLines~items
        if fopLines[i]~pos('latest version of FOP is available at') > 0 then do
            -- found the line, now get the value of the latest level
            parse value fopLines[i] with . '<a' . '>FOP ' lvl '<' .
            leave
        end
    end
    return lvl
