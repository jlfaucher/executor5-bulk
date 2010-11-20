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
 *  switchOODialog.nsi
 *
 *  Take advantage of NSIS's builtin compression and unpackaging to produce an
 *  executable to switch between 4.1.0 and 4.2.0 versions of ooDialog.  The
 *  basic steps to do the switch are:
 *
 *  * Read the registry to get our installed location, abort if not correct.
 *
 *  * Read the registry to get the current version of ooDialog, switch to the
 *    other version.
 *
 *  * Do the switch by: Deleting all files and directories in the current
 *    ooDialog that are not in the switch-to version.  Install, by over-writing,
 *    all files in the switch-to version.
 *
 *  One basic problem that can happen is if the user has an ooDialog program
 *  running or the ooDialog PDF doc open.  This will prevent the install by
 *  overwrite from working for oodial0g.dll and / or oodialog.pdf.  There is
 *  no way to fix that, that I know of.  The advice to the user will be to
 *  close things and rerun switchOODialog.
 *
 *  The other problem would be if the user uninstalls ooRexx completely without
 *  first uninstalling ooDialog beta.  Some
 */



;===============================================================================
;Configuration
;===============================================================================

;--------------------------------
; Defines

  !define SHORTNAME      "ooDialogSwitch"              ; Must be in sync with the ooDialogBeta SHORTNAME
  !define LONGNAME       "ooDialog ${VERSION} (beta)"

  !define SrcDir410 "${SRCDIR}\extensions\platform\windows\oodialog.4.1.0"
  !define SrcDir420 "${SRCDIR}\extensions\platform\windows\oodialog.4.1.0"
;--------------------------------
;Includes

  !include "MUI2.nsh"
  !include "Library.nsh"
  !include "LogicLib.nsh"
  !include "FileFunc.nsh"
  !include "admin.nsh"

;--------------------------------
;General

  Name "Switch ooDialog"
  OutFile "switchOODialog.exe"
  ShowInstdetails hide
  SetOverwrite on
  SetPluginUnload alwaysoff
  RequestExecutionLevel admin
  InstallDir "$PROGRAMFILES\ooRexx"


;--------------------------------
;Interface Settings

  !define MUI_ICON "${SRCDIR}\extensions\platform\windows\oodialog.4.2.0\AppIcon2.ico"
  !define MUI_UNICON "${SRCDIR}\platform\windows\install\uninstall.ico"

;--------------------------------
; Variables

  Var RegVal_installedLocation   ; Our installed location, found in the registry
  Var RegVal_installedVersion    ; Don't need this Version / level of uninstaller program.  This only exists at 410 or greater
  Var ForceVersion               ; Allows the user to force what ooDialog version is in effect.
  Var CurrentVersion             ; The current ooDialog version, read from the registry
  Var NewVersion                 ; The version of ooDialog to switch to.

  Var BinDir420
  Var BinDir410
  Var SamplesDir420
  Var SamplesDir410
  Var SrcDir420
  Var SrcDir410

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
; Pages  We do not need any pages because we use SetSilent in .onInit

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH


;===============================================================================
;Installer Sections
;===============================================================================

;Section "Dummy Section" SecDummy
Section  doSwitch

  SetOutPath "$INSTDIR"
  DetailPrint "NewVersion is: $NewVersion CurrentVersion is: $CurrentVersion"
  ${If} $NewVersion == 420
    Delete AppIcon2.ico
    File "${SrcDir420}\oodialog.ico"
    WriteRegStr HKLM "Software\${SHORTNAME}" "CurrentVersion" "420"
  ${Else}
    Delete oodialog.ico
    File "${SrcDir420}\AppIcon2.ico"
    WriteRegStr HKLM "Software\${SHORTNAME}" "CurrentVersion" "410"
  ${Endif}

SectionEnd

;===============================================================================
;Installer Functions
;===============================================================================

/** .onInit()  Call back function
 *
 * Called by the installer before any page is shown.  We use it to ensure
 * ooRexx is installed and the correct version level.  If so, we check the
 * version of ooDialog currently in effect, (by reading the registry) and swap
 * it.
 */
Function .onInit

  /* Always silent */
  ;SetSilent silent

  /* Check if the user is forcing the current version to something specific. */
  ${GetOptions} "$CMDLINE" "/F="  $R0
  StrCpy $ForceVersion $R0

  /* Be sure we are installed progperly */
  ReadRegStr $RegVal_installedLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "InstallLocation"
  ${If} $RegVal_installedLocation == ""
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "${LONGNAME} has not been installed properly.$\n$\n\
      Please follow the guidelines published for using$\n\
      Switch ooDialog, especially the instructions for$\n\
      installation."

    Abort
  ${Endif}

  MessageBox MB_OK "RegVal_installedLocation: $RegVal_installedLocation."

  /* Be sure, if passed a /F arg, it is correct. */
  ${If} $ForceVersion != ""
    ${If} $ForceVersion != 420
    ${AndIf} $ForceVersion != 410
      MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
        "The /F argument of: '$ForceVersion' is not correct.$\n$\n\
        Aborting."

      Abort
    ${Endif}
  ${Endif}

  ;StrCpy $INSTDIR "$RegVal_installedLocation"
  StrCpy $INSTDIR "C:\home\Interpreters\Rexx\ooRexx"

  StrCpy $BinDir410 "$BINDIR\oodialog410"
  StrCpy $BinDir420 "$BINDIR\oodialog420"
  StrCpy $SamplesDir410 "$SRCDIR\samples\windows\oodialog.4.1.0"
  StrCpy $SamplesDir420 "$SRCDIR\samples\windows\oodialog.4.1.0"

  ${if} $ForceVersion == ""
    MessageBox MB_OK "INSTDIR: $INSTDIR User is NOT forcing version."
    ReadRegStr $CurrentVersion HKLM "Software\${SHORTNAME}" "CurrentVersion"
    ${If} $CurrentVersion == 410
      StrCpy $NewVersion 420
    ${Else}
      StrCpy $NewVersion 410
    ${Endif}
  ${else}
    MessageBox MB_OK "User is forcing version to $ForceVersion."
    ${If} $ForceVersion == 410
      StrCpy $NewVersion 410
      StrCpy $CurrentVersion 420
    ${Else}
      StrCpy $NewVersion 420
      StrCpy $CurrentVersion 410
    ${Endif}
  ${endif}

FunctionEnd


