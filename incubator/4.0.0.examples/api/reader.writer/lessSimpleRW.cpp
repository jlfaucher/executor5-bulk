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
 * This example is similar to the simplestRW and simpleRW examples.  It uses the
 * same .worker .fifo classes and the same basic program code. However, the
 * program code has been placed in a routine, (::routine startWorkers) rather
 * than as the executable code preceding the first directive in the file.
 *
 * This example starts the same basic reader / writer program as simplestRW does
 * by calling the routine containing the program code. In addition, it
 * instantiates and starts a third worker from this C++ code so that there are 3
 * workers, in 3 different threads reading and writing to the fifo buffer.  The
 * output to the console shows the interleaving of the writes from the 3
 * threads.  It also shows that the reads are interleaved with the writes
 * because the reads start before the writes have finished.
 *
 * There are a couple of other miscellaneous examples shown in the C++ code.
 *
 * 1.) The fifo buffer object is created in the called routine and placed in the
 * .local environment.  The C++ code retrieves this buffer from .local for use
 * in the C++ worker.
 *
 * 2.) The lessSimpleRW.rex code package places an 'interpreter' ID string in
 * the .local envrionment.  The C++ code changes the value of the ID string from
 * [# 0] to [# 1] which is visible in the console output.
 *
 * Note that the error handling is very primitive in that the C++ code basically
 * just quits on any error.  In addition, there are a few places where a check
 * for NULLOBJECT is skipped.
 *
 * This example is designed to work correctly with lessSimpleRW.rex as the
 * package code.  When this program is invoked with no arguments, it
 * automatically uses lessSimpleRW.rex.
 *
 * To allow the programmer to experiment a little with changing the Rexx code,
 * this program will accept as the first argument the name of an alternative
 * file to use as the package code.  There is nothing fancy with the argument
 * checking, the program simply uses it. This means, of course, that this
 * program will fail if you pass in an incorrect argument.
 */

#include "oorexxapi.h"
#include <stdio.h>

/* Prototypes for several simple helper functions. The functions themselves are
 * at the bottom of the file.
 */
inline wholenumber_t conditionSubCode(RexxCondition);
bool checkCondition(RexxThreadContext *, const char *);
void printInterpreterVersion(RexxInstance *);

int main (int argc, char **argv)
{
    char *packageName = "lessSimpleRW.rex";
    if ( argc == 2 )
    {
        // Nothing fancy here, if there is an argument, then use it as the
        // package file.
        packageName = argv[1];
    }

    // The arguments to RexxCreateInterpreter.  An array of any number of Rexx
    // options can be passed in, but for this example we do not need any
    // options.  So, we use NULL as the options argument.
    RexxInstance *interpreter;
    RexxThreadContext *threadContext;
    RexxOption *options = NULL;

    if ( RexxCreateInterpreter(&interpreter, &threadContext, options) == 0 )
    {
        printf("Failed to create interpreter, aborting.\n");
        exit(1);
    }
    printInterpreterVersion(interpreter);

    // Load the package containing our needed classes and routine.
    // LoadPackage() behaves in essentially the same way as a ::requires
    // directive in Rexx code would.  The objects defined by directives are
    // created and then any code preceding the first directive is executed.
    RexxPackageObject nutshellPkg = threadContext->LoadPackage(packageName);
    if ( nutshellPkg == NULLOBJECT )
    {
        // Some problem.  If there is a condition pending, checkCondition() will
        // print the information.  If there is not a condition pending, then
        // print something here.
        if ( ! checkCondition(threadContext, "LoadPackage()") )
        {
            printf("Failed to load the %s package\n", packageName);
        }
        exit(1);
    }

    // We will locate our routine object in a directory, the package routines
    // directory. The name needs to be upper-case or the routine will not be
    // found. Try it, lower case the name and see the results.
    char *routineName = "STARTWORKERS";

    // Get the routine object we are going to call.
    RexxDirectoryObject pkgRoutines = threadContext->GetPackageRoutines(nutshellPkg);
    RexxRoutineObject nutshellRoutine = (RexxRoutineObject)threadContext->DirectoryAt(pkgRoutines, routineName);
    if ( nutshellRoutine == NULLOBJECT )
    {
        printf("Failed to locate %s routine object.\n", routineName);
        exit(1);
    }

    // Set up the various Rexx objects we will use from this C++ code.

    // Get the .local environment directory.
    RexxDirectoryObject dotLocal = threadContext->GetLocalEnvironment();

    // Get the .worker class object so we can instantiate a third worker object
    // to run from this C++ code.
    RexxClassObject workerCls = threadContext->FindPackageClass(nutshellPkg, "WORKER");
    if ( workerCls == NULLOBJECT )
    {
        printf("Failed to locate the .worker class object.\n");
        exit(1);
    }

    // Create a worker object, and 2 of the 3 args we will pass to the write()
    // method of the worker object. The third arg, the buffer, is retrieved from
    // the .local environment where it is placed by the startWorkers() routine.
    RexxObjectPtr worker = threadContext->SendMessage0(workerCls, "NEW");
    RexxStringObject msg = threadContext->String("from_C++");
    RexxObjectPtr repetions = threadContext->WholeNumber(7);

    // Create a string object to use to change a .local variable.
    RexxStringObject instanceID = threadContext->String("[# 1]");

    // Now start our basic program by calling the routine with the executable
    // code.
    printf("Using interpreter to call  %s\n\n", routineName);
    RexxObjectPtr result = threadContext->CallRoutine(nutshellRoutine, NULL);

    // If the routine raised a condition, print some information about it.
    // Otherwise do some work from this C++ code.
    if (threadContext->CheckCondition())
    {
        checkCondition(threadContext, routineName);
    }
    else
    {
        // I don't do any error checking here.  As long as no one changes the
        // nutshell package code, this should work without errors.

        // Get the fifo buffer, which is the third argument to the write()
        // method, that we need.  Then change the interpreter.info variable in
        // the .local environment.
        RexxObjectPtr fifoBuffer = threadContext->DirectoryAt(dotLocal, "BUFFER");
        threadContext->DirectoryPut(dotLocal, instanceID, "INTERPRETER.INFO");

        // And finally, start our third worker object writing from this C++
        // code.
        RexxArrayObject writeArgs = threadContext->ArrayOfThree(fifoBuffer, msg, repetions);
        threadContext->SendMessage(worker, "WRITE", writeArgs);
    }

    // Be sure the interpreter instance terminates before we end this process.
    // The Terminate() call will wait for all threads to end.
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
 *
 * @return True if there is a pending condition, otherwise false.
 */
bool checkCondition(RexxThreadContext *context, const char *name)
{
    bool haveCondition = false;
    if (context->CheckCondition())
    {
        RexxDirectoryObject cond = context->GetConditionInfo();

        // There is a condition pending, cond should not be null, check anyway.
        if ( cond != NULLOBJECT )
        {
            haveCondition = true;

            RexxCondition condition;
            context->DecodeConditionInfo(cond, &condition);

            printf("\n%s raised an exception:\n", name);
            printf("Error: %d.%03d:  %s\n\n", condition.rc, conditionSubCode(condition),
                   context->CString(condition.message));
        }
        context->ClearCondition();
    }
    return haveCondition;
}
