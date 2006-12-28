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
/* REXX AIX Support                                     aixtime.c    */
/*                                                                   */
/*   Subroutine Name:   SysGetCurrentTime                            */
/*                                                                   */
/*   Function:          gets the time and date from the system clock */
/*                                                                   */
/*********************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <pthread.h>
#if defined(OPSYS_SUN)
#include <sched.h>
#endif
#include "RexxCore.h"
#include "SysInterpreter.hpp"
#include "SysSemaphore.hpp"

#ifdef AIX
#include <time.h>
#else
#include <sys/time.h>
#endif

static int dayc[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

void SysInterpreter::getCurrentTime(
  REXXDATETIME *Date )                 /* returned data structure    */
{
//  time_t Tp;                         /* long int for               */
//  time_t *Tpnt = NULL;
//  time_t *Clock;
  struct tm *SystemDate;               /* system date structure ptr  */
  struct timeval tv;
//  Tp = time(Tpnt);                   /* get time long              */
//  Clock = &Tp;
  gettimeofday(&tv, NULL);

#ifdef AIX
  struct tm SD;                        /* system date area           */
  SystemDate = localtime_r((time_t *)&tv.tv_sec, &SD);
#else
  SystemDate = localtime((time_t *)&tv.tv_sec); /* convert           */
#endif

  Date->hours = SystemDate->tm_hour;
  Date->minutes = SystemDate->tm_min;
  Date->seconds = SystemDate->tm_sec;
  Date->hundredths = tv.tv_usec/10000;
  Date->microseconds = tv.tv_usec;
  Date->day = SystemDate->tm_mday;
  Date->month = ++SystemDate->tm_mon;
  Date->year = SystemDate->tm_year + 1900;
  Date->weekday = SystemDate->tm_wday;
  Date->yearday = SystemDate->tm_yday + 1;
//  if (Date->month > 2 && (!(Date->year % 4)) && ((Date->year % 100) || (! (Date->year % 400))))
//    Date->yearday++;
}

typedef struct {
  SysSemaphore *sem;                      /* semaphore to wait on              */
  int32_t time;                        /* timeout value                     */
} ASYNC_TIMER_INFO;

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   async_timer                                  */
/*                                                                   */
/*   Function:          acts as a timer thread. Waits on a timer     */
/*                      semaphore with the given timeout.            */
/*                                                                   */
/*   Arguments:         info - struct which holds the semaphore      */
/*                        handle and the timeout value in msecs.     */
/*********************************************************************/
void* async_timer(void *i)
{
  ASYNC_TIMER_INFO *info = (ASYNC_TIMER_INFO *)i;
                                       /* do wait with apprpriate timeout   */
  info->sem->wait(info->time);
  if (info->sem->posted())             /* if sem not posted ..  */
  {
      info->sem->post();                /* do it                 */
  }
}
/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   alarm_starTimer                              */
/*                                                                   */
/*   Function:          starts an asynchronous, single_interval      */
/*                      timer thread. When the timer pops, it will   */
/*                      post an event semaphore.                     */
/*                                                                   */
/*   Arguments:         alarm timer - time interval before the event */
/*                       semaphore is posted                         */
/*********************************************************************/

RexxMethod2(int, alarm_startTimer,
            int32_t, numdays, int32_t, alarmtime)
{
  SysSemaphore *sem;                   /* Event-semaphore                   */
  uint32_t msecInADay = 86400000;      /* number of milliseconds in a day   */
  RexxObject *cancelObj;               /* place object to check for cancel  */
  RexxNumber cancelVal;                /* value of cancel                   */
  ASYNC_TIMER_INFO tinfo;              /* info for the timer thread         */
  RexxThread timerThread;

  sem = new SysSemaphore;
  sem->create();                        // create the semaphore
                                       /* set the state variables           */
  context->SetObjectVariable("EVENTSEMHANDLE", context->NewPointer(&sem));
  context->SetObjectVariable("TIMERSTARTED", context->TrueObject());
  /* setup the info for the timer thread                                    */
  tinfo.sem = sem;
  tinfo.time = msecInADay;

  //TODO:  needs to be redone....
  while (numdays > 0) {                /* is it some future day?            */

      // INCORRECT!!!!!!!!!!!!!!!!!!!!
      timerThread.createThread((RexxActivity *)async_timer); // create the thread

      sem->wait();                      /* wait for semaphore to be posted   */

    cancelObj = context->GetObjectVariable("CANCELED");
    context->ObjectToNumber(cancelObj, &cancelVal);

    if (cancelVal == 1) {              /* If alarm cancelled?               */
      return 0;                          /* get out                           */
    }
    else {
      sem->reset();                     /* Reset the event semaphore         */
    }
    numdays--;                         /* Decrement number of days          */
  }
  tinfo.sem = sem;                     /* setup the info for timer thread   */
  tinfo.time = alarmtime;

  timerThread.createThread((RexxActivity *)async_timer); // create the thread
  sem->wait();                          /* wait for semaphore to be posted   */
                                       /* clear the semaphore state         */
  context->DropObjectVariable("EVENTSEMHANDLE");
  return 0;
}

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   alarm_stopTimer                              */
/*                                                                   */
/*   Function:          stops an asynchronous timer.                 */
/*                                                                   */
/*   Arguments:         timer - timer handle                         */
/*                      eventSemHandle - event semaphore handle      */
/*                        shared between start & stop timer          */
/*********************************************************************/


RexxMethod0(int, alarm_stopTimer)
{
                                       /* clear the semaphore state         */
  RexxObjectPtr sem = context->GetObjectVariable("EVENTSEMHANDLE");
  if (sem != NULLOBJECT)
  {
      SysSemaphore * hev = (SysSemaphore *)sem;    /* event semaphore handle            */

      hev->post();
  }
  return 0;
}












