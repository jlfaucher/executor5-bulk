#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
#/* Copyright (c) 2005-2010 Rexx Language Association. All rights reserved.    */
#/*                                                                            */
#/* This program and the accompanying materials are made available under       */
#/* the terms of the Common Public License v1.0 which accompanies this         */
#/* distribution. A copy is also available at the following address:           */
#/* http://www.oorexx.org/license.html                                         */
#/*                                                                            */
#/* Redistribution and use in source and binary forms, with or                 */
#/* without modification, are permitted provided that the following            */
#/* conditions are met:                                                        */
#/*                                                                            */
#/* Redistributions of source code must retain the above copyright             */
#/* notice, this list of conditions and the following disclaimer.              */
#/* Redistributions in binary form must reproduce the above copyright          */
#/* notice, this list of conditions and the following disclaimer in            */
#/* the documentation and/or other materials provided with the distribution.   */
#/*                                                                            */
#/* Neither the name of Rexx Language Association nor the names                */
#/* of its contributors may be used to endorse or promote products             */
#/* derived from this software without specific prior written permission.      */
#/*                                                                            */
#/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
#/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
#/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
#/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
#/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
#/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
#/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
#/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
#/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
#/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
#/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
#/*                                                                            */
#/*----------------------------------------------------------------------------*/

# NOTE:  /OPT:REF in linker flags eliminates unreferenced functions and data.
#        Need to use /Gy when compiling to use /OPT:REF.

# NMAKE-compatible MAKE file for ooDialog
all:  $(OR_OUTDIR_OOD420)\oodialog.dll

!include "$(OR_LIBSRC)\ORXWIN32.MAK"
C=cl
OPTIONS= $(cflags_common) $(cflags_dll) $(OR_ORYXINCL)
OR_LIB=$(OR_OUTDIR)

# All Source Files
SOURCEF= $(OR_OUTDIR_OOD420)\APICommon.obj $(OR_OUTDIR_OOD420)\oodBarControls.obj $(OR_OUTDIR_OOD420)\oodBaseDialog.obj $(OR_OUTDIR_OOD420)\oodBasicControls.obj \
         $(OR_OUTDIR_OOD420)\oodCommon.obj $(OR_OUTDIR_OOD420)\oodControl.obj $(OR_OUTDIR_OOD420)\oodData.obj $(OR_OUTDIR_OOD420)\oodDeviceGraphics.obj \
         $(OR_OUTDIR_OOD420)\ooDialog.obj $(OR_OUTDIR_OOD420)\oodMenu.obj $(OR_OUTDIR_OOD420)\oodMessaging.obj $(OR_OUTDIR_OOD420)\oodPackageEntry.obj \
         $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodResources.obj $(OR_OUTDIR_OOD420)\oodRoutines.obj $(OR_OUTDIR_OOD420)\oodUser.obj \
         $(OR_OUTDIR_OOD420)\oodUtilities.obj $(OR_OUTDIR_OOD420)\oodViewControls.obj $(OR_OUTDIR_OOD420)\oodialog.res

# All Source files that include APICommon.hpp
APICOMMON_SOURCEF = $(OR_OUTDIR_OOD420)\APICommon.obj $(OR_OUTDIR_OOD420)\oodBaseDialog.obj $(OR_OUTDIR_OOD420)\oodBasicControls.obj \
                    $(OR_OUTDIR_OOD420)\oodCommon.obj $(OR_OUTDIR_OOD420)\oodControl.obj $(OR_OUTDIR_OOD420)\oodData.obj \
                    $(OR_OUTDIR_OOD420)\oodDeviceGraphics.obj $(OR_OUTDIR_OOD420)\ooDialog.obj $(OR_OUTDIR_OOD420)\oodMenu.obj \
                    $(OR_OUTDIR_OOD420)\oodMessaging.obj $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodRoutines.obj \
                    $(OR_OUTDIR_OOD420)\oodUser.obj $(OR_OUTDIR_OOD420)\oodUtilities.obj $(OR_OUTDIR_OOD420)\oodViewControls.obj

# All Source files that include oodCommon.hpp
COMMON_SOURCEF = $(OR_OUTDIR_OOD420)\oodBarControls.obj $(OR_OUTDIR_OOD420)\oodBaseDialog.obj $(OR_OUTDIR_OOD420)\oodBasicControls.obj \
                 $(OR_OUTDIR_OOD420)\oodCommon.obj $(OR_OUTDIR_OOD420)\oodData.obj $(OR_OUTDIR_OOD420)\oodDeviceGraphics.obj $(OR_OUTDIR_OOD420)\oodMenu.obj \
                 $(OR_OUTDIR_OOD420)\oodMessaging.obj $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodRoutines.obj \
                 $(OR_OUTDIR_OOD420)\oodUser.obj $(OR_OUTDIR_OOD420)\oodUtilities.obj $(OR_OUTDIR_OOD420)\oodViewControls.obj

# All Source files that include oodDeviceGraphics.hpp
OODEVICECONTEXT_SOURCEF = $(OR_OUTDIR_OOD420)\oodBaseDialog.obj $(OR_OUTDIR_OOD420)\oodControl.cpp $(OR_OUTDIR_OOD420)\ooDeviceGraphics.cpp \
                          $(OR_OUTDIR_OOD420)\ooDialog.cpp $(OR_OUTDIR_OOD420)\oodRoutines.obj $(OR_OUTDIR_OOD420)\oodMessaging.obj         \
                          $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodUser.obj $(OR_OUTDIR_OOD420)\oodUtilities.obj

# All Source files that include oodData.hpp
OODDATA_SOURCEF = $(OR_OUTDIR_OOD420)\oodBaseDialog.obj $(OR_OUTDIR_OOD420)\ooDialog.cpp $(OR_OUTDIR_OOD420)\oodData.obj $(OR_OUTDIR_OOD420)\oodMessaging.obj \
                  $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodUser.obj


# All Source files that include oodControl.hpp
OODCONTROL_SOURCEF = $(OR_OUTDIR_OOD420)\oodBarControls.obj $(OR_OUTDIR_OOD420)\oodBaseDialog.obj $(OR_OUTDIR_OOD420)\oodBasicControls.obj \
                     $(OR_OUTDIR_OOD420)\oodControl.obj $(OR_OUTDIR_OOD420)\oodData.obj $(OR_OUTDIR_OOD420)\ooDialog.cpp \
                     $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodUser.obj $(OR_OUTDIR_OOD420)\oodViewControls.obj

# All Source files that include oodMessaging.hpp
OODMESSAGING_SOURCEF = $(OR_OUTDIR_OOD420)\oodBaseDialog.obj $(OR_OUTDIR_OOD420)\oodControl.obj $(OR_OUTDIR_OOD420)\oodDeviceGraphics.obj \
                       $(OR_OUTDIR_OOD420)\ooDialog.obj $(OR_OUTDIR_OOD420)\oodMenu.obj $(OR_OUTDIR_OOD420)\oodMessaging.obj \
                       $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodUser.obj

# All Source files that include oodResources.hpp
OODRESOURCES_SOURCEF = $(OR_OUTDIR_OOD420)\oodBasicControls.obj $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodResources.obj \
                       $(OR_OUTDIR_OOD420)\oodViewControls.obj

# All Source files that include oodUser.hpp
OODUSER_SOURCEF = $(OR_OUTDIR_OOD420)\oodPropertySheetDialog.obj $(OR_OUTDIR_OOD420)\oodUser.obj

.c{$(OR_OUTDIR_OOD420)}.obj:
    $(C) $(OPTIONS)  /DINCL_32  -c $(@B).c /Fo$(OR_OUTDIR_OOD420)\$(@B).obj

#
# *** .cpp -> .obj rules
#
{$(OR_OODIALOGSRC_420)}.cpp{$(OR_OUTDIR_OOD420)}.obj:
    @ECHO .
    @ECHO Compiling $(@B).cpp
    $(OR_CC) $(cflags_common) $(cflags_dll) /Fo$(OR_OUTDIR_OOD420)\$(@B).obj $(OR_ORYXINCL)  $(OR_OODIALOGSRC_420)\$(@B).cpp


{$(OR_OODIALOGSRC_420)}.c{$(OR_OUTDIR_OOD420)}.obj:
    @ECHO .
    @ECHO Compiling $(@B).c
    $(OR_CC) $(cflags_common) $(cflags_dll) /Fo$(OR_OUTDIR_OOD420)\$(@B).obj $(OR_ORYXINCL)  $(OR_OODIALOGSRC_420)\$(@B).c


$(OR_OUTDIR_OOD420)\oodialog.dll:     $(SOURCEF)
    $(OR_LINK) \
        $(SOURCEF)  \
    $(lflags_common) $(lflags_dll) \
    $(OR_LIB)\rexx.lib \
    $(OR_LIB)\rexxapi.lib \
    WINMM.LIB \
    COMDLG32.LIB \
    COMCTL32.LIB \
    shlwapi.lib \
    -def:$(OR_OODIALOGSRC_420)\ooDialog.def \
    -out:$(OR_OUTDIR_OOD420)\$(@B).dll


# Update the version information block
$(OR_OUTDIR_OOD420)\oodialog.res: $(OR_OODIALOGSRC_420)\oodialog.rc
    @ECHO .
    @ECHO ResourceCompiling $(@B).res
        $(rc) $(rcflags_common) /i $(OR_OODIALOGSRC_420) /i $(OR_WINKERNELSRC) -r -fo$(OR_OUTDIR_OOD420)\$(@B).res $(OR_OODIALOGSRC_420)\$(@B).rc

# Recompile everything if the make file changes.
$(SOURCEF) : oodialog.mak

# Source .obj files that should be recompiled when header file(s) change.
$(SOURCEF) : ooDialog.hpp
$(COMMON_SOURCEF) : oodCommon.hpp
$(APICOMMON_SOURCEF) : APICommon.hpp
$(OODEVICECONTEXT_SOURCEF) : oodDeviceGraphics.hpp
$(OODDATA_SOURCEF) : oodData.hpp
$(OODCONTROL_SOURCEF) : oodControl.hpp
$(OODMESSAGING_SOURCEF) : oodMessaging.hpp
$(OODRESOURCES_SOURCEF) : oodResources.hpp
$(OODUSER_SOURCEF) : oodUser.hpp
$(OR_OUTDIR_OOD420)\oodMenu.obj : oodMenu.hpp
