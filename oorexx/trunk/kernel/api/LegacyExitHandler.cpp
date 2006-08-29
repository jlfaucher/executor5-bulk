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
/* Implementation of the InterpreterInstance class                            */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#ifndef __REXX64__

#include "ExitHandler.hpp"
#include "rexx.h"
#include "SysInterpreter.hpp"
#include "InternalAPI.hpp"

/**
 * Process a legacy-style exit call.
 *
 * @param activity The current activity.
 * @param function The major function code.
 * @param subfunction
 *                 The minor function code.
 * @param parms    The opaque parameter structure.
 *
 * @return True if the exit was not handled, false otherwise.
 */
bool ExitHandler::legacyCall(RexxActivity *activity, int function, int subfunction, void *parms)
{
     switch(function)
     {
         case RXFNC:   // function call exit, only a single subcode
         {
             return callFunctionExit(activity, (RxFunctionExitParm *)parms);
         }

         case RXCMD:   // subcommand exit, only a single subcode
         {
             return callSubcommandExit(activity, (RxCommandExitParm *)parms);
         }

         case RXMSQ:
         {
             switch(subfunction)
             {
                 case RXMSQPLL:
                 {
                     RxQueuePullParm *ooparms = (RxQueuePullParm *)parms;
                     RXMSQPLL_PARM exit_parm;             /* exit parameters                   */
                     char          retbuffer[DEFRXSTRING];/* Default result buffer             */

                                                          /* Pass along default RXSTRING       */
                     MAKERXSTRING(exit_parm.rxmsq_retc, retbuffer, DEFRXSTRING);
                                                          /* call the handler                  */
                     if (callExit(activity, RXMSQ, RXMSQPLL, (void *)&exit_parm))
                     {
                         return true;                     /* this wasn't handled               */
                     }
                     if (!(exit_parm.rxmsq_retc.strptr))/* if rxstring not null              */
                     {
                         ooparms->queue_item = NULLOBJECT;
                     }
                                                        /* no value returned,                */
                                                        /* return NIL to note empty stack    */
                     else                               /* return resulting object           */
                     {
                         ooparms->queue_item = new_string((RxString *)&exit_parm.rxmsq_retc);
                     }
                                                          /* user give us a new buffer?        */
                     if (exit_parm.rxmsq_retc.strptr != retbuffer)
                     {
                         SysInterpreter::releaseResultMemory(exit_parm.rxmsq_retc.strptr);
                     }
                     return false;                        /* this was handled                  */
                 }
                 case RXMSQPSH:
                 {
                     RxQueuePushParm *ooparms = (RxQueuePushParm *)parms;
                     RXMSQPSH_PARM exit_parm;             /* exit parameters                   */
                     exit_parm.rxmsq_flags.rxfmlifo = ooparms->rxmsq_flags.rxfmlifo;
                     RexxString *queue_item = (RexxString *)ooparms->queue_item;
                                                          /* make into RXSTRING form           */
                     MAKERXSTRING(exit_parm.rxmsq_value, queue_item->getStringData(), queue_item->getLength());
                                                          /* call the handler                  */
                     return callExit(activity, RXMSQ, RXMSQPSH, (void *)&exit_parm);
                 }
                 case RXMSQSIZ:
                 {
                     RxQueueSizeParm *ooparms = (RxQueueSizeParm *)parms;
                     RXMSQSIZ_PARM exit_parm;             /* exit parameters                   */
                                                          /* call the handler                  */
                     if (callExit(activity, RXMSQ, RXMSQSIZ, (void *)&exit_parm))
                     {
                         return true;
                     }
                     // copy the size back
                     ooparms->rxmsq_size = exit_parm.rxmsq_size;
                     return false;

                 }
                 case RXMSQNAM:
                 {
                     RxSetQueueNameParm *ooparms = (RxSetQueueNameParm *)parms;
                     RXMSQNAM_PARM exit_parm;             /* exit parameters                   */
                     char          retbuffer[DEFRXSTRING];/* Default result buffer             */

                     RexxString *name = (RexxString *)ooparms->queue_name;
                                                          /* make into RXSTRING form           */
                     MAKERXSTRING(exit_parm.rxmsq_name, name->getStringData(), name->getLength());
                                                          /* call the handler                  */
                     if (callExit(activity, RXMSQ, RXMSQNAM, (void *)&exit_parm))
                     {
                         return true;
                     }

                     ooparms->queue_name = new_string((RxString *)&exit_parm.rxmsq_name);
                                                          /* user give us a new buffer?        */
                     if (exit_parm.rxmsq_name.strptr != retbuffer)
                     {
                         SysInterpreter::releaseResultMemory(exit_parm.rxmsq_name.strptr);
                     }
                     return false;
                 }
             }
         }
         case RXSIO:
         {
             switch(subfunction)
             {
                 case RXSIOSAY:
                 {
                     RXSIOSAY_PARM exit_parm;             /* exit parameters                   */
                     RxSayParm *ooparms = (RxSayParm *)parms;
                     RexxString *sayoutput = (RexxString *)ooparms->output_string;
                                                          /* make into RXSTRING form           */
                     MAKERXSTRING(exit_parm.rxsio_string, sayoutput->getStringData(),  sayoutput->getLength());
                                                          /* call the handler                  */
                     return callExit(activity, RXSIO, RXSIOSAY, (void *)&exit_parm);

                 }
                 case RXSIOTRC:
                 {
                     RXSIOTRC_PARM exit_parm;             /* exit parameters                   */
                     RxTraceOutputParm *ooparms = (RxTraceOutputParm *)parms;
                     RexxString *output = (RexxString *)ooparms->output_string;
                                                          /* make into RXSTRING form           */
                     MAKERXSTRING(exit_parm.rxsio_string, output->getStringData(),  output->getLength());
                                                          /* call the handler                  */
                     return callExit(activity, RXSIO, RXSIOTRC, (void *)&exit_parm);
                 }
                 case RXSIOTRD:
                 {
                     RxTerminalPullParm *ooparms = (RxTerminalPullParm *)parms;
                     RXSIOTRD_PARM exit_parm;             /* exit parameters                   */
                     char          retbuffer[DEFRXSTRING];/* Default result buffer             */

                     *retbuffer = '\0';
                     MAKERXSTRING(exit_parm.rxsiotrd_retc, retbuffer, 0);
                                                          /* call the handler                  */
                     if (callExit(activity, RXSIO, RXSIOTRD, (void *)&exit_parm))
                     {
                         return true;
                     }
                                                          /* Get input string and return it    */
                     ooparms->input_string = new_string((RxString *)&exit_parm.rxsiotrd_retc);
                                                          /* user give us a new buffer?        */
                     if (exit_parm.rxsiotrd_retc.strptr != retbuffer)
                     {
                         SysInterpreter::releaseResultMemory(exit_parm.rxsiotrd_retc.strptr);
                     }
                     return false;        /* this was handled                  */

                 }
                 case RXSIODTR:
                 {
                     RxDebugReadParm *ooparms = (RxDebugReadParm *)parms;
                     RXSIODTR_PARM exit_parm;             /* exit parameters                   */
                     char          retbuffer[DEFRXSTRING];/* Default result buffer             */

                     *retbuffer = '\0';
                     MAKERXSTRING(exit_parm.rxsiodtr_retc, retbuffer, 0);
                                                          /* call the handler                  */
                     if (callExit(activity, RXSIO, RXSIODTR, (void *)&exit_parm))
                     {
                         return true;
                     }
                                                          /* Get input string and return it    */
                     ooparms->input_string = new_string((RxString *)&exit_parm.rxsiodtr_retc);
                                                          /* user give us a new buffer?        */
                     if (exit_parm.rxsiodtr_retc.strptr != retbuffer)
                     {
                         SysInterpreter::releaseResultMemory(exit_parm.rxsiodtr_retc.strptr);
                     }
                     return false;        /* this was handled                  */
                 }
             }
         }

         case RXHLT:
         {
             switch(subfunction)
             {
                 case RXHLTCLR:
                 {
                     return callExit(activity, RXHLT, RXHLTCLR, NULL);
                 }
                 case RXHLTTST:
                 {
                     RxHaltTestParm *ooparms = (RxHaltTestParm *)parms;
                     RXHLTTST_PARM exit_parm;             /* exit parameters                   */

                                                          /* Clear halt so as not to be sloppy */
                     exit_parm.rxhlt_flags.rxfhhalt = 0;
                                                          /* call the handler                  */
                     if (callExit(activity, RXHLT, RXHLTTST, (void *)&exit_parm))
                     {
                         return true;
                     }

                     // copy the halt test result back to the real one.
                     ooparms->rxhlt_flags.rxfhhalt = exit_parm.rxhlt_flags.rxfhhalt;
                     return false;
                 }
             }
         }

         case RXTRC:     // trace exit, single subcode only
         {
             RxTraceTestParm *ooparms = (RxTraceTestParm *)parms;
             RXTRCTST_PARM exit_parm;             /* exit parameters                   */

                                                  /* Clear Trace bit before  call      */
             exit_parm.rxtrc_flags.rxftrace = 0;
                                                  /* call the handler                  */
             if (callExit(activity, RXTRC, RXTRCTST, (void *)&exit_parm))
             {
                 return true;
             }

             // copy the setting back.
             ooparms->rxtrc_flags.rxftrace = exit_parm.rxtrc_flags.rxftrace;
             return false;
         }

         case RXINI:     // initialization exit, single subcode only
         {
             return callExit(activity, RXINI, RXINIEXT, NULL);
         }

         case RXTER:     // termination exit, single subcode only
         {
             return callExit(activity, RXTER, RXTEREXT, NULL);
         }

         case RXDBG:     // debug exit, single subcode only
         {
             switch(subfunction)
             {
                 case RXDBGTST:
                 {
                     RxDebugTestParm *ooparms = (RxDebugTestParm *)parms;
                     RXDBGTST_PARM exit_parm;             /* exit parameters                   */

                     exit_parm.rxdbg_flags.rxftrace = ooparms->rxdbg_flags.rxftrace;

                     RexxString *filename = (RexxString *)ooparms->filename;
                     MAKERXSTRING(exit_parm.rxdbg_filename, filename->getStringData(), filename->getLength());
                     exit_parm.rxdbg_line = (unsigned long)ooparms->line_number;
                     exit_parm.rxdbg_routine.strlength = 0;
                     exit_parm.rxdbg_routine.strptr = NULL;
                                                          /* call the handler                  */
                     if (callExit(activity, RXDBG, RXDBGTST, (void *)&exit_parm))
                     {
                         return true;
                     }
                     // make sure the flag setting is copied back
                     ooparms->rxdbg_flags.rxftrace = exit_parm.rxdbg_flags.rxftrace;
                     return false;
                 }
                 case RXDBGENTERSUB:
                 case RXDBGLEAVESUB:
                 {
                     RxDebugTestParm *ooparms = (RxDebugTestParm *)parms;
                     RXDBGTST_PARM exit_parm;             /* exit parameters                   */

                     exit_parm.rxdbg_flags.rxftrace = 0;
                     RexxString *filename = (RexxString *)ooparms->filename;
                     MAKERXSTRING(exit_parm.rxdbg_filename, filename->getStringData(), filename->getLength());
                     exit_parm.rxdbg_line = (unsigned long)ooparms->line_number;

                     RexxString *routine = (RexxString *)ooparms->routine_name;
                     MAKERXSTRING(exit_parm.rxdbg_routine, routine->getStringData(), routine->getLength());
                                                          /* call the handler                  */
                     return callExit(activity, RXDBG, subfunction, (void *)&exit_parm);
                 }

                 case RXDBGLOCATELINE:
                 {
                     RxDebugTestParm *ooparms = (RxDebugTestParm *)parms;
                     RXDBGTST_PARM exit_parm;             /* exit parameters                   */

                     exit_parm.rxdbg_flags.rxftrace = 0;
                     RexxString *filename = (RexxString *)ooparms->filename;
                     MAKERXSTRING(exit_parm.rxdbg_filename, filename->getStringData(), filename->getLength());
                     exit_parm.rxdbg_line = (unsigned long)ooparms->line_number;
                     exit_parm.rxdbg_routine.strlength = 0;
                     exit_parm.rxdbg_routine.strptr = NULL;
                                                          /* call the handler                  */
                     return callExit(activity, RXDBG, RXDBGLOCATELINE, (void *)&exit_parm);
                 }

                 case RXDBGSIGNAL:
                 {
                     RXDBGTST_PARM exit_parm;       /* exit parameters                   */

                     exit_parm.rxdbg_flags.rxftrace = 0;
                     exit_parm.rxdbg_filename.strptr = OREF_NULL;
                     exit_parm.rxdbg_line = 0;
                     exit_parm.rxdbg_routine.strptr = OREF_NULL;
                                                          /* call the handler                  */
                     return callExit(activity, RXDBG, RXDBGSIGNAL, (void *)&exit_parm);
                 }
             }
         }
     }
     return true;
}


/**
 * Process a function exit call as a legacy style call.
 *
 * @param ooparms The new style params structure.
 *
 * @return True if the exit was handled, false otherwise.
 */
bool ExitHandler::callFunctionExit(RexxActivity *activity, RxFunctionExitParm *ooparms)
{
    RXFNCCAL_PARM exit_parm;             /* exit parameters                   */
    char          retbuffer[DEFRXSTRING];/* Default result buffer             */

    /* Start building the exit block  */
    exit_parm.rxfnc_flags.rxfferr = 0; /* Initialize error codes to zero */
    exit_parm.rxfnc_flags.rxffnfnd = 0;

    exit_parm.rxfnc_flags.rxffsub = ooparms->rxfnc_flags.rxffsub;
    /* fill in the name parameter        */
    exit_parm.rxfnc_namel = (unsigned short)((RexxString *)ooparms->function_name)->getLength();
    exit_parm.rxfnc_name = (char *)((RexxString *)ooparms->function_name)->getStringData();

    /* Get current active queue name     */
    RexxString *stdqueue = (RexxString *)ooparms->queue_name;
    /* fill in the name                  */
    exit_parm.rxfnc_que = (char *)stdqueue->getStringData();
    /* and the length                    */
    exit_parm.rxfnc_quel = (unsigned short)stdqueue->getLength();
    /* Build arg array of RXSTRINGs      */
    /* get number of args                */
    exit_parm.rxfnc_argc = (unsigned short)ooparms->argument_count;


    /* allocate enough memory for all arguments.           */
    /* At least one item needs to be allocated in order to avoid an error   */
    /* in the sysexithandler!                                               */
    RXSTRING *argrxarray = new RXSTRING[max(exit_parm.rxfnc_argc, 1)];
    if (argrxarray == OREF_NULL)       /* memory error?                     */
    {
        reportException(Error_System_resources);
    }

    RexxObject **arguments = (RexxObject **)ooparms->arguments;
    /* give the arg array pointer        */
    exit_parm.rxfnc_argv = argrxarray;
    /* construct the arg array           */
    for (size_t argindex = 0; argindex < exit_parm.rxfnc_argc; argindex++)
    {
        // classic REXX style interface
        RexxString *temp = (RexxString *)arguments[argindex];
        if (temp != OREF_NULL)
        {         /* got a real argument?              */
                  /* force the argument to a string    */
            temp = (RexxString *)REQUEST_STRING(temp);
            /* point to the string               */
            argrxarray[argindex].strlength = (unsigned long)temp->getLength();
            argrxarray[argindex].strptr = (char *)temp->getStringData();
        }
        else
        {
            /* empty argument                    */
            argrxarray[argindex].strlength = 0;
            argrxarray[argindex].strptr = NULL;
        }
    }
    /* Pass default result RXSTRING      */
    MAKERXSTRING(exit_parm.rxfnc_retc, retbuffer, DEFRXSTRING);

    /* call the handler                  */
    bool wasNotHandled = callExit(activity, RXFNC, RXFNCCAL, (void *)&exit_parm);

    delete argrxarray;

    if (wasNotHandled)
    {
        return TRUE;                     /* this wasn't handled               */
    }

    // copy the flag values
    ooparms->rxfnc_flags.rxfferr = exit_parm.rxfnc_flags.rxfferr;
    ooparms->rxfnc_flags.rxffnfnd = exit_parm.rxfnc_flags.rxffnfnd;

    if (exit_parm.rxfnc_retc.strptr)
    {
        ooparms->return_value = (RexxObjectPtr)new_string((RxString *)&exit_parm.rxfnc_retc);
        /* user give us a new buffer?        */
        if (exit_parm.rxfnc_retc.strptr != retbuffer)
        {
            /* free it                           */
            SysInterpreter::releaseResultMemory(exit_parm.rxfnc_retc.strptr);
        }
    }
    return false;
}

bool ExitHandler::callSubcommandExit(RexxActivity *activity, RxCommandExitParm *ooparms)
{
    RXCMDHST_PARM exit_parm;             /* exit parameters                   */
    char          retbuffer[DEFRXSTRING];/* Default result buffer             */


    exit_parm.rxcmd_flags.rxfcfail = 0;
    exit_parm.rxcmd_flags.rxfcerr = 0;
    RexxString *environment = (RexxString *)ooparms->address_name;
    /* fill in the environment parm      */
    exit_parm.rxcmd_addressl = (unsigned short)environment->getLength();
    exit_parm.rxcmd_address = (char *)environment->getStringData();

    RexxString *cmdname = (RexxString *)ooparms->command_string;
    /* make cmdaname into RXSTRING form  */
    MAKERXSTRING(exit_parm.rxcmd_command, cmdname->getStringData(), cmdname->getLength());

    exit_parm.rxcmd_dll = NULL;        /* Currently no DLL support          */
    exit_parm.rxcmd_dll_len = 0;       /* 0 means .EXE style                */
                                       /* Pass default result RXSTRING      */
    MAKERXSTRING(exit_parm.rxcmd_retc, retbuffer, DEFRXSTRING);

    /* call the handler                  */
    if (callExit(activity, RXCMD, RXCMDHST, (void *)&exit_parm))
    {
        return true;
    }

    ooparms->rxcmd_flags.rxfcfail = exit_parm.rxcmd_flags.rxfcfail;
    ooparms->rxcmd_flags.rxfcerr = exit_parm.rxcmd_flags.rxfcerr;
    /* Get input string and return it    */
    ooparms->return_value = (RexxObjectPtr)new_string((RxString *)&exit_parm.rxcmd_retc);
    /* user give us a new buffer?        */
    if (exit_parm.rxcmd_retc.strptr != retbuffer)
    {
        SysInterpreter::releaseResultMemory(exit_parm.rxcmd_retc.strptr);
    }
    return false;                      /* this was handled                  */
}

/**
 * Call a legacy style exit.
 *
 * @param activity The running activity.
 * @param function The exit function code.
 * @param subfunction
 *                 The exit type subcode.
 * @param parms    The opaque parameter structure.
 *
 * @return True if the exit handled this, false otherwise.
 */
bool ExitHandler::callExit(RexxActivity *activity, int function, int subfunction, void *parms)
{
    ExitContext context;
    // create an exit context for this
    activity->createExitContext(&context);

    RexxExitHandler *handler = (RexxExitHandler *)entryPoint;
                                       /* Exit loaded successfully;  */
                                       /* let it set the return code */
    int result =  (int)(*handler)(function, subfunction, (PEXIT)parms);

    // now destroy this context..., which also reacquires access and checks for raised conditions
    activity->destroyExitContext(&context);
    return result != 0;
}
#endif
