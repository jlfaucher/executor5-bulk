# Set REXX_HOME to point to your build directory if ooRexx is not installed.
#REXX_HOME = C:\work.ooRexx\wc\main\Win32Rel   # For example

# Define RELEASE on the command line, or here, to build a non-debug version.
# I.e.: nMake RELEASE=1
#RELEASE = 1

# Define CPU=X64 on the command line, or here, to build for 64-bit.  By default
# builds for 32-bit.  I.e.: nMake CPU=X64 RELEASE=1  We only need to know the
# CPU in order to pick the right manifest file.
#CPU = X64

!IF DEFINED(REXX_HOME)
INCLUDE = $(INCLUDE);$(REXX_HOME)\api
LIB = $(LIB);$(REXX_HOME)\api
!ENDIF

!IFNDEF CPU
CPU = X86
!ENDIF

!IF "$(CPU)" == "X64"
M_FILE = "rxStart64.exe.manifest"
MACHINE = /MACHINE:X64
!ELSE
M_FILE = "rxStart32.exe.manifest"
MACHINE = /MACHINE:X86
!ENDIF

REXX_LIBS = rexx.lib rexxapi.lib

WARNINGFLAGS = /W3 /Wp64 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE

!IF DEFINED(RELEASE)

OOREXX_CFLAGS = /nologo /EHsc /O2 /Gr /Gs /FAcs /MT $(WARNINGFLAGS) /c
EXE_LFLAGS = /nologo /SUBSYSTEM:Console $(MACHINE) $(REXX_LIBS) user32.lib comdlg32.lib gdi32.lib kernel32.lib
DLL_LFLAGS = /nologo /SUBSYSTEM:Windows $(MACHINE) $(REXX_LIBS) /DLL

!ELSE

OOREXX_CFLAGS =  /nologo /EHsc /Zi /Od /Gr /MTd $(WARNINGFLAGS) /c
EXE_LFLAGS = /nologo /DEBUG -debugtype:cv /SUBSYSTEM:Console $(MACHINE) $(REXX_LIBS) user32.lib comdlg32.lib gdi32.lib kernel32.lib
DLL_LFLAGS = /nologo /DEBUG -debugtype:cv /SUBSYSTEM:Windows $(MACHINE) $(REXX_LIBS) /DLL

!ENDIF

rcflags_common=/DWIN32 /DMANIFEST_FILE=$(M_FILE)

rxStart.exe: rxStart.obj rxStart.res
    link $(EXE_LFLAGS) rxStart.res rxStart.obj -out:$(@B).exe

rxStart.obj: rxStart.cpp
  cl $(OOREXX_CFLAGS) rxStart.cpp

rxStart.res: rxStart.rc
  rc $(rcflags_common) -r -fo$(@B).res $(@B).rc

clean:
  attrib -h *.suo
	del *.exe *.dll *.obj *.ilk *.pdb *.lib *.exp *.suo *.cod *.res *.sln 1>nul 2>&1

