@echo off

makensis /DVERSION=4.2.0.6367 /DNODOTVER=420 /DSRCDIR=C:\work\wc\main.4.1.0.ooDialog /DBINDIR=C:\work\wc\main.4.1.0.ooDialog\Win32Dbg switchOODialog.nsi

if ERRORLEVEL 1 goto error

makensis /DVERSION=4.2.0.6367 /DNODOTVER=420 /DSRCDIR=C:\work\wc\main.4.1.0.ooDialog /DEXEFILE=switchOODialog /DCPU=x86_32 ooDialogBeta.nsi

if ERRORLEVEL 1 goto error
goto aroundError

:error

echo Error building installer(s)

:aroundError
