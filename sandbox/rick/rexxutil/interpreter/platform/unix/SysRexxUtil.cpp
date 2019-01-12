/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2019 Rexx Language Association. All rights reserved.    */
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
/* REXX unix support                                                          */
/*                                                                            */
/* unix-based system utility functions                                        */
/*                                                                            */
/******************************************************************************/
/**********************************************************************
*                                                                     *
*   This program extends the REXX language by providing many          *
*   REXX external functions.                                          *
*   These are a partial list of functions included:                   *
*       SysCls              -- Clear the screen in an OS/2 fullscreen *
*                              or windowed command prompt session.    *
*       SysCurPos           -- Set and/or Query the cursor position   *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysCurState         -- Make the cursor visible or invisible   *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysDriveInfo        -- Returns information about a specific   *
*                              drive.                                 *
*       SysDriveMap         -- Returns a list of the drives on the    *
*                              machine                                *
*       SysDropFuncs        -- Makes all functions in this package    *
*                              unknown to REXX.                       *
*       SysFileDelete       -- Deletes a file                         *
*       SysFileSearch       -- Searches for a file matching a given   *
*                              filespec.                              *
*       SysFileTree         -- Searches for files matching a given    *
*                              filespec, including files in           *
*                              subdirectories.                        *
*       SysGetKey           -- Returns one by of data indicating the  *
*                              key which was pressed,                 *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysGetMessage       -- Retrieves a message text from an OS/2  *
*                              message file, substituting values      *
*                              provided.                              *
*       SysIni              -- Reads and/or updates entries in .INI   *
*                              files.                                 *
*       SysLoadFuncs        -- Makes all functions in this package    *
*                              known to REXX so REXX programs may     *
*                              call them.                             *
*       SysMkDir            -- Creates a directory                    *
*       SysVersion          -- Returns the system  Version number     *
*       SysLinVer           -- Returns the Linux Version number       *
*       SysRmDir            -- Removes a directory                    *
*       SysSearchPath       -- Searches throught a specified path     *
*                              for a file.                            *
*       SysSleep            -- Suspends execution for a number of     *
*                              seconds.                               *
*       SysTempFilename     -- Creates a unique filename              *
*       SysTextScreenRead   -- Reads characters from the screen,      *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysTextScreenSize   -- Returns the size of the window in      *
*                              rows and columns,                      *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*                              for a file.                            *
*                              for a file.                            *
*       SysCreateMutexSem   -- Create a Mutex semaphore               *
*       SysOpenMutexSem     -- Open a Mutex semaphore                 *
*       SysCloseMutexSem    -- Close a Mutex semaphore                *
*       SysRequestMutexSem  -- Request a Mutex semaphore              *
*       SysReleaseMutexSem  -- Release a Mutex semaphore              *
*       SysCreateEventSem   -- Create an Event semaphore              *
*       SysOpenEventSem     -- Open an Event semaphore                *
*       SysCloseEventSem    -- Close an Event semaphore               *
*       SysPostEventSem     -- Post an Event semaphore                *
*       SysResetEventSem    -- Reset an Event semaphore               *
*       SysWaitEventSem     -- Wait on an Event semaphore             *
*       SysAddRexxMacro     -- Load program into macro space          *
*       SysDropRexxMacro    -- Drop program from macro space          *
*       SysReorderRexxMacro -- Reorder program in macro space         *
*       SysQueryRexxMacro   -- Query ordering of macro space program  *
*       SysClearRexxMacroSpace -- Remove all programs from macro space*
*       SysLoadRexxMacroSpace  -- Load a Rexx macro space             *
*       SysSaveRexxMacroSpace  -- Save a Rexx macro space             *
*                              to the destination folder              *
*                              to the destination folder              *
*                              (Merlin only).                         *
*                              (for SysSwitchSession)                 *
*       SysDumpVariables    -- Dump current variables to a file       *
*       SysSetFileDateTime  -- Set the last modified date of a file   *
*       SysGetFileDateTime  -- Get the last modified date of a file   *
*       SysStemSort         -- sort a stem array                      *
*       SysStemDelete       -- delete items in a stem array           *
*       SysStemInsert       -- insert items into a stem array         *
*       SysStemCopy         -- copy items from one stem array to other*
*       SysGetErrortext     -- Retrieve textual desc. of error number *
*       SysQueryProcess     -- Get information on current proc/thread *
*       SysUtilVersion      -- query version of REXXUTIL.DLL          *
*                                                                     *
*       SysAddFuncPkg       -- CREXX for AIX function support         *
*       SysAddCmdPkg        -- CREXX for AIX function support         *
*       SysDropFuncPkg      -- CREXX for AIX function support         *
*       SysDropCmdPkg       -- CREXX for AIX function support         *
*       SysGetpid           -- CREXX for AIX function support         *
*       SysFork             -- CREXX for AIX function support         *
*       SysWait             -- CREXX for AIX function support         *
*       SysCreatePipe       -- CREXX for AIX function support         *
*                                                                     *
*       SysFileCopy         -- Copy files on the file system          *
*       SysFileMove         -- Move / Rename files or directories     *
*       SysIsFile           -- does file exist?                       *
*       SysIsFileDirectory  -- is file a subdirectory?                *
*       SysIsFileLink       -- is file a link?                        *
*                                                                     *
**********************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined( HAVE_LOCALE_H )
# include <locale.h>
#endif

#include "oorexxapi.h"

#if defined( HAVE_SYS_WAIT_H )
# include <sys/wait.h>
#endif

#include <sys/ipc.h>
#include <memory.h>

#if defined( HAVE_MALLOC_H )
# include <malloc.h>
#endif

#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <limits.h>
#include <sys/stat.h>                  /* mkdir() function           */
#include <errno.h>                     /* get the errno variable     */
#include <stddef.h>
#include <sys/types.h>
#if !defined(AIX)
#include <sys/syscall.h>
#endif
#include <sys/utsname.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <netdb.h>


#if defined( HAVE_SYS_SEM_H )
# include <sys/sem.h>
#endif

#include <dirent.h>                    /* directory functions        */
#include <sys/time.h>                  /* needed for the select func */

#include <time.h>                 /* needed for the select func          */

#if defined( HAVE_SYS_SELECT_H )
# include <sys/select.h>           /* needed for the select func          */
#endif

#if defined( HAVE_SYS_LDR_H )
# include <sys/ldr.h>              /* needed for the load   func          */
#endif

#if defined( HAVE_STRINGS_H )
# include <strings.h>
#endif

#include <utime.h>                /* moved, used by AIX & Linux          */

#if defined( HAVE_SYS_UTSNAME_H )
# include <sys/utsname.h>               /* get the uname() function   */
#endif

#include <signal.h>

#if defined( HAVE_SYS_RESOURCE_H )
# include <sys/resource.h>              /* get the getpriority() func */
#endif

#if defined( HAVE_FEATURES_H )
# include <features.h>                 /* catopen()                  */
#endif

#if defined( HAVE_NL_TYPES_H )
# include <nl_types.h>                  /* catopen()                  */
#endif

#include <termios.h>                   /* needed for SysGetKey       */
#include <fnmatch.h>                   /* fnmatch()                  */
#include <libgen.h>                    /* dirname, basename          */

#if !defined( HAVE_UNION_SEMUN )
union semun
{
   int val;
   struct semid_ds *buf;
   unsigned short *array;
};
#endif

#if defined __APPLE__
# define open64 open
// avoid warning: '(l)stat64' is deprecated: first deprecated in macOS 10.6
# define stat64 stat
# define lstat64 lstat
#endif

#define  MAX_DIGITS     9
#define  NO_UTIL_ERROR    "0"          /* No error whatsoever        */
#define  MAX_LINE_LEN   4096           /* max line length */
#define  MAX_READ       0x10000        /* full segment of buffer     */
#define  CH_EOF         0x1A           /* end of file marker         */
#define  CH_CR          '\r'           /* carriage return character  */
#define  CH_NL          '\n'           /* new line character         */
#define  MAX            256            /* temporary buffer length    */
#define  IBUF_LEN       4096           /* Input buffer length        */
#define CURRENT_DIR_FIRST 0            /* search flag 'C'            */
#define ENVIRONMENT_ONLY  1            /* search flag 'N'            */
#define OFFSET          1000           /* needed to prevent collision*/
                                       /* with the return codes      */
#define MAXUSECOUNT     65535          /* max semaphore usecount     */
#define REXXMESSAGEFILE    "rexx.cat"

// SysFileTree initial buffer sizes;
#define FNAMESPEC_BUF_LEN      IBUF_LEN
#define FOUNDFILE_BUF_LEN      IBUF_LEN
#define FILETIME_BUF_LEN       64
#define FILEATTR_BUF_LEN       16
#define FOUNDFILELINE_BUF_LEN  FOUNDFILE_BUF_LEN + FILETIME_BUF_LEN + FILEATTR_BUF_LEN

/*********************************************************************/
/* Numeric Error Return Strings                                      */
/*********************************************************************/

#define  NO_UTIL_ERROR    "0"          /* No error whatsoever        */
#define  ERROR_NOMEM      "2"          /* Insufficient memory        */
#define  ERROR_FILEOPEN   "3"          /* Error opening text file    */

/*********************************************************************/
/* Alpha Numeric Return Strings                                      */
/*********************************************************************/

#define  ERROR_RETSTR   "ERROR:"

/*********************************************************************/
/* Defines uses by SysTree                                           */
/*********************************************************************/

#define  RECURSE        0x0002
#define  DO_DIRS        0x0004
#define  DO_FILES       0x0008
#define  NAME_ONLY      0x0010
#define  EDITABLE_TIME  0x0020
#define  LONG_TIME      0x0040
#define  CASELESS       0x0080
#define  IGNORE         2              /* Ignore attributes entirely */


/******************************************************************************/
/* Defines used by SysGetKey                                                  */
/******************************************************************************/

#define stty(a,b)         (void)tcsetattr(a,TCSANOW,b) /* simple set attr.    */
#define gtty(a,b)         (void)tcgetattr(a,b)         /* simple get attr.    */
#define discard_input(a)  tcflush(a,TCIFLUSH)          /* simple flush        */
#define restore_tty(a)    stty(ttyfd,a)                /* new settings STDIN  */


/* original terminal settings                                                 */
struct termios in_orig;                /* original settings (important!!)     */


/*
 *  Data structure used with SysFileTree()
 *
 *  dFNameSpec, dFoundFile, and dFoundFileLine start out pointing to the static
 *  fNameSpec, foundFile, foundFileLine, buffers.
 *
 *  The nFNameSpec, nFoundFile, and nFoundLine, fields are set to the the byte
 *  count of the static buffers.
 *
 *  If any buffer turns out to be too small, memory is allocated for a larger
 *  buffer and the corresponding count variable is set to the new size.
 *
 *  On return from SysFileTree() any allocated memory in the struct is freed.
 *  To test if memory was or wasn't allocated, test if the count variable is not
 *  or is the same size as the static buffers.
 */
typedef struct RxTreeData
{
   size_t         count;                        // Number of found file lines
   RexxStemObject files;                        // Stem that holds results.
   char           fNameSpec[FNAMESPEC_BUF_LEN]; // File name portion of the search for file spec, may contain glob characters.
   char           foundFile[FOUNDFILE_BUF_LEN]; // Full path name of found file
   char           foundFileLine[FOUNDFILELINE_BUF_LEN];  // Buffer for found file line, includes foundFile, fileTime, and fileAttr
   char           fileTime[FILETIME_BUF_LEN];   // Time and size of found file
   char           fileAttr[FILEATTR_BUF_LEN];   // File attribute string of found file
   char          *dFNameSpec;                   // Starts out pointing at fNameSpec
   char          *dFoundFile;                   // Starts out pointing at foundFile
   char          *dFoundFileLine;               // Starts out pointing at foundFileLine
   size_t         nFNameSpec;                   // CouNt of bytes in dFNameSpec buffer
   size_t         nFoundFile;                   // CouNt of bytes in dFoundFile
   size_t         nFoundFileLine;               // CouNt of bytes in dFoundFileLine buffer
} RXTREEDATA;


/*********************************************************************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/*********************************************************************/


/*************************************************************************
* Function:  get_next_path                                               *
*                                                                        *
* Purpose:   Read one path out of an environment value pointed to by     *
*            ppenv.                                                      *
*            Used by the 'SearchPath' func.                              *
* Return:    0 - read successful                                         *
*            1 - end of environment entry                                *
*************************************************************************/
int get_next_path(
  char * *ppenv,                      /* environment pointer              */
  char * path_buf)                    /* path buffer                      */
{
  int i;                             /* counter                          */

  if(*ppenv == NULL)                 /* environment entry available ?    */
    return (1);                      /* return end of envrionment        */
  if(**ppenv == ':')                 /* if we point to the seperator     */
    (*ppenv)++;                      /* jump over                        */
  if(**ppenv == '\0')                /* if end of environment entry      */
    return (1);                      /* return so                        */
                                     /* copy the path out of the         */
                                     /* environment entry                */
  for(i=0;(**ppenv != ':')&&(**ppenv != '\0');(*ppenv)++){
    if(i>MAX_LINE_LEN)               /* if buffer to short               */
      return (1);                    /* get out                          */
    path_buf[i++] = **ppenv;         /* copy this character              */
  }
  path_buf[i] = '\0';                /* terminate the string             */
  return (0);                        /* found another path               */
}

/*************************************************************************
* Function:  SearchPath                                                  *
*                                                                        *
* Purpose:   Search a file along the given environment entry and return  *
*            the full filespec if found.                                 *
*                                                                        *
* Return:    0 - found the file(=>buf is modified)                       *
*            1 - not found                                               *
*************************************************************************/
int SearchPath(
  int   SearchFlag,                    /* search curr dir first ?    */
  const char * path,                   /* environment variable name  */
  const char * filename,               /* searched file              */
  char * buf,                          /* returned filespec if found */
  size_t   buf_size)                   /* size of the return buffer  */
{

  int   rc = 1;                        /* init rc to not found       */
  DIR   *dp;                           /* directory pointer          */
  struct dirent *ep;                   /* directory entry pointer    */
  int   length;                        /* path length                */
  char  path_buf[IBUF_LEN];            /* current search path        */
  char * penv;                          /* ptr to the environment     */

  if(!SearchFlag){                     /* search curr dir first ?    */
    dp = opendir("./");                /* open current directory     */
    if(dp != NULL){                    /* worked well ?              */
      while((ep = readdir(dp))){       /* while entries in the dir   */
                                       /* if we have a match         */
        if(!strcmp(ep->d_name,filename)){
          if(!getcwd(buf,buf_size))    /* copy the cwd to return buf */
            return rc;                 /* failed, get out            */
          length = strlen(buf);        /* get the length of the path */
          if((length+2+strlen(filename))>buf_size)/* check buf space */
            return rc;                 /* not enough, get out        */
          buf[length] = '/';           /* add a slash                */
          buf[length+1] = '\0';        /* and update the terminater  */
          strcat(buf,filename);        /* now complete the filespec  */
          rc = 0;                      /* Yep,found !                */
        }
      }
      (void)closedir(dp);              /* close the directory        */
    }
  }
  if(!rc)                              /* if we already found it     */
   return rc;                          /* return to caller           */

  /* Now search along the environment entry                          */
  penv = getenv(path);                 /* get the environment entry  */
  if(!penv)                            /* if couldn't get the env    */
    return rc;                         /* get out                    */
                                       /* while we have another path */
                                       /* to search for the file     */
  while(!get_next_path(&penv,path_buf)){
    dp = opendir(path_buf);            /* open the directory         */
    if(dp != NULL){                    /* worked well ?              */
      while((ep = readdir(dp))){       /* while entries in the dir   */
                                       /* if we have a match         */
        if(!strcmp(ep->d_name,filename)){
          if(strlen(path_buf)>buf_size)/* check the size             */
            return rc;                 /* get out                    */
          strcpy(buf,path_buf);        /* copy path to the return buf*/
          length = strlen(buf);        /* get the length of the path */
          if((length+2+strlen(filename))>buf_size)/* check buf space */
            return rc;                 /* not enough, get out        */
          buf[length] = '/';           /* add a slash                */
          buf[length+1] = '\0';        /* and update the terminater  */
          strcat(buf,filename);        /* now complete the filespec  */
          (void)closedir(dp);          /* close the directory        */
          return 0;                    /* Yep,found !                */
        }
      }
      (void)closedir(dp);              /* close the directory        */
    }
  }
  return rc;                           /* return not found           */
}


void restore_terminal(int signal){

  stty(STDIN_FILENO,&in_orig);          /* restore the terminal settings        */
  raise(signal);                        /* propagate signal                     */
}

/******************************************************************************/
/* getkey                                                                     */
/******************************************************************************/
/* echo == false => no echo                                                   */
/* echo == true  => echo                                                      */
/******************************************************************************/
int getkey(char *ret, bool echo)
{
    /* restore original TTY settings on exit */

    int ttyfd = STDIN_FILENO;              /* standard tty filedescriptor        */
  /* Set the cleanup handler for unconditional process termination              */
    struct sigaction new_action;


  /* Set up the structure to specify the new action                             */
    new_action.sa_handler = restore_terminal;
    sigfillset(&new_action.sa_mask);
    new_action.sa_flags = SA_RESTART;

  /* Termination signals                                                        */
    sigaction(SIGINT, &new_action, NULL);  /* exitClear on SIGINT signal          */
    sigaction(SIGTERM, &new_action, NULL); /* exitClear on SIGTERM signal         */
    sigaction(SIGQUIT, &new_action, NULL); /* exitClear on SIGQUIT signal         */
    sigaction(SIGHUP, &new_action, NULL);  /* exitClear on SIGHUP signal          */
    sigaction(SIGTSTP, &new_action, NULL); /* exitClear on SIGTSTP signal         */
    sigaction(SIGTTIN, &new_action, NULL); /* exitClear on SIGTTIN signal         */
    sigaction(SIGTTOU, &new_action, NULL); /* exitClear on SIGTTOU signal         */
  /* Error signals                                                              */
    sigaction(SIGSEGV, &new_action, NULL); /* exitClear on SIGSEGV signal         */
    sigaction(SIGFPE, &new_action, NULL);  /* exitClear on SIGFPE signal          */
    sigaction(SIGILL, &new_action, NULL);  /* exitClear on SIGILL signal          */
    sigaction(SIGBUS, &new_action, NULL);  /* exitClear on SIGBUS signal          */
    sigaction(SIGPIPE, &new_action, NULL); /* exitClear on broken pipe            */


    if (!isatty(ttyfd)) {                 /* connected to a terminal ?          */
      ret[0] = '\0';
      return 0;
    }
    ttyfd = STDIN_FILENO;                 /* STDIN_FILENO is out default fd     */

    /* open tty                           */
    ttyfd = open("/dev/tty", O_RDONLY);   /* get filedescriptor (fd) for tty    */

    struct termios in_raw;                /* global for save reset after SIGNAL */

    gtty(ttyfd, &in_orig);                /* determine existing tty settings */

    /* restore original TTY settings on exit */

    /* change STDIN settings to raw */
    gtty(ttyfd, &in_raw);                 /* save old settings                  */

    in_raw.c_lflag &= ~ICANON;            /* no canonical mode                  */
    if (!echo)                            /* no echo flag set                   */
      in_raw.c_lflag &= ~ECHO;            /* no echo                            */
    in_raw.c_cc[VMIN] = 1;                /* read 1 byte before returning       */
    in_raw.c_cc[VTIME] = 0;               /* return immediatly (no timeout)     */
    stty(ttyfd, &in_raw);                 /* execute settings now!              */


    ret[0] = getchar();                   /* read the char                      */

    ret[1] = '\0';                        /* terminate string                   */

    restore_tty(&in_orig);                /* for standard I/O behavior          */
    close(ttyfd);                         /* close the terminal                 */
    return 0;                             /* everything is fine                 */
}

/*************************************************************************
* Function:  SysSleep                                                    *
*                                                                        *
* Syntax:    call SysSleep secs                                          *
*                                                                        *
* Params:    secs - Number of seconds to sleep.                          *
*                   must be in the range 0 .. 999999999                  *
*                                                                        *
* Return:    0                                                           *
*************************************************************************/
RexxRoutine1(int, SysSleep, RexxStringObject, delay)
{
  double seconds;
  // try to convert the provided delay to a valid floating point number
  if (context->ObjectToDouble(delay, &seconds) == 0 ||
      isnan(seconds) || seconds == HUGE_VAL || seconds == -HUGE_VAL) {
    // 88.902 The &1 argument must be a number; found "&2"
    context->RaiseException2(Rexx_Error_Invalid_argument_number, context->String("delay"), delay);
    return 1;
  }

  // we (arbitrarily) limit the number of seconds to 999999999
  if (seconds < 0.0 || seconds > 999999999) {
    // 88.907 The &1 argument must be in the range &2 to &3; found "&4"
    context->RaiseException(Rexx_Error_Invalid_argument_range,
                            context->ArrayOfFour(context->String("delay"),
                                                 context->String("0"), context->String("999999999"), delay));
    return 1;
  }

  // split into two part: secs and nanoseconds
  long secs = (long)seconds;
  long nanoseconds = (long)((seconds - secs) * 1000000000);

#if defined( HAVE_NANOSLEEP )
  struct timespec    Rqtp, Rmtp;
  Rqtp.tv_sec = secs;
  Rqtp.tv_nsec = nanoseconds;
  nanosleep(&Rqtp, &Rmtp);
#elif defined( HAVE_NSLEEP )
  struct timestruc_t Rqtp, Rmtp;
  Rqtp.tv_sec = secs;
  Rqtp.tv_nsec = nanoseconds;
  nsleep(&Rqtp, &Rmtp);
#else
  sleep(secs);
#endif

  return 0;
}


/**********************************************************************
* Function:  SysCls                                                   *
*                                                                     *
* Syntax:    call SysCls                                              *
*                                                                     *
* Return:    NO_UTIL_ERROR - Successful.                              *
**********************************************************************/


RexxRoutine0(int, SysCls)
{
    system("clear");        /* do the clear               */
    return 0;
}


/*************************************************************************
* Function:  SysMkDir                                                    *
*                                                                        *
* Syntax:    call SysMkDir dir                                           *
*                                                                        *
* Params:    dir - Directory to be created.                              *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*                                                                        *
*************************************************************************/

RexxRoutine1(int, SysMkDir, CSTRING, path, OPTIONAL_int32_t, mode) {
  QualifiedName qualifiedName(path);

  /* Make the dir; standard permissions are rwxr-xr-x                */
  /* we do not restrict permission, this is done by root in the file */
  /* /etc/security/user. We allow anything. System restricts         */
  /* according to the user settings --> smitty/user                  */
  if (argumentOmitted(2)) {
    mode = S_IRWXU | S_IRWXG | S_IRWXO;
  }
  return mkdir(qualifiedName, mode);
}

/*************************************************************************
* Function:  SysSearchPath                                               *
*                                                                        *
* Syntax:    call SysSearchPath path, file [, options]                   *
*                                                                        *
* Params:    path - Environment variable name which specifies a path     *
*                    to be searched (ie 'PATH', 'DPATH', etc).           *
*            file - The file to search for.                              *
*            options -  'C' - Current directory search first (default).  *
*                       'N' - No Current directory search. Only searches *
*                             the path as specified.                     *
*                                                                        *
* Return:    other  - Full path and filespec of found file.              *
*            ''     - Specified file not found along path.               *
*************************************************************************/

RexxRoutine3(RexxStringObject, SysSearchPath, CSTRING, path, CSTRING, file, OPTIONAL_CSTRING, options)
{
  char     buf[IBUF_LEN]={0};          /* returned file name         */
  const char *      opts;                       /* option string              */
  int      SearchFlag;                 /* Search control variable    */
                                       /* default=search current 1st */
  int      ulRc;                       /* return value of SearchPath */


  SearchFlag = CURRENT_DIR_FIRST;      /* set default search flag    */

  if (options != NULL)                 /* process options            */
  {
    if ((*options == 'N') || (*options == 'n'))
    {
      SearchFlag = ENVIRONMENT_ONLY ;
    }
                                       /* do not search current dir  */

    else if ((*options == 'C') || (*options == 'c'))
    {
      SearchFlag = CURRENT_DIR_FIRST;
    }
                                       /* search current 1st(default)*/
    else
    {
      context->InvalidRoutine();
      return NULLOBJECT;
    }
  }

  ulRc = SearchPath(SearchFlag, path, file,
                       (char *)buf, sizeof(buf));

  /* if environment variable could not be found, try again with      */
  /* uppercase name.                                                 */
  if (ulRc)
  {
    char *temp = strdup(path);
    strupr(temp);
    ulRc = SearchPath(SearchFlag, temp, file,
                         (char *)buf, sizeof(buf));
    free(temp);
  }

  return context->NewStringFromAsciiz(retstr);
}

#ifdef LINUX
/*************************************************************************
* Function:  SysLinVer                                                   *
*                                                                        *
* Syntax:    call SysLinVer                                              *
*                                                                        *
* Return:    Linux Version                                               *
*************************************************************************/

RexxRoutine0(RexxStringObject, SysLinVer) {
  struct utsname info;                 /* info structur              */

  if ( uname(&info) < 0 ) {                     /* if no info stored          */
    context->InvalidRoutine();
    return NULLOBJECT;
  }

  char retstr[256];

  snprintf(retstr, sizeof(retstr), "%s %s", info.sysname, info.release);
  return context->NewStringFromAsciiz(retstr);
}
#endif

/*************************************************************************
* Function:  SysVersion                                                  *
*                                                                        *
* Syntax:    call SysVersion                                             *
*                                                                        *
* Return:    Operating System name (LINUX/AIX/WINDOWS) and Version       *
*************************************************************************/

RexxRoutine0(RexxStringObject, SysVersion) {
  if ( uname(&info) < 0 ) {                     /* if no info stored          */
    context->InvalidRoutine();
    return NULLOBJECT;
  }

  char retstr[256];

  snprintf(retstr, sizeof(retstr), "%s %s", info.sysname, info.release);
  return context->NewStringFromAsciiz(retstr);
}


/*************************************************************************
* Semaphore data struct                                                  *
*************************************************************************/

typedef struct RxSemData {
    bool          named;               /* Named semaphore?           */
    sem_t *       handle;              /* Semaphore pointer          */
} RXSEMDATA;


/*************************************************************************
* Function:  SysCreateEventSem                                           *
*                                                                        *
* Syntax:    handle = SysCreateEventSem(<name>)                          *
*                                                                        *
* Params:    name  - optional name for a event semaphore                 *
*                                                                        *
* Return:    handle - token used as a event sem handle for               *
*                     SysPostEventSem, SysClearEventSem,                 *
*                     SysCloseEventSem, and SysOpenEventSem              *
*            '' - Empty string in case of any error                      *
*************************************************************************/

RexxRoutine2(RexxObjectPtr, SysCreateEventSem, OPTIONAL_CSTRING, name, OPTIONAL_CSTRING, reset)
{
    RXSEMDATA *semdata;
    int rc;

    // Note that the reset arg has no meaning on Unix/Linux and is unused.
    semdata = (RXSEMDATA *)malloc(sizeof(RXSEMDATA));
    if (semdata == NULL) {
        return context->String("");
    }
    if (name == NULL) {
        /* this is an unnamed semaphore */
        semdata->handle = (sem_t *)malloc(sizeof(sem_t));
        rc = sem_init(semdata->handle, 0, 0);
        if (rc == -1) {
            free(semdata);
            return context->String("");
        }
        semdata->named = false;
    }
    else {
        /* this is a named semaphore */
        semdata->handle = sem_open(name, (O_CREAT | O_EXCL), (S_IRWXU | S_IRWXG), 0);
        if (semdata->handle == SEM_FAILED ) {
            free(semdata);
            return context->String("");
        }
        semdata->named = true;
    }
    return context->Uintptr((uintptr_t)semdata);
}


/*************************************************************************
* Function:  SysOpenEventSem                                             *
*                                                                        *
* Syntax:    result = SysOpenEventSem(handle)                            *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from DosOpenEventSem                   *
*************************************************************************/

RexxRoutine1(uintptr_t, SysOpenEventSem, CSTRING, name)
{
    RXSEMDATA *semdata;

    semdata = (RXSEMDATA *)malloc(sizeof(RXSEMDATA));
    if (semdata == NULL) {
        return 0;
    }
    semdata->handle = sem_open(name, 0);
    if (semdata->handle == SEM_FAILED ) {
        return 0;
    }
    semdata->named = true;
    return (uintptr_t)semdata;
}


/*************************************************************************
* Function:  SysResetEventSem                                            *
*                                                                        *
* Syntax:    result = SysResetEventSem(handle)                           *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from DosResetEventSem                  *
*************************************************************************/

RexxRoutine1(int, SysResetEventSem, uintptr_t, vhandle)
{
    RXSEMDATA *semdata = (RXSEMDATA *)vhandle;

    sem_init(semdata->handle, 1, 0);
    return 0;
}


/*************************************************************************
* Function:  SysPostEventSem                                             *
*                                                                        *
* Syntax:    result = SysPostEventSem(handle)                            *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from DosPostEventSem                   *
*************************************************************************/

RexxRoutine1(int, SysPostEventSem, uintptr_t, vhandle)
{
    RXSEMDATA *semdata = (RXSEMDATA *)vhandle;
    int rc;

    rc = sem_post(semdata->handle);
    if (rc) {
        return 6;
    }
    return 0;
}


/*************************************************************************
* Function:  SysCloseEventSem                                            *
*                                                                        *
* Syntax:    result = SysCloseEventSem(handle)                           *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from DosCloseEventSem                  *
*************************************************************************/

RexxRoutine1(int, SysCloseEventSem, uintptr_t, vhandle)
{
    RXSEMDATA *semdata = (RXSEMDATA *)vhandle;

    if (semdata->named == false) {
        /* this is an unnamed semaphore so we must free the target */
        if (sem_destroy(semdata->handle)) {
            if (errno == EINVAL) {
                return 6;
            }
            else if (errno) {
                return 102;
            }
        }
    }
    else {
        /* this is a named semaphore */
        if (sem_close(semdata->handle)) {
            if (errno == EINVAL) {
                return 6;
            }
            else if (errno) {
                return 102;
            }
        }
    }
    free(semdata);
    return 0;
}


#define SEM_WAIT_PERIOD 100 /* POSIX says this should be 10ms */


/*************************************************************************
* Function:  SysWaitEventSem                                             *
*                                                                        *
* Syntax:    result = SysWaitEventSem(handle, <timeout>)                 *
*                                                                        *
* Params:    handle - token returned from SysWaitEventSem                *
*                                                                        *
* Return:    result - return code from DosWaitEventSem                   *
*************************************************************************/

RexxRoutine2(int, SysWaitEventSem, uintptr_t, vhandle, OPTIONAL_int, timeout)
{
    RXSEMDATA *semdata = (RXSEMDATA *)vhandle;
    int rc = 0;

    if (timeout != 0) {
        /* this looping construct will cause us to wait longer than the */
        /* specified timeout due to the latency involved in the loop,   */
        /* but that cannot be helped                                    */
        while (timeout > 0) {
            rc = sem_trywait(semdata->handle);
            if (rc == 0) {
                break;
            }
            if (usleep(SEM_WAIT_PERIOD * 1000) == 0) {
                timeout -= SEM_WAIT_PERIOD;
            }
        }
    }
    else {
        rc = sem_wait(semdata->handle);
    }
    if (rc) {
        if (errno == EAGAIN) {
            return 121;
        }
        else if (errno == EINVAL) {
            return 6;
        }
    }
    return 0;
}


/*************************************************************************
* Function:  SysCreateMutexSem                                           *
*                                                                        *
* Syntax:    handle = SysCreateMutexSem(<name>)                          *
*                                                                        *
* Params:    name  - optional name for a event semaphore                 *
*                                                                        *
* Return:    handle - token used as a event sem handle for               *
*                     SysPostEventSem, SysClearEventSem,                 *
*                     SysCloseEventSem, and SysOpenEventSem              *
*            '' - Empty string in case of any error                      *
*************************************************************************/

RexxRoutine1(RexxObjectPtr, SysCreateMutexSem, OPTIONAL_CSTRING, name)
{
    RXSEMDATA *semdata;
    int rc;

    semdata = (RXSEMDATA *)malloc(sizeof(RXSEMDATA));
    if (semdata == NULL) {
        return context->String("");
    }
    if (strlen(name) == 0) {
        /* this is an unnamed semaphore */
        semdata->handle = (sem_t *)malloc(sizeof(sem_t));
        rc = sem_init(semdata->handle, 0, 0);
        if (rc == -1) {
            free(semdata);
            return context->String("");
        }
        semdata->named = false;
    }
    else {
        /* this is a named semaphore */
        semdata->handle = sem_open(name, (O_CREAT | O_EXCL), (S_IRWXU | S_IRWXG), 0);
        if (semdata->handle == SEM_FAILED ) {
            free(semdata);
            return context->String("");
        }
        semdata->named = true;
    }
    rc = sem_post(semdata->handle);
    return context->Uintptr((uintptr_t)semdata);
}


/*************************************************************************
* Function:  SysOpenMutexSem                                             *
*                                                                        *
* Syntax:    result = SysOpenMutexSem(handle)                            *
*                                                                        *
* Params:    handle - token returned from SysCreateMutexSem              *
*                                                                        *
* Return:    result - return code from DosOpenEventSem                   *
*************************************************************************/

RexxRoutine1(uintptr_t, SysOpenMutexSem, CSTRING, name)
{
    RXSEMDATA *semdata;

    semdata = (RXSEMDATA *)malloc(sizeof(RXSEMDATA));
    if (semdata == NULL) {
        return 0;
    }
    semdata->handle = sem_open(name, 0);
    if (semdata->handle == SEM_FAILED ) {
        return 0;
    }
    semdata->named = true;
    return (uintptr_t)semdata;
}


/*************************************************************************
* Function:  SysRequestMutexSem                                          *
*                                                                        *
* Syntax:    result = SysRequestMutexSem(handle, <timeout>)              *
*                                                                        *
* Params:    handle - token returned from SysRequestMutexSem             *
*                                                                        *
* Return:    result - return code from DosWaitEventSem                   *
*************************************************************************/

RexxRoutine2(int, SysRequestMutexSem, uintptr_t, vhandle, OPTIONAL_int, timeout)
{
    RXSEMDATA *semdata = (RXSEMDATA *)vhandle;
    int rc = 0;

    if (timeout != 0) {
        /* this looping construct will cause us to wait longer than the */
        /* specified timeout due to the latency involved in the loop,   */
        /* but that cannot be helped                                    */
        while (timeout > 0) {
            rc = sem_trywait(semdata->handle);
            if (rc == 0) {
                break;
            }
            if (usleep(SEM_WAIT_PERIOD * 1000) == 0) {
                timeout -= SEM_WAIT_PERIOD;
            }
        }
    }
    else {
        rc = sem_wait(semdata->handle);
    }
    if (rc) {
        if (errno == EAGAIN) {
            return 121;
        }
        else if (errno == EINVAL) {
            return 6;
        }
    }
    return 0;
}


/*************************************************************************
* Function:  SysReleaseMutexSem                                          *
*                                                                        *
* Syntax:    result = SysReleaseMutexSem(handle)                         *
*                                                                        *
* Params:    handle - token returned from SysCreateMutexSem              *
*                                                                        *
* Return:    result - return code from DosCloseEventSem                  *
*************************************************************************/

RexxRoutine1(int, SysReleaseMutexSem, uintptr_t, vhandle)
{
    RXSEMDATA *semdata = (RXSEMDATA *)vhandle;
    int rc;
    int val;

    rc = sem_getvalue(semdata->handle, &val);
    if (rc) {
        if (errno == EINVAL) {
            return 6;
        }
        else {
            return 288;
        }
    }
    if (val == 0) {
        rc = sem_post(semdata->handle);
        if (rc) {
            return 6;
        }
    }
    return 0;

}


/*************************************************************************
* Function:  SysCloseMutexSem                                            *
*                                                                        *
* Syntax:    result = SysCloseMutexSem(handle)                           *
*                                                                        *
* Params:    handle - token returned from SysCreateMutexSem              *
*                                                                        *
* Return:    result - return code from DosCloseEventSem                  *
*************************************************************************/

RexxRoutine1(int, SysCloseMutexSem, uintptr_t, vhandle)
{
    RXSEMDATA *semdata = (RXSEMDATA *)vhandle;

    if (semdata->named == false) {
        /* this is an unnamed semaphore so we must free the target */
        if (sem_destroy(semdata->handle)) {
            if (errno == EINVAL) {
                return 6;
            }
            else if (errno) {
                return 102;
            }
        }
    }
    else {
        /* this is a named semaphore */
        if (sem_close(semdata->handle)) {
            if (errno == EINVAL) {
                return 6;
            }
            else if (errno) {
                return 102;
            }
        }
    }
    free(semdata);
    return 0;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
 *                                                                            *
 *   SysFileTree() implmentation and helper functions.                        *
 *                                                                            *
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Enum for the different buffers in the RXTREEDATA struct */
typedef enum
{
    FILESPEC_BUFFER,
    FOUNDFILE_BUFFER,
    FOUNDFILELINE_BUFFER
} TreeDataBuffers;

/**
 * Returns a value that is greater than 'need' by doubling 'have' until that
 * value is reached.
 */
inline size_t neededSize(size_t need, size_t have)
{
    while ( have < need )
    {
        have *= 2;
    }
    return have;
}

void inline outOfMemoryException(RexxThreadContext *c)
{
    c->RaiseException1(Rexx_Error_System_service_user_defined, c->String("failed to allocate memory"));
}

/**
 * <routineName> argument <argPos> must not be a null string
 *
 * SysFileTree argument 2 must not be a null string
 *
 * @param c      Threade context we are operating in.
 * @param fName  Routine name.
 * @param pos    Argument position.
 */
void inline nullStringException(RexxThreadContext *c, CSTRING fName, size_t pos)
{
    c->RaiseException2(Rexx_Error_Incorrect_call_null, c->String(fName), c->StringSize(pos));
}

/**
 * <routineName>() argument <argPos> must be less than <len> characters in
 * length; length is <realLen>
 *
 * SysFileTree() argument 2 must be less than 4095 characters in length; length
 * is 5000
 *
 * Raises 88.900
 *
 * @param c        Thread context we are executing in.
 * @param pos      Argumet position
 * @param len      Fixed length
 * @param realLen  Actual length
 */
static void stringTooLongException(RexxThreadContext *c, CSTRING funcName, size_t pos, size_t len, size_t realLen)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "%s() argument %lu must be less than %lu characters in length; length is %lu",
              funcName, pos, len, realLen);

    c->RaiseException1(Rexx_Error_Incorrect_call_user_defined, c->String(buf));
}

/**
 * This is a SysFileTree specific function.
 *
 * @param c
 * @param pos
 * @param actual
 */
static void badSFTOptsException(RexxThreadContext *c, size_t pos, CSTRING actual)
{
    char buf[256] = {0};
    snprintf(buf, sizeof(buf),
             "SysFileTree argument %lu must be a combination of F, D, B, S, T, L, I, or O; found \"%s\"",
             pos, actual);

    c->RaiseException1(Rexx_Error_Incorrect_call_user_defined, c->String(buf));
}

/**
 * Dynamically allocates memory for a buffer in the RXTREEDATA structure so that
 * it is bigger than the size specified.
 *
 * @param need          The allocated buffer size must be greater than this.
 * @param treeData      Pointer to the RXTREEDATA struct.
 * @param whichBuffer   Identifies the buffer to increase.
 *
 * @return True on success, false if memory allocation fails.
 *
 * @remarks  For SysFileTree() we start out with very large static buffers to
 *           use for full path names and the maniupulation of path names.  But,
 *           on unixes, directories can be nested to any arbitary depth.
 *
 *           It is unlikely, but possible, that a static buffer will be too
 *           small.  If this happens, we double the size of the buffer until it
 *           is large enough by allocating a new buffer.
 */
static bool increaseBuffer(RexxCallContext *c, size_t need, RXTREEDATA *treeData, TreeDataBuffers whichBuffer)
{
    if ( whichBuffer == FILESPEC_BUFFER )
    {
        if ( treeData->nFNameSpec != FNAMESPEC_BUF_LEN )
        {
            free(treeData->dFNameSpec);
        }

        treeData->nFNameSpec = neededSize(need, treeData->nFNameSpec);
        treeData->dFNameSpec = (char *)malloc(sizeof(char) * treeData->nFNameSpec);

        if ( treeData->dFNameSpec == NULL )
        {
            outOfMemoryException(c->threadContext);
            return false;
        }
    }
    else if ( whichBuffer == FOUNDFILE_BUFFER )
    {
        if ( treeData->nFoundFile != FOUNDFILE_BUF_LEN )
        {
            free(treeData->dFoundFile);
        }

        treeData->nFoundFile = neededSize(need, treeData->nFoundFile);
        treeData->dFoundFile = (char *)malloc(sizeof(char) * treeData->nFoundFile);

        if ( treeData->dFoundFile == NULL )
        {
            outOfMemoryException(c->threadContext);
            return false;
        }
    }
    else
    {
        if ( treeData->nFoundFileLine != FOUNDFILELINE_BUFFER )
        {
            free(treeData->dFoundFileLine);
        }

        treeData->nFoundFile = neededSize(need, treeData->nFoundFile);
        treeData->dFoundFile = (char *)malloc(sizeof(char) * treeData->nFoundFile);

        if ( treeData->dFoundFile == NULL )
        {
            outOfMemoryException(c->threadContext);
            return false;
        }
    }

    return true;
}

/**
 * Allocates a buffer twice as big as the buffer passed in.
 *
 * @param c             Call context we are operating in.
 * @param dPath         Pointer to the buffer to reallocate
 * @param nPath         Size of dPath buffer.
 * @param nStaticBuffer Size of original static buffer.
 *
 * @return True on success, false on memory allocation failure.
 *
 * @remarks  NOTE: that the pointer to the buffer to reallocate, may, or may
 *           not, be a pointer to a static buffer.  We must NOT try to free a
 *           static buffer and we MUST free a non-static buffer.
 */
static bool getBiggerBuffer(RexxCallContext *c, char **dPath, size_t *nPath, size_t nStaticBuffer)
{
    if ( *nPath != nStaticBuffer )
    {
        free(*dPath);
    }

    *nPath *= 2;
    *dPath = (char *)malloc(*nPath * sizeof(char));

    if ( *dPath == NULL )
    {
        outOfMemoryException(c->threadContext);
        return false;
    }

    return true;
}

/**
 * This is a SysFileTree specific function.
 *
 * Test if struct stat.st_mode field is a file for the purpose of SysFileTree.
 *
 * @param m
 *
 * @return True if a file, otherwise
 */
inline bool isAcceptableFile(mode_t m)
{
    return (S_ISREG(m)    ||        // if it is a file
            S_ISCHR(m)    ||        // or a device special
            S_ISBLK(m)    ||        // file
            S_ISSOCK(m)   ||        // or a socket
            S_ISLNK(m)    ||        // or a symbolic link
            S_ISFIFO(m));           // or a FIFO
}

inline char typeOfEntry(mode_t m)
{
    if( S_ISLNK(m) )
    {
      return 'l';                // symbolic link
    }
    else if ( S_ISBLK(m) )
    {
      return 'b';                // block device
    }
    else if ( S_ISCHR(m) )
    {
      return 'c';                // character device
    }
    else if ( S_ISDIR(m) )
    {
      return 'd';                // directory
    }
    else if ( S_ISFIFO(m) )
    {
      return 'p';                // FIFO
    }
    else if ( S_ISSOCK(m) )
    {
      return 's';                // socket
    }
    else
    {
      return '-';                // regular file
    }
}

/**
 * This is a SysFileTree specific function.
 *
 * Find the next file in the directory pointed to by dir_handle that matches
 * fileSpec.  The needed information is returnd in finfo and d_name.
 *
 * @param fileSpec     Search specification.
 * @param path         Current path we are searching.
 * @param dir_handle   Directory handle.
 * @param finfo        Returned file info buffer.
 * @param d_name       Returned name of the file found
 * @param caseless     Do case insensitive matching, or not.
 *
 * @return True if a file is found, outherwise false.
 *
 * @remarks  The '?' glob character is not currently supported.  Old notes said
 *           add the implementation here.
 */
bool linFindNextFile(RexxCallContext *c, const char *fileSpec, const char *path, DIR *dir_handle,
                     struct stat *finfo, char **d_name, bool caseless)
{
    char    fullPath[IBUF_LEN];
    int     len;
    char   *dFullPath = fullPath;
    size_t  nFullPath = IBUF_LEN;

    struct dirent *dir_entry = readdir(dir_handle);
    if( dir_entry == NULL )
    {
        return false;
    }

    do
    {
        len = snprintf(dFullPath, nFullPath, "%s%s", path, dir_entry->d_name);
        if ( len >= (int)nFullPath )
        {
            if ( ! getBiggerBuffer(c, &dFullPath, &nFullPath, IBUF_LEN) )
            {
                outOfMemoryException(c->threadContext);
                if ( nFullPath != IBUF_LEN )
                {
                    free(dFullPath);
                }
                return false;
            }
        }

        lstat(dFullPath, finfo);

        if ( isAcceptableFile(finfo->st_mode) )
        {
            // If this is a caseless search, we compare an upper cased copy of
            // the entry name.  fileSpec has already been upper cased at a
            // higher level.
            if ( caseless )
            {
                char  dup_d_name[IBUF_LEN];
                char *pDest = dup_d_name;
                char *pSrc  = dir_entry->d_name;

                for ( ; *pSrc; pDest++, pSrc++ )
                {
                    *pDest = toupper(*pSrc);
                }
                *pDest = '\x0';

                if ( fnmatch(fileSpec, dup_d_name, FNM_NOESCAPE | FNM_PATHNAME | FNM_PERIOD ) == 0 )
                {
                    *d_name = dir_entry->d_name;

                    if ( nFullPath != IBUF_LEN )
                    {
                        free(dFullPath);
                    }
                    return true;
                }
            }
            else
            {
                if ( fnmatch(fileSpec, dir_entry->d_name, FNM_NOESCAPE | FNM_PATHNAME | FNM_PERIOD) == 0 )
                {
                    *d_name = dir_entry->d_name;

                    if ( nFullPath != IBUF_LEN )
                    {
                        free(dFullPath);
                    }
                    return true;
                }
            }
        }
        dir_entry = readdir(dir_handle);
    }
    while( dir_entry != NULL );

    if ( nFullPath != IBUF_LEN )
    {
        free(dFullPath);
    }

    return false;
}

/**
 * This is a SysFileTree specific function.
 *
 * Find the next directory in the directory pointed to by dir_handle that
 * matches fileSpec.  The needed information is returnd in finfo and d_name.
 *
 * @param fileSpec     Search specification.
 * @param path         Current path we are searching.
 * @param dir_handle   Directory handle.
 * @param finfo        Returned file info buffer.
 * @param d_name       Returned name of the directory found.
 * @param caseless     Do case insensitive matching, or not.
 *
 * @return True if a directory is found, outherwise false.
 *
 * @remarks  The '?' glob character is not currently supported.  Old notes said
 *           add the implementation here.
 */
bool linFindNextDir(RexxCallContext *c, const char *fileSpec, const char *path, DIR *dir_handle,
                     struct stat *finfo, char **d_name, bool caseless)
{
    char    fullPath[FOUNDFILE_BUF_LEN];
    char   *dFullPath = fullPath;
    size_t  nFullPath = FOUNDFILE_BUF_LEN;

    struct dirent *dir_entry = readdir(dir_handle);
    if( dir_entry == NULL )
    {
        return false;
    }

    do
    {
        int len = snprintf(dFullPath, nFullPath, "%s%s", path, dir_entry->d_name);
        if ( len >= (int)nFullPath )
        {
            if ( nFullPath != FOUNDFILE_BUF_LEN )
            {
                free(dFullPath);
            }
            nFullPath = neededSize(len, nFullPath);

            dFullPath = (char *)malloc(nFullPath * sizeof(char));
            if ( dFullPath == NULL )
            {
                outOfMemoryException(c->threadContext);
                return false;
            }

            sprintf(dFullPath, "%s%s", path, dir_entry->d_name);
        }

        lstat(fullPath, finfo);

        if ( S_ISDIR(finfo->st_mode) )
        {
            // If this is a caseless search, we compare an upper cased copy of
            // the entry name.  fileSpec has already been upper cased at a
            // higher level.
            if ( caseless )
            {
                char  dup_d_name[IBUF_LEN];
                char *pDest = dup_d_name;
                char *pSrc  = dir_entry->d_name;

                for ( ; *pSrc; pDest++, pSrc++ )
                {
                    *pDest = toupper(*pSrc);
                }
                *pDest = '\x0';

                if ( fnmatch(fileSpec, dup_d_name, FNM_NOESCAPE | FNM_PATHNAME | FNM_PERIOD ) == 0 )
                {
                    *d_name = dir_entry->d_name;
                    return true;
                }
            }
            else
            {
                if ( fnmatch(fileSpec, dir_entry->d_name, FNM_NOESCAPE | FNM_PATHNAME | FNM_PERIOD) == 0 )
                {
                    *d_name = dir_entry->d_name;
                    return true;
                }
            }
        }
        dir_entry = readdir(dir_handle);
    }
    while( dir_entry != NULL );

    return false;
}

/**
 * This is a SysFileTree specific function.
 *
 * Determines the options by converting the character based argument to the
 * correct set of flags.
 *
 * @param c
 * @param opts
 * @param pOpts
 *
 * @return bool
 */
static bool goodOpts(RexxCallContext *c, CSTRING opts, uint32_t *pOpts)
{
    uint32_t options = *pOpts;

    while ( *opts )
    {
        switch( toupper(*opts) )
        {
          case 'S':                      // recurse into subdirectories
              options |= RECURSE;
              break;

          case 'O':                      // only return names
              options |= NAME_ONLY;
              break;

          case 'T':                      // use short time format, ignored if L is used
            options |= EDITABLE_TIME;
            break;

          case 'L':                      // use long time format
              options |= LONG_TIME;
              break;

          case 'F':                      // include only files
              options &= ~DO_DIRS;
              options |= DO_FILES;
              break;

          case 'D':                      // include only directories
              options |= DO_DIRS;
              options &= ~DO_FILES;
              break;

          case 'B':                      // include both files and directories
              options |= DO_DIRS;
              options |= DO_FILES;
              break;

          case 'I':                      // case insensitive?
              options |= CASELESS;
              break;

          default:                       // error, unknown option
            return false;
        }
        opts++;
    }

    *pOpts = options;
    return true;
}

/**
 * This is a SysFileTree specific helper function.
 *
 * Checks the validity of the options argument to SysFileTree, sets the default
 * options, and converts the character based argument to the proper set of
 * flags.
 *
 * @param context
 * @param opts
 * @param options
 * @param argPos
 *
 * @return bool
 */
static bool getOptionsFromArg(RexxCallContext *context, CSTRING opts, uint32_t *options, size_t argPos)
{
    *options = DO_FILES | DO_DIRS;

    if ( argumentExists(argPos) )
    {
        if ( strlen(opts) == 0 )
        {
            nullStringException(context->threadContext, "SysFileTree", argPos);
            return false;
        }

        if ( ! goodOpts(context, opts, options) )
        {
            badSFTOptsException(context->threadContext, argPos, opts);
            return false;
        }
    }

    return true;
}


/**
 * Checks the file specification as input by the user for validity and expands /
 * redoes the value if needed.  For example if the the specification starts with
 * the ~ character, the file spec is expanded to include the full path to the
 * home directory.
 *
 * @param context   Call context we are operating in.
 *
 * @param fSpec     The file specification as passed by the user.
 *                  Can not be the empty string and must be 494 chars or less.
 *
 * @param fileSpec  Buffer to contain the expanded file specification.  At this
 *                  time the buffer is 4096 in length, which seems more than
 *                  sufficient.  However, we check the length when copying into
 *                  it anyway.
 *
 * @param bufLen    Length of fileSpec buffer.
 *
 * @param argPos    Argument position, use for raising conditions.
 *
 * @return True if no error, otherwise false.  False is only returned if an
 *         exception has been raised.
 *
 * @remarks  The fileSpec buffer is IBUF_LEN, or 4096 bytes.
 *           Theoretically, if fSpec starts with the tilde, '~', the string
 *           could be too long after expanding the home directoy.
 */
static bool getFileSpecFromArg(RexxCallContext *context, CSTRING fSpec, char *fileSpec, size_t bufLen, size_t argPos)
{
    size_t len = strlen(fSpec);
    if ( len == 0 )
    {
        nullStringException(context->threadContext, "SysFileTree", argPos);
        return false;
    }
    if ( len >= bufLen - 1) // take into account that a trailing "*" may be appended
    {
        stringTooLongException(context->threadContext, "SysFileTree", argPos, bufLen - 1, len);
        return false;
    }
    strcpy(fileSpec, fSpec);

    // If filespec is '*' then use './ *'
    if ( len == 1 && fileSpec[0] == '*' )
    {
        strcpy(fileSpec, "./*");
    }

    // If fileSpec ends in '/' then append  '*'
    if ( fileSpec[len - 1] == '/' )
    {
        strcat(fileSpec, "*");
    }

    // If filespec begins with '~' generate the absolute path.
    if ( fileSpec[0] == '~' )
    {
        char *temp = SysFileSystem::resolveTilde(fileSpec);
        if ( ! temp )
        {
            outOfMemoryException(context->threadContext);
            return false;
        }

        if ( strlen(temp) >= bufLen )
        {
            stringTooLongException(context->threadContext, "SysFileTree", argPos, bufLen, strlen(temp));
            free(temp);
            return false;
        }

        strcpy(fileSpec, temp);
        free(temp);
    }

    return true;
}

/**
 * Used to retrieve the file name portion of the search file specification the
 * user sent to SysFileTree.
 *
 * @param c
 * @param fileSpec
 * @param treeData
 * @param lastSlashPos
 *
 * @return True on success, false on a memory allocation error.
 */
static bool getFileNameSegment(RexxCallContext *c, char *fileSpec, RXTREEDATA *treeData, int *lastSlashPos)
{
    size_t l;                          // Temporay var for length calculations.
    int slashPos = 0;                  // Position of last slash in fileSpec.
    int len      = strlen(fileSpec);

    // Get maximum position of last '/' and then step back through fileSpec
    // until we are at the beginning of fileSpec, or at the last '/' character.
    slashPos = len - 1;
    do
    {
        slashPos--;
    }
    while( fileSpec[slashPos] != '/' && slashPos >= 0 );

    if ( fileSpec[slashPos] == '/' )
    {
        // We found a slash, if there are characters after the slash, they are
        // the file name portion.  Otherwise, juse use wildcards.
        if ( fileSpec[slashPos + 1] != '\0' )
        {
            l = strlen(&fileSpec[slashPos + 1]) + 1;
            if ( l > treeData->nFNameSpec )
            {
                if ( ! increaseBuffer(c, l, treeData, FILESPEC_BUFFER) )
                {
                    return false;
                }
            }
            strcpy(treeData->dFNameSpec, &fileSpec[slashPos + 1]);
        }
        else
        {
            strcpy(treeData->dFNameSpec, "*");
        }
    }
    else
    {
        // no '/', fileSpec is just a file name.

        l = strlen(&fileSpec[slashPos + 1]) + 1;
        if ( l > treeData->nFNameSpec )
        {
            if ( ! increaseBuffer(c, l, treeData, FILESPEC_BUFFER) )
            {
                return false;
            }
        }
        strcpy(treeData->dFNameSpec, &fileSpec[slashPos + 1]);
    }

    *lastSlashPos = slashPos;
    return true;
}

/**
 *
 *
 * @param c
 * @param fileSpec
 * @param path
 * @param pathLen
 * @param lastSlashPos
 *
 * @return bool
 */
static bool getPathSegment(RexxCallContext *c, char *fileSpec, char **path, size_t *pathLen, int lastSlashPos)
{
    char    savedPath[FOUNDFILE_BUF_LEN];   // Used to save current directory and return to it.
    char   *dPath = *path;
    size_t  nPath = *pathLen;

    if ( fileSpec[lastSlashPos] != '/' )
    {
        // We have no slash in fileSpec, so it can not be a relative or full
        // path name.  We resolve to the current directory.
        while ( getcwd(dPath, nPath) == NULL )
        {
            // The buffer is not big enough, reallocate a bigger buffer.
            if ( ! getBiggerBuffer(c, &dPath, &nPath, *pathLen) )
            {
                return false;
            }
        }

        if ( strlen(dPath) + 1 > nPath )
        {
            // The buffer is not big enough to concatenate a slash, reallocate
            // a bigger buffer.
            if ( ! getBiggerBuffer(c, &dPath, &nPath, *pathLen) )
            {
                return false;
            }
        }

        strcat(dPath, "/");
    }
    else
    {
        // There is a slash in fileSpec, so it is a relative or full path.
        // Copy the path out.
        size_t l = lastSlashPos + 1 + 1;
        if ( l > nPath )
        {
            nPath = neededSize(l, nPath);
            dPath = (char *)malloc(nPath * sizeof(char));

            if ( dPath == NULL )
            {
                outOfMemoryException(c->threadContext);
                return false;
            }
        }
        strncpy(dPath, fileSpec, lastSlashPos + 1);

        *(dPath + lastSlashPos + 1) = '\0';  // Terminate the string

        // At this point path could be a relative path.  We try to resolve it to
        // a full path by saving the current directory, doing a chdir() which
        // will resolve a relative path, and then copying that current directory
        // into path.  If we can not save the current directory or change
        // directory we just keep the relative path.
        if ( getcwd(savedPath, FOUNDFILE_BUF_LEN) != NULL )
        {
            if ( chdir(dPath) == 0 )
            {
                while ( getcwd(dPath, nPath) == NULL )
                {
                    // The buffer is not big enough, reallocate a bigger buffer.
                    if ( ! getBiggerBuffer(c, &dPath, &nPath, *pathLen) )
                    {
                        // Back to current directory.
                        int ignore = chdir(savedPath);
                        return false;
                    }
                }

                if ( strlen(dPath) + 1 > nPath )
                {
                    // The buffer is not big enough to concatenate a slash,
                    // reallocate a bigger buffer.
                    if ( ! getBiggerBuffer(c, &dPath, &nPath, *pathLen) )
                    {
                        return false;
                    }
                }

                if ( lastSlashPos > 0 )
                {
                    strcat(dPath, "/");
                }

                // Back to current directory.
                int ignore = chdir(savedPath);
            }
        }
    }

    if ( nPath != *pathLen )
    {
        *pathLen = nPath;
        *path    = dPath;
    }

    return true;
}

/**
 * This is a SysFileTree() specific function..
 *
 * This function expands the file spec passed in to the function into its full
 * path name.  The full path name is then split into the path portion and the
 * file name portion.  The path portion is returned in path and the file name
 * portion is returned in the RXTREEDATA structure.
 *
 * The path portion will end with the '\' character.
 *
 * @param c          Call context we are operating in.
 *
 * @param fSpec      File specification to search for.
 *
 * @param path       Pointer to the buffer for the returned full path of
 *                   expanded file specification.  This buffer may be
 *                   reallocated if it is not big enough
 *
 * @param pathLen    Pointer to the size of the passed in path buffer.  This
 *                   value will be reset if the path buffer is reallocated.
 *
 * @param treeData   The file name portion of the full path is copied into the
 *                   dFNameSpec field in this struct.
 *
 *
 * @return   True on success, false on error.  The only error would be if memory
 *           allocation failed, in which case a condition has been raised.  It
 *           seems highly unlikely that this function could fail.
 *
 * @remarks  Both the path and the file name destination buffers are checked to
 *           be sure they are big enough.  If either is not big enough, a larger
 *           buffer is allocated.
 */

static bool getPath(RexxCallContext *c, char *fileSpec, char **path, size_t *pathLen, RXTREEDATA *treeData)
{
    int  lastSlashPos;                   // Position of last slash in fileSpec.

    // If fileSpec is exactly "." or ".." then change it to "*" or "../*"
    if ( strcmp(fileSpec, ".") == 0 )
    {
        strcpy(fileSpec, "./*");
    }
    else if ( strcmp(fileSpec, "..") == 0 )
    {
        strcpy(fileSpec, "../*");
    }

    // Get the file name segment of fileSpec.
    if ( ! getFileNameSegment(c, fileSpec, treeData, &lastSlashPos) )
    {
        return false;
    }

    // Now resolve to the fully qualified path name.
    if ( ! getPathSegment(c, fileSpec, path, pathLen, lastSlashPos) )
    {
        return false;
    }

    return true;
}


/**
 * This is a SysFileTree() specific function..
 *
 * Formats a found file entry according to the specified options and adds it to
 * the stem containing the found files.
 *
 * @param c         Call context we are operating in.
 *
 * @param treeData  Struct with data related to finding the files.
 *
 * @param options   Options specifying how the found file line is to be
 *                  formatted.
 *
 * @param finfo     File info structure.
 *
 * @return False on error, otherwise true.
 *
 * @remarks  If the options specify name only, we just need to copy over the
 *           found file name.  Otherwise we need to format a line with the time
 *           stamp and file attributes, plus the found file name.
 *
 *           We use the buffers in the treeData structure to format the found
 *           file line. snprintf() is used to prevent and detect buffer
 *           overflows. We start off trying to use the large static buffers in
 *           the struct, each time a buffer is found to be too small in size, it
 *           is doubled in size by dynamically allocated memory.. Code at the
 *           top level detects and frees any allocated memory.
 *
 *           Note that we can count the characters used for both the time data
 *           and attribute data, so we know the buffers are large enough.
 *
 *           If the file search is a very deep recursion in the host file
 *           system, a very large number of String objects may be created in the
 *           single Call context of SysFileTree.  A reference to each created
 *           object is saved in a hash table to protect it from garbage
 *           collection, which can lead to a very large hash table.  To prevent
 *           the creation of a very large hash table, we create a temp object,
 *           pass that object to the interpreter, and then tell the interpreter
 *           the object no longer needs to be protected in this call context.
 */
bool formatFile(RexxCallContext *c, RXTREEDATA *treeData, uint32_t options, struct stat *finfo)
{
    struct tm *timestamp;
    char   tp;

    if ( options & NAME_ONLY )
    {
        if ( treeData->nFoundFileLine < treeData->nFoundFile )
        {
            if ( ! increaseBuffer(c, treeData->nFoundFile, treeData, FOUNDFILELINE_BUFFER) )
            {
                return false;
            }
        }
        strcpy(treeData->foundFileLine, treeData->foundFile);
    }
    else
    {
#ifdef AIX
        struct tm stTimestamp;
        timestamp = localtime_r(&(finfo->st_mtime), &stTimestamp);
#else
        timestamp = localtime(&(finfo->st_mtime));
#endif
        if ( options & LONG_TIME )
        {

            sprintf(treeData->fileTime, "%4d-%02d-%02d %02d:%02d:%02d  %10zu  ",
                      timestamp->tm_year + 1900,
                      timestamp->tm_mon + 1,
                      timestamp->tm_mday,
                      timestamp->tm_hour,
                      timestamp->tm_min,
                      timestamp->tm_sec,
                      finfo->st_size);
        }
        else
        {
            if ( options & EDITABLE_TIME )
            {
                sprintf(treeData->fileTime, "%02d/%02d/%02d/%02d/%02d  %10zu  ",
                          (timestamp->tm_year) % 100,
                          timestamp->tm_mon + 1,
                          timestamp->tm_mday,
                          timestamp->tm_hour,
                          timestamp->tm_min,
                          finfo->st_size);
            }
            else
            {
                sprintf(treeData->fileTime, "%2d/%02d/%02d  %2d:%02d%c  %10zu  ",
                          timestamp->tm_mon+1,
                          timestamp->tm_mday,
                          timestamp->tm_year % 100,
                          timestamp->tm_hour < 13 ? timestamp->tm_hour : timestamp->tm_hour - 12,
                          timestamp->tm_min,
                          (timestamp->tm_hour < 12 || timestamp->tm_hour == 24) ? 'a' : 'p',
                          finfo->st_size);
            }
        }
        tp = typeOfEntry(finfo->st_mode);

        sprintf(treeData->fileAttr, "%c%c%c%c%c%c%c%c%c%c  ",
                  tp,
                  (finfo->st_mode & S_IREAD)  ? 'r' : '-',
                  (finfo->st_mode & S_IWRITE) ? 'w' : '-',
                  (finfo->st_mode & S_IEXEC)  ? 'x' : '-',
                  (finfo->st_mode & S_IRGRP)  ? 'r' : '-',
                  (finfo->st_mode & S_IWGRP)  ? 'w' : '-',
                  (finfo->st_mode & S_IXGRP)  ? 'x' : '-',
                  (finfo->st_mode & S_IROTH)  ? 'r' : '-',
                  (finfo->st_mode & S_IWOTH)  ? 'w' : '-',
                  (finfo->st_mode & S_IXOTH)  ? 'x' : '-');

        // Now format the complete line.
        int len = snprintf(treeData->dFoundFileLine, treeData->nFoundFileLine, "%s%s%s",
                           treeData->fileTime, treeData->fileAttr, treeData->dFoundFile);
        if ( len >= (int)treeData->nFoundFileLine )
        {
            size_t need = treeData->nFoundFile + strlen(treeData->fileTime) + strlen(treeData->fileAttr) + 1;
            if ( ! increaseBuffer(c, need, treeData, FOUNDFILELINE_BUFFER) )
            {
                return false;
            }

            // The buffer is now guaranteed to be big enough.
            sprintf(treeData->dFoundFileLine, "%s%s%s",
                    treeData->fileTime, treeData->fileAttr, treeData->dFoundFile);
        }
    }

    // Place found file line in the stem.
    RexxStringObject t = c->String(treeData->foundFileLine);

    treeData->count++;
    c->SetStemArrayElement(treeData->files, treeData->count, t);
    c->ReleaseLocalReference(t);

    return true;
}


/**
 * Finds all files matching treeData->fNameSpec starting path, recursing into
 * sub-directories if requested.
 *
 * @param c         Call context we are operating in.
 *
 * @param path      Path to the directory to start in.
 *
 * @param treeData  Struct containing data pertaining to the search and used to
 *                  return the found files.
 *
 * @param options   Options defining the search.  Some comination of the
 *                  following.
 *
 *                  RECURSE       - Indicates that function should search
 *                                  all child subdirectories recursively.
 *                  DO_DIRS       - Indicates that directories should be
 *                                  included in the search.
 *                  DO_FILES      - Indicates that files should be included
 *                                  in the search.
 *                  NAME_ONLY     - Indicates that the output should be
 *                                  restricted to matched file names only.
 *                  EDITABLE_TIME - Indicates time and date fields should
 *                                  be output as one timestamp.
 *                  LONG_TIME     - Indicates time and date fields should
 *                                  be output as one long formatted timestamp
 *                  CASELESS      - Indicates do a case insensitive check for
 *                                  file names.
 *
 * @return True on no error, otherwise false.
 *
 * @remarks  The IBM recursive find file passed in fileSpec as the first
 *           argument, but never used it.  Instead it used fNameSpec in
 *           RXTREEDATA.  So, that argument is eliminated here and fNameSpec is
 *           used.
 *
 *           snprintf() note:
 *
 *           snprintf() doe not write more than size bytes (including the
 *           terminating null byte ('\0')). If the output was truncated due to
 *           this limit then the return value is the number of characters
 *           (excluding the terminating null byte) which would have been written
 *           to the final string if enough space had been available. Thus, a
 *           return value of size or more means that the output was truncated.
 *
 *           Therefore, unlike windows, there is no possibility of having an
 *           unterminated string.  If we detect that the buffers in use are too
 *           small, rather than truncate data, we allocate dynamic memory that
 *           is big enough.  Actually we allocate a buffer twice as big as the
 *           current buffer.
 *
 *           We start with big static buffers, so we need to keep track of
 *           whether memory was allocated or not.  This is done by tracking the
 *           size of the buffers.  If a buffer size does not equal the original
 *           size of the static buffer, then it needs to be freed.
 */
static bool recursiveFindFile(RexxCallContext *c, const char *path, RXTREEDATA *treeData, uint32_t options)
{
    DIR    *dir_handle;                  // Directory handle.
    struct  stat finfo;                  // File information.
    char   *fileName;                    // Found file name returned here.
    int     len;                         // snprintf return to check buffer overflow.
    bool    caseless = options & CASELESS;

    // First, process all of the normal files, saving directories for last.                                                             *

    dir_handle = opendir(path);
    if ( dir_handle == NULL )
    {
        // Old IBM code returned VALID_ROUTINE here.  I think it should probably
        // be an error though.  I.e., it should be false.
        return true;
    }

    // If processing files, and have some files, get all of them.
    if ( (options & DO_FILES) && linFindNextFile(c, treeData->dFNameSpec, path, dir_handle, &finfo, &fileName, caseless) )
    {
        do
        {
            // Build the full name.
            len = snprintf(treeData->dFoundFile, treeData->nFoundFile, "%s%s", path, fileName);
            if ( len >= (int)treeData->nFoundFile )
            {
                if ( ! increaseBuffer(c, len + 1, treeData, FOUNDFILE_BUFFER) )
                {
                    closedir(dir_handle);
                    return false;
                }

                // We are guarenteed the buffer is big enough.
                sprintf(treeData->dFoundFile, "%s%s", path, fileName);
            }

            if ( ! formatFile(c, treeData, options, &finfo) )
            {
              closedir(dir_handle);
              return false;
            }
        }
        while ( linFindNextFile(c, treeData->dFNameSpec, path, dir_handle, &finfo, &fileName, caseless) );
    }

    // Reset the directory handle, (rewinddir doesn't work.)
    closedir(dir_handle);
    dir_handle = opendir(path);

    if ( dir_handle == NULL )
    {
        return true;
    }

    // If processing directories, and have some directories, get all of them.
    if ( (options & DO_DIRS)  &&  linFindNextDir(c, treeData->dFNameSpec, path, dir_handle, &finfo, &fileName, caseless) )
    {
        do
        {
            // Skip dot directories.
            if ( strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0 )
            {
                continue;
            }

            // Build the full name.
            len = snprintf(treeData->dFoundFile, treeData->nFoundFile, "%s%s", path, fileName);
            if ( len >= (int)treeData->nFoundFile )
            {
                if ( ! increaseBuffer(c, len + 1, treeData, FOUNDFILE_BUFFER) )
                {
                    closedir(dir_handle);
                    return false;
                }

                // We are guarenteed the buffer is big enough.
                sprintf(treeData->dFoundFile, "%s%s", path, fileName);
            }

            if ( ! formatFile(c, treeData, options, &finfo) )
            {
              closedir(dir_handle);
              return false;
            }
       }
       while ( linFindNextDir(c, treeData->dFNameSpec, path, dir_handle, &finfo, &fileName, caseless) );

    }

    // Reset the directory handle, (rewinddir doesn't work.)
    closedir(dir_handle);
    dir_handle = opendir(path);
    if ( dir_handle == NULL )
    {
        return true;
    }

    // Do we need to recurse?
    if ( options & RECURSE )
    {
        // Used to create a new directory name.
        char    tmpDirName[FOUNDFILE_BUF_LEN];
        char   *dTmpDirName = tmpDirName;
        size_t  nTmpDirName = FOUNDFILE_BUF_LEN;

        // No need for caseless when matching a star.
        if ( linFindNextDir(c, "*", path, dir_handle, &finfo, &fileName, 0) )
        {
            do
            {
                // Skip dot directories.
                if ( strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0 )
                {
                    continue;
                }

                // Build the new directory name and search the next level.
                len = snprintf(dTmpDirName, nTmpDirName, "%s%s/", path, fileName);
                if ( len >= (int)nTmpDirName )
                {
                    if ( ! getBiggerBuffer(c, &dTmpDirName, &nTmpDirName, FOUNDFILE_BUF_LEN) )
                    {
                        closedir(dir_handle);
                        return false;
                    }

                    // We are guarenteed the buffer is big enough.
                    sprintf(dTmpDirName, "%s%s/", path, fileName);
                }

                if ( ! recursiveFindFile(c, dTmpDirName, treeData, options) )
                {
                    closedir(dir_handle);
                    if ( nTmpDirName != FOUNDFILE_BUF_LEN )
                    {
                        free(dTmpDirName);
                    }
                    return false;
                }
            }
            while ( linFindNextDir(c, "*", path, dir_handle, &finfo, &fileName, 0) );
        }

        if ( nTmpDirName != FOUNDFILE_BUF_LEN )
        {
            free(dTmpDirName);
        }
    }

    closedir(dir_handle);
    return true;
}

/**
 * Intializes the RXTREEDATA struct at start up of SysFileTree()
 *
 * @param treeData
 * @param files
 */
void inline initTreeData(RXTREEDATA *treeData, RexxStemObject files)
{
    treeData->files = files;

    treeData->dFNameSpec = treeData->fNameSpec;
    treeData->nFNameSpec = FNAMESPEC_BUF_LEN;

    treeData->dFoundFile = treeData->foundFile;
    treeData->nFoundFile = FOUNDFILE_BUF_LEN;

    treeData->dFoundFileLine = treeData->foundFileLine;
    treeData->nFoundFileLine = FOUNDFILELINE_BUF_LEN;
}

/**
 * Frees any allocated memory in the RXTREEDATA struct, if needed, on return
 * from SysFileTree()
 *
 * @param treeData
 */
void inline uninitTreeData(RXTREEDATA *treeData)
{
    if ( treeData->nFNameSpec > FNAMESPEC_BUF_LEN )
    {
        free(treeData->dFNameSpec);
    }
    if ( treeData->nFoundFile > FOUNDFILE_BUF_LEN )
    {
        free(treeData->dFoundFile);
    }
    if ( treeData->nFoundFileLine > FOUNDFILELINE_BUF_LEN )
    {
        free(treeData->dFoundFileLine);
    }
}



/**
 * SysFileTree() implementation.  Searches for files in a directory tree.
 *
 * @param  fSpec  [required]  The search pattern, may contain glob characters.
 *                 E.g., .sh
 *
 * @param  files  [required]  A stem to contain the returned results.
 *
 * @param  opts   [optional]  Any combination of the following:
 *
 *                  'B' - Search for files and directories.
 *                  'D' - Search for directories only.
 *                  'F' - Search for files only.
 *                  'O' - Only output file names.
 *                  'S' - Recursively scan subdirectories.
 *                  'T' - Combine time & date fields into one.
 *                  'L' - Long time format
 *                  'I' - Case Insensitive search.
 *
 * @param targetAttr  [optional]  Target attribute mask.  This is not used in
 *                    the Unix version of SysFileTree(), was never used.  But,
 *                    the doc never noted that fact.  It is left here to avoid
 *                    breaking any program that may have used it.
 *
 * @param newAttr     [optional]  New attribute mask.  This is not used in the
 *                    Unix version of SysFileTree(), was never used.  But, the
 *                    doc never noted that fact.  It is left here to avoid
 *                    breaking any program that may have used it.
 *
 * @return  0 on success, non-zero on error.  For all errors a condition is
 *          raised.
 *
 *
 */
RexxRoutine5(uint32_t, SysFileTree, CSTRING, fSpec, RexxStemObject, files, OPTIONAL_CSTRING, opts,
             OPTIONAL_CSTRING, targetAttr, OPTIONAL_CSTRING, newAttr)
{
    char         fileSpec[FNAMESPEC_BUF_LEN]; // File specification to look for.
    char         path[FOUNDFILE_BUF_LEN];     // Path to search along.
    uint32_t     result   = 1;       // Return value, 1 is an error.
    RXTREEDATA  treeData = {0};     // Struct containing data about found files.
    uint32_t     options  = 0;

    initTreeData(&treeData, files);

    char   *dPath = path;
    size_t  nPath = FOUNDFILE_BUF_LEN;

    if ( ! getFileSpecFromArg(context, fSpec, fileSpec, FNAMESPEC_BUF_LEN, 1) )
    {
        goto done_out;
    }

    if ( ! getOptionsFromArg(context, opts, &options, 3) )
    {
        goto done_out;
    }

    if ( ! getPath(context, fileSpec, &dPath, &nPath, &treeData) )
    {
        goto done_out;
    }

    // If caseless, upper case file name portion now
    if ( options & CASELESS )
    {
        char *p = treeData.fNameSpec;
        for ( ; *p; ++p )
        {
          *p = toupper(*p);
        }
    }

    // The old RecursiveFindFile pulls fileSpec from treeData and never uses
    // fileSpec.  fileSpec and treeData.fNameSpec are not equivalent.

    if ( recursiveFindFile(context, dPath, &treeData, options) )
    {

        context->SetStemArrayElement(treeData.files, 0, context->WholeNumber(treeData.count));
        result = 0;
    }

    if ( nPath != FOUNDFILE_BUF_LEN )
    {
        free(dPath);
    }

done_out:

    uninitTreeData(&treeData);
    return result;
}



/*************************************************************************
* Function:  SysTempFileName                                             *
*                                                                        *
* Syntax:    call SysTempFileName template [,filler]                     *
*                                                                        *
* Params:    template - Description of filespec desired.  For example:   *
*                        C:\TEMP\FILE.???                                *
*            filler   - A character which when found in template will be *
*                        replaced with random digits until a unique file *
*                        or directory is found.  The default character   *
*                        is '?'.                                         *
*                                                                        *
* Return:    other - Unique file/directory name.                         *
*            ''    - No more files exist given specified template.       *
*************************************************************************/

RexxRoutine2(RexxStringObject, SysTempFileName, CSTRING, template, CSTRING, fillerOpt)
{
  char   filler = '?';                 /* filler character           */
  char numstr[6];
  size_t  num, max;
  char   *array;
  char   *dir;                         /* the directory              */
  char   *file;                        /* the file prefix            */
  char *tmp;                           /* temporary                  */
  int x, j, i;
  j = 0;                                     /* get a copy of the argument */
  i = 0;
  max = 1;

  if ( strlen(template) > 512 || (fillerOpt != NULL && strlen(fillerOpt) != 1))
  {
      context->InvalidRoutine();
      return NULLOBJECT;
  }

  if ( fillerOpt != NULL )
  {
     filler = fillerOpt[0];
  }

                                       /* get the file id            */
  dir = (char*) malloc(strlen(template) + 1);
  if (dir == NULL)                    /* if something went wrong    */
  {
      return context->NewStringFromAsciiz(ERROR_NOMEM);
  }

  strcpy(dir, template);              /* copy the string             */

  tmp = dir;                          /* set temporary               */

/* search for filter        *********/

  for (x = 0; tmp[x] != 0; x++)
  {
    if (tmp[x] == filler)
    {
      max = max *10;
      j++;
    }
  }

  if (j)
  {
    srand(time(0));
    num = rand();
    num = num % max;

    switch (j)
    {
       case 1:
              sprintf(numstr, "%01u", (int)num);
              break;
       case 2:
              sprintf(numstr, "%02u", (int)num);
              break;
       case 3:
              sprintf(numstr, "%03u", (int)num);
              break;
       case 4:
              sprintf(numstr, "%04u", (int)num);
              break;
       case 5:
              sprintf(numstr, "%05u", (int)num);
              break;
      default:
              context->InvalidRoutine();
              return NULLOBJECT;
    }                                            /* for compatibility     */

    for (x = 0; tmp[x] !=0; x++)
    {
      if (tmp[x] == filler)
      {
        tmp[x] = numstr[i++];
      }
    }
  }                                         /* if we need the filler */

  while(*tmp != 0 )                         /* lets start at the end */
  {
    tmp++;
  }

  while((*tmp != '/') && (*tmp != '\\') && (tmp > dir))
  {
    --tmp;
  }

  if(tmp == dir)
  {                                 /* directory string is '' or '/' */
    if(*dir == '\\'){
      file = dir+1;
      array = tempnam(NULL,file); /* call system routine    */
    }
    else if(*dir == '/'){
      file = dir+1;
      array = tempnam("/",file); /* call system routine     */
    }
    else{
      file = dir;
      array = tempnam(NULL,file);/* call system routine     */
    }
  }
  else
  {                                /* directory string exists    */
    file = tmp+1;                      /* set filename prefix        */
    *tmp = '\0';                       /* terminate directory string */
    array = tempnam(dir,file);/* call system routine        */
  }

  if(strlen(array) > 255)
  {
     free((char *)retstr->strptr);
     retstr->strptr = (char *) malloc(strlen(array) + 1);
  }

  RexxStringObject retstr = context->NewStringFromAsciiz(array);

  free (array);
  free(dir);                           /* free local string          */

  return retstr;
}


/*************************************************************************
* Function:  SysSetPriority                                              *
*                                                                        *
* Syntax:    result = SysSetPriority(Class, Level)                       *
*                                                                        *
* Params:    Class  - The priority class (0-4)                           *
*            Level  - Amount to change (-31 to +31)                      *
*                     (lower to higher priority)                         *
* Return:    0    for correct execution                                  *
*            304  for ERROR_INVALID_PDELTA                               *
*            307  for ERROR_INVALID_PCLASS                               *
*            derived from:                                               *
*            result - return code from DosSetPriority                    *
*                                                                        *
*************************************************************************/

RexxRoutine2(int, SysSetPriority, int32_t, pclass, int32_t, level)
{
  RexxReturnCode    rc;                        /* creation return code                */


  if (pclass == 0)                        /* class 0 -> no change               */
  {
      rc = 0;                             /* no error                           */
  }
                                        /* change the priority                */
                                        /* change according to delta          */
  else if (pclass > 0) && pclass <= 4)
  {
    int pid;                            /* PID                                */
    pid = getpid();                     /* current PID                        */

    int priority;                       /* Priority                           */
                                        /* current priority                   */
    priority = getpriority(PRIO_PROCESS, getpid());

                                        /* Set new priority                   */
    setpriority(PRIO_PROCESS, getpid(),-level);
    rc = 0;
  }

  else
  {
    context->InvalidRoutine();
    return 0;
  }

  return rc;
}



/*************************************************************************
* Function:  SysGetMessage                                               *
*                                                                        *
* Syntax:    call SysGetMessage msgnum [,file] [,str1]...[,str9]         *
*                                                                        *
* Params:    file           - Name of message file to get message from.  *
*                              Default is OSO001.MSG.                    *
*            msgnum         - Number of message being queried.           *
*            str1 ... str9  - Insertion strings.  For messages which     *
*                              contain %1, %2, etc, the str1, str2, etc  *
*                              strings will be inserted (if given).      *
*                                                                        *
* Return:    The message with the inserted strings (if given).           *
* Note:      The set number ist always 1. Therefore the interface        *
*            remains the same as in OS/2 and Win.                        *
*            Reason: keep portability                                    *
*************************************************************************/

RexxRoutine3(RexxStringObject, SysGetMessage, uint32_t, msgnum, OPTIONAL_CSTRING, msgfile, ARGLIST, args) {

#if defined( HAVE_CATOPEN )
  int setnum = 1;                      /* Set number (const 1)       */
  nl_catd catalog;                     /* catalog handle             */
  /* default error msg          */
  const char *default_message = "Error: Message catalog not open";
  /* msg not found  msg         */
  const char *not_found_message = "Error: Message not found";
  /* insertion error  msg       */
  const char *error_insertions = "Error: Unable to generate message (wrong insertions)";
  /* cat not found  msg         */
  const char *cat_not_found_message = "Error: Message catalog not found";

  int icount;                          /* number of insertions       */
  char *temp;
  int count = 0;                       /* number of '%s' in the msg  */


  size_t numargs = context->ArrayItems(args);

  // we only support 9 substitution values
  if (numargs > 11) {
    context->InvalidRoutine();
    return NULLOBJECT;
  }
  /* Get message file name.     */
  /* Use "rexx.cat if not        */
  /* given                      */
  if (msgfile == NULL) {
    msgfile = REXXMESSAGEFILE;
  }

#if defined( HAVE_SETLOCALE )
  setlocale(LC_ALL, "en_US");
#endif
  /* open the catalog           */
  if ((catalog = catopen(msgfile, NL_CAT_LOCALE)) == (nl_catd)-1) {
    return context->NewStringFromAsciiz(cat_not_found_message);
  }

  /* retrieve msg from catalog  */
  char *msg = catgets(catalog, setnum, (int)msgnum, default_message);
  catclose(catalog);                   /* close the catalog          */


  if (*msg == '\0') {                     /* if empty string returned   */
    return context->NewStringFromAsciiz(not_found_message);
  }

  // copy this so we can safely modify it
  msg = strdup(msg);

  /* set number of insertions   */
  if (numargs >= 2) {
    icount = numargs - 2;
  }
  else {
    icount = 0;
  }

  const char *substitutions[10];       // array of string values
  size_t msg_length = 0;               // length of the return msg

  for (size_t i = 3; i <= numargs; i++) {
    // get each of the argument objects as a string value.
    // omitted arguments are not permitted.
    RexxObjectPtr o = context->ArrayAt(i);
    if (o == NULLOBJECT) {
      free(msg);
      context->InvalidRoutine();
      return NULLOBJECT;
    }
    substitutions[i - 3] = context->ObjectToStringValue(o);
    msg_length += strlen(substitutions[i - 3]);
  }

  msg_length += strlen(msg);
  // make sure this has enough space for replacement messages
  msg_length = max(msg_length, 100);

  char *retstr = (char *)malloc(msg_length);
  if (retstr == NULL) {
    free(msg);
    return context->NewStringFromAsciiz(ERROR_NOMEM);
  }

  /* check for too much '%s' in the message                          */
  char *temp = msg;
  int count = 0;
  /* replace all &1..&9 with %s                                         */
  while ((temp = strstr(temp, "&"))) {
    /* replace &1..&9 ?             */
    if (isdigit(*(temp + 1))) {
      *(temp++) = '%';
      *(temp++) = 's';                  /* %s expected                  */
      //      count++;
    }
    else {
      temp++;
    }
  }
  /* now look for number of replacement variables                       */
  temp = const_cast<char *>(msg);        /* reset temp pointer          */
  while ((temp = strstr(temp, "%s"))) {     /* search for the %s           */
    count++;                            /* increment counter           */
    temp += 2;                           /* jump over %s                */
  }

  /* generate full message with insertions                           */
  switch (icount) {
    case(0):
    {
      // no substitutions
      strncpy(retstr, msg_length, msg);
      break;
    }
    case(1):
    {
      snprintf(retstr, msg_length, msg, substitutions[0]);
      break;
    }
    case(2):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1]);
      break;
    }
    case(3):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2]);
      break;
    }
    case(4):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3]);
      break;
    }
    case(5):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4]);
      break;
    }
    case(6):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5]);
      break;
    }
    case(7):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5], substitutions[6]);
      break;
    }
    case(8):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5], substitutions[6], substitutions[7]);
      break;
    }
    case(9):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5], substitutions[6], substitutions[7], substitutions[8]);
      break;
    }
    default:
    {
      // change the message for insertion errors
      strncpy(retstr, msg_length, error_insertions);
      break;
    }
  }

  RexxStringObject result = context->newStringFromAsciiz(retstr);
  free(retstr);
  free(msg);
  return result;
#else

  return context->NewStringFromAsciiz("Error: Message catalog (catopen) not supported");
#endif
}



/*************************************************************************
* Function:  SysGetMessageX                                              *
*                                                                        *
* Syntax:    call SysGetMessageX setnum, msgnum [,file] [,str1]...[,str9]*
*                                                                        *
* Params:    file           - Name of message file to get message from.  *
*                              Default is OSO001.MSG.                    *
*            msgnum         - Number of message being queried.           *
*            str1 ... str9  - Insertion strings.  For messages which     *
*                              contain %1, %2, etc, the str1, str2, etc  *
*                              strings will be inserted (if given).      *
*            setnum         - set number in the catalog                  *
*                                                                        *
* Return:    The message with the inserted strings (if given).           *
* Note:      This is a special Unix only version of SysGetMessage which  *
*            supports the selection of a set in the msg catalog.         *
*************************************************************************/

RexxRoutine3(RexxStringObject, SysGetMessage, uint32_t, setnum, uint32_t, msgnum, OPTIONAL_CSTRING, msgfile, ARGLIST, args)
{
#if defined( HAVE_CATOPEN )
  size_t numargs = context->ArrayItems(args);

  // we only support 9 substitution values
  if(numargs > 12) {
    context->InvalidRoutine();
    return NULLOBJECT;
  }
  /* Get message file name.     */
  /* Use "rexx.cat if not        */
  /* given                      */
  if(msgfile == NULL) {
    msgfile = REXXMESSAGEFILE;
  }
#if defined( HAVE_SETLOCALE )
  setlocale(LC_ALL, "en_US");
#endif
  /* open the catalog           */
  if((catalog = catopen(msgfile, NL_CAT_LOCALE)) == (nl_catd)-1) {
    return context->NewStringFromAsciiz(cat_not_found_message);
  }

  /* retrieve msg from catalog  */
  char *msg = catgets(catalog, setnum, (int)msgnum, default_message);
  catclose(catalog);                   /* close the catalog          */


  if(*msg == '\0') {                     /* if empty string returned   */
    return context->NewStringFromAsciiz(not_found_message);
  }

  // copy this so we can safely modify it
  msg = strdup(msg);

  /* set number of insertions   */
  if(numargs >= 3) {
    icount = numargs - 3;
  }
  else {
    icount = 0;
  }

  const char *substitutions[10];       // array of string values
  size_t msg_length = 0;               // length of the return msg

  for (size_t i = 4; i <= numargs; i++) {
    // get each of the argument objects as a string value.
    // omitted arguments are not permitted.
    RexxObjectPtr o = context->ArrayAt(i);
    if(o == NULLOBJECT) {
      free(msg);
      context->InvalidRoutine();
      return NULLOBJECT;
    }
    substitutions[i - 4] = context->ObjectToStringValue(o);
    msg_length += strlen(substitutions[i - 4]);
  }

  msg_length += strlen(msg);
  // make sure this has enough space for replacement messages
  msg_length = max(msg_length, 100);

  char *retstr = (char *)malloc(msg_length);
  if(retstr == NULL) {
    free(msg);
    return context->NewStringFromAsciiz(ERROR_NOMEM);
  }

  /* check for too much '%s' in the message                          */
  char *temp = msg;
  int count = 0;
  /* replace all &1..&9 with %s                                         */
  while ((temp = strstr(temp, "&"))) {
    /* replace &1..&9 ?             */
    if(isdigit(*(temp + 1))) {
      *(temp++) = '%';
      *(temp++) = 's';                  /* %s expected                  */
      //      count++;
    }
    else {
      temp++;
    }
  }
  /* now look for number of replacement variables                       */
  temp = const_cast<char *>(msg);        /* reset temp pointer          */
  while ((temp = strstr(temp, "%s"))) {     /* search for the %s           */
    count++;                            /* increment counter           */
    temp += 2;                           /* jump over %s                */
  }

  /* generate full message with insertions                           */
  switch(icount) {
    case(0):
    {
      // no substitutions
      strncpy(retstr, msg_length, msg);
      break;
    }
    case(1):
    {
      snprintf(retstr, msg_length, msg, substitutions[0]);
      break;
    }
    case(2):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1]);
      break;
    }
    case(3):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2]);
      break;
    }
    case(4):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3]);
      break;
    }
    case(5):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4]);
      break;
    }
    case(6):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5]);
      break;
    }
    case(7):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5], substitutions[6]);
      break;
    }
    case(8):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5], substitutions[6], substitutions[7]);
      break;
    }
    case(9):
    {
      snprintf(retstr, msg_length, msg, substitutions[0], substitutions[1], substitutions[2], substitutions[3],
               substitutions[4], substitutions[5], substitutions[6], substitutions[7], substitutions[8]);
      break;
    }
    default:
    {
      // change the message for insertion errors
      strncpy(retstr, msg_length, error_insertions);
      break;
    }
  }

  RexxStringObject result = context->newStringFromAsciiz(retstr);
  free(retstr);
  free(msg);
  return result;
#else

  return context->NewStringFromAsciiz("Error: Message catalog (catopen) not supported");
#endif
}


/*************************************************************************
* Function:  SysGetKey                                                   *
*                                                                        *
* Syntax:    call SysGetKey [echo]                                       *
*                                                                        *
* Params:    echo - Either of the following:                             *
*                    'ECHO'   - Echo the inputted key (default).         *
*                    'NOECHO' - Do not echo the inputted key.            *
*                                                                        *
* Return:    The key striked.                                            *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysGetKey, OPTIONAL_CSTRING, echo)
{
  bool      echo = true;               /* Set to false if we         */

  if (echo != NULL) {                  /* validate arguments         */
    if (!strcasecmp(echo, "NOECHO")) echo = false;
    else if (strcasecmp(echo, "ECHO"))
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }
  }

  char buffer[4];

  getkey(buffer, echo);         /* call the complicated part  */
  return context->NewStringFromAsciiz(buffer);
}

/*************************************************************************
* Function:  SysFork                                                     *
*                                                                        *
* Description:   Function to migrate CREXX for AIX procedures.           *
*                                                                        *
* Syntax:    call SysFork()                                              *
*                                                                        *
* Return:    Process_ID   ( to parent child''s ID / to child the ID 0 )  *
*************************************************************************/

RexxRoutine0(int, SysFork)
{
    return fork();
}

/*************************************************************************
* Function:  SysWait                                                     *
*                                                                        *
* Description:   Function to migrate CREXX for AIX procedures.           *
*                                                                        *
* Syntax:    call SysWait()                                              *
*                                                                        *
* Return:    exit code of child                                          *
*************************************************************************/

RexxRoutine0(int, SysWait)
{
  int iStatus;
  wait(&iStatus);
  return iStatus;
}

/*************************************************************************
* Function:  SysCreatePipe                                               *
*                                                                        *
* Description:   Function to migrate CREXX for AIX procedures.           *
*                Function creates an unnamed pipe                        *
*                                                                        *
* Syntax:    call SysCreatePipe( Blocking | Nonblocking )                *
*                                                                        *
* Return:    'handle handle'     ( handle for read and handle for write )*
*************************************************************************/

RexxRoutine1(RexxStringObject, SysCreatePipe, OPTIONAL_CSTRING, blocking)
{
  int  iStatus;
  int  iaH[2];
  char cBlocking = 1;                       // default is blocking

  /* One arg, first char is 'b'?    */
  if (blocking != NULL)
  {
    if (blocking[0] == 'n' || blocking[0] == 'N')
    {`
      cBlocking = 0;                         // non blocking needed
    }
    /* One arg, first char is 'n'?    */
    else if (blocking[0] != 'b' && blocking[0] != 'B')
    {
      context->InvalidRoutine();
      return NULLOBJECT;
    }
  }


  if (pipe(iaH)) {                             /* Create the pipe            */
    perror("*** ERROR: Creating pipe");      /* pipe creation failed       */
    return context->NullString();
  }

  if (!cBlocking) {                            /* Non-blocking?              */
    /* Get file status flags ---------- */
    iStatus = fcntl(iaH[0], F_GETFL, NULL);
    iStatus |= O_NONBLOCK;                   /* Turn on NONBLOCK flag      */
    /* Does set work? ----------------- */
    if (fcntl(iaH[0], F_SETFL, iStatus) == -1) {
      perror("*** ERROR: Setting NONBLOCK flag");  /* No, tell user        */
      close(iaH[0]); close(iaH[1]);                /* Close pipes          */
      return context->NullString();
    }
  }

  char retstr[100];
  snprintf(retstr, sizeof(retstr), "%d %d", iaH[0], iaH[1]); /* Create return string */
  return context->NewStringFromAsciiz(retstr);
}


/*************************************************************************
* Function:  SysGetFileDateTime                                          *
*                                                                        *
* Syntax:    result = SysGetFileDateTime(filename [,timesel])            *
* Params:    filename - name of the file to query                        *
*            timesel  - What filetime to query: Access/Write             *
*                                                                        *
* Return:    -1 - file date/time query failed                            *
*            other - date and time as YYYY-MM-DD HH:MM:SS                *
*************************************************************************/

RexxRoutine2(RexxObjectPtr, SysGetFileDateTime, CSTRING, file, OPTIONAL_CSTRING, timesel)
  {
    struct    stat64 buf;
    struct    tm *newtime;


    QualifiedName qualifiedName(file);

  if(stat64(qualifiedName, &buf) < 0) {
    return context->Int32ToObject(-1);
  }

  if(timesel != NULL) {
    switch(timesel[0]) {
      case 'a':
      case 'A':
        newtime = localtime(&(buf.st_atime));
        break;
      case 'w':
      case 'W':
        newtime = localtime(&(buf.st_mtime));

        break;
      default:
        context->InvalidRoutine();
        return NULLOBJECT;
    }
  }
  else {
    newtime = localtime(&(buf.st_mtime));
  }

  newtime->tm_year += 1900;
  newtime->tm_mon += 1;

  char retstr[100];

  snprintf(retstr, sizeof(retstr), "%4d-%02d-%02d %02d:%02d:%02d",
           newtime->tm_year,
           newtime->tm_mon,
           newtime->tm_mday,
           newtime->tm_hour,
           newtime->tm_min,
           newtime->tm_sec);
  return context->NewStringFromAsciiz(retstr);
}


/*************************************************************************
* Function:  SysSetFileDateTime                                          *
*                                                                        *
* Syntax:    result = SysSetFileDateTime(filename [,newdate] [,newtime]) *
*                                                                        *
* Params:    filename - name of the file to update                       *
*            newdate  - new date to set in format YYYY-MM-DD (YYYY>1800) *
*            newtime  - new time to set in format HH:MM:SS               *
*                                                                        *
* Return:    0 - file date/time was updated correctly                    *
*            -1 - failure attribute update                               *
*************************************************************************/

  RexxRoutine3(int, SysSetFileDateTime, CSTRING, filename, OPTIONAL_CSTRING, newdateSet, OPTIONAL_CSTRING, newtimeSet) {
    struct utimbuf timebuf;
    struct tm *newtime;
    time_t ltime;
    struct stat64 buf;

    QualifiedName qualifiedName(filename);

    if (stat64(qualifiedName, &buf) < 0) {
      return -1;
    }


    if (newdateSet == NULL && newtimeSet == NULL) {
      time(&ltime);
      timebuf.modtime = ltime;
      return utime(qualifiedName, &timebuf) < 0 ? -1 : 0;
    }
    else {
      newtime = localtime(&(buf.st_mtime));
      if (newdateSet != NULL) {

        /* parse new date */
        if (sscanf(newdateSet, "%4d-%2d-%2d", &newtime->tm_year, &newtime->tm_mon, &newtime->tm_mday) != 3) {
          return -1;
        }
        newtime->tm_year -= 1900;
        newtime->tm_mon -= 1;
      }
      if (newtimeSet) {
        /* parse new time */
        if (sscanf(newtimeSet, "%2d:%2d:%2d", &newtime->tm_hour, &newtime->tm_min, &newtime->tm_sec) != 3) {
          return -1;
        }
      }
      ltime = mktime(newtime);
      timebuf.modtime = ltime;

      return utime(qualifiedName, &timebuf) < 0 ? -1 : 0;
  }
}


/**************************************************************************
* Function:  SysQueryProcess                                              *
*                                                                         *
* Params:    "PID"       - (default) returns current process ID           *
*    NEW:    "PPID"      -  returns parent of current process ID          *
*    NEW:    "PGID"      -  returns group ID of current process           *
*    NO      "TID"       -  returns current thread ID                     *
*    YES     "PPRIO"     -  returns current process priority              *
*    NO      "TPRIO"     -  returns current thread priority               *
*    YES     "PTIME"     -  returns current process times                 *
*    NO      "TTIME"     -  returns current thread times                  *
*    NEW:    "PMEM"      -  returns current process max memory size RSS   *
*    NEW:    "PSWAPS"    -  returns current process number of swaps out   *
*    NEW:    "PRCVDSIG"  -  returns current process received signals      *
***************************************************************************/

RexxRoutine1(RexxObjectPtr, SysQueryProcess, OPTIONAL_CSTRING, selector)
{
  unsigned int uiUsedCPUTime  = 0;
  unsigned int uiUsedCPUmsec  = 0;
  unsigned int uiUsedHours    = 0;
  unsigned int uiUsedMinutes  = 0;
  unsigned int uiUsedSeconds  = 0;
  char timebuf[40];
  int iRc = 0;
  struct rusage struResUse;

  if ((selector == NULL) || (!strcasecmp(selector), "PID")) {
    return context->Int32ToObject(getpid());
  }
  else if (!strcasecmp(selector, "PPID")) {
    return context->Int32ToObject(getppid());
  }
  else if (!strcasecmp(selector, "PGID")) {
    return context->Int32ToObject(getpgid(getppid()));
  }
  else if (!strcasecmp(selector, "PPRIO")) {
    return context->Int32ToObject(getpriority(PRIO_PROCESS, 0));
  }

  /* ----------------------------------------------------------------- */
  /* Get process usage data and keep calls together at the end of      */
  /* the function SysQueryProcess.                                     */
  iRc = getrusage(RUSAGE_SELF, &struResUse);
  if (iRc) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "System error; errno = %d", errno);
    return context->NewStringFromAsciiz(buffer);
  }
  if (!strcasecmp(selector, "PTIME")) { /* Calculate the used CPU time*/
    uiUsedCPUmsec  = (unsigned int)struResUse.ru_utime.tv_usec / 1000;
    uiUsedCPUmsec += (unsigned int)struResUse.ru_stime.tv_usec / 1000;
    if (uiUsedCPUmsec >= 1000) {
      uiUsedCPUTime = uiUsedCPUmsec / 1000;
      uiUsedCPUmsec = uiUsedCPUmsec % 1000;
    }
    uiUsedCPUTime += (unsigned int)struResUse.ru_utime.tv_sec;
    uiUsedCPUTime += (unsigned int)struResUse.ru_stime.tv_sec;
    uiUsedHours   = uiUsedCPUTime / 3600;
    uiUsedMinutes = uiUsedCPUTime / 60;
    if (uiUsedMinutes >= 60) uiUsedMinutes = uiUsedMinutes % 60;
    if (uiUsedCPUTime >= 60) uiUsedSeconds = uiUsedCPUTime % 60;
    else
      uiUsedSeconds = uiUsedCPUTime;

    char buffer[200];
    snprintf(buffer, sizeof(buffer), "CPU_Time Summary: %2d:%.2d:%.2d:%.3d  Kernel:",
             uiUsedHours, uiUsedMinutes, uiUsedSeconds, uiUsedCPUmsec);

    uiUsedCPUmsec = (unsigned int)struResUse.ru_stime.tv_usec / 1000;
    uiUsedCPUTime = (unsigned int)struResUse.ru_stime.tv_sec;
    uiUsedHours   = uiUsedCPUTime / 3600;
    uiUsedMinutes = uiUsedCPUTime / 60;
    if (uiUsedMinutes >= 60) uiUsedMinutes = uiUsedMinutes % 60;
    if (uiUsedCPUTime >= 60) uiUsedSeconds = uiUsedCPUTime % 60;
    else
      uiUsedSeconds = uiUsedCPUTime;

    size_t currLen = strlen(buffer);

    snprintf(buffer + currLen, sizeof(buffer) - currLen, " %2d:%.2d:%.2d:%.3d  User:", uiUsedHours,
             uiUsedMinutes, uiUsedSeconds, uiUsedCPUmsec);
    strncat(buffer, sizeof(buffer), timebuf);

    currLen = strlen(buffer);

    uiUsedCPUmsec = (unsigned int)struResUse.ru_utime.tv_usec / 1000;
    uiUsedCPUTime = (unsigned int)struResUse.ru_utime.tv_sec;
    uiUsedHours   = uiUsedCPUTime / 3600;
    uiUsedMinutes = uiUsedCPUTime / 60;
    if (uiUsedMinutes >= 60) uiUsedMinutes = uiUsedMinutes % 60;
    if (uiUsedCPUTime >= 60) uiUsedSeconds = uiUsedCPUTime % 60;
    else
      uiUsedSeconds = uiUsedCPUTime;

    snprintf(buffer + currLen, sizeof(buffer) - currLen, " %2d:%.2d:%.2d:%.3d", uiUsedHours,
             uiUsedMinutes, uiUsedSeconds, uiUsedCPUmsec);

    return context->NewStringFromAsciiz(buffer);
  }
  else if (!strcasecmp(selector, "PMEM")) {  /* Show max memory RSS used   */
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Max_Memory_RSS: %ld", struResUse.ru_maxrss);
    return context->NewStringFromAsciiz(buffer);
  }
  else if (!strcasecmp(selector, "PSWAPS")) { /* Memory has been swapped   */
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Memory_swaps: %ld", struResUse.ru_nswap);
    return context->NewStringFromAsciiz(buffer);
  }
  else if (!strcasecmp(selector, "PRCVDSIG")) { /* Process received signals*/
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Received_signals: %ld", struResUse.ru_nsignals);
    return context->NewStringFromAsciiz(buffer);
  }

  context->InvalidRoutine()
  return NULLOBJECT;
}

/*************************************************************************
* Function:  SysGetErrortext                                             *
*                                                                        *
* Syntax:    call SysGetErrortext errnumber                              *
*                                                                        *
* Params:    errnumber - error number to be described                    *
*                                                                        *
* Return:    Description or empty string                                 *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysGetErrorText, int32_t, errnum)
{
  const char *errmsg = strerror(errnum);
  if (errmsg == NULL)
  {
    return context->NullString();
  }
  else
  {
    return context->NewStringFromAsciiz(errmsg);
  }
}


/*************************************************************************
* Function:  SysFileCopy                                                 *
*                                                                        *
* Syntax:    call SysFileCopy FROMfile TOfile                            *
*                                                                        *
* Params:    FROMfile - file to be copied.                               *
*            TOfile - target file of copy operation.                     *
*                                                                        *
* Return:    Return 0 if no error, or error code.                        *
*************************************************************************/

class AutoClose{
  public:
  AutoClose() : value(-1) {
  };
  AutoClose(int fd) : value(fd) {
  }
  ~AutoClose() {
    close(false);
  }
  AutoClose & operator=(int fd) {
    close(false); value = fd; return *this;
  }
  operator int() const {
    return value;
  }
  int operator==(int fd) {
    return value == fd;
  }
  int close(bool returnError = true);
  private:
  int value; // >= 0 if opened
};

// Returns 0 if no error, -1 otherwise (in this case, errno contains the error code)
int AutoClose::close(bool returnError) {
  int closeStatus = 0;
  if (returnError) {
    if (value >= 0) closeStatus = ::close(value);
  }
  else {
    if (value >= 0) {
      int backup = errno;
      ::close(value);
      errno = backup;
    }
  }
  value = -1;
  return closeStatus;
}

class AutoFree {
 public:
   AutoFree() : value(NULL) { };
   AutoFree(char *p) : value(p) { }
   ~AutoFree() { if (value != NULL) free(value); value = NULL; }
   AutoFree & operator=(char *p) { if (value != NULL) free(value); value = p; return *this; }
   operator char *() const { return value; }
   int operator==(char *p) { return value == p; }
 private:
   char *value;
};

/*
 Remember : realpath will fail if the file does not exist.
 See normalizePathName in SysFileSystem.cpp.
 */
bool SamePaths(CSTRING path1, CSTRING path2) {
  char actualpath1[PATH_MAX + 1];
  char actualpath2[PATH_MAX + 1];
  if (realpath(path1, actualpath1) == NULL) return false;
  if (realpath(path2, actualpath2) == NULL) return false;
  return strcmp(actualpath1, actualpath2) == 0;
}

/*
Returns a new filename whose directory path is the same as filename's directory
and that is not the same as the name of an existing file in this directory.
Used to temporarily rename a file in place (i.e. in its directory).
This new filename must be freed when no longer needed.
*/
char* TemporaryFilename(CSTRING filename, int &errInfo) {
  errInfo = 0;
  AutoFree filenameCopy = strdup(filename); // dirname wants a writable string
  if (filenameCopy != NULL) {
    AutoFree directory = strdup(dirname(filenameCopy)); // Dup because not clear from standard if filenameCopy can be freed just after the call
    if (directory != NULL) {
      char *newFilename = tempnam(directory, NULL);
      if (newFilename != NULL) return newFilename;
    }
  }
  errInfo = errno;
  return NULL;
}

/*
The symbolic links are dereferenced :
- If the source file is a symbolic link, the actual file copied is the target of the symbolic link.
- If the destination file already exists and is a symbolic link, the target of the symbolic link is overwritten by the source file.
If the destination file does not exist then it is created with the access mode of the source file (if possible).
*/
int CopyFile_DereferenceSymbolicLinks(CSTRING fromFile, CSTRING toFile, bool preserveTimestamps, bool preserveMode, bool *timestampsPreserved = NULL, bool *modePreserved = NULL) {
  // initialize output arguments
  if (timestampsPreserved != NULL) *timestampsPreserved = false;
  if (modePreserved != NULL) *modePreserved = false;

  if (SamePaths(fromFile, toFile)) return EEXIST; // did not find a better error code to return

  struct stat64 fromStat;
  if (stat64(fromFile, &fromStat) == -1) return errno;
  AutoClose fromHandle = open64(fromFile, O_RDONLY);
  if (fromHandle == -1) return errno;

  struct stat64 toStat;
  bool toFileCreated = (stat64(toFile, &toStat) == -1);
  AutoClose toHandle = open64(toFile, O_WRONLY | O_CREAT | O_TRUNC, 0666); // default access mode for the moment (like fopen)
  if (toHandle == -1) return errno;

  char buffer[IBUF_LEN];
  while (1) {
    int count = read(fromHandle, &buffer, IBUF_LEN);
    if (count == -1) return errno;
    if (count == 0) break; // EOF
    if (write(toHandle, buffer, count) == -1) return errno;
  }

  if (fromHandle.close() == -1) return errno;
  if (toHandle.close() == -1) return errno;

  if (preserveTimestamps) {
    struct utimbuf timebuf;
    timebuf.actime = fromStat.st_atime;
    timebuf.modtime = fromStat.st_mtime;
    if (utime(toFile, &timebuf) == 0) {
      if (timestampsPreserved != NULL) *timestampsPreserved = true;
    }
  }

  if (toFileCreated || preserveMode) {
    if (chmod(toFile, fromStat.st_mode) == 0) {
      if (modePreserved != NULL) *modePreserved = true;
    }
  }
  return 0;
}

/*
The symbolic links are not dereferenced :
- If the source file is a symbolic link, the actual file copied is the symbolic link itself.
- If the destination file already exists and is a symbolic link, the target of the symbolic link is not overwritten by the source file.
*/
int CopyFile_DontDereferenceSymbolicLinks(CSTRING fromFile, CSTRING toFile, bool force, bool preserveTimestamps, bool preserveMode, bool *timestampsPreserved = NULL, bool *modePreserved = NULL) {
  // initialize output arguments
  if (timestampsPreserved != NULL) *timestampsPreserved = false;
  if (modePreserved != NULL) *modePreserved = false;

  if (SamePaths(fromFile, toFile)) return EEXIST; // did not find a better error code to return

  struct stat64 fromStat;
  if (lstat64(fromFile, &fromStat) == -1) return errno;
  bool fromFileIsSymbolicLink = S_ISLNK(fromStat.st_mode);

  struct stat64 toStat;
  bool toFileExists = (lstat64(toFile, &toStat) == 0);
  bool toFileIsSymbolicLink = (toFileExists && S_ISLNK(toStat.st_mode));

  AutoFree toFileNewname;
  if (toFileExists && (fromFileIsSymbolicLink || toFileIsSymbolicLink)) {
    // Must remove toFile when fromFile is a symbolic link, to let copy the link.
    // Must remove toFile when toFile is a symbolic link, to not follow the link.
    // But do that safely : first rename toFile, then do the copy and finally remove the renamed toFile.
    if (force == false) return EEXIST; // Not allowed by caller to remove it
    int errInfo;
    toFileNewname = TemporaryFilename(toFile, errInfo);
    if (errInfo != 0) return errInfo;
  }

  if (fromFileIsSymbolicLink) {
    off_t pathSize = fromStat.st_size; // The length in bytes of the pathname contained in the symbolic link
    AutoFree pathBuffer = (char *)malloc(pathSize + 1);
    if (pathBuffer == NULL) return errno;
    if (readlink(fromFile, pathBuffer, pathSize) == -1) return errno;
    pathBuffer[pathSize] = '\0';
    if (toFileNewname != NULL && rename(toFile, toFileNewname) == -1) return errno;
    if (symlink(pathBuffer, toFile) == -1) {
      int errInfo = errno;
      // Undo the renaming
      if (toFileNewname != NULL) rename(toFileNewname, toFile);
      return errInfo;
    }
    // Note : there is no API to preserve the timestamps and mode of a symbolic link
  }
  else {
    if (toFileNewname != NULL && rename(toFile, toFileNewname) == -1) return errno;
    int errInfo = CopyFile_DereferenceSymbolicLinks(fromFile, toFile, preserveTimestamps, preserveMode, timestampsPreserved, modePreserved);
    if (errInfo != 0) {
      // Undo the renaming
      if (toFileNewname != NULL) rename(toFileNewname, toFile);
      return errInfo;
    }
  }
  // The copy has been done, now can remove the backup
  if (toFileNewname != NULL) unlink(toFileNewname);
  return 0;

}

RexxRoutine2(int, SysFileCopy, CSTRING, fromFile, CSTRING, toFile) {
  return CopyFile_DereferenceSymbolicLinks(fromFile, toFile, true, false);
  // Note : no error returned if timestamps not preserved
}

/*************************************************************************
* Function:  SysFileMove                                                 *
*                                                                        *
* Syntax:    call SysFileMove FROMfile TOfile                            *
*                                                                        *
* Params:    FROMfile - file to be moved.                                *
*            TOfile - target file of move operation.                     *
*                                                                        *
* Return:    Return code from MoveFile() function.                       *
*************************************************************************/

int MoveFile(CSTRING fromFile, CSTRING toFile) {
  // rename does not return an error if both files resolve to the same file
  // but we want to return an error in this case, to follow the behavior of mv
  if (SamePaths(fromFile, toFile)) return EEXIST; // did not find a better error code to return
  if (rename(fromFile, toFile) == 0)  return 0; // move done
  if (errno != EXDEV) return errno; // move ko, no fallbak

  // Before trying the fallback copy+unlink, ensure that we can unlink fromFile.
  // If we can rename it in place, then we can unlink it.
  int errInfo;
  AutoFree fromFileNewname = TemporaryFilename(fromFile, errInfo);
  if (errInfo != 0) return errInfo;
  if (rename(fromFile, fromFileNewname) == -1) return errno;
  // Good, here we know we can unlink fromFile, undo the rename
  if (rename(fromFileNewname, fromFile) == -1) return errno; // should not happen, but...

  // The files are on different file systems and the implementation does not support that.
  // Try to copy then unlink
  int copyStatus = CopyFile_DontDereferenceSymbolicLinks(fromFile, toFile, false, true, true); // 3rd arg=false : dont't force (good choice ?)
  if (copyStatus != 0) return copyStatus; // fallback ko
  // Note : no error returned if timestamps or mode not preserved

  // copy to is ok, now unlink from
  // in case of error, don't remove toFile, it's a bad idea !
  return unlink(fromFile);
}

RexxRoutine2(int, SysFileMove, CSTRING, fromFile, CSTRING, toFile)
{
  return MoveFile(fromFile, toFile);
}
