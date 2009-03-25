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
/* THIS SOFTWARE IS PROVIDED BY THE COPYright HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYright   */
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
 * A simple example that creates an instance of the interpreter and uses that
 * instance to execute some Rexx programs.
 *
 * After the interpreter instance is created, it is used to execute the
 * simpleRW.rex program, which uses concurrency to read and write to a buffer
 * asynchronously.  That program returns before the reader and writers finish
 * their work.
 *
 * The interpreter instance is then used to run the backward.fnc Rexx program
 * which comes from an old example of using the classic native API.  The
 * interpreter instance passes the program a string, the program returns the
 * string in reverse word order.  This is printed to the screen from this C++
 * code.
 *
 * The simpleRW.rex program prints its output to the screen from the Rexx code
 * using 'say' statements.  Note the backward.fnc output printed in the middle
 * of the simpleRW.rex output.
 *
 * This example is designed to work correctly with simpleRW.rex as the program
 * code. When this program is invoked with no arguments, it automatically uses
 * simpleRW.rex.
 *
 * To allow the programmer to experiment a little with changing the Rexx code,
 * this program will accept as the first argument the name of an alternative
 * file to use as the program code.  There is nothing fancy with the argument
 * checking, the program simply uses it. This means, of course, that this
 * program will fail if you pass in an incorrect argument.
 */

#include "oorexxapi.h"
#include <stdio.h>

/* Prototypes for several simple helper functions. The functions themselves are
 * at the bottom of the file.
 */
inline wholenumber_t conditionSubCode(RexxCondition);
void checkCondition(RexxThreadContext *, const char *);
void printInterpreterVersion(RexxInstance *);

int __cdecl main (int argc, char **argv)
{
    char *programName = "simpleRW.rex";
    if ( argc == 2 )
    {
        programName = argv[1];
    }

    // These are the arguments to RexxCreateInterpreter().  An array of any
    // number of Rexx options can be passed in, but for this example we do not
    // need any options.  So, we use NULL.
    RexxInstance *interpreter;
    RexxThreadContext *threadContext;
    RexxOption *options = NULL;

    if ( RexxCreateInterpreter(&interpreter, &threadContext, options) == 0 )
    {
        printf("Failed to create interpreter, aborting.\n");
        exit(1);
    }
    printInterpreterVersion(interpreter);

    // This string objects will be used as arguments to the backward.fnc
    // program.  They are created here, before either of the programs are
    // executed, so that it is more obvious from the output that there are
    // multiple activities running at the same time.
    char *inputString = "Object Rexx was great, but Open Object Rexx is greater.";
    RexxArrayObject args1 = threadContext->ArrayOfOne(threadContext->String(inputString));
    RexxArrayObject args2 = threadContext->ArrayOfOne(threadContext->String("One if by sea, two if by land"));

    // Execute the program and get the result returned to us.
    printf("Using interpreter to execute %s\n\n", programName);
    RexxObjectPtr result = threadContext->CallProgram(programName, NULL);

    // During the program execution, a condition can be raised if there is an
    // unexpected error. If an exception occurred and is pending,
    // CheckCondtion() will return true. In this case we print out some
    // information on the condition, otherwise we print out the return, if any,
    // from the program. (We know that simpleRW.rex does not return anything.)
    //
    // To see some condition output from this, temporarily rename simpleRW.rex
    // to some other name.
    if (threadContext->CheckCondition())
    {
        checkCondition(threadContext, programName);
    }
    else
    {
        // We know the simpleRW.rex program does not return anything, so this
        // result will always be null and nothing prints.
        //
        // Note that we use ObjectToStringValue().  That is guarenteed to return
        // the ASCII-Z string representation of the object. If we passed in, say
        // an .array object to the CString() function, we would get a crash.
        if (result != NULLOBJECT)
        {
            printf("\nProgram result=%s\n\n", threadContext->ObjectToStringValue(result));
        }
    }

    // Now execute the backward.fnc program with the first argument string.
    programName = "backward.fnc";
    result = threadContext->CallProgram(programName, args1);

    // if an exception occurred, print some information to the screen.
    if ( threadContext->CheckCondition() )
    {
        checkCondition(threadContext, programName);
    }
    else
    {
        if (result != NULLOBJECT)
        {
            // backward.fnc returns a string object.  We know that for sure, so
            // I use CString().  But, to be extra safe, we could use
            // ObjectToStringValue().
            printf("C++ got result -->%s\n", threadContext->CString(result));
        }
    }

    // This next section is just a repeat of the above, but using the second
    // string argument.
    result = threadContext->CallProgram(programName, args2);
    if ( threadContext->CheckCondition() )
    {
        checkCondition(threadContext, programName);
    }
    else
    {
        if (result != NULLOBJECT)
        {
            printf("C++ got result -->%s\n\n", threadContext->CString(result));
        }
    }

    // Now wait for the interpreter to terminate and we are done.
    interpreter->Terminate();

    return 0;
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
 * Given a thread context, prints out some condition information, if there is a
 * pending condition.
 *
 * This function then clears the condition, if one is pending, which is not
 * necessary for this simple example.  In a more sophisticated application, the
 * programmer would need to decide what was appropriate.  Checking then clearing
 * a condition in the C++ code is very similar to trapping a condition and then
 * continuing in Rexx code.
 */
void checkCondition(RexxThreadContext *context, const char *name)
{
    // Note that we make sure there is a condition pending, even though in this
    // example the caller has already checked.  That lets us use the function in
    // some of the other examples without changing it.
    if (context->CheckCondition())
    {
        RexxCondition condition;

        RexxDirectoryObject cond = context->GetConditionInfo();

        // There is a condition pending, cond should not be null, check anyway.
        if ( cond != NULLOBJECT )
        {
            context->DecodeConditionInfo(cond, &condition);

            printf("\n%s raised an exception:\n", name);
            printf("Error: %d.%03d:  %s\n\n", condition.rc, conditionSubCode(condition),
                   context->CString(condition.message));
        }
        context->ClearCondition();
    }
}
