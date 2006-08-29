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
#include <windows.h>
#include <oorexx.h>
#include <stdio.h>
#include <dlgs.h>
#include <malloc.h>
#include "oovutil.h"


RexxFunction1(uintptr_t, FindTheWindow, CSTRING, name)
{
    return FindWindow(NULL, name);
}                               1

RexxFunction1(RexxStringObject, WndGetWindowText, uintptr_t, handle)
{
    HWND hW = (HWND)handle;
    buffer char[256];
    size_t strlength = GetWindowText((HWND)hW, buffer, sizeof(buffer));
    return context->NewString(buffer, strlength);
}

RexxFunction1(RexxBoolean, WndSetWindowText, uintptr_t, handle, CSTRING, text)
{
    return !SetWindowText((HWND)handle, text);
}


RexxFunction2(RexxBoolean, WndEnableWindow, uintptr_t, handle, int, enable)
{
    HWND hW = (HWND)handle;
    int st = 0;
    if (enable != 0)
    {
       if (IsWindowEnabled((HWND) hW))
           st = 0;
       else st = EnableWindow((HWND) hW, true);
    }
    else
    {
       if (!IsWindowEnabled((HWND) hW))
          st = 0;
       else st = EnableWindow((HWND) hW, false);
    }
    return st;
}


RexxFunction2(uintptr_t, WndSetFocus, uintptr_t, dlgHndle, uintptr_t, controlHandle)
{
    return SendMessage((HWND)dlgHandle, GETSETFOCUS, 0, (HWND)controlHandle);
}


RexxFunction1(uintptr_t, WndGetFocus, uintptr_t, handle)
{
    return SendMessage((HWND)handle, GETSETFOCUS, 0, 0);
}

RexxFunction1(uintptr_t, WndSetTopWindow, uintptr_t, handle)
{
    HWND hW = GetForegroundWindow();
    if (hW != (HWND)handle)
    {
        hW = SetForegroundWindow((HWND)handle);
    }
    return hW;
}

RexxFunction0(uintptr_t, WndGetForegroundWindow)
{
    return GetForegroundWindow();
}

RexxFunction1(RexxStringObject, WndGetRectangle, uintptr_t, handle)
{
    RECT r;
    if (!GetWindowRect((HWND)handle, &r))
    {
        return context->NewStringFromAsciiz("");
    }
    char buffer[256];

    sprintf(buffer, "%d %d %d %d", r.left, r.top, r.right, r.bottom);
    return context->NewStringFromAsciiz(buffer);
}


RexxFunction1(int, WndGetId, uintptr_t, handle)
{
    return GetWindowLong((HWND)handle, GWL_ID);
}

RexxFunction1(uintptr_t, WndGetMouseCapture, uintptr_t, handle)
{
    return SendMessage((HWND)handle, GETSETCAPTURE, 0, 0);
}

RexxFunction1(uintptr_t, WndReleaseMouseCapture, uintptr_t, handle)
{
    return SendMessage((HWND)handle, GETSETCAPTURE, 2, 0);
}

RexxFunction2(uintptr_t, WndSetMouseCapture, uintptr_t, handle, uintptr_t, target)
{
    return SendMessage((HWND)handle, GETSETCAPTURE, 1, (HWND)target);
}


RexxFunction0(RexxStringObject, WndGetCursorPosition)
{
    char buffer[256];
    POINT pt;
    GetCursorPos(&pt);
    sprintf(buffer, "%ld %ld", pt.x, pt.y);
    return context->NewStringFromAsciiz(buffer);
}

RexxFunction2(int, WndSetCursorPosition, int, x, int, y)
{
    return !SetCursorPos(x, y);
}

RexxFunction2(uintptr_t, WndSetCursor, uintptr_t, handle, int, res)
{
    HCURSOR oC, hC;
    hC = LoadCursor(NULL, MAKEINTRESOURCE(res));
    oC = (HCURSOR)SetClassLong((HWND)handle, GCL_HCURSOR, (LONG)hC);
    SetCursor(hC);
    return oC;
}

RexxFunction2(uintptr_t, WndReleaseCursor, uintptr_t, handle, uintptr_t, cursor)
{
    HCURSOR hC = (HCURSOR)cursor;
    if (hC) {
        SetClassLong((HWND)handle, GCL_HCURSOR, (LONG)hC);
        return SetCursor(hC);
    }
    else
    {
        SetClassLong((HWND)handle, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
        return SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
}

RexxFunction1(uintptr_t, WndGetKeyButtonState, int, key)
{
    if (GetSystemMetrics(SM_SWAPBUTTON))
    {
        if (key == 1)
        {
            key = 2;
        }
        else if (k == 2)
        {
            key = 1;
        }
    }
    return SendMessage((HWND)hW, GETKEYSTATE, k, 0);
}

RexxFunction3(RexxStringObject, WndScreenToClient, uintptr_t, handle, int, x, int, y)
{
    HWND hW = (HWND)handle;
    char buffer[256];
    buffer[0] = '\0';

    if (hW)
    {
        POINT pt;
        BOOL ret;
        pt.x = x;
        pt.y = y;


        if (ScreenToClient(hW, &pt))
        {
            sprintf(buffer, "%ld %ld", pt.x, pt.y);
        }
    }
    return context->NewStringFromAsciiz(buffer);
}

RexxFunction3(RexxStringObject, WndClientToScreen, uintptr_t, handle, int, x, int, y)
{
    HWND hW = (HWND)handle;
    char buffer[256];
    buffer[0] = '\0';

    if (hW)
    {
        POINT pt;
        BOOL ret;
        pt.x = x;
        pt.y = y;


        if (ClientToScreen(hW, &pt))
        {
            sprintf(buffer, "%ld %ld", pt.x, pt.y);
        }
    }
    return context->NewStringFromAsciiz(buffer);
}

RexxFunction1(RexxBoolean, WinRedrawWindow, uintptr_t handle)
{
    return !RedrawWindow((HWND)handle, NULL, NULL, RDW_ERASE | RDW_ALLCHILDREN);
}

RexxFunction1(int, WinUpdateWindow, uintptr_t handle)
{
    HWND hW = (HWND)handle;
    RECT r;
    GetClientRect(hW, &r);
    InvalidateRect(hW, &r, true);
    return 0;
}

RexxFunction2(int, WinFastUpdate, uintptr_t, handle, RexxBoolean, show)
{
    HWND hW = (HWND)handle;
    LONG st = GetWindowLong(hW, GWL_STYLE);
    if (st)
    {
        if (show)
        {
            st |= WS_VISIBLE;
        }
        else
        {
            st ^= WS_VISIBLE;
        }
        SetWindowLong(hW, GWL_STYLE, st);
        return 0;
    }
    return 1;
}


RexxFunction3(RexxBoolean, WinShowWindow, uintptr_t, handle, CSTRING, options, OPTIONAL_uintptr_t, admin)
{
    HWND hW = (HWND)handle;
    if (admin != 0)
    {
        dlgAdm = (DIALOGADMIN*)admin;
        dlgAdm->AktChild = hW;
    }
    if (strstr(options, "HIDE")) k = SW_HIDE;
    else if (strstr(options, "NORMAL")) k = SW_SHOWNORMAL;
    else if (strstr(options, "MIN")) k = SW_SHOWMINIMIZED;
    else if (strstr(options, "MAX")) k = SW_SHOWMAXIMIZED;
    else if (strstr(options, "DEFAULT")) k = SW_SHOWDEFAULT;
    else if (strstr(options, "RESTORE")) k = SW_SHOWDEFAULT;
    else if (strstr(options, "INACTIVE")) k = SW_SHOWNA;
    else k = SW_SHOW;

    return !ShowWindow(hW, k);
}

RexxFunction6(int, WndSetWindowPosition, uintptr_t, handle, int, x, int, y, int, width, int, height, CSTRING, options)
{
    opts = 0;
    if (strstr(options, "NOMOVE")) opts |= SWP_NOMOVE;
    if (strstr(options, "NOSIZE")) opts |= SWP_NOSIZE;
    if (strstr(options, "HIDEWINDOW")) opts |= SWP_HIDEWINDOW;
    if (strstr(options, "SHOWWINDOW")) opts |= SWP_SHOWWINDOW;
    if (strstr(options, "NOREDRAW")) opts |= SWP_NOREDRAW;

    return !SetWindowPos((HWND)handle, 0, x, y, width, height, opts | SWP_NOZORDER);
}

RexxFunction1(int, WndGetScrollPosition, uintptr_t, handle, int, orientation)
{
    return GetScrollPos((HWND)handle, orientation);
}


RexxFunction4(int, WndSetScrollPosition, uintptr_t, handle, int, orientation, int, pos, RexxBoolean, redraw)
{
    return SetScrollPos((HWND)handle, orientation, pos, redraw);
}

RexxFunction3(int, WndScrollContents, uintptr_t handle, int, xamount, int, yamount)
{
    return ScrollWindow((HWND)handle, xamount, yamount, NULL, NULL);
}

