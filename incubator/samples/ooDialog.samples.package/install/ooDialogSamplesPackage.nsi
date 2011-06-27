/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2011 Rexx Language Association. All rights reserved.    */
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

/**
 *  ooDialogSamplesPackage.nsi
 *
 *  An NSIS script to build the ooDialog Samples Package installer.
 *
 */


;===============================================================================
;Configuration
;===============================================================================

;--------------------------------
; Defines

  !define VERSION        "0.0.1"
  !define VERSIONNODOTS  "0_0_1"
  !define SHORTNAME      "ooDialog_Samples_${VERSIONNODOTS}"
  !define LONGNAME       "ooDialog Sample Package ${VERSION}"
  !define DISPLAYICON    "$INSTDIR\AppIcon2.ico"
  !define SRCDIR         "C:\work.ooRexx\wc\incubator\samples\ooDialog.samples.package"
  !define UNINSTALLER    "uninstall.exe"


;--------------------------------
;Includes

  !include "MUI2.nsh"
  !include "Library.nsh"
  !include "LogicLib.nsh"
  !include "FileFunc.nsh"
;  !include "admin.nsh"

;--------------------------------
;General

  Name "ooDialog Sample Package"
  OutFile "${SHORTNAME}.exe"
  ShowInstdetails show
  SetOverwrite on
  SetPluginUnload alwaysoff
  RequestExecutionLevel user
  InstallDir "$APPDATA\ooDialog.samples.${VERSION}"


;--------------------------------
;Interface Settings

  !define MUI_ICON "AppIcon2.ico"
  !define MUI_UNICON "uninstall.ico"

  !define MUI_CUSTOMPAGECOMMANDS
  !define MUI_WELCOMEFINISHPAGE_BITMAP "orange.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "orange-uninstall.bmp"

  !define MUI_LICENSEPAGE
  !define MUI_FINISHPAGE
  !define MUI_FINISHPAGE_NOAUTOCLOSE

  !define MUI_ABORTWARNING

  !define MUI_UNINSTALLER
  !define MUI_UNFINISHPAGE_NOAUTOCLOSE

;--------------------------------
; Variables

  Var RegVal_installedLocation                    ; example

;--------------------------------
; Pages.
;

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "CPLv1.0.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;===============================================================================
;Installer Sections
;===============================================================================

;Section "Dummy Section" SecDummy
Section  installFiles

  ; Set the installation directory:
  SetOutPath "$INSTDIR"

  DetailPrint "********** ooDialog Samples ${VERSION} ************"

  File "${SRCDIR}\install\CPLv1.0.txt"
  File "${SRCDIR}\install\AppIcon2.ico"

  SetOutPath "$INSTDIR\Controls\Edit\NumberOnly"
  File "${SRCDIR}\Controls\Edit\NumberOnly\restrictedInput.h"
  File "${SRCDIR}\Controls\Edit\NumberOnly\restrictedInput.rc"
  File "${SRCDIR}\Controls\Edit\NumberOnly\RestrictedInput.rex"
  File "${SRCDIR}\Controls\Edit\NumberOnly\restrictedInput32.dll"
  File "${SRCDIR}\Controls\Edit\NumberOnly\restrictedInput64.dll"

  ; Write the uninstall keys.
  DetailPrint "Writing uninstall keys."
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "InstallLocation" '"$INSTDIR"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "DisplayName" "${LONGNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "DisplayIcon" "${DISPLAYICON}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "HelpLink" "http://www.rexxla.org/support.html"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "URLUpdateInfo" "http://sourceforge.net/project/showfiles.php?group_id=119701"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "URLInfoAbout" "http://www.rexxla.org/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "Publisher" "Rexx Language Association"

  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UninstallString" '"$INSTDIR\${UNINSTALLER}"'
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UnInstallLocation" "$INSTDIR" ; dont quote it
  WriteRegStr       HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UninstallVersion" "${VERSION}"
  WriteRegDWORD     HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "NoModify" 0x00000001
  WriteRegDWORD     HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "NoRepair" 0x00000001

  WriteUninstaller "$INSTDIR\${UNINSTALLER}"

SectionEnd

;===============================================================================
;Installer Functions
;===============================================================================

/** .onInit()  Call back function
 *
 * Called by the installer before any page is shown.  We use it to ?
 */
Function .onInit


FunctionEnd


;===============================================================================
;  Uninstaller portion of ooDialogSamplesPackage.nsi.
;===============================================================================

;===============================================================================
;  Uninstaller Sections
;===============================================================================

;-------------------------------------------------------------------------------
; Uninstall section
Section "Uninstall"

  DetailPrint "Removing registry keys."
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}"
  DeleteRegKey HKLM "SOFTWARE\${SHORTNAME}"

  DetailPrint "Deleting switch ooDialog program"

  RMDir /r $RegVal_installedLocation

SectionEnd


;===============================================================================
;  Uninstaller Functions
;===============================================================================

/** un.onInit()  Callback function.
 *
 *  Called by the uninstaller program before any pages are shown.
 *
 */
Function un.onInit

  ReadRegStr $RegVal_installedLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UnInstallLocation"

FunctionEnd

