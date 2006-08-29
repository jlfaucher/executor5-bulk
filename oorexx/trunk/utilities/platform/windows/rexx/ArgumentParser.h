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

#include "SysProcess.hpp"

char * nextArgument(bool getprog, char * argptr, size_t *ndx, size_t *len, bool allocate, size_t maxarglength)
{
    char *tmp;
    char *ret;
    if (argptr[*ndx] == ' ')                      /* skip blanks of previous argument */
    {
        while ((argptr[*ndx] == ' ') && argptr[*ndx] && (*ndx<maxarglength))
        {
            (*ndx)++;
        }
    }
    *len = 0;
    tmp = &argptr[*ndx];

    if (!allocate)
    {
        while ((argptr[*ndx] != ' ') && argptr[*ndx] && (*ndx<maxarglength))
        {
            if (argptr[*ndx] == '\"')
            {
                do
                {
                    if (argptr[*ndx] != '\"') (*len)++;
                    (*ndx)++;
                } while ((argptr[*ndx] != '\"') && argptr[*ndx] && (*ndx<maxarglength));
            }
            if (argptr[*ndx])
            {
                if (argptr[*ndx] != '\"')
                {
                    (*len)++;
                }
                (*ndx)++;
            }
        }
    }
    /* get the program name for REXXHIDE */
    else if (getprog)
    {
        if (argptr[*ndx] == '\"')
        {
            do
            {
                (*len)++;(*ndx)++;
            } while ((argptr[*ndx] != '\"') && argptr[*ndx] && (*ndx<maxarglength));
        }
        while (argptr[*ndx] && (argptr[*ndx] != ' ') && (*ndx<maxarglength))
        {
            (*len)++;
            (*ndx)++;
        }
    }
    else
    {
        while (argptr[*ndx] && (*ndx<maxarglength))
        {
            (*len)++;
            (*ndx)++;
        }
    }

    if (*len)
    {
        if (allocate)
        {
            /* program name must not be enclosed within "" for REXXHIDE */
            if (getprog && (tmp[0] == '\"'))
            {
                tmp++;
                (*len)-=2;
            }

            ret = (char *)ooRexxAllocateMemory((*len)+1);
            memcpy(ret, tmp, (*len)+1);
            if (getprog)
            {
                ret[*len]='\0';
            }
            return ret;
        }
        else
        {
            return tmp;
        }
    }
    else
    {
        return NULL;
    }
}



RxString *getArguments(char * *program, char * argptr, size_t *count, RxString *retarr)
{
    size_t i, isave, len, maxarglen;

    /* don't forget the break after program_name */

    /* WindowsNT accepts 2048 bytes, Windows95/98 1024 bytes */
    if (SysProcess::runningNT())
    {
        maxarglen=2048;
    }
    else
    {
        maxarglen=1024;
    }

    i = 0;
    if (program)
    {
        (*program) = nextArgument(true, argptr, &i, &len, true, maxarglen);  /* for REXXHIDE script is first argument */
    }
    else
    {
        nextArgument(false, argptr, &i, &len, false, maxarglen);    /* skip REXX.EXE */
        nextArgument(false, argptr, &i, &len, false, maxarglen);    /* skip REXX script */
    }

    retarr->strptr = NULL;
    isave = i;
    *count = 0;
    if (nextArgument(false, argptr, &i, &len, false, maxarglen)) (*count)++;

    if (*count)
    {
        i = isave;
        retarr->strptr = nextArgument(false, argptr, &i, &len, true, maxarglen);
        retarr->strlength = len;
    }
    return retarr;
}


void freeArguments(char * program, RxString *arguments)
{
    if (arguments->strptr)
    {
        ooRexxFreeMemory(arguments->strptr);
    }
    if (program)
    {
        ooRexxFreeMemory(program);
    }
}
