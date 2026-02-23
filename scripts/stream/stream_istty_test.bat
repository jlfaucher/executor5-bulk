@echo off

:: Set of test cases for
:: #867 Determine whether a stream is associated with a terminal
:: https://sourceforge.net/p/oorexx/feature-requests/867/

:: Usage:
:: ./stream_istty_test
:: Do not redirect stdout or stderr, this will disrupt the results.
::
:: Output:
:: Displays the test cases and their output both to stdout
:: and to stream_istty_test-out.txt

goto :main

:: Can't use this function, it's too difficult to pass the strings...
:: AI: The practical solution is to not use a function at all,
:: and inline the echo+execute pattern, escaping special characters with ^ at
:: the call site
:execute
    :: Display the command
    echo %1
    echo %1 >> %output%

    :: Execute the command
    %1
    goto :eof

:main
:: Updated by stream_istty.rex which adds text with each execution
set output=stream_istty_test-out.txt
if exist "%output%" del "%output%"

echo set output=%output%
echo set output=%output% >> %output%
echo:
echo: >> %output%

echo rexx stream_istty.rex %%output%% >> %output%
echo rexx stream_istty.rex %%output%%
     rexx stream_istty.rex  %output%

echo rexx stream_istty.rex %%output%% 1^> CON >> %output%
echo rexx stream_istty.rex %%output%% 1^> CON
     rexx stream_istty.rex  %output%  1> CON

echo rexx stream_istty.rex %%output%% 2^> CON >> %output%
echo rexx stream_istty.rex %%output%% 2^> CON
     rexx stream_istty.rex  %output%  2> CON

:: Pass dont_stdout to indicate to not use stdout, because stdout is discarded
echo rexx stream_istty.rex %%output%% dont_stdout 1^> NUL >> %output%
echo rexx stream_istty.rex %%output%% dont_stdout 1^> NUL
     rexx stream_istty.rex  %output%  dont_stdout 1> NUL
:: stdout isTTY=YES, macOS=NO

echo rexx stream_istty.rex %%output%% 2^> NUL >> %output%
echo rexx stream_istty.rex %%output%% 2^> NUL
     rexx stream_istty.rex  %output%  2> NUL
:: stderr isTTY=YES, macOS=NO

echo echo test ^| rexx stream_istty.rex %%output%% >> %output%
echo echo test ^| rexx stream_istty.rex %%output%%
     echo test  | rexx stream_istty.rex  %output%
:: input is not from a terminal
:: stdin TRANSIENT does not help to detect if input is not from a terminal

:: Next command should be non blocking, thanks to echo.
:: But it's blocking, waiting for input.
:: Not activated.
:: echo test | type CON | rexx stream_istty.rex %output%
:: CON is not recognized as a terminal because it is connected via a pipe
:: same as macOS.
::     stdin  streamType: TRANSIENT  isTTY: NO
::     stdout streamType: TRANSIENT  isTTY: YES
::     stderr streamType: TRANSIENT  isTTY: YES

:: Next command is blocking, waiting for input.
:: Not activated.
:: type CON | rexx stream_istty.rex %output%
:: Same results as the previous command with echo.


:: Not blocked (that's good), but it makes me wonder why?
:: AI: < CON is lazy (opens but doesn't read unless asked),
::     while type CON | is eager (actively reads).
echo rexx stream_istty.rex %%output%% ^< CON >> %output%
echo rexx stream_istty.rex %%output%% ^< CON
     rexx stream_istty.rex  %output%   < CON
:: CON is recognized as a terminal

echo echo test ^> in.tmp.txt ^&^& rexx stream_istty.rex %%output%% ^< in.tmp.txt >> %output%
echo echo test ^> in.tmp.txt ^&^& rexx stream_istty.rex %%output%% ^< in.tmp.txt
     echo test  > in.tmp.txt  &&  rexx stream_istty.rex  %output%   < in.tmp.txt
if exist in.tmp.txt del in.tmp.txt

echo rexx stream_istty.rex %%output%% ^> out.tmp.txt ^&^& type out.tmp.txt >> %output%
echo rexx stream_istty.rex %%output%% ^> out.tmp.txt ^&^& type out.tmp.txt
     rexx stream_istty.rex  %output%   > out.tmp.txt  &&  type out.tmp.txt
if exist out.tmp.txt del out.tmp.txt

echo rexx stream_istty.rex %%output%% ^| findstr "^" >> %output%
echo rexx stream_istty.rex %%output%% ^| findstr "^"
     rexx stream_istty.rex  %output%   | findstr "^"
:: stdout TRANSIENT does not help to detect if stdout is not a terminal

echo rexx -e "say .stream~new('NUL')~~open('read')~query('isTTY')" >> %output%
echo rexx -e "say .stream~new('NUL')~~open('read')~query('isTTY')"
     rexx -e "say .stream~new('NUL')~~open('read')~query('isTTY')" > out.tmp.txt
set /p result=<out.tmp.txt
echo      %result%
echo      %result% >> %output%
if exist out.tmp.txt del out.tmp.txt
:: _isatty returns non-zero for NUL.
:: This is a known problem:
:: https://stackoverflow.com/questions/3648711/detect-nul-file-descriptor-isatty-is-bogus
:: https://github.com/pallets/click/pull/1135
