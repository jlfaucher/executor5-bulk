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
/*********************************************************************/
/*                                                                   */
/*     Program Name:   WINRXCMD.C                                    */
/*                                                                   */
/* Descriptive Name:   Windows Command Line Program for Subcommand   */
/*                     Interface.                                    */
/*                                                                   */
/*********************************************************************/
#include <string.h>                    /* Include string functions   */
#include <stdio.h>
#include "ooRexx.h"                    /* for REXX functionality     */
#include "RexxErrorCodes.h"            /* error constants            */

#define BUFFERLEN         256          /* Length of message bufs used*/
#define DLLNAME   "rexx.dll"
                                       /* Macro for argv[1] compares */
#define CASE(x) if (stricmp(x,argv[1]) == 0)


void parmerr(int);


int __cdecl main( int argc, char *argv[ ], char *envp[ ] )
{
   char *       scbname;               /* registration name          */
   char *       scbdll_name;           /* DLL file name              */
   char *       scbdll_proc;           /* DLL procedure name         */
   RexxReturnCode rc;

   if(argc < 2)
   {
       parmerr(Error_RXSUBC_general);  /* Must be at lease 1 argument*/
   }

   CASE("REGISTER"){                   /* Registration check         */
     if (argc < 5)
     {
         parmerr(Error_RXSUBC_register); /* requires 4 parameters */
     }
     scbname=argv[2];                  /* Should be Environment Name */
     scbdll_name=argv[3];              /* Should be Dll Name         */
     scbdll_proc=argv[4];              /* Should be Function Name    */
                                       /* Go register the environment*/
     return ooRexxRegisterLibrarySubcom(scbname, scbdll_name,
         scbdll_proc, NULL, RXSUBCOM_DROPPABLE, SUBCOM_CLASSIC_STYLE);
   }
   CASE("QUERY"){                      /* Query Check                */
     if (argc < 3)
     {
         parmerr(Error_RXSUBC_query);  /* requires 3 parameters */
     }
     if (argc < 4)
     {
         argv[3]="";                   /* if only 3 passed, dummy 4  */
     }
     return ooRexxQuerySubcom(         /* code.                      */
                 argv[2],              /* Should be Dll Name         */
                 argv[3],              /* Should be Function Name    */
                 NULL,                 /* Ptr to storage for existnce*/
                 NULL,                 /* Ptr to storage for userdata*/
                 SUBCOM_CLASSIC_STYLE);/* call style                 */
   }
   CASE("DROP"){                       /* Drop Check                 */
     if (argc < 3)
     {
         parmerr(Error_RXSUBC_drop);   /* Must pass at least 3 args*/
     }
     if (argc < 4)
     {
         argv[3] = "";                 /* if only 3 passed, dummy 4  */
     }
     return ooRexxDeregisterSubcom(    /* code                       */
                 argv[2],              /* Should be Dll Name         */
                 argv[3]);             /* Should be Function Name    */
   }
   CASE("LOAD"){                       /* Load Check                 */
     if (argc < 3)
     {
         parmerr(Error_RXSUBC_load);   /* Must pass at least 3 args*/
     }
     if (argc < 4)
     {
         argv[3]="";                   /* if only 3 passed, dummy 4  */
     }
     return ooRexxLoadSubcom(          /* and return its return code */
                 argv[2],              /* Should be Dll Name         */
                 argv[3]);             /* Should be Function Name    */
                                       /*                            */
   }
   parmerr(Error_RXSUBC_general);      /* Otherwise, must be a error */
   return 0;                           /* dummy return               */
}


void parmerr(
  int arg )
{
   char     dataarea[BUFFERLEN];        /* buf addr to return message   */
   size_t   msglen = 0;                 /* length of returned message   */
   HINSTANCE hDll=NULL;

   memset(dataarea, 0, BUFFERLEN);

   hDll = LoadLibrary(DLLNAME);
   if (hDll)
   {
      if (!LoadString(hDll, arg, dataarea, BUFFERLEN))
          strcpy(dataarea,"Error, but no error message available.");
   }
   else
          strcpy(dataarea,"Error, but no error message available because REXX.DLL not loaded.");

   printf("REX%d: %s",arg, dataarea);
   FreeLibrary(hDll);
   exit(-1);
}
