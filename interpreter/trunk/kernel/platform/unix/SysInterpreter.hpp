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
/*****************************************************************************/
/* Rexx Support                                                              */
/*                                                                           */
/* System interpreter control.  This is the preferred location for all       */
/* system specific global variables, as well as specialized.  Processing     */
/* functions.  The interpreter does not instantiate an instance of this      */
/* class, so most variables and methods should be static.  Each platform     */
/* needs to provide at least a placeholder implementation of this class.     */
/*                                                                           */
/*****************************************************************************/

#ifndef SysInterpreter_DEFINED
#define SysInterpreter_DEFINED

class RexxActivation;
class RexxBuffer;

class SysInterpreter
{
public:
    inline static void timeSliceElapsed()
    {
        timerElapsed = true;               // just set the flag
    }

    inline static bool hasTimeSliceElapsed()
    {
        return timerElapsed;
    }

    static void startTimeSlice();
    static void *allocateResultMemory(size_t size);
    static void  releaseResultMemory(void *mem);
    static bool        savingPrograms();
    static RexxString *messageText(wholenumber_t code);
    static RexxString *getSystemName();
    static RexxString *getVersion();
    static RexxString *getUserid();
    static RexxString *resolveProgramName(RexxString * name, RexxString * parent);
    static RexxObject *valueFunction(RexxString *variable, RexxObject *newValue, RexxString *selector);
    static RexxString *qualifyFileSystemName(RexxString *name);
    static void        shutdown();
    static inline void initializeInterpreter() { ; }
    static void shutdownEnvironment();
    static void restoreEnvironment(void *);
    static void loadImage(char *name, char *&imageBuffer, size_t &size);
    static RexxString *locateProgram(RexxString *name, const char *extensions[], size_t count);
    static RexxObject *getProcessName();
    static RexxString *getInitialAddressForFile(RexxString * name);
    static RexxString *getDefaultCommandEnvironment();
    static bool        accessMemoryPool(MemorySegmentPool *&pool);
    static void programSetup(RexxActivation *);
    static inline void clauseBoundary(RexxActivation *) {;}
    static void getCurrentTime(REXXDATETIME *Date);
    static RexxObject *executeSystemCommand(RexxActivity *activity, RexxString *current_address, RexxString *command, RexxString **error_failure);
    static RexxBuffer *readProgram(RexxString *file_name, ProtectedObject &source);
    static inline bool isRunning() { return timerOwner != 0; }
    static void stopTimeSlice();
    static bool setProcessMessages(bool onoff);
    static bool SysInterpreter::processBreakEvent(int type);
    static bool processSetCommand(char * cmd, int* rc);
    static bool processCdCommand(char * cmd, int *rc);
    static int  windowsCommand(SysActivity *myActivity, char *cmd, RexxString **error_failure);
    static int  callCommandShell(SysActivity * myActivity, char *cmdstring_ptr, RexxString  **error_failure, bool direct, bool explicitConsole);
    static void termination();
    static void initialize();
    static void relinquish();
    static void initializeProcessEnvironment(int module);
    static void terminateProcessEnvironment();
    static RexxObject *pushEnvironment(RexxActivation *);
    static RexxObject *popEnvironment(RexxActivation *);
    static const char *metaVersion;

protected:
    static bool handleExceptions;         // Global switch for Exception Handling
    static bool timerElapsed;             // time slice checker
    static int resourceBundle;            // handle to dll containing string resources
    static bool running;                  // the interpreter is still active
    static int timerOwner;                // the timer thread
    static bool signaled;                 // already handled a signal event
};

#endif

