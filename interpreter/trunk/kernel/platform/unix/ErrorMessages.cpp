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
/* Rexx Unix Support                                                          */
/*                                                                            */
/* Retrieve message from message repository using the X/Open catopen(),       */
/* catgets() and catclose() function calls.                                   */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/

#include <stdio.h>                     /* include standard headers          */
#include <string.h>
#include <ctype.h>
#include <limits.h>

//#include <features.h>
//#include "nl_types.h"
#include <nl_types.h>
#ifdef AIX
#include <mesg.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#define ERROR_TABLE                    /* include error message table       */
#define CCHMAXPATH PATH_MAX+1
#include "RexxCore.h"                    /* incl general definitions        */
#include "StringClass.hpp"
#include "SysInterpreter.hpp"
#include "Interpreter.hpp"


#ifdef LINUX
//#define SECOND_PARAMETER MCLoadAll   /* different sign. Lin-AIX           */
#define SECOND_PARAMETER 1             /* different sign. Lin-AIX           */
#define CATD_ERR -1                    /* Duplicate for AIX                 */
#else
#define SECOND_PARAMETER 0             /* 0 for no  NL_CAT_LOCALE           */
#endif

RexxString * SysInterpreter::messageText(   /* simplified whole code             */
    wholenumber_t code )               /* message code to extract           */
/****************************************************************************/
/* Function:  Return a message from the message catalog, including header.  */
/****************************************************************************/
{
 nl_catd        catd;                  /* catalog descriptor from catopen() */
 int            set_num = 1;           /* message set 1 from catalog        */
 ErrorMessage  *p;                     /* message table scan pointer        */
 int            msgid;                 /* message number                    */
 char           DataArea[256];         /* buf to return message             */
 stringchar_t * message;
                                       /* loop through looking for the      */
                                       /* error code                        */
 for (p = RexxInterpreter::messageMappingTable; p->code != 0; p++) {
   if (p->code == code) {              /* found the target code?            */

     msgid = p->msgid;                 /* get msg number associated w/ error*/
                                       /* open message catalog in NLSPATH   */
     if ((catd = catopen(REXXMESSAGEFILE, SECOND_PARAMETER)) == (nl_catd)CATD_ERR)
     {
#ifdef AIX
       sprintf(DataArea, "/usr/lpp/orexx.msg.en_US/%s", REXXMESSAGEFILE);
#else
       sprintf(DataArea, "/opt/orexx/%s", REXXMESSAGEFILE);
#endif
       if ((catd = catopen(DataArea, SECOND_PARAMETER)) == (nl_catd)CATD_ERR)
       {
         sprintf(DataArea, "\nCannot open REXX message catalog %s.\n",
                                                REXXMESSAGEFILE);
         return new_string((stringchar_t *)DataArea, strlen(DataArea));
       }
     }                                   /* retrieve message from repository  */
     message = (stringchar_t *)catgets(catd, set_num, msgid, NULL);
     if(!message)                    /* got a message ?                   */
     {
#if defined(OPSYS_LINUX) && !defined(OPSYS_SUN)
       sprintf(DataArea, "/opt/orexx/%s", REXXMESSAGEFILE);
       if ((catd = catopen(DataArea, SECOND_PARAMETER)) == (nl_catd)CATD_ERR)
       {
         sprintf(DataArea, "\nCannot open REXX message catalog %s.\n",
                                                REXXMESSAGEFILE);
         return new_string((stringchar_t *)DataArea, strlen(DataArea));
       }
       else
       {
         message = (stringchar_t *)catgets(catd, set_num, msgid, NULL);
         if(!message)                    /* got a message ?                   */
           strcpy(DataArea,"Error message not found!");
         else
           strcpy(DataArea, (char *)message);
       }
#else
       strcpy(DataArea,"Error message not found!");
#endif
     }
     else
       strcpy(DataArea, (char *)message);
     catclose(catd);                 /* close the catalog                 */
                                     /* convert and return the message    */
     return new_string((stringchar_t *)DataArea, strlen(DataArea));
   }
 }
 return OREF_NULL;                     /* no message retrieved              */
}






