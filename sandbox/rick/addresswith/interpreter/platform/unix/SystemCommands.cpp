/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2018 Rexx Language Association. All rights reserved.    */
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
/******************************************************************************/
/* REXX UNIX  Support                                                         */
/*                                                                            */
/* unix specific command processing routines                                  */
/*                                                                            */
/******************************************************************************/

#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <spawn.h>

#include "RexxCore.h"
#include "StringClass.hpp"
#include "Activity.hpp"
#include "ActivityManager.hpp"
#include "SystemInterpreter.hpp"
#include "InterpreterInstance.hpp"
#include "SysInterpreterInstance.hpp"

#include "RexxInternalApis.h"
#include <sys/types.h>
#include <pwd.h>
#include <limits.h>
#include <errno.h>  //@@
#include <fcntl.h>  //@@ O_* constant definitions
#include <sys/stat.h>  //@@ fstat
/*
// select()
#include <sys/ioctl.h>
#include <sys/time.h>
#if defined( HAVE_SYS_SELECT_H )
#include <sys/select.h>
#endif
*/

// "sh" should be our initial ADDRESS() environment across all Unix platforms
#define SYSINITIALADDRESS "sh"

#define MAX_COMMAND_ARGS 400
#define UNKNOWN_COMMAND 127                 /* unknown command return code    */

#define EXPORT_FLAG 1
#define SET_FLAG    2
#define UNSET_FLAG  3
#define MAX_VALUE   1280

extern int putflag;


/**
 * Retrieve the globally default initial address.
 *
 * @return The string name of the default address.
 */
RexxString *SystemInterpreter::getDefaultAddressName()
{
    return GlobalNames::INITIALADDRESS;
}

/* Handle "export" command in same process */
bool sys_process_export(RexxExitContext *context, const char * cmd, RexxObjectPtr &rc, int flag)
{
    char *Env_Var_String = NULL;         /* Environment variable string for   */
    size_t size, allocsize;              /* size of the string                */
    char      **Environment;             /* environment pointer               */
    char  *np;
    size_t i,j,k,l,iLength, copyval;
    char   namebufcurr[1281];             /* buf for extracted name            */
    char   cmd_name[1281];                /* name of the envvariable setting   */
    char   *array, *runarray, *runptr, *endptr, *maxptr;
    char   temparray[1281];
    const char *st;
    char  *tmpptr;
    char   name[1281];                    /* is the name + value + =           */
    char   value[1281];                   /* is the part behind =              */
    char  *del = NULL;                    /* ptr to old unused memory          */
    char  *hit = NULL;
    bool   HitFlag = false;
    l = 0;
    j = 0;
    allocsize = 1281 * 2;

    memset(temparray, '\0', sizeof(temparray));

    Environment = getEnvironment();       /* get the environment               */
    if (flag == EXPORT_FLAG)
    {
        st = &cmd[6];
    }
    else if (flag == UNSET_FLAG)
    {
        st = &cmd[5];
    }
    else
    {
        st = &cmd[3];
    }
    while ((*st) && (*st == ' '))
    {
        st++;
    }
    strcpy(name, st);
    iLength = strlen(st) + 1;

/* if string == EXPORT_FLAG or string == SET and only blanks are delivered */

    if ( ((flag == EXPORT_FLAG) || (flag == SET_FLAG)) &&  (iLength == 1) )
    {
        return false;
    }

    if (!putflag)
    {                        /* first change in the environment ? */
        /* copy all entries to dynamic memory                                   */
        for (;*Environment != NULL; Environment++)
        {                                  /*for all entries in the env         */
            size = strlen(*Environment)+1;   /* get the size of the string        */
            Env_Var_String = (char *)malloc(size); /* and alloc space for it      */
            memcpy(Env_Var_String,*Environment,size);/* copy the string           */
            putenv(Env_Var_String);          /* and chain it in                   */
        }
    }
    putflag = 1;                         /* prevent do it again               */
    Environment = getEnvironment();      /* reset the environment pointer     */

/* do we have a assignment operator? If not return true           */
/* The operating system treads this like no command, and so do we */

    if ( !(strchr(name, '=')) && (flag != UNSET_FLAG) ) /*only set and export */
    {
/* we do not have a assignment operator, but maybe a '|' for a    */
/* controlled output                                              */
        if ( (strchr(name, '|'))  || (strchr(name, '>')) || (strstr(name, ">>")) )
        {
            return false;
        }
        else
        {
            // this worked ok (well, sort of)
            rc = context->False();
            return true;
        }
    }

/* no '=' for unset, so force a shell error message               */

    if ( (strchr(name, '=')) && (flag == UNSET_FLAG) )
    {
        return false;
    }

    for (i=0; (i<iLength) && (name[i]!='='); i++)
    {
        cmd_name[i] = name[i];
    }

/* lets try handling variables in the assignment string */

    cmd_name[i]  = '\0';                /* copy the terminator           */

    i++;                                /* the place after'=' */

/* lets search the value for variable part(s)  */

    strcpy(value, &(name[i]));         /* value contains the part behind '=' */
    array = (char *) malloc(1281);
    strcpy(array, cmd_name);
    array[strlen(cmd_name)] = '=';
    array[i] = '\0';                   /* copy the terminator           */
    runarray = array + strlen(array);
    runptr = value;
    endptr = runptr + strlen(value);   /*this is the end of the input*/
    maxptr = array + MAX_VALUE -1;     /* this is the end of our new string */

    while ((tmpptr = (strchr(runptr, '$'))) != 0)
    {
        Environment = getEnvironment();  /* get the beginning of the environment*/
        HitFlag = true;          /* if not true inputvalue= outputvalue*/
        copyval = tmpptr - runptr;
        if (copyval)   /* runarray should keep the 'real' environment  */
        {
            while ((runarray + copyval) >  maxptr)
            {
                array = (char *) realloc(array, allocsize);
                runarray = array + strlen(array);
                maxptr = array + allocsize - 1;
                allocsize = allocsize * 2;
            }
            memcpy(runarray,runptr, copyval);
            runarray= runarray + copyval; /* a new place to copy */
            *runarray = '\0';
            runptr = tmpptr;              /* now runptr is at the place of $ */
        }
        runptr++;
        for (j = 0;(*runptr != '/') && (*runptr != ':') && (*runptr != '$') &&
            (*runptr); j++)
        {
            memcpy(&(temparray[j]), runptr,1); /*temparray is the env var to search*/
            runptr++;
        }

        temparray[j] = '\0';                /* lets see what we can do    */
        np = NULL;

        for (;(*Environment != NULL) && (hit == NULL) ;Environment++)
        {
            np = *Environment;

            for (k=0;(*np!='=')&&(k<255);np++,k++)
            {
                memcpy(&(namebufcurr[k]),np,1);  /* copy the character                */
            }

            namebufcurr[k] = '\0';                 /* copy the terminator           */

            if (!strcmp(temparray,namebufcurr))     /* have a match ?         */
            {
                hit = *Environment;
                /* copy value to new string*/
            }
        }
        if (hit) /* if we have found an entry of the env var in the env list */
        {
            np ++;                  /* don't copy equal                     */
            while ((runarray + strlen(np)) >  maxptr)
            {
                array = (char *) realloc(array, allocsize);
                runarray = array + strlen(array);
                maxptr = array + allocsize - 1;
                allocsize = allocsize * 2;
            }
            strcpy(runarray, np);
            runarray = runarray + strlen(np);
            *runarray = '\0';
            hit = NULL;
        }
    }

    if (HitFlag == true)
    {
        if (runptr < endptr)
        {
            while ((runarray + strlen(runptr)) >  maxptr)
            {
                array = (char *) realloc(array, allocsize);
                runarray = array + strlen(array);
                maxptr = array + allocsize - 1;
                allocsize = allocsize * 2;
            }
            strcpy(runarray, runptr);      /* if there is a part after a var */
            runarray = runarray + strlen(runptr);
            *runarray = '\0';
        }
    }
    else   /* no hit so lets copy the value as it is                     */
    {
        while ((runarray + strlen(value)) >  maxptr)
        {
            array = (char *) realloc(array, allocsize);
            runarray = array + strlen(array);
            maxptr = array + allocsize - 1;
            allocsize = allocsize * 2;
        }
        strcpy(runarray,value);
        runarray = runarray + strlen(runptr);
        *runarray = '\0';
    }

    Environment = getEnvironment();     /* get the beginning of the environment*/

    for (;*Environment != NULL;Environment++)
    {
        np = *Environment;

        for (i=0;(*np!='=')&&(i<255);np++,i++)
        {
            memcpy(&(namebufcurr[i]),np,1);  /* copy the character                */
        }

        namebufcurr[i] = '\0';                 /* copy the terminator          */

        if (!strcmp(cmd_name,namebufcurr))/* have a match ?         */
        {
            del = *Environment;              /* remember it for deletion          */
        }
    }
    /* find the entry in the environ     */
    if (flag != UNSET_FLAG)
    {
        size = strlen(array)+1;
        Env_Var_String = (char *)malloc(size);/* get the memory                    */
        memcpy(Env_Var_String, array, size);
        int errCode = putenv(Env_Var_String);
        if (errCode != 0)
        {
            // non-zero is an error condition
            context->RaiseCondition("ERROR", context->String(cmd), NULL, context->WholeNumberToObject(errCode));
        }
        else
        {
            rc = context->False();
        }
    }

    if (del)                              /* if there was a old one            */
    {
        free(del);                         /* free it                           */
    }
    rc = context->False();
    return true;
}


/* Returns a copy of s without quotes. Escaped characters are kept unchanged */
char *unquote(const char *s)
{
    if ( s == NULL )
    {
        return NULL;
    }
    size_t size = strlen(s) + 1;
    char *unquoted = (char*)malloc(sizeof(char)*size);
    if ( unquoted == NULL )
    {
        return NULL;
    }
    char *u = unquoted;
    char c;
    bool escape = false;
    do
    {
        c = *s;
        if ( escape )
        {
            *u++ = *s;
            escape = false;
        }
        else if ( c == '\\' )
        {
            escape = true;
        }
        else if ( c != '"' )
        {
            *u++ = *s;
        }
        s++;
    }
    while ( c != '\0' );
    return unquoted;
}


/* Handle "cd XXX" command in same process */
bool sys_process_cd(RexxExitContext *context, const char * cmd, RexxObjectPtr rc)
{
    const char * st;
    const char *home_dir = NULL;            /* home directory path        */
    char *dir_buf = NULL;             /* full directory path        */
    const char *slash;                      /* ptr to '/'                 */
    struct passwd *ppwd;

    st = &cmd[2];
    while ((*st) && (*st == ' '))
    {
        st++;
    }
    if ((!*st) || (strlen(cmd) == 2))
    {
        home_dir = getenv("HOME");
        if (!home_dir)
        {
            return false;
        }
        dir_buf = (char *)malloc(strlen(home_dir)+1);
        strcpy(dir_buf, home_dir);
    }                                  /* if no user name            */
    else if (*(st) == '~' && (*(st+1) == '\0' || *(st+1) == '/'|| *(st+1) == ' ' ))
    {
        if (*(st+1) == '/')              /* if there is a path         */
        {
            st +=2;                        /* jump over '~/'             */
                                           /* get home directory path    */
            home_dir = getenv("HOME");     /* from the environment       */
            if (!home_dir)                  /* if no home dir info        */
            {
                return false;
            }
            /* get space for the buf      */
            dir_buf = (char *)malloc(strlen(home_dir)+strlen(st)+2);
            if (!dir_buf)
            {
                return false;
            }
            /* merge the strings          */
            sprintf(dir_buf, "%s/%s", home_dir, st);
        }
        else
        {
            /* get home directory path    */
            home_dir = getenv("HOME");     /* from the environment       */
                                           /* get space for the buf      */
            dir_buf = (char *)malloc(strlen(home_dir)+2);
            if (!dir_buf)
            {
                return false;
            }
            sprintf(dir_buf, "%s/", home_dir);
        }
    }
    else if (*(st) == '~')             /* cmd is '~username...'      */
    {
        st++;                            /* jump over '~'              */
        slash = strchr(st,'/');          /* search for '/'             */
        if (!slash)                      /* if no '/'                  */
        {
            /* rest of string is username */
            ppwd = getpwnam(st);           /* get info about the user    */
            if (ppwd == NULL || ppwd->pw_dir == NULL)
            {
                return false;
            }
                                           /* get space for the buf      */
            dir_buf = (char *)malloc(strlen(ppwd->pw_dir)+2);
            if (!dir_buf)
            {
                return false;
            }
            /* merge the strings          */
            sprintf(dir_buf, "%s/", ppwd->pw_dir);
        }
        else                            /* there is a slash           */
        {
            char username[256];            // need to copy the user name
            memcpy(username, st, slash - st);
            username[slash - st] = '\0';

            ppwd = getpwnam(username);     /* get info about the user    */
            if (ppwd == NULL || ppwd->pw_dir == NULL)
            {
                return false;
            }
            slash++;                       /* step over the slash        */
                                           /* get space for the buf      */
            dir_buf = (char *)malloc(strlen(ppwd->pw_dir)+strlen(slash)+2);
            if (!dir_buf)
            {
                return false;
            }
            /* merge the strings          */
            sprintf(dir_buf, "%s/%s", ppwd->pw_dir, slash);
        }
    }
    else
    {
        dir_buf = strdup(st);
    }

    char *unquoted = unquote(dir_buf);
    if (unquoted == NULL)
    {
        return false;
    }
    int errCode = chdir(unquoted);
    free(unquoted);

    free(dir_buf);
    if (errCode != 0)
    {
        // non-zero is an error condition
        context->RaiseCondition("ERROR", context->String(cmd), NULL, context->WholeNumberToObject(errCode));
    }
    else
    {
        rc = context->False();
    }
    return true;
}


/*********************************************************************/
/* This function breaks a command up into whitespace-delimited pieces*/
/* to create the pointer array for the execvp call.  It is only used */
/* to support the "COMMAND" command environment, which does not use  */
/* a shell to invoke its commands.                                   */
/*********************************************************************/

bool scan_cmd(const char *parm_cmd, char **argPtr, size_t i)
{
    char *cmd = strdup(parm_cmd);        /* Allocate for copy          */

    char *end = cmd + strlen(cmd);       /* Find the end of the command*/

    /* This loop scans our copy of the command, setting pointers in    */
    /* the args[] array to point to each of the arguments, and null-   */
    /* terminating each one of them.                                   */

    /* LOOP INVARIANT:                                                 */
    /* pos points to the next character of the command to be examined. */
    /* i indicates the next element of the args[] array to be loaded.  */
    for (char *pos = cmd; pos < end; pos++)
    {
        while (*pos==' ' || *pos=='\t')
        {
            pos++;                           /* Skip to first non-white    */
        }

        if (*pos == '\0')                  /* If we're at the end,       */
        {
            break;                           /* get out.                   */
        }

        /* If at this point, we've used up all but one of the available  */
        /* elements of our args[] array, let the user know, and we must  */
        /* terminate.                                                    */
        if (i == MAX_COMMAND_ARGS)
        {
            return false;
        }

        argPtr[i++] = pos;                 /* Point to current argument  */
                                           /* and advance i to next      */
                                           /* element of args[]          */
        while (*pos!=' ' && *pos!='\t' && *pos!='\0')
        {
            pos++;                           /* Look for next whitespace   */
        }                                  /* or end of command          */
        *pos = '\0';                       /* Null-terminate this arg    */

    }

    /* Finally, put a null pointer in args[] to indicate the end.      */
    argPtr[i] = NULL;
    return true;
}

/******************************************************************************/
/* Name:       sys_command                                                    */
/*                                                                            */
/* Arguments:  cmd - Command to be executed                                   */
/*             local_env_type - integer indicating which shell                */
/*                                                                            */
/* Returned:   rc - Return Code                                               */
/*                                                                            */
/* Notes:      Handles processing of a system command.                        */
/*             Uses the 'fork' and 'exec' system calls to create a new process*/
/*             and invoke the shell indicated by the local_env_type argument. */
/*             This is modeled after command handling done in Classic REXX.   */
/******************************************************************************/
RexxObjectPtr RexxEntry systemCommandHandler(RexxExitContext *context, RexxStringObject address, RexxStringObject command, RexxIORedirectorContext *ioContext)
{
    const char *cmd = context->StringData(command);
    const char *envName = context->StringData(address);

    RexxObjectPtr rc = NULLOBJECT;

    /* check for redirection symbols, ignore them when enclosed in double quotes.
       escaped quotes are ignored. */
    bool noDirectInvoc = false;
    bool inQuotes = false;
    bool escape = false;
    size_t i;
    for (i = 0; i<strlen(cmd); i++)
    {
        if (escape)
        {
            escape = false;
        }
        else if (cmd[i] == '\\')
        {
            escape = true;
        }
        else if (cmd[i] == '"')
        {
            inQuotes = !inQuotes;
        }
        else
        {
            /* if we're in the unquoted part and the current character is one of */
            /* the redirection characters or the & for multiple commands then we */
            /* will no longer try to invoke the command directly                 */
            if (!inQuotes && (strchr("<>|&", cmd[i]) != NULL))
            {
                noDirectInvoc = true;
                break;
            }
        }
    }

    if (!noDirectInvoc)
    {
        /* execute 'cd' in the same process */
        size_t commandLen = strlen(cmd);

        if (strcmp(cmd, "cd") == 0)
        {
            if (sys_process_cd(context, cmd, rc))
            {
                return rc;
            }
        }
        else if (commandLen >= 3)
        {
            char tmp[16];
            strncpy(tmp, cmd, 3);
            tmp[3] = '\0';
            if (strcmp("cd ",tmp) == 0)
            {
                if (sys_process_cd(context, cmd, rc))
                {
                    return rc;
                }
            }
            strncpy(tmp, cmd, 4);
            tmp[4] = '\0';
            if (strcmp("set ",tmp) == 0)
            {
                if (sys_process_export(context, cmd, rc, SET_FLAG)) /*unset works fine for me*/
                {
                    return rc;
                }
            }
            strncpy(tmp, cmd, 6);
            tmp[6] = '\0';
            if (strcmp("unset ", tmp) == 0)
            {
                if (sys_process_export(context, cmd, rc, UNSET_FLAG))
                {
                    return rc;
                }
            }
            strncpy(tmp, cmd, 7);
            tmp[7] = '\0';
            if (strcmp("export ", tmp) == 0)
            {
                if (sys_process_export(context, cmd, rc, EXPORT_FLAG))
                {
                    return rc;
                }
            }
        }
    }


    /****************************************************************************/
    /* Invoke the system command handler to execute the command                 */
    /****************************************************************************/
    // if this is the null string, then use the default address environment
    // for the platform
    if (strlen(envName) == 0)
    {
        envName = SYSINITIALADDRESS;
    }

    int errCode = 0;
    {
        int pid = fork();
        int status;

        if (pid != 0)                         /* spawn a child process to run the  */
        {
            waitpid ( pid, &status, 0);          /* command and wait for it to finish */
            if (WIFEXITED(status))               /* If cmd process ended normal       */
            {
                                                 /* Give 'em the exit code            */
                errCode = WEXITSTATUS(status);
            }
            else                              /* Else process died ugly, so        */
            {
                errCode = -(WTERMSIG(status));
                if (errCode == 1)                    /* If process was stopped            */
                {
                    errCode = -1;                   /* Give 'em a -1.                    */
                }
            }
        }
        else // we're in the spawned child process
        {
            if (Utilities::strCaselessCompare("sh", envName) == 0)
            {
                execl(SYSSHELLPATH "/sh", "sh", "-c", cmd, NULL);
            }
            else if (Utilities::strCaselessCompare("bash", envName) == 0)
            {
                execl(SYSSHELLPATH "/bash", "bash", "-c", cmd, NULL);
            }
            else if (Utilities::strCaselessCompare("ksh", envName) == 0)
            {
                execl(SYSSHELLPATH "/ksh", "ksh", "-c", cmd, NULL);
            }
            else if (Utilities::strCaselessCompare("bsh", envName) == 0)
            {
                execl(SYSSHELLPATH "/bsh", "bsh", "-c", cmd, NULL);
            }
            else if (Utilities::strCaselessCompare("csh", envName) == 0)
            {
                execl(SYSSHELLPATH "/csh", "csh", "-c", cmd, NULL);
            }
            else if (Utilities::strCaselessCompare("noshell", envName) == 0)
            {
                char * args[MAX_COMMAND_ARGS+1];      /* Array for argument parsing */
                if (scan_cmd(cmd, args, 0))           /* Parse cmd into arguments   */
                {
                    execvp(args[0], args);            /* Invoke command directly    */
                }
            }
            else // "command" environment (or anything else not covered by above)
            {
                execl(SYSSHELLPATH "/sh", "sh", "-c", cmd, NULL);
            }

            // if the above exec..() calls are successful, the process image is
            // replaced with the command to be executed and execution will never
            // reach this point
            // but if an exec..() call fails to run, we must EXIT this child process
            exit(UNKNOWN_COMMAND);
        }
    }
    // unknown command code?
    if (errCode == UNKNOWN_COMMAND)
    {
        // failure condition
        context->RaiseCondition("FAILURE", context->String(cmd), NULL, context->WholeNumberToObject(errCode));
    }
    else if (errCode != 0)
    {
        // non-zero is an error condition
        context->RaiseCondition("ERROR", context->String(cmd), NULL, context->WholeNumberToObject(errCode));
    }
    return context->False();      // zero return code
}


/**
 * Raise failure.
 *
 * @param ..
 */
RexxObjectPtr ErrorCommandFailure(RexxExitContext *context, CSTRING command, int errCode)
{
    context->RaiseCondition("FAILURE", context->String(command), NULLOBJECT, context->WholeNumberToObject(errCode));
    return NULLOBJECT;
}



/**
 * A redirecting test command handler.
 *
 * @param context    The Exit context.
 * @param address    The environment name.
 * @param command    The command name and arguments.
 * @param ioContext  The IO Redirector context.
 */
RexxObjectPtr RexxEntry ioCommandHandler(RexxExitContext *context, RexxStringObject address, RexxStringObject command, RexxIORedirectorContext *ioContext)
{
// @@ TO DO
// check interleaved output/error
// supprt other shells
// UNKNOWN_COMMAND -> rc from call

    CSTRING commandString = context->CString(command);

/*
    char* argv[MAX_COMMAND_ARGS + 1];
printf("io entry\n");
    // parse command string into argv array, starting at index 2 to leave
    // room for "/bin/sh" and "-c"
    if (scan_cmd(commandString, argv, 2)) // parse commandString into argv[]
    {
    else
    {   // too many arguments
        return ErrorCommandFailure(context, commandString, UNKNOWN_COMMAND);
    }
*/
    int pid;
    int status;
    char* argv[4];
    argv[0] = (char*) "/bin/sh";
    argv[1] = (char*) "-c";
    argv[2] = (char*) commandString;
    argv[3] = NULL;
printf("io args %s %s %s\n", argv[0], argv[1], argv[2]);
    if (ioContext->IsRedirectionRequested())
    {
printf("io redirection requested\n");
        bool need_to_write_input = false;
        bool need_to_read_output = false;
        bool need_to_read_error = false;
        int input[2], output[2], error[2];
        posix_spawn_file_actions_t action;
        posix_spawn_file_actions_init(&action);

        // Create stdin, stdout, and stderr pipes as requested.  pipe() returns
        // two file descriptors: [0] is the pipe read end, [1] is the pipe
        // write end.  The child process (i. e. the command to be run) will
        // inherit both the read and write end of all created pipes.  Of each
        // end *we* will only ever use one end, and the child the other end.
        // Therefore we and the child will close any unused ends.  In addition,
        // we also prepare any file actions that are to be be done by the child.
        // These actions are collected with posix_spawn_file_actions()

        // is stdin redirection requested?
        if (ioContext->IsInputRedirected())
        {
printf("io input redirection requested\n");
            if (pipe2(input, O_NONBLOCK) != 0) // create stdin pipe
//            if (pipe(input) != 0) // create stdin pipe
            {
                return ErrorCommandFailure(context, commandString, UNKNOWN_COMMAND);
            }
            posix_spawn_file_actions_adddup2(&action, input[0], 0); // stdin reads from pipe
            posix_spawn_file_actions_addclose(&action, input[1]); // close unused write end in child
            need_to_write_input = true;
        }

        // is stdout redirection requested?
        if (ioContext->IsOutputRedirected())
        {
printf("io output redirection requested\n");
            if (pipe2(output, O_NONBLOCK) != 0) // create stdout pipe
//            if (pipe(output) != 0) // create stdout pipe
            {
                return ErrorCommandFailure(context, commandString, UNKNOWN_COMMAND);
            }
            // do we want interleaved stdout and stderr redirection?
            // this is a special case .. we just redirect stderr to stdout upfront
            if (ioContext->AreOutputAndErrorSameTarget())
            {
printf("io output and error are the same\n");
                posix_spawn_file_actions_adddup2(&action, output[1], 2); //stderr writes to pipe
            }
            posix_spawn_file_actions_adddup2(&action, output[1], 1); //stdout writes to pipe
            posix_spawn_file_actions_addclose(&action, output[0]); // close unused read end in child
            need_to_read_output = true;
        }

        // now stderr redirection
        // if both stdout and stderr are to be redireted to the same object, then
        // everything was already setup in the previous stdout redirection step
        if (ioContext->IsErrorRedirected() && !ioContext->AreOutputAndErrorSameTarget())
        {
printf("io error redirection requested\n");
            if (pipe2(error, O_NONBLOCK) != 0) // create stderr pipe
//            if (pipe(error) != 0) // create stderr pipe
            {
                return ErrorCommandFailure(context, commandString, UNKNOWN_COMMAND);
            }
            posix_spawn_file_actions_adddup2(&action, error[1], 2); // stderr writes to pipe
            posix_spawn_file_actions_addclose(&action, error[0]); // close unused read end in child
            need_to_read_error = true;
        }

        // ok, everything is setup accordingly, let's fork the redirected command
        if (posix_spawnp(&pid, argv[0], &action, NULL, argv, NULL) != 0)
            {
                return ErrorCommandFailure(context, commandString, UNKNOWN_COMMAND);
            }
printf("io spawned command\n");

        // Let's close all unneeded read- and write ends, but ...
        // in a scenario, where we have redirected input for a command that
        // really doesn't need any or much less of it, the command may
        // finish before we could pipe all stdin input.  The spawned child
        // exits, closing its stdin read end.  At least on Ubuntu 16.04,
        // the following write() operation, instead of failing with an errno
        // indication, will exit(!) the program without any indication for
        // unknown reasons.  To avoid this, we don't close our read end now,
        // which should keep the pipe open and alive for write().
        /*
        if (need_to_write_input)
        {
            close(input[0]); // we close our unused stdout pipe write end
        }
        */
        if (need_to_read_output)
        {
            close(output[1]); // we close our unused stdout pipe write end
        }
        if (need_to_read_error)
        {
            close(error[1]); // we close our unused stderr pipe write end
        }

size_t IO_PIPE_BUF = PIPE_BUF; //@@ set our pipe buffer size
        CSTRING input_buffer;
        size_t input_offset = 0;
        size_t input_length;
        if (need_to_write_input)
        {
            // get all input data from the WITH INPUT clause
            ioContext->ReadInputBuffer(&input_buffer, &input_length);
printf("io input feed total buffer %zd bytes\n", input_length);
        }

int n = 0; //@@ loop counter; avoid endless loop
        // now with the command forked and running, we start our pipe loop:
        // with non-blocking reads and writes, we try to 1) write as much as
        // possible until an error occurs, then 2) read from output until an
        // error indication, and 3) read as much as possible from error.
//@@ should we check if our spawned child is stll alive?
        // we then wait a little and loop again
        while (need_to_write_input || need_to_read_output || need_to_read_error)
        {
n++; if (n>5000) break; //@@ avoid endless loop
printf("io loop #%d, input:%d output:%d error:%d\n", n, need_to_write_input, need_to_read_output, need_to_read_error);
            // do we need to feed it any input lines?
            if (need_to_write_input)
            {
                size_t length;
                ssize_t written;

                // generally, we're using PIPE_BUF for read() and write() buffer
                // sizes, as this ensures atomic reads/writes.  According to
                // POSIX.1 PIPE_BUF is at least 512 bytes; on Linux it's 4096 bytes.

                // this is either the first chunk, or the last chunk which
                // failed to write on the previous loop
                length = input_length - input_offset >= IO_PIPE_BUF ? IO_PIPE_BUF : input_length - input_offset;
                written = write(input[1], &input_buffer[input_offset], length);
printf("io input feed write offset %zd, length %zd, written %zd bytes \n", input_offset, length, written);
                // With atomic writes we really should only get either all bytes
                // written (which might be zero just for the first write() if
                // input_length = 0) or -1 as error indication.
                // Repeat loop until write() fails or we exhaust our input buffer.
                while (written > 0 && input_offset + IO_PIPE_BUF < input_length)
                {
                    input_offset += IO_PIPE_BUF; // chunk size is IO_PIPE_BUF
                    length = input_length - input_offset >= IO_PIPE_BUF ? IO_PIPE_BUF : input_length - input_offset;
                    written = write(input[1], &input_buffer[input_offset], length);
printf("io input feed write offset %zd, length %zd, written %zd bytes \n", input_offset, length, written);
                }
if (written < 0) printf("io input feed write failure %s\n", strerror(errno));
                // we retry only if this is just a temporary failure, otherwise
                // this is either a FAILURE condition, or success
//@@ don't retry if our spawned child has died
//@@ waitpid(pid, &status, WNOHANG) == 0 &&
                if (written == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                {   // just retry once more
                    ;
                }
                else if (written == -1)
                {   // for any other errno we raise FAILURE
                    return ErrorCommandFailure(context, commandString, errno);
printf("io input FAILURE %d\n", errno);
                }
                else
                {   // success
                    need_to_write_input = false; // we won't retry no more
                    close(input[1]); // we're finished with stdin
                    close(input[0]); // we also close our unused stdin pipe read end
printf("io input feed closed\n");
                }
            }

            // do we need to read OUTPUT from the stdout pipe?
            // if we have stdout and sterr interleaved, we'll read both of them here
            if (need_to_read_output)
            {
                char data[IO_PIPE_BUF];
                // read pipe in chunks until read() fails
                ssize_t bytes = read(output[0], data, IO_PIPE_BUF);
                while (bytes > 0)
                {
printf("io output buffer %zd bytes read\n", bytes);
                    ioContext->WriteOutputBuffer(data, bytes);
                    bytes = read(output[0], data, IO_PIPE_BUF);
                }
printf("io output buffer %zd bytes read\n", bytes);
                // we retry only if this is just a temporary failure, otherwise
                // this is either a FAILURE condition, or an EOF and we're done
                if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                {   // just retry once more
printf("io output retry: %s\n", strerror(errno));
                    ;
                }
                else if (bytes == -1)
                {   // for any other errno we raise FAILURE
printf("io output FAILURE: %s\n", strerror(errno));
                    return ErrorCommandFailure(context, commandString, errno);
                }
                else //
                {   // bytes == 0, EOF on pipe, success
                    need_to_read_output = false;
                    close(output[0]); // we're finished with stdout
printf("io output feed closed\n");
                }
            }

            // do we need to read ERROR lines from the stderr pipe?
            // interleaved stout/stderr output has already been covered above
            if (need_to_read_error)
            {
                char data[IO_PIPE_BUF];
                // read pipe in chunks until read() fails
                ssize_t bytes = read(error[0], data, IO_PIPE_BUF);
                while (bytes > 0)
                {
printf("io error buffer %zd bytes read\n", bytes);
                    ioContext->WriteErrorBuffer(data, bytes);
                    bytes = read(error[0], data, IO_PIPE_BUF);
                }
printf("io error buffer %zd bytes read\n", bytes);
                // we retry only if this is just a temporary failure, otherwise
                // this is either a FAILURE condition, or an EOF and we're done
                if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                {   // just retry once more
printf("io error retry: %s\n", strerror(errno));
                    ;
                }
                else if (bytes == -1)
                {   // for any other errno we raise FAILURE
printf("io error FAILURE %s\n", strerror(errno));
                    return ErrorCommandFailure(context, commandString, errno);
                }
                else //
                {   // bytes == 0, EOF on pipe, success
                    need_to_read_error = false;
                    close(error[0]); // we're finished with stderr
printf("io error feed closed\n");
                }
            }

            // we sleep 250 microseconds between retries
            if (need_to_write_input || need_to_read_output || need_to_read_error)
            {
                timeval time;
                time.tv_sec = 0;
                time.tv_usec = 250; // 250 microseconds
                select(0, NULL, NULL, NULL, &time); // select() seems to be most portable
printf("io loop wait 250 usec done\n");
            }
        } // END while (need_to_write_input || need_to_read_output || need_to_read_error)

        posix_spawn_file_actions_destroy(&action);
    }
    else
    {   // no rediretcion requested
printf("io no redirection\n");
        if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, NULL) != 0)
        {
printf("io spawnp != 0, %s\n", strerror(errno));
            return ErrorCommandFailure(context, commandString, UNKNOWN_COMMAND);
        }
    }

    // wait for our child, the forked command
printf("io waitpid(%d) = %d\n", pid,
    waitpid(pid, &status, 0));
    int rc;
    if (WIFEXITED(status)) // child process ended normal
    {
        rc = WEXITSTATUS(status); // set return code
printf("io child ended with rc %d\n", rc);
    }
    else
    {   // child process died
        rc = -(WTERMSIG(status));
        if (rc == 1) // process was stopped
        {
            rc = -1;
printf("io child died");
        }
    }

    if (rc != 0)
    {
        // non-zero is an error condition
        context->RaiseCondition("ERROR", context->String(commandString), NULL, context->WholeNumberToObject(rc));
    }
printf("io returning\n");
    return context->False(); // zero return code
}


/**
 * Register the standard system command handlers.
 *
 * @param instance The created instance.
 */
void SysInterpreterInstance::registerCommandHandlers(InterpreterInstance *_instance)
{
    // Unix has a whole collection of similar environments, services by a single handler
    _instance->addCommandHandler("COMMAND", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);
    _instance->addCommandHandler("", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);
    _instance->addCommandHandler("SH", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);
    _instance->addCommandHandler("KSH", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);
    _instance->addCommandHandler("CSH", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);
    _instance->addCommandHandler("BSH", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);
    _instance->addCommandHandler("BASH", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);
    _instance->addCommandHandler("NOSHELL", (REXXPFN)systemCommandHandler, HandlerType::REDIRECTING);

    _instance->addCommandHandler("IO", (REXXPFN)ioCommandHandler, HandlerType::REDIRECTING);
}

