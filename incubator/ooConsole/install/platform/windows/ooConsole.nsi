/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2014-2014 Rexx Language Association. All rights reserved.    */
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
 *  ooConsole.nsi
 *
 *  This is a basic NSIS install program, used to install ooConsole over the top
 *  of an ooRexx installation, optionally in its own directory (?).
 *
 *  Run as:
 *    makensis /DVERSION=x.x.x.x /DSHORTVERSION=x.x.x /DROOTDIR=yyy /DOUTDIR=xxx /DCPU=zzz /DDEBUGPKG ooConsole.nsi
 *  eg
 *    makensis /DVERSION=1.0.0.8704 /DSHORTVERSION=1.0.0 /DROOTDIR=C:\work\wc\incubator\ooConsole
 *             /DOUTDIR=C:\work\wc\incubator\ooConsole\bin\windows /DCPU=x86_32 /DDEBUGPKG=-debug ooConsole.nsi
 *
 *  Note:
 *    ooConsole.nsi must be in the current directory.
 */


;===============================================================================
;Configuration
;===============================================================================

;--------------------------------
; Defines

  !define LONGNAME       "ooConsole ${VERSION}"        ; Our long name
  !define SHORTNAME      "ooConsole"                   ; Our short name
  !define DISPLAYICON    "$INSTDIR\ooDialog.dll,2"     ; Use the ooDialog icon, the OOD icon TODO
  !define REXXSHORTNAME  "ooRexx"                      ; From ooRexx installer.
  !define REXXLONGNAME   "Open Object Rexx"            ; From ooRexx installer
  !define UNINSTALLER    "uninstallooconsole.exe"

  !define SMooRexxFolder "$SMPROGRAMS\${REXXLONGNAME}"

  !define SrcDir "${ROOTDIR}\src"

  !Define BinDir "${OUTDIR}"
  !define ExamplesDir "${ROOTDIR}\examples"

;--------------------------------
;Includes

  !include "MUI2.nsh"
  !include "Library.nsh"
  !include "LogicLib.nsh"
  !include "WinVer.nsh"
  !include "FileFunc.nsh"
  !include "WordFunc.nsh"
  !include "admin.nsh"
  !include "buttons.nsh"

;--------------------------------
;General

  Name "ooConsole ${VERSION}"
  OutFile "ooConsole-${VERSION}-${CPU}${DEBUGPKG}.exe"
  ShowInstdetails show
  SetOverwrite on
  SetPluginUnload alwaysoff
  RequestExecutionLevel admin
  InstallDir "$PROGRAMFILES\ooRexx"


;--------------------------------
;Interface Settings

  !define MUI_ICON "${ROOTDIR}\install\platform\windows\rexx.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "orange.bmp"

  !define MUI_CUSTOMPAGECOMMANDS
  !define MUI_LICENSEPAGE
  !define MUI_FINISHPAGE
  !define MUI_FINISHPAGE_NOAUTOCLOSE

  !define MUI_ABORTWARNING_TEXT "Are you sure you want to cancel the ooConsole installation?"
  !define MUI_CUSTOMFUNCTION_ABORT onUserAbort

  !define MUI_UNICON "${ROOTDIR}\install\platform\windows\uninstall.ico"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "orange-uninstall.bmp"
  !define MUI_UNFINISHPAGE_NOAUTOCLOSE


;--------------------------------
; Variables

  /* The uninstall program for ooRexx is always placed in the ooRexx
   * installation directory.  This makes the uninstall location equivalent to
   * the install location.
   */
  Var RegVal_installedLocation      ; location of ooRexx uninstall program found in registry
  Var IsAdminUser                   ; is the installer being run by an admin:           true / false
  Var haveProblems                  ; there are problems preventing a clean install     true / false
  Var ooRexxNotInstalled            ; ooRexx is not installed at all                    true / false
  Var ooRexxVersion                 ; Discovered ooRexx version
  Var isMinimumRequiredRexx         ; Installed ooRexx meets minimum required for this version of ooConsole true / false
  Var installerProblemIntro         ; First sentence when the installer detects a file can not be deleted, i.e. ooconsole.dll
  Var ooConsoleIsInstalled          ; Is there an installed ooConsole                    true / false
  Var RegVal_ooConsoleLocation      ; Read from registry, probably not needed until / if we install ooConsole to not under ooRexx

  ; Dialog variables
  Var Dialog
  Var LabelOne
  Var miscUse
  Var LabelOneTxt
;--------------------------------
;Pages

  /* Installer pages */
  !define MUI_PAGE_CUSTOMFUNCTION_LEAVE WelcomePageLeave
  !define MUI_WELCOMEPAGE_TEXT \
      "This Wizard will guide you through the installation of ${LONGNAME}.$\n$\n\
      ooConsole will be integrated into an existing installation of ooRexx.  \
      The ooRexx version must be equal to or greater than 4.1.0.  All ooConsole \
      programs and ooConsole documentation must be closed.  If these requirements \
      are not met, the installer will abort during the installation process.$\n$\n\
      Click Cancel, if needed, to close any ooConsole related files and restart the \
      installation.  Otherwise click Next to continue with the installation."
  !insertmacro MUI_PAGE_WELCOME

  Page custom CheckForProblemsPage

  !insertmacro MUI_PAGE_LICENSE "${ROOTDIR}\CPLv1.0.txt"
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  /* Uninstaller pages */
  !insertmacro MUI_UNPAGE_WELCOME

  ;!define MUI_PAGE_CUSTOMFUNCTION_PRE un.Confirm_Page_pre
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;===============================================================================
;Installer Sections
;===============================================================================

Section  doInstall

  ${if} $ooConsoleIsInstalled == 'true'
    Call RemoveFiles
  ${EndIf}

  DetailPrint "********** Installing ooConsole  **********"
  DetailPrint ""


  ; Install the files
  SetOutPath "$INSTDIR"

    DetailPrint "********** ooConsole ${SHORTVERSION}  **************"
    File "${BinDir}\ooconsole.dll"
    File "${BinDir}\ooConsole.cls"
    File "${BinDir}\ooShapes.cls"
    File "${BinDir}\ooshapes.dll"
    DetailPrint ""

    DetailPrint "********** ooConsole ${SHORTVERSION} Documentation **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\doc

    ; Add the files ...
    File "${ROOTDIR}\doc\ooconsole.pdf"
    File "${ROOTDIR}\doc\ooConsole-ReleaseNotes.txt"

    DetailPrint ""

    DetailPrint "********** ooConsole ${SHORTVERSION} Samples **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\ooConsole

    ; Add the files ...
    File "${ExamplesDir}\*.rex"
    ;File "${ExamplesDir}\*.frm"

    ; Create start menu shortcuts
    DetailPrint "********** ooConsole ${SHORTVERSION} Start Menu Shortcuts **********"

    CreateShortCut  "${SMooRexxFolder}\Uninstall ${SHORTNAME}.lnk" "$INSTDIR\${UNINSTALLER}" "" "$INSTDIR\${UNINSTALLER}" 0
    CreateShortCut  "${SMooRexxFolder}\ooRexx Documentation\ooConsole Reference.lnk" "$INSTDIR\doc\ooconsole.pdf" "" "$INSTDIR\doc\ooconsole.pdf" 0
    CreateShortCut  "${SMooRexxFolder}\ooRexx Documentation\ooConsole ReleaseNotes.lnk" "$INSTDIR\doc\ooConsole-ReleaseNotes.txt" "" "$INSTDIR\doc\ooConsole-ReleaseNotes.txt" 0

    DetailPrint ""

    ; Write the uninstall keys.
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

    ; Write the uninstall executable
    WriteUninstaller "$INSTDIR\${UNINSTALLER}"

SectionEnd

;===============================================================================
;Installer Functions
;===============================================================================

/** .onInit()  Call back function
 *
 * Called by the installer before any page is shown.  We use it to set up some
 * initial variables.
 */
Function .onInit

  StrCpy $haveProblems "false"

  ; See if the installee is an admin.
  Call IsUserAdmin
  Pop $IsAdminUser
  ${if} $IsAdminUser == "true"
    SetShellVarContext all
  ${endif}

  ReadRegStr $RegVal_installedLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REXXSHORTNAME}" "UnInstallLocation"

  ReadRegStr $RegVal_ooConsoleLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "InstallLocation"

  ${if} $RegVal_ooConsoleLocation == ""
    StrCpy $ooConsoleIsInstalled 'false'
  ${else}
    StrCpy $ooConsoleIsInstalled 'true'
  ${endif}

FunctionEnd


/** WelcomePageLeave()
 *
 *  This function is called when the user presses the Next button to continue
 *  on from the Welcome page.
 *
 *  Here we check that ooRexx is installed and is at least the minimum required.
 *
 *  Next, we check for possible problems with existing ooConsole files.
 *
 *  If the ooConsole doc is open, it can not be overwritten.  Or, if a Rexx
 *  ooConsole program is running, ooconsole.dll can not be overwritten.  This will
 *  prevent a clean install.
 *
 *  We check for this possible problem by trying to delete the files.  If we
 *  can not, we abort, keeping the current version intact.
 */
Function WelcomePageLeave

  ${if} $RegVal_installedLocation == ""
      StrCpy $ooRexxNotInstalled 'true'
      StrCpy $haveProblems "true"
      return
  ${endif}

  StrCpy $INSTDIR $RegVal_installedLocation

  ; It is common for the user to delete the ooConsole files without running the
  ; uninstaller.  If that happens, the ooConsole registry key exists and it looks
  ; like ooConsole is installed.  Here we check if the DLL actually exists and
  ; assume ooConsole is not installed if the DLL is not here.
  ${IfNot} ${FileExists} $INSTDIR\ooConsole.dll
      StrCpy $ooConsoleIsInstalled 'false'
  ${EndIf}

  Call DetermineRexxVersion

  ${if} $isMinimumRequiredRexx == "false"
      StrCpy $haveProblems "true"
      return
  ${endif}

  ${if} $ooConsoleIsInstalled == 'false'
      return
  ${EndIf}

  ; Skip this test if we can't find the PDF file to begin with.
  ${If} ${FileExists} $INSTDIR\doc\ooConsole.pdf
      ClearErrors
      CopyFiles /SILENT $INSTDIR\doc\ooConsole.pdf $INSTDIR\doc\ooConsoleTest.pdf
      ${If} ${Errors}
          StrCpy $installerProblemIntro "The installer detected a problem trying to copy ooConsole.pdf$\n$\n"
          StrCpy $haveProblems "true"

          ; ooConsole.pdf was not copied, we don't need to do anything before
          ; aborting.  Aborting is done in the custom page.
          return
      ${EndIf}

      ClearErrors
      Delete $INSTDIR\doc\ooConsole.pdf
      ${If} ${Errors}
          StrCpy $installerProblemIntro "The installer detected a problem with ooConsole.pdf$\n$\n"
          StrCpy $haveProblems "true"

          ; ooConsole.pdf was not deleted, we just need to delete the copy.
          Delete $INSTDIR\doc\ooConsoleTest.pdf
          return
      ${EndIf}

      ; ooConsole.pdf was deleted, we rename the copy to keep things clean
      Rename $INSTDIR\doc\ooConsoleTest.pdf $INSTDIR\doc\ooConsole.pdf
  ${EndIf}

  ; We tested above that this file exists.
  ClearErrors
  CopyFiles /SILENT $INSTDIR\ooConsole.dll $INSTDIR\ooConsoleTest.dll
  ${If} ${Errors}
      StrCpy $installerProblemIntro "The installer detected a problem trying to copy ooConsole.dll$\n$\n"
      StrCpy $haveProblems "true"

      ; ooConsole.dll was not copied, we don't need to do anything before
      ; aborting.

      return
  ${EndIf}

  ClearErrors
  Delete $INSTDIR\ooConsole.dll
  ${If} ${Errors}
      StrCpy $installerProblemIntro "The installer detected a problem with ooConsole.dll$\n$\n"
      StrCpy $haveProblems "true"

      ; ooConsole.dll was not deleted, we just need to delete the copy.
      Delete $INSTDIR\ooConsoleTest.dll

      return
  ${EndIf}

  ; ooConsole.dll was deleted, we rename the copy to keep things clean
  Rename $INSTDIR\ooConsoleTest.dll $INSTDIR\ooConsole.dll

FunctionEnd


/** CheckForProblemsPage()  Custom page function.
 *
 * This page is shown when we detect there will be problems with an
 * installation.  If there are no problems detected the page is skipped.
 */
Function CheckForProblemsPage

  /* Skip this page if no previous version is present */
  ${if} $haveProblems == "false"
      Abort
  ${endif}

  ${buttonEnabled} "Next" 0
  ${buttonEnabled} "Back" 0
  ${buttonText} "Cancel" "Exit"

  ; Set focus on the Cancel, now Exit button
  ; Get Item of Install button
  GetDlgItem $miscUse $HWNDPARENT 2
  ; Enable Install button
  ;EnableWindow $miscUse 1
  ; Set focus on Install button
  SendMessage $HWNDPARENT ${WM_NEXTDLGCTL} $miscUse 1


  ${if} $RegVal_installedLocation == ""
      !insertmacro MUI_HEADER_TEXT "ooRexx Installation." "There is no ooRexx installation detected."

      StrCpy $LabelOneTxt \
      "A version of ooRexx, greater than or equal to$\n\
      version 4.1.0, must be installed prior to the$\n\
      installation of ${LONGNAME}.$\n$\n\
      The installer can not detect any installed ooRexx.$\n$\n\
      The installer is aborting."
  ${elseif} $isMinimumRequiredRexx == "false"
      !insertmacro MUI_HEADER_TEXT "ooRexx Installation." "The installed ooRexx does not meet the minimum required."

      StrCpy $LabelOneTxt \
      "A version of ooRexx, greater than or equal to$\n\
      version 4.1.0, must be installed prior to the$\n\
      installation of ${LONGNAME}.$\n$\n\
      The installed ooRexx appears to be $ooRexxVersion.$\n$\n\
      The installer is aborting."
  ${else}
      !insertmacro MUI_HEADER_TEXT "Previous ooConsole Installation." $installerProblemIntro

      StrCpy $LabelOneTxt \
      "The installer has determined it will not be able to update some ooConsole \
      files.  Most likely because they are in use.$\n$\n\
      You MUST close the ooConsole reference documentation and close \
      any running ooConsole programs before executing the ooConsole \
      installation program.$\n$\n\
      The ooConsole installer must be closed.  Use the Esc key, or press the Exit button.$\n$\n\
      After the installer has ended, please close any running ooConsole programs \
      and make sure all the ooConsole documentation is closed.  Then retry the installation."
  ${endif}

  nsDialogs::Create /NOUNLOAD 1018
  Pop $Dialog

  ${If} $Dialog == error
      Abort
  ${endif}

  ${NSD_CreateLabel} 0 0 80% 120u $LabelOneTxt
  Pop $LabelOne

  nsDialogs::Show

FunctionEnd


/** onUserAbort()
 *
 *  This is a custom function called when the user presses the Cancel button.
 *
 *  If we have installer problems, wrong ooRexx version, files in use, etc.,
 *  then from the problem explanation page we want the Esc key or the Cancel
 *  button to just immediately close the installer.  Otherwise, we want the
 *  normal "Are you sure ..." message box.
 *
 *  From this function, abort means do not quit, if abort is not used, the
 *  installer exits.  So, if we are called with haveProblems true, we just
 *  drop through and exit.  If haveProblems is false, then we put up the message
 *  box and only call abort if the user says No, the do not want to quit.
 */
Function onUserAbort

  ${if} $haveProblems == "false"
      ${If} ${Cmd} `MessageBox MB_YESNO|MB_DEFBUTTON2 "${MUI_ABORTWARNING_TEXT}" IDNO`
          Abort
      ${EndIf}
  ${endif}

FunctionEnd


/** DeterminRexxVersion()
 *
 */
Function DetermineRexxVersion

  GetDllVersion "$INSTDIR\rexx.dll" $R0 $R1
  IntOp $R2 $R0 / 0x00010000
  IntOp $R3 $R0 & 0x0000FFFF
  IntOp $R4 $R1 / 0x00010000
  IntOp $R5 $R1 & 0x0000FFFF

  StrCpy $ooRexxVersion "$R2.$R3.$R4.$R5"

  ${If} $R2 >= 4
  ${AndIf} $R3 >= 1
    StrCpy $isMinimumRequiredRexx "true"
  ${Else}
    StrCpy $isMinimumRequiredRexx "false"
  ${EndIf}

FunctionEnd


/** RemoveFiles()
 *
 *  Deletes all the files of the current ooConsole version.  Note that deleting a
 *  non-existent file does not raise an alarm in the installer, so we just go
 *  ahead and delete things that might, or might not, be there without worrying
 *  about it.
 */
Function RemoveFiles

  DetailPrint "******* Removing current ooConsole files ******"
  DetailPrint ""

  ; We probably do not need to delete these files, we could just over-write
  ; them

  Delete $INSTDIR\ooConsole.dll
  Delete $INSTDIR\ooConsole.cls

  Delete $INSTDIR\doc\ooconsole.pdf
  Delete $INSTDIR\doc\ooConsole-ReleaseNotes.txt

  ; For the examples just delete the whole tree.
  RMDir /r $INSTDIR\samples\ooConsole

  ; For the short cuts, we delete any known short cuts created by any ooConsole
  ; installation.  Each independent ooConsole installation will create all
  ; relevant short cuts for that installation.
  DetailPrint "******* Removing ooConsole Start Menu short cuts ******"
  DetailPrint ""

  Delete  "${SMooRexxFolder}\Uninstall ${SHORTNAME}.lnk"
  Delete  "${SMooRexxFolder}\ooRexx Documentation\ooConsole Reference.lnk"
  Delete  "${SMooRexxFolder}\ooRexx Documentation\ooConsole ReleaseNotes.lnk"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}"

  DetailPrint ""

FunctionEnd

;===============================================================================
;  Uninstaller portion of ooConsole.nsi.
;===============================================================================

;===============================================================================
;  Uninstaller Sections
;===============================================================================

;-------------------------------------------------------------------------------
; Uninstall section
Section "Uninstall"

  DetailPrint ""
  DetailPrint "Removing ooConsole"

  Delete $INSTDIR\ooConsole.dll
  Delete $INSTDIR\ooConsole.cls

  Delete $INSTDIR\doc\ooconsole.pdf
  Delete $INSTDIR\doc\ooConsole-ReleaseNotes.txt

  ; For the examples just delete the whole tree.
  RMDir /r $INSTDIR\samples\ooConsole

  DetailPrint "Removing ooConsole Start Menu short cuts"

  Delete  "${SMooRexxFolder}\Uninstall ${SHORTNAME}.lnk"
  Delete  "${SMooRexxFolder}\ooRexx Documentation\ooConsole Reference.lnk"
  Delete  "${SMooRexxFolder}\ooRexx Documentation\ooConsole ReleaseNotes.lnk"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}"

  Delete "$INSTDIR\${UNINSTALLER}"

SectionEnd

;===============================================================================
;  Uninstaller Functions
;===============================================================================

/** un.onInit()  Callback function.
 *
 *  Called by the uninstaller program before any pages are shown.  We do not do
 *  much here and it is probably not needed.  In the future, we may provide an
 *  'over-write' install where ooConsole is intalled into the ooRexx directory,
 *  and a 'separate' install where ooConsole is installed as its own program in
 *  its own directory.
 *
 *  We could determine how ooConsole was installed by reading the registry and
 *  then doing the approprate uninstall.
 */
Function un.onInit

  /* Just saving for reference

  ${GetOptions} "$CMDLINE" "/U="  $R0
  ${if} $R0 == 'upgrade'
    StrCpy $DoUpgrade 'true'
  ${elseif} $R0 == 'upgradeQuick'
    StrCpy $DoUpgrade 'true'
    StrCpy $DoUpgradeQuick 'true'
  ${endif}

  */

  ; UnInstall as All Users if an admin
  Call un.IsUserAdmin
  Pop $IsAdminUser
  ${if} $IsAdminUser == "true"
    SetShellVarContext all
  ${endif}

FunctionEnd

