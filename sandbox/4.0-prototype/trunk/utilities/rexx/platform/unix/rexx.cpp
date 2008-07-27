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
/* Open Object Rexx                                                           */
/*                                                                            */
/* Executable                                                                 */
/*                                                                            */
/*   main entry point to Rexx for Unix.                                       */
/*                                                                            */
/******************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char **argv)
{
    int rc = 0;                              /* set default return                */
    int argCount = 0;                        /* argument to RexxMain              */

    char arg_buffer[2048];
    arg_buffer[0] = '\0';                /* default to no argument string     */
    char *program_name = NULL;                 /* no program to run yet             */
    for (int i = 1; i < argc; i++)           /* loop through the arguments        */
    {
        /* is this option a switch?          */
        if (program_name == NULL && (*(cp=*(argv+i)) == '-'))
        {
            switch (*++cp)
            {
                case 'i':
                case 'I':            /* image build                       */
                    ooRexxSaveImage();       // go do the save
                    return 0;

                case 'v':
                case 'V':            /* display version string            */
                    ptr = RexxGetVersionInformation();
                    fprintf(stderr, ptr);
                    if (ptr != NULL)
                    {
                        free(ptr);
                    }
                    break;

                default:                       /* ignore other switches             */
                    break;
            }
        }
        else                               /* convert into an argument string   */
        {
            if (program_name == NULL)        /* no name yet?                      */
            {
                program_name = argv[i];        /* program is first non-option       */
            }
            else                             /* part of the argument string       */
            {
                if (arg_buffer[0] != '\0')     /* not the first one?                */
                {
                    ooRexxStrcat(arg_buffer, sizeof(arg_buffer), " ");     /* add an blank                      */
                }
                ooRexxStrcat(arg_buffer, sizeof(arg_buffer), argv[i]);   /* add this to the argument string   */
                ++argCount;
            }
        }
    }
    /* missing a program name?           */
    if (program_name == NULL)
    {
        /* give a simple error message       */
        fprintf(stderr,"Syntax is \"rexx [-v] filename [arguments]\".\n");
        return -1;
    }

    argCount = (argCount==0) ? 0 : 1;  /* is there an argument ?            */
                                       /* make an argument                  */
    MakeRxString(argument, arg_buffer, strlen(arg_buffer));
    /* run this via RexxStart            */
    rc = ooRexxStart(argCount, (RxString *)&argument, program_name, NULL, "CMD", RXCOMMAND, NULL, *rexxrc, NULL);
    return rc ? rc : rexxrc;
}
