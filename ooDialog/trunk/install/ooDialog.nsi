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
 *    makensis /DVERSION=x.x.x.x /DNODOTVER=xxx /DSRCDIR=y /DBINDIR=xxx /DCPU=xxx ooDialog.nsi
 *  eg
 *    makensis /DVERSION=4.2.0.6367 /DNODOTVER=420 /DSRCDIR=C:\work\wc\ooDialog\trunk /DBINDIR=C:\work\wc\ooDialog\trunk\Win32Rel /DCPU=x86_32 ooDialog.nsi
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
  !define DISPLAYICON    "$INSTDIR\ooDialog.dll,2"                   ; Use the ooDialog icon, the OOD icon
  !define REXXSHORTNAME  "ooRexx"                                    ; From ooRexx installer.
  !define REXXLONGNAME   "Open Object Rexx"                          ; From ooRexx installer

  !define SrcDir "${SRCDIR}\oodialog"
  !Define BinDir420 "${BINDIR}"
  !define SamplesDir420 "${SRCDIR}\examples"

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
  OutFile "ooDialog-${VERSION}-${CPU}.exe"
  ShowInstdetails show
  SetOverwrite on
  SetPluginUnload alwaysoff
  RequestExecutionLevel admin
  InstallDir "$PROGRAMFILES\ooRexx"


;--------------------------------
;Interface Settings

  !define MUI_ICON "${SRCDIR}\ooDialog\AppIcon2.ico"
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
  Var RegVal_installedVersion       ; Version / level of ooRexx uninstaller program.  This only exists at 410 or greater
  Var RegVal_startMenuFolder        ; Which start menu folder is in use for ooRexx.
  Var IsAdminUser                   ; is the installer being run by an admin:           true / false

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${SRCDIR}\install\CPLv1.0.txt"
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;===============================================================================
;Installer Sections
;===============================================================================

Section  doInstall

  Call CheckForProblems

  DetailPrint "********** Installing ooDialog  **********"
  DetailPrint ""

  Call RemoveFiles

  ; Install the files
  SetOutPath "$INSTDIR"

    DetailPrint "********** ooDialog 4.2.0 Framework **********"
    File "${BinDir420}\oodialog.dll"
    File "${BinDir420}\ooDialog.cls"
    File "${BinDir420}\oodPlain.cls"
    File "${BinDir420}\oodWin32.cls"
    DetailPrint ""

    DetailPrint "********** ooDialog 4.2.0 ooRexxTry **********"
    File "${SamplesDir420}\ooRexxTry\ooRexxTry.rex"
    DetailPrint ""

    DetailPrint "********** ooDialog 4.2.0 Documentation **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\doc
    ; Add the files ...
    File /oname=ooDialog.pdf "${SRCDIR}\doc\oodialog420.pdf"
    File /oname=oodGuide.pdf "${SRCDIR}\doc\oodguide420.pdf"

    CreateShortCut "$RegVal_startMenuFolder\Documentation\ooRexx ooDialog User Guide.lnk" "$INSTDIR\doc\oodguide.pdf" "" "$INSTDIR\doc\oodguide.pdf" 0
    DetailPrint ""

    DetailPrint "********** ooDialog 4.2.0 Samples **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog
    ; Add the files ...
    File "${SrcDir}\oodialog.ico"
    File "${SamplesDir420}\*.rex"
    File "${SamplesDir420}\*.h"
    File "${SamplesDir420}\*.inp"
    File "${SamplesDir420}\*.ico"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\bmp
    ; Add the files ...
    File "${SamplesDir420}\bmp\*.bmp"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls
    ; Add the files ...
    File "${SamplesDir420}\controls\*.rex"
    File "${SamplesDir420}\controls\*.rc"
    File "${SamplesDir420}\controls\*.h"
    File "${SamplesDir420}\controls\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\examples
    ; Add the files ...
    File "${SamplesDir420}\examples\*.rex"
    File "${SamplesDir420}\examples\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\examples\resources
    ; Add the files ...
    File "${SamplesDir420}\examples\resources\*.bmp"
    File "${SamplesDir420}\examples\resources\*.h"
    File "${SamplesDir420}\examples\resources\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\menus
    ; Add the files ...
    File "${SamplesDir420}\menus\*.rex"
    File "${SamplesDir420}\menus\*.h"
    File "${SamplesDir420}\menus\*.bmp"
    File "${SamplesDir420}\menus\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\mouse
    ; Add the files ...
    File "${SamplesDir420}\mouse\*.rex"
    File "${SamplesDir420}\mouse\*.h"
    File "${SamplesDir420}\mouse\*.rc"
    File "${SamplesDir420}\mouse\*.cur"
    File "${SamplesDir420}\mouse\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\oleinfo
    ; Add the files ...
    File "${SamplesDir420}\oleinfo\*.rex"
    File "${SamplesDir420}\oleinfo\*.txt"
    File "${SamplesDir420}\oleinfo\*.bmp"
    File "${SamplesDir420}\oleinfo\*.rc"

    SetOutPath $INSTDIR\samples\oodialog\ooRexxTry
    ; Add the files ...
    File "${SamplesDir420}\ooRexxTry\ooRexxTry.rex"

    SetOutPath $INSTDIR\samples\oodialog\ooRexxTry\doc
    ; Add the files ...
    File "${SamplesDir420}\ooRexxTry\doc\ooRexxTry.pdf"

    SetOutPath $INSTDIR\samples\oodialog\propertySheet.tabs
    ; Add the files ...
    File "${SamplesDir420}\propertySheet.tabs\*.cls"
    File "${SamplesDir420}\propertySheet.tabs\oodListViews.rex"
    File "${SamplesDir420}\propertySheet.tabs\PropertySheetDemo.rex"
    File "${SamplesDir420}\propertySheet.tabs\TabDemo.rex"
    ; File "${SamplesDir420}\propertySheet.tabs\TabOwnerDemo.rex" ; Do not expose this example yet
    File "${SamplesDir420}\propertySheet.tabs\ticketWizard.rex"
    File "${SamplesDir420}\propertySheet.tabs\UserTabDemo.rex"


    SetOutPath $INSTDIR\samples\oodialog\propertySheet.tabs\rc
    ; Add the files ...
    File "${SamplesDir420}\propertySheet.tabs\rc\oodListViews*"
    File "${SamplesDir420}\propertySheet.tabs\rc\PropertySheetDemo*"
    ; File "${SamplesDir420}\propertySheet.tabs\rc\TabOwnerDemo*" ; Do not expose this example yet
    File "${SamplesDir420}\propertySheet.tabs\rc\ticketWizard*"
    File "${SamplesDir420}\propertySheet.tabs\rc\UserTabDemo*"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\rc
    ; Add the files ...
    File "${SamplesDir420}\rc\*.rc"
    File "${SamplesDir420}\rc\*.h"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\res
    ; Add the files ...
    File "${SamplesDir420}\res\*.dll"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\simple
    ; Add the files ...
    File "${SamplesDir420}\simple\*.h"
    File "${SamplesDir420}\simple\*.rc"
    File "${SamplesDir420}\simple\*.rex"
    File "${SamplesDir420}\simple\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\sysinfo
    ; Add the files ...
    File "${SamplesDir420}\sysinfo\*.rex"
    File "${SamplesDir420}\sysinfo\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\source
    ; Add the files ...
    File "${BinDir420}\ooDialog.cls"
    File "${BinDir420}\oodWin32.cls"
    File "${BinDir420}\oodPlain.cls"
    File "${SrcDir}\0_READ_ME_FIRST.txt"
    File "${SrcDir}\build_ooDialog_cls.rex"
    File "${SrcDir}\AnimatedButton.cls"
    File "${SrcDir}\BaseDialog.cls"
    File "${SrcDir}\CategoryDialog.cls"
    File "${SrcDir}\ControlDialog.cls"
    File "${SrcDir}\DeprecatedClasses.cls"
    File "${SrcDir}\DialogControls.cls"
    File "${SrcDir}\DialogExtensions.cls"
    File "${SrcDir}\DynamicDialog.cls"
    File "${SrcDir}\EventNotification.cls"
    File "${SrcDir}\Menu.cls"
    File "${SrcDir}\PlainBaseDialog.cls"
    File "${SrcDir}\PropertySheet.cls"
    File "${SrcDir}\RcDialog.cls"
    File "${SrcDir}\ResDialog.cls"
    File "${SrcDir}\UserDialog.cls"
    File "${SrcDir}\UtilityClasses.cls"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\tutorial
    ; Add the files ...
    File "${SamplesDir420}\tutorial\*.rex"
    File "${SamplesDir420}\tutorial\*.bmp"
    File "${SamplesDir420}\tutorial\*.rc"
    File "${SamplesDir420}\tutorial\*.h"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\userGuide
    ; Add the files ...
    File "${SamplesDir420}\userGuide\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise02
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise02\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise03
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise03\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise04
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise04\*.h"
    File "${SamplesDir420}\userGuide\exercises\Exercise04\*.rc"
    File "${SamplesDir420}\userGuide\exercises\Exercise04\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise05
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise05\*.h"
    File "${SamplesDir420}\userGuide\exercises\Exercise05\*.rc"
    File "${SamplesDir420}\userGuide\exercises\Exercise05\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise05\res
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise05\res\res.mak"
    File "${SamplesDir420}\userGuide\exercises\Exercise05\res\ProductView.dll"
    File "${SamplesDir420}\userGuide\exercises\Exercise05\res\ProductIcon.bmp"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Customer
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Customer\*.h"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Customer\*.rc"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Customer\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Customer\bmp
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Customer\bmp\*.ico"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Customer\bmp\*.bmp"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Order
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Order\*.h"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Order\*.rc"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Order\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Order\bmp
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Order\bmp\*.ico"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Order\bmp\*.bmp"

    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\OrderMgr
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\OrderMgr\*.h"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\OrderMgr\*.rc"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\OrderMgr\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Product
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Product\*.h"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Product\*.rc"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Product\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Product\res
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Product\res\*.ico"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Product\res\*.bmp"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Product\res\*.dll"
    File "${SamplesDir420}\userGuide\exercises\Exercise06\Product\res\res.mak"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Samples\ReadMe.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples\DlgData
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Samples\DlgData\*.h"
    File "${SamplesDir420}\userGuide\exercises\Samples\DlgData\*.rc"
    File "${SamplesDir420}\userGuide\exercises\Samples\DlgData\*rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples\DlgData\res
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Samples\DlgData\res\res.mak"
    File "${SamplesDir420}\userGuide\exercises\Samples\DlgData\res\ASimpleDialog.dll"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Samples\Popups
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Samples\Popups\*rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Support
    ; Add the files ...
    File "${SamplesDir420}\userGuide\exercises\Support\*.cls"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\wav
    ; Add the files ...
    File "${SamplesDir420}\wav\*.wav"
    File "${SamplesDir420}\wav\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\winsystem
    File "${SamplesDir420}\winsystem\*.rex"
    File "${SamplesDir420}\winsystem\*.rc"
    File "${SamplesDir420}\winsystem\*.h"
    File "${SamplesDir420}\winsystem\*.frm"
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
  ReadRegStr $RegVal_installedVersion HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REXXSHORTNAME}" "UninstallVersion"
  ReadRegStr $RegVal_startMenuFolder HKLM "Software\${SHORTNAME}\" "StartMenuFolder"

  ${if} $RegVal_installedVersion == ""
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "A version of ooRexx, greater than or equal to$\n\
      version 4.1.0, must be installed prior to the$\n\
      installation of ${LONGNAME}.$\n$\n\
      The installer is aborting."
      Abort
  ${endif}

  StrCpy $INSTDIR $RegVal_installedLocation

FunctionEnd


/** CheckForProblems()
 *
 *  If the ooDialog doc is open, it can not be overwritten.  Or, if a Rexx
 *  ooDialog program is running, oodialog.dll can not be overwritten.  This will
 *  prevent a clean install.
 *
 *  We check for this possible problem by trying to rename the files.  If we
 *  can not, we abort, keeping the current version intact.
 */
Function CheckForProblems

  ClearErrors
  Rename $INSTDIR\doc\oodialog.pdf $INSTDIR\doc\oodialogTest.pdf
  ${If} ${Errors}
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "The installer detected a problem with oodialog.pdf$\n$\n\
      You MUST close the ooDialog reference documentation and close$\n\
      any running ooDialog programs before executing the ooDialog$\n\
      installation program.$\n$\n\
      Please close the ooDialog Method Reference documentation, and$\n\
      make sure no ooDialog programs are running.$\n$\n\
      The ooDialog installer is aborting."
      Abort
  ${EndIf}

  ; We do not try oodguide.pdf because if it does not exist, Errors will be set,
  ; so we can not differentiate between an erro because the file is opened or
  ; because it does not exist.

  ClearErrors
  Rename $INSTDIR\oodialog.dll $INSTDIR\oodialogTest.dll
  ${If} ${Errors}
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "The installer detected a problem with oodialog.dll$\n$\n\
      You MUST close any running ooDialog programs and close$\n\
      the ooDialog reference documentation before executing$\n\
      the ooDialog installation program.$\n$\n\
      Please close any running ooDialog programs and make sure$\n\
      the ooDialog Method Reference documentation is closed.$\n$\n\
      The ooDialog installer is aborting."

      ; Restore the ooDialog doc renamed above.
      Rename $INSTDIR\doc\oodialogTest.pdf $INSTDIR\doc\oodialog.pdf

      Abort
  ${EndIf}

  ; Rename the files back so we don't orphen the renamed files.
  Rename $INSTDIR\doc\oodialogTest.pdf $INSTDIR\doc\oodialog.pdf
  Rename $INSTDIR\oodialog.dll $INSTDIR\oodialogTest.dll

FunctionEnd


/** RemoveFiles()
 *
 *  Deletes all the files of the current ooDialog version
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

  ; For the examples just delete the whole tree.
  RMDir /r $INSTDIR\samples\oodialog

  DetailPrint ""

FunctionEnd
