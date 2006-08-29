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
* Module Name: ooRexx.h
*
* ooRexx Common Definitions File
*
\*********************************************************************/

#ifndef OOREXX_INCLUDED
#define OOREXX_INCLUDED

#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
#define BEGIN_EXTERN_C() extern "C" {
#define END_EXTERN_C() }
#else
#define BEGIN_EXTERN_C()
#define END_EXTERN_C()
#endif

#include "rexxapitypes.h"

typedef char *RexxStringPointer;
typedef size_t RexxStringLength;
typedef ssize_t RexxNumber;
typedef size_t RexxUnsignedNumber;
typedef int RexxBoolean;
typedef uintptr_t RexxProcessID;
typedef uintptr_t RexxThreadID;
typedef void * RexxUserData;

typedef ssize_t RexxReturnCode;

BEGIN_EXTERN_C()

typedef RexxReturnCode (RexxEntry *RexxCallback)();   // PFN pointer to a function

END_EXTERN_C()
#include "oorexxapi.h"


#ifdef __REXX64__
#define REXX_DEFAULT_DIGITS 19
#else
#define REXX_DEFAULT_DIGITS 9
#endif

// this requests that the conversion be done using "computer" limits rather than
// "human" limits.
#define REXX_NO_DIGITS -1

#define RxIsNullString(r)      ((r).strptr == 0)
#define RxIsZeroLenString(r)   ((r).strptr != 0 && (r).strlength == 0)
#define RxIsValidString(r)     ((r).strptr != 0 && (r).strlength == 0)
#define RxStringLength(r)      (RxIsNullString(r) ? 0 : (r).strlength)
#define RxStringPtr(r)         (r).strptr)
#define MakeRxString(r,p,l)  { (r).strptr = (RexxStringPointer)p; (r).strlength=(RexxStringLength)l; }

BEGIN_EXTERN_C()


/***   RexxAllocateMemory            */

void * RexxEntry ooRexxAllocateMemory(
    RexxStringLength);     /* number of bytes to allocate */

/***   RexxFreeMemory                */


RexxReturnCode RexxEntry ooRexxFreeMemory(
                  void *);   /* pointer to the memory returned by    */
                             /* RexxAllocateMemory                   */

END_EXTERN_C();

/*********************************************************************/
/*                                                                   */
/*                            Common                                 */
/*                                                                   */
/*********************************************************************/

/* This section defines return codes and constants for REXX calls    */

#include "rexxapidefs.h"

/***    Structure for external interface string (RexxString) */
typedef struct _RxString
{
        RexxStringLength  strlength;           /*   length of string         */
        RexxStringPointer strptr;              /*   pointer to string        */
}  RxString;

/* DATETIME structure ----------------------------------------------- */
typedef struct _RexxTimeStamp {       /* REXX time stamp format       */
  unsigned short hours;               /* hour of the day (24-hour)    */
  unsigned short minutes;             /* minute of the hour           */
  unsigned short seconds;             /* second of the minute         */
  unsigned short hundredths;          /* hundredths of a second       */
  unsigned short day;                 /* day of the month             */
  unsigned short month;               /* month of the year            */
  unsigned short year;                /* current year                 */
  unsigned short weekday;             /* day of the week              */
  unsigned long  microseconds;        /* microseconds                 */
  unsigned long  yearday;             /* day number within the year   */
  unsigned short valid;               /* valid time stamp marker      */
} RexxTimeStamp;


/***    Structure for system exit block (RXSYSEXIT) */

typedef struct _RexxExit {
   RexxStringPointer  sysexit_name;    /* subcom enviro for sysexit  */
   RexxNumber         sysexit_code;    /* sysexit function code      */
}  RexxExit;



/***    Structure of Shared Variable Request Block */

typedef struct _VariableRequest {     /* shvb */
    struct _VariableRequest *shvnext; /* pointer to the next block   */
    RxString           shvname;       /* Pointer to the name buffer  */
    RxString           shvvalue;      /* Pointer to the value buffer */
    RexxStringLength   shvnamelen;    /* Length of the name value    */
    RexxStringLength   shvvaluelen;   /* Length of the fetch value   */
    RexxNumber         shvcode;       /* Function code for this block*/
    RexxNumber         shvret;        /* Individual Return Code Flags*/
}   VariableRequest;


/***    Main Entry Point to the REXX Interpreter */

BEGIN_EXTERN_C();

RexxReturnCode RexxEntry ooRexxStart (
         RexxNumber,                   /* Num of args passed to rexx */
         RxString *,                   /* Array of args passed to rex */
         RexxStringPointer,            /* [d:][path] filename[.ext]  */
         RxString *,                   /* Loc of rexx proc in memory */
         RexxStringPointer,            /* ASCIIZ initial environment.*/
         RexxNumber,                   /* type (command,subrtn,funct) */
         RexxExit *,                   /* SysExit env. names &  codes */
         RexxReturnCode *,             /* Ret code from if numeric   */
         RxString *);                  /* Retvalue from the rexx proc */

void RexxEntry ooRexxWaitForTermination(void);

RexxReturnCode RexxEntry ooRexxDidRexxTerminate(void);

RexxBoolean RexxEntry ooRexxSetProcessMessages(RexxBoolean onoff);

/***    Subcommand Interface */

/* This typedef simplifies coding of a Subcommand handler.           */
typedef RexxReturnCode RexxEntry ooRexxSubcomHandler(
                                RexxCallContext *,
                                RexxStringObject,
                                RexxUnsignedNumber *,
                                RexxObjectPtr *);

#define SUBCOM_CLASSIC_STYLE 1
#define SUBCOM_OBJECT_STYLE 2

/***   ooRexxRegisterLibrarySubcom -- Register a library entry point           */
/***   as a Subcommand handler */

RexxReturnCode RexxEntry ooRexxRegisterLibrarySubcom(
         RexxStringPointer,                // Name of subcom handler
         RexxStringPointer,                // Name of library
         RexxStringPointer,                // Name of procedure in library
         RexxUserData,                     // User area
         RexxUnsignedNumber,               // Drop authority.
         RexxNumber);                      // the subcom call style


/***   RexxRegisterSubcom -- Register an entry point          */
/***   as a Subcommand handler */

RexxReturnCode RexxEntry ooRexxRegisterSubcom(
         RexxStringPointer,             // Name of subcom handler
         RexxCallback,                  // address of handler
         RexxUserData,                  // User area
         RexxNumber);                   // the subcom call style

/***    RexxQuerySubcom - Query an environment for Existance */

RexxReturnCode RexxEntry ooRexxQuerySubcom(
         RexxStringPointer,            // Name of the Environment
         RexxStringPointer,            // libray Module Name
         RexxUnsignedNumber *,         // Stor for existance code
         RexxUserData *,               // Stor for user word
         RexxNumber);                  // the subcom call style

/***    RexxDeregisterSubcom - Drop registration of a Subcommand     */
/***    environment */

RexxReturnCode RexxEntry ooRexxDeregisterSubcom(
         RexxStringPointer,            /* Name of the Environment    */
         RexxStringPointer);           /* library module (optional)  */

/***    Shared Variable Pool Interface */

/***    RexxVariablePool - Request Variable Pool Service */

RexxReturnCode RexxEntry ooRexxVariablePool(
         VariableRequest *);           /* Pointer to list of request blocks */

/***    External Function Interface */

#define FUNCTION_CLASSIC_STYLE 1
#define FUNCTION_OBJECT_STYLE 2
#define FUNCTION_TYPED_STYLE 3

/***    RexxRegisterLibraryFunction - Register a function in the AFT */

RexxReturnCode RexxEntry ooRexxRegisterLibraryFunction(
        RexxStringPointer,             // Name of function to add
        RexxStringPointer,             // library name
        RexxStringPointer,             // Entry in library
        RexxNumber);                   // the function call style

/***    RexxRegisterFunction - Register a function in the AFT */

RexxReturnCode RexxEntry ooRexxRegisterFunction(
        RexxStringPointer,             // Name of function to add
        RexxCallback,                  // Entry point
        RexxNumber);                   // the function call style

/***    RexxDeregisterFunction - Delete a function from the AFT */

RexxReturnCode RexxEntry ooRexxDeregisterFunction (
        RexxStringPointer);             /* Name of function to remove */

/***    RexxQueryFunction - Scan the AFT for a function */

RexxReturnCode RexxEntry ooRexxQueryFunction (
        RexxStringPointer);             /* Name of function to find   */

/***   System Exits */

typedef void * RexxExitParm;          // opaque data type for exit parameters


/***    Subfunction RXFNCCAL - External Function Calls */
typedef struct
{
   unsigned rxfferr  : 1;              /* Invalid call to routine.   */
   unsigned rxffnfnd : 1;              /* Function not found.        */
   unsigned rxffsub  : 1;              /* Called as a subroutine     */
}  RxFunctionExitFlags;

// this structure is used for both RXFNC and RXEXF exits
typedef struct
{
   RxFunctionExitFlags rxfnc_flags;    // function flags
   RexxStringObject    function_name;  // name of the function
   RexxStringObject    queue_name;     // current queue name
   RexxNumber          argument_count; // count of arguments
   RexxObjectPtr       *arguments;     // array of argument objects
   RexxObjectPtr       return_value;   // function return value
}  RxFunctionExitParm;

/***    Subfunction RXCMDHST -- Process Host Commands     */

typedef struct
{
   unsigned rxfcfail : 1;               /* Command failed.            */
   unsigned rxfcerr  : 1;               /* Command ERROR occurred.    */
}  RxCommandExitFlags;

typedef struct
{
   RxCommandExitFlags  rxcmd_flags;      /* error/failure flags        */
   RexxStringObject    address_name;     /* Pointer to address name.   */
   RexxStringObject    command_string;   /* The command string.        */
   RexxObjectPtr       return_value;     /* returned result object     */
}  RxCommandExitParm;


/***     Subfunction RXMSQPLL -- Pull Entry from Queue */

typedef struct
{
   RexxStringObject   queue_item;      /* Pointer to dequeued entry  */
} RxQueuePullParm;


/***    Subfunction RXMSQPSH -- Push Entry on Queue */
typedef struct
{
   unsigned rxfmlifo : 1;               /* Stack entry LIFO if set    */
}  RxQueuePushFlags;

typedef struct
{
   RxQueuePushFlags  rxmsq_flags;        /* LIFO/FIFO flag             */
   RexxStringObject  queue_item;         /* The entry to be pushed.    */
}  RxQueuePushParm;


/***    Subfunction RXMSQSIZ -- Return the Current Queue Size */

typedef struct
{
   RexxStringLength  rxmsq_size;       /* Number of Lines in Queue   */
}  RxQueueSizeParm;


/***    Subfunction RXMSQNAM -- Set Current Queue Name */

typedef struct
{
   RexxStringObject queue_name;        /* RxString containing        */
}  RxSetQueueNameParm;


/***    Subfunction RXSIOSAY -- Perform SAY Clause */

typedef struct
{
   RexxStringObject  output_string;    /* String to display.         */
}  RxSayParm;


/***    Subfunction RXSIOTRC -- Write Trace Output */

typedef struct
{
   RexxStringObject  output_string;    /* Trace line to display.     */
}  RxTraceOutputParm;


/***    Subfunction RXSIOTRD -- Read Input from the Terminal */

typedef struct
{
   RexxStringObject  input_string;     /* RxString for output.       */
}  RxTerminalPullParm;


/***    Subfunction RXSIODTR -- Read Debug Input from the Terminal */

typedef struct
{
   RexxStringObject  input_string;     /* RxString for output.       */
}  RxDebugReadParm;


/***    Subfunction RXHSTTST -- Test for HALT Condition */

typedef struct
{
   unsigned rxfhhalt : 1;              /* Set if HALT occurred.      */
}  RxHaltFlags;

typedef struct
{
   RxHaltFlags rxhlt_flags;            /* Set if HALT occurred       */
}  RxHaltTestParm;


/***    Subfunction RXTRCTST -- Test for TRACE Condition */

typedef struct
{
   unsigned rxftrace : 1;              /* Set to run external trace. */
}  RxTraceFlags;

typedef struct
{
   RxTraceFlags rxtrc_flags;           /* Set to run external trace  */
}  RxTraceTestParm;

typedef struct
{
   unsigned rxftrace : 1;              /* Set to run external trace. */
}  RxDebugFlags;

typedef struct
{
   RxDebugFlags        rxdbg_flags;    /* Set to run external trace before */
   RexxUnsignedNumber  line_number;
   RexxStringObject    filename;
   RexxStringObject    routine_name;
}  RxDebugTestParm;


typedef struct
{
   RexxStringObject    variable_name;  /* the name of the variable to retrieve */
   RexxObjectPtr       variable_value; /* returned variable value (as object)  */
}  RxNovalueParm;

typedef struct
{
   RexxStringObject    name;           /* name of the requested value          */
   RexxStringObject    selector;       /* name of the selector                 */
   RexxObjectPtr       old_value;      /* the returned old value               */
   RexxObjectPtr       new_value;      /* option new value to set.             */
}  RxValueParm;


typedef RexxReturnCode RexxEntry ooRexxExitHandler(RexxExitContext *, RexxNumber, RexxNumber, RexxExitParm);


#define EXIT_CLASSIC_STYLE 1
#define EXIT_OBJECT_STYLE 2

/***      RexxRegisterLibraryExit - Register a system exit. */

RexxReturnCode RexxEntry ooRexxRegisterLibraryExit(
         RexxStringPointer,            // Name of the exit handler
         RexxStringPointer,            // Name of the library
         RexxStringPointer,            // Name of the procedure
         RexxUserData,                 // User area
         RexxUnsignedNumber,           // Drop authority.
         RexxNumber);                  // the exit call style

/***      RexxRegisterExit - Register a system exit. */

RexxReturnCode RexxEntry ooRexxRegisterExit(
         RexxStringPointer,            // Name of the exit handler
         RexxCallback,                 // Address of exit handler
         RexxUserData,                 // User area
         RexxNumber);                  // the exit call style

/***    RexxDeregisterExit - Drop registration of a system exit. */

RexxReturnCode RexxEntry ooRexxDeregisterExit (
         RexxStringPointer,             /* Exit name                  */
         RexxStringPointer);            /* library module name (optional) */

/***    RexxQueryExit - Query an exit for existance. */

RexxReturnCode RexxEntry ooRexxQueryExit (
         RexxStringPointer,            // Exit name
         RexxStringPointer,            // library Module name.
         RexxUnsignedNumber,           // Stor for existance code
         RexxUserData *,               // User data.
         RexxNumber *);                // the exit call style

/***    Macro Space Interface */

/***    RexxAddMacro - Register a function in the Macro Space        */

RexxReturnCode RexxEntry ooRexxAddMacro(
         RexxStringPointer,           /* Function to add or change   */
         RexxStringPointer,           /* Name of file to get function*/
         RexxUnsignedNumber);         /* Flag indicating search pos  */

/***    RexxDropMacro - Remove a function from the Macro Space       */

RexxReturnCode RexxEntry ooRexxDropMacro(
         RexxStringPointer);          /* Name of function to remove */

/***    RexxSaveMacroSpace - Save Macro Space functions to a file    */

RexxReturnCode RexxEntry ooRexxSaveMacroSpace(
         RexxUnsignedNumber,          /* Argument count (0==save all)*/
         RexxStringPointer *,         /* List of funct names to save */
         RexxStringPointer);          /* File to save functions in   */

/***    RexxLoadMacroSpace - Load Macro Space functions from a file  */

RexxReturnCode RexxEntry ooRexxLoadMacroSpace(
         RexxUnsignedNumber,          /* Argument count (0==load all)*/
         RexxStringPointer *,         /* List of funct names to load */
         RexxStringPointer);          /* File to load functions from */

/***    RexxQueryMacro - Find a function's search-order position     */

RexxReturnCode RexxEntry ooRexxQueryMacro (
         RexxStringPointer,           /* Function to search for      */
         RexxUnsignedNumber *);       /* Ptr for position flag return*/

/***    RexxReorderMacro - Change a function's search-order          */
/***                            position                             */

RexxReturnCode RexxEntry ooRexxReorderMacro(
         RexxStringPointer,           /* Name of funct change order  */
         RexxUnsignedNumber);         /* New position for function   */

/***    RexxClearMacroSpace - Remove all functions from a MacroSpace */

RexxReturnCode RexxEntry ooRexxClearMacroSpace(
         void );                      /* No Arguments.               */

/***    Queuing Services */

#define MAX_QUEUE_NAME_LENGTH 250

/***    RexxCreateQueue - Create an External Data Queue */

RexxReturnCode RexxEntry ooRexxCreateQueue (
        RexxStringPointer,             /* Name of queue created       */
        RexxStringLength,              /* Size of buf for ret name    */
        RexxStringPointer,             /* Requested name for queue    */
        RexxBoolean *);                /* Duplicate name flag.        */

/***    RexxDeleteQueue - Delete an External Data Queue */

RexxReturnCode RexxEntry ooRexxDeleteQueue(
        RexxStringPointer);             /* Name of queue to be deleted */

/*** RexxQueryQueue - Query an External Data Queue for number of      */
/***                  entries                                         */

RexxReturnCode RexxEntry ooRexxQueryQueue (
        RexxStringPointer,             /* Name of queue to query      */
        RexxUnsignedNumber *);         /* Place to put element count  */

/***  ooRexxAddQueue - Add an entry to an External Data Queue */

RexxReturnCode RexxEntry ooRexxAddQueue (
        RexxStringPointer,             /* Name of queue to add to     */
        RxString *,                    /* Data string to add          */
        RexxUnsignedNumber);           /* Queue type (FIFO|LIFO)      */

/***  ooRexxPullQueue - Retrieve data from an External Data Queue */

RexxReturnCode RexxEntry ooRexxPullQueue (
        RexxStringPointer,             /* Name of queue to read from  */
        RxString *,                    /* RxString to receive data  */
        RexxTimeStamp *,               /* time stamp data             */
        RexxUnsignedNumber);           /* wait status (WAIT|NOWAIT)   */

/***  ooRexxAddQueue - Add an entry to an External Data Queue */

RexxReturnCode RexxEntry ooRexxClearQueue (
        RexxStringPointer);            /* Name of queue to clear      */

/* BEGIN Common Prototypes */

/* The following prototypes are always included, because they are    */
/* common to all interfaces                                          */

RexxReturnCode RexxEntry ooRexxShutDownAPI(void);

END_EXTERN_C();

#endif

