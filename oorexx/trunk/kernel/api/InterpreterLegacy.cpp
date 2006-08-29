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
/* REXX API support                                                           */
/*                                                                            */
/* Legacy 32-bit based APIs.                                                  */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"                    /* bring in global defines           */
#include "rexx.h"
#include "RexxActivity.hpp"

/******************************************************************************/
/* Name:       InternSetResetTrace                                            */
/*                                                                            */
/* Arguments:  procid - Process id of target REXX procedure                   */
/*             threadid - Thread id of target REXX procedure                  */
/*             flag - set trace on (1) or off (0) ?                           */
/*                                                                            */
/* Returned:   rc - RXARI_OK (halt condition set)                             */
/*                  RXARI_NOT_FOUND (couldn't find threadid)                  */
/*                                                                            */
/******************************************************************************/

RexxReturnCode internSetResetTrace(process_id_t procid, thread_id_t threadid, bool flag)
{
    RexxThread t(threadid);             /* create thread information      */
    return RexxActivity::setTrace(t, flag) ? RXARI_OK : RXARI_NOT_FOUND;
}


/******************************************************************************/
/* Name:       RexxSetTrace                                                   */
/*                                                                            */
/* Arguments:  procid - Process id of target REXX procedure                   */
/*             threadid - Thread id of target REXX procedure                  */
/*                                                                            */
/* Returned:   rc - RXARI_OK (halt condition set)                             */
/*                  RXARI_NOT_FOUND (couldn't find threadid)                  */
/*                                                                            */
/* Notes:      Sends request to the activity to turn on interactive trace in  */
/*             the target activation.                                         */
/******************************************************************************/
APIRET APIENTRY RexxSetTrace(process_id_t procid, thread_id_t threadid)
{
  return (APIRET)(internSetResetTrace(procid, threadid, true));     /* 1 to set trace on */
}


/******************************************************************************/
/* Name:       RexxResetTrace                                                 */
/*                                                                            */
/* Arguments:  procid - Process id of target REXX procedure                   */
/*             threadid - Thread id of target REXX procedure                  */
/*                                                                            */
/* Returned:   rc - RXARI_OK (halt condition set)                             */
/*                  RXARI_NOT_FOUND (couldn't find threadid)                  */
/*                                                                            */
/* Notes:      Sends request to the activity to turn off interactive trace in */
/*             the target activation.                                         */
/******************************************************************************/
APIRET APIENTRY RexxResetTrace(process_id_t procid, thread_id_t threadid)
{
  return (APIRET)(internSetResetTrace(procid, threadid, false));  /* 0 to set trace off */
}

/******************************************************************************/
/* Name:       RexxSetHalt                                                    */
/*                                                                            */
/* Arguments:  procid - Process id of target REXX procedure                   */
/*             threadid - Thread id of target REXX procedure                  */
/*                                                                            */
/* Returned:   rc - RXARI_OK (halt condition set)                             */
/*                  RXARI_NOT_FOUND (couldn't find threadid)                  */
/*                                                                            */
/* Notes:      Sends request to the activity to flip on the halt flag in the  */
/*             target activation.                                             */
/******************************************************************************/
APIRET APIENTRY RexxSetHalt(process_id_t procid, thread_id_t threadid)
{
    RexxThread t(threadid);             /* create thread information      */
    return RexxActivity::halt(t, OREF_NULL) ? RXARI_OK : RXARI_NOT_FOUND;
}
