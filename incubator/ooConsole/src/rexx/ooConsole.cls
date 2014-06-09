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

::requires 'ooShapes.cls'
::requires 'ooconsole' LIBRARY


/** class: ooConsole
 * ...
 *
 */
::class 'ooConsole' public inherit ooConsoleConstants

::method allocConsole class external "LIBRARY ooconsole oocon_allocConsole_cls"
::method attachConsole class external "LIBRARY ooconsole oocon_attachConsole_cls"
::method freeConsole class external "LIBRARY ooconsole oocon_freeConsole_cls"
::method version class external "LIBRARY ooconsole oocon_version_cls"
::method init external "LIBRARY ooconsole oocon_init"
::method unint external "LIBRARY ooconsole oocon_uninit"
::attribute errRC get external "LIBRARY ooconsole oocon_errRC_atr"
::attribute handle get external "LIBRARY ooconsole oocon_handle_atr"
::method addAlias external "LIBRARY ooconsole oocon_addAlias"
::method generateCtrlEvent external "LIBRARY ooconsole oocon_generateCtrlEvent"
::method getAlias external "LIBRARY ooconsole oocon_getAlias"
::method getAliases external "LIBRARY ooconsole oocon_getAliases"
::method getAliasesLength external "LIBRARY ooconsole oocon_getAliasesLength"
::method getAliasExes external "LIBRARY ooconsole oocon_getAliasExes"
::method getAliasExesLength external "LIBRARY ooconsole oocon_getAliasExesLength"
::method getCP external "LIBRARY ooconsole oocon_getCP"
::method getDisplayMode external "LIBRARY ooconsole oocon_getDisplayMode"
::method getHistoryInfo external "LIBRARY ooconsole oocon_getHistoryInfo"
::method getNumberOfMouseButtons external "LIBRARY ooconsole oocon_getNumberOfMouseButtons"
::method getOriginalTitle external "LIBRARY ooconsole oocon_getOriginalTitle"
::method getOutputCP external "LIBRARY ooconsole oocon_getOutputCP"
::method getProcessList external "LIBRARY ooconsole oocon_getProcessList"
::method getSelectionInfo external "LIBRARY ooconsole oocon_getSelectionInfo"
::method getTitle external "LIBRARY ooconsole oocon_getTitle"
::method getWindow external "LIBRARY ooconsole oocon_getWindow"
::method setCP external "LIBRARY ooconsole oocon_setCP"
::method setCtrlHandler external "LIBRARY ooconsole oocon_setCtrlHandler"
::method setHistoryInfo external "LIBRARY ooconsole oocon_setHistoryInfo"
::method setOutputCP external "LIBRARY ooconsole oocon_setOutputCP"
::method setTitle external "LIBRARY ooconsole oocon_setTitle"

::method test external "LIBRARY ooconsole oocon_test"                           -- Do not document, internal use only

::class 'ScreenBuffer' public subclass ooConsole
::method close external "LIBRARY ooconsole sb_close"
::method createScreenBuffer external "LIBRARY ooconsole sb_createScreenBuffer"
::method fillOutputAttribute external "LIBRARY ooconsole sb_fillOutputAttribute"
::method fillOutputCharacter external "LIBRARY ooconsole sb_fillOutputCharacter"
::method getCursorInfo external "LIBRARY ooconsole sb_getCursorInfo"
::method getFontInfo external "LIBRARY ooconsole sb_getFontInfo"
::method getFontSize external "LIBRARY ooconsole sb_getFontSize"
::method getLargestWindowSize external "LIBRARY ooconsole sb_getLargestWindowSize"
::method getMode external "LIBRARY ooconsole sb_getMode"
::method getScreenBufferInfo external "LIBRARY ooconsole sb_getScreenBufferInfo"
::method getScreenBufferInfoEX external "LIBRARY ooconsole sb_getScreenBufferInfoEx"
::method getScreenBufferSize external "LIBRARY ooconsole sb_getScreenBufferSize"
::method getWindowRect external "LIBRARY ooconsole sb_getWindowRect"
::method getWindowSize external "LIBRARY ooconsole sb_getWindowSize"
::method readOutput external "LIBRARY ooconsole sb_readOutput"
::method readOutputAttribute external "LIBRARY ooconsole sb_readOutputAttribute"
::method readOutputCharacter external "LIBRARY ooconsole sb_readOutputCharacter"
::method scrollScreenBuffer external "LIBRARY ooconsole sb_scrollScreenBuffer"
::method setActiveScreenBuffer external "LIBRARY ooconsole sb_setActiveScreenBuffer"
::method setCursorInfo external "LIBRARY ooconsole sb_setCursorInfo"
::method setCursorPosition external "LIBRARY ooconsole sb_setCursorPosition"
::method setDisplayMode external "LIBRARY ooconsole sb_setDisplayMode"
::method setFontInfo external "LIBRARY ooconsole sb_setFontInfo"
::method setMode external "LIBRARY ooconsole sb_setMode"
::method setScreenBufferInfoEx external "LIBRARY ooconsole sb_setScreenBufferInfoEx"
::method setScreenBufferSize external "LIBRARY ooconsole sb_setScreenBufferSize"
::method setTextAttribute external "LIBRARY ooconsole sb_setTextAttribute"
::method setWindowRect external "LIBRARY ooconsole sb_setWindowRect"
::method setWindowSize external "LIBRARY ooconsole sb_setWindowSize"
::method write external "LIBRARY ooconsole sb_write"
::method writeOutput external "LIBRARY ooconsole sb_writeOutput"
::method writeOutputAttribute external "LIBRARY ooconsole sb_writeOutputAttribute"
::method writeOutputCharacter external "LIBRARY ooconsole sb_writeOutputCharacter"

::class 'StdOutput' public subclass ScreenBuffer
::method init external "LIBRARY ooconsole stdout_init"
::method getStdHandle external "LIBRARY ooconsole stdout_getStdHandle"
::method setStdHandle external "LIBRARY ooconsole stdout_setStdHandle"

::class 'StdError' public subclass ScreenBuffer
::method init external "LIBRARY ooconsole stderr_init"
::method getStdHandle external "LIBRARY ooconsole stderr_getStdHandle"
::method setStdHandle external "LIBRARY ooconsole stderr_setStdHandle"

::class 'StdInput' public subclass ooConsole
::method init external "LIBRARY ooconsole stdin_init"
::method flushInputBuffer external "LIBRARY ooconsole stdin_flushInputBuffer"
::method getStdHandle external "LIBRARY ooconsole stdin_getStdHandle"
::method getMode external "LIBRARY ooconsole stdin_getMode"
::method getNumberOfInputEvents external "LIBRARY ooconsole stdin_getNumberOfInputEvents"
::method peekInput external "LIBRARY ooconsole stdin_peekInput"
::method read external "LIBRARY ooconsole stdin_read"
::method readInput external "LIBRARY ooconsole stdin_readInput"
::method setMode external "LIBRARY ooconsole stdin_setMode"
::method setStdHandle external "LIBRARY ooconsole stdin_setStdHandle"
::method writeInput external "LIBRARY ooconsole stdin_writeInput"

::class 'CharInfo' public
::method init class external "LIBRARY ooconsole char_init_cls"
::method from class external "LIBRARY ooconsole char_from_cls"
::method init external "LIBRARY ooconsole char_init"
::attribute char get external "LIBRARY ooconsole char_char"
::attribute setChar set external "LIBRARY ooconsole char_setChar"
::attribute attribute get external "LIBRARY ooconsole char_attribute"
::attribute setAttribute set external "LIBRARY ooconsole char_setAttribute"

::class 'InputRecord' public
::method init class external "LIBRARY ooconsole inrec_init_cls"
::method init external "LIBRARY ooconsole inrec_init"
::attribute controlKeys get external "LIBRARY ooconsole inrec_controlKeys"
::attribute setControlKeys set external "LIBRARY ooconsole inrec_setControlKeys"

::method keyFrom class external "LIBRARY ooconsole inrec_keyFrom_cls"
::attribute char get external "LIBRARY ooconsole inrec_char"
::attribute setChar set external "LIBRARY ooconsole inrec_setChar"
::attribute keyDown get external "LIBRARY ooconsole inrec_keyDown"
::attribute setKeyDown set external "LIBRARY ooconsole inrec_setKeyDown"
::attribute vKeyCode get external "LIBRARY ooconsole inrec_vKeyCode"
::attribute setVKeyCode set external "LIBRARY ooconsole inrec_setVKeyCode"
::attribute vScanCode get external "LIBRARY ooconsole inrec_vScanCode"
::attribute setVScanCode set external "LIBRARY ooconsole inrec_setVScanCode"
::attribute repeatCount get external "LIBRARY ooconsole inrec_repeatCount"
::attribute setRepeatCount set external "LIBRARY ooconsole inrec_setRepeatCount"

::method mouseFrom class external "LIBRARY ooconsole inrec_mouseFrom_cls"
::attribute position get external "LIBRARY ooconsole inrec_position"
::attribute setPosition set external "LIBRARY ooconsole inrec_setPosition"
::attribute buttonState get external "LIBRARY ooconsole inrec_buttonState"
::attribute setButtonState set external "LIBRARY ooconsole inrec_setButtonState"
::attribute eventFlag get external "LIBRARY ooconsole inrec_eventFlag"
::attribute setEventFlag set external "LIBRARY ooconsole inrec_setEventFlag"

::method windowFrom class external "LIBRARY ooconsole inrec_windowFrom_cls"
::attribute size get external "LIBRARY ooconsole inrec_size"
::attribute setSize set external "LIBRARY ooconsole inrec_setSize"

::method focusFrom class external "LIBRARY ooconsole inrec_focusFrom_cls"
::attribute setFocus get external "LIBRARY ooconsole inrec_setFocus"
::attribute setSetFocus set external "LIBRARY ooconsole inrec_setSetFocus"

::method menuFrom class external "LIBRARY ooconsole inrec_menuFrom_cls"
::attribute id get external "LIBRARY ooconsole inrec_ID"
::attribute setID set external "LIBRARY ooconsole inrec_setID"

/**
 *  The ooConsoleConstants class contains all Constant values ...
 */
::class 'ooConsoleConstants' public mixinclass Object

