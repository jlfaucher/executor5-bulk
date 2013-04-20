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
    RexxArrayObject    callArg;      // The legacy single string argument to a Rexx program
    RexxArrayObject    rxCArgs;      // Array of C style program arguments for SysCArgs put in .local
    RexxArrayObject    rxOodArgs;    // Similar to rxCArgs, but contains all arguments, .oodCArgs
    HINSTANCE          hInstance;    // This instance
    const char        *oodProgram;   // Rexx progrm to run
    const char        *mainMsg;      // Used to pass text to display to the dialog proc edure
    dlgType_t          dlgType;      // If a display dialog is used, specifies what is displayed
    bool               showVersion;  // Show version and quit
    bool               showHelp;     // Show help and quit
} programArguments;
typedef programArguments *pProgramArguments;

static void setDlgIcon(HWND hDlg, pProgramArguments pa)
{
     HANDLE hIcon = LoadImage(pa->hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON,
                             GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
      if( hIcon )
      {
          SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
      }
}

INT_PTR CALLBACK WinMainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HFONT font = NULL;

    if ( uMsg == WM_INITDIALOG )
    {
        pProgramArguments pa = (pProgramArguments)lParam;

        setDlgIcon(hDlg, pa);

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

        return TRUE;
    }

    switch ( uMsg )
    {
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg, wParam);

                    if ( font )
                    {
                        DeleteObject(font);
                    }
                    return TRUE;
            }
        default:
            break;
    }

    return FALSE;
}


inline void localFree(void *p)
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

static int32_t displaySyntax(pProgramArguments pa)
{
    pa->dlgType = syntaxDisplay;
    pa->mainMsg = long_syntax_text;

    DialogBoxParam(pa->hInstance, MAKEINTRESOURCE(IDD_OODIALOG_DISPLAY), NULL, WinMainDlgProc, (LPARAM)pa);

    return 0;
}

static int32_t displayVersion(RexxThreadContext *c, pProgramArguments pa)
{
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
            localFree(args[j]);
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
            case 'v' :
            case 'V' :
                pa->showVersion = true;
                c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                LocalFree(argv[i]);
                break;

            case 'h' :
                pa->showHelp = true;
                c->ArrayPut(pa->rxOodArgs, c->String(argv[i]), i + 1);
                LocalFree(argv[i]);
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
    /* // temp debug
    int32_t i = 0;
    char buf[2048] = {'\0'};

    for ( int32_t i = 0; i < argc; i++)
    {
        strcat(buf, argv[i]);
        strcat(buf, "\n");
    }
    internalInfoMsgBox(buf, "ooDialog Execute Program: Parse Args OK");
    */
    int32_t currentArg = 0;
    bool    result     = false;

    pa->rxOodArgs = c->NewArray(argc);

    if ( ! getFlags(c, pa, argv, argc, &currentArg) )
    {
        displaySyntax(pa);
        goto done_out;
    }

    if ( pa->showHelp || pa->showVersion )
    {
        // When these are set, we're done;
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
    internalInfoMsgBox(tmp, "Program args");

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
 * Standare Windows entry point.
 *
 * @param hInstance
 * @param hPrevInstance
 * @param lpCmdLine
 * @param nCmdShow
 *
 * @return int
 *
 * @notes  Testing shows that lpCmdLine is never null.
 */
int WINAPI WinMain(
    HINSTANCE hInstance,	    // handle to current instance
    HINSTANCE hPrevInstance,	// handle to previous instance
    LPSTR lpCmdLine,	        // pointer to command line
    int nCmdShow)
{
    programArguments pa = { 0 };
    int32_t          rc = 0;    // Rexx program RC

    pa.hInstance = hInstance;

    if ( strlen(lpCmdLine) == 0 )
    {
        return displaySyntax(&pa);
    }

    // Get the command line in a C style argument vector form.
    int32_t   argc;
    char    **argv = commandLineToArgv(&argc);
    if ( argv == NULL )
    {
        return -1;
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

    if ( pa.showVersion )
    {
        displayVersion(c, &pa);
        goto done_out;
    }

    if ( pa.showHelp )
    {
        displaySyntax(&pa);
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


