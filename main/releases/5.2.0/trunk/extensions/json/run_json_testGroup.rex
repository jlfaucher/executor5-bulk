#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/* run_json_testGroup.rex — standalone runner for ooRexxUnit testGroups        */
/*                                                                            */
/* Usage:  rexx run_json_testGroup.rex [testGroupFile]                        */
/*                                                                            */
/* Runs the specified testGroup file, or json_01_Claude.testGroup by default. */
/* This script loads ooRexxUnit, invokes the testGroup container, builds      */
/* a test suite from all its test classes, and runs them with a console        */
/* formatter.                                                                 */
/*----------------------------------------------------------------------------*/

parse arg testGroupFile
testGroupFile = testGroupFile~strip
if testGroupFile == "" then
  testGroupFile = "json_01_Claude.testGroup"

parse source . . thisFile
thisDir = fileSpec("location", thisFile)

/* Ensure ooRexxUnit framework is in the path */
ooRexxUnitDir = thisDir || "oorexxunit"
curPath = value("PATH", , 'ENVIRONMENT')
call value "PATH", ooRexxUnitDir || ":" || curPath, 'ENVIRONMENT'

/* Load the framework */
call "OOREXXUNIT.CLS"

/* Invoke the testGroup to get a TestGroup container */
call (testGroupFile) .nil
tg = result

/* If the testGroup returned a TestGroup, build and execute the suite.   */
/* If it returned an ooTestResult (self-running testGroup), use that.    */
if tg~isA(.TestGroup) then do
  suite = tg~suite
  testResult = suite~execute(, .true)
end
else
  testResult = tg

/* Print results */
formatter = .SimpleConsoleFormatter~new(testResult)
formatter~setTitle("json.cls ooRexxUnit Test Results")
formatter~print(testResult)

if testResult~wasSuccessful then exit 0
else exit 1
