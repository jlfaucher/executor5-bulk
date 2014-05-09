/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2014 Rexx Language Association. All rights reserved.    */
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
  !define MUI_COMPONENTSPAGE
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
  Var keyFileName                   ; Used to construct the full path name of a check for locked file.

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${ROOTDIR}\install\CPLv1.0.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;===============================================================================
;Installer Sections
;===============================================================================

;-------------------------------------------------------------------------------
;  Hidden section to delete the existing ooDialog

Section -deleteExisting
  Call RemoveFiles
SectionEnd

;-------------------------------------------------------------------------------
; Core component
Section  "ooDialog ${SHORTVERSION} Core (required)" SecCore
  SectionIn 1 RO

  DetailPrint "********** Installing ooDialog  **********"
  DetailPrint "  NSIS installer with max string length ${NSIS_MAX_STRLEN}"
  DetailPrint ""


  ; Install the files
  SetOutPath "$INSTDIR"

    DetailPrint "********** ooDialog ${SHORTVERSION} Framework **********"
    File "${BinDir}\ooDialog.cls"
    File "${BinDir}\ooDialog.com"
    File "${BinDir}\oodialog.dll"
    File "${BinDir}\ooDialog.exe"
    File "${BinDir}\oodPlain.cls"
    File "${BinDir}\oodWin32.cls"
    File "${BinDir}\ooShapes.cls"
    File "${BinDir}\ooShapes.dll"
    DetailPrint ""

    DetailPrint "********** ooDialog ${SHORTVERSION} ooRexxTry **********"
    File "${ExamplesDir}\ooRexxTry\ooRexxTry.rex"
    CreateShortCut "${SMooRexxFolder}\Try Rexx (GUI).lnk" "$INSTDIR\rexx.exe" '"$INSTDIR\ooRexxTry.rex"' "$INSTDIR\rexx.exe"
    DetailPrint ""
SectionEnd

;-------------------------------------------------------------------------------
; Documentation component

Section  "ooDialog ${SHORTVERSION} Documentation" SecDoc

    DetailPrint "********** ooDialog ${SHORTVERSION} Documentation **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\doc
    ; Add the files ...
    File "${ROOTDIR}\doc\oodialog.pdf"
    File "${ROOTDIR}\doc\oodguide.pdf"
    File "${ROOTDIR}\doc\ooDialog_ReleaseNotes.txt"
    File "${ExamplesDir}\ooRexxTry\doc\ooRexxTry.pdf"

    CreateShortCut  "${SMooRexxFolder}\ooRexx Documentation\ooDialog Reference.lnk" "$INSTDIR\doc\oodialog.pdf" "" "$INSTDIR\doc\oodialog.pdf" 0
    CreateShortCut  "${SMooRexxFolder}\ooRexx Documentation\ooDialog User Guide.lnk" "$INSTDIR\doc\oodguide.pdf" "" "$INSTDIR\doc\oodguide.pdf" 0
    CreateShortCut  "${SMooRexxFolder}\ooRexx Documentation\ooDialog Release Notes.lnk" "$INSTDIR\doc\ooDialog_ReleaseNotes.txt" "" "$INSTDIR\doc\ooDialog_ReleaseNotes.txt" 0
    CreateShortCut  "${SMooRexxFolder}\ooRexx Documentation\ooRexxTry Reference.lnk" "$INSTDIR\doc\ooRexxTry.pdf" "" "$INSTDIR\doc\ooRexxTry.pdf" 0
    DetailPrint ""
SectionEnd

;-------------------------------------------------------------------------------
; Examples component
Section  "ooDialog ${SHORTVERSION} Examples" SecExample

    DetailPrint "********** ooDialog ${SHORTVERSION} Samples **********"
    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog
    ; Add the files ...
    File "${SrcDir}\oodialog.ico"
    File "${ExamplesDir}\*.rex"
    File "${ExamplesDir}\*.h"
    File "${ExamplesDir}\*.ico"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\bmp
    ; Add the files ...
    File "${ExamplesDir}\bmp\*.bmp"
    File "${ExamplesDir}\bmp\*.ico"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls
    ; Add the files ...
    File "${ExamplesDir}\controls\*.rex"
    File "${ExamplesDir}\controls\*.rc"
    File "${ExamplesDir}\controls\*.h"
    File "${ExamplesDir}\controls\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ComboBox
    ; Add the files ...
    File "${ExamplesDir}\controls\ComboBox\*.h"
    File "${ExamplesDir}\controls\ComboBox\*.rc"
    File "${ExamplesDir}\controls\ComboBox\*.rex"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ListBox
    ; Add the files ...
    File "${ExamplesDir}\controls\ListBox\*.rex"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ListView
    ; Add the files ...
    File "${ExamplesDir}\controls\ListView\*.rex"
    File "${ExamplesDir}\controls\ListView\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ListView\rc
    ; Add the files ...
    File "${ExamplesDir}\controls\ListView\rc\*.rc"
    File "${ExamplesDir}\controls\ListView\rc\*.h"
    File "${ExamplesDir}\controls\ListView\rc\*.bmp"
    File "${ExamplesDir}\controls\ListView\rc\*.dll"
    File "${ExamplesDir}\controls\ListView\rc\res.mak"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ListView\subitem.editing
    ; Add the files ...
    File "${ExamplesDir}\controls\ListView\subitem.editing\*.h"
    File "${ExamplesDir}\controls\ListView\subitem.editing\*.rc"
    File "${ExamplesDir}\controls\ListView\subitem.editing\*.rex"
    File "${ExamplesDir}\controls\ListView\subitem.editing\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ToolBar
    ; Add the files ...
    File "${ExamplesDir}\controls\ToolBar\*.rex"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ToolBar\rc
    ; Add the files ...
    File "${ExamplesDir}\controls\ToolBar\rc\*.bmp"
    File "${ExamplesDir}\controls\ToolBar\rc\*.dll"
    File "${ExamplesDir}\controls\ToolBar\rc\*.h"
    File "${ExamplesDir}\controls\ToolBar\rc\*.rc"
    File "${ExamplesDir}\controls\ToolBar\rc\res.mak"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\ToolTip
    ; Add the files ...
    File "${ExamplesDir}\controls\ToolTip\*.rex"
    File "${ExamplesDir}\controls\ToolTip\*.rc"
    File "${ExamplesDir}\controls\ToolTip\*.h"
    File "${ExamplesDir}\controls\ToolTip\*.ico"
    File "${ExamplesDir}\controls\ToolTip\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\TreeView
    ; Add the files ...
    File "${ExamplesDir}\controls\TreeView\*.rex"
    File "${ExamplesDir}\controls\TreeView\*.inp"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\controls\TreeView\rc
    ; Add the files ...
    File "${ExamplesDir}\controls\TreeView\rc\*.rc"
    File "${ExamplesDir}\controls\TreeView\rc\*.h"
    File "${ExamplesDir}\controls\TreeView\rc\*.bmp"

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

    SetOutPath $INSTDIR\samples\oodialog\propertySheet.tabControls
    ; Add the files ...
    File "${ExamplesDir}\propertySheet.tabControls\oodListViews.rex"
    File "${ExamplesDir}\propertySheet.tabControls\PropertySheetDemo.rex"
    File "${ExamplesDir}\propertySheet.tabControls\TabDemo.rex"
    ; File "${ExamplesDir}\propertySheet.tabControls\TabOwnerDemo.rex"   ; Do not expose this example yet
    File "${ExamplesDir}\propertySheet.tabControls\ticketWizard.rex"
    File "${ExamplesDir}\propertySheet.tabControls\UserTabDemo.rex"


    SetOutPath $INSTDIR\samples\oodialog\propertySheet.tabControls\rc
    ; Add the files ...
    File "${ExamplesDir}\propertySheet.tabControls\rc\oodListViews*"
    File "${ExamplesDir}\propertySheet.tabControls\rc\PropertySheetDemo*"
    ; File "${ExamplesDir}\propertySheet.tabControls\rc\TabOwnerDemo*"   ; Do not expose this example yet
    File "${ExamplesDir}\propertySheet.tabControls\rc\ticketWizard*"
    File "${ExamplesDir}\propertySheet.tabControls\rc\UserTabDemo.h"
    ; File "${ExamplesDir}\propertySheet.tabControls\rc\UserTabDemo.rc"  ; Do not included in distribution, maybe in the future.

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
    SetOutPath $INSTDIR\samples\oodialog\resizableDialogs
    ; Add the files ...
    File "${ExamplesDir}\resizableDialogs\*.txt"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\resizableDialogs\DialogAreaU
    ; Add the files ...
    File "${ExamplesDir}\resizableDialogs\DialogAreaU\*.h"
    File "${ExamplesDir}\resizableDialogs\DialogAreaU\*.rex"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\resizableDialogs\ResizingAdmin
    ; Add the files ...
    File "${ExamplesDir}\resizableDialogs\ResizingAdmin\*.rex"

    ; Set the installation directory:
    SetOutPath $INSTDIR\samples\oodialog\resizableDialogs\ResizingAdmin\rc
    ; Add the files ...
    File "${ExamplesDir}\resizableDialogs\ResizingAdmin\rc\*.bmp"
    File "${ExamplesDir}\resizableDialogs\ResizingAdmin\rc\*.dll"
    File "${ExamplesDir}\resizableDialogs\ResizingAdmin\rc\*.h"
    File "${ExamplesDir}\resizableDialogs\ResizingAdmin\rc\*.rc"

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
    File "${SrcDir}\ReBar.cls"
    File "${SrcDir}\ResDialog.cls"
    File "${SrcDir}\ShellObjects.cls"
    File "${SrcDir}\ToolBar.cls"
    File "${SrcDir}\ToolTip.cls"
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
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise04\Extras
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise04\Extras\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise04\Extras\DlgData
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise04\Extras\DlgData\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise04\Extras\DlgData\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise04\Extras\DlgData\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise04\Extras\DlgData\res
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise04\Extras\DlgData\res\*.dll"

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
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise05\Support
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise05\Support\NumberOnlyEditEx.cls"

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
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Extras
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Extras\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Extras\Popups
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Extras\Popups\*.rex"

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
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise06\Support
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise06\Support\*.cls"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Customer
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Customer\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Customer\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Customer\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Customer\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Customer\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Customer\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Customer\bmp\*.bmp"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Extras
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Extras\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Extras\Person
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Extras\Person\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Extras\Person\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Extras\Person\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Extras\Person\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Extras\Wow4
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Extras\Wow4\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Extras\Wow4\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Order
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Order\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Order\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Order\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Order\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Order\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Order\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Order\bmp\*.bmp"

    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\OrderMgr
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\OrderMgr\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\OrderMgr\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\OrderMgr\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Product
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Product\res
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\res\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\res\*.bmp"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\res\*.dll"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Product\res\res.mak"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise07\Support
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Support\*.cls"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Support\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Support\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise07\Support\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Customer
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Customer\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Customer\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Customer\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Customer\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Customer\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Customer\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Customer\bmp\*.bmp"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Customer\bmp\*.cur"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Extras
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Extras\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Extras\Person
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Extras\Person\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Extras\Person\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Extras\Person\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Extras\Person\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Order
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Order\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Order\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Order\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Order\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Order\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Order\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Order\bmp\*.bmp"

    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\OrderMgr
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\OrderMgr\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\OrderMgr\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\OrderMgr\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Product
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Product\res
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\res\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\res\*.bmp"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\res\*.dll"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Product\res\res.mak"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise08\Support
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Support\*.cls"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Support\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Support\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise08\Support\*.rex"

     ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Customer
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Customer\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Customer\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Customer\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Customer\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Customer\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Customer\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Customer\bmp\*.bmp"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Customer\bmp\*.cur"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Extras
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Extras\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Extras\Person
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Extras\Person\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Extras\Person\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Extras\Person\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Extras\Person\*.txt"

   ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Extras\Person\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Extras\Person\bmp\*.ico"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Order
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Order\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Order\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Order\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Order\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Order\bmp
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Order\bmp\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Order\bmp\*.bmp"

    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\OrderMgr
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\OrderMgr\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\OrderMgr\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\OrderMgr\*.rex"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Product
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\*.rex"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\*.txt"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Product\res
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\res\*.ico"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\res\*.bmp"
    ; File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\res\*.dll"
    ; File "${ExamplesDir}\userGuide\exercises\Exercise09\Product\res\res.mak"

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR\samples\oodialog\userGuide\exercises\Exercise09\Support
    ; Add the files ...
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Support\*.cls"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Support\*.h"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Support\*.rc"
    File "${ExamplesDir}\userGuide\exercises\Exercise09\Support\*.rex"

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
    DetailPrint "********** ooDialog ${SHORTVERSION} Start Menu Shortcuts **********"

    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Display Window Tree.lnk" "$INSTDIR\ooDialog.exe" '"$INSTDIR\samples\oodialog\winsystem\displayWindowTree.rex"' "$INSTDIR\rexx.exe"
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Windows Manager.lnk" "$INSTDIR\ooDialog.exe" '"$INSTDIR\samples\oodialog\winsystem\usewmgr.rex"' "$INSTDIR\rexx.exe"

    CreateDirectory "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog"

    SetOutPath $INSTDIR\samples\oodialog\Controls\ComboBox
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Combo Box Types.lnk" "$INSTDIR\ooDialog.exe" '"$INSTDIR\samples\oodialog\Controls\ComboBox\comboBoxTypes.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"

    SetOutPath $INSTDIR\samples\oodialog\Controls\ToolTip
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Custom Position Tool Tips.lnk" "$INSTDIR\ooDialog.exe" '"$INSTDIR\samples\oodialog\Controls\ToolTip\customPositionToolTip.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"

    SetOutPath $INSTDIR\samples\oodialog\Controls\ListView
    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\List-view Views.lnk" "$INSTDIR\ooDialog.exe" '"$INSTDIR\samples\oodialog\Controls\ListView\columnIcons.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"

    CreateShortCut  "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Samples.lnk" "$INSTDIR\ooDialog.exe" '"$INSTDIR\samples\oodialog\sample.rex"' "$INSTDIR\samples\oodialog\oodialog.ico"
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


/** .onMouseOverSection()  Call back function
 *
 * Invoked when the user puts the mouse over one of the componets that can be
 * installed.  This is what provides the description of each component.
 */
Function .onMouseOverSection

  !insertmacro MUI_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} "Installs the core components of ${LONGNAME} to the ooRexx installation directory."
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDoc} "Install the ${LONGNAME} documentation."
    !insertmacro MUI_DESCRIPTION_TEXT ${SecExample} "Install example ${LONGNAME} programs."
 !insertmacro MUI_DESCRIPTION_END

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
 *
 *  However, it turns out some people do not install the doc.  So, we need to
 *  check if the doc file(s) actuatlly exist before we test them.
 */
Function CheckForProblems

check_for_locked_files:

  StrCpy $keyFileName '$INSTDIR\oodialog.dll'

  LockedList::IsFileLocked $keyFileName
  Pop $R0
  ${If} $R0 == true
    goto ooDialogIsRunning
  ${EndIf}

  StrCpy $keyFileName '$INSTDIR\ooDialog.exe'
  LockedList::IsFileLocked $keyFileName
  Pop $R0
  ${If} $R0 == true
    goto ooDialogIsRunning
  ${EndIf}

  StrCpy $keyFileName '$INSTDIR\ooDialog.com'
  LockedList::IsFileLocked $keyFileName
  Pop $R0
  ${If} $R0 == true
    goto ooDialogIsRunning
  ${EndIf}

  StrCpy $keyFileName '$INSTDIR\doc\oodialog.pdf'
  LockedList::IsFileLocked $keyFileName
  Pop $R0
  ${If} $R0 == true
    goto ooDialogDocIsOpened
  ${EndIf}

  StrCpy $keyFileName '$INSTDIR\doc\oodguide.pdf'
  LockedList::IsFileLocked $keyFileName
  Pop $R0
  ${If} $R0 == true
    goto ooDialogDocIsOpened
  ${EndIf}

  goto done_out

  ooDialogIsRunning:
      MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION|MB_TOPMOST \
        "WARNING.  The file:$\n$\n\
          $keyFileName$\n$\n\
        is locked and can not be updated by the installer.  This indicates$\n\
        that all ooDialog programs have not been halted$\n$\n\
        Continuing with the install in this case is known to cause problems.$\n$\n\
        To Retry:$\n$\n\
        Ensure all ooDialog programs are halted and all ooDialog documenta-$\n\
        tion is closed and then push Retry.$\n$\n\
        To Quit the install and fix the problem:$\n$\n\
        Push Cancel.  Determine which ooDialog programs are open and close$\n\
        them.  Ensure no other ooDialog programs are open and that all$\n\
        ooDialog documentation is closed.  Then retry the install." \
        /SD IDCANCEL IDRETRY check_for_locked_files

        Quit

  ooDialogDocIsOpened:
      MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION|MB_TOPMOST \
        "WARNING.  The file:$\n$\n\
          $keyFileName$\n$\n\
        is locked and can not be updated by the installer.  This indicates$\n\
        that all ooDialog documentation has not been closed$\n$\n\
        Continuing with the install in this case is known to cause problems.$\n$\n\
        To Retry:$\n$\n\
        Ensure all ooDialog programs are halted and all ooDialog documenta-$\n\
        tion is closed and then push Retry.$\n$\n\
        To Quit the install and fix the problem:$\n$\n\
        Push Cancel.  Determine why the documentation is open and close it.$\n\
        Ensure all ooDialog programs are closed and any other ooDialog$\n\
        documentation is closed.  Then retry the install." \
        /SD IDCANCEL IDRETRY check_for_locked_files

        Quit

  done_out:

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

  Delete "${SMooRexxFolder}\ooRexx Documentation\ooRexx ooDialog Method Reference.lnk"
  Delete "${SMooRexxFolder}\ooRexx Documentation\ooRexx ooDialog Reference.lnk"
  Delete "${SMooRexxFolder}\ooRexx Documentation\ooRexx ooDialog User Guide.lnk"
  Delete "${SMooRexxFolder}\ooRexx Documentation\ooDialog Reference.lnk"
  Delete "${SMooRexxFolder}\ooRexx Documentation\ooDialog User Guide.lnk"
  Delete "${SMooRexxFolder}\ooRexx Documentation\ooRexxTry Reference.lnk"

  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Display Event Log.lnk"  ; Only pre ooDialog 4.2.3
  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Display Window Tree.lnk"
  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\Windows Manager.lnk"
  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Samples.lnk"
  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Calculator.lnk"
  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\Change Editor.lnk"
  Delete "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog\FTYPE Changer.lnk"

  RMDir /r "${SMooRexxFolder}\${REXXSHORTNAME} Samples\ooDialog"

  DetailPrint ""

FunctionEnd
