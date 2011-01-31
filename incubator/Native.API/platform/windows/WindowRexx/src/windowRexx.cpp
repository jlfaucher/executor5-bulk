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
 *  Code for the implementation of the .WindowRexx class, available as an
 *  external package library: windowRexx.frm
 *
 *  It is expected that the methods of the .WindowRexx class will be merged into
 *  the .Window class of ooDialog.
 *
 *  Not much comment here, if you have questions contact Mark Miesfeld on the
 *  ooRexx devel list.  If not subscribed go to:
 *
 *  https://sourceforge.net/mail/?group_id=119701
 *
 *  to subscribe.
 */

#include <windows.h>
#include <CommCtrl.h>
#include <stdio.h>
#include "oorexxapi.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL REXXENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        //
    }
    else if ( fdwReason == DLL_PROCESS_DETACH )
    {
        //
    }
    return(TRUE);
}

#ifdef __cplusplus
}
#endif

/*-----------------------------------------------------------------------------*
 *
 * Some defines, structs, and helper functions copied from ooDialog
 *
*------------------------------------------------------------------------------*/
#define API_FAILED_MSG            "system API %s() failed; OS error code %d"

/* Struct for the WindowBase object CSelf. */
typedef struct _wbCSelf {
    HWND              hwnd;
    RexxObjectPtr     rexxHwnd;
    RexxObjectPtr     rexxSelf;
    uint32_t          sizeX;
    uint32_t          sizeY;
} CWindowBase;
typedef CWindowBase *pCWindowBase;

inline void systemServiceException(RexxMethodContext *c, char *msg)
{
    c->RaiseException1(Rexx_Error_System_service_user_defined, c->String(msg));
}

void systemServiceExceptionCode(RexxMethodContext *context, const char *msg, const char *arg1, DWORD rc)
{
    TCHAR buffer[256];
    _snprintf(buffer, sizeof(buffer), msg, arg1, rc);
    systemServiceException(context, buffer);
}


inline void pointer2string(char *result, void *pointer)
{
    sprintf(result, "0x%p", pointer);
}

RexxStringObject pointer2string(RexxMethodContext *c, void *pointer)
{
    char buf[32];
    pointer2string(buf, pointer);
    return c->String(buf);
}

/**
 * Gets a Class object.
 *
 * This is for use for classes visible within the scope of the context, like say
 * .PlainBaseDialog, or .Rect.  Use c->FindClass() to directly get classes from
 * the environment like .Bag or .Directory.
 *
 * @param c     The method context we are operating in.
 * @param name  The name of the class to try and find.
 *
 * @return The class object or null on failure.
 *
 * @remarks  When null is returned an error has been raised: 98.909
 *
 *   98: The language processor detected a specific error during execution. The
 *   associated error gives the reason for the error.
 *
 *   909: Class "class" not found
 */
RexxClassObject rxGetContextClass(RexxMethodContext *c, CSTRING name)
{
    RexxClassObject theClass = c->FindContextClass(name);
    if ( theClass == NULL )
    {
        c->RaiseException1(Rexx_Error_Execution_noclass, c->String(name));
    }
    return theClass;
}

RexxObjectPtr createNewWindow(RexxMethodContext *c, HWND hwnd)
{
    RexxClassObject windowCls = rxGetContextClass(c, "WINDOWREXX");
    if ( windowCls == NULLOBJECT )
    {
        return c->Nil();
    }

    return c->SendMessage1(windowCls, "NEW", pointer2string(c, hwnd));
}

RexxMethod1(RexxObjectPtr, window_findWindow_cls, CSTRING, title)
{
    HWND hwnd = FindWindow(NULL, title);

    if ( hwnd == NULL )
    {
        return context->Nil();
    }
    else
    {
        return createNewWindow(context, hwnd);
    }
}


RexxMethod2(RexxObjectPtr, window_init, POINTERSTRING, _hwnd, OSELF, self)
{
    HWND hwnd = (HWND)_hwnd;

    if ( !IsWindow(hwnd) )
    {
        context->RaiseException1(Rexx_Error_Incorrect_method_user_defined,
                                 context->String("Argument 1 is not a valid window handle"));
    }
    else
    {
        RexxBufferObject obj = context->NewBuffer(sizeof(CWindowBase));
        if ( obj == NULLOBJECT )
        {
            return NULLOBJECT;
        }

        RECT r = {0};
        if ( GetWindowRect(hwnd, &r) == 0 )
        {
            systemServiceExceptionCode(context, API_FAILED_MSG, "GetWindowRect", GetLastError());
            return NULLOBJECT;
        }

        pCWindowBase pcwb = (pCWindowBase)context->BufferData(obj);
        memset(pcwb, 0, sizeof(CWindowBase));

        pcwb->hwnd     = hwnd;
        pcwb->rexxSelf = self;
        pcwb->rexxHwnd = pointer2string(context, hwnd);

        pcwb->sizeX =  (uint32_t)(r.right - r.left);
        pcwb->sizeY =  (uint32_t)(r.bottom - r.top);

        context->SetObjectVariable("CSELF", obj);
    }
    return NULLOBJECT;
}


RexxMethod3(uint32_t, window_sendKeyPress, uint8_t, key, OPTIONAL_RexxArrayObject, modifiers, CSELF, pCSelf)
{
    LPINPUT keys  = NULL;
    uint32_t ret = 0;
    uint32_t count = 0;

    if ( argumentExists(2) )
    {
        count = (uint32_t)context->ArrayItems(modifiers);
    }
    count++;

    keys = (INPUT *)LocalAlloc(LPTR, sizeof(INPUT) * count * 2);
    if ( keys == NULL )
    {
        context->RaiseException1(Rexx_Error_System_service_user_defined,
                                 context->String("Failed to allocate memory"));
        goto done_out;
    }

    if ( count == 1 )
    {
        keys[0].type = INPUT_KEYBOARD;
        keys[0].ki.dwExtraInfo = GetMessageExtraInfo();
        keys[0].ki.wVk = key;

        keys[1].ki.wVk = key;
        keys[1].type = INPUT_KEYBOARD;
        keys[1].ki.dwFlags = KEYEVENTF_KEYUP;
        keys[1].ki.dwExtraInfo = GetMessageExtraInfo();
    }
    else
    {
        uint32_t      modKey;
        RexxObjectPtr mod;

        // For each key initialize a down key and an up key event. We want the
        // modifier keys pressed down first and released last.
        for ( size_t i = 0; i < count; i++)
        {
            if ( i == count - 1 )
            {
                keys[i].type = INPUT_KEYBOARD;
                keys[i].ki.dwExtraInfo = GetMessageExtraInfo();
                keys[i].ki.wVk = key;

                keys[i + 1].ki.wVk = key;
                keys[i + 1].type = INPUT_KEYBOARD;
                keys[i + 1].ki.dwFlags = KEYEVENTF_KEYUP;
                keys[i + 1].ki.dwExtraInfo = GetMessageExtraInfo();
            }
            else
            {
                mod = context->ArrayAt(modifiers, i + 1);
                if ( mod == NULLOBJECT || ! context->ObjectToUnsignedInt32(mod, &modKey) ||
                     modKey == 0 || modKey > 255 )
                {
                    context->RaiseException1(
                        Rexx_Error_Incorrect_method_user_defined,
                        context->String("Argument 2 does not contain a valid virtual key code"));

                    goto done_out;
                }

                keys[i].type = INPUT_KEYBOARD;
                keys[i].ki.dwExtraInfo = GetMessageExtraInfo();
                keys[i].ki.wVk = modKey;

                keys[i + count + 1].ki.wVk = modKey;
                keys[i + count + 1].type = INPUT_KEYBOARD;
                keys[i + count + 1].ki.dwFlags = KEYEVENTF_KEYUP;
                keys[i + count + 1].ki.dwExtraInfo = GetMessageExtraInfo();
            }
        }
    }

    pCWindowBase pcwb = (pCWindowBase)pCSelf;

    if ( SetForegroundWindow(pcwb->hwnd) )
    {
        ret = SendInput(count * 2, keys, sizeof(INPUT));
    }

done_out:
    if ( keys != NULL )
    {
        LocalFree(keys);
    }
    return ret;
}


/** Window::toForeground()
 *
 *
 *  When this is merged to ooDialog, this function should use:
 *
 *  oodSetForegroundWindow()
 *
 */
RexxMethod1(RexxObjectPtr, window_toForeground, CSELF, pCSelf)
{
    HWND hwnd = ((pCWindowBase)pCSelf)->hwnd;

    SetLastError(0);

    HWND hwndOld =  GetForegroundWindow();
    if (hwndOld != hwnd)
    {
        SetForegroundWindow(hwnd);
    }
    return pointer2string(context, hwndOld);
}

RexxMethodEntry window_methods[] =
{
    REXX_METHOD(window_findWindow_cls,  window_findWindow_cls),
    REXX_METHOD(window_init,            window_init),
    REXX_METHOD(window_sendKeyPress,    window_sendKeyPress),
    REXX_METHOD(window_toForeground,    window_toForeground),

    REXX_LAST_METHOD()
};

RexxPackageEntry window_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // needs at least the 4.0.0 interpreter
    "windowrexx",                        // name of the package
    "1.0.0",                             // package information
    NULL,                                // no load/unload functions
    NULL,
    NULL,                                // the exported functions
    window_methods                       // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(window);
