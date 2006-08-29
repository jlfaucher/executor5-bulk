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
/*  File Name:          REXX.C                                   */
/*                                                                   */
/*  Description:        Provides a sample call to the REXX           */
/*                      interpreter, passing in an environment name, */
/*                      a file name, and a single argument string.   */
/*                                                                   */
/*  Entry Points:       main - main entry point                      */
/*                                                                   */
/*  Input:              None                                         */
/*                                                                   */
/*  Output:             returns 0 in all cases.                      */
/*                                                                   */
/*********************************************************************/


#include <windows.h>
#include "oorexx.h"
#include <malloc.h>
#include <stdio.h>                          /* needed for printf()        */
#include <string.h>                         /* needed for strlen()        */
#include "InternalAPI.hpp"

#include "ArgumentParser.h"  /* defines getArguments and freeArguments */

//
//  MAIN program
//
int __cdecl main(int argc, char *argv[])
{
    char *cp;                            /* option character pointer          */
    RxString arguments;                  /* rexxstart argument                */
    size_t argcount;

    /*
     * Convert the input array into a single string for the Object REXX
     * argument string. Initialize the RXSTRING variable to point to this
     * string. Keep the string null terminated so we can print it for debug.
     * First argument is name of the REXX program
     * Next argument(s) are parameters to be passed
    */


    int currentArg;                      // current argument position

    // first loop over any Rexx option arguments that may have been specified.
    // these proceed the program name, and may result in quick termination too,
    // based on the option.
    for (currentArg = 1; currentArg < argc; currentArg++)
    {     /* loop through the arguments        */
          /* is this an option switch?         */
        if ((*(cp=*(argv + currentArg)) == '-' || *cp == '/'))
            switch (*++cp)
            {
                // just display the verion
                case 'v':
                case 'V':            /* version display */
                 {
                     char *version = (char *)ooRexxGetVersionInformation();
                     if (version != NULL)
                     {
                         printf(version, "Interpreter");
                         ooRexxFreeMemory(version);
                     }
                 }
                 return 0;

                default:                       // ignore other switches...
                    break;
            }
        else
        {
            break;                            // we've reached the end of the options...process real execution
        }
    }

    // we need to have at least a program name.  If not there,
    // this is an error.
    if (currentArg >= argc)
    {
        printf("Syntax: REXX ProgramName [parameter_1....parameter_n]\n");
        // try to unload the orexx memory manager
        return -1;
    }
    // get the program name...anything else is part of the argument string.
    char *program_name = argv[currentArg++];

    char  arg_buffer[8192];              // starting argument buffer
    arg_buffer[0] = '\0';                // default to no argument string

    // now we get the raw command line and get the string part from that.
    strcpy(arg_buffer, GetCommandLine());
    getArguments(NULL, arg_buffer, &argcount, &arguments);

    RexxNumber rexxrc;
    /* Here we call the interpreter.  We don't really need to use     */
    /* all the casts in this call; they just help illustrate          */
    /* the data types used.                                           */
    RexxReturnCode rc = ooRexxStart(argcount,
        &arguments,
        (RexxStringPointer)program_name,
        NULL,
        (RexxStringPointer)"CMD",
        RXCOMMAND,
        NULL,
        &rexxrc,
        NULL);

    // free any argument storage
    freeArguments(NULL, &arguments);
    // return interpeter or
    return rc != 0 ? rc : rexxrc;          // rexx program return cd
}
