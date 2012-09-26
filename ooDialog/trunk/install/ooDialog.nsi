/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2012 Rexx Language Association. All rights reserved.    */
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
 *  ooDialog.nsi
 *
 *  This is a basic NSIS install program, used to install ooDialog over the top
 *  of an ooRexx installation.
 *
 *  Run as:
 *    makensis /DVERSION=x.x.x.x /DSHORTVERSION=x.x.x /DROOTDIR=yyy /DOUTDIR=xxx /DCPU=zzz /DDEBUG ooDialog.nsi
 *  eg
 *    makensis /DVERSION=4.2.0.6367 /DSHORTVERSION=4.2.0 /DROOTDIR=C:\work\wc\ooDialog\trunk
 *             /DOUTDIR=C:\work\wc\ooDialog\trunk\Win32Rel /DCPU=x86_32 /DDEBUGPKG=-debug ooDialog.nsi
 *
 *  Note:
 *    ooDialog.nsi must be in the current directory.
 */


;===============================================================================
;Configuration
;===============================================================================

;--------------------------------
; Defines

  !define LONGNAME       "ooDialog ${VERSION} (beta)"  ; Our long name
  !define SHORTNAME      "ooDialog"                    ; Our short name
  !define DISPLAYICON    "$INSTDIR\ooDialog.dll,2"     ; Use the ooDialog icon, the OOD icon
  !define REXXSHORTNAME  "ooRexx"                      ; From ooRexx installer.
  !define REXXLONGNAME   "Open Object Rexx"            ; From ooRexx installer

  !define SMooRexxFolder "$SMPROGRAMS\${REXXLONGNAME}"

  !define SrcDir "${ROOTDIR}\oodialog"
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

;--------------------------------
;General

  Name "ooDialog ${VERSION}"
  OutFile "ooDialog-${VERSION}-${CPU}${DEBUGPKG}.exe"
  ShowInstdetails show
  SetOverwrite on
  SetPluginUnload alwaysoff
  RequestExecutionLevel admin
  InstallDir "$PROGRAMFILES\ooRexx"


;--------------------------------
;Interface Settings

  !define MUI_ICON "${ROOTDIR}\ooDialog\AppIcon2.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "orange.bmp"

  !define MUI_LICENSEPAGE
  !define MUI_FINISHPAGE
  !define MUI_FINISHPAGE_NOAUTOCLOSE

  !define MUI_ABORTWARNING

;--------------------------------
; Variables

  /* The uninstall program for ooRexx is always placed in the ooRexx
   * installation directory.  This makes the uninstall location equivalent to
   * the install location.
   */
  Var RegVal_installedLocation      ; location of ooRexx uninstall program found in registry
  Var IsAdminUser                   ; is the installer being run by an admin:           true / false
  Var ooRexxVersion                 ; Discovered ooRexx version
  Var isMinimumRequiredRexx         ; Installed ooRexx meets minimum required for this version of ooDialog true / false
  Var installerProblemIntro         ; First sentence when the installer detects a file can not be deleted, i.e. oodialog.dll

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${ROOTDIR}\install\CPLv1.0.txt"
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;===============================================================================
;Installer Sections
;===============================================================================

Section  doInstall

  DetailPrint "********** Installing ooDialog  **********"
  DetailPrint ""

  Call RemoveFiles

  ; Install the files
  SetOutPath "$INSTDIR"

    DetailPrint "********** ooDialog 4.2.0 Framework **********"
    File "${BinDir}\oodialog.dll"
    File "${BinDir}\ooDialog.cls"
    File "${BinDir}\oodPlain.cls"
    File "${BinDir}\oodWin32.cls"
    DetailPrint ""

    DetailPrint "********** ooDialog 4.2.0 ooRexxTry **********"
    File "${ExamplesDir}\ooRexxTry\ooRexxTry.rex"
    CreateShortCut "${SMooRexxFolder}\Try Rexx (GUI).lnk" "$INSTDIR\rexx.exe" '"$INSTDIR\ooRexxTry.rex"' "$INSTDIR\rexx.exe"
    DetailPrint ""

    DetailPrint "********** ooDialog 4.2.0 Documentation **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\doc
    ; Add the files ...
    File "${ROOTDIR}\doc\oodialog.pdf"
    File "${ROOTDIR}\doc\oodguide.pdf"
    File "${ExamplesDir}\ooRexxTry\doc\ooRexxTry.pdf"

    CreateShortCut  "${SMooRexxFolder}\Documentation\ooDialog Reference.lnk" "$INSTDIR\doc\oodialog.pdf" "" "$INSTDIR\doc\oodialog.pdf" 0
    CreateShortCut  "${SMooRexxFolder}\Documentation\ooDialog User Guide.lnk" "$INSTDIR\doc\oodguide.pdf" "" "$INSTDIR\doc\oodguide.pdf" 0
    CreateShortCut  "${SMooRexxFolder}\Documentation\ooRexxTry Reference.lnk" "$INSTDIR\doc\ooRexxTry.pdf" "" "$INSTDIR\doc\ooRexxTry.pdf" 0
    DetailPrint ""

    DetailPrint "********** ooDialog 4.2.0 Samples **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog
    ; Add the files ...
    File "${SrcDir}\oodialog.ico"
    File "${ExamplesDir}\*.rex"
    File "${ExamplesDir}\*.h"
    File "${ExamplesDir}\*.inp"
    File "${ExamplesDir}\*.ico"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\bmp
    ; Add the files ...
    File "${ExamplesDir}\bmp\*.bmp"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls
    ; Add the files ...
    File "${ExamplesDir}\controls\*.rex"
    File "${ExamplesDir}\controls\*.rc"
    File "${ExamplesDir}\controls\*.h"
    File "${ExamplesDir}\controls\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\examples
    ; Add the files ...
    File "${ExamplesDir}\examples\*.rex"
    File "${ExamplesDir}\examples\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\examples\resources
    ; Add the files ...
    File "${ExamplesDir}\examples\resources\*.bmp"
    File "${ExamplesDir}\examples\resources\*.h"
    File "${ExamplesDir}\examples\resources\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\menus
    ; Add the files ...
    File "${ExamplesDir}\menus\*.rex"
    File "${ExamplesDir}\menus\*.h"
    File "${ExamplesDir}\menus\*.bmp"
    File "${ExamplesDir}\menus\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\mouse
    ; Add the files ...
    File "${ExamplesDir}\mouse\*.rex"
    File "${ExamplesDir}\mouse\*.h"
    File "${ExamplesDir}\mouse\*.rc"
    File "${ExamplesDir}\mouse\*.cur"
    File "${ExamplesDir}\mouse\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\oleinfo
    ; Add the files ...
    File "${ExamplesDir}\oleinfo\*.rex"
    File "${ExamplesDir}\oleinfo\*.txt"
    File "${ExamplesDir}\oleinfo\*.bmp"
    File "${ExamplesDir}\oleinfo\*.rc"

    SetOutPath $INSTDIR\samples\oodialog\ooRexxTry
    ; Add the files ...
    File "${ExamplesDir}\ooRexxTry\ooRexxTry.rex"

    SetOutPath $INSTDIR\samples\oodialog\ooRexxTry\doc
    ; Add the files ...
    File "${ExamplesDir}\ooRexxTry\doc\ooRexxTry.pdf"

    SetOutPath $INSTDIR\samples\oodialog\propertySheet.tabs
    ; Add the files ...
    File "${ExamplesDir}\propertySheet.tabs\*.cls"
    File "${ExamplesDir}\propertySheet.tabs\oodListViews.rex"
    File "${ExamplesDir}\propertySheet.tabs\PropertySheetDemo.rex"
    File "${ExamplesDir}\propertySheet.tabs\TabDemo.rex"
    ; File "${ExamplesDir}\propertySheet.tabs\TabOwnerDemo.rex"   ; Do not expose this example yet
    File "${ExamplesDir}\propertySheet.tabs\ticketWizard.rex"
    File "${ExamplesDir}\propertySheet.tabs\UserTabDemo.rex"


    SetOutPath $INSTDIR\samples\oodialog\propertySheet.tabs\rc
    ; Add the files ...
    File "${ExamplesDir}\propertySheet.tabs\rc\oodListViews*"
    File "${ExamplesDir}\propertySheet.tabs\rc\PropertySheetDemo*"
    ; File "${ExamplesDir}\propertySheet.tabs\rc\TabOwnerDemo*"   ; Do not expose this example yet
    File "${ExamplesDir}\propertySheet.tabs\rc\ticketWizard*"
    File "${ExamplesDir}\propertySheet.tabs\rc\UserTabDemo.h"
    ; File "${ExamplesDir}\propertySheet.tabs\rc\UserTabDemo.rc"  ; Do not included in distribution, maybe in the future.

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\rc
    ; Add the files ...
    File "${ExamplesDir}\rc\*.rc"
    File "${ExamplesDir}\rc\*.h"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\res
    ; Add the files ...
    File "${ExamplesDir}\res\*.dll"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\simple
    ; Add the files ...
    File "${ExamplesDir}\simple\*.h"
    File "${ExamplesDir}\simple\*.rc"
    File "${ExamplesDir}\simple\*.rex"
    File "${ExamplesDir}\simple\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\sysinfo
    ; Add the files ...
    File "${ExamplesDir}\sysinfo\*.h"
    File "${ExamplesDir}\sysinfo\*.rex"
    File "${ExamplesDir}\sysinfo\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\source
    ; Add the files ...
    File "${BinDir}\ooDialog.cls"
    File "${BinDir}\oodWin32.cls"
    File "${BinDir}\oodPlain.cls"
    File "${SrcDir}\0_READ_ME_FIRST.txt"
    File "${SrcDir}\build_ooDialog_cls.rex"
    File "${SrcDir}\AnimatedButton.cls"
    File "${SrcDir}\BaseDialog.cls"
    File "${SrcDir}\ControlDialog.cls"
    File "${SrcDir}\DeprecatedClasses.cls"
    File "${SrcDir}\DialogControls.cls"
    File "${SrcDir}\DialogExtensions.cls"
    File "${SrcDir}\DynamicDialog.cls"
    File "${SrcDir}\EventNotification.cls"
    File "${SrcDir}\ListView.cls"
    File "${SrcDir}\Menu.cls"
    File "${SrcDir}\PlainBaseDialog.cls"
    File "${SrcDir}\RcDialog.cls"
    File "${SrcDir}\ResDialog.cls"
    File "${SrcDir}\TreeView.cls"
    File "${SrcDir}\UserDialog.cls"
    File "${SrcDir}\UtilityClasses.cls"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\tutorial
    ; Add the files ...
    File "${ExamplesDir}\tutorial\*.rex"
    File "${ExamplesDir}\tutorial\*.bmp"
    File "${ExamplesDir}\tutorial\*.rc"
    File "${ExamplesDir}\tutorial\*.h"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\userGuide
    ; Add the files ...
    File "${ExamplesDir}\userGuide\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise02
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise02\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise03
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise03\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise04
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise04\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise04\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise04\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise05
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise05\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise05\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise05\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise05\res
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise05\res\res.mak"
    File "${ExamplesDir}\userGuide\exercises\Exercise05\res\ProductView.dll"
    File "${ExamplesDir}\userGuide\exercises\Exercise05\res\ProductIcon.bmp"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Customer
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Customer\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Customer\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Customer\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Customer\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Customer\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Customer\bmp\*.bmp"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Order
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Order\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Order\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Order\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Order\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Order\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Order\bmp\*.bmp"

    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\OrderMgr
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\OrderMgr\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\OrderMgr\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\OrderMgr\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Product
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Product\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Product\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Product\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Product\res
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Product\res\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Product\res\*.bmp"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Product\res\*.dll"
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Product\res\res.mak"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Samples\ReadMe.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples\DlgData
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Samples\DlgData\*.h"
    File "${ExamplesDir}\userGuide\exercises\Samples\DlgData\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Samples\DlgData\*rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples\DlgData\res
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Samples\DlgData\res\res.mak"
    File "${ExamplesDir}\userGuide\exercises\Samples\DlgData\res\ASimpleDialog.dll"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples\Popups
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Samples\Popups\*rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Support
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Support\*.cls"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\wav
    ; Add the files ...
    File "${ExamplesDir}\wav\*.wav"
    File "${ExamplesDir}\wav\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\winsystem
    File "${ExamplesDir}\winsystem\*.rex"
    File "${ExamplesDir}\winsystem\*.rc"
    File "${ExamplesDir}\winsystem\*.h"
    File "${ExamplesDir}\winsystem\*.frm"
    DetailPrint ""


    ; Create start menu shortcuts
    DetailPrint "********** ooDialog 4.2.0 Start Menu Shortcuts **********"

    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Display Event Log.lnk" "$INSTDIR\rexxpaws.exe" '"$INSTDIR\samples\oodialog\winsystem\eventlog.rex"' "$INSTDIR\rexx.exe"
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Windows Manager.lnk" "$INSTDIR\rexxhide.exe" '"$INSTDIR\samples\oodialog\winsystem\usewmgr.rex"' "$INSTDIR\rexx.exe"

    CreateDirectory "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog"
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Samples.lnk" "$INSTDIR\rexxhide.exe" '"$INSTDIR\samples\oodialog\sample.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Calculator.lnk" "$INSTDIR\rexxhide.exe" '"$INSTDIR\samples\oodialog\calculator.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Change Editor.lnk" "$INSTDIR\rexxhide.exe" '"$INSTDIR\samples\oodialog\editrex.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\FTYPE Changer.lnk" "$INSTDIR\rexxhide.exe" '"$INSTDIR\samples\oodialog\ftyperex.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"
    DetailPrint ""


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

  ${if} $RegVal_installedLocation == ""
    Call WrongRexxVersion
    Abort
  ${endif}

  StrCpy $INSTDIR $RegVal_installedLocation

  Call DetermineRexxVersion

  ${if} $isMinimumRequiredRexx == "false"
    Call WrongRexxVersion
    Abort
  ${endif}

  Call CheckForProblems

FunctionEnd


/** WrongRexxVersion()
 *
 * Puts up a generic message box stating that ooRexx needs to be installed and
 * at least XX version.
 */
Function WrongRexxVersion

  ${if} $RegVal_installedLocation == ""
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "A version of ooRexx, greater than or equal to$\n\
      version 4.1.0, must be installed prior to the$\n\
      installation of ${LONGNAME}.$\n$\n\
      The installer can not detect any installed ooRexx.$\n$\n\
      The installer is aborting."
  ${else}
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "A version of ooRexx, greater than or equal to$\n\
      version 4.1.0, must be installed prior to the$\n\
      installation of ${LONGNAME}.$\n$\n\
      The installed ooRexx appears to be $ooRexxVersion.$\n$\n\
      The installer is aborting."
  ${endif}

FunctionEnd


/** InstallerProblem()
 *
 * Puts up a generic message box stating that the intaller detected a problem
 */
Function InstallerProblem

  MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
    "$installerProblemIntro\
    You MUST close the ooDialog reference documentation and close$\n\
    any running ooDialog programs before executing the ooDialog$\n\
    installation program.$\n$\n\
    The ooDialog installer is aborting.$\n$\n\
    After the installer closes, please close any running$\n\
    ooDialog programs and make sure all the ooDialog$\n\
    documentation is closed, then retry the installation."

FunctionEnd


/** DeterminRexxVersion()
 *
 *
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


/** CheckForProblems()
 *
 *  If the ooDialog doc is open, it can not be overwritten.  Or, if a Rexx
 *  ooDialog program is running, oodialog.dll can not be overwritten.  This will
 *  prevent a clean install.
 *
 *  We check for this possible problem by trying to delete the files.  If we
 *  can not, we abort, keeping the current version intact.
 */
Function CheckForProblems

  ClearErrors
  CopyFiles /SILENT $INSTDIR\doc\oodialog.pdf $INSTDIR\doc\oodialogTest.pdf
  ${If} ${Errors}
      StrCpy $installerProblemIntro "The installer detected a problem trying to copy oodialog.pdf$\n$\n"
      Call InstallerProblem

      ; ooDialog.pdf was not copied, we don't need to do anything before
      ; aborting.
      Abort
  ${EndIf}

  ClearErrors
  Delete $INSTDIR\doc\oodialog.pdf
  ${If} ${Errors}
      StrCpy $installerProblemIntro "The installer detected a problem with oodialog.pdf$\n$\n"
      Call InstallerProblem

      ; ooDialog.pdf was not deleted, we just need to delete the copy.
      Delete $INSTDIR\doc\oodialogTest.pdf
      Abort
  ${EndIf}

  ; ooDialog.pdf was deleted, we rename the copy to keep things clean
  Rename $INSTDIR\doc\oodialogTest.pdf $INSTDIR\doc\oodialog.pdf

  ; We check oodguide.pdf also, but it may not exist.
  ${If} ${FileExists} $INSTDIR\doc\oodguide.pdf
    ClearErrors
    CopyFiles /SILENT $INSTDIR\doc\oodguide.pdf $INSTDIR\doc\oodguideTest.pdf
    ${If} ${Errors}
        StrCpy $installerProblemIntro "The installer detected a problem trying to copy oodguide.pdf$\n$\n"
        Call InstallerProblem

        ; oodguide.pdf was not copied, we don't need to do anything before
        ; aborting.
        Abort
    ${EndIf}

    ClearErrors
    Delete $INSTDIR\doc\oodguide.pdf
    ${If} ${Errors}
        StrCpy $installerProblemIntro "The installer detected a problem with oodguide.pdf$\n$\n"
        Call InstallerProblem

        ; oodguide.pdf was not deleted, we just need to delete the copy.
        Delete $INSTDIR\doc\oodguideTest.pdf
        Abort
    ${EndIf}

    ; oodguide.pdf was deleted, we rename the copy to keep things clean
    Rename $INSTDIR\doc\oodguideTest.pdf $INSTDIR\doc\oodguide.pdf
  ${EndIf}

  ClearErrors
  CopyFiles /SILENT $INSTDIR\oodialog.dll $INSTDIR\oodialogTest.dll
  ${If} ${Errors}
      StrCpy $installerProblemIntro "The installer detected a problem trying to copy oodialog.dll$\n$\n"
      Call InstallerProblem

      ; oodialog.dll was not copied, we don't need to do anything before
      ; aborting.

      Abort
  ${EndIf}

  ClearErrors
  Delete $INSTDIR\oodialog.dll
  ${If} ${Errors}
      StrCpy $installerProblemIntro "The installer detected a problem with oodialog.dll$\n$\n"
      Call InstallerProblem

      ; oodialog.dll was not deleted, we just need to delete the copy.
      Delete $INSTDIR\oodialogTest.dll

      Abort
  ${EndIf}

  ; ooDialog.dll was deleted, we rename the copy to keep things clean
  Rename $INSTDIR\oodialogTest.dll $INSTDIR\oodialog.dll

FunctionEnd


/** RemoveFiles()
 *
 *  Deletes all the files of the current ooDialog version.  Note that deleting a
 *  non-existent file does not raise an alarm in the installer, so we just go
 *  ahead and delete things that might, or might not, be there without worrying
 *  about it.
 */
Function RemoveFiles

  DetailPrint ""
  DetailPrint "Removing current ooDialog"

  ; We probably do not need to delete these files, we could just over-write
  ; them

  Delete $INSTDIR\oodialog.dll
  Delete $INSTDIR\ooDialog.cls
  Delete $INSTDIR\oodPlain.cls
  Delete $INSTDIR\oodWin32.cls
  Delete $INSTDIR\ooRexxTry.rex

  Delete $INSTDIR\doc\oodguide.pdf
  Delete $INSTDIR\doc\oodialog.pdf
  Delete $INSTDIR\doc\ooRexxTry.pdf

  ; For the examples just delete the whole tree.
  RMDir /r $INSTDIR\samples\oodialog

  ; For the short cuts, we delete any known short cut created in any 4.0.0 or
  ; later installation.  Each independent ooDialog installation will create all
  ; relevant short cuts for that installation.
  DetailPrint "Removing ooDialog Start Menu short cuts"

  Delete "${SMooRexxFolder}\Try Rexx (GUI).lnk"

  Delete "${SMooRexxFolder}\Documentation\ooRexx ooDialog Method Reference.lnk"
  Delete "${SMooRexxFolder}\Documentation\ooRexx ooDialog Reference.lnk"
  Delete "${SMooRexxFolder}\Documentation\ooRexx ooDialog User Guide.lnk"
  Delete "${SMooRexxFolder}\Documentation\ooDialog Reference.lnk"
  Delete "${SMooRexxFolder}\Documentation\ooDialog User Guide.lnk"
  Delete "${SMooRexxFolder}\Documentation\ooRexxTry Reference.lnk"

  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Display Event Log.lnk"
  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Windows Manager.lnk"

  RMDir /r "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog"

  DetailPrint ""

FunctionEnd
