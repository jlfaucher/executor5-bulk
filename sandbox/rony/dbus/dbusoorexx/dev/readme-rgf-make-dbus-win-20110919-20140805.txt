add path to:

since 20140805
     add2path E:\Programme\CMake\bin
     add2path E:\Programme\Expat_2.1.0\Bin

     [old: add2path E:\Programme\CMake 2.8\bin]
     [old: add2path E:\Programme\Expat 2.0.1\Bin]

oder:

     add2path E:\Programme\libxml2\bin


run:

     F:\work\svn\bsf4oorexx\trunk\bsf4oorexx.dev\source_cc\env4_32bit_xp.cmd
or:
     "e:\Programme\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat"



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

    cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=E:/Programme/dbus -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:\Programme\Expat 2.0.1\Source\lib" F:/work/git/dbus-win-20110919/dbus/cmake

    cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=E:/Programme/dbus -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:\Programme\Expat 2.0.1\Source\lib" F:/work/git/dbus-win-20110919/dbus/cmake


======================================


---

cmake -G "NMake Makefiles" -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:/Programme/Expat 2.0.1" F:/work/git/dbus-win-20110919/dbus/cmake

cmake -G "NMake Makefiles" -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR="E:/Programme/Expat 2.0.1/Source/lib/expat.h" F:/work/git/dbus-win-20110919/dbus/cmake

cmake -G "NMake Makefiles" F:/work/git/dbus-win-20110919/dbus/cmake




----------------------------


cmake -G "NMake Makefiles" F:/work/git/dbus-win-20110919/dbus/cmake

cmake -G "NMake Makefiles" DBUS_USE_EXPAT:BOOL=ON LIBEXPAT_DIR:FILEPATH==E:/Programme/Expat 2.0.1  F:/work/git/dbus-win-20110919/dbus/cmake


cmake -G "NMake Makefiles" -DDBUS_USE_EXPAT=ON -DLIBEXPAT_DIR:FILEPATH="E:/Programme/Expat 2.0.1/Bin/libexpat.dll" F:/work/git/dbus-win-20110919/dbus/cmake



cmake -G "NMake Makefiles" -DDBUS_USE_EXPAT=ON -DLIBEXPAT_INCLUDE_DIR=E:/Programme/Expat 2.0.1/Source/lib F:/work/git/dbus-win-20110919/dbus/cmake



cmake -G "NMake Makefiles" LIBXML2_INCLUDE_DIR:PATH=E:/Programme/libxml2/include LIBXML2_LIBRARIES:FILEPATH=E:/Programme/libxml2/include/lib F:/work/git/dbus-win-20110919/dbus/cmake

LIBXML2_LIBRARIES=E:/Programme/libxml2/include/lib

DBUS_USE_EXPAT:BOOL=ON LIBEXPAT_DIR:FILEPATH==E:/Programme/Expat 2.0.1

//Path to a file.
LIBEXPAT_INCLUDE_DIR:PATH=E:/Programme/Expat 2.0.1/Source/lib/expat.h
LIBEXPAT_LIBRARIES:FILEPATH=E:/Programme/Expat 2.0.1/Bin/libexpat.lib

LIBXML2_LIBRARIES LIBXML2_INCLUDE_DIR=E:/Programme/libxml2

--------- 2014-08-05, rgf -------------

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=E:/Programme/dbus-20140805 -DDBUS_USE_EXPAT=ON -DEXPAT_INCLUDE_DIR=E:/Programme/Expat_2.1.0/Source/lib F:/work/git/dbus-win-20140805/dbus/cmake
