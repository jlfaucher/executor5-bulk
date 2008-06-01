/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

/** Very simplistic command line interface to MoniorWindows.
 *
 *    MonitorWindows runs as a hidden window.  This command line interface can
 *    be used to bring MonitorWindows to the screen.  It can also be used to
 *    start the program in the background.  Note that when started in the
 *    background there is no console output.
 *
 *    The interface is intended to support a number of commands, thear are not
 *    all implemented.  The argumements are:
 *        /show    /query
 *        /start   /report
 *        /stop    /save
 */

cmdLine = arg(1)

  if cmdLine~words \== 1 then return doHelp()

  cmd = getCommand(cmdLine)
  if cmd == .nil then return doHelp()

  if monitorIsRunning() then hwnd = readMonitorHandle()
  else hwnd = 0

  select
    when cmd == "SHOW" then do
      if hwnd == 0 then do
        say 'MonitorWindows is not running.  Use the /start command.'
        return 0
      end

      ret = SendWinMsg("ANY", hwnd, .WM_USER_MONITORSHOW, 400, 345)
    end

    when cmd == 'START' then do
      if hwnd \== 0 then do
        say 'MonitorWindows is already running.  Use the /show command.'
        return 0
      end

      pid = RxWinExec("rexxhide MonitorWindows.rex monitorConsole.log")
      if pid > 31 then do
        say 'Started MonitorWindows.  PID:' pid
      end
      else do
        say 'MonitorWindows may not have started.  PID:' pid
      end
    end

    when cmd == 'BRINGUP' then do
      pid = 0
      if hwnd == 0 then do
        pid = RxWinExec("rexxhide MonitorWindows.rex monitorConsole.log")
        if pid > 31 then do
          say 'Started MonitorWindows.  PID:' pid
        end
        else do
          say 'MonitorWindows could not be started.  PID:' pid
          return pid
        end

        -- If MonitorWindows was started, it takes the OS a finite amount of
        -- time to get the window created and working.  If there is no pause,
        -- the window message is sent before the application is ready to respond
        -- to messages.
        if pid <> 0 then j = SysSleep(.3)

        hwnd = readMonitorHandle()
      end

      ret = SendWinMsg("ANY", hwnd, .WM_USER_MONITORSHOW, 400, 345)
    end

    when cmd == 'STOP' then do
      if hwnd == 0 then do
        say 'MonitorWindows is not running.'
        return 0
      end

      ret = SendWinMsg("ANY", hwnd, .WM_USER_MONITOR_CMD, .MONITOR_CLOSE, 0)
    end

    when cmd == 'REPORT' then do
      if hwnd == 0 then do
        say 'MonitorWindows is not running.'
        return 0
      end

      ret = SendWinMsg("ANY", hwnd, .WM_USER_MONITOR_CMD, .MONITOR_REPORT, 0)
    end

    when cmd == 'SAVE' then do
      if hwnd == 0 then do
        say 'MonitorWindows is not running.'
        return 0
      end

      ret = SendWinMsg("ANY", hwnd, .WM_USER_MONITOR_CMD, .MONITOR_SAVE, .SAVE_STATS)
    end

    when cmd == "QUERY" then do
      if hwnd \== 0 then say "MonitorWindows is running."
      else say "MonitorWindows is not running."
    end

    otherwise do
      say cmd 'is not a valid command.'
      return doHelp()
    end
  end
  -- End select


::requires 'activatedWindows.frm'

::routine doHelp public

  say 'manage.rex is a command line interface to MonitorWindows'
  say 'Usage: manage /cmd'
  say
  say 'Recognized commands are:'
  say '  /start    Starts MonitorWindows.'
  say '  /show     Brings a running MonitorWindows to the screen.'
  say '  /bringup  Starts and displays the MonitorWindows main window.'
  say '  /query    Dispalys if MonitorWindows is running or not.'
  say '  /report   Shows the report screen.'
  say '  /stop     Halts MonitorWindows. (Note that nothing is saved.)'
  say
  say 'Case is not significant.'
  say
return 0

::routine getCommand
  use arg cmdLine

  goodCmds = .set~of("SHOW", "START", "STOP", "QUERY", "REPORT", "SAVE",   -
                     "BRINGUP")

  cmd = cmdLine~word(1)~upper
  if cmd~left(1) \== '/' then return .nil

  cmd = cmd~strip('L', '/')

  if goodCmds~hasIndex(cmd) then return cmd

return .nil
