@echo off
REM Use this batch file to run the OCube test.
REM Note that the starting target file for this test is
REM ocube.test.bak, NOT test.txt. This allows the test to be
REM repeated many times without you manually saving
REM test.txt before every test run.
copy ocube.test.bak ocube.test.txt /y
rexx ocube.rex ocube.proc.txt ocube.test.txt ocube.test.bak -verbose -truncinlines
