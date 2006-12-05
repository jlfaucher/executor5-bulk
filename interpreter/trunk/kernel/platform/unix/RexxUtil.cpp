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
/* REXX Unix/Linux Support                                                    */
/*                                                                            */
/* Unix system utility functions                                              */
/*                                                                            */
/******************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* Include files */

#ifdef AIX
#include "locale.h"
#endif // AIX

// Needed by all platforms
#include "oorexx.h"
#include "rexx.h"
#include "SysFileSystem.hpp"

#ifdef LINUX
#include <sys/wait.h>
#include "SysSemaphore.hpp"
#endif // LINUX

// Needed by all platforms
#include <sys/ipc.h>
#include <memory.h>
#include <malloc.h>
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
#include <sys/ipc.h>
#include <semaphore.h>
#include <dirent.h>                    /* directory functions        */
#include <sys/time.h>                  /* needed for the select func */

#ifdef AIX
#include <time.h>                 /* needed for the select func          */
#include <sys/select.h>           /* needed for the select func          */
#include <sys/ldr.h>              /* needed for the load   func          */
#include <strings.h>
#endif // AIX

#include <utime.h>                /* moved, used by AIX & Linux          */
#include <sys/utsname.h>               /* get the uname() function   */
#include <signal.h>
#include <sys/resource.h>              /* get the getpriority() func */

#if defined(LINUX) && !defined(OPSYS_SUN)
#include <features.h>                  /* catopen()                  */
#endif  // defined(LINUX) && !defined(OPSYS_SUN)

#include <nl_types.h>                  /* catopen()                  */
#include <termios.h>                   /* needed for SysGetKey       */
#include "SysThread.hpp"               /* for the SysGetKey semaphore*/
#include <fnmatch.h>                   /* fnmatch()                  */

extern int opencnt[][2];               /* open count array for sems  */
extern char *resolve_tilde(char *);

extern void SysInitialize(void);

#define INVALID_ROUTINE 40
#define  MAX_DIGITS     9
#define  NO_UTIL_ERROR    "0"          /* No error whatsoever        */
#define  VALID_ROUTINE    0            /* Successful completion      */
//#define  MAX_LINE_LEN   2048         /* max line length */
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

#define UTIL_VERS "4.00"


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

#if defined(AIX) || defined(LINUX)
/*********************************************************************/
/* Defines used by SysStemSort                                       */
/*********************************************************************/
#define SORT_CASESENSITIVE 0
#define SORT_CASEIGNORE    1


#define SORT_ASCENDING 0
#define SORT_DECENDING 1

#define SORT_NUMERIC       3
#endif // defined(AIX) || defined(LINUX)

#define SORT_DEF_AVG_SIZE  20


/*********************************************************************/
/* Defines uses by SysTree                                           */
/*********************************************************************/

#define  RECURSE        0x0002
#define  DO_DIRS        0x0004
#define  DO_FILES       0x0008
#define  NAME_ONLY      0x0010
#define  EDITABLE_TIME  0x0020
#define  LONG_TIME      0x0040
#define  IGNORE         2              /* Ignore attributes entirely */


/******************************************************************************/
/* Defines used by SysGetKey                                                  */
/******************************************************************************/

#define stty(a,b)         (void)tcsetattr(a,TCSANOW,b) /* simple set attr.    */
#define gtty(a,b)         (void)tcgetattr(a,b)         /* simple get attr.    */
#define discard_input(a)  tcflush(a,TCIFLUSH)          /* simple flush        */
#define restore_tty(a)    stty(ttyfd,a)                /* new settings STDIN  */


/* semaphore to prevent mulitple SysGetKey calls                              */
static SysMutex SysGetKeySemaphore;
/* original terminal settings                                                 */
struct termios in_orig;                /* original settings (important!!)     */


/*********************************************************************/
/* RxTree Structure used by SysTree.                                 */
/*********************************************************************/

typedef struct RxTreeData {
    unsigned long count;               /* Number of lines processed  */
    SHVBLOCK      shvb;                /* Request block for RxVar    */
    unsigned long stemlen;             /* Length of stem             */
    unsigned long vlen;                /* Length of variable value   */
    char          TargetSpec[MAX+1];   /* Target filespec            */
    char          truefile[MAX+1];     /* expanded file name         */
    char          Temp[MAX+80];        /* buffer for returned values */
    char          varname[MAX];        /* Buffer for variable name   */
    unsigned long nattrib;             /* New attrib, diff for each  */
} RXTREEDATA;


/*********************************************************************/
/* RxTree Structure used by GetLine, OpenFile and CloseFile          */
/*********************************************************************/

typedef struct _GetFileData {
    char *        buffer;              /* file read buffer           */
    unsigned long size;                /* file size                  */
    off_t         data;                /* data left in buffer        */
    unsigned long residual;            /* size left to read          */
    char *        scan;                /* current scan position      */
    FILE          *handle;             /* file handle                */
} GetFileData;


/*********************************************************************/
/* Rexx default message file.                                        */
/*********************************************************************/

#define REXXMESSAGEFILE    "rexx.cat"


/********************************************************************
* Function:  ReadNextBuffer(filedata)                               *
*                                                                   *
* Purpose:   Reads the next buffer of data.                         *
*                                                                   *
* RC:        0       buffer was read                                *
*            1     - error occurred reading buffer                  *
*********************************************************************/

static int ReadNextBuffer(
    GetFileData  *filedata )            /* global file information   */
{
    off_t         size;                 /* size to read              */
    char *        endptr;               /* end of file pointer       */

    /* get size of this read */
    if(filedata->residual >= MAX_READ)
        size = MAX_READ;                /* read as much as possible  */
    else
        size = filedata->residual;      /* take the rest             */
    /* read the file in */
    filedata->data = fread(filedata->buffer, 1, size, filedata->handle);
    if(!filedata->data)                /* problems ?                 */
        return (1);                    /* get out                    */
    if (filedata->data != size)        /* not get all of it?         */
        filedata->residual = 0;        /* no residual                */
    else                               /* residual is remainder      */
        filedata->residual = filedata->residual - size;
    /* look for a EOF mark */
    endptr = (char *)memchr(filedata->buffer, CH_EOF, filedata->data);

    if (endptr) {                      /* found an EOF mark?         */
        /* set new length */
        filedata->data = (unsigned long)(endptr - filedata->buffer);
        filedata->residual = 0;        /* no residual                */
    }
    filedata->scan = filedata->buffer; /* set position to beginning  */
    return 0;
}


/***********************************************************************/
/* Function: strupr(string)                                            */
/* Purpose:  Uppercas the given string                                 */
/***********************************************************************/

static void strupr(char *string){

    for(;*string != '\0';string++)       /* while not at the end       */
        *string = toupper(*string);
}


/********************************************************************
* Function:  OpenFile(file, filedata)                               *
*                                                                   *
* Purpose:   Prepares a file for reading.                           *
*                                                                   *
* RC:        0     - file was opened successfully                   *
*            1     - file open error occurred                       *
*********************************************************************/

static int OpenFile(
     char *       file,                /* file name                  */
     GetFileData *filedata )           /* global file information    */
{
     struct stat finfo;                /* file information           */
     char *      endptr = NULL;        /* end of buffer pointer      */

    /* try to open the file */
    if((filedata->handle = fopen(file,"r")) == NULL)
        return (1);                    /* return failure             */
    /* retrieve the file size */
    if((stat(file,&finfo) == -1)||(!finfo.st_size)){
        fclose(filedata->handle);      /* close the file             */
        return (1);                    /* and quit                   */
    }
    if (finfo.st_size <= MAX_READ) {   /* less than a single buffer  */
        /* allocate buffer for file */
        if((filedata->buffer = (char *)malloc(finfo.st_size)) == NULL ) {
            fclose(filedata->handle);  /* close the file             */
            return (1);                /* and quit                   */
        }
        filedata->size = finfo.st_size;/* save file size             */
        filedata->residual = 0;        /* no left over information   */
        /* read the file in */
        filedata->data = fread(filedata->buffer, 1,
                               finfo.st_size, filedata->handle);
        if(filedata->data != finfo.st_size) {/*  problems?           */
            free(filedata->buffer);    /* free the buffer            */
            fclose(filedata->handle);  /* close the file             */
            return (1);                /* and quit                   */
        }                              /* look for a EOF mark        */
        endptr = (char *)memchr(filedata->buffer, CH_EOF, filedata->data);
        if (endptr)                    /* found an EOF mark          */
            /* set new length */
            filedata->data = (unsigned long)(endptr - filedata->buffer);
        filedata->scan = filedata->buffer;/* set position to beginning*/
    }
    else {                              /* need to read partial       */
        /* allocate buffer for read */
        if((filedata->buffer = (char *)malloc(MAX_READ)) == NULL ) {
            fclose(filedata->handle);  /* close the file             */
            return (1);                /* and quit                   */
        }
        filedata->size = finfo.st_size;/* save file size             */
                                       /* and set remainder          */
        filedata->residual = filedata->size;
                                       /* read the file in           */
        if (ReadNextBuffer(filedata)) {
            free(filedata->buffer);    /* free the buffer            */
            fclose(filedata->handle);  /* close the file             */
            return 1;
        }
    }
    return 0;                          /* file is opened             */
}


/********************************************************************
* Function:  CloseFile(filedata)                                    *
*                                                                   *
* Purpose:   Close a file                                           *
*********************************************************************/

static void CloseFile(
    GetFileData *filedata )            /* global file information    */
{
    fclose(filedata->handle);          /* close the file             */
    free(filedata->buffer);            /* release the file buffer    */
}


/*********************************************************************
* Function:  GetLine(line, size, filedata)                          *
*                                                                   *
* Purpose:   Reads a line of data using buffered reads.  At end of  *
*            file, zero is returned to indicate nothing left.       *
*                                                                   *
* RC:        true -  line was read successfully                     *
*            false - end of file was reached                        *
*********************************************************************/

static int GetLine(
    char *        line,                /* returned line              */
    off_t         size,                /* size of line buffer        */
    GetFileData   *filedata )          /* file handle                */
{
    char *        scan;                /* current scan pointer       */
    off_t         length;              /* line length                */
    unsigned long copylength;          /* copied length              */

    if (!(filedata->data)) {           /* if out of current buffer   */
        if (filedata->residual) {      /* may be another buffer      */
            ReadNextBuffer(filedata);  /* try to read one            */
            if (!filedata->data)       /* nothing more?              */
                return 1;              /* all done                   */
        }
        else
            return (1);                /* return EOF condition       */
    }
    /* look for a line feed */
    scan = (char *)memchr(filedata->scan, CH_NL, filedata->data);
    if (scan) {                        /* found one                  */
        /* calculate the length */
        length = (unsigned long)(scan - filedata->scan);
        if (length < size) {
            copylength = length;       /* get length to copy         */
        }
        else {
            copylength = size;         /* get length to copy         */
        }
        /* copy over the data */
        memcpy(line, filedata->scan, copylength);
        line[copylength] = '\0';       /* make into ASCIIZ string    */

        /* we don't want the CR character in the result string */
        if ( line[copylength - 1] == CH_CR ) {
            line[copylength - 1] = '\0';
        }

        filedata->data -= length + 1;  /* reduce the length          */
        filedata->scan = scan + 1;     /* set new scan point         */

        if (!filedata->data) {         /* all used up                */
            if (filedata->residual)    /* more to read               */
            ReadNextBuffer(filedata);  /* read the next buffer       */
        }
    return 0;                          /* this worked ok             */
    }
    else {                             /* ran off the end            */
/* now we have scanned the whole buffer, but didn't find LF.         */
/* we have two situation that can appear:                            */
/* 1.) size > filedata->data ==> there is still room in the working  */
/*     buffer, we can see whether we have scanned the whole file     */
/*     --> ReadNextBuffer, or this was it, and we return             */
/* 2.) size < filedata->buffer ==> we have scanned to the end of the */
/*     buffer, more than what would fit into it, but still we        */
/*     haven't had a hit. So copy all elements into the buffer       */
/*     read the next buffer, GetLine to get the next LF              */
/*     and return what was put into buffer. Be ALWAYS AWARE that     */
/*     that buffer limits to 2047 bytes, and that we only return up  */
/*     to 2047 bytes of a line. The rest of the line is not returned */
/*     and not checked for search argument. Nevertheless, this       */
/*     garantees, that the line counter (argument 'N') corresponds   */
/*     with the input file                                           */

        /* get length to copy */
        if (size > filedata->data) {
            copylength = filedata->data;/* copy the rest into linebuffer */
            /* copy over the data */
            memcpy(line, filedata->scan, copylength);
            line[copylength] = '\0';   /* make into ASCIIZ string    */

            /* all data should be read, filedata->data must be zero */
            filedata->data -= copylength;
            /* scan should be at the end */
            filedata->scan += copylength;/* set new scan point       */

            /* if no more data to read in the file, return OK */
            if (!filedata->residual)
                return 0;
            else
                return GetLine(line + copylength, size - copylength, filedata);
        }
        else {  /* the line is full, scan until LF found but no copy */
            if (size < filedata->data) {
                copylength = size;
            }
            else {
                copylength = filedata->data;
            }
            /* copy over the data */
            memcpy(line, filedata->scan, copylength);
            line[copylength] = '\0';   /* make into ASCIIZ string    */

            filedata->data  = 0;       /* no data in buffer          */
            filedata->scan += filedata->data;/* set scan point to end */

            if (filedata->residual) {  /* more to read               */
                ReadNextBuffer(filedata);/* read the next buffer     */
                return GetLine(line + copylength, 0, filedata);
            }
            else
                return 0;
        }
    }
}


/********************************************************************
* Function:  mystrstr(haystack, needle, hlen, nlen, sensitive)      *
*                                                                   *
* Purpose:   Determines if the string 'needle' is in the            *
*            string 'haystack' by returning it's position or        *
*            a NULL if not found.  The length of haystack and       *
*            needle are given by 'hlen' and 'nlen' respectively.    *
*                                                                   *
*            If 'sensitive' is true, then the search is case        *
*            sensitive, else it is case insensitive.                *
*                                                                   *
* RC:        num  - The pos where needle was found.                 *
*            NULL - needle not found.                               *
*                                                                   *
* Used By:   SysFileSearch()                                        *
*********************************************************************/

static unsigned long mystrstr(
    char          *haystack,
    char          *needle,
    unsigned long hlen,
    unsigned long nlen,
    bool          sensitive)
{
    char          line[MAX_LINE_LEN];
    char          target[MAX_LINE_LEN];
    unsigned long p;

    /* Copy line  - Change nulls to spaces and uppercase if needed    */
    for (p = 0; p < hlen; p++) {
        if (haystack[p] == '\0')
            line[p] = ' ';
        else if (sensitive)
            line[p] = haystack[p];
        else line[p] = (char)toupper(haystack[p]);
    }
    line[p] = '\0';

    /* Copy target  - Change nulls to spaces and uppercase if needed  */
    for (p = 0; p < nlen; p++) {
        if (needle[p] == '\0')
            target[p] = ' ';
        else if (sensitive)
            target[p] = needle[p];
        else target[p] = (char)toupper(needle[p]);
    }
    target[p] = '\0';
    return ((unsigned long)strstr(line, target));
}


/*************************************************************************
* Function:  get_next_path                                               *
*                                                                        *
* Purpose:   Read one path out of an environment value pointed to by     *
*            ppenv.                                                      *
*            Used by the 'SearchPath' func.                              *
* Return:    0 - read successful                                         *
*            1 - end of environment entry                                *
*************************************************************************/

static int get_next_path(
    char **ppenv,                    /* environment pointer              */
    char *path_buf)                  /* path buffer                      */
{
    int i;                           /* counter                          */

    if(*ppenv == NULL)               /* environment entry available ?    */
        return (1);                  /* return end of envrionment        */
    if(**ppenv == ':')               /* if we point to the seperator     */
        (*ppenv)++;                  /* jump over                        */
    if(**ppenv == '\0')              /* if end of environment entry      */
        return (1);                  /* return so                        */
    /* copy the path out of the environment entry */
    for(i=0;(**ppenv != ':')&&(**ppenv != '\0');(*ppenv)++){
        if(i>MAX_LINE_LEN)           /* if buffer to short               */
            return (1);              /* get out                          */
        path_buf[i++] = **ppenv;     /* copy this character              */
    }
    path_buf[i] = '\0';              /* terminate the string             */
    return (0);                      /* found another path               */
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

static int SearchPath(
    int  SearchFlag,                     /* search curr dir first ?    */
    char *path,                          /* environment variable name  */
    char *filename,                      /* searched file              */
    char *buf,                           /* returned filespec if found */
    int  buf_size)                       /* size of the return buffer  */
{

    int   rc = 1;                        /* init rc to not found       */
    DIR   *dp;                           /* directory pointer          */
    struct dirent *ep;                   /* directory entry pointer    */
    int   length;                        /* path length                */
    char  path_buf[IBUF_LEN];            /* current search path        */
    char  *penv;                         /* ptr to the environment     */

    if(!SearchFlag){                     /* search curr dir first ?    */
        dp = opendir("./");              /* open current directory     */
        if(dp != NULL){                  /* worked well ?              */
            while(ep = readdir(dp)){     /* while entries in the dir   */
                 /* if we have a match */
                 if(!strcmp(ep->d_name,filename)){
                     if(!getcwd(buf,buf_size)) /* copy the cwd to return buf */
                         return rc;      /* failed, get out            */
                     length = strlen(buf); /* get the length of the path */
                     if((int)(length+2+strlen(filename))>buf_size)/* check buf space */
                         return rc;      /* not enough, get out        */
                     buf[length] = '/';  /* add a slash                */
                     buf[length+1] = '\0';/* and update the terminater */
                     strcat(buf,filename);/* now complete the filespec */
                     rc = 1;             /* Yep,found !                */
                 }
            }
            (void)closedir(dp);          /* close the directory        */
        }
    }
    if(!rc)                              /* if we already found it     */
        return rc;                       /* return to caller           */

    /* Now search along the environment entry */
    penv = getenv(path);                 /* get the environment entry  */
    if(!penv)                            /* if couldn't get the env    */
        return rc;                       /* get out                    */
    /* while we have another path to search for the file     */
    while(!get_next_path(&penv,path_buf)){
        dp = opendir(path_buf);          /* open the directory         */
        if(dp != NULL){                  /* worked well ?              */
            while(ep = readdir(dp)){     /* while entries in the dir   */
                /* if we have a match */
                if(!strcmp(ep->d_name,filename)){
                    if((int)strlen(path_buf)>buf_size)/* check the size     */
                        return rc;       /* get out                    */
                    strcpy(buf,path_buf);/* copy path to the return buf*/
                    length = strlen(buf);/* get the length of the path */
                    if((int)(length+2+strlen(filename))>buf_size)/* check buf space */
                        return rc;       /* not enough, get out        */
                    buf[length] = '/';   /* add a slash                */
                    buf[length+1] = '\0';/* and update the terminater  */
                    strcat(buf,filename);/* now complete the filespec  */
                    (void)closedir(dp);  /* close the directory        */
                    return 1;            /* Yep,found !                */
                }
            }
            (void)closedir(dp);          /* close the directory        */
        }
    }
    return rc;                           /* return not found           */
}


/*****************************************************************
* Function:  getpath(string, path, filename)                     *
*                                                                *
* Purpose:  This function gets the PATH and FILENAME of the file *
*           target contained in STRING.  The path will end with  *
*           the '/' char if a path is supplied.                  *
*                                                                *
*****************************************************************/

static void getpath(
    char *string,
    char *path,
    char *filename )
{
    int    iLen;                       /* length of filespec         */
    int    LastSlashPos;               /* position of last slash     */
    char   szSavePath[IBUF_LEN];       /* Keep path to get back to   */

    if (!strcmp(string, "."))          /* period case?               */
        strcpy(string, "./*");         /* make it a * request        */
    else if (!strcmp(string, ".."))    /* double period case?        */
        strcpy(string, "../*");        /* make it a ../ * request    */
    iLen = strlen(string);             /* Get length of full file    */
                                       /* spec                       */
    LastSlashPos = iLen-1;             /* Get max pos of last '/'    */

    /* Step back through string until it begins or at last '/' char  */
    do
        LastSlashPos--;
    while((string[LastSlashPos] != '/') && (LastSlashPos>=0));

    if (string[LastSlashPos] == '/') { /* if we have a slash         */
        /* Get file name from filespec (just after last '/')         */
        if (string[LastSlashPos+1])    /* have a real name?          */
            /* copy it over */
            strcpy(filename, &string[LastSlashPos+1]);
        else
            strcpy(filename, "*");     /* just use wildcards         */
    }
    else {                             /* no '/' just filename       */
        strcpy(filename, &string[LastSlashPos+1]);
    }

    /* Now resolve to fully qualified path */
    iLen = strlen(filename);           /* Get file name length       */
    if (string[LastSlashPos] != '/') { /* if we have no slash        */
        if (!getcwd(path, (IBUF_LEN - iLen - 2)))
            strcpy(path, "./");        /* if no cwd set current dir  */
        else
            strcat(path, "/");
    }
    else {                             /* there is path info         */
        strncpy(path, string, LastSlashPos+1); /* copy the path out  */
        *(path+LastSlashPos+1) = '\0'; /* make ASCII-Z string        */
        if (getcwd(szSavePath, (IBUF_LEN - 1 ))) {
            if (!chdir(path)) {        /* If successful get           */
                if ((getcwd(path, (IBUF_LEN - iLen - 2 ))) &&
                 ( LastSlashPos > 0 ))
                    strcat(path, "/"); /* Add if not root dir         */
                chdir(szSavePath);     /* Back to current dir         */
            }
        }
    }
}


/*****************************************************************
* Function:  LinFindNextFile(path, dir_handle, finfo, d_name)    *
*                                                                *
* Purpose:  This function finds the next file in the directory   *
*           PATH pointed by DIR_HANDLE which matchs the filespec.*
*           All needed info is returned via the FINFO struct     *
*           and the D_NAME pointer.                              *
*                                                                *
* Note:  '?' is currently not supported. Add the impletmentation *
*        here !                                                  *
*****************************************************************/

static int LinFindNextFile(
    char        *filespec,             /* filespec to search for     */
    char        *path,                 /* current path               */
    DIR         *dir_handle,           /* directory handle           */
    struct stat *finfo,                /* return buf for the finfo   */
    char        **d_name)              /* name of the file found     */
{
    struct dirent *dir_entry;          /* Directory entry            */
    char full_path[IBUF_LEN+1];
    char filespec_t[IBUF_LEN+3];       /* copy of filespec for the   */
                                       /* strtok routine             */
    char *token;                       /* ptr to current token       */
    char *start_ptr;                   /* ptr to begin of filename   */
    char *found;                       /* ptr to substring found     */
    char *last_token;
    char *last_found;
    int  bad,no_delimiter, found_flag;

   if(!(dir_entry = readdir(dir_handle))) /* get first entry         */
       return 0;                       /* no entry or EOF            */

   do {
       /* make full spec             */
       sprintf(full_path, "%s%s", path, dir_entry->d_name);
       lstat(full_path, finfo);        /* read the info about it     */

       if(S_ISREG(finfo->st_mode) ||   /* if it is a file            */
        S_ISCHR(finfo->st_mode) ||     /* or a device special        */
        S_ISBLK(finfo->st_mode) ||     /* file                       */
        S_ISSOCK(finfo->st_mode) ||    /* or a socket                */
        S_ISLNK(finfo->st_mode) ||     /* or a symbolic link         */
        S_ISFIFO(finfo->st_mode)) {    /* or a FIFO                  */

           if(fnmatch(filespec,dir_entry->d_name,FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD)==0){
               *d_name = dir_entry->d_name;/* retptr to the name location */
               return 1;               /* return success             */
           }
       }
    }
    while(dir_entry = readdir(dir_handle));/* while have entries     */
    return 0;                          /* no file found or EOF       */
}


/*****************************************************************
* Function:  LinFindNextDir(path, dir_handle, finfo, d_name)     *
*                                                                *
* Purpose:  This function finds the next dir in the directory    *
*           PATH pointed by DIR_HANDLE. All needed info is       *
*           returned via the FINFO struct and the D_NAME pointer.*
*                                                                *
* Note:  '?' is currently not supported. Add the impletmentation *
*        here !                                                  *
*****************************************************************/

static int LinFindNextDir(
    const char  *filespec,             /* filespec to search for     */
    char        *path,                 /* current path               */
    DIR         *dir_handle,           /* directory handle           */
    struct stat *finfo,                /* return buf for the finfo   */
    char        **d_name)              /* name of the file found     */
{
    struct dirent *dir_entry;          /* Directory entry            */
    char full_path[IBUF_LEN+1];
    char filespec_t[IBUF_LEN+3];       /* copy of filespec for the   */
                                       /* strtok routine             */
    char *token;                       /* ptr to current token       */
    char *start_ptr;                   /* ptr to begin of filename   */
    char *found;                       /* ptr to substring found     */
    char *last_token;
    char *last_found;
    int  bad,no_delimiter, found_flag;

    if(!(dir_entry = readdir(dir_handle))) /* get first entry        */
        return 0;                      /* no entry or EOF            */

    do {
        /* make full spec */
        sprintf(full_path, "%s%s", path, dir_entry->d_name);
        lstat(full_path, finfo);       /* read the info about it     */

        if(S_ISDIR(finfo->st_mode)) {  /* if it is a directory       */

            if(fnmatch(filespec,dir_entry->d_name,FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD)==0){
                *d_name = dir_entry->d_name;/* retptr to the name location */
                return 1;              /* return success             */
            }
        }
    }
    while(dir_entry = readdir(dir_handle));/* while have entries     */
    return 0;                          /* no file found or EOF       */
}


/*********************************************************************/
/* Function: ULONG FormatFile(                                       */
/*                                                                   */
/* Purpose:  Returns the new file attribute, given the mask of       */
/*           attributes to be cleared/set and the current attribute  */
/*           settings.                                               */
/*                                                                   */
/*********************************************************************/

static unsigned long FormatFile(
    RXTREEDATA   *ldp,                 /* Pointer to local data      */
    int          *smask,               /* Mask of attributes to      */
                                       /* search for                 */
    int          *dmask,               /* Mask of attributes to set  */
    unsigned long options,             /* Search and output format   */
    struct stat  *finfo )              /* File info sturcture        */
{
    unsigned short rc;
    struct tm      *timestamp;         /* Time info about the file   */
    struct tm      stTimestamp;        /* Time info about the file   */
    char           tp;                 /* type of the entry          */

    if (options & NAME_ONLY)           /* name only?                 */
        strcpy(ldp->Temp, ldp->truefile); /* just copy it over       */
    else {
#ifdef AIX
        timestamp = localtime_r(&(finfo->st_mtime),&stTimestamp); /* get the time info */
#else
        timestamp = localtime(&(finfo->st_mtime));/* get the time info */
#endif
        /* add long time format */
        if (options & LONG_TIME)       /* need the long time format? */
            /* format as such */
            sprintf(ldp->Temp, "%4d-%02d-%02d %02d:%02d:%02d  %10lu  ",
                    timestamp->tm_year+1900,
                    timestamp->tm_mon+1,
                    timestamp->tm_mday,
                    timestamp->tm_hour,
                    timestamp->tm_min,
                    timestamp->tm_sec,
                    finfo->st_size);
        else {
            if (options & EDITABLE_TIME) /* need the "smushed" form? */
                /* format as such */
                sprintf(ldp->Temp, "%02d/%02d/%02d/%02d/%02d  %10lu  ",
                        (timestamp->tm_year)%100,
                        timestamp->tm_mon+1,
                        timestamp->tm_mday,
                        timestamp->tm_hour,
                        timestamp->tm_min,
                        finfo->st_size);
            else                       /* give the pretty form       */
                sprintf(ldp->Temp, "%2d/%02d/%02d  %2d:%02d%c  %10lu  ",
                        timestamp->tm_mon+1,
                        timestamp->tm_mday,
                        (timestamp->tm_year)%100,
                        (timestamp->tm_hour < 13?
                        timestamp->tm_hour:
                        (timestamp->tm_hour-(short)12)),
                        timestamp->tm_min,
                        ((timestamp->tm_hour < 12 ||
                        timestamp->tm_hour == 24)?'a':'p'),
                        finfo->st_size);
        }

        /* find the type of the entry */
        if(S_ISLNK(finfo->st_mode))
            tp = 'l';                  /* symbolic link              */
        else if(S_ISBLK(finfo->st_mode))
            tp = 'b';                  /* block device               */
        else if(S_ISCHR(finfo->st_mode))
            tp = 'c';                  /* character device           */
        else if(S_ISDIR(finfo->st_mode))
            tp = 'd';                  /* directory                  */
        else if(S_ISFIFO(finfo->st_mode))
            tp = 'p';                  /* FIFO                       */
        else if(S_ISSOCK(finfo->st_mode))
            tp = 's';                  /* socket                     */
        else
            tp = '-';                  /* regular file               */

        /* format the attributes now */
        sprintf(ldp->Temp, "%s%c%c%c%c%c%c%c%c%c%c  %s", ldp->Temp, tp,
                (((finfo->st_mode)&S_IREAD)?'r':'-'),
                (((finfo->st_mode)&S_IWRITE)?'w':'-'),
                (((finfo->st_mode)&S_IEXEC)?'x':'-'),
                (((finfo->st_mode)&S_IRGRP)?'r':'-'),
                (((finfo->st_mode)&S_IWGRP)?'w':'-'),
                (((finfo->st_mode)&S_IXGRP)?'x':'-'),
                (((finfo->st_mode)&S_IROTH)?'r':'-'),
                (((finfo->st_mode)&S_IWOTH)?'w':'-'),
                (((finfo->st_mode)&S_IXOTH)?'x':'-'),
                ldp->truefile);
    }
    /* Place new string in Stem */
    ldp->vlen = strlen(ldp->Temp);
    ldp->count++;
    sprintf(ldp->varname+ldp->stemlen, "%ld", ldp->count);
    ldp->shvb.shvnext = NULL;
    ldp->shvb.shvname.strptr = ldp->varname;
    ldp->shvb.shvname.strlength = strlen(ldp->varname);
    ldp->shvb.shvvalue.strptr = ldp->Temp;
    ldp->shvb.shvvalue.strlength = ldp->vlen;
    ldp->shvb.shvnamelen = ldp->shvb.shvname.strlength;
    ldp->shvb.shvvaluelen = ldp->vlen;
    ldp->shvb.shvcode = RXSHV_SET;
    ldp->shvb.shvret = 0;

    rc = RexxVariablePool(&ldp->shvb);
    if (rc & (RXSHV_BADN | RXSHV_MEMFL)) {
        return 1;
    }
    return 0;                          /* good return                */
}


/*****************************************************************************
* Function: RecursiveFindFile( FileSpec, path, lpd, smask, dmask, options )  *
*                                                                            *
* Purpose:  Finds all files starting with FileSpec, and will look down the   *
*           directory tree if required.                                      *
*                                                                            *
* Params:   FileSpec - ASCIIZ string which designates filespec to search     *
*                       for.                                                 *
*           path     - ASCIIZ string for current path                        *
*                                                                            *
*           ldp      - Pointer to local data structure.                      *
*                                                                            *
*           smask    - Array of integers which describe the source attribute *
*                       mask.  Only files with attributes matching this mask *
*                       will be found.                                       *
*                                                                            *
*           dmask    - Array of integers which describe the target attribute *
*                       mask.  Attributes of all found files will be set     *
*                       using this mask.                                     *
*                                                                            *
*             Note:  Both source and targets mask are really arrays of       *
*                    integers.  Each index of the mask corresponds           *
*                    to a different file attribute.  Each indexe and         *
*                    its associated attribute follows:                       *
*                                                                            *
*                         mask[0] = FILE_ARCHIVED                            *
*                         mask[1] = FILE_DIRECTORY                           *
*                         mask[2] = FILE_HIDDEN                              *
*                         mask[3] = FILE_READONLY                            *
*                         mask[4] = FILE_SYSTEM                              *
*                                                                            *
*                    A negative value at a given index indicates that        *
*                    the attribute bit of the file is not set.  A positive   *
*                    number indicates that the attribute should be set.      *
*                    A value of 0 indicates a "Don't Care" setting.          *
*                                                                            *
*           options  - The search/output options.  The following options     *
*                       may be ORed together when calling this function:     *
*                                                                            *
*                    RECURSE     - Indicates that function should search     *
*                                   all child subdirectories recursively.    *
*                    DO_DIRS     - Indicates that directories should be      *
*                                   included in the search.                  *
*                    DO_FILES    - Indicates that files should be included   *
*                                   in the search.                           *
*                    NAME_ONLY   - Indicates that the output should be       *
*                                   restricted to filespecs only.            *
*                    EDITABLE_TIME - Indicates time and date fields should   *
*                                   be output as one timestamp.              *
*                    LONG_TIME   - Indicates time and date fields should     *
*                                   be output as one long formatted timestamp*
*                                                                            *
*****************************************************************************/

static long RecursiveFindFile(
    char          *FileSpec,           /* Filespecs to search for    */
    char          *path,               /* current directory          */
    RXTREEDATA    *ldp,                /* Pointer to local data      */
    int           *smask,              /* Mask of attributes to      */
                                       /* search for                 */
    int           *dmask,              /* Mask of attributes to set  */
    unsigned long options )            /* Search and output format   */
                                       /* options                    */
{
    char  tempfile[MAX+1];             /* Used to hold temp file name*/
    DIR *dir_handle;                   /* Directory handle           */
    struct stat finfo;                 /* file information           */
    char *filename;

    /********************************************************************
    * First, process all of the normal files, saving directories for    *
    * last.                                                             *
    ********************************************************************/

    if(!(dir_handle = opendir(path))) {/* get the directory handle   */
        return 1;                      /* return  on non-zero        */
    }

    if (options&DO_FILES &&            /* if processing files        */
                                       /* and have some              */
     (LinFindNextFile(ldp->TargetSpec,path ,dir_handle,&finfo, &filename))) {
                                       /* Get the rest of the files  */
        do {
                                       /* build the full name        */
            sprintf(ldp->truefile, "%s%s", path, filename);
                                       /* passed back ok?            */
            if (FormatFile(ldp, smask, dmask, options, &finfo)) {
                closedir(dir_handle);  /* close the search           */
                return 1;              /* error on non-zero          */
            }
        } while (LinFindNextFile(ldp->TargetSpec,path, dir_handle, &finfo, &filename));
    }
    closedir(dir_handle);              /* reset the                  */
    dir_handle = opendir(path);        /* directory handle (rewinddir*/
                                       /* doesn't work!)             */
    if (options & DO_DIRS  &&          /* need directories?          */
                                       /* and have some              */
     (LinFindNextDir(ldp->TargetSpec,path,dir_handle,&finfo,&filename))) {
        do {
            /* dot directory? */
            if (!strcmp(filename, ".") || !strcmp(filename, ".."))
                continue;              /* skip this one              */
            /* build the full name */
            sprintf(ldp->truefile, "%s%s", path, filename);
            /* passed back ok? */
            if (FormatFile(ldp, smask, dmask, options, &finfo)) {
                closedir(dir_handle);  /* close the search           */
                return 1;              /* error on non-zero          */
            }
        } while (LinFindNextDir(ldp->TargetSpec,path, dir_handle, &finfo, &filename));
    }
    closedir(dir_handle);              /* reset the                  */
    dir_handle = opendir(path);        /* directory handle (rewinddir*/
                                       /* doesn't work!)             */
    if (options & RECURSE) {           /* need to recurse?           */
                                       /* and have some              */
        if (LinFindNextDir("*",path,dir_handle,&finfo,&filename)) {
            do {
                                       /* dot directory?             */
                if (!strcmp(filename, ".") ||
                 !strcmp(filename, ".."))
                    continue;          /* skip this one              */
                /* build the new directory */
                sprintf(tempfile, "%s%s/", path, filename);
                                       /* search the next level      */
                if (RecursiveFindFile(ldp->TargetSpec, tempfile, ldp,
                                      smask, dmask, options)) {
                    closedir(dir_handle); /* close the search        */
                    return 1;             /* error on non-zero       */
                }
            } while (LinFindNextDir("*",path,dir_handle,&finfo,&filename));
        }
    }
    closedir(dir_handle);
    return 0;                          /* finished                   */
}


void restore_terminal(int signal){

  stty(STDIN_FILENO,&in_orig);          /* restore the terminal settings        */
  SysInitialize();                      /* restore all signal handler           */
  raise(signal);                        /* propagate signal                     */
}


/******************************************************************************/
/* getkey                                                                     */
/******************************************************************************/
/* echo == false => no echo                                                   */
/* echo == true  => echo                                                      */
/******************************************************************************/

static int getkey(char *ret, bool echo)
{
    /* restore original TTY settings on exit */

    int ttyfd = STDIN_FILENO;            /* standard tty filedescriptor */
    int tty_mode = -1;                   /* 1: raw, 0: cooked, -1: initial */
    int need_tty_reset = 0;              /* do we need a reset       */
    /* Set the cleanup handler for unconditional process termination */
    struct sigaction new_action;


    /* Set up the structure to specify the new action */
    new_action.sa_handler = restore_terminal;
    sigfillset(&new_action.sa_mask);
    new_action.sa_flags = SA_RESTART;

    /* Termination signals */
    sigaction(SIGINT, &new_action, NULL); /* exitClear on SIGINT signal */
    sigaction(SIGTERM, &new_action, NULL);/* exitClear on SIGTERM signal */
    sigaction(SIGQUIT, &new_action, NULL);/* exitClear on SIGQUIT signal */
    sigaction(SIGHUP, &new_action, NULL); /* exitClear on SIGHUP signal */
    sigaction(SIGTSTP, &new_action, NULL);/* exitClear on SIGTSTP signal */
    sigaction(SIGTTIN, &new_action, NULL);/* exitClear on SIGTTIN signal */
    sigaction(SIGTTOU, &new_action, NULL);/* exitClear on SIGTTOU signal */
    /* Error signals */
    sigaction(SIGSEGV, &new_action, NULL); /* exitClear on SIGSEGV signal */
    sigaction(SIGFPE, &new_action, NULL);  /* exitClear on SIGFPE signal */
    sigaction(SIGILL, &new_action, NULL);  /* exitClear on SIGILL signal */
    sigaction(SIGBUS, &new_action, NULL);  /* exitClear on SIGBUS signal */
    sigaction(SIGPIPE, &new_action, NULL); /* exitClear on broken pipe */

    if( !isatty(ttyfd) ){                /* connected to a terminal? */
        ret[0] = '\0';
        return 0;
    }
    ttyfd = STDIN_FILENO;                /* STDIN_FILENO is out default fd */

    /* open tty */
    ttyfd = open("/dev/tty", O_RDONLY);  /* get filedescriptor (fd) for tty */

    struct termios in_raw;               /* global for save reset after SIGNAL */

    gtty(ttyfd, &in_orig);               /* determine existing tty settings */

    /* restore original TTY settings on exit */

    /* change STDIN settings to raw */
    gtty(ttyfd, &in_raw);                /* save old settings        */

    in_raw.c_lflag &= ~ICANON;           /* no canonical mode        */
    if (!echo)                           /* no echo flag set         */
        in_raw.c_lflag &= ~ECHO;         /* no echo                  */
    in_raw.c_cc[VMIN] = 1;               /* read 1 byte before returning */
    in_raw.c_cc[VTIME] = 0;              /* return immediatly (no timeout) */
    stty(ttyfd, &in_raw);                /* execute settings now!    */

    ret[0] = getchar();                  /* read the char            */

    ret[1] = '\0';                       /* terminate string         */

    restore_tty(&in_orig);               /* for standard I/O behavior */
    close(ttyfd);                        /* close the terminal       */
    return 0;                            /* everything is fine       */
}


unsigned long APIENTRY RexxStemSort(char *stemname, int order, int type,
    size_t start, size_t end, size_t firstcol, size_t lastcol);


/*********************************************************************/
/****************  REXXUTIL Functions  *******************************/
/****************  REXXUTIL Functions  *******************************/
/****************  REXXUTIL Functions  *******************************/
/*********************************************************************/


/*************************************************************************
* Function:  SysSleep                                                    *
*                                                                        *
* Syntax:    call SysSleep secs                                          *
*                                                                        *
* Params:    secs - Number of seconds to sleep.                          *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*************************************************************************/

RexxMethod1(int, SysSleep, double, timein)
{
    long  secs;                        /* Time to sleep in secs      */
    long  nanoseconds;                 /* decimals value             */
    long  digits;                      /* number of decimal digits   */
    struct timeval tv;                 /* time for the select func   */
#if defined( HAVE_NANOSLEEP )
    struct timespec    Rqtp, Rmtp;
#elif defined( HAVE_NSLEEP )
    struct timestruc_t Rqtp, Rmtp;
#endif
    long nano;


    secs = (long)timein;                     /* get non-fractional part    */
    timein -= (double)secs;                    /* get the fractional part    */
    nanoseconds = (long)(timein * 100000000);
#if defined( HAVE_NANOSLEEP )
    Rqtp.tv_sec = secs;
    Rqtp.tv_nsec = nanoseconds;
    nanosleep(&Rqtp, &Rmtp);
#elif defined( HAVE_NSLEEP )
    Rqtp.tv_sec = secs;
    Rqtp.tv_nsec = nanoseconds;
    nsleep(&Rqtp, &Rmtp);
#else
    sleep( secs );
#endif
    return 0;                          /* this worked ok             */
}


/**********************************************************************
* Function:  SysCls                                                   *
*                                                                     *
* Syntax:    call SysCls                                              *
*                                                                     *
* Return:    NO_UTIL_ERROR - Successful.                              *
**********************************************************************/

RexxFunction0(int, SysCls)
{
    system("clear");                   /* do the clear               */             // think about the use of 'execve', Weigold
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

RexxFunction1(int, SysMkDir, CSTRING, path)
{
    unsigned long rc;                  /* Ret code of func           */
    char          *home_dir = NULL;    /* home directory path        */
    char          *dir_buf = NULL;     /* full directory path        */

    /* check for using '~/'       */
    if(*(path) == '~'){
        dir_buf = resolve_tilde(path); /* get absolut path           */
        path = dir_buf;                /* directory to make          */
    }
    /* Make the dir; standard permissions are rwxr-xr-x                */
    /* we do not restrict permission, this is done by root in the file */
    /* /etc/security/user. We allow anything. System restricts         */
    /* according to the user settings --> smitty/user                  */
    rc = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    if(dir_buf)                        /* did we allocate ?          */
        free(dir_buf);                 /* free the buffer memory     */
    if(!rc){                           /* if worked well             */
        return (int)rc;
    }
    else {                             /* if there was an error      */
        switch (errno) {
            case EACCES:
                rc = 5;                           /* result: Access denied   */
                break;
            case EEXIST:
                rc = 87;                          /*result: already exists   */
                break;
            case EMLINK:
                rc = 206;                          /* result: exceeds range  */
                break;
            case EROFS:
                rc = 108;                          /* result:read only system*/
                break;
            default:
                rc = 2;                            /* result: file not found */
                break;
        }
    }
    return rc;
}


/*************************************************************************
* Function:  SysRmDir                                                    *
*                                                                        *
* Syntax:    call SysRmDir dir                                           *
*                                                                        *
* Params:    dir - Directory to be removed.                              *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*                                                                        *
*************************************************************************/

RexxFunction1(int, SysRmDir, CSTRING, path)
{
    unsigned long rc;                  /* Ret code of func           */
    char          *home_dir = NULL;    /* home directory path        */
    char          *dir_buf = NULL;     /* full directory path        */

    /* check for using '~/'       */
    if(*(path) == '~'){
        dir_buf = resolve_tilde(path); /* get absolut path           */
        path = dir_buf;                /* directory to remove        */
    }

    rc = rmdir(path);                  /* Remove the directory       */
    if(dir_buf)                        /* did we allocate?           */
        free(dir_buf);                 /* free the buffer memory     */
    if(!rc){                           /* if worked well             */
        return rc;
    }
    else {                             /* if there was an error      */
        switch (errno) {
            case EACCES:
                rc = 5;                           /* result: Access denied */
                break;
            case EEXIST:
                rc = 87;                          /*result: already exists */
                break;
#ifdef LINUX
            case ENOTEMPTY:                   /* somtimes used for EEXITST */
                rc = 87;                          /*result: already exists */
                break;
#endif
            case EBUSY:
                rc = 5;                        /* result: currently in use */
                break;
            case ENOENT:
                rc = 87;                         /* result: doesn't exitst */
                break;
            case EROFS:
                rc = 108;                       /* result:read only system */
                break;
            default:
                rc = 2;                           /* result: dir not found */
                break;
        }
    }
    return rc;
}


/*************************************************************************
* Function:  SysFileDelete                                               *
*                                                                        *
* Syntax:    call SysFileDelete file                                     *
*                                                                        *
* Params:    file - file to be deleted.                                  *
*                                                                        *
* Return:    Return code from DosDelete() function.                      *
*************************************************************************/

RexxMethod1(int, SysFileDelete, CSTRING, path)
{
    int   rc;                          /* Ret code of func           */
    char  *home_dir = NULL;            /* home directory path        */
    char  *dir_buf = NULL;             /* full directory path        */

    /* check for using '~/' */
    if(*(path) == '~'){
        dir_buf = resolve_tilde(path); /* get absolut path           */
        path = dir_buf;                /* file to remove             */
    }

    rc = remove(path);                 /* Remove the file            */
    if(rc){                            /* if error, get error code   */
        rc = errno;
    }
    if(dir_buf)                        /* did we allocate ?          */
        free(dir_buf);                 /* free the buffer memory     */
    return rc;
}


/*************************************************************************
* Function:  SysFileSearch                                               *
*                                                                        *
* Syntax:    call SysFileSearch target, file, stem [, options]           *
*                                                                        *
* Params:    target  - String to search for.                             *
*            file    - Filespec to search.                               *
*            stem    - Stem variable name to place results in.           *
*            options - Any combo of the following:                       *
*                       'C' - Case sensitive search (non-default).       *
*                       'N' - Preceed each found string in result stem   *
*                              with it line number in file (non-default).*
*                                                                        *
* Return:    NO_UTIL_ERROR   - Successful.                               *
*            ERROR_NOMEM     - Out of memory.                            *
*************************************************************************/

RexxFunction4(int, SysFileSearch, CSTRING, target, CSTRING, file, RexxStemObject, stem, OPTIONAL_CSTRING, opts)
{
    unsigned long ptr;                 /* Pointer to char str found  */
    int           num = 0;             /* Line number                */
    unsigned long len;                 /* Length of string           */
    unsigned long len2;                /* Length of string           */
    char          ibuf[1024];
    bool          linenums = false;    /* Set true for linenums in   */
                                       /* output                     */
    bool          sensitive = false;   /* Set true for case-sens     */
                                       /* search                     */
    GetFileData   filedata;            /* file read information      */
    char          *home_dir;           /* ptr to the environment     */
    char          *dir_buf = NULL;     /* directory buffer           */
    char          *line = NULL;        /* Line read from file        */
    size_t        count = 0;           /* number of stem members     */
    size_t        vlen;

    if (opts != NULL)                  /* process options            */
    {
        if (strstr(opts, "N") || strstr(opts, "n"))
          linenums = true;

        if (strstr(opts, "C") || strstr(opts, "c"))
          sensitive = true;
    }

    if(*(file) == '~'){                /* check for using '~/'       */
        dir_buf = resolve_tilde(file); /* get absolut path           */
        file = dir_buf;                /* full path to the file      */
    }
                                       /* Initialize data area       */
    if (OpenFile(file, &filedata)) {   /* open the file              */
        if(dir_buf)                    /* did we allocate ?          */
            free(dir_buf);             /* free it                    */
        return 3;
    }

    line = (char *) malloc(4096 * sizeof(char));
                                       /* do the search...found lines*/
                                       /* are saved in stem vars     */
    while (!GetLine(line, MAX_LINE_LEN - 1, &filedata)) {
        len = strlen(line);
        num++;

        ptr = mystrstr(line, target, len, strlen(target), sensitive);
        if (ptr != '\0') {
            if (linenums) {
                sprintf(ibuf, "%d ", num);
                len2 = strlen(ibuf);
                if (len < sizeof(ibuf) - len2) {
                    memcpy(ibuf+len2, line, len);
                }
                else {
                    memcpy(ibuf+len2, line, sizeof(ibuf) - len2);
                }
                if (sizeof(ibuf) < len + len2) {
                    vlen = sizeof(ibuf);
                }
                else {
                    vlen = len + len2;
                }
            }
            else {
                memcpy(ibuf, line, len);
                vlen = len;
            }
            count++;
            if (ibuf[vlen - 1] == '\n')
            {
                vlen--;
            }
            context->SetStemArrayElement(stem, count, context->NewString(ibuf, vlen));
        }
    }
    free(line);
    CloseFile(&filedata);              /* Close that file            */
    /* set stem.0 to lines read   */
    context->SetStemArrayElement(stem, 0, context->NumberToObject(count));
    if(dir_buf)                        /* did we allocate ?          */
        free(dir_buf);                 /* free it                    */
    return 0;                          /* no error on call           */
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

RexxFunction3(RexxStringObject, SysSearchPath, CSTRING, path, CSTRING, file, OPTIONAL_CSTRING, option)
{
    char     buf[IBUF_LEN]={0};        /* returned file name         */
    char    *opts;                     /* option string              */
    int      SearchFlag;               /* Search control variable    */
                                       /* default=search current 1st */
    int      ulRc;                     /* return value of SearchPath */

    SearchFlag = CURRENT_DIR_FIRST;    /* set default search flag    */

    if (option != NULL) {          /* process options            */
        opts = option;                 /* point to the options       */
        if ((*opts == 'N') || (*opts == 'n'))
            SearchFlag = ENVIRONMENT_ONLY ;
                                       /* do not search current dir  */

        else if ((*opts == 'C') || (*opts == 'c'))
            SearchFlag = CURRENT_DIR_FIRST;
                                       /* search current 1st(default)*/
        else {
            context->InvalidRoutine();
            return context->NullString();
        }
    }
    ulRc = SearchPath(SearchFlag, path, file,
                      (char *)buf, sizeof(buf));

    /* if environment variable could not be found, try again with    */
    /* uppercase name.                                               */
    if (ulRc) {
        strupr( path );
        ulRc = SearchPath(SearchFlag, path, option,
                          (char *)buf, sizeof(buf));
    }

    return context->NewStringFromAsciiz(buf);
}


/*************************************************************************
* Function:  SysVersion                                                  *
*                                                                        *
* Syntax:    call SysVersion                                             *
*                                                                        *
* Return:    Operating System name (LINUX/AIX/WINDOWS) and Version       *
*************************************************************************/

RexxFunction0(RexxObjectPtr, SysVersion)
{
    struct utsname info;               /* info structur              */
    char   retbuf[256];

    if(uname(&info) < 0) {             /* if no info stored          */
        context->InvalidRoutine();
        return context->NullString();
    }

#ifdef AIX
    snprintf(retbuf, sizeof(retbuf),"%s %s.%s",info.sysname,info.version, info.release);
#else
    snprintf(retbuf, sizeof(retbuf),"%s %s",info.sysname,info.release);
#endif
    return context->NewStringFromAsciiz(retbuf);
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

RexxFunction2(RexxObjectPtr, SysCreateEventSem, OPTIONAL_CSTRING, name, OPTIONAL_CSTRING, reset)
{
    RXSEMDATA *semdata;
    int rc;

    // Note that the reset arg has no meaning on Unix/Linux and is unused.
    semdata = (RXSEMDATA *)malloc(sizeof(RXSEMDATA));
    if (semdata == NULL) {
        return 0;
    }
    if (strlen(name) == 0) {
        /* this is an unnamed semaphore */
        semdata->handle = (sem_t *)malloc(sizeof(sem_t));
        rc = sem_init(semdata->handle, 0, 0);
        if (rc == -1) {
            free(semdata);
            return context->NullString();
        }
        semdata->named = false;
    }
    else {
        /* this is a named semaphore */
        sem_unlink(name);
        semdata->handle = sem_open(name, (O_CREAT | O_EXCL), (S_IRWXU | S_IRWXG), 0);
        if (semdata->handle == SEM_FAILED ) {
            free(semdata);
            return context->NullString();
        }
        semdata->named = true;
    }
    return context->UintptrToObject((uintptr_t)semdata);
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

RexxMethod1(int, SysOpenEventSem, CSTRING, name)
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
    return (uintptr_t)semdata->handle;
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

RexxMethod1(int, SysResetEventSem, POINTER, vhandle)
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

RexxMethod1(int, SysPostEventSem, POINTER, vhandle)
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

RexxMethod1(int, SysCloseEventSem, POINTER, vhandle)
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

RexxFunction2(int, SysWaitEventSem, POINTER, vhandle, OPTIONAL_int, timeout)
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
            if (usleep(SEM_WAIT_PERIOD * 1000)) {
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

RexxFunction1(RexxStringObject, SysCreateMutexSem, OPTIONAL_CSTRING, name)
{
    RXSEMDATA *semdata;
    int rc;

    semdata = (RXSEMDATA *)malloc(sizeof(RXSEMDATA));
    if (semdata == NULL) {
        return context->NullString();
    }
    if (strlen(name) == 0) {
        /* this is an unnamed semaphore */
        semdata->handle = (sem_t *)malloc(sizeof(sem_t));
        rc = sem_init(semdata->handle, 0, 0);
        if (rc == -1) {
            free(semdata);
            return context->NullString();
        }
        semdata->named = false;
    }
    else {
        /* this is a named semaphore */
        sem_unlink(name);
        semdata->handle = sem_open(name, (O_CREAT | O_EXCL), (S_IRWXU | S_IRWXG), 0);
        if (semdata->handle == SEM_FAILED ) {
            free(semdata);
            return context->NullString();
        }
        semdata->named = true;
    }
    rc = sem_post(semdata->handle);
    return context->UintptrToObject((uintptr_t)semdata);
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

RexxFunction1(int, SysOpenMutexSem, CSTRING, name)
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
    return (uintptr_t)semdata->handle;
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

RexxFunction2(int, SysRequestMutexSem, POINTER, vhandle, OPTIONAL_int, timeout)
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
            if (usleep(SEM_WAIT_PERIOD * 1000)) {
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

RexxFunction1(int, SysReleaseMutexSem, POINTER, vhandle)
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

RexxFunction1(int, SysCloseMutexSem, POINTER, vhandle)
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


/*************************************************************************
* Function:  SysFileTree                                                 *
*                                                                        *
* Syntax:    call SysFileTree filespec, stem [, options]                 *
*                                                                        *
* Params:    filespec - Filespec to search for (may include * and ?).    *
*            stem     - Name of stem var to store results in.            *
*            options  - Any combo of the following:                      *
*                        'B' - Search for files and directories.         *
*                        'D' - Search for directories only.              *
*                        'F' - Search for files only.                    *
*                        'O' - Only output file names.                   *
*                        'S' - Recursively scan subdirectories.          *
*                        'T' - Combine time & date fields into one.      *
*                        'L' - Long time format                          *
*                                                                        *
* Return:    NO_UTIL_ERROR   - Successful.                               *
*            ERROR_NOMEM     - Out of memory.                            *
*************************************************************************/

RexxFunction5(int, SysFileTree, CSTRING, filespec, RexxStemObject, stem, OPTIONAL_CSTRING, stroptions, OPTIONAL_CSTRING, mask, OPTIONAL_CSTRING, newMask)
{
    char          path[SysFileSystem::MaximumFileNameBuffer]; /* path to search along       */
    char          *optptr;             /* option scan pointer        */
    unsigned long options;             /* Mask of options            */
    unsigned long y;                   /* Temp counter (II)          */
    int           smask[5];            /* Source attribute mask      */
    int           dmask[5];            /* Target attribute mask      */
    RxTreeData    ldp;                 /* local data                 */
    char          *temp;
    int           rc;

    options = DO_FILES|DO_DIRS;        /* Clear if we should not     */
                                       /* display files              */
    smask[0] = IGNORE;                 /* No mask unless specified   */
    dmask[0] = IGNORE;                 /* No mask unless specified   */
    path[0] = '\0';                    /* no path yet                */
    rc = 0;                            /* pass back result           */
    char FileSpec[SysFileSystem::MaximumFileNameBuffer];

                                       /* validate arguments         */
    if (strlen(filespec) > 255) {
        context->InvalidRoutine();
        return 0;
    }
    /* initialize data area */
    ldp.count = 0;
    strcpy(ldp.varname, context->ObjectToStringValue(context->GetStemValue(stem)));
    ldp.stemlen = strlen(ldp.varname);
    /* uppercase the name  */
    temp = ldp.varname;
    for(unsigned int k=0;k<strlen(ldp.varname);k++) {/* loop through mem and  */
        *temp = toupper(*temp);        /* uppercase each char        */
        temp++;
    }

    if (ldp.varname[ldp.stemlen-1] != '.')
        ldp.varname[ldp.stemlen++] = '.';

    strcpy(FileSpec, filespec);        /* get file spec              */

    /* if filespec is '*' then use './ *' */
    if((strlen(FileSpec)==1)&&(FileSpec[0] == '*'))
        strcpy(FileSpec, "./*");

    /* If FileSpec ends in / then append */
    if (FileSpec[strlen(FileSpec)-1] == '/')
        strcat(FileSpec, "*");

    /* if filespec begins with '~' generate absolute path */
    if(FileSpec[0] == '~'){
        if(!(temp = resolve_tilde(FileSpec)))/* get the absolute path*/
            return INVALID_ROUTINE;
        strcpy(FileSpec,temp);         /* work with the new one      */
        free(temp);                    /* free temp space            */
    }

    if (strlen(stroptions) > 0) {      /* check third option         */
        optptr = stroptions;           /* copy the pointer           */
        while (*optptr) {              /* while more characters      */
            switch(toupper(*optptr)) { /* process each option        */
                case 'S':              /* recurse on subdirectories  */
                    options |= RECURSE;/* Should we recurse          */
                    break;
                case 'O':              /* only return names          */
                    options |= NAME_ONLY; /* Should include names only */
                    break;
                case 'T':              /* editable time?             */
                    options |= EDITABLE_TIME; /* create editable timestamp */
                    break;
                case 'L':              /* long time format?          */
                    options |= LONG_TIME; /* create timestamp        */
                    break;
                case 'F':              /* include only files?        */
                    options &= ~DO_DIRS; /* Should not include dirs! */
                    options |= DO_FILES; /* Should include files !   */
                    break;
                case 'D':              /* include only directories?  */
                    options |= DO_DIRS; /* Should include dirs !     */
                    options &= ~DO_FILES; /* Should not include files! */
                    break;
                case 'B':              /* include both files and dirs*/
                    options |= DO_DIRS; /* Should include dirs !     */
                    options |= DO_FILES; /* Should include files !   */
                    break;
                default:               /* unknown option             */
                    return INVALID_ROUTINE; /* raise an error        */
            }
            optptr++;                  /* step the option pointer    */
        }
    }
    /* get path and name */
    getpath(FileSpec, path, ldp.TargetSpec);
    /* recursively search */
    if (RecursiveFindFile(FileSpec, path, &ldp, smask, dmask, options)) {
        context->InvalidRoutine();
        return 0;
    }
    /* return lines read */
    sprintf(ldp.Temp, "%d", (int)ldp.count);
    ldp.varname[ldp.stemlen] = '0';
    ldp.varname[ldp.stemlen+1] = 0;
    ldp.shvb.shvnext = NULL;
    ldp.shvb.shvname.strptr = ldp.varname;
    ldp.shvb.shvname.strlength = ldp.stemlen+1;
    ldp.shvb.shvnamelen = ldp.stemlen+1;
    ldp.shvb.shvvalue.strptr = ldp.Temp;
    ldp.shvb.shvvalue.strlength = strlen(ldp.Temp);
    ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
    ldp.shvb.shvcode = RXSHV_SET;
    ldp.shvb.shvret = 0;
    if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
        context->InvalidRoutine();
        return 0;
    }

    return 0;                          /* no error on call           */
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

RexxFunction2(RexxStringObject, SysTempFileName, CSTRING, ltemplate, OPTIONAL_CSTRING, fillerArg)
{
    RexxStringObject temp;
    char          filler = '?';        /* filler character           */
    char          numstr[6];
    unsigned long num, start, max;
    char          *array;
    char          *dir;                /* the directory              */
    char          *file;               /* the file prefix            */
    char          *tmp;                /* temporary                  */
    int           x, j, i;

    j = 0;                             /* get a copy of the argument */
    i = 0;
    max = 1;

    if (fillerArg != NULL)
    {
        filler = *fillerArg;
    }
                                       /* get the file id            */
    dir = (char*) malloc(strlen(ltemplate) + 1);
    if (dir == NULL){                  /* if something went wrong    */
        return context->NullString();
    }

    strcpy(dir, ltemplate);       /* copy the string            */

    tmp = dir;                         /* set temporary              */

    /* search for filter        *********/

    for (x = 0; tmp[x] != 0; x++) {
        if (tmp[x] == filler) {
            max = max *10;
            j++;
        }
    }

    if (j) {
        srand(time(0));
        num = rand();
        num = num % max;

        switch (j) {
            case 1:
                sprintf(numstr, "%01lu", num);
                break;
            case 2:
                sprintf(numstr, "%02lu", num);
                break;
            case 3:
                sprintf(numstr, "%03lu", num);
                break;
            case 4:
                sprintf(numstr, "%04lu", num);
                break;
            case 5:
                sprintf(numstr, "%05lu", num);
                break;
            default:
//              return INVALID_ROUTINE;/* raise error condition      */
                return context->NullString();
        }                              /* for compatibility          */

        for (x = 0; tmp[x] !=0; x++) {
            if (tmp[x] == filler) {
                tmp[x] = numstr[i++];
            }
        }
    }                                  /* if we need the filler      */

    while(*tmp != 0 ) {                /* lets start at the end      */
        tmp++;
    }

    while((*tmp != '/') && (*tmp != '\\') && (tmp > dir)) {
        --tmp;
    }

    if(tmp == dir) {                   /* directory string is '' or '/' */
        if(*dir == '\\'){
            file = dir+1;
            array = tempnam(NULL,file);/* call system routine        */
        }
        else if(*dir == '/'){
            file = dir+1;
            array = tempnam("/",file); /* call system routine        */
        }
        else{
            file = dir;
            array = tempnam(NULL,file);/* call system routine        */
        }
    }
    else {                             /* directory string exists    */
        file = tmp+1;                  /* set filename prefix        */
        *tmp = '\0';                   /* terminate directory string */
        array = tempnam(dir,file);     /* call system routine        */
    }

    temp = context->NewStringFromAsciiz(array);
    free (array);
    free(dir);                         /* free local string          */

    return temp;
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

RexxFunction2(int, SysSetPriority, int32_t, pclass, int32_t, level)
{
    unsigned long rc = 0;              /* creation return code       */

    if ((unsigned long)pclass == 0){   /* class 0 -> no change       */
      rc = 0;                          /* no error                   */
    }
    /* change the priority        */
    else if (((unsigned long)pclass > 0) && ((unsigned long)pclass <= 4)){
        int pid;                       /* PID                        */
        pid = getpid();                /* current PID                */

        int priority;                  /* Priority                   */
                                       /* current priority           */
        priority = getpriority(PRIO_PROCESS, getpid());

        /* Set new priority           */
        setpriority(PRIO_PROCESS, getpid(),-level);
        rc = 0;
    }
    else{
        rc = 307;
        context->InvalidRoutine();
    }

    return rc;
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

RexxFunction4(RexxObjectPtr, SysGetMessageX,
              OPTIONAL_CSTRING, filename,
              RexxNumber, setnum,
              RexxUnsignedNumber, msgnum,
              ARGLIST, ins_string)
{
        nl_catd catalog;                   /* catalog handle             */
                                           /* default error msg          */
        char default_message[] = {"Error: Message catalog not open !\0"};
                                           /* msg not found  msg         */
        char not_found_message[] = {"Error: Message not found !\0"};
                                           /* insertion error  msg       */
        char error_insertions[] = {"Error: Unable to generate message \
                                  (wrong insertions)\0"};
                                           /* cat not found  msg         */
        char cat_not_found_message[] = {"Error: Message catalog not found !\0"};
        char *msg;                         /* msg retrieved from catalog */
        int icount;                        /* number of insertions       */
        int msg_length = 0;                /* length of the return msg   */
        const char *msgfile;               /* name of the message file   */
        char *temp;
        int count = 0;                     /* number of '%s' in the msg  */
        char *retbuf;

        /* get message number */
        if (msgnum < 0)
            context->InvalidRoutine();
            return context->NullString();

        /* Get message file name. Use "rexx.cat if not given */
        if (filename != NULL)
            msgfile = filename;            /* use provided message file  */
        else
            msgfile = REXXMESSAGEFILE;

    #ifdef AIX
        setlocale(LC_ALL, "en_US");
    #endif
                                           /* open the catalog           */
        if((catalog = catopen(msgfile, NL_CAT_LOCALE)) == (nl_catd)-1){
            return context->NewStringFromAsciiz(cat_not_found_message);
        }

        /* retrieve msg from catalog */
        msg = catgets(catalog, (int)setnum, (int)msgnum, default_message);

        if(*msg == '\0')                   /* if empty string returned   */
            msg = not_found_message;       /* it means msg not found     */

        /* set number of insertions */
        icount = context->ArraySize(ins_string);

        /* calculate length of the return message */
        for (int i = 0; i < icount; i++) {
            msg_length += strlen(context->ObjectToStringValue(context->ArrayAt(ins_string, i)));
        }
        msg_length += strlen(msg);
        msg_length -= icount*2;

        /* alloc needed space for the return message (add 100 for default msgs) */
        if(!(retbuf = (char *)malloc(msg_length+100))){
            strcpy(retbuf, "Error: No memory");
            return context->NewStringFromAsciiz(retbuf);
        }

        /* check for too much '%s' in the message */
        temp = msg;
        /* replace all &1..&9 with %s */
        while(temp = strstr(temp, "&")){
            if(isdigit(*(temp+1))){        /* replace &1..&9 ?           */
                *(temp++) = '%';
                *(temp++) = 's';           /* %s expected                */
            }
            else
                temp++;
        }
        /* now look for number of replacement variables                  */
        temp = msg;                        /* reset temp pointer         */
        while(temp = strstr(temp,"%s")){   /* search for the %s          */
            count ++;                      /* increment counter          */
            temp += 2;                     /* jump over %s               */
        }
        if(count > icount)
            icount = 10;                   /* go to error case           */

        /* generate full message with insertions */
        switch(icount){
            case(1):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(2):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(3):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 2)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(4):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 2)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 3)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(5):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 2)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 3)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 4)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(6):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 2)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 3)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 4)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 5)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(7):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 2)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 3)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 4)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 5)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 6)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(8):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 2)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 3)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 4)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 5)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 6)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 7)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(9):
                if(sprintf(retbuf, msg,
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 0)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 1)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 2)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 3)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 4)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 5)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 6)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 7)),
                           context->ObjectToStringValue(context->ArrayAt(ins_string, 8)))
                   != msg_length)
                    strcpy(retbuf, error_insertions);
                break;
            case(10):
                strcpy(retbuf, error_insertions); /* error case */
                break;
            default:
                strcpy(retbuf, msg);
                break;
        }

        catclose(catalog);                 /* close the catalog          */
        return context->NewStringFromAsciiz(retbuf);
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

RexxFunction3(RexxObjectPtr, SysGetMessage,
              OPTIONAL_CSTRING, filename,
              RexxUnsignedNumber, msgnum,
              ARGLIST, ins_string)
{
    return SysGetMessageX_impl(context, filename, 1, msgnum, ins_string);
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

RexxFunction1(RexxObjectPtr, SysGetKey, OPTIONAL_CSTRING, strecho)
{
    static RXSEMDATA *semdata = NULL;;
    int val, rc;
    bool      echo = true;             /* Set to false if we         */
    char retbuf[256];

    /* create the mutex semaphore, if necessary */
    if (semdata == NULL) {
        semdata = (RXSEMDATA *)malloc(sizeof(RXSEMDATA));
        /* this is an unnamed semaphore */
        semdata->handle = (sem_t *)malloc(sizeof(sem_t));
        rc = sem_init(semdata->handle, 0, 0);
        if (rc == -1) {
            free(semdata);
            return context->NullString();
        }
        semdata->named = false;
        rc = sem_post(semdata->handle);
    }
    /* request the semaphore */
    rc = sem_wait(semdata->handle);
    /* get the key */
    if (strcasecmp(strecho,"NOECHO") == 0)
        echo = false;
    else if (strcasecmp(strecho, "ECHO")) {
        context->InvalidRoutine();
        return context->NullString();
    }
    getkey(retbuf,echo);       /* call the complicated part  */
    /* release the semaphore */
    rc = sem_getvalue(semdata->handle, &val);
    if (val == 0) {
        rc = sem_post(semdata->handle);
        if (rc) {
            return context->NullString();
        }
    }

    return context->NewStringFromAsciiz(retbuf);
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

RexxFunction0(int, SysFork)
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

RexxFunction0(int, SysWait)
{
    int iStatus;

    wait( &iStatus );
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

RexxFunction1(RexxObjectPtr, SysCreatePipe, CSTRING, opt)
{
    int  iStatus;
    int  iaH[2];
    char cBlocking = 0;
    char retbuf[256];

    if (opt == NULL )                  /* No arg?                    */
        cBlocking = 1;                 /* Default is blocking        */
    /* One arg, first char is 'b'? */
    else if (*opt == 'b' || *opt == 'B')
        cBlocking = 1;                 /* Wants blocking             */
    /* One arg, first char is 'n'? */
    else if (*opt == 'n' || *opt == 'N')
        cBlocking = 0;                 /* Wants non-blocking         */

    if (pipe(iaH)) {                   /* Create the pipe            */
        strcpy(retbuf, "*** ERROR: Creating pipe");
        return context->NewStringFromAsciiz(retbuf);
    }
    if (!cBlocking) {                  /* Non-blocking?              */
        /* Get file status flags ---------- */
        iStatus = fcntl(iaH[0], F_GETFL, NULL);
        iStatus |= O_NONBLOCK;         /* Turn on NONBLOCK flag      */
        /* Does set work? ----------------- */
        if (fcntl(iaH[0], F_SETFL, iStatus) == -1) {
            strcpy(retbuf, "*** ERROR: Setting NONBLOCK flag");
            return context->NewStringFromAsciiz(retbuf);
        }
    }

    sprintf(retbuf, "%d %d", iaH[0], iaH[1]);/* Create return string */
    return context->NewStringFromAsciiz(retbuf);
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

RexxFunction2(RexxObjectPtr, SysGetFileDateTime, CSTRING, filename, OPTIONAL_CSTRING, selector)
{
    long      rc;                      /* Ret code                   */
    struct    stat buf;
    struct    tm *newtime;
    char     *dir_buf = NULL;          /* full directory path        */
    bool      fOk = true;
    bool      alloc_Flag = false;
    char      retbuf[256];

    if(*filename == '~') {
        dir_buf = resolve_tilde(filename);
        alloc_Flag = true;
    }
    else {
        dir_buf = filename;
    }

    if (stat(dir_buf, &buf) < 0) {
        fOk = false;
    }

    if(fOk) {
        if (strlen(selector)) {
            switch (*selector) {
                case 'a':
                case 'A':
                    newtime = localtime(&(buf.st_atime));
                    break;
                case 'w':
                case 'W':
                    newtime = localtime(&(buf.st_mtime));
                    break;
                default:
                    context->InvalidRoutine();   /* raise error condition      */
                    return context->NullString();
            }
        }
        else
            newtime = localtime(&(buf.st_mtime));

        newtime->tm_year += 1900;
        newtime->tm_mon += 1;

        snprintf(retbuf, sizeof(retbuf), "%4d-%02d-%02d %02d:%02d:%02d",
                newtime->tm_year, newtime->tm_mon, newtime->tm_mday,
                newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
    }
    if( (dir_buf) && (alloc_Flag == true) )
        free(dir_buf);                         /* free the buffer memory  */
    if (!fOk)
        strcpy(retbuf, "-1");
    return context->NewStringFromAsciiz(retbuf);
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

RexxFunction3(int, SysSetFileDateTime, CSTRING, filename, OPTIONAL_CSTRING, newdate, OPTIONAL_CSTRING, newtime)
{
    long      rc;                      /* Ret code                   */
    bool      fOk = true;
    bool      fCloseFile = false;
    struct utimbuf timebuf;
    struct tm tmptime;
    time_t ltime;
    struct stat buf;
    char  *dir_buf = NULL;
    bool      alloc_Flag = false;

    /* we expect one to three parameters, if three parameters are      */
    /* specified then the second may be omitted to set only a new time,*/
    /* if only one is specified then the file is set to current time   */


    if(*(filename) == '~') {
        dir_buf = resolve_tilde(filename);
        alloc_Flag = true;
    }
    else {
        dir_buf = filename;
    }

    if (stat(dir_buf, &buf) < 0) {
        fOk =  false;
    }

    if (strlen(newdate) == 0 && strlen(newtime) == 0) {
        time(&ltime);
        timebuf.modtime = ltime;
        if (utime(dir_buf, &timebuf) < 0) {
            fOk = false;
        }
    }
    else {
        newtime = ctime(&(buf.st_mtime));
        if (strlen(newdate) > 0) {
            /* parse new date */
            if (sscanf(newdate, "%4d-%2d-%2d", &tmptime.tm_year,
                       &tmptime.tm_mon, &tmptime.tm_mday) != 3)
                fOk = false;
            tmptime.tm_year -= 1900;
            tmptime.tm_mon -= 1;
        }
        if (strlen(newtime) > 0) {
            /* parse new time */
            if (sscanf(newtime, "%2d:%2d:%2d", &tmptime.tm_hour,
                       &tmptime.tm_min, &tmptime.tm_sec) != 3)
                fOk = false;
        }
        ltime = mktime(&tmptime);
        timebuf.modtime = ltime;
        if (utime(dir_buf, &timebuf) < 0) {
            fOk = false;
        }
    }

    if( (dir_buf) && (alloc_Flag == true) )
        free(dir_buf);                 /* free the buffer memory     */
    if (!fOk)
        return -1;
    return 0;
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

RexxFunction1(RexxObjectPtr, SysQueryProcess, OPTIONAL_CSTRING, option)
{
    unsigned int uiUsedCPUTime  = 0;
    unsigned int uiUsedCPUmsec  = 0;
    unsigned int uiUsedHours    = 0;
    unsigned int uiUsedMinutes  = 0;
    unsigned int uiUsedSeconds  = 0;
    char timebuf[40];
    int iRc = 0;
    struct rusage struResUse;
    char errbuf[256];

    if (!strcasecmp(option, "PID")) {
        return context->NumberToObject((RexxNumber)(getpid()));
    }
    else if (!strcasecmp(option, "PPID")) {
        return context->NumberToObject((RexxNumber)(getppid()));
    }
    else if (!strcasecmp(option, "PGID")) {
        return context->NumberToObject((RexxNumber)(getpgid(getppid())));
    }
    else if (!strcasecmp(option, "PPRIO")) {
        return context->NumberToObject((RexxNumber)(getpriority(PRIO_PROCESS, 0)));
    }

    /* --------------------------------------------------------------- */
    /* Get process usage data and keep calls together at the end of    */
    /* the function SysQueryProcess.                                   */
    iRc = getrusage ( RUSAGE_SELF, &struResUse);
    if ( iRc ) {
        snprintf(errbuf, sizeof(errbuf), " System error; errno = %d", errno);
        return context->NewStringFromAsciiz(errbuf);
    }
    if (!strcasecmp(option, "PTIME")) {/* Calculate the used CPU time*/
        uiUsedCPUmsec  = (unsigned int) struResUse.ru_utime.tv_usec/1000;
        uiUsedCPUmsec += (unsigned int) struResUse.ru_stime.tv_usec/1000;
        if (uiUsedCPUmsec >= 1000 ) {
            uiUsedCPUTime = uiUsedCPUmsec / 1000;
            uiUsedCPUmsec = uiUsedCPUmsec % 1000;
        }
        uiUsedCPUTime += (unsigned int) struResUse.ru_utime.tv_sec;
        uiUsedCPUTime += (unsigned int) struResUse.ru_stime.tv_sec;
        uiUsedHours   = uiUsedCPUTime / 3600;
        uiUsedMinutes = uiUsedCPUTime / 60;
        if (uiUsedMinutes >= 60 )
            uiUsedMinutes = uiUsedMinutes % 60;
        if (uiUsedCPUTime >= 60 )
            uiUsedSeconds = uiUsedCPUTime % 60;
        else
            uiUsedSeconds = uiUsedCPUTime;

        snprintf(errbuf, sizeof(errbuf), "CPU_Time Summary: %2d:%.2d:%.2d:%.3d  Kernel:",
                 uiUsedHours, uiUsedMinutes, uiUsedSeconds, uiUsedCPUmsec );

        uiUsedCPUmsec = (unsigned int) struResUse.ru_stime.tv_usec/1000;
        uiUsedCPUTime = (unsigned int) struResUse.ru_stime.tv_sec;
        uiUsedHours   = uiUsedCPUTime / 3600;
        uiUsedMinutes = uiUsedCPUTime / 60;
        if (uiUsedMinutes >= 60 )
            uiUsedMinutes = uiUsedMinutes % 60;
        if (uiUsedCPUTime >= 60 )
            uiUsedSeconds = uiUsedCPUTime % 60;
        else
            uiUsedSeconds = uiUsedCPUTime;

        snprintf(timebuf, sizeof(timebuf), " %2d:%.2d:%.2d:%.3d  User:",
                 uiUsedHours, uiUsedMinutes, uiUsedSeconds, uiUsedCPUmsec );
        strcat(errbuf, timebuf);

        uiUsedCPUmsec = (unsigned int) struResUse.ru_utime.tv_usec/1000;
        uiUsedCPUTime = (unsigned int) struResUse.ru_utime.tv_sec;
        uiUsedHours   = uiUsedCPUTime / 3600;
        uiUsedMinutes = uiUsedCPUTime / 60;
        if (uiUsedMinutes >= 60 )
            uiUsedMinutes = uiUsedMinutes % 60;
        if (uiUsedCPUTime >= 60 )
            uiUsedSeconds = uiUsedCPUTime % 60;
        else
            uiUsedSeconds = uiUsedCPUTime;

        snprintf(timebuf, sizeof(timebuf), " %2d:%.2d:%.2d:%.3d", uiUsedHours,
                 uiUsedMinutes, uiUsedSeconds, uiUsedCPUmsec );
        strcat(errbuf, timebuf);

        return context->NewStringFromAsciiz(errbuf);
    }
    else if (!strcasecmp(option, "PMEM")) { /* Show max memory RSS used */
        snprintf(errbuf, sizeof(errbuf), "Max_Memory_RSS: %ld",
                 struResUse.ru_maxrss);
        return context->NewStringFromAsciiz(errbuf);
    }
    else if (!strcasecmp(option, "PSWAPS")) {/* Memory has been swapped */
        snprintf(errbuf, sizeof(errbuf), "Memory_swaps: %ld",
                 struResUse.ru_nswap);
        return context->NewStringFromAsciiz(errbuf);
    }
    else if (!strcasecmp(option, "PRCVDSIG")) {/* Process received signals*/
        snprintf(errbuf, sizeof(errbuf), "Received_signals: %ld",
                 struResUse.ru_nsignals);
        return context->NewStringFromAsciiz(errbuf);
    }

    context->InvalidRoutine();
    return context->NullString();
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

RexxFunction1(RexxStringObject, SysGetErrortext, int, errnum)
{
    char  *errmsg = NULL;

    errmsg = strerror( errnum );
    if (errmsg == NULL )
        return context->NullString();
    return context->NewStringFromAsciiz(errmsg);
}


/*************************************************************************/
/*************************************************************************/
/******************** Linux Only Functions *******************************/
/*************************************************************************/
/*************************************************************************/


#ifdef LINUX
/*************************************************************************
* Function:  SysLinVer                                                   *
*                                                                        *
* Syntax:    call SysLinVer                                              *
*                                                                        *
* Return:    Linux Version                                               *
*************************************************************************/

RexxFunction0(RexxObjectPtr, SysLinVer)
{
    struct utsname info;               /* info structur              */
    char retbuf[256];

    if(uname(&info) < 0) {             /* if no info stored          */
        context->InvalidRoutine();
        return context->NullString();
    }

    sprintf(retbuf, "%s %s",info.sysname,info.release);
    return context->NewStringFromAsciiz(retbuf);
}
#endif


/*************************************************************************/
/*************************************************************************/
/******************** AIX Only Functions *********************************/
/*************************************************************************/
/*************************************************************************/


#ifdef AIX
/*************************************************************************
* Function:  SysAddFuncPkg                                               *
*                                                                        *
* Description:   Function to enable load of old CREXX libraries for AIX. *
*                The Path must be fully qualified in REXX procedure,     *
*                it is used to select the old >load< function call.      *
*                                                                        *
* Syntax:    call SysAddFuncPkg ( /Fully_qualified_path/Package_name )   *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    null string                                                 *
*************************************************************************/

RexxFunction0(CSTRING, SysAddPkg, CSTRING, argstring)
{
    long   rc = NULL;
    int    j;                          /* Counter                    */
    long   arglength;                  /* length of the count        */
    RXFUNCBLOCK *funcblock;            /* Base for function blocks   */
    PRXINITFUNCPKG InitFunc;           /* Pointer returned from load */


    arglength = strlen(argstr);     /* get length of string       */
    if ( (arglength == 0 ) ||          /* if null string             */
     (arglength >= MAXNAME) ||         /* or too long                */
     (!strchr( argstring, '/' )) ) {
        context->InvalidRoutine();
        return context->NullString;
    }

    /*  loadAndInit? for load  */
    if (!(InitFunc = (PRXINITFUNCPKG)load(argstring,0,NULL))) {
        if ( InitFunc == NULL ) {
            fprintf(stderr, " *** Unable to load library %s !\nError message: errno = %d;",\
                    argstring, errno);
            perror(" REXXUTIL");
            rc = 1;
        }
    }
    if ( rc == NULL ) {
        /* Call the initialization routine for the library (which should */
        /* be the function pointer returned to us by load).              */
        rc = (*InitFunc)(&funcblock);
        if (rc) {                      /* If routine indicates error,*/
                                       /* tell the user.             */
            fprintf(stderr,"*** Library load routine gave error %d.\n",rc);
            return(rc);                /* don't load anything        */
        }

        /* Now run through the array of func blocks, adding them to the  */
        /* list of external functions or subcommand handlers.  Note that */
        /* we use the external function types in all cases, but since    */
        /* the only thing affected is the function pointers, there's no  */
        /* problem - the RXFUNCBLOCK and RXSUBCOMBLOCK types are         */
        /* otherwise identical.                                          */
        for (j=0; funcblock[j].name != NULL; j++) {
#ifdef ORXLD_DEBUG
            fprintf(stderr,"REXXUTIL: PKGFunction %s \n", funcblock[j].name);
#endif
            if ( funcblock[j].function && (rc == NULL) ) {
                RexxRegisterFunctionDll(funcblock[j].name,
                                        argstring, funcblock[j].name);
            }
        }
    }
    return context->NullString;
}


/*************************************************************************
* Function:  SysAddCmdPkg    like SysAddFuncPkg                          *
*                                                                        *
*************************************************************************/

RexxFunction0(CSTRING, SysAddCmdPkg)
{
    long lRc;

    lRc = SysAddFuncPkg(name,          /* Function name              */
                        numargs,       /* Number of arguments        */
                        args,          /* Argument array             */
                        queuename,     /* Current queue              */
                        retstr );      /* Return RXSTRING            */

    return context->NullString;
}


/*************************************************************************
* Function:  SysDropFuncPkg                                              *
*                                                                        *
* Description:   Function to enable load of old CREXX libraries for AIX. *
*                It is used for the old >load< function call for the     *
*                list of function to be dropped from REXX.               *
*                The Path must not be fully qualified in REXX procedure. *
*                                                                        *
* Syntax:    call SysDropFuncPkg                                         *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/

RexxFunction1(CSTRING, SysDropFuncPkg, CSTRING, pkgstr)
{
    long   rc = NULL;
    int    j;                          /* Counter                    */
    long   arglength;                  /* length of the count        */
    char  *argstring;                  /* input sleep time           */
    RXFUNCBLOCK *funcblock;            /* Base for function blocks   */
    PRXINITFUNCPKG InitFunc;           /* Pointer returned from load */

    argstring = pkgstr;                /* point to the string        */
    arglength = strlen(pkgstr);        /* get length of string       */
    if ( (arglength == 0 ) ||          /* if null string             */
     (arglength >= MAXNAME) ) {        /* or too long                */
        context->InvalidRoutine();
        return context->NullString;
    }

    /* loadAndInit? for load */
    if (!(InitFunc = (PRXINITFUNCPKG)load(argstring,0,NULL))) {
        if ( InitFunc == NULL ) {
            fprintf(stderr, " *** Unable to drop library %s !\nError message: errno = %d;",\
                    argstring, errno);
            perror(" REXXUTIL");
            rc = 1;
        }
    }
    if ( rc == NULL ) {
        /* Call the initialization routine for the library (which should */
        /* be the function pointer returned to us by load).              */
        rc = (*InitFunc)(&funcblock);
        if (rc) {                      /* If routine indicates error,*/
                                       /* tell the user.             */
            fprintf(stderr," *** Library drop routine gave error %d.\n",rc);
            return context->NullString;           /* don't load anything */
        }

        /* Now run through the array of func blocks, adding them to the  */
        /* list of external functions or subcommand handlers.  Note that */
        /* we use the external function types in all cases, but since    */
        /* the only thing affected is the function pointers, there's no  */
        /* problem - the RXFUNCBLOCK and RXSUBCOMBLOCK types are         */
        /* otherwise identical.                                          */
        for (j=0; funcblock[j].name != NULL; j++) {
#ifdef ORXLD_DEBUG
            fprintf(stderr,"REXXUTIL: PKGFunction %s \n", funcblock[j].name);
#endif
            if ( funcblock[j].function && (rc == NULL) ) {
                RexxDeregisterFunction( funcblock[j].name );
            }
        }
    }
    return context->NullString;
}


/*************************************************************************
* Function:  SysDropCmdPkg    like SysDropFuncPkg                        *
*                                                                        *
*************************************************************************/

RexxFunction0(CSTRING, SysDropCmdPkg)
{
    long lRc;

    lRc = SysDropFuncPkg(name,         /* Function name              */
                         numargs,      /* Number of arguments        */
                         args,         /* Argument array             */
                         queuename,    /* Current queue              */
                         retstr );     /* Return RXSTRING            */

    return context->NullString;
}


/*************************************************************************
* Function:  SysGetpid                                                   *
*                                                                        *
* Description:   Function to migrate CREXX for AIX procedures.           *
*                                                                        *
* Syntax:    call SysGetpid()                                            *
*                                                                        *
* Return:    Process_ID                                                  *
*************************************************************************/

RexxFunction0(pid_t, SysGetPid)
{
    return getpid();
}

/*************************************************************************
* Function:  SysIsFile                                                *
*                                                                        *
* Syntax:    call SysIsFile file                                      *
*                                                                        *
* Params:    file - file to check existance of.                          *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxFunction1(int, SysIsFile, CSTRING, filename)
{
  struct stat finfo;                   /* return buf for the finfo   */

  stat(filename, &finfo);              /* read the info about it     */

  if(S_ISREG(finfo->st_mode) ||        /* if it is a file            */
     S_ISBLK(finfo->st_mode) {         /* file                       */
    return 1;                          /* True - Is a File           */
  }
  else {
    return 0;                          /* False - Is something else  */
  }
}

/*************************************************************************
* Function:  SysIsFileDirectory                                                 *
*                                                                        *
* Syntax:    call SysIsFileDirectory dir                                        *
*                                                                        *
* Params:    dir - dir to check existance of.                            *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxFunction1(int, SysIsFileDirectory, CSTRING, filename)
{
  struct stat finfo;                   /* return buf for the finfo   */

  stat(filename, &finfo);              /* read the info about it     */

  if(S_ISDIR(finfo->st_mode) {         /* is a subdir?               */
    return 1;                          /* True - Is a File           */
  }
  else {
    return 0;                          /* False - Is something else  */
  }
}

/*************************************************************************
* Function:  SysIsFileLink                                               *
*                                                                        *
* Syntax:    call SysIsFileLink file                                     *
*                                                                        *
* Params:    file - file to check if it is a Link (Alias).               *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxFunction1(int, SysIsFileLink, CSTRING, filename)
{
  struct stat finfo;                   /* return buf for the finfo   */

  lstat(args[0].strptr, &finfo);       /* read the info about it     */

  if(S_ISLNK(finfo->st_mode) {         /* or a symbolic link         */
    return 1;                          /* True - Is a File           */
  }
  else {
    return 0;                          /* False - Is something else  */
  }
}



#endif                                 /* CREXX funcs                */

