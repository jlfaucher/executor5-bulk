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
/* REXX AIX Support                                            aixmain.c      */
/*                                                                            */
/* Main interface to the REXX interpreter                                     */
/*                                                                            */
/******************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "RexxCore.h"                    /* bring in global defines           */
#include "oorexx.h"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"

/******************************************************************************/
/* Name:       ooRexxStart                                                    */
/*                                                                            */
/* Arguments:  argcount - Number of args in arglist                           */
/*             arglist - Array of args (array of RXSTRINGs)                   */
/*             programname - REXX program to run                              */
/*             instore - Instore array (array of 2 RXSTRINGs)                 */
/*             envname - Initial cmd environment                              */
/*             calltype - How the program is called                           */
/*             exits - Array of system exit names (array of RXSTRINGs)        */
/*                                                                            */
/* Returned:   result - Result returned from program                          */
/*             rc - Return code from program                                  */
/*                                                                            */
/* Notes:  Primary path into Object REXX.  Makes sure Object REXX is up       */
/*   and runs the requested program.                                          */
/*                                                                            */
/******************************************************************************/
RexxReturnCode RexxEntry ooRexxStart (
    RexxNumber argcount,          /* Num of args passed to rexx */
    RxString * arglist,           /* Array of args passed to rex */
    RexxStringPointer programname,/* [d:][path] filename[.ext]  */
    RxString * instore,           /* Loc of rexx proc in memory */
    RexxStringPointer envname,    /* ASCIIZ initial environment.*/
    RexxNumber calltype,          /* type (command,subrtn,funct) */
    RexxExit *exits,              /* SysExit env. names &  codes */
    RexxReturnCode *retcode,      /* Ret code from if numeric   */
    RxString *result)             /* Retvalue from the rexx proc */
{

  RexxReturnCode rc = RexxInterpreter::startRexxProgram(argcount, arglist,
     (stringchar_t *)programname, instore, (stringchar_t *)envname,
      calltype, exits, retcode, result);
  return rc;                           /* return the error code (negated)   */
}


#ifndef __REXX64__
#include "rexx.h"

/******************************************************************************/
/* Name:       RexxStart                                                      */
/*                                                                            */
/* Arguments:  argcount - Number of args in arglist                           */
/*             arglist - Array of args (array of RXSTRINGs)                   */
/*             programname - REXX program to run                              */
/*             instore - Instore array (array of 2 RXSTRINGs)                 */
/*             envname - Initial cmd environment                              */
/*             calltype - How the program is called                           */
/*             exits - Array of system exit names (array of RXSTRINGs)        */
/*                                                                            */
/* Returned:   result - Result returned from program                          */
/*             rc - Return code from program                                  */
/*                                                                            */
/* Notes:  Primary path into Object REXX.  Makes sure Object REXX is up       */
/*   and runs the requested program.                                          */
/*                                                                            */
/******************************************************************************/
APIRET APIENTRY RexxStart (
    long argcount,                /* Num of args passed to rexx */
    PRXSTRING arglist,            /* Array of args passed to rex */
    char *programname,            /* [d:][path] filename[.ext]  */
    PRXSTRING instore,            /* Loc of rexx proc in memory */
    char *envname,                /* ASCIIZ initial environment.*/
    long calltype,                /* type (command,subrtn,funct) */
    PRXSYSEXIT exits,             /* SysExit env. names &  codes */
    short *retcode,               /* Ret code from if numeric   */
    PRXSTRING result)             /* Retvalue from the rexx proc */
{
    RexxReturnCode rc = 0;
    RexxReturnCode retcodeNew = 0;
    rc = RexxInterpreter::startRexxProgram(argcount, (RxString *)arglist, (stringchar_t *)programname,
        (RxString *)instore, (stringchar_t *)envname,  calltype, (RexxExit *)exits, &retcodeNew, (RxString *)result);


    *retcode = (short)retcodeNew;

    return (APIRET)rc;            /* return the error code (negated)   */
}
#endif
