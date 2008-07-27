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
#include "oorexx.h"
#include "APIUtil.h"
#include <stdio.h>
#include <string.h>
#include <ddeml.h>
#include <time.h>
#include <shlobj.h>

#define STR_BUFFER    256
#define MAX_TIME_DATE 128
#define MAX_VARNAME   256


#define WINSYSDLL "RXWINSYS.DLL"

#define MSG_TIMEOUT  5000 // 5000ms
#if (WINVER >= 0x0500)
#define MSG_TIMEOUT_OPTS (SMTO_ABORTIFHUNG|SMTO_NORMAL|SMTO_NOTIMEOUTIFNOTHUNG)
#else
#define MSG_TIMEOUT_OPTS (SMTO_ABORTIFHUNG|SMTO_NORMAL)
#endif

void Little2BigEndian(BYTE *pbInt, int iSize);


#define ISHEX(value) \
   ((value[0] == '0') && (toupper(value[1]) == 'X'))

#define CONVERT_HEX_OR_INT(value, target)          \
     {\
         if (ISHEX(value))                         \
              (target) = strtol(value, '\0',16);   \
         else                                      \
              (target) = atol(value);              \
     }


#define GET_HKEY(argum, ghk) { \
     if (strstr(argum,"MACHINE")) ghk = HKEY_LOCAL_MACHINE; else \
     if (strstr(argum,"CLASSES")) ghk = HKEY_CLASSES_ROOT; else \
     if (strstr(argum,"CURRENT_USER")) ghk = HKEY_CURRENT_USER; else \
     if (strstr(argum,"USERS")) ghk = HKEY_USERS; else \
     if (strstr(argum,"PERFORMANCE")) ghk = HKEY_PERFORMANCE_DATA; else \
     if (strstr(argum,"CURRENT_CONFIG")) ghk = HKEY_CURRENT_CONFIG; else \
     if (strstr(argum,"DYN_DATA")) ghk = HKEY_DYN_DATA; else \
     if (ISHEX(argum)) ghk = (HKEY) strtoul(argum,'\0',16); else \
     ghk = (HKEY) atol(argum); }




#define GET_ACCESS(argu, acc) \
{ \
      if (!stricmp(argu,"READ"))    acc = 'R'; else \
      if (!stricmp(argu,"OPEN"))    acc = 'O'; else \
      if (!stricmp(argu,"CLOSE"))   acc = 'C'; else  \
      if (!stricmp(argu,"NUM"))     acc = 'N'; else  \
      if (!stricmp(argu,"CLEAR"))   acc = 'L'; else  \
      if (!stricmp(argu,"WRITE"))   acc = 'W'; else  \
         RETERR; \
}

#define GET_TYPE_INDEX(type, index)   \
{                                     \
    switch (type)                     \
    {                                 \
      case EVENTLOG_ERROR_TYPE:       \
         index=0;                     \
          break;                      \
      case EVENTLOG_WARNING_TYPE:     \
         index=1;                     \
         break;                       \
      case EVENTLOG_INFORMATION_TYPE: \
         index=2;                     \
         break;                       \
      case EVENTLOG_AUDIT_SUCCESS:    \
         index=3;                     \
         break;                       \
      case EVENTLOG_AUDIT_FAILURE:    \
         index=4;                     \
         break;                       \
      default:                        \
        index=4;                      \
    }                                 \
}


RexxFunction2(uintptr_t, WSCreateRegistryKey, uintptr_t, key, CSTRING, subkeyName)
{
      HKEY hkResult;

      // create the ken
      if (RegCreateKey((HKEY)key, subkeyName, &hkResult) == ERROR_SUCCESS)
      {
          return (uintptr_t)hkResult;
      }
      else
      {
          return 0;
      }
}


RexxFunction2(uintptr_t, WSOpenRegistryKey, CSTRING, keyName, OPTIONAL_CSTRING, subkeyName, OPTIONAL_CSTRING, accessArg)
{
    HKEY hkResult;
    DWORD access=0;

    GET_HKEY(keyName, hk);

    // if no subkey given, just return the predefined handle
    if (subkeyName == NULL)
    {
        return (uintptr_t)hk;
    }

    // default access is ALL
    if (accessArg == NULL || stricmp(accessArg, "ALL"))
    {
        access = KEY_ALL_ACCESS;
    }
    else
    {
       if (strstr(accessArg, "WRITE")) access |= KEY_WRITE;
       if (strstr(accessArg, "READ")) access |= KEY_READ;
       if (strstr(accessArg, "QUERY")) access |= KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE;
       if (strstr(accessArg, "EXECUTE")) access |= KEY_EXECUTE;
       if (strstr(accessArg, "NOTIFY")) access |= KEY_NOTIFY;
       if (strstr(accessArg, "LINK")) access |= KEY_CREATE_LINK;
    }

    if (RegOpenKeyEx(hk, subkeyName, 0, access, &hkResult) == ERROR_SUCCESS)
    {
        return (uintptr_t)hkResult;
    }
    else
    {
        return 0;
    }
}


RexxFunction1(RexxBoolean, WSCloseRegistryKey, uintptr_t, hk)
{
    return (RegClose((HKEY)hk) == ERROR_SUCCESS) ? 0 : 1;
}


RexxFunction1(RexxBoolean, WSDeleteRegistryKey, uintptr_t, hk)
{
    return (RegDeleteKey((HKEY)hk) == ERROR_SUCCESS) ? 0 : 1;
}


RexxFunction1(int, WSQueryRegistryKey, uintptr_t, hk)
{
    char Class[256];
    DWORD retcode, cbClass, cSubKeys, cbMaxSubKeyLen,
    cbMaxClassLen, cValues, cbMaxValueNameLen, cbMaxValueLen, cbSecurityDescriptor;
    FILETIME ftLastWriteTime;
    SYSTEMTIME stTime;

    cbClass = 256;

    if ((retcode=RegQueryInfoKey ((HKEY)hk,      // handle of key to query
           Class,    // address of buffer for class string
           &cbClass,    // address of size of class string buffer
           NULL,    // reserved
           &cSubKeys,    // address of buffer for number of subkeys
           &cbMaxSubKeyLen,    // address of buffer for longest subkey name length
           &cbMaxClassLen,    // address of buffer for longest class string length
           &cValues,    // address of buffer for number of value entries
           &cbMaxValueNameLen,    // address of buffer for longest value name length
           &cbMaxValueLen,    // address of buffer for longest value data length
           &cbSecurityDescriptor,    // address of buffer for security descriptor length
           &ftLastWriteTime     // address of buffer for last write time
    )) == ERROR_SUCCESS)
    {
        char buffer[512];
        if (FileTimeToSystemTime(&ftLastWriteTime,    // pointer to file time to convert
                             &stTime))     // pointer to structure to receive system time
            sprintf(buffer, "%s, %ld, %ld, %04d/%02d/%02d, %02d:%02d:%02d",
                   Class, cSubKeys, cValues, stTime.wYear, stTime.wMonth, stTime.wDay,
                   stTime.wHour, stTime.wMinute, stTime.wSecond);
        else
            sprintf(buffer, "%s, %ld, %ld",Class, cSubKeys, cValues);

        return context->NewStringFromAsciiz(buffer);
    }
    return context->NewStringFromAsciiz("0");
}


RexxFunction1(int, WSListRegistryKey, CSTRING, keyName, RexxStemObject, stem)
{
    DWORD retcode, ndx=0;

    GET_HKEY(keyName, hk);
    do {
       retcode = RegEnumKey(hk,    // handle of key to query
                            ndx++,    // index of subkey to query
                            Name,    // address of buffer for subkey name
                            sizeof(Name));     // size of subkey buffer
       if (retcode == ERROR_SUCCESS)
       {
           context->SetStemArrayItem(stem, ndx, context->NewStringFromAsciiz(Name));
       }
       else if (retcode != ERROR_NO_MORE_ITEMS)
       {
           return 1;
       }
    } while (retcode == ERROR_SUCCESS);
    context->SetStemArrayItem(stem, 0, context->NumberToObject(ndx - 1));
    return 0;
}


RexxFunction1(RexxBoolean, WSFlushRegistryKey, uintptr_t, hk)
{
    return (RegFlushKey((HKEY)hk) == ERROR_SUCCESS) ? 0 : 1;
}


RexxFunction4(RexxBoolean, WSSetRegistryValue, CSTRING, key, CSTRING, name, RexxObjectPtr, value, CSTRING, type)
{
    DWORD valType;
    DWORD dwNumber;
    DWORD dataLen;
    CONST BYTE * data;

    GET_HKEY(keyName, hk);

    if (!strcmp(type, "EXPAND")) valType = REG_EXPAND_SZ;
    else if (!strcmp(type, "MULTI")) valType = REG_MULTI_SZ;
    else if (!strcmp(type, "NUMBER")) valType = REG_DWORD;
    else if (!strcmp(type, "BINARY")) valType = REG_BINARY;
    else if (!strcmp(type, "LINK")) valType = REG_LINK;
    else if (!strcmp(type, "RESOURCELIST")) valType = REG_RESOURCE_LIST;
    else if (!strcmp(type, "RESOURCEDESC")) valType = REG_FULL_RESOURCE_DESCRIPTOR;
    else if (!strcmp(type, "RESOURCEREQS")) valType = REG_RESOURCE_REQUIREMENTS_LIST;
    else if (!strcmp(type, "BIGENDIAN")) valType = REG_DWORD_BIG_ENDIAN;
    else if (!strcmp(type, "NONE")) valType = REG_NONE;
    else
      valType = REG_SZ;

    if ((valType == REG_DWORD) || (valType == REG_DWORD_BIG_ENDIAN))
    {
        RexxNumber temp;
        context->ObjectToNumber(value, &temp);

        dwNumber = (DWORD)temp;

        if (valType == REG_DWORD_BIG_ENDIAN)
          Little2BigEndian((BYTE *) &dwNumber, sizeof(dwNumber));

        data = (CONST BYTE *) &dwNumber;
        dataLen = sizeof(dwNumber);
    }
    else
    {
        RexxStringObject strValue = context->ObjectToString(value);
       data = (CONST BYTE *) context->StringData(strValue);
       switch (valType)
       {
         case REG_BINARY:
         case REG_NONE:
         case REG_LINK:
         case REG_RESOURCE_LIST:
         case REG_FULL_RESOURCE_DESCRIPTOR:
         case REG_RESOURCE_REQUIREMENTS_LIST:
           dataLen = context->StringLength(strValue);
           break;

         case REG_EXPAND_SZ:
         case REG_MULTI_SZ:
         case REG_SZ:
           dataLen = context->StringLength(strValue) + 1;
           break;
       }
    }

    return (RegSetValueEx(hk, name, 0, valType, data, dataLen) == ERROR_SUCCESS) ? 0 : 1;
}


RexxFunction2(RexxStringObject, WSQueryRegistryValue, CSTRING, key, CSTRING, name)
{
    DWORD valType, cbData;
    char * valData, *vType;
    ULONG intsize;

    cbData = sizeof(valData);

    GET_HKEY(key, hk);

    if (RegQueryValueEx(hk,        // handle of key to query
                        name,        // address of name of value to query
                        NULL,        // reserved
                        &valType,    // address of buffer for value type
                        NULL,        // NULL to get the size
                        &cbData) == ERROR_SUCCESS) {        // address of data buffer size
        valData = GlobalAlloc(GPTR, cbData);

        if (!valData)
        {
            context->InvalidRoutine();
            return 0;
        }


        if (RegQueryValueEx(hk,    // handle of key to query
                        name,      // address of name of value to query
                        NULL,      // reserved
                        &valType,    // address of buffer for value type
                        valData,    // address of data buffer
                        &cbData) == ERROR_SUCCESS) {        // address of data buffer size

            char *retBuffer = GlobalAlloc(GMEM_FIXED, cbData + 10);
            if (!valData)
            {
                GlobalFree(valData);
                context->InvalidRoutine();
                return 0;
            }

            switch (valType)
            {
               case REG_MULTI_SZ:
                  vType = "MULTI";
                  sprintf(retBuffer, "%s, ",vType);
                  memcpy(&retBuffer[strlen(retBuffer)], valData, cbData);
                  break;
               case REG_DWORD:
                  vType = "NUMBER";
                  sprintf(retBuffer,"%s, %ld",vType, *(DWORD *)valData);
                  break;
               case REG_BINARY:
                  vType = "BINARY";
                  sprintf(retBuffer,"%s, ",vType);
                  memcpy(&retBuffer[strlen(retBuffer)], valData, cbData);
                  break;
               case REG_NONE:
                  vType = "NONE";
                  sprintf(retstr->strptr,"%s, ",vType);
                  memcpy(&retBuffer[strlen(retBuffer)], valData, cbData);
                  break;
               case REG_SZ:
                  vType = "NORMAL";
                  sprintf(retBuffer,"%s, %s",vType, valData);
                  break;
               case REG_EXPAND_SZ:
                  vType = "EXPAND";
                  sprintf(retBuffer,"%s, %s",vType, valData);
                  break;
               case REG_RESOURCE_LIST:
                  vType = "RESOURCELIST";
                  sprintf(retBuffer,"%s, ",vType);
                  memcpy(&retBuffer[strlen(retBuffer)], valData, cbData);
                  break;
               case REG_FULL_RESOURCE_DESCRIPTOR:
                  vType = "RESOURCEDESC";
                  sprintf(retBuffer,"%s, ",vType);
                  memcpy(&retBuffer[strlen(retBuffer)], valData, cbData);
                  break;
               case REG_RESOURCE_REQUIREMENTS_LIST:
                  vType = "RESOURCEREQS";
                  sprintf(retBuffer,"%s, ",vType);
                  memcpy(&retBuffer[strlen(retBuffer)], valData, cbData);
                  break;
               case REG_LINK:
                  vType = "LINK";
                  sprintf(retBuffer,"%s, ",vType);
                  memcpy(&retBuffer[strlen(retBuffer)], valData, cbData);
                  break;
               case REG_DWORD_BIG_ENDIAN:
                  {
                    DWORD dwNumber;
                    vType = "BIGENDIAN";
                    dwNumber = * (DWORD *)valData;
                    Little2BigEndian((BYTE *) &dwNumber, sizeof(dwNumber));
                    sprintf(retBuffer,"%s, %ld",vType, dwNumber);
                  }
                  break;

               default:
                  vType = "OTHER";
                  sprintf(retBuffer,"%s,",vType);
            }
            if ((valType == REG_MULTI_SZ) ||
                (valType == REG_BINARY) ||
                (valType == REG_RESOURCE_LIST) ||
                (valType == REG_FULL_RESOURCE_DESCRIPTOR) ||
                (valType == REG_RESOURCE_REQUIREMENTS_LIST) ||
                (valType == REG_NONE))
                cbData = strlen(vType) + 2 + cbData;
            else
                cbData = strlen(retstr->strptr);

            GlobalFree(valData);
            RexxStringObject result = context->NewString(retBuffer, cbData);
            GlobalFree(retBuffer);
            return result;
        }
        GlobalFree(valData);
    }
    return context->NewStringFromAsciiz("0");
}


RexxFunction4(int, WSListRegistryValue, CSTRING, key, RexxStemObject, stem)
{
    DWORD retcode, ndx=0, valType, cbValue, cbData, initData = 1024;
    char * valData, Name[256];
    char sname[300];
    SHVBLOCK shvb;

    GET_HKEY(key, hk);

    valData = GlobalAlloc(GPTR, initData);
    if (!valData)
    {
        context->InvalidRoutine();
        return 0;
    }

    do {
       cbData = initData;
       cbValue = sizeof(Name);
       retcode = RegEnumValue(hk,    // handle of key to query
                            ndx++,    // index of subkey to query
                            Name,    // address of buffer for subkey name
                            &cbValue,
                            NULL,    // reserved
                            &valType,    // address of buffer for type code
                            valData,    // address of buffer for value data
                            &cbData);     // address for size of data buffer

       if ((retcode == ERROR_MORE_DATA) && (cbData > initData))   /* we need more memory */
       {
          GlobalFree(valData);
          initData = cbData;
          valData = GlobalAlloc(GPTR, cbData);
          if (!valData)
          {
              context->InvalidRoutine();
              return 0;
          }
          ndx--;                      /* try to get the previous one again */
          cbValue = sizeof(Name);
          retcode = RegEnumValue(hk,    // handle of key to query
                            ndx++,    // index of subkey to query
                            Name,    // address of buffer for subkey name
                            &cbValue,
                            NULL,    // reserved
                            &valType,    // address of buffer for type code
                            valData,    // address of buffer for value data
                            &cbData);     // address for size of data buffer
       }

       if (retcode == ERROR_SUCCESS)
       {

           sprintf(sname,"%d.NAME",ndx);
           context->SetStemElement(stem, sname, context->NewStringFromAsciiz(Name));
           sprintf(sname,"%d.TYPE",ndx);
           char *valName = "OTHER";
           switch (valType) {
              case REG_EXPAND_SZ:
                 valName = "EXPAND";
                 break;
              case REG_NONE:
                 valName = "NONE";
                 break;
              case REG_DWORD:
                 "NUMBER";
                 break;
              case REG_MULTI_SZ:
                 "MULTI";
                 break;
              case REG_BINARY:
                 "BINARY";
                 break;
              case REG_SZ:
                 "NORMAL";
                 break;
              case REG_RESOURCE_LIST:
                 "RESOURCELIST";
                 break;
              case REG_FULL_RESOURCE_DESCRIPTOR:
                 "RESOURCEDESC";
                 break;
              case REG_RESOURCE_REQUIREMENTS_LIST:
                 "RESOURCEREQS";
                 break;
              case REG_LINK:
                 "LINK";
                 break;
              case REG_DWORD_BIG_ENDIAN:
                 "BIGENDIAN";
                 break;
              default:
                 "OTHER";
           }
           context->SetStemElement(stem, sname, context->NewStringFromAsciiz(valName));

           sprintf(sname,"%d.DATA", ,ndx);
           if ((valType == REG_MULTI_SZ) ||
               (valType == REG_BINARY) ||
               (valType == REG_LINK) ||
               (valType == REG_RESOURCE_LIST) ||
               (valType == REG_FULL_RESOURCE_DESCRIPTOR) ||
               (valType == REG_RESOURCE_REQUIREMENTS_LIST) ||
               (valType == REG_NONE))
           {
               context->SetStemElement(stem, sname, context->NewString(valData, cbData));
           }
           else
           if ((valType == REG_EXPAND_SZ) || (valType == REG_SZ))
           {
               context->SetStemElement(stem, sname, context->NewStringFromAsciiz(valData));
           }
           else if ((valType == REG_DWORD) || (valType == REG_DWORD_BIG_ENDIAN))
           {
               char tmp[30];
               DWORD dwNumber;

               dwNumber = *(DWORD *) valData;
               if (valType == REG_DWORD_BIG_ENDIAN)
                 Little2BigEndian((BYTE *)&dwNumber, sizeof(dwNumber));
               ltoa(dwNumber, tmp, 10);
               context->SetStemElement(stem, sname, context->NewStringFromAsciiz(tmp));
           }
           else
           {
               context->SetStemElement(stem, sname, context->NewStringFromAsciiz(""));
           }
       }
       else if (retcode != ERROR_NO_MORE_ITEMS)
       {
           GlobalFree(valData);
           return 1;
       }
    } while (retcode == ERROR_SUCCESS);
    GlobalFree(valData);
    return 0;
}

RexxFunction2(uintptr_t, WSConnectRegistryFile, CSTRING, key, CSTRING, file)
{
    HKEY hkResult;
    GET_HKEY(key,  hk);

    return RegConnectRegistry(file, hk, &hkResult) == ERROR_SUCCESS ? (uintptr_t)hkResult : 0;
}

bool checkPrivilege()
{
    if (IsRunningNT())
    {
        HANDLE hToken;              /* handle to process token */
        TOKEN_PRIVILEGES tkp;        /* ptr. to token structure */
        /* set SE_BACKUP_NAME privilege.  */

        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        {
            return false;
        }

        LookupPrivilegeValue(NULL, SE_BACKUP_NAME,&tkp.Privileges[0].Luid);

        tkp.PrivilegeCount = 1;  /* one privilege to set    */
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        /* Set SE_BACKUP_NAME privilege for this process. */

        AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);

        return GetLastError() == ERROR_SUCCESS;
    }
    return true;
}


RexxFunction2(uintptr_t, WSSaveRegistryFile, CSTRING, key, CSTRING, file)
{
    if (!checkPrivilege())
    {
        return GetLastError();
    }

    GET_HKEY(key, hk);
    return RegSaveKey(hk, file, NULL) == ERROR_SUCCESS ? 0 : GetLastError();
}


RexxFunction2(uintptr_t, WSUnloadRegistryFile, CSTRING, key, CSTRING, file)
{
    if (!checkPrivilege())
    {
        return GetLastError();
    }

    GET_HKEY(key, hk);
    return RegUnloadKey(hk, file) == ERROR_SUCCESS ? 0 : GetLastError();
}


RexxFunction3(uintptr_t, WSLoadRegistryFile, CSTRING, key, CSTRING, subkey, CSTRING, file)
{
    if (!checkPrivilege())
    {
        return GetLastError();
    }

    GET_HKEY(key, hk);
    return RegLoadKey(hk, subkey, file) == ERROR_SUCCESS ? 0 : GetLastError();
}


RexxFunction3(uintptr_t, WSRestoreRegistryFile, CSTRING, key, CSTRING, file, CSTRING, volatile)
{
    if (!checkPrivilege())
    {
        return GetLastError();
    }

    GET_HKEY(key, hk);
    DWORD vola = 0;
    if (!strcmp(argv[3].strptr, "VOLATILE"))
    {
        vola = REG_WHOLE_HIVE_VOLATILE;
    }
    return RegRestoreKey(hk, subkey, vola) == ERROR_SUCCESS ? 0 : GetLastError();
}


RexxFunction3(uintptr_t, WSReplaceRegistryFile, CSTRING, key, OPTIONAL_CSTRING, subkey, CSTRING, file, OPTIONAL_CSTRING, backup)
{
    if (!checkPrivilege())
    {
        return GetLastError();
    }

    GET_HKEY(key, hk);
    DWORD vola = 0;
    if (!strcmp(argv[3].strptr, "VOLATILE"))
    {
        vola = REG_WHOLE_HIVE_VOLATILE;
    }
    return RegReplaceKey(hk, subkey, file, backup) == ERROR_SUCCESS ? 0 : GetLastError();
}


HDDEDATA CALLBACK DDECallback(UINT wType,
                              UINT wFmt,
                              HCONV hConv,
                              HSZ hsz1,
                              HSZ hsz2,
                              HDDEDATA hDDEData,
                              DWORD dwData1,
                              DWORD dwData2)
{
    return NULL;
}

bool ProgmanCmd(LPSTR lpszCmd)
{
    DWORD dwDDEInst = 0;
    UINT ui;
    HSZ hszProgman;
    HCONV hConv;
    HDDEDATA hExecData;
    HDDEDATA exRes;



    ui = DdeInitialize(&dwDDEInst,
                       DDECallback,
                       CBF_FAIL_ALLSVRXACTIONS,
                       0l);

    if (ui != DMLERR_NO_ERROR) return false;



    hszProgman = DdeCreateStringHandle(dwDDEInst,
                                       "PROGMAN",
                                       CP_WINANSI);

    hConv = DdeConnect(dwDDEInst,
                       hszProgman,
                       hszProgman,
                       NULL);


    DdeFreeStringHandle(dwDDEInst, hszProgman);

    if (!hConv) return false;


    hExecData = DdeCreateDataHandle(dwDDEInst,
                                    lpszCmd,
                                    lstrlen(lpszCmd)+1,
                                    0,
                                    NULL,
                                    0,
                                    0);


    exRes = DdeClientTransaction((void FAR *)hExecData,
                         (DWORD)-1,
                         hConv,
                         NULL,
                         0,
                         XTYP_EXECUTE,
                         2000, // ms timeout
                         NULL);


    DdeDisconnect(hConv);
    DdeUninitialize(dwDDEInst);

    if (!exRes) return false;
    return true;
}


bool ShowPMGroup(LPSTR lpszGroup, WORD wCmd)
{
    char buf[512];

    if (lstrlen(lpszGroup))
    {
        wsprintf(buf,
                 "[ShowGroup(%s,%u)]",
                 lpszGroup,
                 wCmd);
    }

    return ProgmanCmd(buf);
}


//-----------------------------------------------------------------------------
//
// Function: bool GetCurrentUserDesktopLocation
//
// This function reads the  CurrentUser Desktop Path from the registry.
//
// Syntax:   call GetCurrentUserDesktopLocation( LPBYTE szDesktopDir, LPDWORD  lpcbData )
//
// Params:
//
//  szDesktopDir   - Drive and Path of the Desktop to be returned
//  lpcbData       _ Max Size of szDesktopDir on entry, Size of szDesktopDir on exit
//
//   return :  true  - No error
//             false - Error
//-----------------------------------------------------------------------------
#define IDS_REGISTRY_KEY_CURRENT_SHELLFOLDER "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
#define IDS_CURRENT_DESKTOP                  "Desktop"

bool GetCurrentUserDesktopLocation ( LPBYTE szDesktopDir, LPDWORD  lpcbData )
{


   HKEY hKey;                      //handle of key
   long rc;
   DWORD Type ;

   szDesktopDir[0] ='\0';         //initialize return

   if ( (rc = RegOpenKeyEx(HKEY_CURRENT_USER,
                           IDS_REGISTRY_KEY_CURRENT_SHELLFOLDER,
                           0,
                           KEY_QUERY_VALUE,
                           &hKey)) == ERROR_SUCCESS )
   {
      if ( (rc = RegQueryValueEx(hKey,                      // handle of key to query
                           IDS_CURRENT_DESKTOP ,            // address of name of value to query
                           NULL,                            // reserved
                           &Type,                         // address of buffer for value type
                           szDesktopDir ,                   // address of returned data
                           lpcbData)) == ERROR_SUCCESS )    // .. returned here
      {
        RegCloseKey ( hKey ) ;
        return true ;
      }
      RegCloseKey ( hKey ) ;
   }

   // Error occured
   return false ;

}
//-----------------------------------------------------------------------------
//
// Function: bool GetAllUserDesktopLocation
//
// This function reads All UsersDesktop Path from the registry.
//
// Syntax:   call GetAllUserDesktopLocation( LPBYTE szDesktopDir, LPDWORD  lpcbData )
//
// Params:
//
//  szDesktopDir   - Drive and Path of the Desktop to be returned
//  lpcbData       _ Max Size of szDesktopDir on entry, Size of szDesktopDir on exit
//
//   return :  true  - No error
//             false - Error
//-----------------------------------------------------------------------------
#define IDS_REGISTRY_KEY_ALL_NT_SHELLFOLDER "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
#define IDS_ALL_NT_DESKTOP                  "Common Desktop"

#define IDS_REGISTRY_KEY_ALL_9x_SHELLFOLDER ".DEFAULT\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
#define IDS_ALL_9x_DESKTOP                  "Desktop"


bool GetAllUserDesktopLocation ( LPBYTE szDesktopDir, LPDWORD  lpcbData )
{


   HKEY hKey;                      //handle of key
   long rc;
   DWORD lpType ;
   LPTSTR lpValueName ;

   szDesktopDir[0] ='\0';         //initialize return

   // Test, if 95/98/Millenium or NT/Win2000
   if ( IsRunningNT() )
   {
     rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                       IDS_REGISTRY_KEY_ALL_NT_SHELLFOLDER,
                       0,
                       KEY_QUERY_VALUE,
                       &hKey) ;
     lpValueName = IDS_ALL_NT_DESKTOP ;
   }
   else
   {
     rc = RegOpenKeyEx(HKEY_USERS,
                       IDS_REGISTRY_KEY_ALL_9x_SHELLFOLDER,
                       0,
                       KEY_QUERY_VALUE,
                       &hKey) ;
     lpValueName = IDS_ALL_9x_DESKTOP ;
   }
   if ( rc == ERROR_SUCCESS )
   {
      if ( (rc = RegQueryValueEx(hKey,                      // handle of key to query
                                 lpValueName ,                    // address of name of value to query
                                 NULL,                            // reserved
                                 &lpType,                         // address of buffer for value type
                                 szDesktopDir ,                   // address of returned data
                                 lpcbData)) == ERROR_SUCCESS )    // .. returned here
      {
        RegCloseKey ( hKey ) ;
        return true ;
      }
      RegCloseKey ( hKey ) ;
   }

   // Error occured
   return false ;

}

RexxFunction2(int, WSProgManagerAddGroup, CSTRING, group, OPTIONAL_CSTRING, path)
{
    char buf[1024];

    if (path == NULL || strlen(path_ == 0)
    {
        wsprintf(buf, "[CreateGroup(%s,%s)]", group, path);
    }
    else
    {
        wsprintf(buf, "[CreateGroup(%s)]", group);
    }

    return ProgmanCmd(buf);
}


RexxFunction1(int, WSProgManagerDeleteGroup, CSTRING, group)
{
    char buf[512];

    if (strlen(group) > 0)
    {
        wsprintf(buf, "[DeleteGroup(%s)]", group);
    }

    return ProgmanCmd(buf);
}


RexxFunction1(int, WSProgManagerRestoreGroup, CSTRING, group)
{
    return !ShowPMGroup(group, 1);
}


RexxFunction1(int, WSProgManagerMinimizeGroup, CSTRING, group)
{
    return !ShowPMGroup(group, 2);
}


RexxFunction1(int, WSProgManagerMaximizeGroup, CSTRING, group)
{
    return !ShowPMGroup(group, 3);
}

RexxFunction9(int, WSProgManagerAddItem, CSTRING, cmdLine, CSTRING, caption, OPTIONAL_CSTRING, iconPath,
    OPTIONAL_int, iconIndex, CSTRING, dir, RexxBoolean, last, OPTIONAL_int, hotkey, OPTIONAL_int, modifier,
    OPTIONAL_RexxBoolean, bMin)
{
    char buf[2048];
    int Pos = last ? -1 : 0;

    if (iconPath != NULL && strlen(iconPath) != 0)
    {
        wsprintf(buf,
                 "[AddItem(%s,%s,%s,%u,%d,%d,%s,%d,%d)]",
                 cmdLine,
                 caption,
                 iconPath,
                 iconIndex,
                 Pos,Pos,
                 dir,
                 MAKEWORD( hotkey, modifier),
                 bMin);
    }
    else
    {
        wsprintf(buf,
                 "[AddItem(%s,%s,"","",%d,%d,%s,%d,%d)]",
                 cmdLine,
                 caption,
                 Pos, Pos,
                 dir,
                 MAKEWORD( hotkey, modifier),
                 bMin);
    }

    return ProgmanCmd(buf);
}

RexxFunction1(int, WSProgManagerDeleteItem, CSTRING, item)
{
    char buf[512];

    wsprintf(buf, "[DeleteItem(%s)]", item);
    return ProgmanCmd(buf);
}

RexxFunction1(int, WSProgManagerLeave, CSTRING, option)
{
    char buf[256];

    wsprintf(buf, "[ExitProgman(%u)]", stricmp(option, "SAVE") == 0 ? 1 : 0);
    return ProgmanCmd(buf);
}

RexxFunction10(int, WSProgManagerAddDesktopIcon, CSTRING, name, CSTRING, program, CSTRING, icon,
    int, iconIndex, CSTRING, workDir, CSTRING, location, CSTRING, arguments, int, hotkey, int, modifier, CSTRING, run)
{
   HRESULT      hres;
   IShellLink*  psl;
   bool         bRc = true;
   int          iRun = SW_NORMAL;

   CoInitialize(NULL);

   // Get a pointer to the IShellLink interface.
   hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, &psl);

   if (SUCCEEDED(hres))
   {
      IPersistFile* ppf;

      // Set the path to the shortcut target
      psl->lpVtbl->SetPath(psl, program);

      // icon location. default of iIconIndex is 0, set in WINSYSTM.CLS
      psl->lpVtbl->SetIconLocation(psl, icon, iconIndex);

      // command-line arguments
      psl->lpVtbl->SetArguments( psl, arguments );

      //shortcut key, the conversion to hex is done in WINSYSTM.CLS
      // modificationflag:
      // The modifier flags can be a combination of the following values:
      // HOTKEYF_SHIFT   = SHIFT key      0x01
      // HOTKEYF_CONTROL = CTRL key       0x02
      // HOTKEYF_ALT     = ALT key        0x04
      // HOTKEYF_EXT     = Extended key   0x08
      psl->lpVtbl->SetHotkey(psl, MAKEWORD( hotkey, modifier));

      // working directory
      psl->lpVtbl->SetWorkingDirectory( psl, workDir );

      // run in normal, maximized , minimized window, default is NORMAL, set in WINSYSTM.CLS
      if ( !stricmp(run, "MAXIMIZED") )
         iRun = SW_SHOWMAXIMIZED;
      else
      if ( !stricmp(run, "MINIMIZED") )
      {
         iRun = SW_SHOWMINNOACTIVE;
      }

      psl->lpVtbl->SetShowCmd( psl, iRun );

      // Query IShellLink for the IPersistFile interface for saving the
      // shortcut in persistent storage.
      hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile, &ppf);

      if (SUCCEEDED(hres))
      {
         WCHAR wsz[MAX_PATH];
         CHAR  szShortCutName[MAX_PATH];
         CHAR  szDesktopDir[MAX_PATH];
         DWORD dwSize = MAX_PATH;

        // If strlen(location) is < 6, then name contains a full qualified filename
        if (strlen(location) > 5)
        {
           // if icon should only be created on the desktop of current user
           // get current user
           if (!stricmp(location, "PERSONAL"))
           {
             bRc = GetCurrentUserDesktopLocation ( szDesktopDir , &dwSize ) ;
           }
           else
           {  // Location is COMMON
             bRc = GetAllUserDesktopLocation ( szDesktopDir , &dwSize ) ;
           }

           if ( bRc)
           {
             //.lnk must be added to identify the file as a shortcut
             sprintf( szShortCutName, "%s\\%s.lnk", szDesktopDir, name);
           }
        }
        else   /* empty specifier so it's a link */
        {
          sprintf( szShortCutName, "%s.lnk", name); /* lpszName contains a full qualified filename */
        }

        // Continueonly, if bRC is true
        if ( bRc )
        {

          // Ensure that the string is Unicode.
          MultiByteToWideChar(CP_ACP, 0, szShortCutName, -1, wsz, MAX_PATH);

          // Save the link by calling IPersistFile::Save.
          hres = ppf->lpVtbl->Save(ppf, wsz, true);
          if (!SUCCEEDED(hres))
            bRc = false;

          ppf->lpVtbl->Release(ppf);
        }
        else
          bRc = false;

      }
      else
        bRc = false;

      psl->lpVtbl->Release(psl);

   }
   else
      bRc = false;

   return bRc;
}


RexxFunction1(int, WSProgManagerDeleteDesktopIcon, CSTRING, name, CSTRING, location)
{
   CHAR  szDesktopDir[MAX_PATH];
   CHAR  szShortCutName[MAX_PATH];
   DWORD dwSize = MAX_PATH;

   // get the location (directory) of the shortcut file in dependency of
   // the specified location
   if (!stricmp(location, "PERSONAL"))
   {
      GetCurrentUserDesktopLocation ( szDesktopDir , &dwSize );
   }
   else
   {  // Location is COMMON
      GetAllUserDesktopLocation ( szDesktopDir , &dwSize );
   }

   //.lnk must be added to identify the file as a shortcut
   sprintf( szShortCutName, "%s\\%s.lnk", szDesktopDir, name);

   return !DeleteFile(szShortCutName) ? GetLastError() : 0;
}


#define WSFTS 9
CHAR * WindowsSystemFuncTab[WSFTS] = {\
                     "WSRegistryKey", \
                     "WSRegistryValue", \
                     "WSRegistryFile", \
                     "WSProgManager", \
                     "WSEventLog", \
                     "WSCtrlWindow",
                     "WSCtrlSend",
                     "WSCtrlMenu",
                     "WSClipboard"
                     };


ULONG APIENTRY RemoveWinSysFuncs(
  PUCHAR funcname,
  ULONG argc,
  RXSTRING argv[],
  PUCHAR qname,
  PRXSTRING retstr )

{
   INT rc, i;
   bool err = false;

   for (i=0;i<WSFTS;i++)
   {
      rc = RexxDeregisterFunction(WindowsSystemFuncTab[i]);
      if (rc) err = true;
   }
   rc = RexxDeregisterFunction("RemoveWinSysFuncs");
   if (rc) err = true;
   rc = RexxDeregisterFunction("InstWinSysFuncs");
   if (rc) err = true;
   if (!err)
      RETC(0)
   else
      RETC(1)

}



ULONG APIENTRY InstWinSysFuncs(
  PUCHAR funcname,
  ULONG argc,
  RXSTRING argv[],
  PUCHAR qname,
  PRXSTRING retstr )

{
   INT rc, i;
   bool err = false;
   retstr->strlength = 1;

   rc = RexxRegisterFunctionDll(
     "RemoveWinSysFuncs",
     WINSYSDLL,
     "RemoveWinSysFuncs");

   if (rc) err = true;

   for (i=0;i<WSFTS;i++)
   {
      rc = RexxRegisterFunctionDll(WindowsSystemFuncTab[i],WINSYSDLL,WindowsSystemFuncTab[i]);
      if (rc) err = true;
   }

   if (err)
      RETC(1)
   else
      RETC(0)
}


//Functions for reading Event Log

//-----------------------------------------------------------------------------
//
//  Function GetEvPaMessageFile
//
//  Gets the name of the message file from the registry
//
//  char * GetEvPaMessageFile(char * pMessageFile, char * pchSource,char * pchSourceName, char * chMessageFile)
//
//       pMessaegFile  - (in) What to get
//                            "EventMessageFile"     get event message file name
//                            "ParameterMessageFile" get parameter message file
//
//       pchSource     - (in) second part of the key to build
//
//       pchSourceName - (in) third part of the key to build
//
//       chMessageFile - (in/out) name of the message file
//                                string buffer with length MAX_PATH
//
//-----------------------------------------------------------------------------
void GetEvPaMessageFile(char * pMessageFile, char * pchSource,char * pchSourceName, char * chMessageFile)
{
   DWORD dataSize;                 //size of the message file name returned from RegQueryValueEx
   DWORD valType = REG_EXPAND_SZ;  //type for call to RegQueryValueEx
   char chKey[] = "SYSTEM\\CurrentControlSet\\Services\\EventLog\\";  //first part of key for message files
   char *pchKey;                   //contains name of complete key
   char *valData;                  //value returned from RegQueryValueEx
   HKEY hKey;                      //handle of key
   int rc;

   chMessageFile[0] ='\0';         //initialize return

   //build the complete key --> first part + second part from input + third part from input
   pchKey = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, strlen(chKey) + strlen(pchSource) + 1 + strlen(pchSourceName) +1);
   sprintf(pchKey, "%s%s\\%s", chKey, pchSource, pchSourceName);

   if ( (rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     pchKey,
                     0,
                     KEY_QUERY_VALUE,
                     &hKey)) == ERROR_SUCCESS )
   {
      if ( (rc = RegQueryValueEx(hKey,                      // handle of key to query
                           pMessageFile,                  // address of name of value to query
                           NULL,                            // reserved
                           &valType,                      // address of buffer for value type
                           NULL,                            // NULL to get the size ...
                           &dataSize)) == ERROR_SUCCESS )// .. returned here
      {
         //no error getting the size of message file name, allocate buffer and get the value
         valData = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, dataSize);

         if ( (rc = RegQueryValueEx(hKey,                   // handle of key to query
                              pMessageFile,                // address of name of value to query
                              NULL,                         // reserved
                              &valType,                   // address of buffer for value type
                              valData,                      // address of buffer for value data
                              &dataSize)) == ERROR_SUCCESS )
         {
            ExpandEnvironmentStrings(valData,chMessageFile,MAX_PATH);
         }
         GlobalFree(valData);
      }
   }
   GlobalFree(pchKey);
}

//-----------------------------------------------------------------------------
//
//  Function SearchMessage
//
//  Load message files.
//  Search the message in message files, format and return the message
//
//  bool SearchMessage(char * chFileNames, DWORD dwMessageID, char ** pchInsertArray, LPVOID *lpMsgBuf )
//
//       chFileNames    - (in) filenames, separated by semicolons
//       dwMessageID    - (in) message id to be searched
//       pchInsertArray - (in) array with replacement strings for message formatting
//       lpMsgBuf       - (out) buffer containing the formatted message
//
//  return : 1 - message found and returned in lpMsgBuf
//           0 -  message not found or other error
//-----------------------------------------------------------------------------

bool SearchMessage(char * chFileNames, DWORD dwMessageID, char ** pchInsertArray, LPVOID *lpMsgBuf )
{
   HINSTANCE hInstance = 0;   //return value
   char *    pBuffer=0;       //buffer where chFileNames is copied to
   char *    pTemp=0;         //pointers for parsing ...
   char *    pNext=0;         //the file names
   int       rc;
   bool      fRc=0;

   if (chFileNames[0] != '\0')
   {
      pBuffer = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, strlen(chFileNames)+1);
      strcpy(pBuffer,chFileNames);
      pTemp = pBuffer;

      while (pTemp != 0 && fRc != 1)
      {
         pNext  = strchr(pTemp,';');
         if (pNext != NULL )
         {
            *pNext = '\0';
            pNext++;
         }

         if ( (hInstance = LoadLibrary(pTemp)) != 0 )
         {
            rc = FormatMessage(
                  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                  hInstance,                                 //message file handle
                  dwMessageID,                               //message id
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                  (LPTSTR)lpMsgBuf,                          //output for formatted message
                  0,                                         //not needed for FORMAT_MESSAGE_ALLOCATE_BUFFER
                  pchInsertArray );                          //replacement strings

            if ( rc != 0 ) //no error
               fRc = 1;

            FreeLibrary(hInstance);
            hInstance = 0;
         }
         pTemp = pNext;
      }
      if (pBuffer)
         GlobalFree(pBuffer);
   }
   return fRc;
}

//-----------------------------------------------------------------------------
//
//  Function BuildMessage
//
//  Extracts the user name from an event log record
//
//  void BuildMessager(PEVENTLOGRECORD pEvLogRecord, char * pchSource, char ** pMessage)
//
//       pEvLogRecord - (in)event log record
//       pchSource    - (in)source within event log record
//       pMessage     - (out)formatted message
//
//-----------------------------------------------------------------------------
void BuildMessage(PEVENTLOGRECORD pEvLogRecord , char * pchSource, char ** pMessage )
{
   char * pchString = (char*)0;      //pointer to replacement string within event log record
   int    iStringLen = 0;            //accumulation of the string length
   int    i, rc;

   HINSTANCE hInstance=0;             //handle for message files
   LPVOID    lpMsgBuf=0;              //buffer to format the string
   char      *pchInsertArray[100];    //pointer array to replacement strings
   char      chMessageFile[MAX_PATH]; //name of message file
   char      *pchPercent;             //pointer to "%%" within a replacement string

   //initialize insert array
   memset(pchInsertArray,0,sizeof(pchInsertArray));

   //point to first replacement string
   pchString = (char*)pEvLogRecord + pEvLogRecord->StringOffset;

   //loop over all replacement strings, substitute "%%" and build an array of replacement strings
   for (i=0; i < pEvLogRecord->NumStrings ; i++)
   {
      //If a replacement string contains "%%", the name of the parameter message file must be
      //read from the registry log. "%%" is followed by an id which is the id of parameter strings
      //to be loaded from the parameter message file
      if ( (pchPercent = strstr(pchString,"%%")) )
      {
         //the replacement strings contains placeholder for parameters
         //get name of parameter message file from registry
         GetEvPaMessageFile("ParameterMessageFile",pchSource,(char *)pEvLogRecord + sizeof(EVENTLOGRECORD),chMessageFile);

         //load strings from the parameter message file
         if ( (hInstance = LoadLibrary(chMessageFile)) != 0 )
         {
            //load the parameters to be inserted into the replacement string
            rc = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
                                hInstance,                                 //handle to message file
                                atoi(pchPercent+2),                        //id of message to be loaded
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                                (LPTSTR)&pchInsertArray[i],
                                0,
                                0 );

            if (rc == 0) //error getting strings
            {
               //use empty string
               pchInsertArray[i] = LocalAlloc(LMEM_FIXED,1);
               *pchInsertArray[i] ='\0';
            }
            FreeLibrary(hInstance);
            hInstance =0;
         }
         else
         {
            //parameter message file could not be loaded, use empty string
            pchInsertArray[i] = LocalAlloc(LMEM_FIXED,1);
            *pchInsertArray[i] ='\0';
         }
         //accumulate sum of string length
         iStringLen += strlen(pchInsertArray[i])+1;
      }
      else
      {
         //the replacement strings contains NO placeholder for parameters, use them as is
         iStringLen += strlen(pchString)+1;
         pchInsertArray[i] = LocalAlloc(LMEM_FIXED,strlen(pchString)+1);
         strcpy(pchInsertArray[i],pchString);
      }
      //point to next replacement string within log record
      pchString += strlen(pchString)+1;
   }

   //get name of event log  message file from registry
   GetEvPaMessageFile("EventMessageFile",pchSource,(char *)pEvLogRecord + sizeof(EVENTLOGRECORD),chMessageFile);

   //Search the message in the all event message files
   rc = SearchMessage(chMessageFile,pEvLogRecord->EventID,pchInsertArray,&lpMsgBuf);

   if (rc == 0) //no message found or any other error
   {
      //use replacement strings for the message if available
      if (pEvLogRecord->NumStrings)
      {
         *pMessage = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, iStringLen+1);

         for (i=0;i < pEvLogRecord->NumStrings;i++)
         {
            strcat(*pMessage,pchInsertArray[i]);
            strcat(*pMessage," ");
         }
      }
      else //return empty string
      {
         *pMessage    = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, 1);
         *pMessage[0] = '\0';
      }
   }
   else
   {
      //message formatted return it
      *pMessage = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, strlen((char*)lpMsgBuf) + 1);
      strcpy(*pMessage,lpMsgBuf);
      //lpMsgBuf must be given free with LocalFree
      LocalFree(lpMsgBuf);
   }

   //free replacement strings
   for (i=0; i < pEvLogRecord->NumStrings ; i++)
   {
      LocalFree(pchInsertArray[i]);
   }
}

//-----------------------------------------------------------------------------
//
//  Function GetUser
//
//  Extracts the user name from an event log record
//
//  char * GetUser(PEVENTLOGRECORD pEvLogRecord)
//
//       pEvLogRecord - (in)pointer to event log record
//
//  return  char * - pointer to user name (account) from event log record or
//                   pointer to "N/A" when no user available or in error case
//                   (pointer is allocated from heap)
//                   must be freed vy caller
//
//-----------------------------------------------------------------------------
char * GetUser(PEVENTLOGRECORD pEvLogRecord)
{
   SID   *psid;                           //points to security structure withi event log record
   char * pchUserId=0;                    //returned userid, from heap, chDefUserId in any error case
                                          //or when no userid available
   ULONG sizeId=0;                        //0 to get the size of the uder id
   char   chDefUserId[] = "N/A";          //default userid
   int    rc;
   SID_NAME_USE strDummy;                 //dummies needed for call to ...
   char  *pchDummy=0;                     //LookupAccontSid ...
   DWORD sizeDummy=0;                     //but not subject of interest


   if (pEvLogRecord->UserSidLength != 0) //no SID record avaialable return default userid
   {
      //get the SID record within event log record
      psid = (SID *)((char*)pEvLogRecord + pEvLogRecord->UserSidOffset);

      //get the size of the pchUserIid and pchDummy
      rc = LookupAccountSid(NULL,        // address of string for system name
                            psid,        // address of security identifier
                            pchUserId,   // address of string for account name
                            &sizeId,     // address of size account string
                            pchDummy,    // address of string for referenced domain (not needed)
                            &sizeDummy,  // address of size domain string
                            &strDummy ); // address of structure for SID type
      if (rc == 0) //error
      {
         if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) //expected because sizes are set to 0
         {
            rc = max(sizeId,strlen(chDefUserId)+1);
            pchUserId = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, max(sizeId,strlen(chDefUserId)+1));
            pchDummy = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, sizeDummy);
            //now get it ...
            rc = LookupAccountSid(NULL,                    // address of string for system name
                                  psid,                    // address of security identifier
                                  pchUserId,               // address of string for account name
                                  &sizeId,                 // address of size account string
                                  pchDummy,                // address of string for referenced domain
                                  &sizeDummy,              // address of size domain string
                                  &strDummy );                 // address of structure for SID type
            GlobalFree(pchDummy); //don't want this
            //error: return default
            if (rc == 0) strcpy(pchUserId,chDefUserId);
         }
      }
   }

   //if not already set return default
   if (pchUserId == (char*)0)
   {
      pchUserId = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, strlen(chDefUserId)+1);
      strcpy(pchUserId,chDefUserId);
   }
   return pchUserId;
}

//-----------------------------------------------------------------------------
//
//  Function GetEvData
//
//  Extracts the data  name from an event log record
//
//  char * GetEvData(PEVENTLOGRECORD pEvLogRecord, char ** pchData)
//
//       pEvLogRecord - (in)pointer to event log record
//
//       pchData      - (in/out) data, allocated from heap
//                               empty string when no data
//
//-----------------------------------------------------------------------------
void GetEvData(PEVENTLOGRECORD pEvLogRecord, char ** pchData)
{
    unsigned char * puchAct;
    char            pTemp[3];
    DWORD           i;

    if (pEvLogRecord->DataLength != 0)
    {
       *pchData = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,(pEvLogRecord->DataLength + 1) * 2);

       puchAct =  (char*)pEvLogRecord + pEvLogRecord->DataOffset;

       for ( i=0; i<pEvLogRecord->DataLength; i++)
       {
         sprintf(pTemp, "%02x", *puchAct);
         strcat(*pchData, pTemp);
         puchAct++;
       }
    }
    else
      *pchData    = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, 1);
}

//-----------------------------------------------------------------------------
//
// Function: WSEventLog
//
// This function reads the contents of the event log file. Each event log record is
// returned in a stem.# variable.
//
//                            0        1          2         3       4         5         6       7     8
// Syntax:   call WSEventLog( access, [options], [hEvent], [stem], [server], [source], [start],[num],[backupFileName]
//                            9     10        11  12    13
//                            type, category, id, data, strings
//                          )
//
// Params:
//
//   access - What to do with the event log
//            'OPEN'    returns event log handle
//                      0 in error case
//            'CLOSE'
//            'READ'    returns a stem containing the records
//            'NUM'     returns number of record or 0 if no records available or error
//            'CLEAR"   clear an backup the event log ........................
//
//   options - 'FORWARDS'       - The log is read in forward chronological order. (default)
//                              (oldest first)
//             'BACKWARDS'      - The log is read in backward chronological order.
//                              (youngest first)
//
//   hEvent -  eventlog handle, returned when eventlog is opened before
//
//   stem      Name of stem variable to store results in.
//             stem.0 contains the number of returned records
//
//   server -  Universal Naming Convention (UNC) name of the server on which the
//             event log is to be opened. If this parameter is not spcified,
//             the log is opened on the local computer.
//
//   source -  Name of the source. The source name must be a subkey of a logfile
//             entry under the EventLog key in the registry.
//             'System'       - system log
//             'Security'     - security log
//             'Application'  - application log (default if source is empty or when source not found
//
//   start  -  Record number of event log record to start. The oldest record is always the first record !!!
//             (independent if BACKEWRDS or FORWARDS is specified)
//
//   num  -    Number of event log record to read
//
//   backupFileName - if specified togetherr with 'CLEAR' the event log is backed up
//                    before it is cleared
//
//-----------------------------------------------------------------------------

RexxFunction1 (int, WSCloseEventLog, int, handle)
{
    return CloseEventLog((HANDLE)handle) ? 0 : 1;
}

RexxFunction2 (uintptr_t, WSOpenEventLog, CSTRING, server, CSTRING, source)
{
    if (strlen(server) == 0)
    {
        server = NULL;
    }

    if (strlen(source) == 0)
    {
        source = "Application";
    }
    return OpenEventLog(server, source);
}

RexxFunction5 (int, WSReadEventLog, uintptr_t, logHandle, CSTRING, option, RexxStemObject, stem,
    uint32_t, start, int32_t, num)
{
    DWORD  bufSize = 1024;               //initial size for event log buffer
    Handle eEventLog = (HANDLE)logHandle;
    dwReadFlags = EVENTLOG_FORWARDS_READ; //default
    //no handle as input, so the event log must be opened
    if (hEventLog == 0)
    {
        context->InvalidRoutine();
        return 0;
    }
    //check parameter options
    if (strlen(option) != 0)
    {
        if (stricmp(option,"FORWARDS") == 0)
        {
            dwReadFlags = EVENTLOG_FORWARDS_READ; //default
        }
        else if (stricmp(argv[1].strptr,"BACKWARDS") == 0)
        {
            dwReadFlags = EVENTLOG_BACKWARDS_READ;
        }
        else
        {
            context->InvalidRoutine();
            return 0;
        }
    }

    bufSize = 64 * bufSize; //assume that when complete log should be read (64K)

    if (start == 1 && num =- -1)
    {
        dwReadFlags |= EVENTLOG_SEQUENTIAL_READ;
    }
    else
    {
        //start and num contains values
        dwReadFlags |= EVENTLOG_SEEK_READ;
        bufSize = num * bufSize;
    }

    PEVENTLOGRECORD pEvLogRecord;        //pointer to one event record
    PVOID  pBuffer = 0;                  //buffer for event records
    DWORD  pnBytesRead;                  //returned from ReadEventlog
    DWORD  pnMinNumberOfBytesNeeded;     //returned from ReadEventlog if not all read
    char * pchEventSource  = (char *)0;  //source from event log record
    char * pchComputerName = (char *)0;  //computer name of event
    char * pchMessage=0;                 //text of detail string
    char * pchData=0;                    //data of event log record
    char   time[MAX_TIME_DATE];          //converted time of event
    char   date[MAX_TIME_DATE];          //converted date of event
    struct tm * DateTime;                //converted from elapsed seconds
    char * pchStrBuf = (char *)0;        //temp buffer for event string
    DWORD  bufferPos=0;                  //position within the eventlog buffer
    bool   cont = 1;                     //continue flag for while loop
    char   evType[5][12]={"Error","Warning","Information","Success","Failure"}; //event type strings
    int    evTypeIndex;
    char * pchUser;
    ULONG  count;                        //number of event log records retuned in stem


    GetNumberOfEventLogRecords(hEventLog,&numEvRecords);
    if (start > numEvRecords)
    {
        return 1;
    }

    pBuffer = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, bufSize);
    count = 0;

    while (cont)
    {
        while ( (rc = ReadEventLog(hEventLog,
            dwReadFlags,
            start,
            pBuffer,
            bufSize,
            &pnBytesRead,
            &pnMinNumberOfBytesNeeded) ) && (count < num) )
        {
            pEvLogRecord = pBuffer;
            bufferPos = 0;

            while ( (bufferPos < pnBytesRead) && (count < num) )
            {
                if (dwReadFlags & EVENTLOG_FORWARDS_READ)
                {
                    start = pEvLogRecord->RecordNumber+1;
                }
                else
                {
                    start = pEvLogRecord->RecordNumber-1;
                }
                count++; //count number of events

                //get index to event type string
                GET_TYPE_INDEX(pEvLogRecord->EventType, evTypeIndex);

                //get time and date
                //DateTime = localtime(&pEvLogRecord->TimeGenerated);   // convert to local time
                DateTime = localtime(&pEvLogRecord->TimeWritten);   // convert to local time
                strftime(date, MAX_TIME_DATE,"%x", DateTime);
                strftime(time, MAX_TIME_DATE,"%X", DateTime);

                pchEventSource  = (char*)pEvLogRecord + sizeof(EVENTLOGRECORD);
                pchComputerName = pchEventSource + strlen(pchEventSource)+1;

                pchUser = GetUser(pEvLogRecord);

                BuildMessage(pEvLogRecord,pchSource,&pchMessage);

                GetEvData(pEvLogRecord,&pchData);

                pchStrBuf = GlobalAlloc(GMEM_FIXED,
                    strlen(evType[evTypeIndex])+1+
                    strlen(date)+1+
                    strlen(time)+1+
                    strlen(pchEventSource)+3+
                    12+
                    strlen(pchUser)+1+
                    strlen(pchComputerName)+1+
                    strlen(pchMessage)+3+
                    strlen(pchData)+3+10);

                //Type Date Time Source Id User Computer Details
                sprintf( pchStrBuf,
                    "%s %s %s '%s' %u %s %s '%s' '%s'",
                    evType[evTypeIndex],
                    date,
                    time,
                    pchEventSource,
                    LOWORD(pEvLogRecord->EventID),
                    pchUser,
                    pchComputerName,
                    pchMessage,
                    pchData);

                GlobalFree(pchMessage);
                GlobalFree(pchData);
                pchMessage = 0;
                GlobalFree(pchUser);

                context->SetStemArrayElement(stem, count, context->NewString(pchStrBuf, strlen(pchStrBuf)));
                bufferPos += pEvLogRecord->Length;

                GlobalFree(pchStrBuf);

                //position to next event record
                pEvLogRecord = (PEVENTLOGRECORD)(((char*)pEvLogRecord) + pEvLogRecord->Length);
            }
            memset(pBuffer,0,bufSize);
        }
        rc = GetLastError();
        // if buffer is to small, reallocate it, pnMinNumberOfBytesNeeded is returned from ReadEventLog

        if ( (count == num) || (start > numEvRecords) || (start < 1) )
            cont = 0;
        else
        {
            if (rc == ERROR_INSUFFICIENT_BUFFER)
            {
                bufSize += pnMinNumberOfBytesNeeded;
                pBuffer = GlobalReAlloc(pBuffer,bufSize,GMEM_FIXED|GMEM_ZEROINIT);
            }
            else
                cont = 0; //stop reading, reached end or error (except ERROR_INSUFFICIENT_BUFFER)
        }//while (cont)

        if (pBuffer) GlobalFree(pBuffer);

        if ( rc == ERROR_HANDLE_EOF || count == num || (start > numEvRecords) || (start < 1) ) //all OK complete log read, also true for an empty log{
        {
            context->SetStemArrayElement(stem, 0, context->NumberToObject(count));
            return 0;
        }
    }
    return 1;
}

RexxFunction7 (int, WSWriteEventLog, CSTRING, server, CSTRING, source, int, type, int, category, int, id, RexxStringObject, data, RexxArrayObject, extras)
{
    WORD     wEventType       = type;
    WORD     wEventCategory   = category;
    DWORD    dwEventId        = id;
    LPVOID   lpData           = (void *)0;
    DWORD    dwDataSize       = 0;
    WORD     wNumStrings      = 0;
    char      *lpStrings[100];
    HANDLE   hEventLog;

    if (strlen(server) == 0)
    {
        server = NULL;
    }

    if (strlen(source) == 0)
    {
        source = "Application";
    }

    //check and prepare event log data

    // check the event type (initialized to 1 = default)
    // EVENTLOG_SUCCESS                   0X0000
    // EVENTLOG_ERROR_TYPE                0x0001
    // EVENTLOG_WARNING_TYPE              0x0002
    // EVENTLOG_INFORMATION_TYPE         0x0004
    // EVENTLOG_AUDIT_SUCCESS            0x0008
    // EVENTLOG_AUDIT_FAILURE           0x0010

    dwDataSize = context->StringLength(data);

    lpData = context->StringValue(data);
    wNumStrings = context->ArraySize(extras);

    DWORD i;
    for ( i = 1; i < wNumStrings; i++)
    {
       //count the number of strings and create the string array
       lpStrings[i - 1] = context->ObjectToStringValue(context->ArrayAt(extras, i));
    }

    if ( (hEventLog = RegisterEventSource(server, source)) == NULL )
       rcGetLastError = GetLastError();

    if ( !rcGetLastError )
    {
       //write the log
       if ( ReportEvent( hEventLog,
                         wEventType,
                         wEventCategory,
                         dwEventId,
                         NULL,
                         wNumStrings,
                         dwDataSize,
                         lpStrings,
                         lpData) == NULL ) rcGetLastError = GetLastError();

       //deregister the event source
       DeregisterEventSource( hEventLog);
    }

    return rcGetLastError;
}


RexxFunction2(uintptr_t, WSFindChildWindow, uintptr_t, handle, CSTRING, name)
{
    return FindWindowEx((HWND)handle, NULL, NULL, name);
}


RexxFunction1(uintptr_t, WSFindWindow, CSTRING, name)
{
    return FindWindow(NULL, name);
}


RexxFunction1(uintptr_t, WSSetForegroundWindow, uintptr_t, handle)
{
    HWND hW =  GetForegroundWindow();
    if (hW != (HWND)handle)
    {
        SetForegroundWindow((HWND)handle);
    }
    return hW;
}


RexxFunction0(uintptr_t, WSGetForegroundWindow)
{
    return GetForegroundWindow();
}


RexxFunction2(int, WSSetWindowTitle, uintptr_t, handle, CSTRING, title)
{
    if (handle == 0)
    {
        return !SetConsoleTitle(title);

    }
    return !SetWindowText((HWND)handle, title)
}


RexxFunction1(RexxStringObject, WSGetWindowTitle, uintptr_t, handle)
{
    if (handle == 0)
    {
        char buffer[256];
        size_t length = GetConsoleTitle(buffer, sizeof(buffer));

        return context->NewString(buffer, length);
    }

    // GetWindowText do not work for controls from other processes and for listboxes
    // Now using WM_GETTEXT which works for all controls.
    // The return string was limitted to 255 characters, now it's unlimited.

    // at first get the text length to determine if the default lenght (255)of strptr would be exceeded.
    lResult = SendMessageTimeout(hW, WM_GETTEXTLENGTH ,0, 0, MSG_TIMEOUT_OPTS, MSG_TIMEOUT, &dwResult);
    // time out or error?
    if (lResult == 0)
    {
        return context->NewStringFromAsciiz("");
    }
    DWORD bufferLength = dwResult + 1;

    tmp_ptr = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, bufferLength);
    if (!tmp_ptr)
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }
    // now get the text
    lResult = SendMessageTimeout(hW, WM_GETTEXT, dwResult + 1, (LPARAM)tmp_ptr,
        MSG_TIMEOUT_OPTS, MSG_TIMEOUT, &dwResult);
    // time out or error?
    if (lResult == 0)
    {
        GlobalFree(tmp_ptr);
        return context->NewStringFromAsciiz("");
    }

    // if still no text found, the control may be a listbox, for which LB_GETTEXT must be used
    if ( dwResult == 0 )
    {
        // at first get the selected item
        int curSel;
        lResult = SendMessageTimeout(hW, LB_GETCURSEL, 0, 0, MSG_TIMEOUT_OPTS, MSG_TIMEOUT, &dwResult);
        // time out or error?
        if (lResult == 0)
        {
            GlobalFree(tmp_ptr);
            return context->NewStringFromAsciiz("");
        }
        else
        {
            curSel = (int) dwResult;
        }
        if (curSel != LB_ERR)
        {
            // get the text length and allocate a new strptr if needed
            lResult = SendMessageTimeout(hW, LB_GETTEXTLEN, curSel, 0, MSG_TIMEOUT_OPTS, MSG_TIMEOUT, &dwResult);
            // time out or error?
            if (lResult == 0)
            {
                GlobalFree(tmp_ptr);
                return context->NewStringFromAsciiz("");
            }

            if (dwResult > bufferLength)
            {
                GlobalFree(tmp_ptr);
                tmp_ptr = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, dwResult + 1);
                if (!tmp_ptr)
                {
                    context->InvalidRoutine();
                    return NULLOBJECT;
                }
                bufferLength = dwResult + 1;
            }
            lResult = SendMessageTimeout(hW, LB_GETTEXT, curSel, (LPARAM)tmp_ptr, MSG_TIMEOUT_OPTS, MSG_TIMEOUT, &dwResult);
            // time out or error?
            if (lResult == 0)
            {
                GlobalFree(tmp_ptr);
                return context->NewStringFromAsciiz("");
            }
            else
            {
                return context->NewString(tmp_ptr, dwResult);
            }
        }
    }
    return context->NewStringFromAsciiz("");
}


RexxFunction1(RexxStringObject, WSGetWindowClass, uintptr_t, handle)
{
    char buffer[256];
    size_t length = GetClassName(HWND)handle, buffer, sizeof(buffer));

    return context->NewString(buffer, length);
}


RexxFunction1(int, WSShowWindow, uintptr_t, handle)
{
    return ShowWindow((HWND)handle, SW_SHOW);
}


RexxFunction1(int, WSHideWindow, uintptr_t, handle)
{
    return ShowWindow((HWND)handle, SW_HIDE);
}


RexxFunction1(int, WSMaximizeWindow, uintptr_t, handle)
{
    return ShowWindow((HWND)handle, SW_SHOWMAXIMIZED);
}


RexxFunction1(int, WSMinimizeWindow, uintptr_t, handle)
{
    return ShowWindow((HWND)handle, SW_MINIMIZE);
}


RexxFunction1(int, WSRestoreWindow, uintptr_t, handle)
{
    return ShowWindow((HWND)handle, SW_RESTORE);
}


RexxFunction1(uintptr_t, WSNextWindow, uintptr_t, handle)
{
    return GetWindow((HWND)handle, GW_HWNDNEXT);
}


RexxFunction1(uintptr_t, WSPreviousWindow, uintptr_t, handle)
{
    return GetWindow((HWND)handle, GW_HWNDPREV);
}


RexxFunction1(uintptr_t, WSFirstChildWindow, uintptr_t, handle)
{
    return GetWindow((HWND)handle, GW_HWNDCHILD);
}


RexxFunction1(uintptr_t, WSFirstWindow, uintptr_t, handle)
{
    return GetWindow((HWND)handle, GW_HWNDFIRST);
}



RexxFunction1(uintptr_t, WSLastWindow, uintptr_t, handle)
{
    return GetWindow((HWND)handle, GW_HWNDLAST);
}


RexxFunction1(uintptr_t, WSOwnerWindow, uintptr_t, handle)
{
    return GetWindow((HWND)handle, GW_OWNER);
}


RexxFunction1(int, WSWindowId, uintptr_t, handle)
{
    return GetDlgCtrlID(hW);
}


RexxFunction3(uintptr_t, WSGetWindowAtPos, int, x, int, y, OPTIONAL_uintptr_t, handle)
{
    POINT pt;
    pt.x = x;
    pt.y = y;

    if (handle == 0)
    {
        return WindowFromPoint(pt);
    }
    return ChildWindowFromPointEx((HWND)handle, pt, CWP_ALL);
}


RexxFunction1(RexxStringObject, WSGetWindowRectangle, OPTIONAL_uintptr_t, handle)
{
    RECT r;
    if (handle && GetWindowRect((HWND)handle, &r))
    {
        char buffer[256];
        sprintf(buffer,"%d,%d,%d,%d",r.left, r.top, r.right, r.bottom);
        return context->NewStringFromAsciiz(buffer);
    }
    return context->NewStringFromAsciiz("");
}

RexxFunction1(RexxStringObject, WSGetWindowState, OPTIONAL_uintptr_t, handle)
{
    HWND hW = (HWND)handle;

    if (!IsWindow(hW))
    {
        return context->NewStringFromAsciiz("Illegal Handle");
    }
    else
    {
        char buffer[256];

        sprintf(buffer, "%s %s",
            IsWindowEnabled(hW) ? "Enabled" : "Disabled",
            IsWindowVisible(hW) ? "Visible" : "Invisible");

        if (IsZoomed(hW))
        {
            strcat(buffer, " Zoomed");
        }
        else if (IsIconic(hW))
        {
            strcat(buffer, " Minimized");
        }
        if (GetForegroundWindow() == hW)
        {
            strcat(buffer, " Foreground");
        }
        return context->NewStringFromAsciiz(buffer);
    }
}

RexxFunction2(int, WSEnableWindow, OPTIONAL_uintptr_t, handle, RexxBoolean, state)
{
    return EnableWindow((HWND)handle, state);
}

RexxFunction2(int, WSMoveWindow, OPTIONAL_uintptr_t, handle, int, x, int, y)
{
    RECT r;
    HWND hW = (HWND)handle;
    if (!GetWindowRect(hW, &r))
    {
        return 1;
    }
    return !MoveWindow(hW, x, y, r.right - r.left, r.bottom-r.top, true))
}

RexxFunction2(int, WSSizeWindow, OPTIONAL_uintptr_t, handle, int, w, int, h)
{
    RECT r;
    HWND hW = (HWND)handle;
    if (!GetWindowRect(hW, &r))
    {
        return 1;
    }
    return !SetWindowPos(hW, r.left, r.right, w, h, SWP_NOMOVE | SWP_NOZORDER);
}

RexxFunction4(int, WSSendKey, uintptr_t, handle, uintptr_t, key, CSTRING, option, OPTIONAL_uintptr_t, lparam)
{
    hW = (HWND)handle;
    if (hW)
    {
        WPARAM k = (WPARAM)key;
        LPARAM l = (LPARAM)l;

        /* Combination of keys (e.g. SHIFT END) and function keys (e.g. F1) dont worked                 */
        /* The reson is, that the WM_KEWUP/Doen message cannot handle this !                            */
        /* To get the function keys working, WM_KEYDOWN/UP can be used, but the message MUST be posted! */
        /* To get key combination working the keybd_event function must be use. But .....               */
        /* The window must be in the Foreground and The handle must be associated:                      */
        /* To get these working, the REXX code must be lok like:                                        */
        /* EditHandle = npe~Handle                                                                      */
        /* npe~ToForeground                                                                             */
        /* npe~AssocWindow(np~Handle)                                                                   */
        /* npe~SendKeyDown("SHIFT")                                                                     */
        /* npe~SendKeyDown("HOME")                                                                      */
        /* npe~SendKeyUp("SHIFT")                                                                       */
        /* npe~SendKeyUp("HOME")                                                                        */

        // just send the key Down event
        if (*option == 'D')
        {
            HWND HwndFgWin = GetForegroundWindow();
            if ( hW == HwndFgWin)
            {
                if (l != 0)
                    keybd_event((BYTE)k,0,KEYEVENTF_EXTENDEDKEY,0);
                else
                    keybd_event((BYTE)k,0,0,0);

                return 1;
            }
            else
            {
                return PostMessage(hW, WM_KEYDOWN, k, l);
            }
        }
        // key up event
        else if (*option == 'U')
        {
            HwndFgWin = GetForegroundWindow();
            if ( hW == HwndFgWin )
            {
                if (l == 1)
                    keybd_event((BYTE)k,0,KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY,0);
                else
                    keybd_event((BYTE)k,0,KEYEVENTF_KEYUP,0);

                return 1;
            }
            else
            {
                return PostMessage(hW,WM_KEYUP, k, l | 0xC0000000);
            }
        }
        // key "typed" event, which is a key up/key down combo or just the char value directly
        else
        {
            /* WM_CHAR don't works for ALT key combinations                             */
            /* WM_SYSKEYDOWN and hW,WM_SYSKEYUP must used                               */
            if ( l != 0 )
            {
                PostMessage(hW,WM_SYSKEYDOWN, k,l);
                return PostMessage(hW,WM_SYSKEYUP, k,l);
            }
            else
            {
                return SendNotifyMessage(hW,WM_CHAR, k,l);
            }
        }
    }
    return 0;
}

RexxFunction3(int, WSSendMessage, uintptr_t, handle, CSTRING, message, CSTRING, wparam, CSTRING, lparam)
{
    WPARAM w;
    LPARAM l;
    DWORD  m;
    ULONG n[3];

    hW = (HWND)handle;

    CONVERT_HEX_OR_INT(message, m);
    CONVERT_HEX_OR_INT(wparam, w);
    CONVERT_HEX_OR_INT(lparam, l);

    return SendNotifyMessage(hW, m, w, l);
}

RexxFunction1(uint32_t, WSMapVirtualKey, uint32_t, key)
{
    return MapVirtualKey(key);
}

RexxFunction1(uintptr_t, WSGetMenu, uintptr_t, handle)
{
    return GetMenu((HWND)handle);
}

RexxFunction1(uintptr_t, WSGetSysMenu, uintptr_t, handle)
{
    return GetSysMenu((HWND)handle, false);
}

RexxFunction2(uintptr_t, WSGetSubMenu, uintptr_t, handle, CSTRING, name)
{
    return GetSubMenu((HMENU)handle, name);
}

RexxFunction2(uintptr_t, WSGetMenuItemID, uintptr_t, handle, int, position)
{
    return GetMenuItemID((HMENU)handle, position);
}

RexxFunction1(int, WSGetMenuItemCount, uintptr_t, handle)
{
    return GetMenuItemCount((HMENU)handle);
}

RexxFunction3(RexxStringObject, WSGetMenuItemTest, uintptr_t, handle, int, position, CSTRING, option)
{
    int flag = MF_BYPOSITION;
    if (*option == 'C')
    {
        flag = MF_BYCOMMAND;
    }
    char buffer[256];

    DWORD length = GetMenuString(hM, position, buffer, sizeof(buffer), flag);
    return context->NewString(buffer, length);
}


RexxFunction1(RexxBoolean, WSGetMenuState, uintptr_t, handle)
{
    return IsMenu((HMENU)handle);
}


RexxFunction1(int, WSCopyClipboard, RexxStringObject, data)
{
    char *dataPtr = context->StringData(data);
    size_t length = context->StringLength(data);

    HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length + 1);
    char *membase = (char *) GlobalLock(hmem);
    int ret = 0;

    if (membase != NULL)
    {
        memcpy(membase, dataPtr, length + 1);
        if (OpenClipboard(NULL) && EmptyClipboard())
        {
            ret = SetClipboardData(CF_TEXT, hmem);
        }
        GlobalUnlock(membase);
        CloseClipboard();
    }
    return !ret;
}

RexxFunction0(RexxStringObject, WSPasteClipboard)
{
    HGLOBAL hmem;
    char * membase;
    if (IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(NULL))
    {
        hmem = GetClipboardData(CF_TEXT);
        membase = (char *) GlobalLock(hmem);
        if (membase != NULL)
        {
            RexxStringObject result = context->NewStringFromAsciiz(membase);
            GlobalUnlock(membase);
            CloseClipboard();
            return result;
        }
        else
        {
            CloseClipboard();
        }
    }
    return context->NewStringFromAsciiz("");
}

RexxFunction0(int, WSEmptyClipboard)
{
    if (IsClipboardFormatAvailable(CF_TEXT))
    {
        if (!OpenClipboard(NULL))
        {
            return 1;
        }
        int ret = !EmptyClipboard();
        CloseClipboard();
        return ret;
    }
    return 0;
}

RexxFunction0(RexxBoolean, WSClipboardAvailable)
{
    return IsClipboardFormatAvailable(CF_TEXT);
}

void Little2BigEndian(BYTE *pbInt, int iSize)
{
  /* convert any integer number from little endian to big endian or vice versa */
  BYTE  bTemp[32];
  int   i;

  if (iSize <= 32)
  {
    for (i = 0; i < iSize; i++)
      bTemp[i] = pbInt[iSize - i - 1];

    memcpy(pbInt, bTemp, iSize);
  } /* endif */
}

// now build the actual entry list
ooRexxFunctionEntry rxwinsys_functions[] =
{
    REXX_TYPED_FUNCTION(WSCreateRegistryKey, WSCreateRegistryKey)
    REXX_TYPED_FUNCTION(WSOpenRegistryKey, WSOPenRegistryKey)
    REXX_TYPED_FUNCTION(WSCloseRegistryKey, WSCloseRegistryKey)
    REXX_TYPED_FUNCTION(WSDeleteRegistryKey, WSDeleteRegistryKey)
    REXX_TYPED_FUNCTION(WSQueryRegistryKey, WSQuerygistryKey)
    REXX_TYPED_FUNCTION(WSListRegistryKey, WSListRegistryKey)
    REXX_TYPED_FUNCTION(WSFlushRegistryKey, WSFlushRegistryKey)
    REXX_TYPED_FUNCTION(WSSetRegistryValue, WSSetRegistryValue)
    REXX_TYPED_FUNCTION(WSQueryRegistryValue, WSQueryRegistryValue)
    REXX_TYPED_FUNCTION(WSListRegistryValue, WSListRegistryValue)
    REXX_TYPED_FUNCTION(WSConnectRegistryFile, WSConnectRegistryFile)
    REXX_TYPED_FUNCTION(WSSaveRegistryFile, WSSaveRegistryFile)
    REXX_TYPED_FUNCTION(WSUnloadRegistryFile, WSUnloadRegistryFile)
    REXX_TYPED_FUNCTION(WSRestoreRegistryFile, WSRestoreRegistryFile)
    REXX_TYPED_FUNCTION(WSReplaceRegistryFile, WSReplaceRegistryFile)
    REXX_TYPED_FUNCTION(WSProgManagerAddGroup, WSProgManagerAddGroup)
    REXX_TYPED_FUNCTION(WSProgManagerDeleteGroup, WSProgManagerDeleteGroup)
    REXX_TYPED_FUNCTION(WSProgManagerMinimizeGroup, WSProgManagerMinimizeGroup)
    REXX_TYPED_FUNCTION(WSProgManagerMaximizeGroup, WSProgManagerMaximizeGroup)
    REXX_TYPED_FUNCTION(WSProgManagerAddItem, WSProgManagerAddItem)
    REXX_TYPED_FUNCTION(WSProgManagerDeleteItem, WSProgManagerDeleteItem)
    REXX_TYPED_FUNCTION(WSProgManagerLeave, WSProgManagerLeave)
    REXX_TYPED_FUNCTION(WSProgManagerAddDesktopIcon, WSProgManagerAddDesktopIcon)
    REXX_TYPED_FUNCTION(WSProgManagerDeleteDesktopIcon, WSProgManagerDeleteDesktopIcon)
    REXX_TYPED_FUNCTION(WSCloseEventLog, WSCloseEventLog)
    REXX_TYPED_FUNCTION(WSOpenEventLog, WSOpenEventLog)
    REXX_TYPED_FUNCTION(WSReadEventLog, WSReadEventLog)
    REXX_TYPED_FUNCTION(WSWriteEventLog, WSWriteEventLog)
    REXX_TYPED_FUNCTION(WSFindChildWindow, WSFindChildWindow)
    REXX_TYPED_FUNCTION(WSFindWindow   ,WSFindWindow)
    REXX_TYPED_FUNCTION(WSSetForegroundWindow ,WSSetForegroundWindow)
    REXX_TYPED_FUNCTION(WSGetForegroundWindow ,WSGetForegroundWindow)
    REXX_TYPED_FUNCTION(WSSetWindowTitle ,WSSetWindowTitle)
    REXX_TYPED_FUNCTION(WSGetWindowTitle ,WSGetWindowTitle)
    REXX_TYPED_FUNCTION(WSShowWindow ,WSShowWindow)
    REXX_TYPED_FUNCTION(WSHideWindow ,WSHideWindow)
    REXX_TYPED_FUNCTION(WSMaximizeWindow ,WSMaximizeWindow)
    REXX_TYPED_FUNCTION(WSMinimizeWindow ,WSMinimizeWindow)
    REXX_TYPED_FUNCTION(WSRestoreWindow ,WSRestoreWindow)
    REXX_TYPED_FUNCTION(WSNextWindow ,WSNextWindow)
    REXX_TYPED_FUNCTION(WSPreviousWindow ,WSPreviousWindow)
    REXX_TYPED_FUNCTION(WSFirstChildWindow ,WSFirstChildWindow)
    REXX_TYPED_FUNCTION(WSFirstWindow ,WSFirstWindow)
    REXX_TYPED_FUNCTION(WSLastWindow ,WSLastWindow)
    REXX_TYPED_FUNCTION(WSOwnerWindow ,WSOwnerWindow)
    REXX_TYPED_FUNCTION(WSWindowId ,WSWindowId)
    REXX_TYPED_FUNCTION(WSGetWindowAtPos ,WSGetWindowAtPos)
    REXX_TYPED_FUNCTION(WSGetWindowRectangle ,WSGetWindowRectangle)
    REXX_TYPED_FUNCTION(WSGetWindowState ,WSGetWindowState)
    REXX_TYPED_FUNCTION(WSEnableWindow ,WSEnableWindow)
    REXX_TYPED_FUNCTION(WSMoveWindow ,WSMoveWindow)
    REXX_TYPED_FUNCTION(WSSizeWindow  ,WSSizeWindow)
    REXX_TYPED_FUNCTION(WSSendKey ,WSSendKey)
    REXX_TYPED_FUNCTION(WSSendMessage ,WSSendMessage)
    REXX_TYPED_FUNCTION(WSMapVirtualKey ,WSMapVirtualKey)
    REXX_TYPED_FUNCTION(WSGetSysMenu ,WSGetSysMenu)
    REXX_TYPED_FUNCTION(WSGetSubMenu ,WSGetSubMenu)
    REXX_TYPED_FUNCTION(WSGetMenuItemId ,WSGetMenuItemId)
    REXX_TYPED_FUNCTION(WSGetMenuItemCount ,WSGetMenuItemCount)
    REXX_TYPED_FUNCTION(WSGetMenuItemTest ,WSGetMenuItemTest)
    REXX_TYPED_FUNCTION(WSGetMenuState ,WSGetMenuState)
    REXX_TYPED_FUNCTION(WSCopyClipboard ,WSCopyClipboard)
    REXX_TYPED_FUNCTION(WSPasteClipboard ,WSPasteClipboard)
    REXX_TYPED_FUNCTION(WSEmptyClipboard ,WSEmptyClipboard)
    REXX_TYPED_FUNCTION(WSClipboardAvailable ,WSClipboardAvailable)
};

ooRexxPackageEntry rxmath_package_entry =
{
    STANDARD_PACKAGE_HEADER
    "RXWINSYS",                          // name of the package
    "1.2",                               // package information
    NULL,                                // no load/unload functions
    NULL,
    rxwinsys_functions,                  // the exported functions
    NULL                                 // no methods in rxmath.
};

// package loading stub.
OOREXX_GET_PACKAGE(rxmath);
