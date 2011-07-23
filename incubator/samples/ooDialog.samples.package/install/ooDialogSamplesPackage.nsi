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
 *  An NSIS script to build the ooDialog Samples installer.
 *
 */


;===============================================================================
;Configuration
;===============================================================================

;--------------------------------
; Defines

  !define VERSION        "0.0.2"
  !define VERSIONNODOTS  "0_0_2"
  !define SHORTNAME      "ooDialog_Samples_${VERSIONNODOTS}"
  !define LONGNAME       "Extra ooDialog Samples ${VERSION}"
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

  Name "Extra ooDialog Samples"
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

  !define UninstLog "uninstall.log"
  Var UninstLog

  !define SHACF_FILESYSTEM 1  ; For SHAutoComplete()

  !macro AddItem Path
   FileWrite $UninstLog "${Path}$\r$\n"
  !macroend
  !define AddItem "!insertmacro AddItem"

  !macro File FilePath FileName
   FileWrite $UninstLog "$OUTDIR\${FileName}$\r$\n"
   File "${FilePath}${FileName}"
  !macroend
  !define File "!insertmacro File"

  !macro CreateDirectory Path
   CreateDirectory "${Path}"
   FileWrite $UninstLog "${Path}$\r$\n"
  !macroend
  !define CreateDirectory "!insertmacro CreateDirectory"

  !macro SetOutPath Path
   SetOutPath "${Path}"
   FileWrite $UninstLog "${Path}$\r$\n"
  !macroend
  !define SetOutPath "!insertmacro SetOutPath"

  !macro WriteUninstaller Path
   WriteUninstaller "${Path}"
   FileWrite $UninstLog "${Path}$\r$\n"
  !macroend
  !define WriteUninstaller "!insertmacro WriteUninstaller"

;--------------------------------
; Variables

  Var RegVal_installedLocation                    ; Where we are installed
  Var DeleteWholeTree                             ; Delete using log file or by rmdir
  Var LogFileExists

  ; Dialog variables
  Var Dialog
  Var Delete_ooRexx_Tree_CK
  Var Label_One
  Var Label_Two

;--------------------------------
; Pages.
;

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "CPLv1.0.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  UninstPage custom un.Uninstall_By_Log_page un.Uninstall_By_Log_leave
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;===============================================================================
;Installer Sections
;===============================================================================

;-------------------------------------------------------------------------------
;  Hidden section to open the log file

Section -openlogfile
  CreateDirectory "$INSTDIR"
  IfFileExists "$INSTDIR\${UninstLog}" +3
    FileOpen $UninstLog "$INSTDIR\${UninstLog}" w
    Goto +4
  SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
  FileOpen $UninstLog "$INSTDIR\${UninstLog}" a
  FileSeek $UninstLog 0 END
SectionEnd

Section  installFiles

  ; Set the installation directory:
  ${SetOutPath} "$INSTDIR"

  DetailPrint "******** Extra ooDialog Samples ${VERSION} **********"

  ${File} "${SRCDIR}\install\" "CPLv1.0.txt"
  ${File} "${SRCDIR}\install\" "AppIcon2.ico"
  ${File} "${SRCDIR}\" "ReleaseNotes.txt"
  ${File} "${SRCDIR}\" "Version.txt"

  ${CreateDirectory} "$INSTDIR\Controls"
  ${CreateDirectory} "$INSTDIR\Controls\Edit"
  ${CreateDirectory} "$INSTDIR\Dialogs"
  ${CreateDirectory} "$INSTDIR\Menus"

  ${SetOutPath} "$INSTDIR\Controls\Edit\NumberOnly"
  ${File} "${SRCDIR}\Controls\Edit\NumberOnly\" "ReadMeFirst.txt"
  ${File} "${SRCDIR}\Controls\Edit\NumberOnly\" "CalcMPG.h"
  ${File} "${SRCDIR}\Controls\Edit\NumberOnly\" "CalcMPG.rc"
  ${File} "${SRCDIR}\Controls\Edit\NumberOnly\" "CalcMPG.rex"
  ${File} "${SRCDIR}\Controls\Edit\NumberOnly\" "NumberOnlyEditEx.cls"
  ${File} "${SRCDIR}\Controls\Edit\NumberOnly\" "CalcMPG32.dll"
  ${File} "${SRCDIR}\Controls\Edit\NumberOnly\" "CalcMPG64.dll"

  ${SetOutPath} "$INSTDIR\Dialogs\application.icon"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "appIcon.h"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "ApplicationIcon.rc"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "ApplicationIcon32.dll"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "ApplicationIcon64.dll"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "Camera.ico"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "Question32.ico"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "RcAppIcon.rex"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "ResAppIcon.rex"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "UserRcAppIcon.rex"
  ${File} "${SRCDIR}\Dialogs\application.icon\" "UserRcVariation.rex"

  ${SetOutPath} "$INSTDIR\Dialogs\dlgData"
  ${File} "${SRCDIR}\Dialogs\dlgData\" "dlgData.h"
  ${File} "${SRCDIR}\Dialogs\dlgData\" "dlgData.rc"
  ${File} "${SRCDIR}\Dialogs\dlgData\" "dlgData.rex"
  ${File} "${SRCDIR}\Dialogs\dlgData\" "symbolicDlgData.rex"

  ; Write the uninstall keys.
  DetailPrint "Writing uninstall keys."
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "InstallLocation" '"$INSTDIR"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "DisplayName" "${LONGNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "DisplayIcon" "${DISPLAYICON}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}" "HelpLink" "http://www.oorexx.org/support.html"
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

  DetailPrint "Removing registry keys set when installing ${LONGNAME}."
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}"
  DeleteRegKey HKLM "SOFTWARE\${SHORTNAME}"

  Call un.Delete_Installed_Files

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
  StrCpy $DeleteWholeTree 'false'


FunctionEnd

/** un.Uninstall_By_Log_page()  Custom page function.
 *
 * This is a custom page that follows the page that checks if it is okay to
 * stop rxapi.  Here we check for the uninstall log.  We then give the user the
 * option of simply deleting the ooRexx directory tree (50 times faster on my
 * system) or using the log file to individually delete only the install files.
 *
 * If the uninstall log is missing, the user does not have the option of
 * deleting individual files and directories.
 */
Function un.Uninstall_By_Log_page

  StrCpy $LogFileExists 'false'
  IfFileExists "$INSTDIR\${UninstLog}" 0 +4
    StrCpy $LogFileExists 'true'

  ${if} $LogFileExists == 'false'
    !insertmacro MUI_HEADER_TEXT \
      "${UninstLog} NOT found." \
      "The option of only removing files installed by the prior ooRexx installer is not available."

    StrCpy $0 \
      "Because the ${UninstLog} file is missing, the uninstall process must remove all \
      files in the $INSTDIR directory tree.$\n$\n\
      WARNING: This will remove all folders and files in the $INSTDIR folder, including \
      any folders or files not placed there by the original installation.$\n$\n\
      If there are any personl folders or files in the $INSTDIR directory tree that need \
      to be saved, please cancel the uninstall, move the files, and restart the uninstall \
      program."
  ${else}
    !insertmacro MUI_HEADER_TEXT \
      "Choose the method for removing installed files." \
      "Delete only installed files or delete entire directory tree?"

    StrCpy $0 \
      "The uninstall program can use an install log to delete only the folders and files \
      placed in the $INSTDIR directory tree by the original installation program.$\n$\n\
      Optionally, the entire $INSTDIR directory tree can be deleted.$\n$\n\
      WARNING: Deleting the entire directory tree will remove all folders and files in the \
      $INSTDIR folder.  This will include any folders or files not placed there by the \
      installation."

  ${endif}

  nsDialogs::Create /NOUNLOAD 1018
  Pop $Dialog

  ${If} $Dialog == error
    Abort
  ${endif}

  ${if} $LogFileExists == 'false'
    ${NSD_CreateLabel} 0 0 100% 80u $0
    Pop $Label_One

    ${NSD_CreateCheckBox} 0 84u 100% 8u "Delete entire directory tree"
    Pop $Delete_ooRexx_Tree_CK
    ${NSD_Check} $Delete_ooRexx_Tree_CK
    EnableWindow $Delete_ooRexx_Tree_CK 0
  ${else}
    ${NSD_CreateLabel} 0 0 100% 84u $0
    Pop $Label_One

    ${NSD_CreateLabel} 0 92u 100% 16u "To DELETE the entire $INSTDIR directory tree, check the check box."
    Pop $Label_Two

    ${NSD_CreateCheckBox} 0 116u 100% 8u "Delete entire directory tree"
    Pop $Delete_ooRexx_Tree_CK
  ${endif}

  ; Set focus to the page dialog rather than the installer dialog, set focus to
  ; the check box, and then show the page dialog
  SendMessage $Dialog ${WM_SETFOCUS} $HWNDPARENT 0
  SendMessage $Dialog ${WM_NEXTDLGCTL} $Delete_ooRexx_Tree_CK 1

  nsDialogs::Show

FunctionEnd

/** un.Uninstall_By_Log_leave()  Call back function.
 *
 * Invoked by the uninstaller when the user clicks Next on the uninstall using
 * the log page.
 */
Function un.Uninstall_By_Log_leave

  ${NSD_GetState} $Delete_ooRexx_Tree_CK $0

  ${if} $0 == 1
    StrCpy $DeleteWholeTree 'true'
  ${else}
    StrCpy $DeleteWholeTree 'false'
  ${endif}

FunctionEnd

/** un.Delete_Installed_Files()
 *
 * Deletes the installed files in the manner specified by the user.  Either by
 * using the log file to delete only files installed by the previous
 * installaltion or by simply deleting the whole installation directory.
 */
Function un.Delete_Installed_Files

  DetailPrint "Deleting ${LONGNAME}"

  ${if} $DeleteWholeTree == 'true'
    DetailPrint "Uninstall files by deleting the $INSTDIR directory tree"
    RMDir /r "$INSTDIR"
  ${else}
    DetailPrint "Uninstall files using the install log file"
    Push $R0
    Push $R1
    Push $R2
    SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
    FileOpen $UninstLog "$INSTDIR\${UninstLog}" r
    StrCpy $R1 0

    GetLineCount:
      ClearErrors
      FileRead $UninstLog $R0
      IntOp $R1 $R1 + 1
      IfErrors 0 GetLineCount

    LoopRead:
      FileSeek $UninstLog 0 SET
      StrCpy $R2 0
      FindLine:
      FileRead $UninstLog $R0
      IntOp $R2 $R2 + 1
      StrCmp $R1 $R2 0 FindLine

      StrCpy $R0 $R0 -2
      IfFileExists "$R0\*.*" 0 +3
        RMDir $R0  #is dir
      Goto +3
      IfFileExists $R0 0 +2
        Delete $R0 #is file

      IntOp $R1 $R1 - 1
      StrCmp $R1 0 LoopDone
      Goto LoopRead
    LoopDone:
    FileClose $UninstLog

    Delete "$INSTDIR\${UninstLog}"
    Delete "$INSTDIR\${UNINSTALLER}"
    RMDir "$INSTDIR"
    Pop $R2
    Pop $R1
    Pop $R0
  ${endif}

FunctionEnd

