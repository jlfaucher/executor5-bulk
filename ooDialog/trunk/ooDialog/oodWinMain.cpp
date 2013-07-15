/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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
 * oodMain.cpp
 *
 * ooDialog.exe, a stand alone interface to run ooDialog programs.  This is
 * essentially rexxhide, with some ooDialog specific modifications.
 *
 * At this time there is no ooDialog specific code that would prevent
 * ooDialog.exe from running any ooRexx program with no console.
 */

#include <windows.h>
#include <commctrl.h>
#include <oorexxapi.h>
#include <stdio.h>
#include <shlwapi.h>
#include <shlobj.h>
#include "oodShared.hpp"
#include "oodExecutable.hpp"

inline CSTRING level2text(uint32_t level)
{

    switch ( level )
    {
        case SECURITY_MANDATORY_UNTRUSTED_RID: return "Untrusted";
        case SECURITY_MANDATORY_LOW_RID:       return "Low";
        case SECURITY_MANDATORY_MEDIUM_RID:    return "Medium";
        case SECURITY_MANDATORY_HIGH_RID:      return "High";
        case SECURITY_MANDATORY_SYSTEM_RID:    return "System";
        default:                               return "Unknown";
    }
}

inline bool requiresElevation(pConfigureArguments pca)
{
    if ( pca->isVistaOrLater && ! pca->isRunAsAdmin )
    {
        return true;
    }
    return false;
}

static void setDlgIcon(HWND hDlg, HINSTANCE hInstance)
{
     HANDLE hIcon = LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON,
                              GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
      if( hIcon )
      {
          SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
      }
}

/**
 * Formats an error message for a system error and shows it in a message box
 *
 * @param function  Name of system function that failed
 * @param title     Caption for message box
 * @param dw        System error code
 */
static void reportError(HWND hwnd, CSTRING function, CSTRING title, HRESULT dw)
{
    char  msgBuf[HUGE_BUF_SIZE];
	void *formatMsgBuf;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&formatMsgBuf, 0, NULL);

    _snprintf(msgBuf, HUGE_BUF_SIZE, "Error executing Windows API:\n\nFunction:\t\t%s\nError Code:\t%d\n\n%s",
              function, dw, formatMsgBuf);

	internalErrorMsgBox(hwnd, msgBuf, title);
	LocalFree(formatMsgBuf);
}

/**
 * Formats an error message for a system error, plus some follow up text, and
 * shows it in a message box
 *
 * @param function  Name of system function that failed
 * @param title     Caption for message box
 * @param dw        System error code
 */
static void reportErrorPlus(HWND hwnd, CSTRING function, CSTRING title, CSTRING extraMsg, HRESULT dw)
{
    char  msgBuf[HUGE_BUF_SIZE];
	void *formatMsgBuf;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&formatMsgBuf, 0, NULL);

    _snprintf(msgBuf, HUGE_BUF_SIZE, "Error executing Windows API:\n\nFunction:\t\t%s\n"
                                     "Error Code:\t%d\n\n%s\n\n%s",
              function, dw, formatMsgBuf, extraMsg);

	internalErrorMsgBox(hwnd, msgBuf, title);
	LocalFree(formatMsgBuf);
}

/**
 * Formats an error message using static memory and shows it in a message box.
 *
 * This is primarily for use in memory allocation errors.  The idea is to avoid
 * allocating memory to display an out of memory message.  If we failed to
 * allocate memory, I doubt this will work anyway.
 *
 * @param fmtStr
 * @param title
 * @param dw
 */
static void reportError(HWND hwnd, CSTRING fmtStr, CSTRING title, CSTRING function, uint32_t rc)
{
    char  msgBuf[HUGE_BUF_SIZE];

    _snprintf(msgBuf, HUGE_BUF_SIZE, fmtStr, function, rc);
	internalErrorMsgBox(hwnd, msgBuf, title);
}

/**
 * Formats an error message that uses a single, numerical, insert and shows it
 * in a message box.
 *
 * @param fmt
 * @param insert
 */
static void reportError(CSTRING fmt, uint32_t insert, CSTRING title)
{
    char buf[MEDIUM_BUF_SIZE];

    _snprintf(buf, MEDIUM_BUF_SIZE, fmt, insert);
    internalErrorMsgBox(buf, title);
}

inline void unicodeConversionErr(void)
{
    internalErrorMsgBox(UNICODE_CONVERSION_ERR, OS_ERR_TITLE);
}


/**
 *
 *
 *
 * @return uint32_t
 *
 * @notes  This function must be run on Vista or later, or it fails.  We assume
 *         the caller has already checked the OS.
 */
uint32_t getProcessIntegrityLevel(HWND hwnd, uint32_t *rc)
{
    PTOKEN_MANDATORY_LABEL pTokenIL       = NULL;
    uint32_t               integrityLevel = 0;
    uint32_t               error          = ERROR_SUCCESS;
    HANDLE                 hToken         = NULL;
    uint32_t               tokenSize      = 0;

    // Open the primary access token of the process with TOKEN_QUERY.
    if ( ! OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) )
    {
        error = GetLastError();
        reportError(hwnd, "OpenProcessToken", OS_ERR_TITLE, error);
        goto done_out;
    }

    // Query the size of the token integrity level so we can allocate a buffer
    // for it.
    if ( ! GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, (DWORD *)&tokenSize) )
    {
        if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
        {
            error = GetLastError();
            reportError(hwnd, "GetTokenInformation", OS_ERR_TITLE, error);
            goto done_out;
        }
    }

    // Now we allocate a buffer for the integrity level information.
    pTokenIL = (TOKEN_MANDATORY_LABEL *)LocalAlloc(LPTR, tokenSize);
    if (pTokenIL == NULL)
    {
        error = GetLastError();
        reportError(hwnd, "LocalAlloc", OS_ERR_TITLE, error);
        goto done_out;
    }

    // Retrieve token integrity level information.
    if ( ! GetTokenInformation(hToken, TokenIntegrityLevel, pTokenIL, tokenSize, (DWORD *)&tokenSize))
    {
        error = GetLastError();
        reportError(hwnd, "GetTokenInformation", OS_ERR_TITLE, error);
        goto done_out;
    }

    // Integrity Level SIDs are in the form of S-1-16-0xXXXX. (e.g.
    // S-1-16-0x1000 stands for low integrity level SID). There is one and
    // only one subauthority.
    integrityLevel = *GetSidSubAuthority(pTokenIL->Label.Sid, 0);

done_out:
    if ( hToken )
    {
        CloseHandle(hToken);
    }
    if ( pTokenIL)
    {
        LocalFree(pTokenIL);
    }

    *rc = error;

    return integrityLevel;
}


/**
 *
 *
 *
 * @param hwnd
 * @param isVistaOrLater
 * @param rc
 *
 * @return bool
 */
bool isUserInAdminGroup(HWND hwnd, bool isVistaOrLater, uint32_t *rc)
{
    HANDLE   hTokenToCheck = NULL;
    HANDLE   hToken        = NULL;
    bool     inGroup       = false;
    uint32_t error         = ERROR_SUCCESS;
    DWORD    cbSize        = 0;

    // Open the primary access token of the process for query and duplicate.
    if ( ! OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hToken) )
    {
        error = GetLastError();
        reportError(hwnd, "OpenProcessToken", OS_ERR_TITLE, error);
        goto done_out;
    }

    if ( isVistaOrLater )
    {
        // Running Windows Vista or later.  Determine token type: limited,
        // elevated, or default.
        TOKEN_ELEVATION_TYPE elevType;
        if ( ! GetTokenInformation(hToken, TokenElevationType, &elevType, sizeof(elevType), &cbSize) )
        {
            error = GetLastError();
            reportError(hwnd, "GetTokenInformation", OS_ERR_TITLE, error);
            goto done_out;
        }

        // If limited, get the linked elevated token for further check.
        if ( TokenElevationTypeLimited == elevType )
        {
            if ( ! GetTokenInformation(hToken, TokenLinkedToken, &hTokenToCheck, sizeof(hTokenToCheck), &cbSize) )
            {
                error = GetLastError();
                reportError(hwnd, "GetTokenInformation", OS_ERR_TITLE, error);
                goto done_out;
            }
        }
    }

    // CheckTokenMembership requires an impersonation token. If we just got a
    // linked token, it already is an impersonation token.  If we did not get
    // a linked token, duplicate the original into an impersonation token for
    // CheckTokenMembership.
    if ( ! hTokenToCheck )
    {
        if ( ! DuplicateToken(hToken, SecurityIdentification, &hTokenToCheck) )
        {
            error = GetLastError();
            reportError(hwnd, "DuplicateToken", OS_ERR_TITLE, error);
            goto done_out;
        }
    }

    // Create the SID corresponding to the Administrators group.
    BYTE adminSID[SECURITY_MAX_SID_SIZE];

    cbSize = sizeof(adminSID);
    if ( ! CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &cbSize) )
    {
        error = GetLastError();
        reportError(hwnd, "CreateWellKnownSid", OS_ERR_TITLE, error);
        goto done_out;
    }

    // Check if the token to be checked contains admin SID.
    // http://msdn.microsoft.com/en-us/library/aa379596(VS.85).aspx:
    // To determine whether a SID is enabled in a token, that is, whether it
    // has the SE_GROUP_ENABLED attribute, call CheckTokenMembership.
    BOOL inAdminGroup;
    if ( ! CheckTokenMembership(hTokenToCheck, &adminSID, &inAdminGroup) )
    {
        error = GetLastError();
        reportError(hwnd, "CheckTokenMembership", OS_ERR_TITLE, error);
        goto done_out;
    }

    inGroup = inAdminGroup ? true : false;

done_out:
    if ( hToken )
    {
        CloseHandle(hToken);
    }
    if ( hTokenToCheck )
    {
        CloseHandle(hTokenToCheck);
    }

    *rc = error;

    return inGroup;
}


/**
 *
 *
 *
 * @return bool
 *
 * @notes  This function must be run on Vista or later, or it fails.  We assume
 *         the caller has already checked the OS.
 */
bool isProcessElevated(HWND hwnd, uint32_t *rc)
{
    uint32_t error    = ERROR_SUCCESS;
    HANDLE   hToken   = NULL;
    bool     elevated = false;

    // Open the primary access token of the process with TOKEN_QUERY.
    if ( ! OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        error = GetLastError();
        reportError(hwnd, "OpenProcessToken", OS_ERR_TITLE, error);
        goto done_out;
    }

    // Retrieve token elevation information.
    TOKEN_ELEVATION elevation;
    uint32_t        size;
    if ( ! GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), (DWORD *)&size) )
    {
        error = GetLastError();
        reportError(hwnd, "GetTokenInformation", OS_ERR_TITLE, error);
        goto done_out;
    }

    elevated = elevation.TokenIsElevated ? true : false;

done_out:
    if ( hToken )
    {
        CloseHandle(hToken);
    }
    *rc = error;

    return elevated;
}


/**
 *
 *
 * @param hwnd
 * @param isVistaOrLater
 * @param rc
 *
 * @return bool
 */
bool runningAsAdmin(HWND hwnd, uint32_t *rc)
{
    PSID     adminGroup = NULL;
    bool     result     = false;
    uint32_t error      = ERROR_SUCCESS;

    // get SID of the admin group.
    SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
    if ( ! AllocateAndInitializeSid(&authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
                                    0, 0, 0, 0, 0, 0, &adminGroup) )
    {
        error = GetLastError();
        reportError(hwnd, "AllocateAndInitializeSid", OS_ERR_TITLE, error);
        goto done_out;
    }

    // Determine whether the SID of administrators group is enabled in
    // the primary access token of the process.
    BOOL isAdmin;
    if ( ! CheckTokenMembership(NULL, adminGroup, &isAdmin) )
    {
        error = GetLastError();
        reportError(hwnd, "CheckTokenMembership", OS_ERR_TITLE, error);
        goto done_out;
    }

done_out:
    if ( adminGroup )
    {
        FreeSid(adminGroup);
    }
    *rc = error;

    return isAdmin ? true : false;
}


uint32_t determineStatus(HWND hDlg, pConfigureArguments pca)
{
    OSVERSIONINFO osver = { sizeof(osver) };
    uint32_t      rc    = ERROR_SUCCESS;

    pca->requiresElevation = true;

    if ( ! GetVersionEx(&osver) )
    {
        rc = GetLastError();
        reportError(hDlg, "GetVersionEx", OS_ERR_TITLE, rc);
        goto done_out;
    }

    if (osver.dwMajorVersion >= 6)
    {
        pca->isVistaOrLater = true;
    }

    pca->isInAdminGroup = isUserInAdminGroup(hDlg, pca->isVistaOrLater, &rc);
    if ( rc != ERROR_SUCCESS )
    {
        goto done_out;
    }

    pca->isRunAsAdmin = runningAsAdmin(hDlg, &rc);
    if ( rc != ERROR_SUCCESS )
    {
        goto done_out;
    }

    if ( pca->isVistaOrLater )
    {
        pca->isElevated = isProcessElevated(hDlg, &rc);
        if ( rc != ERROR_SUCCESS )
        {
            goto done_out;
        }

        pca->integrityLevel = getProcessIntegrityLevel(hDlg, &rc);
    }

    pca->requiresElevation = requiresElevation(pca);

done_out:
    return rc;
}

/**
 * Creates a mono spaced font for the size specified.
 *
 * @param fontSize
 *
 * @return HFONT
 */
HFONT createMonoSpacedFont(int32_t fontSize)
{
    int   width = 0;                              // average character width
    int   escapement = 0;                         // angle of escapement
    int   orientation = 0;                        // base-line orientation angle
    int   weight = FW_NORMAL;                     // font weight
    BOOL  italic = FALSE;                         // italic attribute option
    BOOL  underline = FALSE;                      // underline attribute option
    BOOL  strikeOut = FALSE;                      // strikeout attribute option
    uint32_t charSet = DEFAULT_CHARSET;           // character set identifier
    uint32_t outputPrecision = OUT_TT_PRECIS;     // output precision
    uint32_t clipPrecision = CLIP_DEFAULT_PRECIS; // clipping precision
    uint32_t quality = DEFAULT_QUALITY;           // output quality
    uint32_t pitchAndFamily = FF_DONTCARE;        // pitch and family

    HDC hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
    int height = -MulDiv(fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    DeleteDC(hdc);

    HFONT f = CreateFont(height, width, escapement, orientation, weight, italic, underline, strikeOut,
                         charSet, outputPrecision, clipPrecision, quality, pitchAndFamily, "Courier New");
    return f;
}


/**
 * The dialog procedure for the 'main' dialog.  This is the dialog that displays
 * if the ooDialog.exe executable is run with no arguments, or with the -H
 * option flag.
 *
 * @param hDlg
 * @param uMsg
 * @param wParam
 * @param lParam
 *
 * @return INT_PTR CALLBACK
 */
INT_PTR CALLBACK WinMainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HFONT font = NULL;

    if ( uMsg == WM_INITDIALOG )
    {
        pProgramArguments pa = (pProgramArguments)lParam;

        setDlgIcon(hDlg, pa->hInstance);

        // Initialize the edit control that displays the long text.
        font = createMonoSpacedFont(10);

        HWND hEdit  = GetDlgItem(hDlg, IDC_EDIT);

        SendMessage(hEdit, WM_SETFONT, (WPARAM)font, TRUE);
        SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)pa->mainMsg);

        // Initialize the short banner text and the dialog title.
        char tmp[MEDIUM_BUF_SIZE];
        HWND hMsg = GetDlgItem(hDlg, IDC_ST_SHORT_MSG);

        switch ( pa->dlgType )
        {
            case conditionDisplay :
                SetWindowText(hDlg, "ooDialog Execute Program: Condition Raised");
                _snprintf(tmp, MEDIUM_BUF_SIZE, "A Condition was raised while executing:\n\t%s", pa->oodProgram);
                break;

            case versionDisplay :
                SetWindowText(hDlg, "ooDialog Execute Program: Version Information");
                _snprintf(tmp, MEDIUM_BUF_SIZE, "ooDialog version %u.%u.%u.%u", OOD_VER, OOD_REL, OOD_MOD, OOD_BLD);
                break;

            case syntaxDisplay :
                SetWindowText(hDlg, "ooDialog Execute Program: Syntax Information");
                _snprintf(tmp, MEDIUM_BUF_SIZE, "ooDialog version %u.%u.%u.%u", OOD_VER, OOD_REL, OOD_MOD, OOD_BLD);
                break;

            default :
                SetWindowText(hDlg, "ooDialog Execute Program: Internal Error");
                _snprintf(tmp, MEDIUM_BUF_SIZE, "An unexpected internal error has happend:\n");
                break;
        }
        SendMessage(hMsg, WM_SETTEXT, 0, (LPARAM)tmp);

        // Set the proper icon image.
        LPCTSTR imageName = IDI_INFORMATION;
        if ( pa->dlgType == conditionDisplay )
        {
            imageName = IDI_ERROR;
        }
        HANDLE hNewIcon = LoadImage(NULL, imageName, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE);
        HWND   hSt      = GetDlgItem(hDlg, IDC_ST_DISPLAY_ICON);
        SendMessage(hSt, STM_SETICON, (WPARAM)hNewIcon, 0);

        if ( pa->allowServices )
        {
            ShowWindow(GetDlgItem(hDlg, IDC_GB_SERVICES), SW_SHOWNA);
            ShowWindow(GetDlgItem(hDlg, IDC_CK_FILEASSOC), SW_SHOWNA);
        }

        setWindowPtr(hDlg, GWLP_USERDATA, (LONG_PTR)pa);

        return TRUE;
    }

    switch ( uMsg )
    {
        case WM_COMMAND:
            switch ( LOWORD(wParam) )
            {
                case IDC_CK_FILEASSOC :
                {
                    if ( HIWORD(wParam) == BN_CLICKED )
                    {
                        if ( IsDlgButtonChecked(hDlg, IDC_CK_FILEASSOC) == BST_CHECKED )
                        {
                            SetDlgItemText(hDlg, IDOK, "Continue");
                        }
                        else
                        {
                            SetDlgItemText(hDlg, IDOK, "Close");
                        }
                    }
                }
                    break;

                case IDOK:
                {
                    if ( IsDlgButtonChecked(hDlg, IDC_CK_FILEASSOC) == BST_CHECKED )
                    {
                        pProgramArguments pa = (pProgramArguments)getWindowPtr(hDlg, GWLP_USERDATA);

                        pa->doConfigure = true;
                    }

                    EndDialog(hDlg, wParam);

                    if ( font )
                    {
                        DeleteObject(font);
                    }
                }
                return TRUE;

                case IDCANCEL:
                {
                    EndDialog(hDlg, wParam);

                    if ( font )
                    {
                        DeleteObject(font);
                    }
                }
                    return TRUE;
            }

        default:
            break;
    }

    return FALSE;
}

/**
 * Generates the 'display name' in the extension record to match what the
 * variables in the record indicate.  That is, if the variables in the record
 * indicate that the .ext is registered for all users, the display name is marde
 * accordingly.
 *
 * @param rec
 *
 * @note  We use this scheme:  The display name has 3 columns.  The first
 *        column indicates the registered status for all users, the second
 *        column indicates the registered status for the current user, and the
 *        third column is the .ext name.
 *
 *        A '-' character indicates the .ext is not registered.  A '*' character
 *        indicates the .ext is registered.  A '+' character indicates the .ext
 *        is registered, but not registered to the ooDialog prog ID.
 */
static void updateExtDisplayName(pExtensionInfo rec)
{
    char *prefix = "- -  ";
    if ( rec->allUsers )
    {
        if ( rec->curUser )
        {
            prefix = "* *  ";
        }
        else if ( rec->curUserOther )
        {
            prefix = "* +  ";
        }
        else
        {
            prefix = "* -  ";
        }
    }
    else if ( rec->curUser )
    {
        if ( rec->allUsersOther )
        {
            prefix = "+ *  ";
        }
        else
        {
            prefix = "- *  ";
        }
    }
    else
    {
        // Not associated with our program ID, but may be in use by others.
        if ( rec->allUsersOther )
        {
            if ( rec->curUserOther )
            {
                prefix = "+ +  ";
            }
            else
            {
                prefix = "+ -  ";
            }
        }
        else if ( rec->curUser )
        {
            prefix = "- +  ";
        }
    }

    _snprintf(rec->displayName, MAX_EXT_DISPLAY, "%s%s", prefix, rec->extension);
}

/**
 * Given an extension information record for the specified ext, determines where
 * (in which locations) the extension is associated with the specified progID,
 * and or if it is associated with a different progID.
 *
 * Essentially this fills out the fields of the record for an extension that
 * show where, or if, that extension is registered to a file type.
 *
 * @param keyName  The extension, .ood, for example
 * @param rec      An allocated extension information record
 * @param progID   The progID we are interested in.  Actually this should always
 *                 be OODIALOG_PROGID_VERSIONED, but for testing and flexibility
 *                 we use an input argument.
 *
 */
void qualifyExtensionInfo(char *extKeyName, pExtensionInfo rec, char *progID)
{
    char     value[MAX_HKEY_VALUE] = {'\0'};
    uint32_t maxValue              = MAX_HKEY_VALUE;
    HKEY     hKey, hSubkey;

    // See if extKeyName is a subkey for current user
    if( RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Classes", 0, KEY_READ, &hKey) == ERROR_SUCCESS )
    {
        if ( RegOpenKeyEx(hKey, extKeyName, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
        {
            if ( RegQueryValueEx(hSubkey, "", NULL, NULL, (LPBYTE)value, (LPDWORD)&maxValue) == ERROR_SUCCESS )
            {
                if ( StrCmpI(value, progID) == 0 )
                {
                    rec->curUser = true;
                }
                else
                {
                    rec->curUserOther = true;
                }
            }

            RegCloseKey(hSubkey);
        }
        RegCloseKey(hKey);
    }

    // See if extKeyName is a subkey for local machine classes
    if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes", 0, KEY_READ, &hKey) == ERROR_SUCCESS )
    {
        if ( RegOpenKeyEx(hKey, extKeyName, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
        {
            value[0] = '\0';
            maxValue = MAX_HKEY_VALUE;

            if ( RegQueryValueEx(hSubkey, "", NULL, NULL, (LPBYTE)value, (LPDWORD)&maxValue) == ERROR_SUCCESS )
            {
                if ( StrCmpI(value, progID) == 0 )
                {
                    rec->allUsers = true;
                }
                else
                {
                    rec->allUsersOther = true;
                }
            }
            RegCloseKey(hSubkey);
        }
        RegCloseKey(hKey);
    }

    updateExtDisplayName(rec);
}

/**
 * Searches the specified list box for an extension record that matches the
 * extension specified.  Returns NULL if no recored is found.
 *
 * @param hLB
 * @param extension
 * @param index  [out]
 *
 * @return pExtensionInfo
 */
pExtensionInfo getExtRec(HWND hLB, CSTRING extension, uint32_t *index)
{
    LRESULT count = SendMessage(hLB, LB_GETCOUNT, NULL, NULL);
    if ( count > 0 )
    {
        for (LRESULT i = 0; i < count; i++ )
        {
            pExtensionInfo info = (pExtensionInfo)SendMessage(hLB, LB_GETITEMDATA, i, 0);
            if ( StrCmpI(info->extension, extension) == 0 )
            {
                *index = (uint32_t)i;
                return info;
            }
        }
    }
    return NULL;
}

/**
 * Checks an extension record to see if the extension is registered in the
 * current scope.  That is, if the current scope is all users, is the extension
 * registered for all users?
 *
 * @return bool
 */
inline bool extRegisteredInScope(pAssocArguments paa, pExtensionInfo existing)
{
    if ( paa->allUsers )
    {
        return existing->allUsers;
    }
    else
    {
        return existing->curUser;
    }
}

/**
 * Updates the extension record using the assumption that it is registered in
 * the current scope.
 *
 * @param paa
 * @param existing
 *
 * @note  If the record has the 'other' set to true, we change it to false
 *        because the extension was just updated to point to our prog ID.  We
 *        don't actuall need to check what it was, we just do it.
 */
void nowRegisteredInScope(pAssocArguments paa, pExtensionInfo existing)
{
    if ( paa->allUsers )
    {
        existing->allUsers      = true;
        existing->allUsersOther = false;
    }
    else
    {
        existing->curUser       = true;
        existing->curUserOther  = false;
    }
    updateExtDisplayName(existing);
}

/**
 * Gets the extension information records for all extensions mapped to the
 * specified progID currently in the registry.
 *
 * @param progID  The progID we are looking for IN
 * @param count   The count of records found    OUT
 *
 * @return An allocated array of extension records.
 *
 * @note  The caller is responsible for freeing the array.
 */
pExtensionInfo getExtensionRecords(HWND hDlg, char *progID, size_t *count)
{
    uint32_t cSubKeys = 0;
    HKEY     hKCR     = HKEY_CLASSES_ROOT;

    // We only need the number of subkeys.
    uint32_t retCode = RegQueryInfoKey(HKEY_CLASSES_ROOT, NULL, NULL, NULL, (LPDWORD)&cSubKeys,
                                       NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    uint32_t maxName;                  // Size of name buffer
    char     keyName[MAX_HKEY_NAME];   // Buffer for subkey name

    uint32_t maxValue = MAX_HKEY_VALUE; // Size of value buffer
    char     value[MAX_HKEY_VALUE];     // Buffer for default value for subkey

    // Enumerate the subkeys, and see if the prog ID matches our ooDialog prog
    // ID.  We go through the loop twice, once to count the number of matches so
    // we can allocate the array of extension records.  Then a second time to
    // actually fill in the records.

    pExtensionInfo recs    = NULL;
    size_t         matches = 0;

    for ( uint32_t i = 0; i < cSubKeys; i++ )
    {
        maxName = MAX_HKEY_NAME;
        retCode = RegEnumKeyEx(hKCR, i, keyName, (LPDWORD)&maxName, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            if ( *keyName == '.' )
            {
                HKEY hExtKey;

                retCode = RegOpenKeyEx(hKCR, keyName, 0, KEY_QUERY_VALUE, &hExtKey);
                if ( retCode == ERROR_SUCCESS )
                {
                    maxValue = MAX_HKEY_VALUE;
                    value[0] = '\0';

                    retCode = RegQueryValueEx(hExtKey, "", NULL, NULL, (LPBYTE)value, (LPDWORD)&maxValue);
                    if ( retCode == ERROR_SUCCESS )
                    {
                        if ( StrCmpI(value, progID) == 0 )
                        {
                            matches++;
                        }
                    }
                    RegCloseKey(hExtKey);
                }
            }
        }
    }

    if ( matches == 0 )
    {
        goto done_out;
    }

    recs = (pExtensionInfo)LocalAlloc(LPTR, matches * sizeof(extensionInfo));
    if ( recs == NULL )
    {
        reportError(hDlg, OUT_OF_MEMORY_ERR_FMT, OS_ERR_TITLE, "LocalAlloc", GetLastError());
        goto done_out;
    }

    size_t processed = 0;
    for ( uint32_t i = 0; i < cSubKeys; i++ )
    {
        maxName = MAX_HKEY_NAME;
        retCode = RegEnumKeyEx(hKCR, i, keyName, (LPDWORD)&maxName, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            if ( *keyName == '.' )
            {
                HKEY hExtKey;

                retCode = RegOpenKeyEx(hKCR, keyName, 0, KEY_QUERY_VALUE, &hExtKey);
                if ( retCode == ERROR_SUCCESS )
                {
                    maxValue = MAX_HKEY_VALUE;
                    value[0] = '\0';

                    retCode = RegQueryValueEx(hExtKey, "", NULL, NULL, (LPBYTE)value, (LPDWORD)&maxValue);
                    if ( retCode == ERROR_SUCCESS )
                    {
                        if ( StrCmpI(value, progID) == 0 )
                        {
                            pExtensionInfo current = &recs[processed++];

                            current->exists = true;
                            strcpy(current->extension, keyName);

                            qualifyExtensionInfo(keyName, current, progID);

                            printf("%s -> %s\n", keyName, value);
                        }
                    }
                    RegCloseKey(hExtKey);
                }
            }
        }
    }

done_out:
    *count = matches;
    return recs;
}

/**
 * Determines if our ooDialog file type is registered, and if so if it is
 * registered for all users and if it is registered for the current user.
 *
 * @param paa
 */
void checkRegistration(pAssocArguments paa)
{
    char     value[MAX_HKEY_VALUE] = {'\0'};
    uint32_t maxValue              = MAX_HKEY_VALUE;
    HKEY     hKey, hSubkey;

    paa->ftypeIsRegistered  = false;
    paa->registeredAllUsers = false;
    paa->registeredCurUsers = false;

    // See if our progID is a subkey of classes root
    if( RegOpenKeyEx(HKEY_CLASSES_ROOT, paa->progID, 0, KEY_QUERY_VALUE , &hKey) == ERROR_SUCCESS )
    {
        paa->ftypeIsRegistered = true;

        char buf[MEDIUM_BUF_SIZE];
        _snprintf(buf, MEDIUM_BUF_SIZE, "SOFTWARE\\Classes\\%s", paa->progID);

        // See if progID is a subkey for local machine
        if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, buf, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
        {
            paa->registeredAllUsers = true;
            RegCloseKey(hSubkey);
        }

        // See if progID is a subkey for current user
        if( RegOpenKeyEx(HKEY_CURRENT_USER, buf, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
        {
            paa->registeredCurUsers = true;
            RegCloseKey(hSubkey);
        }

        RegCloseKey(hKey);
    }
}

/**
 * Removes all items from the specified list box, if there are any items.
 *
 * Each item in any of the list boxes has an extension record struct set as the
 * item data for the item.  These records need to be freed before the list box
 * is emptied.
 *
 * @param hLB
 */
void maybeEmptyLB(HWND hLB)
{
    LRESULT count = SendMessage(hLB, LB_GETCOUNT, NULL, NULL);
    if ( count > 0 )
    {
        for (LRESULT i = 0; i > count; i++ )
        {
            pExtensionInfo info = (pExtensionInfo)SendMessage(hLB, LB_GETITEMDATA, i, 0);
            safeLocalFree(info);
        }
        SendMessage(hLB, LB_RESETCONTENT, 0, 0);
    }
}

/**
 * Not used at this time, maybe, probably, not needed
 *
 * @param info
 *
 * @return bool
 */
static bool isSuggestedExt(pExtensionInfo info)
{
    for ( size_t i = 0; i < OOD_SUGGESTED_EXT_COUNT; i++ )
    {
        if ( StrCmpI(oodSuggestedExts[i], info->extension) == 0 )
        {
            // We change the case to match the suggested case. NOTE may need to make this conditional.
            memset(info->extension, 0, sizeof(info->extension));
            strcpy(info->extension, oodSuggestedExts[i]);
            return true;
        }
    }
    return false;
}

/**
 * Adds the suggested file extensions to the list box.  The first time through,
 * the list box will be empty.  But, we may do this several times if the user
 * adds and / or removes the registration for the file type.
 *
 * If there are any items in the list box, we need to not only remove them, but
 * to also free the extension memory.
 *
 * @param hDlg
 * @param paa
 *
 * @return bool
 */
static bool addSuggestedExt(HWND hDlg, pAssocArguments paa)
{
    HWND hLB = paa->lbSuggested;

    maybeEmptyLB(hLB);

    for ( size_t i = 0; i < OOD_SUGGESTED_EXT_COUNT; i++ )
    {
        pExtensionInfo info = (pExtensionInfo)LocalAlloc(LPTR, sizeof(extensionInfo));

        if ( info == NULL )
        {
            reportError(hDlg, OUT_OF_MEMORY_ERR_FMT, OS_ERR_TITLE, "LocalAlloc", GetLastError());
            return false;
        }

        strcpy(info->extension, oodSuggestedExts[i]);
        info->suggested = true;

        qualifyExtensionInfo(oodSuggestedExts[i], info, paa->progID);

        LRESULT index = SendMessage(hLB, LB_ADDSTRING, i, (LPARAM)info->displayName);
        SendMessage(hLB, LB_SETITEMDATA, index, (LPARAM)info);
    }
    return true;
}

/**
 * Fills the current extensions list box.
 *
 * This should always be called after the suggested extensions list box is
 * filled, because if a currently registered extension matches one of the
 * suggested extensions we remove the suggested extension from its list box and
 * use its record.
 *
 * @param hDlg
 * @param paa
 */
void addCurrentExt(HWND hDlg, pAssocArguments paa)
{
    size_t count = 0;
    HWND   hLB   = paa->lbCurrent;

    maybeEmptyLB(hLB);

    pExtensionInfo recs = getExtensionRecords(hDlg, paa->progID, &count);

    for ( size_t i = 0; i < count; i++ )
    {
        uint32_t idx;
        LRESULT  index;

        pExtensionInfo current  = &recs[i];
        pExtensionInfo existing = getExtRec(paa->lbSuggested, current->extension, &idx);

        if ( existing != NULL )
        {
            SendMessage(paa->lbSuggested, LB_DELETESTRING, idx, 0);
            LocalFree(current);
            current = existing;
        }

        index = SendMessage(hLB, LB_ADDSTRING, 0, (LPARAM)current->displayName);
        SendMessage(hLB, LB_SETITEMDATA, index, (LPARAM)current);
    }
}

void testRegistry()
{
    HKEY envKey;
    char msg[MEDIUM_BUF_SIZE];
    char buf[MEDIUM_BUF_SIZE];

    char *key = "Console\\Left";
    HKEY hKey;
    uint32_t rc = RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, STANDARD_RIGHTS_READ | KEY_QUERY_VALUE, &hKey);
    if ( rc == ERROR_SUCCESS )
    {
        uint32_t type;
        uint32_t count = MEDIUM_BUF_SIZE;

        rc = RegQueryValueEx(hKey, "FaceName", NULL, (LPDWORD)&type, (LPBYTE)buf, (LPDWORD)&count);
        if ( rc == ERROR_SUCCESS )
        {
            _snprintf(msg, MEDIUM_BUF_SIZE, "Queried key=%s for FaceName value=%s, type=0x%x count=%d\n", key, buf, type, count);
            internalInfoMsgBox(msg, "ooDialog");
        }
        else
        {
            _snprintf(msg, MEDIUM_BUF_SIZE, "RegQueryValueEx() for default rc=%d\n", rc);
            internalInfoMsgBox(msg, "ooDialog");
        }
        RegCloseKey(hKey);
    }
    else
    {
        _snprintf(msg, MEDIUM_BUF_SIZE, "RegOpenKeyEx() for %s failed rc=%d\n", key, rc);
        internalInfoMsgBox(msg, "ooDialog");
    }


    rc = RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, STANDARD_RIGHTS_READ | KEY_QUERY_VALUE, &envKey);
    if ( rc == ERROR_SUCCESS )
    {
        uint32_t type;
        uint32_t count = MEDIUM_BUF_SIZE;

        rc = RegQueryValueEx(envKey, "PATHEXT", NULL, (LPDWORD)&type, (LPBYTE)buf, (LPDWORD)&count);
        if ( rc == ERROR_SUCCESS )
        {
            _snprintf(msg, MEDIUM_BUF_SIZE, "PATHEXT=%s, type=0x%x count=%d\n", buf, type, count);
            internalInfoMsgBox(msg, "ooDialog");
        }
        else
        {
            _snprintf(msg, MEDIUM_BUF_SIZE, "RegQueryValueEx() failed rc=%d\n", rc);
            internalInfoMsgBox(msg, "ooDialog");
        }
        RegCloseKey(envKey);
    }
    else
    {
        _snprintf(msg, MEDIUM_BUF_SIZE, "RegOpenKeyEx() failed rc=%d\n", rc);
        internalInfoMsgBox(msg, "ooDialog");
    }


}

void setAssocControls(HWND hDlg, pAssocArguments paa, bool flag)
{
    BOOL enable = flag ? TRUE : FALSE;

    EnableWindow(paa->lbCurrent, enable);
    EnableWindow(paa->lbSuggested, enable);
    EnableWindow(paa->lbPathExt, enable);

    EnableWindow(GetDlgItem(hDlg, IDC_EDIT_EXTENSION), enable);
    EnableWindow(GetDlgItem(hDlg, IDC_PB_ADD_EXTENSION), enable);
    EnableWindow(GetDlgItem(hDlg, IDC_PB_ADD_CURRENT), enable);
    EnableWindow(GetDlgItem(hDlg, IDC_PB_REMOVE_CURRENT), enable);
    EnableWindow(GetDlgItem(hDlg, IDC_PB_ADD_PATHEXT), enable);
    EnableWindow(GetDlgItem(hDlg, IDC_PB_REMOVE_PATHEXT), enable);
}


/**
 * Sets the state of the dialog controls.
 *
 * @param hDlg
 * @param paa
 *
 * @note This is a little complex.  At this time I think the Register push
 *       button should always be enabled.
 */
void setRegisterdState(HWND hDlg, pAssocArguments paa)
{
    // The static text for these controls can vary during the life-time of the
    // dialog:
    SetDlgItemText(hDlg, IDC_ST_REGISTERED, paa->ftypeIsRegistered ? "True" : "False");
    SetDlgItemText(hDlg, IDC_ST_REGALL, paa->registeredAllUsers ? "Yes" : "No");
    SetDlgItemText(hDlg, IDC_ST_REGCURRENT, paa->registeredCurUsers ? "Yes" : "No");

    if ( ! paa->ftypeIsRegistered )
    {
        if ( paa->allUsers )
        {
            SetWindowText(paa->pbRegister, "Register ooDialog File Type For All Users");
        }
        else
        {
            SetWindowText(paa->pbRegister, "Register ooDialog File Type For the Current User");
        }

        setAssocControls(hDlg, paa, false);

        addSuggestedExt(hDlg, paa);
        addCurrentExt(hDlg, paa);
        return;
    }

    // Our progID is registered for either all users or current user.  Maybe
    // both, but not necessarily both.  How we set the state is dependent on the
    // scope and if progID is registered for that scope.
    if ( paa->registeredAllUsers && paa->registeredCurUsers )
    {
        if ( paa->allUsers )
        {
            SetWindowText(paa->pbRegister, "Un-register ooDialog File Type For All Users");
        }
        else
        {
            SetWindowText(paa->pbRegister, "Un-register ooDialog File Type For the Current User");
        }
    }
    else if ( paa->registeredAllUsers )
    {
        if ( paa->allUsers )
        {
            SetWindowText(paa->pbRegister, "Un-register ooDialog File Type For All Users");
        }
        else
        {
            SetWindowText(paa->pbRegister, "Register ooDialog File Type For the Current User");
        }
    }
    else
    {
        if ( paa->allUsers )
        {
            SetWindowText(paa->pbRegister, "Register ooDialog File Type For All Users");
        }
        else
        {
            SetWindowText(paa->pbRegister, "Un-register ooDialog File Type For the Current User");
        }
    }

    setAssocControls(hDlg, paa, true);
    addSuggestedExt(hDlg, paa);
    addCurrentExt(hDlg, paa);
}

void setStaticText(HWND hDlg, pAssocArguments paa)
{
    SetDlgItemText(hDlg, IDC_GB_ASSOCIATE, paa->allUsers ?
                   "Associating File Extensions with ooDialog.exe File Type for All Users" :
                   "Associating File Extensions with ooDialog.exe File Type for the Current User");

    SetDlgItemText(hDlg, IDC_ST_FTYPE, paa->friendlyName);
    SetDlgItemText(hDlg, IDC_ST_SCOPE, paa->allUsers ? "All Users" : "CurrentUser");
    SetDlgItemText(hDlg, IDC_ST_RUNAS, paa->isRunAsAdmin ? "True" : "False");
    SetDlgItemText(hDlg, IDC_ST_ELEVATED, paa->isElevated ? "True" : "False");
}

void setUp(HWND hDlg, pAssocArguments paa)
{
    setWindowPtr(hDlg, GWLP_USERDATA, (LONG_PTR)paa);

    paa->lbSuggested = GetDlgItem(hDlg, IDC_LB_SUGGESTED);
    paa->lbCurrent   = GetDlgItem(hDlg, IDC_LB_CURRENT);
    paa->lbPathExt   = GetDlgItem(hDlg, IDC_LB_PATHEXT);
    paa->pbRegister  = GetDlgItem(hDlg, IDC_PB_REGISTER);
    paa->edit        = GetDlgItem(hDlg, IDC_EDIT_EXTENSION);

    LoadString(paa->hInstance, IDS_FRIENDLY_NAME, paa->friendlyName, MAX_FRIENDLY_NAME);

    HFONT font = createMonoSpacedFont(8);

    SendMessage(paa->lbSuggested, WM_SETFONT, (WPARAM)font, FALSE);
    SendMessage(paa->lbCurrent, WM_SETFONT, (WPARAM)font, FALSE);
    SendMessage(paa->lbPathExt, WM_SETFONT, (WPARAM)font, FALSE);

    // Leave room for both the ending NULL and to add a '.'
    SendMessage(paa->edit, EM_SETLIMITTEXT, MAX_EXT_NAME - 2, 0);

    setDlgIcon(hDlg, paa->hInstance);
    setStaticText(hDlg, paa);

    setRegisterdState(hDlg, paa);
}

/**
 * Determines and returns the predefined registry key and its descriptive string
 * that matches the scope we are using.  Local machine or  current user.
 *
 * @param paa
 * @param user  Descriptive string [out]
 *
 * @return HKEY
 */
HKEY getScopeVars(pAssocArguments paa, char **user)
{
    HKEY  hPreDefKey = HKEY_CURRENT_USER;
    char *u          = "the current user.";

    if ( paa->allUsers )
    {
        hPreDefKey = HKEY_LOCAL_MACHINE;
        u          = "all users";
    }
    *user = u;
    return hPreDefKey;
}


/**
 *
 *
 * @author Administrator (7/14/2013)
 *
 * @param hDlg
 * @param paa
 *
 * @return bool
 */
bool deleteRegProgID(HWND hDlg, pAssocArguments paa)
{
    char *user;
    HKEY hPreDefKey = getScopeVars(paa, & user);

    char valueBuf[MEDIUM_BUF_SIZE];
    _snprintf(valueBuf, MEDIUM_BUF_SIZE, "SOFTWARE\\Classes\\%s", paa->progID);

    uint32_t rc = SHDeleteKey(hPreDefKey, valueBuf);
    if ( rc != ERROR_SUCCESS )
    {
        char extraMsg[SMALL_BUF_SIZE];

        _snprintf(extraMsg, SMALL_BUF_SIZE, "Failed to delete the %s key for %s", valueBuf, user);

        reportErrorPlus(hDlg, "SHDeleteKey", REG_ERR_TITLE, extraMsg, rc);
        return false;
    }
    return true;
}


/**
 * Deletes all extension (.xxx) subkeys for our ooDialog file type (prog ID)
 * within the scope (all users / current user) we are under.
 *
 * @param hDlg
 * @param paa
 *
 * @return bool
 *
 * @note  When querying a subkey value, I have had a problem with file not found
 *        error.  This seems to be when the default value for the subkey is not
 *        set.  So, I ignore that specific error.  In getExtensionRecords() we
 *        just ignore all errors and that might be what we should do here.
 *        After all, if we can't query the default value for an extension
 *        subkey, it was probably not set by use.
 */
static bool deleteRegAllExt(HWND hDlg, pAssocArguments paa)
{
    char     *user;
    HKEY      hPreDefKey = getScopeVars(paa, & user);
    uint32_t  rc = 0;
    HKEY      hClassesKey;
    char      extraMsg[SMALL_BUF_SIZE];

    rc = RegOpenKeyEx(hPreDefKey, "SOFTWARE\\Classes\\", 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &hClassesKey);
    if ( rc != ERROR_SUCCESS )
    {
        _snprintf(extraMsg, SMALL_BUF_SIZE, "Failed to open the %s registry key for %s", "SOFTWARE\\Classes\\", user);

        reportErrorPlus(hDlg, "RegOpenKeyEx", REG_ERR_TITLE, extraMsg, rc);
        return false;
    }

    bool success = true;
    HWND hLB     = paa->lbCurrent;
    char buf[512];

    LRESULT count = SendMessage(hLB, LB_GETCOUNT, NULL, NULL);
   // sprintf(buf, "Found %d items in lbCurrent", count);
   // internalErrorMsgBox(hDlg, buf, "Going to delete");
    if ( count > 0 )
    {
        for ( LRESULT i = 0; i < count; i++ )
        {
            pExtensionInfo info = (pExtensionInfo)SendMessage(hLB, LB_GETITEMDATA, i, 0);
           // internalErrorMsgBox(hDlg, info->extension, "Going to delete");
            if ( extRegisteredInScope(paa, info) )
            {
                rc = RegDeleteKey(hClassesKey, info->extension);
                if ( rc != ERROR_SUCCESS )
                {
                    _snprintf(extraMsg, SMALL_BUF_SIZE,
                              "Failed to delete the SOFTWARE\\Classes\\%s registry subkey for %s",
                              info->extension, user);

                    reportErrorPlus(hDlg, "RegDeleteKey", REG_ERR_TITLE, extraMsg, rc);
                    success = false;
                }
            }
        }
    }

    RegCloseKey(hClassesKey);
    return success;

    /* old code
    // We only need the number of subkeys.
    uint32_t cSubkeys;
    rc = RegQueryInfoKey(HKEY_CLASSES_ROOT, NULL, NULL, NULL, (LPDWORD)&cSubkeys,
                         NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    uint32_t maxName;                  // Size of name buffer
    char     keyName[MAX_HKEY_NAME];   // Buffer for subkey name

    uint32_t maxValue = MAX_HKEY_VALUE; // Size of value buffer
    char     value[MAX_HKEY_VALUE];     // Buffer for default value for subkey
    bool     success = true;
    uint32_t i       = 0;
    CSTRING  progID  = paa->progID;

    for ( i = 0; i < cSubkeys; i++ )
    {
        maxName = MAX_HKEY_NAME;
        rc = RegEnumKeyEx(hClassesKey, i, keyName, (LPDWORD)&maxName, NULL, NULL, NULL, NULL);
        if ( rc == ERROR_SUCCESS )
        {
            if ( *keyName == '.' )
            {
                HKEY hExtKey;
                if ( StrStrI(keyName, "rxd") != NULL )
                {
                    internalErrorMsgBox(hDlg, keyName, "Found .rxd");
                }
                rc = RegOpenKeyEx(hClassesKey, keyName, 0, DELETE | KEY_QUERY_VALUE, &hExtKey);
                if ( rc == ERROR_SUCCESS )
                {
                    maxValue = MAX_HKEY_VALUE;
                    value[0] = '\0';

                    rc = RegQueryValueEx(hExtKey, "", NULL, NULL, (LPBYTE)value, (LPDWORD)&maxValue);
                    RegCloseKey(hExtKey);

                    if ( rc == ERROR_SUCCESS )
                    {
                        if ( StrCmpI(value, progID) == 0 )
                        {
                            rc = RegDeleteKey(hClassesKey, keyName);
                            if ( rc != ERROR_SUCCESS )
                            {
                                _snprintf(extraMsg, SMALL_BUF_SIZE,
                                          "Failed to delete the SOFTWARE\\Classes\\%s registry subkey for %s",
                                          keyName, user);

                                reportErrorPlus(hDlg, "RegDeleteKey", REG_ERR_TITLE, extraMsg, rc);
                                success = false;
                            }
                        }
                    }
                    else if ( rc != ERROR_FILE_NOT_FOUND )
                    {
                        _snprintf(extraMsg, SMALL_BUF_SIZE, "Failed to query the SOFTWARE\\Classes\\%s registry subkey for %s",
                                  keyName, user);

                        reportErrorPlus(hDlg, "RegQueryValueEx", REG_ERR_TITLE, extraMsg, rc);
                        success = false;
                    }
                }
                else
                {
                    _snprintf(extraMsg, SMALL_BUF_SIZE, "Failed to open the SOFTWARE\\Classes\\%s registry subkey for %s",
                              keyName, user);

                    reportErrorPlus(hDlg, "RegOpenKeyEx", REG_ERR_TITLE, extraMsg, rc);
                    success = false;
                }
            }
        }
        else if ( rc != ERROR_NO_MORE_ITEMS )
        {
            _snprintf(extraMsg, SMALL_BUF_SIZE, "Failed to enumerate the %s registry subkey for %s", "SOFTWARE\\Classes\\", user);

            reportErrorPlus(hDlg, "RegEnumKeyEx", REG_ERR_TITLE, extraMsg, rc);
            success = false;
        }
    }

    RegCloseKey(hClassesKey);
    return success;
    */
}

/**
 * Creates, or opens, a subkey and, optionally, writes its Default string value.
 *
 * @param openedKey  An already open registry key
 * @param subkey     A subkey of 'openedKey to create or open
 * @param defValue   The value for the Default entry for subkey.  If this is
 *                   null, then writing a value for Default is just skipped.
 *
 * @param hDlg       The window handle of the dialog, for error displary
 * @param user       A string for the error dispaly that specifies if we are
 *                   writing the subkey in current user or all users (local
 *                   machine.)
 *
 * @return The opened subkey on success, otherwise NULL.
 *
 * @note  There may be one problem here, if we create the key, but fail to set
 *        the default value, we don't clean up the created key.  TODO  We could
 *        check the disposition and if was created, we could delete the key if
 *        we fail to set the default value.
 */
HKEY writeRegSubkeyValue(HKEY openedKey, char *subkey, char *defValue, HWND hDlg, char *user)
{
    uint32_t disp;
    uint32_t rc;
    char     extraMsg[SMALL_BUF_SIZE];
    HKEY     hKey = NULL;

    rc = RegCreateKeyEx(openedKey, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | DELETE, NULL, &hKey, (LPDWORD)&disp);
    if ( rc != ERROR_SUCCESS )
    {
        _snprintf(extraMsg, SMALL_BUF_SIZE, "Failed to create or open the %s registry key for %s", subkey, user);

        reportErrorPlus(hDlg, "RegCreateKeyEx", REG_ERR_TITLE, extraMsg, rc);
        return NULL;
    }

    // Set the default value, if requested ...
    if ( defValue != NULL )
    {
        size_t  len  = strlen(defValue) + 1;

        rc = RegSetValueEx(hKey, "", 0, REG_SZ, (LPBYTE)defValue, (uint32_t)len);
        if ( rc != ERROR_SUCCESS )
        {
            _snprintf(extraMsg, SMALL_BUF_SIZE,
                      "Failed to set the default value (%s) for the %s registry key for %s",
                      defValue, subkey, user);

            reportErrorPlus(hDlg, "RegSetValueEx", REG_ERR_TITLE, extraMsg, rc);

            RegCloseKey(hKey);
            hKey = NULL;
        }
    }
    return hKey;
}

/**
 * Writes a named string value for an opened registry key.
 *
 * @param openedKey  An already open registry key
 * @param valueName  The name of the value to be written
 * @param value      The value to be written
 * @param hDlg       The window handle of the dialog, for error displary
 * @param user       A string for the error dispaly that specifies if we are
 *                   writing the subkey in current user or all users (local
 *                   machine.)
 *
 * @return True on success, false on error.
 */
static bool writeRegKeyValue(HKEY openedKey, char *valueName, char *value, HWND hDlg, char *user)
{
    uint32_t rc;
    size_t  len = strlen(value) + 1;

    rc = RegSetValueEx(openedKey, valueName, 0, REG_SZ, (LPBYTE)value, (uint32_t)len);
    if ( rc != ERROR_SUCCESS )
    {
        char extraMsg[SMALL_BUF_SIZE];

        _snprintf(extraMsg, SMALL_BUF_SIZE,
                  "Failed to set the %s value (%s) for %s",
                  valueName, value, user);

        reportErrorPlus(hDlg, "RegSetValueEx", REG_ERR_TITLE, extraMsg, rc);

        return false;
    }
    return true;
}

static bool registerRegExt(HWND hDlg, pAssocArguments paa, CSTRING extension)
{
    char *user;
    HKEY hPreDefKey = getScopeVars(paa, & user);
    HKEY hKey       = NULL;
    char subkeyName[MEDIUM_BUF_SIZE];

    // Creae, or overwrite, the .ext subkey of software\classees, and set the
    // default value to our prog ID
    _snprintf(subkeyName, MEDIUM_BUF_SIZE, "SOFTWARE\\Classes\\%s", extension);

    hKey = writeRegSubkeyValue(hPreDefKey, subkeyName, paa->progID, hDlg, user);
    if ( hKey == NULL )
    {
        return false;
    }

    return true;
}

// HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.ood

// HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.rxd

/**
 * Removes the ooDialog ftype and (optionally ?) its associated extensions
 *
 * @param hDlg
 * @param paa
 *
 * @return bool
 *
 * @note  TODO add code to check if the user wants the extensions removed.  For
 *        now we just do it. Possibly add code, or another function, to
 *        completely clean up the registry stuff that was, or may have been
 *        added by us.
 *
 * @note  We need to be sure to remove the extensions (if we are going to)
 *        before we remove the ftype (progID).  If we remove the progID first
 *        and an error happens removing the extension, we end up with a problem
 *        with any extension not removed where the registry refuses to open that
 *        extension.
 */
static bool removeProgID(HWND hDlg, pAssocArguments paa)
{
    if ( ! deleteRegAllExt(hDlg, paa) )
    {
        return false;
    }

    if ( ! deleteRegProgID(hDlg, paa) )
    {
        return false;
    }

    return true;
}

static bool addProgID(HWND hDlg, pAssocArguments paa)
{
    char *user;
    HKEY hPreDefKey = getScopeVars(paa, & user);
    HKEY hProgIdKey = NULL;
    HKEY hKey       = NULL;
    char valueBuf[MEDIUM_BUF_SIZE];

    // Do the ooRexx.ooDialog.1 subkey of software\classees
    _snprintf(valueBuf, MEDIUM_BUF_SIZE, "SOFTWARE\\Classes\\%s", paa->progID);

    hProgIdKey = writeRegSubkeyValue(hPreDefKey, valueBuf, OODIALOG_FRIENDLY_NAME, hDlg, user);
    if ( hProgIdKey == NULL )
    {
        goto done_out;
    }

    // Do the DefaultIcon subkey of ooRexx.ooDialog.1
    _snprintf(valueBuf, MEDIUM_BUF_SIZE, "%s,-%d", paa->exeName, IDI_APP_BLUE_BEAKER_ICON);

    hKey = writeRegSubkeyValue(hProgIdKey, "DefaultIcon", valueBuf, hDlg, user);
    if ( hKey == NULL )
    {
        goto done_out;
    }
    RegCloseKey(hKey);

    // Do the CurVer subkey of ooRexx.ooDilaog.1
    hKey = writeRegSubkeyValue(hProgIdKey, "CurVer", paa->progID, hDlg, user);
    if ( hKey == NULL )
    {
        goto done_out;
    }
    RegCloseKey(hKey);

    // Do the FiendlyTypeName value of ooRexx.ooDialog.1
    _snprintf(valueBuf, MEDIUM_BUF_SIZE, "@%s,-%d", paa->exeName, IDS_FRIENDLY_NAME);

    if ( ! writeRegKeyValue(hProgIdKey, "FriendlyTypeName", valueBuf, hDlg, user) )
    {
        goto done_out;
    }

    // Do the InfoTip value of ooRexx.ooDailog.1
    _snprintf(valueBuf, MEDIUM_BUF_SIZE, "@%s,-%d", paa->exeName, IDS_INFOTIP);

    if ( ! writeRegKeyValue(hProgIdKey, "InfoTip", valueBuf, hDlg, user) )
    {
        goto done_out;
    }

    // Do the shell subkey of ooRexx.ooDiallog.1 and its subkeys.
    // This will create the shell\open sub subky and set its default to Run
    hKey = writeRegSubkeyValue(hProgIdKey, "shell\\open", "Run", hDlg, user);
    if ( hKey == NULL )
    {
        goto done_out;
    }
    RegCloseKey(hKey);

    // Now create the shell\open\command sub sub subkey
    _snprintf(valueBuf, MEDIUM_BUF_SIZE, "\"%s\" \"%%1\" %%*", paa->exeName);

    hKey = writeRegSubkeyValue(hProgIdKey, "shell\\open\\command", valueBuf, hDlg, user);
    if ( hKey == NULL )
    {
        goto done_out;
    }
    RegCloseKey(hKey);

    /* TODO add code to set an editor. (shell\edit) */

    /* TODO add code to use rexxc ? (shell\compile) */

    // Do the shellex\DropHandler subkey of ooRexx.ooDialog.1
    hKey = writeRegSubkeyValue(hProgIdKey, "ShellEx\\DropHandler", OODIALOG_DROP_HANDLER, hDlg, user);
    if ( hKey == NULL )
    {
        goto done_out;
    }

    RegCloseKey(hKey);
    RegCloseKey(hProgIdKey);

    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return true;

done_out:
    if ( hProgIdKey != NULL )
    {
        RegCloseKey(hProgIdKey);

        // We delete the key because it was not completely written.  We don't
        // care if it fails or not because if it fails we can't do any thing
        // about it.
        deleteRegProgID(hDlg, paa);

    }
    return false;
}

INT_PTR pbAddExtension(HWND hDlg)
{
    pAssocArguments paa = (pAssocArguments)getWindowPtr(hDlg, GWLP_USERDATA);

    pExtensionInfo info = NULL;
    char           text[MAX_EXT_NAME];
    int32_t        count = MAX_EXT_NAME;

    count = GetWindowText(paa->edit, text, count);
    if ( count == 0 )
    {
        goto err_out;
    }

    info = (pExtensionInfo)LocalAlloc(LPTR, sizeof(extensionInfo));
    if ( info == NULL )
    {
        reportError(hDlg, OUT_OF_MEMORY_ERR_FMT, OS_ERR_TITLE, "LocalAlloc", GetLastError());
        goto err_out;
    }
    if ( *text != '.' )
    {
        strcpy(info->extension, ".");
    }
    strcat(info->extension, text);

    qualifyExtensionInfo(info->extension, info, paa->progID);

    // If this extension is already registered in this scope, do nothing
    if ( extRegisteredInScope(paa, info) )
    {
        goto err_out;
    }

    // See if extension is already listed in the current list box
    uint32_t index;
    pExtensionInfo existing = getExtRec(paa->lbCurrent, info->extension, &index);
    if ( existing != NULL )
    {
        // It can't be registered in this scope, we already checked. We just
        // want to register and update the existing item.
        if ( ! registerRegExt(hDlg, paa, existing->extension) )
        {
            goto err_out;
        }

        nowRegisteredInScope(paa, existing);

        SendMessage(paa->lbCurrent, LB_DELETESTRING, index, 0);

        index = (uint32_t)SendMessage(paa->lbCurrent, LB_ADDSTRING, 0, (LPARAM)existing->displayName);
        SendMessage(paa->lbCurrent, LB_SETITEMDATA, index, (LPARAM)existing);

        LocalFree(info);
        goto done_out;
    }

    // See if the user entered an extenstion that is also in the suggested list
    // box.
    existing = getExtRec(paa->lbSuggested, info->extension, &index);
    if ( existing != NULL )
    {
        // It can't be registered in this scope, we already checked. We just
        // want to register, update the existing item, update the list boxes.
        if ( ! registerRegExt(hDlg, paa, existing->extension) )
        {
            goto err_out;
        }

        nowRegisteredInScope(paa, existing);

        SendMessage(paa->lbSuggested, LB_DELETESTRING, index, 0);

        index = (uint32_t)SendMessage(paa->lbCurrent, LB_ADDSTRING, 0, (LPARAM)existing->displayName);
        SendMessage(paa->lbCurrent, LB_SETITEMDATA, index, (LPARAM)existing);

        LocalFree(info);
        goto done_out;
    }

    // Okay, completely new, just register it and update the list box
    if ( ! registerRegExt(hDlg, paa, info->extension) )
    {
        goto err_out;
    }

    nowRegisteredInScope(paa, info);

    index = (uint32_t)SendMessage(paa->lbCurrent, LB_ADDSTRING, 0, (LPARAM)info->displayName);
    SendMessage(paa->lbCurrent, LB_SETITEMDATA, index, (LPARAM)info);

    goto done_out;

err_out:
    safeLocalFree(info);

done_out:
    SetWindowText(paa->edit, "");
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
    return TRUE;
}

/**
 * Handles a click on the Register / Un-register push button
 *
 * @param hDlg
 *
 * @return INT_PTR
 *
 * @note  If scope is all users and all users is registered, then we must be
 *        removing the prog ID.  Otherwise we must be adding the prog ID.
 *
 *        Same logic if scope is current user.  If current user is registered,
 *        we must be removing the prog ID, otherwise we are adding.
 *
 *        If the operation is not a success, then we disable the push button so
 *        that the user does not try again.
 *
 *        When the operation is a success, then the state of the dialog must
 *        always be reset.
 */
INT_PTR pbRegister(HWND hDlg)
{
    pAssocArguments paa = (pAssocArguments)getWindowPtr(hDlg, GWLP_USERDATA);

    bool removing;
    bool success;

    if ( paa->allUsers )
    {
        if ( paa->registeredAllUsers )
        {
            success  = removeProgID(hDlg, paa);
            removing = true;
        }
        else
        {
            success  = addProgID(hDlg, paa);
            removing = false;
        }
    }
    else
    {
        if ( paa->registeredCurUsers )
        {
            success  = removeProgID(hDlg, paa);
            removing = true;
        }
        else
        {
            success  = addProgID(hDlg, paa);
            removing = false;
        }
    }

    if ( success )
    {
        if ( removing )
        {
            if ( paa->allUsers )
            {
                paa->registeredAllUsers = false;
            }
            else
            {
                paa->registeredCurUsers = false;
            }
        }
        else
        {
            if ( paa->allUsers )
            {
                paa->registeredAllUsers = true;
            }
            else
            {
                paa->registeredCurUsers = true;
            }
        }

        paa->ftypeIsRegistered = (paa->registeredAllUsers || paa->registeredCurUsers) ? true : false;
        setRegisterdState(hDlg, paa);
    }
    else
    {
        EnableWindow(paa->pbRegister, FALSE);
    }
    return TRUE;
}

INT_PTR handleButtonClick(HWND hDlg, WPARAM wParam, LPARAM lParam)
{

    switch ( LOWORD(wParam) )
    {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, wParam);
            return TRUE;

        case IDC_PB_REGISTER :
            return pbRegister(hDlg);

        case IDC_PB_ADD_EXTENSION :
            return pbAddExtension(hDlg);

        case IDC_PB_ADD_CURRENT :

            break;

        case IDC_PB_REMOVE_CURRENT :

            break;

        case IDC_PB_ADD_PATHEXT :

            break;

        case IDC_PB_REMOVE_PATHEXT :

            break;


        default:
            break;
    }
    return FALSE;
}

/**
 *  HKEY_CURRENT_USER \ Environment
 *
 *  HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session
 *  Manager\Environment
 *
 * @param hDlg
 * @param uMsg
 * @param wParam
 * @param lParam
 *
 * @return INT_PTR CALLBACK
 */
INT_PTR CALLBACK FileAssocDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( uMsg == WM_INITDIALOG )
    {
        pAssocArguments paa = (pAssocArguments)lParam;

        // Query the registry to see if ooDialog ftype / progID is already
        // registered.  If so ...
        checkRegistration(paa);

        //strcpy(paa->progID, "SlickEdit"); // temp for testing

        setUp(hDlg, paa);

        return TRUE;
    }

    switch ( uMsg )
    {
        case WM_COMMAND:
            // This is okay because we do not have any menus:
            if ( HIWORD(wParam) == BN_CLICKED )
            {
                return handleButtonClick(hDlg, wParam, lParam);
            }
            break;

        default:
            break;
    }

    return FALSE;
}


INT_PTR CALLBACK ConfigureDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( uMsg == WM_INITDIALOG )
    {
        pConfigureArguments pca = (pConfigureArguments)lParam;

        setDlgIcon(hDlg, pca->hInstance);

        if ( pca->wasElevated )
        {
            // We are only restarted as elevated when all users is checked.
            CheckDlgButton(hDlg, IDC_RB_ALL, BST_CHECKED);
        }
        else
        {
            CheckDlgButton(hDlg, IDC_RB_CURRENT, BST_CHECKED);
        }

        uint32_t rc = determineStatus(hDlg, pca);
        if ( rc != ERROR_SUCCESS )
        {
            internalErrorMsgBox("Operaring system errors prevent continuing.  Going to abort",
                                "ooDialog Configure Services: Windows Error");

            EndDialog(hDlg, wParam);
            return FALSE;
        }

        SetDlgItemText(hDlg, IDC_ST_INADMINGROUP, pca->isInAdminGroup ? "True" : "False");
        SetDlgItemText(hDlg, IDC_ST_ISRUNASADMIN, pca->isRunAsAdmin ? "True" : "False");

        if ( pca->isVistaOrLater )
        {
            SetDlgItemText(hDlg, IDC_ST_ISELEVATED, pca->isElevated ? "True" : "False");
            SetDlgItemText(hDlg, IDC_ST_IL, level2text(pca->integrityLevel));
        }
        else
        {
            SetDlgItemText(hDlg, IDC_ST_ISELEVATED, "N/A");
            SetDlgItemText(hDlg, IDC_ST_IL, "N/A");

            // If not running as admin on XP, can not select all users.
            if ( ! pca->isRunAsAdmin )
            {
                EnableWindow(GetDlgItem(hDlg, IDC_RB_ALL), FALSE);
            }
        }

        setWindowPtr(hDlg, GWLP_USERDATA, (LONG_PTR)pca);

        if ( pca->wasElevated )
        {
            PostMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_PB_CONFIGURE, BN_CLICKED), 0);
        }

        return TRUE;
    }

    pConfigureArguments pca = (pConfigureArguments)getWindowPtr(hDlg, GWLP_USERDATA);

    switch ( uMsg )
    {
        case WM_COMMAND:
            switch ( LOWORD(wParam) )
            {
                case IDC_RB_ALL :
                {
                    if ( HIWORD(wParam) == BN_CLICKED && pca->requiresElevation )
                    {
                        Button_SetElevationRequiredState(GetDlgItem(hDlg, IDC_PB_CONFIGURE), TRUE);
                    }
                }
                break;

                case IDC_RB_CURRENT :
                {
                    if ( HIWORD(wParam) == BN_CLICKED && pca->requiresElevation )
                    {
                        Button_SetElevationRequiredState(GetDlgItem(hDlg, IDC_PB_CONFIGURE), FALSE);
                    }
                }
                break;

                case IDC_PB_CONFIGURE :
                {
                    if ( HIWORD(wParam) == BN_CLICKED )
                    {
                        uint32_t error;
                        bool     isAllUsers = IsDlgButtonChecked(hDlg, IDC_RB_ALL) ? true : false;

                        if ( isAllUsers && pca->requiresElevation )
                        {
                            char exeName[MAX_PATH];

                            if ( GetModuleFileName(NULL, exeName, ARRAYSIZE(exeName)) )
                            {
                                // Re-execute ourself as Administrator
                                SHELLEXECUTEINFO sei = { sizeof(sei) };

                                sei.lpVerb       = "runas";
                                sei.lpFile       = exeName;
                                sei.lpParameters = "-confElevatedAllUsers";
                                sei.hwnd         = hDlg;
                                sei.nShow        = SW_NORMAL;

                                if ( ! ShellExecuteEx(&sei))
                                {
                                    error = GetLastError();
                                    if ( error == ERROR_CANCELLED )
                                    {
                                        // The user refused the elevation.
                                        // Do nothing ...
                                    }
                                    else
                                    {
                                        reportError(hDlg, "ShellExecuteEx", OS_ERR_TITLE, error);
                                    }
                                }
                                else
                                {
                                    // Quit this process.
                                    ShowWindow(hDlg, SW_HIDE);
                                    EndDialog(hDlg, TRUE);
                                }
                            }
                            else
                            {
                                error = GetLastError();
                                reportError(hDlg, "GetModuleFileName", OS_ERR_TITLE, error);
                            }
                        }
                        else
                        {
                            // Start the file assoc dialog.
                            assocArguments aa = { 0 };

                            aa.isRunAsAdmin = pca->isRunAsAdmin;
                            aa.hInstance    = pca->hInstance;
                            aa.allUsers     = isAllUsers;
                            aa.isElevated   = pca->isElevated;
                            aa.exeName      = pca->exeName;

                            strcpy(aa.progID, OODIALOG_PROGID_VERSIONED);

                            DialogBoxParam(pca->hInstance, MAKEINTRESOURCE(IDD_FILEASSOC), hDlg, FileAssocDlgProc, (LPARAM)&aa);
                        }
                    }
                }
                    break;

                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg, wParam);
                    return TRUE;

            }
            break;

        default:
            break;
    }

    return FALSE;
}


inline bool haveFlag(CSTRING a)
{
    return *a == '-' || *a == '/';
}

static int32_t displayCondition(RexxThreadContext *c, pProgramArguments pa)
{
    wholenumber_t code = 0;

    char *msg = getStandardConditionMsg(c, &code, NULL);
    if ( msg )
    {
        pa->mainMsg = msg;
        pa->dlgType = conditionDisplay;

        DialogBoxParam(pa->hInstance, MAKEINTRESOURCE(IDD_OODIALOG_DISPLAY), NULL, WinMainDlgProc, (LPARAM)pa);

        LocalFree(msg);
    }
    return (int32_t)code;
}

static int32_t ooDialogServices(pProgramArguments pa)
{
    configureArguments ca = { 0 };
    ca.hInstance = pa->hInstance;
    ca.exeName   = pa->exeName;

    DialogBoxParam(pa->hInstance, MAKEINTRESOURCE(IDD_CONFIGURE_SERVICES), NULL, ConfigureDlgProc, (LPARAM)&ca);

    return 0;
}

static int32_t displaySyntax(pProgramArguments pa)
{
    pa->dlgType = syntaxDisplay;
    pa->mainMsg = long_syntax_text;

    DialogBoxParam(pa->hInstance, MAKEINTRESOURCE(IDD_OODIALOG_DISPLAY), NULL, WinMainDlgProc, (LPARAM)pa);

    if ( pa->doConfigure )
    {
        ooDialogServices(pa);
    }
    return 0;
}

static int32_t displayVersion(RexxThreadContext *c, pProgramArguments pa)
{
    if ( pa->showShortVersion )
    {
        char buf[MEDIUM_BUF_SIZE];

        _snprintf(buf, MEDIUM_BUF_SIZE, "ooDialog version %u.%u.%u.%u", OOD_VER, OOD_REL, OOD_MOD, OOD_BLD);
        internalInfoMsgBox(buf, "ooDialog Execute Program: Version Information");
        return 0;
    }

    char *ver = getCompleteVersion(c);
    if ( ver )
    {
        pa->dlgType = versionDisplay;
        pa->mainMsg = ver;
        DialogBoxParam(pa->hInstance, MAKEINTRESOURCE(IDD_OODIALOG_DISPLAY), NULL, WinMainDlgProc, (LPARAM)pa);

        LocalFree(ver);
    }
    else
    {
        internalErrorMsgBox("ooDialog internal error, could not get version string.", "ooDialog Execute Program: Internal Error");
        return -1;
    }
    return 0;
}

/**
 * Gets the wide character string command line arguments in conventional argv /
 * argc format and converst the argument array to an array of ANSI strings.
 *
 * @param count
 *
 * @return An array of null-terminated ANSI strings, or null on error.
 *
 * @note  The returned array does not contain the executable name.
 *
 *        If there is no error, it is the responsibility of the caller to free
 *        both the returned array and the strings in the array.
 *
 *        On error, there is no memory to free.  We free the allocated argv and
 *        any strings it may point to, even though we do not need to free this
 *        memory.  On error, we are going to immediately exit the program and
 *        the OS will free the memory.  We just do it because it looks a little
 *        cleaner.
 */
static char **commandLineToArgv(int32_t *count, char *exeName)
{
    LPWSTR  *szArglist = NULL;
    char   **args      = NULL;
    int32_t nArgs      = 0;
    int32_t i;

    char buf[512] = {'\0'};  // Possible error message buffer

    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if ( NULL == szArglist )
    {
        reportError(OS_PARSING_ERR_FMT, GetLastError(), OS_ERR_TITLE);
        goto done_out;
    }

    // Need to save the fully qualified executable name.
    char *tmp = unicode2ansi(szArglist[0]);
    if ( tmp )
    {
        strcpy(exeName, tmp);
        LocalFree(tmp);

        if ( PathIsRelative(exeName) )
        {
            PathFindOnPath(exeName, NULL);
        }
    }
    else
    {
        unicodeConversionErr();
        goto done_out;
    }

    // For the returned argument array, we are going skip the first arg, the
    // executable name.
    nArgs--;

    args = (char **)LocalAlloc(LPTR, nArgs * sizeof(char **));
    if ( args == NULL )
    {
        unicodeConversionErr();
        goto done_out;
    }

    for ( i = 0; i < nArgs; i++)
    {
        LPWSTR a = szArglist[i + 1];

        if ( wcslen(a) == 0 )
        {
            reportError(EMPTY_STRING_ARG_ERR_FMT, i + 1, USER_ERR_TITLE);
            goto error_out;
        }

        tmp = unicode2ansi(a);
        if ( tmp )
        {
            args[i] = tmp;
        }
        else
        {
            unicodeConversionErr();
            goto error_out;
        }
    }

    goto done_out;

error_out:
    if ( args != NULL )
    {
        for ( int32_t j = 0; j < nArgs; j++ )
        {
            safeLocalFree(args[j]);
        }
        LocalFree(args);
    }
    args  = NULL;
    nArgs = 0;

done_out:
    LocalFree(szArglist);
    *count = nArgs;

    return args;
}

static bool getFlags(RexxThreadContext *c, pProgramArguments pa, char **argv, int32_t argc, int32_t *currentArg)
{
    int32_t i = *currentArg;

    while ( i < argc && haveFlag(argv[i]) )
    {
        switch ( argv[i][1] )
        {
            case '?' :
            case 'h' :
                pa->showShortHelp = true;
                c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                LocalFree(argv[i]);
                break;

            case 'H' :
                pa->showHelp = true;
                c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                LocalFree(argv[i]);
                break;

            case 's' :
                pa->doConfigure = true;
                c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                LocalFree(argv[i]);
                break;

            case 'v' :
                pa->showShortVersion = true;
                c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                LocalFree(argv[i]);
                break;

            case 'V' :
                pa->showVersion = true;
                c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                LocalFree(argv[i]);
                break;

            case '-' :
                if ( stricmp(argv[i], "--version") == 0 )
                {
                    pa->showVersion = true;
                    c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                    LocalFree(argv[i]);
                }
                else if ( stricmp(argv[i], "--help") == 0 )
                {
                    pa->showHelp = true;
                    c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                    LocalFree(argv[i]);
                }
                break;

            default :
            {
                char msg[256];

                sprintf(msg, "Invalid command line argument:\n\n"
                             "  Argument:\t%s\n"
                             "  Position:\t\t%d\n",
                             argv[i], i + 1);
                internalErrorMsgBox(msg, "ooDialog Execute Program: User Error");

                LocalFree(argv[i]);
                return false;
            }
        }
        i++;
    }

    *currentArg = i;
    return true;
}

static bool parseArgs(RexxThreadContext *c, pProgramArguments pa, char **argv, int32_t argc, LPSTR lpCmdLine)
{
    int32_t currentArg = 0;
    bool    result     = false;

    pa->rxOodArgs = c->NewArray(argc);

    if ( ! getFlags(c, pa, argv, argc, &currentArg) )
    {
        displaySyntax(pa);
        goto done_out;
    }

    if ( pa->showShortHelp || pa->showShortVersion || pa->showHelp || pa->showVersion || pa->doConfigure )
    {
        // If any of these are set, we're done;
        result = true;
        goto done_out;
    }

    if ( currentArg >= argc )
    {
        internalErrorMsgBox("The ooDialog program to execute was not specified.", "ooDialog Execute Program: User Error");
        displaySyntax(pa);
        goto done_out;
    }

    size_t prgLen = strlen(argv[currentArg]);

    pa->oodProgram = (char *)LocalAlloc(LPTR, prgLen + 1);
    if ( pa->oodProgram == NULL )
    {
        internalErrorMsgBox("The Operating System failed to allocate memory.", "ooDialog Execute Program: Windows Error");
        LocalFree(argv[currentArg]);
        goto done_out;
    }

    memcpy((void *)pa->oodProgram, argv[currentArg], prgLen);
    LocalFree(argv[currentArg++]);

    if ( currentArg >= argc )
    {
        pa->callArg = c->NewArray(0);
        result = true;
        goto done_out;
    }

    char *tmp = strstr(lpCmdLine, pa->oodProgram);
    tmp += prgLen;
    while( isspace(*tmp) )
    {
        tmp++;
    }

    pa->callArg = c->ArrayOfOne(c->String(tmp));
    pa->rxCArgs = c->NewArray(argc - currentArg);

    for ( int32_t j = 1; currentArg < argc; currentArg++, j++ )
    {
        c->ArrayPut(pa->rxCArgs,   c->String(argv[currentArg]), currentArg + 1);
        c->ArrayPut(pa->rxOodArgs, c->String(argv[currentArg]), j);
        LocalFree(argv[currentArg]);
    }
    result = true;

done_out:
    LocalFree(argv);
    return result;
}

/**
 * Standard Windows entry point.
 *
 * @param hInstance
 * @param hPrevInstance
 * @param lpCmdLine
 * @param nCmdShow
 *
 * @return int
 *
 * @notes  Testing shows that lpCmdLine is never null.
 *
 *         If the first argument is exactly: -confElevatedAllUsers then this a
 *         special case.  We were in the ConfigureDlgProc procedure and the user
 *         requested to change or set the All Users file associations, on Vista
 *         or later, and elevation was required.  So, we have been restarted
 *         through ShellExcuteEx().
 *
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    programArguments pa = { 0 };
    int32_t          rc = 0;

    pa.hInstance = hInstance;

    if ( strlen(lpCmdLine) == 0 )
    {
        pa.allowServices = true;
        return displaySyntax(&pa);
    }

    // Get the command line in a C style argument vector form.
    int32_t   argc;
    char    **argv = commandLineToArgv(&argc, pa.exeName);
    if ( argv == NULL )
    {
        return -1;
    }

    if ( strcmp(argv[0], "-confElevatedAllUsers") == 0 )
    {
        configureArguments ca = { 0 };

        ca.hInstance   = hInstance;
        ca.wasElevated = true;
        ca.exeName     = pa.exeName;

        DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_CONFIGURE_SERVICES), NULL, ConfigureDlgProc, (LPARAM)&ca);
        return 0;
    }

    // Mostly, we will be executing a Rexx program.  parseArgs() needs an
    // interpreter thread context, so we create the interpreter here.  For the
    // few times it is not needed, the overhead is negligible.
    RexxThreadContext *c;
    RexxInstance      *interpreter;

    if ( ! RexxCreateInterpreter(&interpreter, &c, NULL) )
    {
        internalErrorMsgBox("The ooRexx interpreter instance could not be created",
                            "ooDialog Execute Program: ooRexx Error");
        return -1;
    }

    if ( ! parseArgs(c, &pa, argv, argc, lpCmdLine) )
    {
        rc = -1;
        goto done_out;
    }

    if ( pa.showVersion || pa.showShortVersion )
    {
        displayVersion(c, &pa);
        goto done_out;
    }

    if ( pa.showHelp || pa.showShortHelp )
    {
        pa.allowServices = pa.showHelp ? true : false;
        displaySyntax(&pa);
        goto done_out;
    }

    if ( pa.doConfigure )
    {
        ooDialogServices(&pa);
        goto done_out;
    }

    // We put 2 argument arrays into the .local environment
    RexxDirectoryObject localEnv = (RexxDirectoryObject)c->GetLocalEnvironment();

    c->DirectoryPut(localEnv, pa.rxCArgs, "SYSCARGS");
    c->DirectoryPut(localEnv, pa.rxOodArgs, "OODCARGS");

    // Have the interpreter run the program:
    RexxObjectPtr result = c->CallProgram(pa.oodProgram, pa.callArg);

    if ( c->CheckCondition() )
    {
        rc = displayCondition(c, &pa);
        rc = -rc;  // Traditionally, the return code is negated on a condition.
    }
    else
    {
        if ( result != NULLOBJECT )
        {
            c->ObjectToInt32(result, &rc);
        }
    }

done_out:
    interpreter->Terminate();

    return rc;
}


