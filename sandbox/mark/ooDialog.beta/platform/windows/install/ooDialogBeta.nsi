/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
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
 *  ooDialogBeta.nsi
 *
 *  This is a basic NSIS install program, used to install ooDialog 4.2.0 beta.
 *
 *  It also installs switchOODialog.  switchOODialog is an executable used to
 *  switch between the 4.2.0 and 4.1.0 versions of ooDialog.
 *
 *  Run as:
 *    makensis /DVERSION=x.x.x.x /DNODOTVER=xxx /DSRCDIR=y /DEXEFILE=xxx /DCPU=xxx ooDialogBeta.nsi
 *  eg
 *    makensis /DVERSION=4.2.0.6367 /DNODOTVER=420 /DSRCDIR=C:\work\wc\main.4.1.0.ooDialog /DEXEFILE=switchOODialog /DCPU=x86_32 ooDialogBeta.nsi
 *
 *  Note:
 *    ooDialogBeta.nsi must be in the current directory.
 */


;===============================================================================
;Configuration
;===============================================================================

;--------------------------------
; Defines

  !define LONGNAME       "ooDialog ${VERSION} (beta)"  ; Our long name
  !define SHORTNAME      "ooDialogSwitch"              ; Our short name
  !define InstallFile    "${EXEFILE}.exe"              ; The switch ooDialog executable
  !define DISPLAYICON    "$INSTDIR\ooDialog.dll,2"     ; Use the ooDialog icon, the OOD icon
  !define REXXSHORTNAME  "ooRexx"                      ; From ooRexx installer.
  !define REXXLONGNAME   "Open Object Rexx"            ; From ooRexx installer
  !define UNINSTALLER    "uninstall_${EXEFILE}.exe"


;--------------------------------
;Includes

  !include "MUI2.nsh"
  !include "Library.nsh"
  !include "LogicLib.nsh"
  !include "FileFunc.nsh"
  !include "WordFunc.nsh"
  !include "admin.nsh"

;--------------------------------
;General

  Name "ooDialog Beta ${VERSION}"
  OutFile "ooDialog-${VERSION}-${CPU}.exe"
  ShowInstdetails show
  ShowUninstDetails show
  SetOverwrite on
  SetPluginUnload alwaysoff
  RequestExecutionLevel admin
  InstallDir "$PROGRAMFILES\ooRexx"


;--------------------------------
;Interface Settings

  !define MUI_ICON "${SRCDIR}\extensions\platform\windows\oodialog.4.2.0\AppIcon2.ico"
  !define MUI_UNICON "${SRCDIR}\platform\windows\install\uninstall.ico"
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

  /* The uninstall program for ooRexx is always placed in the ooRexx
   * installation directory.  This makes the uninstall location equivalent to
   * the install location.
   */
  Var RegVal_installedLocation      ; location of ooRexx uninstall program found in registry
  Var RegVal_installedVersion       ; Version / level of ooRexx uninstaller program.  This only exists at 410 or greater
  Var IsAdminUser                   ; is the installer being run by an admin:           true / false
  Var SwitchOODialogQualifiedName   ; The fully qualified switchOODialog file name.

  Var RegVal_startMenuFolder        ; location of, possible, alternate start menu folder
  Var RegVal_ourUninstallLocation   ; location of ooDialog beta uninstall program
  Var RegVal_ourUninstallString     ; ooDialog beta uninstall program

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${SRCDIR}\CPLv1.0.txt"
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;===============================================================================
;Installer Sections
;===============================================================================

Section  doInstall

  DetailPrint "********** Installing Switch ooDialog program **********"
  DetailPrint ""

  ; If found, we silently uninstall any existing switch ooDialog.
  ${If} $RegVal_ourUninstallString != ""
    DetailPrint "Found old ooDialog beta, cleaning up."
    ExecWait '$RegVal_ourUninstallString /S _?=$RegVal_ourUninstallLocation' $0
    DetailPrint ""
  ${Endif}

  ; Only 1 file is installed.
  SetOutPath "$INSTDIR"
  File "${InstallFile}"

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

  DetailPrint "Writing current ooDialog version key."
  WriteRegStr HKLM "Software\${SHORTNAME}" "CurrentVersion" "410"

  ; We create the Switch ooDialog short cut within the ooRexx Start Menu folder, if we find it.
  ; Otherwise we create a new ooDialog Start Menu folder, and record its name in the registry.
  ; Then on uninstall, we check for a recorded name and delete that folder.
  ${If} ${FileExists} "$SMPROGRAMS\${REXXLONGNAME}"
    DetailPrint "Creating Switch ooDialog short cut in ${REXXLONGNAME} Start Menu folder"
    CreateShortCut "$SMPROGRAMS\${REXXLONGNAME}\Switch ooDialog.lnk" '"$SwitchOODialogQualifiedName"'   ; Need to double quote.
    CreateShortCut "$SMPROGRAMS\${REXXLONGNAME}\Uninstall ${LONGNAME}.lnk" "$INSTDIR\${UNINSTALLER}" "" "$INSTDIR\${UNINSTALLER}" 0
  ${Else}
    DetailPrint "Creating Switch ooDialog short cut in ${LONGNAME} Start Menu folder"
    CreateDirectory "$SMPROGRAMS\${LONGNAME}"
    CreateShortCut "$SMPROGRAMS\${LONGNAME}\Switch ooDialog.lnk" '"$SwitchOODialogQualifiedName"'   ; Need to double quote.
    CreateShortCut "$SMPROGRAMS\${REXXLONGNAME}\Uninstall ${LONGNAME}.lnk" "$INSTDIR\${UNINSTALLER}" "" "$INSTDIR\${UNINSTALLER}" 0
    WriteRegStr HKLM "Software\${SHORTNAME}" "StartMenuFolder" "$SMPROGRAMS\${LONGNAME}"
  ${Endif}

  DetailPrint "Executing Switch ooDialog to set current version to 4.2.0."
  ExecWait '"$SwitchOODialogQualifiedName" /F=420' $0

  WriteUninstaller "$INSTDIR\${UNINSTALLER}"

SectionEnd

;===============================================================================
;Installer Functions
;===============================================================================

/** .onInit()  Call back function
 *
 * Called by the installer before any page is shown.  We use it to ensure
 * ooRexx is installed and the correct version level.
 *
 * ooRexx needs to be at least 4.1.0.  Since 4.1.0 was the first version where
 * the UninstallVersion was used, we only need to check if that registry entry
 * is the empty string or not.
 */
Function .onInit

  ; See if the installee is an admin.
  Call IsUserAdmin
  Pop $IsAdminUser
  ${if} $IsAdminUser == "true"
    SetShellVarContext all
  ${endif}

  ReadRegStr $RegVal_installedLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REXXSHORTNAME}" "UnInstallLocation"
  ReadRegStr $RegVal_installedVersion HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REXXSHORTNAME}" "UninstallVersion"

  ${if} $RegVal_installedVersion == ""
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "A version of ooRexx, greater than or equal to$\n\
      version 4.1.0, must be installed prior to the$\n\
      installation of ${LONGNAME}.$\n$\n\
      The installer is aborting."
      Abort
  ${endif}

  StrCpy $INSTDIR $RegVal_installedLocation
  StrCpy $SwitchOODialogQualifiedName "$INSTDIR\${InstallFile}"

  ReadRegStr $RegVal_ourUninstallString HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UninstallString"
  ReadRegStr $RegVal_ourUninstallLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UnInstallLocation"

FunctionEnd


;===============================================================================
;  Uninstaller portion of ooDialogBeta.nsi.
;===============================================================================

;===============================================================================
;  Uninstaller Sections
;===============================================================================

;-------------------------------------------------------------------------------
; Uninstall section
Section "Uninstall"

  DetailPrint "Setting ooDialog back to version 4.1.0"
  ExecWait '"$SwitchOODialogQualifiedName" /F=410' $0

  DetailPrint "Removing registry keys."
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}"
  DeleteRegKey HKLM "SOFTWARE\${SHORTNAME}"

  DetailPrint "Removing Start Menu items."
  ${If} $RegVal_startMenuFolder == ""
    Delete "$SMPROGRAMS\${REXXLONGNAME}\Switch ooDialog.lnk"
    Delete "$SMPROGRAMS\${REXXLONGNAME}\Uninstall ${LONGNAME}.lnk"
  ${Else}
    RMDir /r $RegVal_startMenuFolder
  ${Endif}

  DetailPrint "Deleting switch ooDialog program"
  Delete $SwitchOODialogQualifiedName

  Delete $INSTDIR\${UNINSTALLER}

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

  ; UnInstall as All Users if an admin
  Call un.IsUserAdmin
  Pop $IsAdminUser
  ${if} $IsAdminUser == "true"
    SetShellVarContext all
  ${endif}

  ReadRegStr $RegVal_installedLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UnInstallLocation"
  ReadRegStr $RegVal_startMenuFolder HKLM "Software\${SHORTNAME}\" "StartMenuFolder"

  StrCpy $SwitchOODialogQualifiedName "$RegVal_installedLocation\${InstallFile}"
FunctionEnd

