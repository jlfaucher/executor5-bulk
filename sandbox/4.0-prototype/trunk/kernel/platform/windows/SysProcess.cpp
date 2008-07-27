/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/*****************************************************************************/
/* REXX Windows Support                                                      */
/*                                                                           */
/* Process support for Windows                                               */
/*                                                                           */
/*****************************************************************************/

#include "windows.h"
#include "SysProcess.hpp"

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   relinquish                                   */
/*                                                                   */
/*   Function:          Performs peekmsg to yield to other processes */
/*                                                                   */
/*********************************************************************/

void SysProcess::relinquish(void)
{
    MSG msg;

   /*  If there is a msg in the message queue, dispatch it to the appropriate
    *  window proc.
    */

    if (!useMessageLoop)
    {
        return;
    }

    if (PeekMessage (&msg,   // message structure
      NULL,                  // handle of window receiving the message
      0,                     // lowest message to examine
      0,
      PM_REMOVE))            // highest message to examine
    {
      TranslateMessage(&msg);// Translates virtual key codes
      DispatchMessage(&msg); // Dispatches message to window
    }
}

SysProcess::PlatformVersion SysProcess::platformVersion = SysProcess::NoSystem;
bool SysProcess::useMessageLoop = false;

void SysProcess::setSystemVersion()
{
    OSVERSIONINFO version_info;
    version_info.dwOSVersionInfoSize = sizeof(version_info);  // if not set --> violation error
    GetVersionEx(&version_info);

    if (version_info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        platformVersion = Windows98;
    }
    else
    {
        platformVersion = WindowsNT;
    }
}

/**
 * Get the current user name information.
 *
 * @param buffer The buffer (of at least MAX_USERID_LENGTH characters) into which the userid is copied.
 */
void SysProcess::getUserID(char *buffer)
{
    DWORD account_size = MAX_USERID_LENGTH;
    GetUserName(buffer, &account_size);
}

#define OM_WAKEUP (WM_USER+10)

/**
 * Callback procedure for message-based timer operations.
 *
 * @param hwnd    The window handle.
 * @param uMsg    The message identifier.
 * @param idEvent The event id.
 * @param dwTime  The time value.
 */
void CALLBACK sleepTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    DWORD threadId;
    KillTimer(NULL, idEvent);                /* kill the timer that just ended */
    threadId = GetCurrentThreadId();
    PostThreadMessage(threadId, OM_WAKEUP, 0 , 0);   /* send ourself the wakeup message*/
}


/**
 * Do process specific sleep.
 *
 * @param milliseconds
 *               The number of milliseconds to sleep.
 */
void SysProcess::sleep(int milliseconds)
{
    if (useMessageLoop)
    {
        MSG msg;
        if ( !(SetTimer(NULL, 0, (LONG)milliseconds, (TIMERPROC) sleepTimerProc)) )
        {
            return;
        }
        while (GetMessage (&msg, NULL, 0, 0) )
        {
            if (msg.message == OM_WAKEUP)    /* If our message exit loop   */
            {
                break;
            }
            TranslateMessage( &msg );
            DispatchMessage ( &msg );
        }
    }
    else
    {
        Sleep(milliseconds);
    }
}
