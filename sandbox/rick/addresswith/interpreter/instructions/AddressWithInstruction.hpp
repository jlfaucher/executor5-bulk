/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2018 Rexx Language Association. All rights reserved.    */
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
/******************************************************************************/
/* REXX Kernel                                  AddressWithInstruction.hpp    */
/*                                                                            */
/* ADDRESS WITH instruction Class Definitions                                 */
/*                                                                            */
/******************************************************************************/
#ifndef Included_InstructionAddressWith
#define Included_InstructionAddressWith

#include "RexxInstruction.hpp"

class RexxInstructionAddress : public RexxInstruction
{
 friend class LanguageParser;
 public:
    inline void operator delete(void *) { }

    // the different types for doing ADDRESS WITH
    enum
    {
        STEM_VARIABLE,
        STREAM_NAME,
        DYNAMIC
    } RedirectionType;

    // the options for how how output is handled
    enum
    {
        APPEND,
        REPLACE
    } OutputOption;


    RexxInstructionAddressWith(RexxString *, RexxInternalObject *);
    inline RexxInstructionAddressWith(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(MarkReason reason);
    virtual void flatten(Envelope *);

    virtual void execute(RexxActivation *, ExpressionStack *);

    RexxString *environment;                 // An environment string (static form)
    RexxInternalObject *command;             // A command expression
    RexxObject *inputSource;                 // The input source expression
    RexxObject *outputTarget;                // The output target expression
    RexxObject *errorTarget;                 // The output target expression
    RedirectionType inputType;               // The type of redirection target to evaluate
    RedirectionType outputType;              // The output redirection type
    RedirectionType errorType;               // The error redirection type
    OutputOption outputOption;               // option for the output stream
    OutputOption errorOption;                // the option for the error stream
};
#endif

