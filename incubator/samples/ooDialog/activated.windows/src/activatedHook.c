/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

/** A special purpose ooRexx external function library. The NoteActivatedWindows
 *  function is an ooRexx callable function.  It is used to either install or
 *  uninstall a system global hook that monitors when the shell is going to
 *  activate a top-level window.
 *
 *  Note that the hook procedure within this DLL can be called within the
 *  context of any thread in the same Desktop as the thread that installs the
 *  hook.
 *
 *  The hook must be installed from an application using a valid window handle.
 *  Messages are posted back to the window procedure associated with the window.
 *  This is designed to work with ooDialog, but there is no reason it could not
 *  work with some other ooRexx application that had a valid window and message
 *  processing loop.
 */
#define NTDDI_VERSION NTDDI_WIN2KSP4

#include <windows.h>
#include <rexx.h>

/* The user message sent to the dialog window to notify it of a new active
 * window.  WM_USER + 0x0101 is 0x0501.  ooDialog's user messages start at
 * WM_USER + 0x0601 so this message will not interfere.
 */
#define WM_USER_WINDOWACTIVATED      WM_USER + 0x0101

/* Return codes, returned to ooRexx from NoteActivatedWindows(). */
#define SUCCESS_RC                              0
#define ERROR_BAD_HWND_RC                      -1
#define ERROR_HOOK_ALREADY_INSTALLED_RC        -2
#define ERROR_HOOK_NOT_INSTALLED_RC            -3
#define ERROR_BAD_ARGS_RC                      -4

/** The shared data seqment.  The window handle to the dialog that installed the
 *  hook is saved here.  This must be in a shared segment or it will be
 *  reinitialized each time this DLL is loaded into a process's memory space.
 */
#pragma data_seg("shared_data")
static HWND	 hwndDlg = NULL;
static HHOOK hHookProc = NULL;
#pragma data_seg()

/* Function prototypes */
LRESULT CALLBACK ShellHookProc(int, WPARAM, LPARAM);

/* Handle to this DLL instance */
HANDLE	hInstance = NULL;


/**
 *  The Rexx external function.  Syntax in ooRexx:
 *    ret = NoteActivatedWindows(hwnd, mode)
 *
 * argv[0]  hwnd  Handle of window to receive window activated messages.  This
 *                would be a dialog handle in ooDialog.
 *
 * argv[1]  mode  [I]nstall or [U]ninstall
 *
 * Returns to ooRexx:
 *
 *       0  Succes.
 *      -1  The hwnd parameter is not valid.
 *      -2  Can not install the hook when it is aleady installed.
 *      -3  Can not remove hook when it is not installed.
 *      -4  The parameters supplied are not correct.
 *   other  System error code, the hook was not set.
 *
 */
ULONG APIENTRY NoteActivatedWindows(
  PUCHAR funcname,
  ULONG argc,
  RXSTRING argv[],
  PUCHAR qname,
  PRXSTRING retstr )
{
    int code = ERROR_BAD_ARGS_RC;

    if ( argc == 2 && argv[1].strptr[0] == 'I' )
    {
        if ( hwndDlg == NULL && hHookProc == NULL )
        {

            hwndDlg = (HWND)atol(argv[0].strptr);
            if ( hwndDlg != 0 && IsWindow(hwndDlg) )
            {
                hHookProc = SetWindowsHookEx(WH_SHELL, (HOOKPROC)ShellHookProc, hInstance, 0);
                if ( hHookProc != NULL )
                {
                    code = SUCCESS_RC;
                }
                else
                {
                    hwndDlg = NULL;
                    code = (int)GetLastError();
                }
            }
            else
            {
                hwndDlg = NULL;
                code = ERROR_BAD_HWND_RC;
            }
        }
        else
        {
            code = ERROR_HOOK_ALREADY_INSTALLED_RC;
        }
    }
    else if ( argc == 2 && argv[1].strptr[0] == 'U' )
    {
        if ( hHookProc != NULL )
        {
            UnhookWindowsHookEx(hHookProc);
            hwndDlg = NULL;
            hHookProc = NULL;
            code = SUCCESS_RC;
        }
        else
        {
            code = ERROR_HOOK_NOT_INSTALLED_RC;
        }
    }

    sprintf(retstr->strptr, "%d", code);
    retstr->strlength = strlen(retstr->strptr);
    return 0;
}



/** The shell hook procedure.  When the HSHELL_WINDOWACTIVATED message is
 * received, a message is posted back to the dialog that installed the hook.
 *
 * wParam is the window being activated, lParam is true if the window is in full
 * screen mode, otherwise false.  The parameters are simply passed on to the
 * dialog.
 */
LRESULT CALLBACK ShellHookProc(int code, WPARAM wParam, LPARAM lParam)
{

    if ( code == HSHELL_WINDOWACTIVATED && hwndDlg != NULL )
    {
        PostMessage(hwndDlg, WM_USER_WINDOWACTIVATED, wParam, (LPARAM)lParam);
    }
	return CallNextHookEx(hHookProc, code, wParam, lParam);
}


#ifdef __cplusplus
extern "C" {
#endif

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
   if (fdwReason == DLL_PROCESS_ATTACH)
   {
      hInstance = hinstDLL;
   }
   return TRUE;
}

#ifdef __cplusplus
}
#endif
