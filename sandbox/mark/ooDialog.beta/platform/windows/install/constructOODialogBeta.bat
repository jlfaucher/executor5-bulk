@echo off


REM makensis %OODDOTVER% %NODOTVER% %SRCDIR% %SWITCHEXE% %CPUDEF% ooDialogBeta.nsi

makensis /DVERSION=4.2.0.6413 /DNODOTVER=420 /DSRCDIR=C:\work.ooRexx\wc\main.4.1.0.ooDialog /DBINDIR=C:\work.ooRexx\wc\main.4.1.0.ooDialog\Win32Dbg switchOODialog420_410.nsi

if ERRORLEVEL 1 goto error

REM makensis %OODDOTVER% %NODOTVER% %SRCDIR% %BINDIR% switchOODialog.nsi

makensis /DVERSION=4.2.0.6413 /DNODOTVER=420 /DSRCDIR=C:\work.ooRexx\wc\main.4.1.0.ooDialog /DEXEFILE=switchOODialog420_410 /DCPU=x86_32 ooDialog420_410.nsi

if ERRORLEVEL 1 goto error
goto aroundError

:error

echo Error building installer(s)

:aroundError
