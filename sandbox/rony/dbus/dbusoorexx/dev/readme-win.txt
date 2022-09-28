20110827, 20110920, rgf

to compile dbusoorexx.cc on Windows:

- 32 bit:

  F:\work\svn\bsf4oorexx\trunk\bsf4oorexx.dev\source_cc\env4_32bit_xp.cmd
  nmake /f Makefile32 clean && nmake /f Makefile32 RELEASE=1
----------------------------------------
- 64 bit: 

  F:\work\svn\bsf4oorexx\trunk\bsf4oorexx.dev\source_cc\env4_64bit_xp.cmd
  nmake /f Makefile64 clean && nmake /f Makefile64 RELEASE=1

=================================================================================================

to run:
   add2path F:\work\svn\bsf4oorexx\sandbox\rgf\misc\dbusoorexx\tests\dev
   add2path F:\work\svn\bsf4oorexx\sandbox\rgf\misc\dbusoorexx\tests
   add2path E:\Programme\dbus\bin
   add2path F:\work\svn\bsf4oorexx\sandbox\rgf\misc\dbusoorexx

=================================================================================================

to compile the "D-BUS" source with CMake:

add path to:

     add2path E:\Programme\CMake 2.8\bin

     add2path E:\Programme\Expat 2.0.1\Bin

oder:

     add2path E:\Programme\libxml2\bin


run:

     F:\work\svn\bsf4oorexx\trunk\bsf4oorexx.dev\source_cc\env4_32bit_xp.cmd

---rgf, 2011-09-19

-----------------------------
Win32 Microsoft nmake
1. install cmake and libexpat
2. get dbus sources
3. unpack dbus sources into a sub directory (referred as <dbus-src-root> later)
4. mkdir dbus-build
5. cd dbus-build
6. run
    cmake -G "NMake Makefiles" [<options, see below>] <dbus-src-root>/cmake
    nmake
    nmake install
----------------------------

expat:

======================================
works !!! (rgf, 2011-09-19)

--> // MAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.
--> CMAKE_BUILD_TYPE:STRING=Debug
to change:

      -DCMAKE_BUILD_TYPE=Release
      -DCMAKE_BUILD_TYPE=RelWithDebInfo
      -DCMAKE_BUILD_TYPE=MinSizeRel

------------------------------------
installs into default directory:

   cmake -G "NMake Makefiles" -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:\Programme\Expat 2.0.1\Source\lib" F:/work/git/dbus-win-20110919/dbus/cmake

------------------------------------
installs into: E:/Programme/dbus

    cmake -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=E:/Programme/dbus -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:\Programme\Expat 2.0.1\Source\lib" F:/work/git/dbus-win-20110919/dbus/cmake

    cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=E:/Programme/dbus -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:\Programme\Expat 2.0.1\Source\lib" F:/work/git/dbus-win-20110919/dbus/cmake

    cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=E:/Programme/dbus -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:\Programme\Expat 2.0.1\Source\lib" F:/work/git/dbus-win-20110919/dbus/cmake


======================================
