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
/*                                                                   */
/*   Subroutine Name:   SysGetCurrentTime                            */
/*                                                                   */
/*   Function:          gets the time and date from the system clock */
/*********************************************************************/

#include "RexxCore.h"
#include "IntegerClass.hpp"
#include "SysInterpreter.hpp"
#include "Interpreter.hpp"

void CALLBACK timerProc(HWND, UINT, UINT, DWORD);
void CALLBACK alarmTimerProc(HWND, UINT, UINT, DWORD);

static int dayc[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

#define TIMESLICE_STACKSIZE 2048
#define TIMESLICEMS 10
#define ALARMSLEEP 330

void SysInterpreter::getCurrentTime(
  REXXDATETIME *Date )                 /* returned data structure    */
/*********************************************************************/
/* Function:  Return a time stamp to the kernel date/time functions. */
/*********************************************************************/
{
  SYSTEMTIME SystemDate; /* system date structure      */

  GetLocalTime(&SystemDate);        /* via Windows                */

  Date->hours = SystemDate.wHour;
  Date->minutes = SystemDate.wMinute;
  Date->seconds = SystemDate.wSecond;
  Date->hundredths = SystemDate.wMilliseconds / 10;
  Date->microseconds = SystemDate.wMilliseconds * 1000;
  Date->day = SystemDate.wDay;
  Date->month = SystemDate.wMonth;
  Date->year = SystemDate.wYear;
  Date->weekday = SystemDate.wDayOfWeek;
  Date->yearday = (uint16_t)(dayc[Date->month - 1] + Date->day);
  if (Date->month > 2 && (!(Date->year % 4)) && ((Date->year % 100) || (! (Date->year % 400))))
    Date->yearday++;                   /* adjust for leap year       */
}

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   SysTimeSliceElapsed                          */
/*                                                                   */
/*     Used to see if iClauseCounter == CLAUSESPERYIELD              */
/*     This is used in windows as the duration of a time slice       */
/*                                                                   */
/*********************************************************************/

DWORD WINAPI timeSliceControl(void * args)
{
#ifdef TIMESLICE
   do
   {
      Sleep(TIMESLICEMS);
      // make sure we process the message loop, if necessary
      SysProcess::relinquish();
   } while (SysInterpreter::isRunning());
#endif
   return 0;
}

void SysInterpreter::startTimeSlice()
/******************************************************************************/
/* Function:  Make sure we ahve a Timer running and reset TimeSlice Sem       */
/******************************************************************************/
{
#ifdef TIMESLICE
   DWORD thread;
   if (!timerOwner != 0)            /* Is there a timer?         */
   {
       /* create a thread with low priority to check the message queue for WM_TIMER */

       timerOwner = CreateThread(NULL, TIMESLICE_STACKSIZE, timeSliceControl, NULL, 0, &thread);
       SetThreadPriority(timerOwner, THREAD_PRIORITY_NORMAL+1);  /* set a higher priority */
   }
#endif
}


void SysInterpreter::stopTimeSlice()
/******************************************************************************/
/* Function:  Stop the thread that examines the message queue for WM_TIMER    */
/******************************************************************************/
{
#ifdef TIMESLICE
   TerminateThread(timerOwner, 0);
   timerOwner = 0;
#endif
}


// control the use of the message loop for relinquishing control
bool SysInterpreter::setProcessMessages(bool onoff)
/******************************************************************************/
/* Function:  Control the use of the message loop for process control         */
/******************************************************************************/
{
   bool old;
   old = SysProcess::useMessageLoop;
   SysProcess::useMessageLoop = onoff;
   return old;
}

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   alarm_starTimer                              */
/*                                                                   */
/*   Function:          starts an asynchronous, single_interval      */
/*                      timer. When the timer pops, it will post an  */
/*                      event semaphore.                             */
/*                                                                   */
/*   Arguments:         alarm timer - time interval before the event */
/*                       semaphore is posted                         */
/*********************************************************************/
RexxMethod2(int, alarm_startTimer,
    RexxNumber, numdays,
    RexxNumber, alarmtime)
{
    /* Time-out value                    */
    DWORD ulTimeout = INFINITE;
    DWORD msecInADay = 86400000;         /* number of milliseconds in a day   */
    DWORD timerIntervall = 60000;        /* resolution 1 min */
    RexxObjectPtr cancelObj;             /* place object to check for cancel  */
    MSG msg;                             /* to retrieve WM_TIMER */
    HANDLE semHandle = 0;                /* Event-semaphore handle repository */
    UINT_PTR timerHandle = 0;            /* Timer handle                      */
    DWORD rc;
    /* Create an event semaphore to be   */
    /* posted by the timer               */
    semHandle = CreateEvent(NULL, 1, 0, NULL);
    // get a system problem
    if (semHandle == NULL)
    {
        context->RaiseException(Rexx_Error_System_service);
        return 0;
    }

    /* set the state variables           */
    context->SetObjectVariable("EVENTSEMHANDLE", context->NewPointer(semHandle));
    context->SetObjectVariable("TIMERSTARTED", context->TrueObject());

    while (numdays > 0)
    {                /* is it some future day?            */

                     /* start timer to wake up after a day*/
        timerHandle = SetTimer(NULL, 0, msecInADay, NULL);
        if (timerHandle == 0)
        {            /* Error received?          */
            context->RaiseException(Rexx_Error_System_service);
            return 0;
        }

        /* wait for WM_TIMER message */
        if (SysProcess::useMessageLoop)  /* without peekmessage for VAC++ */
        {
            do
            {
                // message structure
                if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg); // Translates virtual key codes
                    DispatchMessage(&msg);  // Dispatches message to window
                }
                else
                {
                    Sleep(ALARMSLEEP);
                }
            }
            while ((msg.message != WM_TIMER) && (WaitForSingleObject(semHandle, 0) != WAIT_OBJECT_0));
        }
        else
        {
            WaitForSingleObject(semHandle, INFINITE);
        }

        SetEvent(semHandle);

        /* get the cancel state              */
        cancelObj = context->GetObjectVariable("CANCELED");

        if (cancelObj == context->TrueObject())
        {              /* If alarm cancelled?               */
            rc = KillTimer(NULL, timerHandle);      /* stop timer                        */
            /* Close the event semaphore         */
            rc = CloseHandle(semHandle);
            if (rc == false)
            {                   /* Error received?                   */
                context->RaiseException(Rexx_Error_System_service);
                return 0;
            }
            return 0;
        }
        else
        {
            /* Reset the event semaphore         */
            rc = SetEvent(semHandle);
            if (rc == false)
            {                   /* Error received?                   */
                context->RaiseException(Rexx_Error_System_service);
                return 0;
            }
        }
        numdays--;                         /* Decrement number of days          */
    }

    /* start the timer                   */
    timerHandle = SetTimer(NULL, 0, (UINT)alarmtime, NULL);
    if (!timerHandle)
    {                       /* Error received?                   */
        context->RaiseException(Rexx_Error_System_service);
        return 0;
    }

    /* wait for WM_TIMER message */
    if (SysProcess::useMessageLoop)  /* without peekmessage for VAC++ */
    {
        do
        {
            if (PeekMessage (&msg,   // message structure
                NULL,                  // handle of window receiving the message
                0,                     // lowest message to examine
                0,
                PM_REMOVE))            // highest message to examine
            {
                TranslateMessage(&msg);// Translates virtual key codes
                DispatchMessage(&msg); // Dispatches message to window
            }
            else Sleep(ALARMSLEEP);

        }
        while ((msg.message != WM_TIMER) && (WaitForSingleObject(semHandle, 0) != WAIT_OBJECT_0));
    }
    else
    {
        WaitForSingleObject(semHandle, INFINITE);
    }

    SetEvent(semHandle);

    /* Since the timer may have          */
    /* already popped.                   */
    rc = KillTimer(NULL, timerHandle);          /* stop timer                        */
    if (rc == false)
    {                       /* Error received?                   */
        context->RaiseException(Rexx_Error_System_service);
        return 0;
    }
    /* Close the event semaphore         */
    rc = CloseHandle(semHandle);
    if (rc == false)
    {                       /* Error received?                   */
        context->RaiseException(Rexx_Error_System_service);
        return 0;
    }
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
/*                        shared between start & stop timer          */
/*********************************************************************/


RexxMethod0(int, alarm_stopTimer)
{
                                       /* clear the semaphore state         */
  RexxObjectPtr sem = context->GetObjectVariable("EVENTSEMHANDLE");
  if (sem != NULLOBJECT)
  {
      DWORD  rc;                           /* return code                       */
      HANDLE hev = (HANDLE)context->PointerValue(sem);    /* event semaphore handle            */

      rc = SetEvent(hev);
      if (rc == false) {                   /* Error received?                   */
          context->RaiseException(Rexx_Error_System_service);
          return 0;
      }
  }
  return 0;
}



