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
 *  running or the ooDialog PDF doc open.  This will prevent the installer from
 *  overwriting oodialg.dll and / or oodialog.pdf.  To prevent this, we first
 *  try to delete oodialog.pdf and oodialog.dll and abort the install if the
 *  delete fails.
 *
 *  The other problem would be if the user uninstalls ooRexx completely without
 *  first uninstalling ooDialog beta.  Some switch ooDialog stuff may not get
 *  cleaned up.
 *
 *  switchOODialog can take several arguments
 *
 *    switchOODialog /F=<version>  Force version.  <version> Must be 410 or 420
 *
 *    switchOODialog /V            Show the installer
 *                                 By default, switchOODialog is invisible.
 *                                 This is useful for debugging.
 *
 *    switchOODialog /I            Displays the current ooDialog version in
 *                                 effect.
 */


;===============================================================================
;Configuration
;===============================================================================

;--------------------------------
; Defines

  !define SHORTNAME      "Switch_ooDialog"                    ; Must be in sync with the switch ooDialog SHORTNAME
  !define LONGNAME       "Switch ooDialog ${VERSION} (beta)"  ; Does not have to be in sync
  !define OOREXXLONGNAME "Open Object Rexx"                   ; Must be kept in sync with ooRexx install

  !Define BinDir410 "${BINDIR}\oodialog410"
  !Define BinDir420 "${BINDIR}\oodialog420"
  !define SrcDir410 "${SRCDIR}\extensions\platform\windows\oodialog.4.1.0"
  !define SrcDir420 "${SRCDIR}\extensions\platform\windows\oodialog.4.2.0"
  !Define SamplesDir410 "${SRCDIR}\samples\windows\oodialog.4.1.0"
  !Define SamplesDir420 "${SRCDIR}\samples\windows\oodialog.4.2.0"

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
  OutFile "switchOODialog420_410.exe"
  ShowInstdetails show
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
  Var RegVal_startMenuFolder     ; Which start menu folder is in use for ooRexx.
  Var RegVal_installedVersion    ; Don't need this Version / level of uninstaller program.  This only exists at 410 or greater
  Var ForceVersion               ; Allows the user to force what ooDialog version is in effect.
  Var CurrentVersion             ; The current ooDialog version, read from the registry
  Var NewVersion                 ; The version of ooDialog to switch to.
  Var Message                    ; Used to hold a string.

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
; Pages.  Even though we usually do not show any pages, the INSTFILES page is
; usefull for debugging.  The /V option will let us see the page.

  !insertmacro MUI_PAGE_INSTFILES


;===============================================================================
;Installer Sections
;===============================================================================

;Section "Dummy Section" SecDummy
Section  doSwitch

  Call PrintSwitchInfo

  Call CheckForProblems

  ; Set the installation directory:
  SetOutPath "$INSTDIR"
  DetailPrint ""

  Call RemoveFiles

  ${If} $NewVersion == 420
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
    File "${SrcDir420}\oodialog.ico"
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
    File "${SrcDir420}\0_READ_ME_FIRST.txt"
    File "${SrcDir420}\build_ooDialog_cls.rex"
    File "${SrcDir420}\AnimatedButton.cls"
    File "${SrcDir420}\BaseDialog.cls"
    File "${SrcDir420}\CategoryDialog.cls"
    File "${SrcDir420}\ControlDialog.cls"
    File "${SrcDir420}\DeprecatedClasses.cls"
    File "${SrcDir420}\DialogControls.cls"
    File "${SrcDir420}\DialogExtensions.cls"
    File "${SrcDir420}\DynamicDialog.cls"
    File "${SrcDir420}\EventNotification.cls"
    File "${SrcDir420}\Menu.cls"
    File "${SrcDir420}\PlainBaseDialog.cls"
    File "${SrcDir420}\PropertySheet.cls"
    File "${SrcDir420}\RcDialog.cls"
    File "${SrcDir420}\ResDialog.cls"
    File "${SrcDir420}\UserDialog.cls"
    File "${SrcDir420}\UtilityClasses.cls"

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

    ; Record the ooDialog version now in effect:
    DetailPrint "Recording ooDialog version now in effect: 4.2.0"
    WriteRegStr HKLM "Software\${SHORTNAME}" "CurrentVersion" "420"
  ${Else}
    DetailPrint "********** ooDialog 4.1.0 Framework **********"
    File "${BinDir410}\oodialog.dll"
    File "${BinDir410}\OODIALOG.CLS"
    File "${BinDir410}\OODPLAIN.CLS"
    File "${BinDir410}\OODWIN32.CLS"
    DetailPrint ""

    DetailPrint "********** ooDialog 4.1.0 ooRexxTry **********"
    File "${SamplesDir410}\ooRexxTry\ooRexxTry.rex"
    DetailPrint ""

    DetailPrint "********** ooDialog 4.1.0 Documentation **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\doc
    ; Add the files ...
    File /oname=ooDialog.pdf "${SRCDIR}\doc\oodialog410.pdf"
    DetailPrint ""

    DetailPrint "********** ooDialog 4.1.0 Samples **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog
    ; Add the files ...
    File "${SrcDir410}\oodialog.ico"
    File "${SamplesDir410}\*.rex"
    File "${SamplesDir410}\*.inp"
    File "${SamplesDir410}\*.ico"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\bmp
    ; Add the files ...
    File "${SamplesDir410}\bmp\*.bmp"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\examples
    ; Add the files ...
    File "${SamplesDir410}\examples\*.rex"
    File "${SamplesDir410}\examples\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\examples\resources
    ; Add the files ...
    File "${SamplesDir410}\examples\resources\*.bmp"
    File "${SamplesDir410}\examples\resources\*.h"
    File "${SamplesDir410}\examples\resources\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\oleinfo
    ; Add the files ...
    File "${SamplesDir410}\oleinfo\*.rex"
    File "${SamplesDir410}\oleinfo\*.txt"
    File "${SamplesDir410}\oleinfo\*.bmp"
    File "${SamplesDir410}\oleinfo\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\ooRexxTry
    ; Add the files ...
    File "${SamplesDir410}\ooRexxTry\ooRexxTry.rex"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\ooRexxTry\doc
    ; Add the files ...
    File "${SamplesDir410}\ooRexxTry\doc\ooRexxTry.pdf"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\rc
    ; Add the files ...
    File "${SamplesDir410}\rc\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\res
    ; Add the files ...
    File "${SamplesDir410}\res\*.res"
    File "${SamplesDir410}\res\*.dll"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\sysinfo
    ; Add the files ...
    File "${SamplesDir410}\sysinfo\*.rex"
    File "${SamplesDir410}\sysinfo\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\source
    ; Add the files ...
    File "${BinDir410}\OODIALOG.CLS"
    File "${BinDir410}\OODWIN32.CLS"
    File "${BinDir410}\OODPLAIN.CLS"
    File "${SrcDir410}\advctrl.cls"
    File "${SrcDir410}\anibuttn.cls"
    File "${SrcDir410}\basedlg.cls"
    File "${SrcDir410}\build.rex"
    File "${SrcDir410}\catdlg.cls"
    File "${SrcDir410}\dialog.cls"
    File "${SrcDir410}\dlgext.cls"
    File "${SrcDir410}\dyndlg.cls"
    File "${SrcDir410}\makedll.bat"
    File "${SrcDir410}\msgext.cls"
    File "${SrcDir410}\oodutils.cls"
    File "${SrcDir410}\plbdlg.cls"
    File "${SrcDir410}\pludlg.cls"
    File "${SrcDir410}\propsht.cls"
    File "${SrcDir410}\resdlg.cls"
    File "${SrcDir410}\stddlg.cls"
    File "${SrcDir410}\stdext.cls"
    File "${SrcDir410}\userdlg.cls"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\tutorial
    ; Add the files ...
    File "${SamplesDir410}\tutorial\*.rex"
    File "${SamplesDir410}\tutorial\*.bmp"
    File "${SamplesDir410}\tutorial\*.rc"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\wav
    ; Add the files ...
    File "${SamplesDir410}\wav\*.wav"
    File "${SamplesDir410}\wav\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\winsystem
    ; Add the files ...
    File "${SamplesDir410}\winsystem\*.rex"
    File "${SamplesDir410}\winsystem\*.rc"
    File "${SamplesDir410}\winsystem\*.h"
    File "${SamplesDir410}\winsystem\*.frm"
    DetailPrint ""

    ; Record the ooDialog version now in effect:
    DetailPrint "Recording ooDialog version now in effect: 4.1.0"
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

  /* We read the current version key first so we can do an early abort for the
   * /I option.
   */
  ReadRegStr $CurrentVersion HKLM "Software\${SHORTNAME}" "CurrentVersion"

  /* Check if the user just wants to know the ooDialog version in effect. */
  ClearErrors
  ${GetOptions} "$CMDLINE" "/I"  $R0
  ${IfNot} ${Errors}
    Call DisplayInfo
  ${Endif}

  ClearErrors
  ${GetOptions} "$CMDLINE" "/?"  $R0
  ${IfNot} ${Errors}
    Call DisplayInfo
  ${Endif}

  /* Check if the user is forcing the current version to something specific. */
  ClearErrors
  ${GetOptions} "$CMDLINE" "/F="  $R0
  StrCpy $ForceVersion $R0

  /* Check if the user is forcing us to be visible.  Usefull for debugging.
   * This logic is a little convoluted.  We check the command line for the
   * presence of /V.  If it is not there, the error flag is set, and we force
   * the installer to silent mode.
   */
  ClearErrors
  ${GetOptions} "$CMDLINE" "/V"  $R0
  ${If} ${Errors}
    SetSilent silent
    ClearErrors
  ${Endif}

  /* Be sure we are installed properly. Note that the UnInstallLocation and the
   * InstallLocation keys in the registry contain the same directory.  But, the
   * UnInstallLocation key has the directory unquoted and InstallLocation quotes
   * the directory.  For some reason, copying the quoted version drops the ':'
   * out of the path, causing things to fail.
   */
  ReadRegStr $RegVal_installedLocation HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "UnInstallLocation"
  ${If} $RegVal_installedLocation == ""
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "${LONGNAME} has not been installed properly.$\n$\n\
      Please follow the guidelines published for using$\n\
      Switch ooDialog, especially the instructions for$\n\
      installation.$\n$\n\
      Aborting."
    Abort
  ${Endif}

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

  StrCpy $INSTDIR "$RegVal_installedLocation"

  ReadRegStr $RegVal_startMenuFolder HKLM "Software\${SHORTNAME}\" "StartMenuFolder"

  /* If the user did not force a version, the new version is the opposite of
   * the current version.
   */
  ${if} $ForceVersion == ""
    ${If} $CurrentVersion == 410
      StrCpy $NewVersion 420
    ${Else}
      StrCpy $NewVersion 410
    ${Endif}
  ${else}
    ${If} $ForceVersion == 410
      StrCpy $NewVersion 410
      StrCpy $CurrentVersion 420
    ${Else}
      StrCpy $NewVersion 420
      StrCpy $CurrentVersion 410
    ${Endif}
  ${endif}

FunctionEnd

/** CheckForProblems()
 *
 *  If the ooDialog doc is open, it can not be overwritten and the switched to
 *  version will have the wrong doc.  Or, if a Rexx ooDialog program is running
 *  oodialog.dll can not be overwritten, and the switched to version will not
 *  work.
 *
 *  We check for this possible problem by trying to delete the files.  If we
 *  can not, we abort, keeping the current version intact.
 */
Function CheckForProblems

  ClearErrors
  Delete $INSTDIR\doc\oodialog.pdf
  ${If} ${Errors}
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "switchOODialog detected a problem with oodialog.pdf$\n$\n\
      You MUST close the ooDialog reference documentation and close$\n\
      any running ooDialog programs before executing the Switch$\n\
      ooDialog program.$\n$\n\
      Please close the ooDialog Method Reference documentation, and$\n\
      make sure no ooDialog programs are running.$\n$\n\
      switchOODialog is aborting."
      Abort
  ${EndIf}

  ${If} $CurrentVersion == 420
    ClearErrors
    Delete $INSTDIR\oodguide.pdf
    ${If} ${Errors}
      MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
        "switchOODialog detected a problem with oodguide.pdf$\n$\n\
        You MUST close all the ooDialog documentation and close$\n\
        any running ooDialog programs before executing the Switch$\n\
        ooDialog program.$\n$\n\
        Please close the ooDialog User Guid documentation, and$\n\
        make sure no ooDialog programs are running.$\n$\n\
        switchOODialog is aborting."
        Abort

        ; Restore the oodialog.pdf file deleted above.
        SetOutPath $INSTDIR\doc

        ${If} $CurrentVersion == 420
          File /oname=ooDialog.pdf "${SRCDIR}\doc\oodialog420.pdf"
        ${Else}
          File /oname=ooDialog.pdf "${SRCDIR}\doc\oodialog410.pdf"
        ${EndIf}

        Abort
    ${EndIf}
  ${EndIf}

  ClearErrors
  Delete $INSTDIR\oodialog.dll
  ${If} ${Errors}
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST \
      "switchOODialog detected a problem with oodialog.dll$\n$\n\
      You MUST close any running ooDialog programs and close$\n\
      the ooDialog reference documentation before executing$\n\
      the Switch ooDialog program.$\n$\n\
      Please close any running ooDialog programs and make sure$\n\
      the ooDialog Method Reference documentation is closed.$\n$\n\
      switchOODialog is aborting."

      ; Restore the ooDialog docs deleted above.
      SetOutPath $INSTDIR\doc

      ${If} $CurrentVersion == 420
        File /oname=ooDialog.pdf "${SRCDIR}\doc\oodialog420.pdf"
        File /oname=oodGuide.pdf "${SRCDIR}\doc\oodguide420.pdf"
      ${Else}
        File /oname=ooDialog.pdf "${SRCDIR}\doc\oodialog410.pdf"
      ${EndIf}

      Abort
  ${EndIf}

FunctionEnd

/** DisplayInfo
 *
 * Puts up a message box to show information on the switchDialog program.
 * Called when the /I arg is used.
 */
Function DisplayInfo

  StrCpy $Message \
       "Switches the active version of ooDialog.  Syntax:$\n$\n\
        switchOODialog [param]$\n$\n\
        With no argument switches the active ooDialog to the opposite.$\n\
        Valid arguments are:$\n$\n\
        /V		Switch ooDialog 'Visibly'.$\n$\n\
        /F=key		Force the active version to 'key'.$\n$\n\
        /I		Show this information.$\n$\n\
        /?		Same as /I.$\n$\n\
        'key' must be 420 or 410."

  ${If} $CurrentVersion == 410
    MessageBox MB_OK "Switch ooDialog  4.2.0 <<-->> 4.1.0$\n$\n\
                      Switch ooDialog version:$\t${VERSION}$\n\
                      Active version of ooDialog:$\t4.1.0$\n$\n$\n$Message"

  ${Else}
    MessageBox MB_OK "Switch ooDialog  4.2.0 <<-->> 4.1.0$\n$\n\
                      Switch ooDialog version:$\t${VERSION}$\n\
                      Active version of ooDialog:$\t4.2.0$\n$\n$\n$Message"
  ${EndIf}

  Abort
FunctionEnd

/** PrintSwitchInfo()
 *
 *  Print some info on what the switch is doing.  This is used for debugging.
 *  This is never seen, unless the /V options is used.
 */
Function PrintSwitchInfo

  ${If} $NewVersion == 420
    DetailPrint "Current ooDialog Version is: 4.1.0"
    DetailPrint "Switching to Version:        4.2.0"
    DetailPrint ""
  ${Else}
    DetailPrint "Current ooDialog Version is: 4.2.0"
    DetailPrint "Switching to Version:        4.1.0"
    DetailPrint ""
  ${Endif}

FunctionEnd

/** RemoveFiles()
 *
 *  Deletes all the files in the current ooDialog version that are not present
 *  in the switch to version.
 */
Function RemoveFiles

  ${If} $NewVersion == 420
    DetailPrint "Removing files not present in ooDialog 4.2.0"

    RMDir /r $INSTDIR\samples\oodialog\source
    Delete $INSTDIR\samples\oodialog\bmp\movie.bmp
    Delete $INSTDIR\samples\oodialog\bmp\oodlist1.bmp
    Delete $INSTDIR\samples\oodialog\bmp\oodlist2.bmp
    Delete $INSTDIR\samples\oodialog\bmp\psdemolv.bmp
    Delete $INSTDIR\samples\oodialog\bmp\psdemoTab.bmp
    Delete $INSTDIR\samples\oodialog\bmp\psdemotv.bmp
    Delete $INSTDIR\samples\oodialog\bmp\ticket.bmp
    Delete $INSTDIR\samples\oodialog\examples\readme.txt
    Delete $INSTDIR\samples\oodialog\oodlist.rex
    Delete $INSTDIR\samples\oodialog\oopet.rex
    Delete $INSTDIR\samples\oodialog\oopet1.rex
    Delete $INSTDIR\samples\oodialog\ooticket.rex
    Delete $INSTDIR\samples\oodialog\propdemo.rex
    Delete $INSTDIR\samples\oodialog\rc\movies.rc
    Delete $INSTDIR\samples\oodialog\rc\oodlist.rc
    Delete $INSTDIR\samples\oodialog\rc\oopet.rc
    Delete $INSTDIR\samples\oodialog\rc\propdemo.rc
    Delete $INSTDIR\samples\oodialog\rc\ticket.rc
    Delete $INSTDIR\samples\oodialog\res\oopet.dll
    Delete $INSTDIR\samples\oodialog\tutorial\em_categ.rc
    Delete $INSTDIR\samples\oodialog\tutorial\em_categ.rex
    Delete $INSTDIR\samples\oodialog\tutorial\emp_mend.rex
    Delete $INSTDIR\samples\oodialog\tutorial\emp_menu.rex
    Delete $INSTDIR\samples\oodialog\tutorial\empvalid.rex
    Delete $INSTDIR\samples\oodialog\tutorial\persdata.rc
    Delete $INSTDIR\samples\oodialog\tutorial\textscrl.rc
    Delete $INSTDIR\samples\oodialog\tutorial: textscrl.rex
  ${Else}
    DetailPrint "Removing files not present in ooDialog 4.1.0"
    DetailPrint ""
    DetailPrint "Removing ooDialog User Guide and Start Menu shortcut"
    Delete "$RegVal_startMenuFolder\Documentation\ooRexx ooDialog User Guide.lnk"
    Delete $INSTDIR\doc\oodguide.pdf

    RMDir /r $INSTDIR\samples\oodialog\source
    Delete $INSTDIR\samples\oodialog\AnimalGame.rex
    Delete $INSTDIR\samples\oodialog\bmp\oodListViews1.bmp
    Delete $INSTDIR\samples\oodialog\bmp\oodListViews2.bmp
    Delete $INSTDIR\samples\oodialog\bmp\propertySheetDemoListView.bmp
    Delete $INSTDIR\samples\oodialog\bmp\propertySheetDemoTab.bmp
    Delete $INSTDIR\samples\oodialog\bmp\propertySheetDemoTreeView.bmp
    Delete $INSTDIR\samples\oodialog\bmp\TabOwnerDemoListView.bmp
    Delete $INSTDIR\samples\oodialog\bmp\TabOwnerDemoTab.bmp
    Delete $INSTDIR\samples\oodialog\bmp\TabOwnerDemoTreeView.bmp
    Delete $INSTDIR\samples\oodialog\bmp\ticketWizardMovie.bmp
    Delete $INSTDIR\samples\oodialog\bmp\ticketWizardRexxLA.bmp
    Delete $INSTDIR\samples\oodialog\bmp\ticketWizardTheater.bmp
    Delete $INSTDIR\samples\oodialog\bmp\ticketWizardTicket.bmp
    RMDir /r $INSTDIR\samples\oodialog\controls
    Delete $INSTDIR\samples\oodialog\dlgAreaUDemo.h
    Delete $INSTDIR\samples\oodialog\dlgAreaUDemoThree.rex
    Delete $INSTDIR\samples\oodialog\dlgAreaUDemoTwo.rex
    Delete $INSTDIR\samples\oodialog\examples\ReadMe.txt
    Delete $INSTDIR\samples\oodialog\examples\resources\CheckIn.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\ClosePalette.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\CodeReview.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\LinkToWeb.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\LockModule.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\LockProject.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\ProjectReview.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\SaveAll.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\SaveModule.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\SaveProject.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\SplitModule.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\Update.bmp
    Delete $INSTDIR\samples\oodialog\examples\resources\useTools.h
    Delete $INSTDIR\samples\oodialog\examples\useTools.rex
    RMDir /r $INSTDIR\samples\oodialog\menus
    RMDir /r $INSTDIR\samples\oodialog\mouse
    Delete $INSTDIR\samples\oodialog\oodListViews.rex
    Delete $INSTDIR\samples\oodialog\ooDraw.h
    Delete $INSTDIR\samples\oodialog\PropertySheetDemo.rex
    Delete $INSTDIR\samples\oodialog\rc\AnimalGame.h
    Delete $INSTDIR\samples\oodialog\rc\AnimalGame.rc
    Delete $INSTDIR\samples\oodialog\rc\ldvideo.h
    Delete $INSTDIR\samples\oodialog\rc\oodListViews.h
    Delete $INSTDIR\samples\oodialog\rc\oodListViews.rc
    Delete $INSTDIR\samples\oodialog\rc\PropertySheetDemo.h
    Delete $INSTDIR\samples\oodialog\rc\PropertySheetDemo.rc
    Delete $INSTDIR\samples\oodialog\rc\TabDemo.h
    Delete $INSTDIR\samples\oodialog\rc\TabOwnerDemo.h
    Delete $INSTDIR\samples\oodialog\rc\TabOwnerDemo.rc
    Delete $INSTDIR\samples\oodialog\rc\ticketWizard.h
    Delete $INSTDIR\samples\oodialog\rc\ticketWizard.rc
    Delete $INSTDIR\samples\oodialog\res\AnimalGame.dll
    Delete $INSTDIR\samples\oodialog\res\PropertySheetDemo.dll
    Delete $INSTDIR\samples\oodialog\res\TabOwnerDemo.dll
    Delete $INSTDIR\samples\oodialog\samplesSetup.rex
    RMDir /r $INSTDIR\samples\oodialog\simple
    Delete $INSTDIR\samples\oodialog\sysinfo\sysInfo.h
    Delete $INSTDIR\samples\oodialog\TabDemo.rex
    Delete $INSTDIR\samples\oodialog\TabOwnerDemo.rex
    Delete $INSTDIR\samples\oodialog\ticketWizard.rex
    Delete $INSTDIR\samples\oodialog\tutorial\employe4validate.rex
    Delete $INSTDIR\samples\oodialog\tutorial\employe7.h
    Delete $INSTDIR\samples\oodialog\tutorial\employe9menuDyn.rex
    Delete $INSTDIR\samples\oodialog\tutorial\employee10menuRc.rex
    Delete $INSTDIR\samples\oodialog\tutorial\employee11tab.db
    Delete $INSTDIR\samples\oodialog\tutorial\employee11tab.h
    Delete $INSTDIR\samples\oodialog\tutorial\employee11tab.rc
    Delete $INSTDIR\samples\oodialog\tutorial\employee11tab.rex
    Delete $INSTDIR\samples\oodialog\tutorial\textScroll.rc
    Delete $INSTDIR\samples\oodialog\tutorial\textScroll.rex
    RMDir /r $INSTDIR\samples\oodialog\userGuide
    Delete $INSTDIR\samples\oodialog\UserTabDemo.h
    Delete $INSTDIR\samples\oodialog\UserTabDemo.rc
    Delete $INSTDIR\samples\oodialog\UserTabDemo.rex
  ${Endif}

  DetailPrint ""

FunctionEnd
