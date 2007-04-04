;------------------------------------------------------------------------------*
;
; Copyright (c) 2007 Rexx Language Association. All rights reserved.
;
; This program and the accompanying materials are made available under
; the terms of the Common Public License v1.0 which accompanies this
; distribution. A copy is also available at the following address:
; http://www.oorexx.org/license.html
;
; Redistribution and use in source and binary forms, with or
; without modification, are permitted provided that the following
; conditions are met:
;
; Redistributions of source code must retain the above copyright
; notice, this list of conditions and the following disclaimer.
; Redistributions in binary form must reproduce the above copyright
; notice, this list of conditions and the following disclaimer in
; the documentation and/or other materials provided with the distribution.
;
; Neither the name of Rexx Language Association nor the names
; of its contributors may be used to endorse or promote products
; derived from this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
; "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
; LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
; FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
; OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
; SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
; TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
; OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
; OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
; NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;------------------------------------------------------------------------------*

; NMAKE_lineout  Nmake Helper command.  Write a blank line to the console.

		.486p
		.model	flat, syscall
		.stack	4096

; Define for stdout handle from Winbase.h
STD_OUTPUT_HANDLE EQU	-11

; Prototypes for Windows System functions from kernel32.lib
		GetStdHandle PROTO NEAR32 stdcall,
			nStdHandle:DWORD		; ID of handle to get

		WriteFile PROTO NEAR32 stdcall,
			hFile:DWORD,			; Handle of file to write to
			lpBuffer:NEAR32,		; Data to write to file
			nNumberOfBytesToWrite:DWORD,	; Count of bytes to write
			lpNumberOfBytesWritten:NEAR32,	; Returned, count bytes written
			lpOverlapped:NEAR32		; Pointer to OVERLAPPED struct

		ExitProcess PROTO NEAR32 stdcall,
			dwExitCode:DWORD		; Return code

		.data
		stdout	DD 0
		nl	DB 13, 10
		count	DD 0

		.code
		public	__start

__start		proc	near32
		INVOKE GetStdHandle,			; Get the handle to standard out
			STD_OUTPUT_HANDLE		; and put it in stdout

		mov	stdout, eax

		INVOKE WriteFile,			; Write new line to standard out
			stdout,
			NEAR32 PTR nl,
			2,
			NEAR32 PTR count,
			0

		INVOKE ExitProcess,			; Exit with 0
			0

__start		endp
		end

