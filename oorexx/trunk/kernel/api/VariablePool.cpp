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
/*                                                                            */
/* Rexx variable pool API support                                             */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"                       /* Object REXX kernel declares    */
#include "oorexx.h"
#include "Interpreter.hpp"
#include "ContextApi.hpp"

RexxBoolean stemSortStub(RexxNativeActivation *context, RexxStringPointer stemname, int order, int type,
    stringsize_t start, stringsize_t end, stringsize_t firstcol, stringsize_t lastcol)
{
    // and get a retriever for this variable
    RexxStemVariable *retriever = (RexxStemVariable *)context->getVariableRetriever((stringchar_t *)stemname);

    /* this must be a stem variable in order for the sorting to work. */

    if ( (!isOfClass(StemVariable, retriever)) && (!isOfClass(CompoundVariable, retriever)))
    {
        return FALSE;
    }

    RexxString *tail = OREF_NULLSTRING ;

    // if this is a compound retriever, then we need to prefix the stem.
    if (isOfClass(CompoundVariable, retriever))
    {
        // scan to the first period...that will define the stem.
        stringsize_t position = 0;

        while (stemname[position] != '.')
        {
            position++;                      /* step to the next character        */
        }
        tail = new_upper_string(stemname + position + 1);
    }


    return retriever->sort(context->getContext(), tail, order, type, start, end, firstcol, lastcol);
}

BEGIN_EXTERN_C()

/******************************************************************************/
/* RexxStemSort - 32-bit undocumented API to sort stem elements.              */
/*                                                                            */
/* Input:                                                                     */
/*   stemname - an ASCII-Z stem name.                                         */
/*   order    - the sort order                                                */
/*   type     - The type of sort                                              */
/*   start    - index of the first element to sort                            */
/*   end      - index of the last element to sort                             */
/*   firstcol - first column position to sort                                 */
/*   lastcol -  last column position to sort                                  */
/*                                                                            */
/* Output:                                                                    */
/*   true if the sort succeeded, false for any parameter errors.              */
/******************************************************************************/
RexxReturnCode RexxEntry ooRexxStemSort(RexxCallContext *c, char *stemname, int order, int type,
    size_t start, size_t end, size_t firstcol, size_t lastcol)

{
    ApiContext context(c);
    try
    {
        return stemSortStub(context.context, stemname, order, type, start, end, firstcol, lastcol);
    }
    catch (ActivityException)
    {
        context.context->clearException();
    }
    return FALSE;
}

END_EXTERN_C()


/******************************************************************************/
/* RexxVariablePool - documented API to access REXX variable pools.           */
/*                                                                            */
/* Input:                                                                     */
/*   pshvblock - Pointer to one or more variable request blocks.              */
/*                                                                            */
/* Output:                                                                    */
/*   rc - Composite return code for all request blocks (individual rc's are   */
/*        set within the shvret fields of each request block).                */
/******************************************************************************/
RexxReturnCode RexxEntry ooRexxVariablePool(VariableRequest *request)
{
    // get a call context for this
    RexxCallContext *c = RexxInterpreter::getCurrentCallContext();
    if (c == NULL)
    {
        // somehow, this call is being made at the wrong time
        return RXSHV_NOAVL;
    }
    ApiContext context(c);
    // the variable pool interface handles its own try/catches.
    return context.context->variablePoolInterface(request);
}


#ifndef __REXX64__
#include "rexx.h"


APIRET APIENTRY RexxVariablePool(SHVBLOCK *pshvblock)
{
    // get a call context for this
    RexxCallContext *c = RexxInterpreter::getCurrentCallContext();
    if (c == NULL)
    {
        // somehow, this call is being made at the wrong time
        return RXSHV_NOAVL;
    }
    ApiContext context(c);

    APIRET retcode = 0;          /* initialize composite rc           */

    try
    {
        while (pshvblock)
        {                   /* while more request blocks         */
            VariableRequest request;

            // copy old information into the request block
            request.shvnext = NULL;
            request.shvname.strptr = pshvblock->shvname.strptr;
            request.shvname.strlength = pshvblock->shvname.strlength;
            request.shvvalue.strptr = pshvblock->shvvalue.strptr;
            request.shvvalue.strlength = pshvblock->shvvalue.strlength;
            request.shvcode = pshvblock->shvcode;

            context.context->variablePoolRequest(&request);

            // copy the updated information back.
            pshvblock->shvret = (unsigned char)request.shvret;
            pshvblock->shvnamelen = (unsigned long)request.shvnamelen;
            pshvblock->shvvaluelen = (unsigned long)request.shvvaluelen;
            pshvblock->shvname.strptr = request.shvname.strptr;
            pshvblock->shvname.strlength = (unsigned long)request.shvname.strlength;
            pshvblock->shvvalue.strptr = request.shvvalue.strptr;
            pshvblock->shvvalue.strlength = (unsigned long)request.shvvalue.strlength;

            retcode |= pshvblock->shvret;        /* accumulate the return code        */
            pshvblock = pshvblock->shvnext;      /* step to the next block            */
        }
        return retcode;                       /* return composite return code      */

    }
    // intercept any termination failures
    catch (ActivityException)
    {
        /* set failure in current            */
        pshvblock->shvret |= (uint8_t)RXSHV_MEMFL;
        retcode |= pshvblock->shvret;       /* OR with the composite             */
        return retcode;                     /* stop processing requests now      */
    }
}

#endif
