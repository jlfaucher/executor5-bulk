/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2024-2026 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
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

--------------------------------------------------------------------------------
-- Orchestrator: enumerates test cases, runs each one via test_yaml-single.rex --
-- with a timeout, and collects results. Writes a log file that is flushed    --
-- after every test so we know where a hang occurred.                          --
--------------------------------------------------------------------------------

  Parse Arg args

  -- Defaults
  suitePath  = ""
  verbose    = .false
  doJson     = .false
  showFails  = .false
  onlyId     = ""
  timeoutSec = 10
  logFile    = "test-suite-results.log"

  -- Parse arguments
  Do While args \== ""
    Parse Var args option args
    Select Case option
      When "-v"  Then verbose   = .true
      When "-j"  Then doJson    = .true
      When "-f"  Then showFails = .true
      When "-t"  Then Do
        Parse Var args timeoutSec args
      End
      When "-id" Then Do
        Parse Var args onlyId args
        If onlyId == "" Then Do
          Say "Error: -id requires a test ID argument"
          Exit 1
        End
      End
      Otherwise
        If suitePath == "" Then suitePath = option
        Else Do
          Say "Error: unexpected argument '"option"'"
          Exit 1
        End
    End
  End

  -- Detect platform
  Parse Source operatingSystem .
  isWindows = (operatingSystem~caselessAbbrev("WINDOWS"))

  -- Locate or clone the test suite
  If suitePath == "" Then Do
    If isWindows Then
      suitePath = value("TEMP",,"ENVIRONMENT")"\yaml-test-suite"
    Else
      suitePath = "/tmp/yaml-test-suite"
    If \SysFileExists(suitePath"/229Q/===") Then Do
      Say "Cloning official YAML Test Suite into" suitePath"..."
      Address System "git clone --depth 1 --branch data" -
        "https://github.com/yaml/yaml-test-suite.git" suitePath -
        With Output Using (.array~new) Error Using (.array~new)
      If RC \= 0 Then Do
        Say "Error: git clone failed (RC="RC")"
        Exit 1
      End
    End
    Else Say "Using cached test suite at" suitePath
  End

  If \SysFileExists(suitePath) Then Do
    Say "Error: test suite path not found:" suitePath
    Exit 1
  End

  -- Find our own directory (for test_yaml-single.rex)
  Parse Source . . myPath
  myDir = FileSpec("Location", myPath)
  workerScript = myDir"test_yaml-single.rex"

  -- Enumerate test cases
  testCases = .array~new
  Call EnumerateTests suitePath, testCases

  Say "Found" testCases~items "test cases in" suitePath
  Say "Log file:" logFile
  If isWindows Then
    Say "Timeout: not available on Windows (tests run without timeout)"
  Else
    Say "Timeout per test:" timeoutSec "seconds"
  Say ""

  -- Open log
  log = .Stream~new(logFile)~~open("WRITE REPLACE")

  -- Counters
  passCount      = 0
  failCount      = 0
  timeoutCount   = 0
  errorCount     = 0
  skipCount      = 0
  jsonPassCount  = 0
  jsonFailCount  = 0

  failures    = .array~new
  timeouts    = .array~new

  Do testCase Over testCases

    id      = testCase["id"]
    dir     = testCase["dir"]
    name    = testCase["name"]
    isError = testCase["isError"]

    If onlyId \== "" Then Do
      If id \== onlyId & \id~caselessStartsWith(onlyId"/") Then Do
        skipCount += 1
        Iterate
      End
    End

    -- Log before running (flush immediately)
    log~lineOut("RUNNING:" id "-" name)

    -- Build command
    jsonArg = ""
    If doJson Then jsonArg = "json"
    If isWindows Then
      cmd = 'rexx' workerScript '"'dir'"' jsonArg '2>&1'
    Else
      cmd = "timeout" timeoutSec "rexx" workerScript '"'dir'"' jsonArg "2>&1"

    -- Run
    outLines. = ""
    outLines.0 = 0
    Address System cmd With Output Using (outLines.)
    exitCode = RC

    -- Collect output
    output = ""
    Do i = 1 To outLines.0
      If output \== "" Then output = output"; "
      output = output || outLines.i
    End

    -- Interpret result
    Select
      When exitCode == 124 | exitCode == 137 Then Do
        -- timeout (124) or killed (137)
        timeoutCount += 1
        failCount += 1
        timeouts~append(id "-" name)
        logLine = "TIMEOUT:" id "-" name
        If verbose | showFails Then Say "  TIMEOUT:" id "-" name
      End
      When exitCode == 0 Then Do
        passCount += 1
        If output~caselessPos("PASS+JSON") > 0 Then jsonPassCount += 1
        logLine = "PASS:" id "-" name
        If verbose & \showFails Then Say "  PASS:" id "-" name
      End
      When exitCode == 1 Then Do
        failCount += 1
        failures~append(id":" output "-" name)
        logLine = "FAIL:" id "-" output "-" name
        If verbose | showFails Then Say "  FAIL:" id "-" name "("output")"
      End
      When exitCode == 2 Then Do
        passCount += 1  -- parse passed, JSON comparison failed
        jsonFailCount += 1
        failures~append(id": [json]" output "-" name)
        logLine = "JSON_FAIL:" id "-" output "-" name
        If verbose | showFails Then Say "  JSON FAIL:" id "-" name "("output")"
      End
      Otherwise Do
        errorCount += 1
        failCount += 1
        failures~append(id": exit="exitCode output "-" name)
        logLine = "ERROR(" || exitCode || "):" id "-" output "-" name
        If verbose | showFails Then Say "  ERROR:" id "-" name "("output")"
      End
    End

    -- Write and flush log
    log~lineOut(logLine)

  End

  log~close

  -- Summary
  Say ""
  Say "================================================================"
  Say "Results:" passCount "/" passCount + failCount "passed," -
    failCount "failed"
  Say "================================================================"
  Say ""
  Say "  Parse pass:    " passCount
  Say "  Parse fail:    " failCount - timeoutCount - errorCount
  Say "  Timeouts:      " timeoutCount
  Say "  Errors:        " errorCount
  If skipCount > 0 Then
    Say "  Skipped:       " skipCount
  If doJson Then Do
    Say ""
    Say "  JSON pass:     " jsonPassCount
    Say "  JSON fail:     " jsonFailCount
  End

  If timeouts~items > 0 Then Do
    Say ""
    Say "Timeouts:"
    Do line Over timeouts
      Say "  " line
    End
  End

  If failures~items > 0 Then Do
    Say ""
    Say "Failures (first 50):"
    Do i = 1 To Min(50, failures~items)
      Say "  " failures[i]
    End
    If failures~items > 50 Then
      Say "  ... and" failures~items - 50 "more"
  End

  Say ""
  Say "Full log:" logFile

  Exit failCount > 0

/******************************************************************************/
/*                                                                            */
/* Routines                                                                   */
/*                                                                            */
/******************************************************************************/

--------------------------------------------------------------------------------
-- EnumerateTests: collect all test case directories                          --
--------------------------------------------------------------------------------

::routine EnumerateTests
  Use Strict Arg basePath, testCases

  Call SysFileTree basePath"/*", "dirs.", "DO"
  Do i = 1 To dirs.0
    dirPath = dirs.i~strip
    dirName = FileSpec("Name", dirPath)

    If dirName~length < 2 Then Iterate
    If dirName[1] == "." Then Iterate
    If dirName == "name" | dirName == "tags" | dirName == "meta" Then Iterate

    If SysFileExists(dirPath"/in.yaml") Then Do
      tc = .table~new
      tc["id"]      = dirName
      tc["dir"]     = dirPath
      tc["name"]    = ReadTestName(dirPath)
      tc["isError"] = SysFileExists(dirPath"/error")
      testCases~append(tc)
    End
    Else Do
      Call SysFileTree dirPath"/*", "subdirs.", "DO"
      Do j = 1 To subdirs.0
        subPath = subdirs.j~strip
        subName = FileSpec("Name", subPath)
        If SysFileExists(subPath"/in.yaml") Then Do
          tc = .table~new
          tc["id"]      = dirName"/"subName
          tc["dir"]     = subPath
          tc["name"]    = ReadTestName(subPath)
          tc["isError"] = SysFileExists(subPath"/error")
          testCases~append(tc)
        End
      End
    End
  End
  Return

--------------------------------------------------------------------------------
-- ReadTestName                                                               --
--------------------------------------------------------------------------------

::routine ReadTestName
  Use Strict Arg dir
  nameFile = dir"/==="
  If \SysFileExists(nameFile) Then Return "(no description)"
  s = .Stream~new(nameFile)~~open("READ")
  If s~state \== "READY" Then Return "(cannot read ===)"
  line = s~lineIn
  s~close
  Return line~strip
