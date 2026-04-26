/*----------------------------------------------------------------------------*/
/*                                                                            */
/* test_json_seriot.rex — Test json.cls against the JSONTestSuite             */
/*                       (Nicolas Seriot, https://github.com/nst/JSONTestSuite)*/
/*                                                                            */
/* Each test file in test_parsing/ is named with a prefix:                    */
/*   y_  — must be accepted (valid JSON)                                      */
/*   n_  — must be rejected (invalid JSON)                                    */
/*   i_  — implementation-defined (either result is acceptable)               */
/*                                                                            */
/* Copyright (c) 2010-2026 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

parse source . . fileSpec
testDir = fileSpec~left(fileSpec~lastPos("/"))  -- directory of this script
suiteDir = testDir || "JSONTestSuite/test_parsing"

/* Check that the test suite directory exists; clone if needed */
if \SysFileExists(suiteDir) then do
  say "JSONTestSuite not found — cloning from GitHub..."
  cloneDir = testDir || "JSONTestSuite"
  gitCmd = "git clone https://github.com/nst/JSONTestSuite.git" cloneDir
  out. = ""; err. = ""
  address system gitCmd with output stem out. error stem err.
  if rc \== 0 then do
    say "ERROR: git clone failed (rc="rc")"
    say "Please clone manually: git clone https://github.com/nst/JSONTestSuite.git"
    exit 1
  end
  say "Clone successful."
  if \SysFileExists(suiteDir) then do
    say "ERROR: JSONTestSuite cloned but" suiteDir "not found"
    exit 1
  end
end

/* Collect all .json files */
ret = SysFileTree(suiteDir || "/*.json", "files", "FO")
if files.0 == 0 then do
  say "ERROR: No .json files found in" suiteDir
  exit 1
end

/* Sort the file list for reproducible output */
arr = .array~new
do i = 1 to files.0
  arr~append(files.i)
end
arr~sort

/* Counters */
yPass = 0; yFail = 0; yTotal = 0
nPass = 0; nFail = 0; nTotal = 0
iAccept = 0; iReject = 0; iTotal = 0
failures = .array~new

do f over arr
  /* Extract just the filename */
  slashPos = f~lastPos("/")
  if slashPos == 0 then slashPos = f~lastPos("\")
  fn = f~substr(slashPos + 1)

  /* Try to read and parse */
  parsed = tryParse(f)

  /* Determine expected behaviour from prefix */
  prefix = fn~left(2)

  select case prefix
    when "y_" then do
      yTotal += 1
      if parsed then do
        yPass += 1
      end
      else do
        yFail += 1
        failures~append("FAIL y_ (should accept):" fn)
      end
    end
    when "n_" then do
      nTotal += 1
      if \parsed then do
        nPass += 1
      end
      else do
        nFail += 1
        failures~append("FAIL n_ (should reject):" fn)
      end
    end
    when "i_" then do
      iTotal += 1
      if parsed then iAccept += 1
                else iReject += 1
    end
    otherwise nop
  end
end

/* Print results */
say "=" ~copies(70)
say "JSONTestSuite results for json.cls"
say "=" ~copies(70)
say ""
say "y_ tests (must accept):" yPass"/"yTotal "passed"
if yFail > 0 then
  say "   " yFail "FAILURES"
say "n_ tests (must reject):" nPass"/"nTotal "passed"
if nFail > 0 then
  say "   " nFail "FAILURES"
say "i_ tests (implementation-defined):" iAccept "accepted," iReject "rejected (of" iTotal")"
say ""

total = yPass + nPass
totalTests = yTotal + nTotal
say "Overall:" total"/"totalTests "mandatory tests passed"
say ""

if failures~items > 0 then do
  say "-" ~copies(70)
  say "Failures:"
  say "-" ~copies(70)
  do msg over failures
    say "  " msg
  end
  say ""
end

say "Done."
exit (yFail + nFail > 0)  -- exit 0 if all pass, 1 if any failures

::requires "json.cls"

/** Tries to read and parse a JSON file.
 *  Returns .true if parsing succeeds, .false otherwise.
 */
::routine tryParse
  use strict arg filePath

  /* Read file contents */
  content = ""
  signal on notready name readFail
  s = .stream~new(filePath)
  s~open("read")
  chars = s~chars
  if chars > 0 then content = s~charin(1, chars)
  s~close
  signal off notready

  /* Try to parse */
  signal on syntax name parseFail
  json = .json~new
  doc = json~fromJSON(content)
  return .true

readFail:
  s~close
  return .false

parseFail:
  return .false
