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
/*********************************************************************\
*
* Module Name: REXX.H
*
* ooRexx Common Definitions File
*
\*********************************************************************/

#ifndef REXXSAA_INCLUDED
#define REXXSAA_INCLUDED

#include "rexxapitypes.h"

typedef unsigned long APIRET;             // API return types

/*********************************************************************/
/*                                                                   */
/*                            Common                                 */
/*                                                                   */
/*********************************************************************/

/* This section defines return codes and constants for REXX calls    */

#include "rexxapidefs.h"

/***    Structure for external interface string (RXSTRING) */

typedef struct _RXSTRING {             /* rxstr */
        unsigned long  strlength;      /*   length of string         */
        char *    strptr;              /*   pointer to string        */
} RXSTRING;

/***    Macros for RexxString manipulation                   */

#define RXNULLSTRING(r)      ((r).strptr == 0)
#define RXZEROLENSTRING(r)   ((r).strptr != 0 && (r).strlength == 0)
#define RXVALIDSTRING(r)     ((r).strptr != 0 && (r).strlength == 0)
#define RXSTRLEN(r)          (RXNULLSTRING(r) ? 0 : (r).strlength)
#define RXSTRPTR(r)          )(r).strptr)
#define MAKERXSTRING(r,p,l)  { (r).strptr = (char *)p; (r).strlength=(unsigned long)l; }


typedef RXSTRING     *PRXSTRING;       /* pointer to a RXSTRING      */

/***    Structure for system exit block (RXSYSEXIT) 32-bit */

typedef struct _RXSYSEXIT {            /* syse */
   char *   sysexit_name;              /* subcom enviro for sysexit  */
   long  sysexit_code;                 /* sysexit function code      */
}  RXSYSEXIT;


typedef RXSYSEXIT *PRXSYSEXIT;         /* pointer to a RXSYSEXIT     */


/***    Shared Variable Pool Interface defines */

/***    Structure of Shared Variable Request Block (SHVBLOCK) */

typedef struct _SHVBLOCK {            /* shvb */
    struct _SHVBLOCK  *shvnext;       /* pointer to the next block   */
    RXSTRING           shvname;       /* Pointer to the name buffer  */
    RXSTRING           shvvalue;      /* Pointer to the value buffer */
    unsigned long      shvnamelen;    /* Length of the name value    */
    unsigned long      shvvaluelen;   /* Length of the fetch value   */
    unsigned char      shvcode;       /* Function code for this block*/
    unsigned char      shvret;        /* Individual Return Code Flags*/
}   SHVBLOCK;

typedef SHVBLOCK *PSHVBLOCK;

typedef char *PEXIT;                  /* ptr to exit parameter block */


/*********************************************************************/
/*                                                                   */
/*                            32-bit                                 */
/*                                                                   */
/*********************************************************************/

/***    Main Entry Point to the REXX Interpreter */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
APIRET APIENTRY RexxStart (
         long ,                        /* Num of args passed to rexx */
         PRXSTRING,                    /* Array of args passed to rex */
         const char *,                 /* [d:][path] filename[.ext]  */
         PRXSTRING,                    /* Loc of rexx proc in memory */
         const char *,                 /* ASCIIZ initial environment.*/
         long ,                        /* type (command,subrtn,funct) */
         PRXSYSEXIT,                   /* SysExit env. names &  codes */
         short *,                      /* Ret code from if numeric   */
         PRXSTRING );                  /* Retvalue from the rexx proc */

typedef APIRET (APIENTRY *PFNREXXSTART)(long, PRXSTRING, const char *, PRXSTRING,
                                        const char *, long, PRXSYSEXIT, short *,
                                        PRXSTRING);
#else
APIRET APIENTRY RexxStart (
         long,                         /* Num of args passed to rexx */
         PRXSTRING,                    /* Array of args passed to rex */
         char *,                       /* [d:][path] filename[.ext]  */
         PRXSTRING,                    /* Loc of rexx proc in memory */
         char *,                       /* ASCIIZ initial environment.*/
         long ,                        /* type (command,subrtn,funct) */
         PRXSYSEXIT,                   /* SysExit env. names &  codes */
         short *,                      /* Ret code from if numeric   */
         PRXSTRING );                  /* Retvalue from the rexx proc */

typedef APIRET (APIENTRY *PFNREXXSTART)(long, PRXSTRING, char *, PRXSTRING,
                                        char *, long, PRXSYSEXIT, short *,
                                        PRXSTRING);
#endif


/***   Uppercase Entry Point Name */
#define REXXSTART   RexxStart


/***    Subcommand Interface */

/* This typedef simplifies coding of a Subcommand handler.           */
typedef APIRET APIENTRY RexxSubcomHandler(PRXSTRING,
                                unsigned short *,
                                PRXSTRING);

/***   RexxRegisterSubcomDll -- Register a DLL entry point           */
/***   as a Subcommand handler */

#ifdef __cplusplus
APIRET APIENTRY RexxRegisterSubcomDll (
         const char *,                         /* Name of subcom handler     */
         const char *,                         /* Name of DLL                */
         const char *,                         /* Name of procedure in DLL   */
         char *,                               /* User area                  */
         unsigned long  );                     /* Drop authority.            */

typedef APIRET (APIENTRY *PFNREXXREGISTERSUBCOMDLL)(const char *, const char *, const char *,
                                                    char *, unsigned long);
#else
APIRET APIENTRY RexxRegisterSubcomDll (
         char *,                          /* Name of subcom handler     */
         char *,                          /* Name of DLL                */
         char *,                          /* Name of procedure in DLL   */
         char *,                          /* User area                  */
         unsigned long);                  /* Drop authority.            */

typedef APIRET (APIENTRY *PFNREXXREGISTERSUBCOMDLL)(char *, char *, char *,
                                                    char *, unsigned long);
#endif


/***   Uppercase Entry Point Name */
#define REXXREGISTERSUBCOMDLL  RexxRegisterSubcomDll


/***   RexxRegisterSubcomExe -- Register an EXE entry point          */
/***   as a Subcommand handler */

#ifdef __cplusplus
APIRET APIENTRY RexxRegisterSubcomExe (
         const char *,                 /* Name of subcom handler     */
         PFN,                          /* address of handler in EXE  */
         char *);                      /* User area                  */

typedef APIRET (APIENTRY *PFNREXXREGISTERSUBCOMEXE)(const char *, PFN, char *);

#else
APIRET APIENTRY RexxRegisterSubcomExe (
         char *,                          /* Name of subcom handler     */
         PFN,                          /* address of handler in EXE  */
         char *);                      /* User area                  */


typedef APIRET (APIENTRY *PFNREXXREGISTERSUBCOMEXE)(char *, PFN, char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXREGISTERSUBCOMEXE  RexxRegisterSubcomExe


/***    RexxQuerySubcom - Query an environment for Existance */

#ifdef __cplusplus
APIRET APIENTRY RexxQuerySubcom(
         const char *,                 /* Name of the Environment    */
         const char *,                 /* DLL Module Name            */
         unsigned short *,             /* Stor for existance code    */
         char *);                      /* Stor for user word         */

typedef APIRET (APIENTRY *PFNREXXQUERYSUBCOM)(const char *, const char *, unsigned short *,
                                              char *);

#else
APIRET APIENTRY RexxQuerySubcom(
         char *,                          /* Name of the Environment    */
         char *,                          /* DLL Module Name            */
         unsigned short *,                /* Stor for existance code    */
         char * );                        /* Stor for user word         */

typedef APIRET (APIENTRY *PFNREXXQUERYSUBCOM)(char *, char *, unsigned short *,
                                              char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXQUERYSUBCOM  RexxQuerySubcom


/***    RexxDeregisterSubcom - Drop registration of a Subcommand     */
/***    environment */

#ifdef __cplusplus
APIRET APIENTRY RexxDeregisterSubcom(
         const char *,                         /* Name of the Environment    */
         const char * );                       /* DLL Module Name            */

typedef APIRET (APIENTRY *PFNREXXDEREGISTERSUBCOM)(const char *, const char *);

#else
APIRET APIENTRY RexxDeregisterSubcom(
         char *,                          /* Name of the Environment    */
         char * );                        /* DLL Module Name            */

typedef APIRET (APIENTRY *PFNREXXDEREGISTERSUBCOM)(char *, char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXDEREGISTERSUBCOM  RexxDeregisterSubcom



/***    Shared Variable Pool Interface */

/***    RexxVariablePool - Request Variable Pool Service */

APIRET APIENTRY RexxVariablePool(
         PSHVBLOCK);                  /* Pointer to list of SHVBLOCKs*/

typedef APIRET (APIENTRY *PFNREXXVARIABLEPOOL)(PSHVBLOCK);

/***   Uppercase Entry Point Name */
#define REXXVARIABLEPOOL  RexxVariablePool


/***    External Function Interface */
/* This typedef simplifies coding of an External Function.           */

typedef unsigned long APIENTRY RexxFunctionHandler(char *,
                                  unsigned long,
                                  PRXSTRING,
                                  char *,
                                  PRXSTRING);

/***    RexxRegisterFunctionDll - Register a function in the AFT */

#ifdef __cplusplus
APIRET APIENTRY RexxRegisterFunctionDll (
        const char *,                          /* Name of function to add    */
        const char *,                          /* Dll file name (if in dll)  */
        const char *);                         /* Entry in dll               */

typedef APIRET (APIENTRY *PFNREXXREGISTERFUNCTIONDLL)(const char *, const char *, const char *);

#else
APIRET APIENTRY RexxRegisterFunctionDll (
        char *,                           /* Name of function to add    */
        char *,                           /* Dll file name (if in dll)  */
        char *);                          /* Entry in dll               */

typedef APIRET (APIENTRY *PFNREXXREGISTERFUNCTIONDLL)(char *, char *, char *);
#endif


/***   Uppercase Entry Point Name */
#define REXXREGISTERFUNCTIONDLL  RexxRegisterFunctionDll


/***    RexxRegisterFunctionExe - Register a function in the AFT */

#ifdef __cplusplus
APIRET APIENTRY RexxRegisterFunctionExe (
        const char *,                  /* Name of function to add    */
        PFN);                          /* Entry point in EXE         */

typedef APIRET (APIENTRY *PFNREXXREGISTERFUNCTIONEXE)(const char *, PFN);

#else
APIRET APIENTRY RexxRegisterFunctionExe (
        char *,                        /* Name of function to add    */
        PFN);                          /* Entry point in EXE         */

typedef APIRET (APIENTRY *PFNREXXREGISTERFUNCTIONEXE)(char *, PFN);
#endif


/***   Uppercase Entry Point Name */
#define REXXREGISTERFUNCTIONEXE  RexxRegisterFunctionExe


/***    RexxDeregisterFunction - Delete a function from the AFT */

#ifdef __cplusplus
APIRET APIENTRY RexxDeregisterFunction (
        const char * );                         /* Name of function to remove */

typedef APIRET (APIENTRY *PFNREXXDEREGISTERFUNCTION)(const char *);

#else
APIRET APIENTRY RexxDeregisterFunction (
        char * );                         /* Name of function to remove */

typedef APIRET (APIENTRY *PFNREXXDEREGISTERFUNCTION)(char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXDEREGISTERFUNCTION  RexxDeregisterFunction


/***    RexxQueryFunction - Scan the AFT for a function */

#ifdef __cplusplus
APIRET APIENTRY RexxQueryFunction (
        const char * );                         /* Name of function to find   */

typedef APIRET (APIENTRY *PFNREXXQUERYFUNCTION)(const char *);

#else
APIRET APIENTRY RexxQueryFunction (
        char * );                         /* Name of function to find   */

typedef APIRET (APIENTRY *PFNREXXQUERYFUNCTION)(char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXQUERYFUNCTION  RexxQueryFunction


/***   System Exits */

/***    Subfunction RXFNCCAL - External Function Calls */

typedef  struct _RXFNC_FLAGS {          /* fl */
   unsigned rxfferr  : 1;              /* Invalid call to routine.   */
   unsigned rxffnfnd : 1;              /* Function not found.        */
   unsigned rxffsub  : 1;              /* Called as a subroutine     */
}  RXFNC_FLAGS ;

typedef  struct _RXFNCCAL_PARM {        /* fnc */
   RXFNC_FLAGS       rxfnc_flags ;     /* function flags             */
   char *            rxfnc_name;       /* Pointer to function name.  */
   unsigned short    rxfnc_namel;      /* Length of function name.   */
   char *            rxfnc_que;        /* Current queue name.        */
   unsigned short    rxfnc_quel;       /* Length of queue name.      */
   unsigned short    rxfnc_argc;       /* Number of args in list.    */
   PRXSTRING         rxfnc_argv;       /* Pointer to argument list.  */
   RXSTRING          rxfnc_retc;       /* Return value.              */
}  RXFNCCAL_PARM;


/***    Subfunction RXCMDHST -- Process Host Commands     */

typedef  struct _RXCMD_FLAGS {          /* fl */
   unsigned rxfcfail : 1;              /* Command failed.            */
   unsigned rxfcerr  : 1;              /* Command ERROR occurred.    */
}  RXCMD_FLAGS;

typedef  struct _RXCMDHST_PARM {        /* rx */
   RXCMD_FLAGS       rxcmd_flags;      /* error/failure flags        */
   char *            rxcmd_address;    /* Pointer to address name.   */
   unsigned short    rxcmd_addressl;   /* Length of address name.    */
   char *            rxcmd_dll;        /* dll name for command.      */
   unsigned short    rxcmd_dll_len;    /* Length of dll name.        */
   RXSTRING          rxcmd_command;    /* The command string.        */
   RXSTRING          rxcmd_retc;       /* Pointer to return buffer   */
}  RXCMDHST_PARM;


/***     Subfunction RXMSQPLL -- Pull Entry from Queue */

typedef struct _RXMSQPLL_PARM {        /* pll */
   RXSTRING          rxmsq_retc;       /* Pointer to dequeued entry  */
                                       /* buffer.  User allocated.   */
} RXMSQPLL_PARM;


/***    Subfunction RXMSQPSH -- Push Entry on Queue */
typedef  struct _RXMSQ_FLAGS {          /* fl */
   unsigned rxfmlifo : 1;              /* Stack entry LIFO if set    */
}  RXMSQ_FLAGS;

typedef  struct _RXMSQPSH_PARM {        /* psh */
   RXMSQ_FLAGS       rxmsq_flags;      /* LIFO/FIFO flag             */
   RXSTRING          rxmsq_value;      /* The entry to be pushed.    */
}  RXMSQPSH_PARM;


/***    Subfunction RXMSQSIZ -- Return the Current Queue Size */

typedef struct _RXMSQSIZ_PARM {        /* siz */
   unsigned long     rxmsq_size;       /* Number of Lines in Queue   */
}  RXMSQSIZ_PARM;


/***    Subfunction RXMSQNAM -- Set Current Queue Name */

typedef struct _RXMSQNAM_PARM {        /* nam */
   RXSTRING          rxmsq_name;       /* RXSTRING containing        */
                                       /* queue name.                */
}  RXMSQNAM_PARM;


/***    Subfunction RXSIOSAY -- Perform SAY Clause */

typedef struct _RXSIOSAY_PARM {        /* say */
   RXSTRING          rxsio_string;     /* String to display.         */
}  RXSIOSAY_PARM;


/***    Subfunction RXSIOTRC -- Write Trace Output */

typedef struct _RXSIOTRC_PARM { /* trcparm */
   RXSTRING          rxsio_string;     /* Trace line to display.     */
}  RXSIOTRC_PARM;


/***    Subfunction RXSIOTRD -- Read Input from the Terminal */

typedef struct _RXSIOTRD_PARM {        /* trd */
   RXSTRING          rxsiotrd_retc;    /* RXSTRING for output.       */
}  RXSIOTRD_PARM;


/***    Subfunction RXSIODTR -- Read Debug Input from the Terminal */

typedef struct _RXSIODTR_PARM {        /* dtr */
   RXSTRING          rxsiodtr_retc;    /* RXSTRING for output.       */
}  RXSIODTR_PARM;


/***    Subfunction RXHSTTST -- Test for HALT Condition */

typedef struct _RXHLT_FLAGS {          /* fl Halt flag               */
   unsigned rxfhhalt : 1;              /* Set if HALT occurred.      */
}  RXHLT_FLAGS;

typedef struct _RXHLTTST_PARM {        /* tst */
   RXHLT_FLAGS rxhlt_flags;            /* Set if HALT occurred       */
}  RXHLTTST_PARM;


/***    Subfunction RXTRCTST -- Test for TRACE Condition */

typedef struct _RXTRC_FLAGS {          /* fl Trace flags             */
   unsigned rxftrace : 1;              /* Set to run external trace. */
}  RXTRC_FLAGS;

typedef struct _RXTRCTST_PARM {        /* tst */
   RXTRC_FLAGS rxtrc_flags;            /* Set to run external trace  */
}  RXTRCTST_PARM;

typedef struct _RXDBG_FLAGS {          /* fl Trace flags             */
   unsigned rxftrace;                  /* Set to run external trace. */
}  RXDBG_FLAGS;

typedef struct _RXDBGTST_PARM {        /* tst */
   RXDBG_FLAGS rxdbg_flags;            /* Set to run external trace before */
   unsigned long rxdbg_line;
   RXSTRING    rxdbg_filename;
   RXSTRING    rxdbg_routine;
}  RXDBGTST_PARM;

/* This typedef simplifies coding of an Exit handler.                */
typedef long APIENTRY RexxExitHandler(long, long, PEXIT);

/***      RexxRegisterExitDll - Register a system exit. */

#ifdef __cplusplus
APIRET APIENTRY RexxRegisterExitDll (
         const char *,                 /* Name of the exit handler   */
         const char *,                 /* Name of the DLL            */
         const char *,                 /* Name of the procedure      */
         char *,                       /* User area                  */
         unsigned long );              /* Drop authority             */

typedef APIRET (APIENTRY *PFNREXXREGISTEREXITDLL)(const char *, const char *, const char *,
                                                  char *, unsigned long);
#else
APIRET APIENTRY RexxRegisterExitDll (
         char *,                       /* Name of the exit handler   */
         char *,                       /* Name of the DLL            */
         char *,                       /* Name of the procedure      */
         char *,                       /* User area                  */
         unsigned long );              /* Drop authority             */

typedef APIRET (APIENTRY *PFNREXXREGISTEREXITDLL)(char *, char *, char *,
                                                  char *, unsigned long);
#endif


/***   Uppercase Entry Point Name */
#define REXXREGISTEREXITDLL  RexxRegisterExitDll


/***      RexxRegisterExitExe - Register a system exit. */

#ifdef __cplusplus
APIRET APIENTRY RexxRegisterExitExe (
         const char *,                 /* Name of the exit handler   */
         PFN,                          /* Address of exit handler    */
         char *);                      /* User area                  */

typedef APIRET (APIENTRY *PFNREXXREGISTEREXITEXE)(const char *, PFN, char *);

#else
APIRET APIENTRY RexxRegisterExitExe (
         char *,                       /* Name of the exit handler   */
         PFN,                          /* Address of exit handler    */
         char *);                      /* User area                  */

typedef APIRET (APIENTRY *PFNREXXREGISTEREXITEXE)(char *, PFN, char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXREGISTEREXITEXE  RexxRegisterExitExe


/***    RexxDeregisterExit - Drop registration of a system exit. */

#ifdef __cplusplus
APIRET APIENTRY RexxDeregisterExit (
         const char *,                          /* Exit name                  */
         const char * ) ;                       /* DLL module name            */

typedef APIRET (APIENTRY *PFNREXXDEREGISTEREXIT)(const char *, const char *);

#else
APIRET APIENTRY RexxDeregisterExit (
         char *,                          /* Exit name                  */
         char * ) ;                       /* DLL module name            */

typedef APIRET (APIENTRY *PFNREXXDEREGISTEREXIT)(char *, char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXDEREGISTEREXIT  RexxDeregisterExit


/***    RexxQueryExit - Query an exit for existance. */

#ifdef __cplusplus
APIRET APIENTRY RexxQueryExit (
         const char *,                 /* Exit name                  */
         const char *,                 /* DLL Module name.           */
         unsigned short *,             /* Existance flag.            */
         char * );                     /* User data.                 */

typedef APIRET (APIENTRY *PFNREXXQUERYEXIT)(const char *, const char *, unsigned short *, char *);

#else
APIRET APIENTRY RexxQueryExit (
         char *,                       /* Exit name                  */
         char *,                       /* DLL Module name.           */
         unsigned short *,             /* Existance flag.            */
         char * );                     /* User data.                 */

typedef APIRET (APIENTRY *PFNREXXQUERYEXIT)(char *, char *, unsigned short *, char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXQUERYEXIT  RexxQueryExit

/***    Asynchronous Request Interface */
/***    RexxSetHalt - Request Program Halt */

APIRET APIENTRY RexxSetHalt(
         process_id_t,                /* Process Id                  */
         thread_id_t);                /* Thread Id                   */

typedef APIRET (APIENTRY *PFNREXXSETHALT)(process_id_t, thread_id_t);

/***   Uppercase Entry Point Name */
#define REXXSETHALT  RexxSetHalt


/***    RexxSetTrace - Request Program Trace */

APIRET APIENTRY RexxSetTrace(
         process_id_t,                /* Process Id                  */
         thread_id_t);                /* Thread Id                   */

typedef APIRET (APIENTRY *PFNREXXSETTRACE)(process_id_t, thread_id_t);

/***   Uppercase Entry Point Name */
#define REXXSETTRACE  RexxSetTrace


/***    RexxResetTrace - Turn Off Program Trace */

APIRET APIENTRY RexxResetTrace(
         process_id_t,                /* Process Id                  */
         thread_id_t);                /* Thread Id                   */

typedef APIRET (APIENTRY *PFNREXXRESETTRACE)(process_id_t, thread_id_t);

/***   Uppercase Entry Point Name */
#define REXXRESETTRACE  RexxResetTrace

/***    Macro Space Interface */

/***    RexxAddMacro - Register a function in the Macro Space        */

#ifdef __cplusplus
APIRET APIENTRY RexxAddMacro(
         const char *,                 /* Function to add or change   */
         const char *,                 /* Name of file to get function*/
         unsigned long  );             /* Flag indicating search pos  */

typedef APIRET (APIENTRY *PFNREXXADDMACRO)(const char *, const char *, unsigned long);

#else
APIRET APIENTRY RexxAddMacro(
         char *,                       /* Function to add or change   */
         char *,                       /* Name of file to get function*/
         unsigned long);               /* Flag indicating search pos  */

typedef APIRET (APIENTRY *PFNREXXADDMACRO)(char *, char *, unsigned long);
#endif

/***   Uppercase Entry Point Name */
#define REXXADDMACRO  RexxAddMacro


/***    RexxDropMacro - Remove a function from the Macro Space       */

#ifdef __cplusplus
APIRET APIENTRY RexxDropMacro (
         const char * );                        /* Name of function to remove */

typedef APIRET (APIENTRY *PFNREXXDROPMACRO)(const char *);

#else
APIRET APIENTRY RexxDropMacro (
         char * );                        /* Name of function to remove */

typedef APIRET (APIENTRY *PFNREXXDROPMACRO)(char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXDROPMACRO  RexxDropMacro


/***    RexxSaveMacroSpace - Save Macro Space functions to a file    */

#ifdef __cplusplus
APIRET APIENTRY RexxSaveMacroSpace (
         unsigned long ,                      /* Argument count (0==save all)*/
         const char * *,                      /* List of funct names to save */
         const char *);                       /* File to save functions in   */

typedef APIRET (APIENTRY * PFNREXXSAVEMACROSPACE)(unsigned long, const char * *, const char *);

#else
APIRET APIENTRY RexxSaveMacroSpace (
         unsigned long ,                 /* Argument count (0==save all)*/
         char * *,                       /* List of funct names to save */
         char *);                        /* File to save functions in   */

typedef APIRET (APIENTRY * PFNREXXSAVEMACROSPACE)(unsigned long, char * *, char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXSAVEMACROSPACE  RexxSaveMacroSpace


/***    RexxLoadMacroSpace - Load Macro Space functions from a file  */

#ifdef __cplusplus
APIRET APIENTRY RexxLoadMacroSpace (
         unsigned long ,                      /* Argument count (0==load all)*/
         const char * *,                      /* List of funct names to load */
         const char *);                       /* File to load functions from */

typedef APIRET (APIENTRY *PFNREXXLOADMACROSPACE)(unsigned long, const char * *, const char *);

#else
APIRET APIENTRY RexxLoadMacroSpace (
         unsigned long ,                 /* Argument count (0==load all)*/
         char * *,                       /* List of funct names to load */
         char *);                        /* File to load functions from */

typedef APIRET (APIENTRY *PFNREXXLOADMACROSPACE)(unsigned long, char * *, char *);
#endif

/***   Uppercase Entry Point Name */
#define REXXLOADMACROSPACE  RexxLoadMacroSpace


/***    RexxQueryMacro - Find a function's search-order position     */

#ifdef __cplusplus
APIRET APIENTRY RexxQueryMacro (
         const char *,                         /* Function to search for      */
         unsigned short * );                   /* Ptr for position flag return*/

typedef APIRET (APIENTRY *PFNREXXQUERYMACRO)(const char *, unsigned short *);

#else
APIRET APIENTRY RexxQueryMacro (
         char *,                               /* Function to search for      */
         unsigned short * );                   /* Ptr for position flag return*/

typedef APIRET (APIENTRY *PFNREXXQUERYMACRO)(char *, unsigned short *);
#endif

/***   Uppercase Entry Point Name */
#define REXXQUERYMACRO  RexxQueryMacro


/***    RexxReorderMacro - Change a function's search-order          */
/***                            position                             */

#ifdef __cplusplus
APIRET APIENTRY RexxReorderMacro(
         const char *,                        /* Name of funct change order  */
         unsigned long  );                    /* New position for function   */

typedef APIRET (APIENTRY *PFNREXXREORDERMACRO)(const char *, unsigned long);

#else
APIRET APIENTRY RexxReorderMacro(
         char *,                         /* Name of funct change order  */
         unsigned long  );                    /* New position for function   */

typedef APIRET (APIENTRY *PFNREXXREORDERMACRO)(char *, unsigned long);
#endif


/***   Uppercase Entry Point Name */
#define REXXREORDERMACRO  RexxReorderMacro


/***    RexxClearMacroSpace - Remove all functions from a MacroSpace */

APIRET APIENTRY RexxClearMacroSpace(
         void );                      /* No Arguments.               */

typedef APIRET (APIENTRY *PFNREXXCLEARMACROSPACE)(void);

/***   Uppercase Entry Point Name */
#define REXXCLEARMACROSPACE  RexxClearMacroSpace

/***    Queing Services */

/***    RexxCreateQueue - Create an External Data Queue */

#ifdef __cplusplus
APIRET APIENTRY RexxCreateQueue (
        const char *,                          /* Name of queue created       */
        unsigned long,                         /* Size of buf for ret name    */
        const char *,                          /* Requested name for queue    */
        unsigned long *) ;                     /* Duplicate name flag.        */

typedef APIRET (APIENTRY *PFNREXXCREATEQUEUE)(const char *, unsigned long, const char *, unsigned long *);

#else
APIRET APIENTRY RexxCreateQueue (
        char *,                                /* Name of queue created       */
        unsigned long,                         /* Size of buf for ret name    */
        char *,                                /* Requested name for queue    */
        unsigned long *) ;                     /* Duplicate name flag.        */

typedef APIRET (APIENTRY *PFNREXXCREATEQUEUE)(char *, unsigned long, char *, unsigned long *);
#endif


/***    RexxDeleteQueue - Delete an External Data Queue */

#ifdef __cplusplus
APIRET APIENTRY RexxDeleteQueue (
        const char * );                         /* Name of queue to be deleted */

typedef APIRET (APIENTRY *PFNREXXDELETEQUEUE)(const char *);

#else
APIRET APIENTRY RexxDeleteQueue (
        char * );                         /* Name of queue to be deleted */

typedef APIRET (APIENTRY *PFNREXXDELETEQUEUE)(char *);
#endif


/*** RexxQueryQueue - Query an External Data Queue for number of      */
/***                  entries                                         */

#ifdef __cplusplus
APIRET APIENTRY RexxQueryQueue (
        const char *,                          /* Name of queue to query      */
        unsigned long *);                      /* Place to put element count  */

typedef APIRET (APIENTRY *PFNREXXQUERYQUEUE)(const char *, unsigned long *);

#else
APIRET APIENTRY RexxQueryQueue (
        char *,                           /* Name of queue to query      */
        unsigned long *);                 /* Place to put element count  */

typedef APIRET (APIENTRY *PFNREXXQUERYQUEUE)(char *, unsigned long *);
#endif


/***    RexxAddQueue - Add an entry to an External Data Queue */

#ifdef __cplusplus
APIRET APIENTRY RexxAddQueue (
        const char *,                          /* Name of queue to add to     */
        PRXSTRING,                             /* Data string to add          */
        unsigned long );                       /* Queue type (FIFO|LIFO)      */

typedef APIRET (APIENTRY *PFNREXXADDQUEUE)(const char *, PRXSTRING, unsigned long);

#else
APIRET APIENTRY RexxAddQueue (
        char *,                           /* Name of queue to add to     */
        PRXSTRING,                     /* Data string to add          */
        unsigned long );                       /* Queue type (FIFO|LIFO)      */

typedef APIRET (APIENTRY *PFNREXXADDQUEUE)(char *, PRXSTRING, unsigned long);
#endif


#include "rexxplatformapis.h"


/* BEGIN Common Prototypes */

/* The following prototypes are always included, because they are    */
/* common to all interfaces                                          */

APIRET APIENTRY RexxShutDownAPI(void);

typedef APIRET (APIENTRY *PFNREXXSHUTDOWNAPI)(void);

/***   Uppercase Entry Point Name */
#define REXXSHUTDOWNAPI  RexxShutDownAPI


/***   RexxAllocateMemory            */

void *APIENTRY RexxAllocateMemory(
                   unsigned long );           /* number of bytes to allocate */

typedef void *(APIENTRY *PFNREXXALLOCATEMEMORY)(unsigned long);


/***   RexxFreeMemory                */


APIRET APIENTRY RexxFreeMemory(
                   void *);  /* pointer to the memory returned by    */
                             /* RexxAllocateMemory                   */

typedef APIRET (APIENTRY *PFNREXXFREEMEMORY)(void *);
/* END Common Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* REXXSAA_INCLUDED */

