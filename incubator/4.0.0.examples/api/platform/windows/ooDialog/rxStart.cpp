/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2009 Rexx Language Association. All rights reserved.    */
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

/**
 *  This is the code for the executable program that runs logon.rex.  It shows
 *  how to create an instance of the interpreter and use it to execute Rexx
 *  programs.  It is very similar to other examples that show how to create an
 *  interpreter instance and execute a program.  The only thing noteworthy about
 *  this is that the resulting executable, rxStart.exe, is linked with a
 *  manifest to allow executing an ooDialog program.  Without the manifest,
 *  ooDialog would fail to initialize correctly.  The details of this are
 *  explained in the ReadMe.txt and Makefile files.
 */

#include "oorexxapi.h"
#include <stdio.h>
#include "logon.h"     // Pick up the return code defines.

/* Prototypes for several simple helper functions. The functions themselves are
 * at the bottom of the file.
 */
inline wholenumber_t conditionSubCode(RexxCondition);
int checkCondition(RexxThreadContext *, const char *);
void printInterpreterVersion(RexxInstance *);

int __cdecl main(int argc, char **argv)
{
    int rc = UNEXPECTED_ERROR_RC;
    char *programName = "logon.rex";

    // These are the arguments to RexxCreateInterpreter().  An array of any
    // number of Rexx options can be passed in, but for this example we do not
    // need any options.  So, we use NULL.
    RexxInstance *interpreter;
    RexxThreadContext *threadContext;
    RexxOption *options = NULL;

    if ( RexxCreateInterpreter(&interpreter, &threadContext, options) == 0 )
    {
        printf("Failed to create interpreter, aborting.\n");
        exit(NO_INTERPRETER_RC);
    }
    printInterpreterVersion(interpreter);

    // Execute the program and get the result returned to us.  There are no
    // arguments to the logon program, so we pass in NULL.
    RexxObjectPtr result = threadContext->CallProgram(programName, NULL);

    // During the program execution, a condition can be raised if there is an
    // unexpected error. If an exception occurred and is pending,
    // CheckCondtion() will return true. In this case we print out some
    // information on the condition.  If a condition is raised, the return from
    // the Rexx program is meaningless.
    if (threadContext->CheckCondition())
    {
        rc = checkCondition(threadContext, programName);
    }
    else
    {
        // logon.rex will returns a return code specifying the outcome of the
        // log on attempt.  This return will be a Rexx object, we convert it to
        // an int, print the outcome, and return the corresponding rc.
        if (result != NULLOBJECT)
        {
            wholenumber_t ret;
            if ( threadContext->ObjectToWholeNumber(result, &ret) )
            {
                printf("\nLogon returns: %d.  ", ret);
                rc = (int)ret;

                switch ( rc )
                {
                    case USER_GRANTED_ACCESS_RC :
                        printf("User is granted access to the system.\n");
                        break;
                    case USER_CANCELED_RC :
                        printf("User canceled the logon process.\n");
                        break;
                    case USER_DENIED_ACCESS_RC :
                        printf("User is denied access to the system.\n");
                        break;

                    default :
                        printf("This is an unexpected error from the logon process.\n");
                        break;
                }
            }
            else
            {
                // This shouldn't happen, but we will print out what is returned
                // if it does.  ObjectToStringValue() is guaranteed to give us a
                // string representation of a Rexx object.
                printf("\nUnexpected error from logon return=%s\n", threadContext->ObjectToStringValue(result));
            }
        }
        else
        {
            // Similar to the comment directly above, this shouldn't happen. But
            // if it does, let the user know.
            printf("\nUnexpected error, the logon process did not return a result.\n");
        }
    }

    // Now wait for the interpreter to terminate and we are done.
    interpreter->Terminate();

    return rc;
}


/**
 * Below are several helper functions that are not really needed for this simple
 * example.  There are just used to print some informative messages to the
 * screen.
 */

/**
 * Given an interpreter instance, prints out the interpreter version and
 * language version.  The documentation in the ooRexx programming guide explains
 * the byte encoding of the version numbers.
 */
void printInterpreterVersion(RexxInstance *interpreter)
{
    wholenumber_t ver = interpreter->InterpreterVersion();
    wholenumber_t lang = interpreter->LanguageLevel();
    printf("Created interpreter instance version=%d.%d.%d language level=%d.%02d\n\n",
           (ver & 0xff0000) >> 16, (ver & 0x00ff00) >> 8, ver & 0x0000ff, (lang & 0xff00) >> 8, lang & 0x00ff);
}

/**
 * Given a condition object, extracts and returns as a whole number the subcode
 * of the condition.
 */
inline wholenumber_t conditionSubCode(RexxCondition condition) { return (condition.code - (condition.rc * 1000)); }

/**
 * Given a thread context, prints out some condition information. This function
 * assumes the caller has ensured there is a pending condition.
 *
 * This function then clears the condition, if one is pending, which is not
 * necessary for this simple example.  In a more sophisticated application, the
 * programmer would need to decide what was appropriate.  Checking then clearing
 * a condition in the C++ code is very similar to trapping a condition and then
 * continuing in Rexx code.
 */
int checkCondition(RexxThreadContext *context, const char *name)
{
    int rc = CONDITION_RAISED_RC;
    RexxCondition condition;

    RexxDirectoryObject cond = context->GetConditionInfo();
    if ( cond != NULLOBJECT )
    {
        context->DecodeConditionInfo(cond, &condition);

        printf("\n%s raised an exception:\n", name);
        printf("Error: %d.%03d:  %s\n\n", condition.rc, conditionSubCode(condition),
               context->CString(condition.message));

        if ( condition.code == Rexx_Error_System_service_user_defined )
        {
            rc = DATABASE_ERROR_RC;
        }
    }
    context->ClearCondition();

    return rc;
}
