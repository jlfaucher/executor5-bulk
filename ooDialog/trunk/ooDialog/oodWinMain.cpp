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
#include "oodShared.hpp"
#include "oodExecutable.hpp"

typedef enum
{
    noDislpay = 0,
    conditionDisplay,
    versionDisplay,
    syntaxDisplay
} dlgType_t;

typedef struct _programArguments
{
    RexxArrayObject    callArg;           // The legacy single string argument to a Rexx program
    RexxArrayObject    rxCArgs;           // Array of C style program arguments for SysCArgs put in .local
    RexxArrayObject    rxOodArgs;         // Similar to rxCArgs, but contains all arguments, .oodCArgs
    HINSTANCE          hInstance;         // This exectuable's instance
    const char        *oodProgram;        // Rexx progrm to run
    const char        *mainMsg;           // Used to pass text to display to the dialog proc edure
    dlgType_t          dlgType;           // If a display dialog is used, specifies what is displayed
    bool               doSetupScreen;     // Show the set up dialog, not used since we only have file association at this time.
    bool               showShortVersion;  // Show short version and quit
    bool               showVersion;       // Show version and quit
    bool               showShortHelp;     // Show short help and quit
    bool               showHelp;          // Show help and quit
    bool               allowServices;     // Show or don't show the configure services controls
    bool               doConfigure;       // Show the configure services dialog
} programArguments;
typedef programArguments *pProgramArguments;

typedef struct _assocArguments
{
    HINSTANCE          hInstance;         // This executable's instance
    bool               allUsers;          // If true file associations is for all users, otherwise current user
    bool               isRunAsAdmin;
    bool               isElevated;
} assocArguments;
typedef assocArguments *pAssocArguments;

typedef struct _configureArguments
{
    HINSTANCE          hInstance;         // This executable's instance
    uint32_t           integrityLevel;
    bool               isVistaOrLater;    // Os is at least Vista
    bool               isInAdminGroup;
    bool               isRunAsAdmin;
    bool               isElevated;
    bool               requiresElevation;
    bool               wasElevated;
} configureArguments;
typedef configureArguments *pConfigureArguments;


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
 * Formats an error message and shows it in a message box
 *
 * @param function
 * @param title
 * @param dw
 */
static void reportError(HWND hwnd, CSTRING function, CSTRING title, HRESULT dw)
{
    char  msgBuf[HUGE_BUF_SIZE];
	void *formatMsgBuf;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&formatMsgBuf, 0, NULL);

    _snprintf(msgBuf, HUGE_BUF_SIZE, "Error executing Windows API:\n\nFunction:\t\t%s\nError Code:\t%d\n\n%s",
              function, dw, formatMsgBuf);

	MessageBox(hwnd, (LPCTSTR)msgBuf, title, MB_ICONERROR);
	LocalFree(formatMsgBuf);
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
        reportError(hwnd, "OpenProcessToken", "Windows Operating System Error", error);
        goto done_out;
    }

    // Query the size of the token integrity level so we can allocate a buffer
    // for it.
    if ( ! GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, (DWORD *)&tokenSize) )
    {
        if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
        {
            error = GetLastError();
            reportError(hwnd, "GetTokenInformation", "Windows Operating System Error", error);
            goto done_out;
        }
    }

    // Now we allocate a buffer for the integrity level information.
    pTokenIL = (TOKEN_MANDATORY_LABEL *)LocalAlloc(LPTR, tokenSize);
    if (pTokenIL == NULL)
    {
        error = GetLastError();
        reportError(hwnd, "LocalAlloc", "Windows Operating System Error", error);
        goto done_out;
    }

    // Retrieve token integrity level information.
    if ( ! GetTokenInformation(hToken, TokenIntegrityLevel, pTokenIL, tokenSize, (DWORD *)&tokenSize))
    {
        error = GetLastError();
        reportError(hwnd, "GetTokenInformation", "Windows Operating System Error", error);
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
        reportError(hwnd, "OpenProcessToken", "Windows Operating System Error", error);
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
            reportError(hwnd, "GetTokenInformation", "Windows Operating System Error", error);
            goto done_out;
        }

        // If limited, get the linked elevated token for further check.
        if ( TokenElevationTypeLimited == elevType )
        {
            if ( ! GetTokenInformation(hToken, TokenLinkedToken, &hTokenToCheck, sizeof(hTokenToCheck), &cbSize) )
            {
                error = GetLastError();
                reportError(hwnd, "GetTokenInformation", "Windows Operating System Error", error);
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
            reportError(hwnd, "DuplicateToken", "Windows Operating System Error", error);
            goto done_out;
        }
    }

    // Create the SID corresponding to the Administrators group.
    BYTE adminSID[SECURITY_MAX_SID_SIZE];

    cbSize = sizeof(adminSID);
    if ( ! CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &cbSize) )
    {
        error = GetLastError();
        reportError(hwnd, "CreateWellKnownSid", "Windows Operating System Error", error);
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
        reportError(hwnd, "CheckTokenMembership", "Windows Operating System Error", error);
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
        reportError(hwnd, "OpenProcessToken", "Windows Operating System Error", error);
        goto done_out;
    }

    // Retrieve token elevation information.
    TOKEN_ELEVATION elevation;
    uint32_t        size;
    if ( ! GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), (DWORD *)&size) )
    {
        error = GetLastError();
        reportError(hwnd, "GetTokenInformation", "Windows Operating System Error", error);
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
        reportError(hwnd, "AllocateAndInitializeSid", "Windows Operating System Error", error);
        goto done_out;
    }

    // Determine whether the SID of administrators group is enabled in
    // the primary access token of the process.
    BOOL isAdmin;
    if ( ! CheckTokenMembership(NULL, adminGroup, &isAdmin) )
    {
        error = GetLastError();
        reportError(hwnd, "CheckTokenMembership", "Windows Operating System Error", error);
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
        reportError(hDlg, "GetVersionEx", "Windows Operating System Error", rc);
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

INT_PTR CALLBACK WinMainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HFONT font = NULL;

    if ( uMsg == WM_INITDIALOG )
    {
        pProgramArguments pa = (pProgramArguments)lParam;

        setDlgIcon(hDlg, pa->hInstance);

        // Initialize the edit control that displays the long text.
        HWND hEdit  = GetDlgItem(hDlg, IDC_EDIT);
        int  height = -MulDiv(10, GetDeviceCaps(GetDC(hDlg), LOGPIXELSY), 72);

        font = CreateFont(height, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_ONLY_PRECIS,
                          CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_MODERN, "Courier New");

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

        setDlgIcon(hDlg, paa->hInstance);

        SetDlgItemText(hDlg, IDC_ST_SCOPE, paa->allUsers ? "All Users" : "CurrentUser");
        SetDlgItemText(hDlg, IDC_ST_RUNAS, paa->isElevated ? "True" : "False");
        SetDlgItemText(hDlg, IDC_ST_ELEVATED, paa->isElevated ? "True" : "False");


        setWindowPtr(hDlg, GWLP_USERDATA, (LONG_PTR)paa);

        return TRUE;
    }

    switch ( uMsg )
    {
        case WM_COMMAND:
            switch ( LOWORD(wParam) )
            {
                case IDOK:
                case IDCANCEL:
                {
                    EndDialog(hDlg, wParam);
                }
                return TRUE;

                default:
                    break;
            }

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
                                        reportError(hDlg, "ShellExecuteEx", "Windows Operating System Error", error);
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
                                reportError(hDlg, "GetModuleFileName", "Windows Operating System Error", error);
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


inline void safeLocalFree(void *p)
{
    if ( p != NULL )
    {
        LocalFree(p);
    }
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
static char **commandLineToArgv(int32_t *count)
{
    LPWSTR  *szArglist = NULL;
    char   **args      = NULL;
    int32_t nArgs      = 0;
    int32_t i;

    char buf[512] = {'\0'};  // Possible error message buffer

    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if ( NULL == szArglist )
    {
        sprintf(buf, "Operating system parsing of the command line failed.\n\n"
                     "Last reported error code: %d\n", GetLastError());
        internalErrorMsgBox(buf, "ooDialog Execute Program: Windows Error");
        goto done_out;
    }

    // We are going skip the first arg which is the executable name.
    nArgs--;

    args = (char **)LocalAlloc(LPTR, nArgs * sizeof(char **));
    if ( args == NULL )
    {
        sprintf(buf, "Conversion from Unicode to ANSI, or memory\n"
                     "allocation, failed.");
        internalErrorMsgBox(buf, "ooDialog Execute Program: Windows Error");
        goto done_out;
    }

    for ( i = 0; i < nArgs; i++)
    {
        LPWSTR a = szArglist[i + 1];

        if ( wcslen(a) == 0 )
        {
            sprintf(buf, "Argument %d is the empty string (\"\").\n\n"
                         "This is not allowed.\n", i + 1);
            internalErrorMsgBox(buf, "ooDialog Execute Program: User Error");
            goto error_out;
        }

        char *tmp = unicode2ansi(a);
        if ( tmp )
        {
            args[i] = tmp;
        }
        else
        {
            sprintf(buf, "Conversion from Unicode to ANSI, or memory\n"
                         "allocation, failed.");
            internalErrorMsgBox(buf, "ooDialog Execute Program: Windows Error");
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
    char    **argv = commandLineToArgv(&argc);
    if ( argv == NULL )
    {
        return -1;
    }

    if ( strcmp(argv[0], "-confElevatedAllUsers") == 0 )
    {
        configureArguments ca = { 0 };

        ca.hInstance   = hInstance;
        ca.wasElevated = true;

        DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_CONFIGURE_SERVICES), NULL, ConfigureDlgProc, (LPARAM)&ca);
        return 0;
    }

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


