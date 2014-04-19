/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2014-2014 Rexx Language Association. All rights reserved.    */
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
 * ooConsole.cpp
 *
 * ...
 */

#include "ooConsole.hpp"

#include <stdio.h>
#include <string.h>

#include "APICommon.hpp"
#include "ooShapes.hpp"

// #include <memory>
// #include <new>
// using namespace std;


// Initialized in ooConsoleLoad().
RexxObjectPtr       TheTrueObj        = NULLOBJECT;
RexxObjectPtr       TheFalseObj       = NULLOBJECT;
RexxObjectPtr       TheNilObj         = NULLOBJECT;
RexxObjectPtr       TheZeroObj        = NULLOBJECT;
RexxObjectPtr       TheOneObj         = NULLOBJECT;
RexxObjectPtr       TheTwoObj         = NULLOBJECT;
RexxObjectPtr       TheNegativeOneObj = NULLOBJECT;
RexxObjectPtr       TheZeroPointerObj = NULLOBJECT;
RexxDirectoryObject TheDotLocalObj    = NULLOBJECT;


/**
 * RexxPackageLoader function.
 *
 * The package loader function is called when the library package is first
 * loaded.  This makes it the ideal place for any initialization that must be
 * done prior to the Rexx program start up.  We call sqlite3_initialize() and
 * set up a few global values.
 *
 * @param c  Thread context pointer passed from the interpreter when this package
 *           is loaded.
 *
 * @return Nothing is returned
 */
void RexxEntry ooConsoleLoad(RexxThreadContext *c)
{
    TheTrueObj    = c->True();
    TheFalseObj   = c->False();
    TheNilObj     = c->Nil();
    TheZeroObj    = TheFalseObj;
    TheOneObj     = TheTrueObj;

    TheNegativeOneObj = c->WholeNumber(-1);
    c->RequestGlobalReference(TheNegativeOneObj);

    TheTwoObj = c->WholeNumber(2);
    c->RequestGlobalReference(TheTwoObj);

    TheZeroPointerObj = c->NewPointer(NULL);
    c->RequestGlobalReference(TheZeroPointerObj);

    RexxDirectoryObject local = c->GetLocalEnvironment();
    if ( local != NULLOBJECT )
    {
        TheDotLocalObj = local;

        c->DirectoryPut(local, c->NullString(), "ROUTINEERRORMESSAGE");
    }
    else
    {
        severeErrorException(c, NO_LOCAL_ENVIRONMENT_MSG);
        return;
    }
}

/**
 * RexxPackageUnloader function.
 *
 * The package unloader function is called when the library package is unloaded
 * by the interpreter. The unloading process happens when the last interpreter
 * instance is destroyed during the last cleanup stages.
 *
 * We just do nothing at this time.
 *
 * @param c  Thread context pointer passed from the intepreter when this package
 *           is unloaded.
 *
 * @return Nothing is returned
 */
void RexxEntry ooConsoleUnload(RexxThreadContext *c)
{
    // Purposively do nothing
}




/**
 * Generic function to return either the compact or full version for the
 * ooConsole extension.
 *
 * This is not part of the SQLite library, it is an ooConsole only interface.
 * Used by both the object orientated (.ooConsole) and classic Rexx interfaces.
 *
 * @param c     Thread context we are operating in.
 * @param full  If true, the full version is returned, otherwise the one line
 *              version.
 *
 * @return A formatted string specifying the ooConsole version.
 */
RexxStringObject genGetVersion(RexxThreadContext *c, logical_t full, logical_t minimal)
{
    char   buf[512] = {'\0'};
    size_t bits = 32;

#ifdef __REXX64__
    bits = 64;
#endif

    if ( full )
    {
        char buf1[512];

        snprintf(buf1, sizeof(buf1), "ooConsole Built %s %s\n"
                                     "          Copyright (c) RexxLA %s.\n"
                                     "          All Rights Reserved.\n\n",
                 __DATE__, __TIME__, OOCONSOLE_COPYRIGHT_YEAR);
        strcat(buf, buf1);

        size_t rx = c->InterpreterVersion();

        snprintf(buf1, sizeof(buf1), "Rexx:     Open Object Rexx Version %d.%d.%d\n\n",
                 (rx >> 16) & 0xff, (rx >> 8) & 0xff, rx & 0x0000ff);
        strcat(buf, buf1);

    }
    else
    {
        if ( minimal )
        {
            snprintf(buf, sizeof(buf), "%d.%d.%d.%d\n",
                     OOCONSOLE_VER_MAJOR, OOCONSOLE_VER_MINOR, OOCONSOLE_VER_LEVEL, OOCONSOLE_VER_BUILD);
        }
        else
        {
            snprintf(buf, sizeof(buf), "ooConsole Version %d.%d.%d.%d (%d bit)\n",
                     OOCONSOLE_VER_MAJOR, OOCONSOLE_VER_MINOR, OOCONSOLE_VER_LEVEL, OOCONSOLE_VER_BUILD, bits);
        }
    }

    return c->String(buf);
}


/**
 *  Methods for the .ooConsole class.
 */
#define OOCONSOLE_CLASS    "ooConsole"

static inline pCooConsole ensureCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassInitializationException(c, "ooConsole");
    }
    return (pCooConsole)pCSelf;
}

/**
 * Gets the standard handles for this console.
 *
 * @param c
 * @param pCon
 *
 * @return bool
 */
static bool goodHandles(RexxMethodContext *c, pCooConsole pCon)
{
    pCon->errRC = 0;

    pCon->hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if ( pCon->hStdOut == INVALID_HANDLE_VALUE || pCon->hStdOut == NULL )
    {
        pCon->errRC = GetLastError();
        pCon->isValid = false;
        return false;
    }

    pCon->hStdIn  = GetStdHandle(STD_INPUT_HANDLE);
    if ( pCon->hStdIn == INVALID_HANDLE_VALUE || pCon->hStdIn == NULL )
    {
        pCon->errRC = GetLastError();
        pCon->isValid = false;
        return false;
    }

    pCon->hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    if ( pCon->hStdErr == INVALID_HANDLE_VALUE || pCon->hStdErr == NULL )
    {
        pCon->errRC = GetLastError();
        pCon->isValid = false;
        return false;
    }

    pCon->isValid = true;
    return true;
}

/**
 * Checks both that the console CSelf is not null and that the console is
 * usable.  Usable means that the we are in a character mode window.
 *
 * @param c
 * @param pCSelf
 *
 * @return pCooConn
 */
static pCooConsole requiredConsole(RexxMethodContext *c, void *pCSelf)
{
    pCooConsole pCon = ensureCSelf(c, pCSelf);
    if ( pCon != NULL )
    {
        if ( pCon->isValid )
        {
            if ( pCon->isLongTerm )
            {
                return pCon;
            }
            if ( goodHandles(c, pCon) )
            {
                return pCon;
            }
        }
        pCon = NULL;
    }
    return pCon;
}



/** ooConsole::version  [class method]
 *
 *  @param type  [optional]  The style of the version output returned.  Keywords
 *               are, but only 1st letter is required:
 *
 *                 Compact
 *                 Full
 *                 OneLine
 *
 *               The defualt is OneLine
 */
RexxMethod1(RexxObjectPtr, oocon_version_cls, OPTIONAL_CSTRING, type)
{
    if ( argumentExists(1) )
    {
        switch ( toupper(*type) )
        {
            case 'O' :
                return genGetVersion(context->threadContext, FALSE, FALSE);
                break;

            case 'F' :
                return genGetVersion(context->threadContext, TRUE, FALSE);
                break;

            case 'C' :
                return genGetVersion(context->threadContext, FALSE, TRUE);
                break;

            default :
                wrongArgOptionException(context->threadContext, 1, VALID_VERSION_TYPES, type);
                return TheZeroObj;
                break;
        }
    }

    return genGetVersion(context->threadContext, FALSE, FALSE);
}

/** ooConsole::init()
 *
 *  @param If ... default is true.
 */
RexxMethod2(RexxObjectPtr, oocon_init, OPTIONAL_logical_t, longTerm, OSELF, self)
{
    // Get a buffer for the ooConsole CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooConsole));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooConsole pCon = (pCooConsole)context->BufferData(cselfBuffer);
    memset(pCon, 0, sizeof(CooConsole));

    if ( argumentOmitted(1) )
    {
        longTerm = TRUE;
    }
    pCon->isLongTerm = longTerm ? true : false;

    if ( ! goodHandles(context, pCon) )
    {
        pCon->isValid = false;
        return NULLOBJECT;
    }

    pCon->isValid    = true;

    return TheZeroObj;
}

/** ooConsole::uninit()
 *
 *  At this point, the only purpose of uninit()
 */
RexxMethod1(RexxObjectPtr, oocon_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooConsole pCon = (pCooConsole)pCSelf;

#if OOCONSOLEDBG == 1
        printf("ooConsole::uninit()\n");
#endif
    }
    return TheZeroObj;
}

/** ooConsole::screenBuffersize()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_screenBufferSize, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->hStdOut, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    //char buffer[100];
    //wsprintf(buffer, "%d %d", csbiInfo.dwSize.Y, csbiInfo.dwSize.X);

    pcon->errRC = 0;
    //return context->String(buffer);
    return rxNewSize(context, csbiInfo.dwSize.Y, csbiInfo.dwSize.X);
}


/**
 * Internal use only.
 *
 * Transient method used to test various things as they come up.
 *
 */
RexxMethod1(int, oocon_test, ARGLIST, args)
{
    RexxMethodContext *c = context;

    size_t size = c->ArraySize(args);

    printf("No testes at this time\n");
    return 0;
}




#define ooConsole_Routines_Section

/** screenBuffersize()
 *
 *
 */
RexxRoutine0(RexxObjectPtr, oocon_screenBufferSize_rtn)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if ( hStdOut == INVALID_HANDLE_VALUE || hStdOut == NULL )
    {
        return context->UnsignedInt32(GetLastError());
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(hStdOut, &csbiInfo) == 0 )
    {
        return context->UnsignedInt32(GetLastError());
    }

    char buffer[100];
    wsprintf(buffer, "%d %d", csbiInfo.dwSize.Y, csbiInfo.dwSize.X);
    return context->String(buffer);
}



/** ooconVersion()
 *
 *
 */
RexxRoutine1(RexxStringObject, oocon_version_rtn, OPTIONAL_CSTRING, type)
{
    if ( argumentExists(1) )
    {
        switch ( toupper(*type) )
        {
            case 'O' :
                return genGetVersion(context->threadContext, FALSE, FALSE);

            case 'F' :
                return genGetVersion(context->threadContext, TRUE, FALSE);

            case 'C' :
                return genGetVersion(context->threadContext, FALSE, TRUE);

            default :
                wrongArgOptionException(context->threadContext, 1, VALID_VERSION_TYPES, type);
                return context->String("0");
        }
    }

    return genGetVersion(context->threadContext, FALSE, FALSE);
}

/** ooconTest()
 *
 *  Private routine to use for quick tests.  Code varies as to what it does.
 */
RexxRoutine0(int, oocon_test_rtn)
{
    printf("No tests at this time.");

    return 0;
}




REXX_TYPED_ROUTINE_PROTOTYPE(oocon_screenBufferSize_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oocon_verison_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oocon_test_rtn);

RexxRoutineEntry ooConsole_functions[] =
{
    REXX_TYPED_ROUTINE(oocon_screenBufferSize_rtn, oocon_screenBufferSize_rtn),
    REXX_TYPED_ROUTINE(oocon_version_rtn,          oocon_version_rtn),
    REXX_TYPED_ROUTINE(oocon_test_rtn,             oocon_test_rtn),
    REXX_LAST_ROUTINE()
};


// .ooConsole
REXX_METHOD_PROTOTYPE(oocon_version_cls);

REXX_METHOD_PROTOTYPE(oocon_init);
REXX_METHOD_PROTOTYPE(oocon_uninit);
REXX_METHOD_PROTOTYPE(oocon_screenBufferSize);
REXX_METHOD_PROTOTYPE(oocon_test);

RexxMethodEntry ooConsole_methods[] = {
    REXX_METHOD(oocon_version_cls,                oocon_version_cls),
    REXX_METHOD(oocon_init,                       oocon_init),
    REXX_METHOD(oocon_uninit,                     oocon_uninit),
    REXX_METHOD(oocon_screenBufferSize,           oocon_screenBufferSize),
    REXX_METHOD(oocon_test,                       oocon_test),
    REXX_LAST_METHOD()
};

RexxPackageEntry ooConsole_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_1_0,              // needs at least the 4.1.0 interpreter
    "ooConsole",                         // name of the package
    "0.0.1",                             // package information
    ooConsoleLoad,                       // package load function
    ooConsoleUnload,                     // package unload function
    ooConsole_functions,                 // the exported functions
    ooConsole_methods                    // the exported methods
};

OOREXX_GET_PACKAGE(ooConsole);
