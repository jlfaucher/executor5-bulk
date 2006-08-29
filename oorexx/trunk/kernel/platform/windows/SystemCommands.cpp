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
/******************************************************************************/
/*  wincmd.c - C methods for handling calls to system exits and subcommand    */
/*             handlers.                                                      */
/*                                                                            */
/*  C methods:                                                                */
/*    SysCommand     - Method to invoke a subcommand handler                  */
/*                                                                            */
/*  Internal routines:                                                        */
/*    sys_command - Run a command through system command processor.           */
/******************************************************************************/
#include <string.h>                    /* Get strcpy, strcat, etc.       */

#include <process.h>
#include <stdlib.h>

#include "RexxCore.h"                    /* global REXX declarations     */
#include "StringClass.hpp"
#include "SysInterpreter.hpp"
#include "SysProcess.hpp"

#define CMDBUFSIZE32S 260              /* Max size of executable cmd     */
#define CMDBUFSIZENT 8092              /* Max size of executable cmd     */
#define CMDDEFNAME32S "COMMAND.COM"    /* Default Win 95   cmd handler   */
#define CMDDEFNAMENT "CMD.EXE"         /* Default Win NT   cmd handler   */
#define DEFEXT "REX"                   /* Default Win  REXX program ext  */
#define UNKNOWN_COMMAND 1              /* unknown command return code    */
#include "direct.h"

#define SYSENV "CMD"                   /* Default windows cmd environment*/
#define COMSPEC "COMSPEC"              /*      cmd handler env name      */

#define SHOWWINDOWFLAGS SW_HIDE        // determines visibility of cmd
                                       // window SHOW, HIDE etc...

/******************************************************************************/
/* Name:       SysCommand                                                     */
/*                                                                            */
/* Arguments:  cmdenv - Command environment (string OREF)                     */
/*             cmd - Command to be executed (string OREF)                     */
/*                                                                            */
/* Returned:   result - Array object containing:                              */
/*                      1) rc from command (integer OREF)                     */
/*                      2) error condition (integer OREF)                     */
/*                                                                            */
/* Notes:      Handles processing of a command.  First passes cmd on to the   */
/*             current subcommand handler.  If there isn't one registered     */
/*             and the current command environment matches the default system */
/*             environment, we'll try passing the command to the system for   */
/*             execution.                                                     */
/******************************************************************************/
RexxObject * SysInterpreter::executeSystemCommand(
  RexxActivity      * activity,        /* activity running on               */
  RexxString        * current_address, /* target address                    */
  RexxString        * command,         /* command to issue                  */
  RexxString       ** error_failure )  /* error or failure flags            */
{
  int          rc    = 0;              /* Return code from call             */
  RexxObject * result;

  *error_failure = OREF_NULL;          /* default to clean call             */

  SysActivity *myActivity = &activity->systemInfo;

  // is this the system environment?
  if (current_address->strICompare((stringchar_t *)SYSENV) == 0)
  {
      SysActivity *myActivity = &activity->systemInfo;
      {
          // NO kernel access this section
          UnsafeBlock ublock;
                                           /* issue the command                   */
          rc = windowsCommand(myActivity, (char *)command->getStringData(), error_failure);
      }
      result = new_integer(rc);        /* get the command return code         */
      if (rc != 0)                     /* command error?                      */
      {
                                       /*   send error condition back         */
          *error_failure = OREF_ERRORNAME;
      }
  }
  else
  {                                          /* real command failure */
    *error_failure = OREF_FAILURENAME;
    result = new_integer(RXSUBCOM_NOTREG);   /* just give not registered rc */
  }
  return result;                                /* Return result value        */
}



/* Handle "SET XX=YYY" command in same process */
bool SysInterpreter::processSetCommand(char * cmd, int*rc)
{
    char * eqsign, * st;
    char name[256];
    char value[4096];
    eqsign = strchr(cmd, '=');
    // no equal sign, not a valid SET command.
    if (!eqsign)
    {
        return false;
    }

    // step over the "SET " and find the first non-blank
    st = cmd + 4;
    while (*st == ' ')
    {
        st++;
    }

    // missing variable name?
    if (st == eqsign)
    {
        return false;
    }
    strncpy(name, st, eqsign-st);
    name[eqsign-st]='\0';

    // now expand any environment variables in the string and set.
    if (ExpandEnvironmentStrings(eqsign+1, value, 4095)
        && SetEnvironmentVariable(name,value)) *rc = 0; else *rc = GetLastError();
    return true;
}


/* Handle "CD XXX" command in same process */
bool SysInterpreter::processCdCommand(char * cmd, int *rc)
{
    char * st;

    st = &cmd[3];
    while (*st == ' ')
    {
        st++;
    }
    if (!*st) return false;

    if ((strlen(st) == 2) && (st[1] == ':'))
         *rc = _chdrive(toupper( *st ) - 'A' + 1);
    else
         *rc = _chdir(st);
    return true;
}



/******************************************************************************/
/* Name:       sys_command                                                    */
/*                                                                            */
/* Arguments:  cmd - Command to be executed                                   */
/*                                                                            */
/* Returned:   rc - Return Code                                               */
/*                  Note: if non-zero rc from DosExecPgm return DosExecPgm rc */
/*                  else if non-zero termination code from system return code */
/*                  else return rc from executed command                      */
/*                                                                            */
/* Notes:      Handles processing of a system command.  Finds location of     */
/*             system command handler using the COMSPEC environment variable  */
/*             and invokes the system specific routine which invokes the      */
/*             command handler with the command to be executed                */
/*                                                                            */
/******************************************************************************/
int SysInterpreter::windowsCommand(SysActivity *myActivity, char *cmd, RexxString **error_failure)
{
    char       *cl_opt = " /c ";         /* "/c" opt for sys cmd handler      */
    char       *cmdstring_ptr;           /* Command string pointer            */
    char        cmdstring[CMDBUFSIZENT]; /* Largest cmd we can give system    */
    size_t      max_cmd_length;          /* Maximum size of command           */
    int         rc;                      /* Return code                       */
    char       *interncmd;

    size_t      uMaxStringLength;        // max length of string (system
                                         // specific)
    char        tmp[8];
    size_t      i = 0;
    char       *filepart;
    bool        fileFound, searchFile;

    // if the command starts with the no-echo escape, ignore that part.
    if (cmd[0] == '@')
    {
        interncmd = cmd + 1;
    }
    else
    {
        interncmd = cmd;
    }

    // scan forward to the first non-blank character of the command.
    while (*interncmd == ' ')
    {
        interncmd++;
    }

    /* check for redirection symbols, ignore them when enclosed in double quotes */
    bool usesRedirects = false;
    size_t commandLength = strlen(interncmd);
    bool inLiteral = false;
        for (i = 0; i < commandLength; i++)
    {
        switch (interncmd[i])
        {
            // quoted string...we switch on or off
            case '"':
                inLiteral = !inLiteral;
                break;

            case '<':
            case '>':
            case '|':
            case '&':
                // if a redirection character is encountered outside of a quoted string....
                if (!inLiteral)
                {
                    // rats, redirections used
                    usesRedirects = true;
                }
                break;
        }
    }


    // if redirection isn't used, we need see if we can do special handling for some of
    // the commands so they behave better in process.
    if (!usesRedirects)
    {
        // hand
        // first check for a 'SET" command...we can handle those differently.
        strncpy(tmp, interncmd, 4);
        tmp[4] = '\0';
        if (stricmp("set ",tmp) == 0)
        {
            if (processSetCommand(interncmd, &rc))
            {
                return rc;
            }
        }
        else
        {
            // now look for a CD command
            strncpy(tmp, interncmd, 3);
            tmp[3] = '\0';
            if (!stricmp("cd ",tmp))
            {
                if (processCdCommand(interncmd, &rc))
                {
                    return rc;
                }
            }
            else
            {          /* check for drive letter change */
                if ((tmp[1] == ':') && ((tmp[2] == ' ') || (tmp[2] == '\0')))
                {
                    // directly change the drive
                    return _chdrive(toupper( tmp[0] ) - 'A' + 1);
                }
                else
                {
                    /* check if a START command is specified, if so don not start it directly */
                    strncpy(tmp, interncmd, 6);
                    tmp[6] = '\0';
                    usesRedirects = stricmp("start ",tmp) == 0;
                }
            }
        }
    }


    // we have to shell out a command, so we need to figure out which command processor to use.

    // location of command interpreter is normally specified in COMSPEC, but assume a
    // reasonable default if it isn't specified.
    char *sysCmdHandler = getenv(COMSPEC);
    if (sysCmdHandler == NULL)
    {
        // CMD.EXE on NT Command.com on 32s
        sysCmdHandler = SysProcess::runningNT() ? CMDDEFNAMENT : CMDDEFNAME32S;
    }

    /* Get len of handler                  */
    size_t lengthCmdHandler = strlen(sysCmdHandler);

    cmdstring_ptr = cmdstring;           /* Set pointer to cmd buffer           */
    /****************************************************************************/
    /* Compute maximum size of command we can fit into string being passed to   */
    /* CreateProcess                                                            */
    /****************************************************************************/

    // string length is system specific
    uMaxStringLength = (SysProcess::runningNT()) ? CMDBUFSIZENT : CMDBUFSIZE32S;
    max_cmd_length = uMaxStringLength -       /* Maximum size of string         */
        (lengthCmdHandler+1) - /* Minus length of sys cmd handler*/
        strlen(cl_opt) -         /* Minus length of cmd line option*/
        1;                       /* Minus length of terminating \0 */

    myActivity->runningInConsole = false;
    bool explicitConsole = false;

    /* check whether or not program to invoke is cmd or command */
    _strupr(strncpy(cmdstring_ptr, interncmd, max_cmd_length-1));
    // make sure there is a terminator on this
    cmdstring_ptr[max_cmd_length - 1] = '\0';
    if (!SysProcess::running98())
        searchFile = strstr(cmdstring_ptr, "CMD") != 0;
    else
        searchFile = strstr(cmdstring_ptr, "COMMAND") != 0;

    // if this is an explicit CMD or COMMAND invocation, we need to do additional processing.
    if (searchFile)
    {
        // command start off with a quoted string?  Step over it.
        if (cmdstring_ptr[0] == '\"')
        {
            cmdstring_ptr++;
            while (cmdstring_ptr[i] != '\"') i++;
            cmdstring_ptr[i]='\0';
        }
        else
        {
            // single quoted?
            if (cmdstring_ptr[0] == '\'')
            {
                cmdstring_ptr++;
                while (cmdstring_ptr[i] && (cmdstring_ptr[i] != '\'')) i++;
                cmdstring_ptr[i]='\0';
            }
            // find first blank delimited token
            else
            {
                while (cmdstring_ptr[i] && (cmdstring_ptr[i] != ' ')) i++;
                cmdstring_ptr[i]='\0';
            }
        }

        // now see if we can find an executable with this name.
        if (!SysProcess::running98())
            fileFound = SearchPath(NULL, cmdstring_ptr, ".EXE", CMDBUFSIZENT-1, cmdstring, &filepart) != 0;
        else
            fileFound = SearchPath(NULL, cmdstring_ptr, ".COM", CMDBUFSIZENT-1, cmdstring, &filepart) != 0;
        cmdstring_ptr = cmdstring;           /* Set pointer again to cmd buffer (might have been increased) */

        // if the actually command being executed matches the command handler, we need to do some
        // make sure we don't double launch the command shell.
        if (fileFound && stricmp(sysCmdHandler, cmdstring_ptr) == 0)
        {
            myActivity->runningInConsole = true;
            explicitConsole = true;
        }
    }

    /* first check whether we can run command directly as a program (no file redirection when not cmd or command) */
    if (explicitConsole || !usesRedirects)
    {
        // go do it.
        rc = callCommandShell(myActivity, interncmd, error_failure, true, explicitConsole);
        // if this was not a failure, then we're done...otherwise reset and try again by
        // explicitly shelling the command handler.
        if (*error_failure != OREF_FAILURENAME)
        {
            return rc;   /* command processed, exit */
        }
        else *error_failure = OREF_NULL;
    }
    /* no we couldn't, so pass command to cmd.exe or command.com */

    strcpy(cmdstring_ptr, sysCmdHandler);             /* Put in system cmd handler      */

    /* the following lines checks whether or not a /k option is specified */
    /* if so the /c option must not be concatenated */
    /* /k can only be specified as the first argument to keep the command handler active */
    if (!( (strlen(interncmd) > 1) && (*interncmd == '/')
        && ((interncmd[1] == 'k') || (interncmd[1] == 'K'))
        && ((interncmd[2] == ' ') || (interncmd[2] == '\0')) ))
        strcat(cmdstring_ptr,cl_opt);
    else
    {
        myActivity->runningInConsole = true;
        strcat(cmdstring_ptr, " ");
    }

    // fix buffer modification problem.
    strcat(cmdstring_ptr,interncmd);        /* And cmd to be executed         */

    /****************************************************************************/
    /* Invoke the system command handler to execute the command                 */
    // On NT, this will be the actual rc from the executing program
    // on Win32s this will be 0 if Winexec spawned the program, 1-31
    // reflect errors from WinExec; where WinExec rc=0 is mapped to 1
    /****************************************************************************/
    // Call system specific routine
    rc = callCommandShell(myActivity, cmdstring_ptr, error_failure, false, explicitConsole);

    myActivity->runningInConsole = false;
    return rc;
}                                      // SystemCommand


/*-----------------------------------------------------------------------------
 | Name:       sysCommandNT                                                   |
 |                                                                            |
 | Arguments:  cmd - Command to be executed                                   |
 |                                                                            |
 | Returned:   rc - Return Code                                               |
 |                  Note: if CreateProcess fails return GetLastError code     |
 |                  else return rc from executed command                      |
 |                                                                            |
 | Notes:      Handles processing of a system command on a Windows NT system  |
 |                                                                      |
  ----------------------------------------------------------------------------*/
int SysInterpreter::callCommandShell(SysActivity *myActivity, char *cmdstring_ptr, RexxString  **error_failure, bool direct, bool explicitConsole)
{
    DWORD rc;
    STARTUPINFO siStartInfo;                  // process startup info
    PROCESS_INFORMATION piProcInfo;           // returned process info
    CHAR ctitle[256];
    DWORD creationFlags;
    bool titleChanged;

    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    ZeroMemory(&piProcInfo, sizeof(piProcInfo));
    /****************************************************************************/
    /* Invoke the system command handler to execute the command                 */
    /****************************************************************************/
    siStartInfo.cb = sizeof(siStartInfo);

    siStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    titleChanged = GetConsoleTitle(ctitle, 255) != 0;
    siStartInfo.lpTitle = cmdstring_ptr;
    creationFlags = GetPriorityClass(GetCurrentProcess()) | CREATE_NEW_PROCESS_GROUP;
    if (!siStartInfo.hStdInput && !siStartInfo.hStdOutput && !titleChanged)  /* is REXXHIDE running without console */
    {
        if (!direct)
        {
            siStartInfo.wShowWindow = SHOWWINDOWFLAGS;
            siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
        }
        else
        {
            if (explicitConsole)
            {
                creationFlags |= CREATE_NEW_CONSOLE; /* new console if CMD ord COMMAND was specified */
            }
        }
    }
    else              /* just use standard handles if we are running in a console */
    {
        if (direct)
        {
            siStartInfo.dwFlags = STARTF_USESTDHANDLES; /* no SW_HIDE for direct commands */
        }
        else
        {
            siStartInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        }
        siStartInfo.wShowWindow = SHOWWINDOWFLAGS;
    }

    if ( CreateProcess ( NULL,           // address of module name
        cmdstring_ptr,       // address of command line
        NULL,                // address of process security attrs
        NULL,                // address of thread security attrs
        true,                // new process inherits handles?
    // creation flags
        creationFlags,
        NULL,                // address of new environment block
        NULL,                // address of current directory name
        &siStartInfo,        // address of STARTUPINFO
        &piProcInfo))        // address of PROCESS_INFORMATION
    // good rc from create now
    // Wait for process to end and
    {
        if (titleChanged)
        {
            SetConsoleTitle(siStartInfo.lpTitle);
        }
        myActivity->commandHostProcess = piProcInfo.hProcess;
        myActivity->commandHostProcessId = piProcInfo.dwProcessId;

        if (WAIT_FAILED != WaitForSingleObject (piProcInfo.hProcess, INFINITE))
        {
            // complete,ok?, get terminate rc
            GetExitCodeProcess ( piProcInfo.hProcess, &rc );
        }
        else
        {
            rc = GetLastError ();         // bad termination? get error code
            *error_failure = OREF_FAILURENAME;
        } // WaitForSingleObject
        /* the new process must be detached so it will be discarded automatically after execution */
        /* The thread must be closed first */
        if (titleChanged)
        {
            SetConsoleTitle(ctitle);
        }
        CloseHandle(piProcInfo.hThread);
        CloseHandle(piProcInfo.hProcess);
    }
    else
    {
        rc = GetLastError ();           // Couldn't create process
        *error_failure = OREF_FAILURENAME;
    } // CreateProcess

    myActivity->commandHostProcess = NULL;
    myActivity->commandHostProcessId = 0;
    return rc;
}                                      // SysCommandNT

