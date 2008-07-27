/*----------------------------------------------------------------------------*/
/*                                                                            */
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

#include "SysUtil.hpp"
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

/**
 * Portable implementation of an ascii-z caseless string compare.
 *
 * @param opt1   First string argument
 * @param opt2   Second string argument.
 *
 * @return The compare result.  Returns 0, negative, or positive depending
 *         one the ordering compare result.
 */
int SysUtil::stricmp(const char *op1, const char *op2)
{
    while (tolower(*op1) == tolower(*op2))
    {
        if (*op1 == 0)
        {
            return 0;
        }
        op1++;
        op2++;
    }

    return(tolower(*op1) - tolower(*op2));
}

/**
 * Portable implementation of a caseless memory compare.
 *
 * @param opt1   First memory location to compare.
 * @param opt2   Second memory location.
 * @param len    Length to compare.
 *
 * @return The compare result.  Returns 0, negative, or positive depending
 *         one the ordering compare result.
 */
int SysUtil::memicmp(void *mem1, void *mem2, size_t len)
{
    char *op1 = (char *)mem1;
    char *op2 = (char *)mem2;
    while(len != 0)
    {
        if (tolower(*((char *)op1)) != tolower(*((char *)op2)))
        {
            return tolower(*((char *)op1)) - tolower(*((char *)op2));

        }
        op1++;
        op2++;
        len--;
    }
    return 0;

}

/**
 * Portable implementation of an ascii-z string to uppercase (in place).
 *
 * @param str    String argument
 *
 * @return The address of the str unput argument.
 */
void SysUtil::strupper(char *str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }

    return;
}


/**
 * Portable implementation of an ascii-z string to uppercase (in place).
 *
 * @param str    String argument
 *
 * @return The address of the str unput argument.
 */
void SysUtil::strlower(char *str)
{
    while (*str)
    {
        *str = tolower(*str);
        str++;
    }

    return;
}
