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
 * ooDialog.exe, is a stand alone interface to run ooDialog programs.  This is
 * essentially rexxhide, with some ooDialog specific modifications.
 *
 * We use oodMain.cpp to produce an ooDialog.com executable whose sole purpose
 * is to produce a console mode app to handle some command line options.  If
 * these specific command line options are detected, they are processed and the
 * .com executable then exits.
 *
 * If they are not present, the .com executable uses ShellExecute() to launch
 * the Windows subsystem mode ooDialog.exe.
 *
 * Currently -v and --version are the only 2 options handled in the .com
 * version.  If ooDialog.com is executed from a command prompt, this allows us
 * to print the version string using printf().
 */

#include <windows.h>
#include <oorexxapi.h>
#include <stdio.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <tchar.h>
#include "oodShared.hpp"
#include "oodExecutable.hpp"

inline bool haveFlag(const char *a)
{
    return *a == '-' || *a == '/';
}

inline void printStandardVersion(void)
{
    printf(OOD_VER_STR"\n");
}

inline void printShortHelp(void)
{
    printf(short_syntax_text);
}

inline void printLongHelp(void)
{
    printf(long_syntax_text);
}

/**
 * Print out a comprehensive version string which includes the interpreter
 * version, date and time built, etc. This requires starting up the interpreter
 * and ending it. May be over-kill.
 *
 * If we fail to start the interpreter, we just ignore it and print out the
 * standard version string.
 */
static void printFullVersion(void)
{
    RexxThreadContext *c;
    RexxInstance      *interpreter;

    if ( RexxCreateInterpreter(&interpreter, &c, NULL) )
    {
        char *buf = getCompleteVersion(c);
        if ( buf )
        {
            printf(buf);
            LocalFree(buf);
        }
        else
        {
            printStandardVersion();
        }

        interpreter->Terminate();
        return;
    }

    printStandardVersion();
    return;
}

/**
 * Simple function to check for the 'test' flag.  The test flag is used for
 * internal purposes and not documented.  We use it to run tests that work
 * better printing directly to the console.
 *
 * @param argc
 * @param argv
 *
 * @return bool
 */
static bool isTestRequest(int argc, char **argv)
{
    if ( argc > 1 && argv[1][0] == '-' && argv[1][1] == 't' )
    {
        return true;
    }
    return false;
}


/**
 * Simple function to check for the version options, -v and --version.  If found
 * we use printf the version string specified.
 *
 * @param argc
 * @param argv
 *
 * @return bool
 */
static bool isVersionRequest(int argc, char **argv)
{
    for ( int i = 1; i < argc && haveFlag(argv[i]); i++ )
    {
        switch ( argv[i][1] )
        {
            case 'h' :
            case '?' :
                printShortHelp();
                return true;

            case 'v' :
                printStandardVersion();
                return true;

            case '-' :
                if ( stricmp(argv[i], "--version") == 0 )
                {
                    printFullVersion();
                    return true;
                }
                else if ( stricmp(argv[i], "--help") == 0 )
                {
                    printLongHelp();
                    return true;
                }
                break;

            default :
                break;
        }
    }

    return false;
}

void checkRegistration(char *progID)
{
    char     value[MAX_HKEY_VALUE] = {'\0'};
    uint32_t maxValue              = MAX_HKEY_VALUE;
    HKEY     hKey, hSubkey;

    // See if our progID is a subkey of classes root
    if( RegOpenKeyEx(HKEY_CLASSES_ROOT, progID, 0, KEY_QUERY_VALUE , &hKey) == ERROR_SUCCESS )
    {
        printf("%s is registered\n", progID);

        char buf[MEDIUM_BUF_SIZE];

        strcpy(buf, "SOFTWARE\\Classes\\");
        strcat(buf, progID);

        // See if progID is a subkey for local machine
        if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, buf, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
        {
            printf("%s is registered for all users\n", progID);

            RegCloseKey(hSubkey);
        }

        // See if progID is a subkey for current user
        if( RegOpenKeyEx(HKEY_CURRENT_USER, buf, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
        {
            printf("%s is registered for current user\n", progID);
            RegCloseKey(hSubkey);
        }

        RegCloseKey(hKey);
    }
}

void qualifyExtensionInfo(char *keyName, pExtensionInfo rec, char *progID)
{
    char     value[MAX_HKEY_VALUE] = {'\0'};
    uint32_t maxValue              = MAX_HKEY_VALUE;
    HKEY     hKey, hSubkey;

    // See if keyName is a subkey for current user
    if( RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Classes", 0, KEY_READ, &hKey) == ERROR_SUCCESS )
    {
        if ( RegOpenKeyEx(hKey, keyName, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
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

    // See if keyName is a subkey for local machine classes
    if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes", 0, KEY_READ, &hKey) == ERROR_SUCCESS )
    {
        if ( RegOpenKeyEx(hKey, keyName, 0, KEY_QUERY_VALUE, &hSubkey) == ERROR_SUCCESS )
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

    char *prefix = "   ";
    if ( rec->allUsers )
    {
        if ( rec->curUser )
        {
            prefix = "** ";
        }
        else if ( rec->curUserOther )
        {
            prefix = "*+ ";
        }
        else
        {
            prefix = "*  ";
        }
    }
    else if ( rec->curUser )
    {
        if ( rec->allUsersOther )
        {
            prefix = "+* ";
        }
        else
        {
            prefix = " * ";
        }
    }
    else
    {
        // This can not happen can it?
        printf("ERROR extension=%s in neither All Users NOR Current Users\n");
        prefix = "ERR ";
    }

    _snprintf(rec->displayName, MAX_EXT_DISPLAY, "%s%s", prefix, keyName);
}

pExtensionInfo getExtensionRecords(char *progID, size_t *count)
{
    uint32_t cSubKeys = 0;
    HKEY     hKCR     = HKEY_CLASSES_ROOT;

    // We only need the number of subkeys.
    uint32_t retCode = RegQueryInfoKey(HKEY_CLASSES_ROOT, NULL, NULL, NULL, (LPDWORD)&cSubKeys,
                                       NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    printf("RegQueryInfoKey() retCode=%d retCode=0x%08x count=%d\n", retCode, retCode, cSubKeys);
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
        /* What to do, we are not in an interpreter context ? */
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
 * Standard main entry point.
 *
 */
int __cdecl main(int argc, char *argv[])
{
    if ( isTestRequest(argc, argv) )
    {
        size_t count = 0;
        pExtensionInfo recs = getExtensionRecords("SlickEdit", &count);

        printf("\ngot extensions recs=%p count=%d\n", recs, count);

        for ( size_t i = 0; i < count; i++ )
        {
            pExtensionInfo current = &recs[i];
            printf("%s\n", current->displayName);
        }

        printf("\ncheck registration SlickEdit\n");
        checkRegistration("SlickEdit");

        printf("\ncheck registration soap\n");
        checkRegistration("soap");
        return 0;
    }

    if ( isVersionRequest(argc, argv) )
    {
        return 0;
    }

    // We want the command line as a string to pass to ShellExecute, but we need
    // to strip off the leading executable name:
    LPTSTR  cmdLine = GetCommandLineA();
    char   *tmp     = cmdLine;

    while( *tmp && ! isspace(*tmp) )
    {
        tmp++;
    }
    while( *tmp && isspace(*tmp) )
    {
        tmp++;
    }

    ShellExecute(NULL, "open", "ooDialog.exe", tmp, NULL, SW_SHOWNORMAL);

    return 0;
}

