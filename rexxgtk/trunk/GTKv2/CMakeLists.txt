#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2014-2014 Rexx Language Association. All rights reserved.    */
#/*                                                                            */
#/* This program and the accompanying materials are made available under       */
#/* the terms of the Common Public License v1.0 which accompanies this         */
#/* distribution. A copy is also available at the following address:           */
#/* http://www.oorexx.org/license.html                                         */
#/*                                                                            */
#/* Redistribution and use in source and binary forms, with or                 */
#/* without modification, are permitted provided that the following            */
#/* conditions are met:                                                        */
#/*                                                                            */
#/* Redistributions of source code must retain the above copyright             */
#/* notice, this list of conditions and the following disclaimer.              */
#/* Redistributions in binary form must reproduce the above copyright          */
#/* notice, this list of conditions and the following disclaimer in            */
#/* the documentation and/or other materials provided with the distribution.   */
#/*                                                                            */
#/* Neither the name of Rexx Language Association nor the names                */
#/* of its contributors may be used to endorse or promote products             */
#/* derived from this software without specific prior written permission.      */
#/*                                                                            */
#/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
#/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
#/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
#/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
#/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
#/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
#/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
#/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
#/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
#/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
#/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
#/*                                                                            */
#/*----------------------------------------------------------------------------*/


#/*----------------------------------------------------------------------------*/
#/* Global settings                                                            */
#/*----------------------------------------------------------------------------*/
cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0010 OLD)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCSourceCompiles)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)


#/*----------------------------------------------------------------------------*/
#/* Project settings                                                           */
#/*----------------------------------------------------------------------------*/
project (ooRexxGTK2)
# The version of ooRexxGTK to make
set (ORX_MAJOR 2)
set (ORX_MINOR 0)
set (ORX_MOD_LVL 0)
set (ORX_BLD_LVL 0)
set (ORX_VERSION ${ORX_MAJOR}.${ORX_MINOR}.${ORX_MOD_LVL})
set (ORX_NODOT_VERSION ${ORX_MAJOR}${ORX_MINOR}${ORX_MOD_LVL})
set (ORX_VER_STR ${ORX_MAJOR}.${ORX_MINOR}.${ORX_MOD_LVL})
string(TIMESTAMP ORX_YEAR %Y)
set (ORX_COPY_YEAR 2005-${ORX_YEAR})
string(TOUPPER ${CMAKE_SYSTEM_NAME} ORX_SYS_STR)

# Get the SVN revision number of an svn command line client is
# available.  This version will run at build time rather than only
# at configuration generation time.
FIND_PROGRAM(SVN_EXECUTABLE svn
  DOC "subversion command line client")

# only do this if we have an svn client.
if (SVN_EXECUTABLE)
    MACRO(Subversion_GET_REVISION dir variable)
      EXECUTE_PROCESS(COMMAND ${SVN_EXECUTABLE} info ${dir}
        OUTPUT_VARIABLE ${variable}
        OUTPUT_STRIP_TRAILING_WHITESPACE)
      STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
        "\\2" ${variable} "${${variable}}")
    ENDMACRO(Subversion_GET_REVISION)

    Subversion_GET_REVISION(${CMAKE_SOURCE_DIR} ORX_BLD_LVL)
endif ()

message(STATUS "SVN Revision Number is ${ORX_BLD_LVL}")

# The following supports the versioning of the rexx shared libraries
# This defines the oorexx library version. For ooRexx this should always be the
# same as the ORX_MAJOR number.
set (ORX_CURRENT ${ORX_MAJOR})
# Each public release of ooRexx should increment this number by one except
# when the ORX_MINOR goes back to zero. If that is the case then this entry
# should also be set to zero.
set (ORX_REVISION 0)
# For ooRexx, this number should ALWAYS be zero. This will force the linker to
# utilize all previous releases for linking.
set (ORX_AGE 0)
# Please note that the version-info has nothing to do with the release version.
# You need to know exactly what you are doing in order to get this correct.
set(VERSION_INFO "-version-info ${ORX_CURRENT}:${ORX_REVISION}:${ORX_AGE}")
# This is version information set on library properties
set(ORX_VERSION ${ORX_CURRENT}.${ORX_AGE}.${ORX_REVISION})

# The following are general settings
# Always create shared libraries (DLLs)
set (CMAKE_CXX_CREATE_SHARED_MODULE 1)
# Set the platform subdirectory name
if (WIN32)
   set (ORX_PLATFORM_DIR platform/windows)
else ()
   set (ORX_PLATFORM_DIR platform/unix)
endif ()


# Common install variables
   SET(CPACK_PACKAGE_VERSION_MAJOR ${ORX_MAJOR})
   SET(CPACK_PACKAGE_VERSION_MINOR ${ORX_MINOR})
   SET(CPACK_PACKAGE_VERSION_PATCH ${ORX_MOD_LVL})
   set(CPACK_PACKAGE_NAME "ooRexxGTK2")
   set(CPACK_PACKAGE_LONG_NAME "Open Object Rexx GTK2")
   set(CPACK_PACKAGE_VENDOR "Rexx Language Association")
   set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/ReleaseNotes)
   set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Open Object Rexx GTK2 Interface Library")
   set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/CPLv1.0.txt)
   set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/ReleaseNotes)

# Set install variables
if (WIN32)
   # NOTE: specifying "." for the destination will place the files in the
   # root directory of the NSIS install.  This needs to be a "." to get
   # the files in the correct location.
   set (INSTALL_EXECUTABLE_DIR .)
   set (INSTALL_LIB_DIR .)
   set (INSTALL_CLS_DIR .)
   set (INSTALL_REX_DIR .)
   set (INSTALL_DOC_DIR doc)
   set (INSTALL_INCLUDE_DIR api)
   set (INSTALL_SAMPLES_DIR samples)

else ()
   set (CMAKE_INSTALL_PREFIX /usr)
   set (catdir ${CMAKE_INSTALL_PREFIX}/bin)
   set (INSTALL_EXECUTABLE_DIR ${CMAKE_INSTALL_PREFIX}/bin)
   set (INSTALL_CLS_DIR ${CMAKE_INSTALL_PREFIX}/bin)
   set (INSTALL_REX_DIR ${CMAKE_INSTALL_PREFIX}/bin)
   set (INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
   set (INSTALL_SAMPLES_DIR ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
   find_path(INSTALL_LIB_DIR libc.so PATHS ${CMAKE_INSTALL_PREFIX}/lib/* ${CMAKE_INSTALL_PREFIX}/lib64)
   set (INSTALL_MAN_DIR ${CMAKE_INSTALL_PREFIX}/share/man)
endif ()

# Set compiler and linker flags common to all build environments
include_directories(${PROJECT_BINARY_DIR})
if (WIN32)
   add_definitions(/DVMAJOR=${ORX_MAJOR}
            /DVMINOR=${ORX_MINOR}
            /DVREL=${ORX_MOD_LVL}
            /DORX_BLD=${ORX_BLD_LVL}
            /DORX_VER_STR=${ORX_VER_STR}
            /DOOREXX_COPY_YEAR="${ORX_COPY_YEAR}"
            /DORX_SYS_STR="${CMAKE_SYSTEM_NAME}"
            /DORX_SHARED_LIBRARY_EXT="${CMAKE_SHARED_LIBRARY_SUFFIX}"
            /D_CRT_SECURE_NO_DEPRECATE
            /D_CRT_SECURE_NO_WARNINGS
            /D_CRT_NONSTDC_NO_DEPRECATE
            /DWIN32 /D_WINDOWS /DHAVE_CONFIG_H)
   if ($ENV{Platform} STREQUAL "X64")
      set (EXE_MANIFEST_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${ORX_PLATFORM_DIR}/rexx64.exe.manifest)
      set (NSIS_CPU "x86_64")
   else ()
      set (EXE_MANIFEST_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${ORX_PLATFORM_DIR}/rexx32.exe.manifest)
      set (NSIS_CPU "x86_32")
   endif ()
   file(TO_NATIVE_PATH ${EXE_MANIFEST_FILE} EXE_MANIFEST_FILE)
   set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:524288")
else ()
   add_definitions(-DORX_VER=${ORX_MAJOR} -DORX_REL=${ORX_MINOR} -DORX_MOD=${ORX_MOD_LVL} -DORX_FIX=0
            -DVMAJOR=${ORX_MAJOR} -DVMINOR=${ORX_MINOR} -DVREL=${ORX_MOD_LVL}
            -DOOREXX_COPY_YEAR="${ORX_COPY_YEAR}"
            -DORX_SYS_STR="${CMAKE_SYSTEM_NAME}"
            -DORX_SHARED_LIBRARY_EXT="${CMAKE_SHARED_LIBRARY_SUFFIX}"
            -D${ORX_SYS_STR} -DOPSYS_${ORX_SYS_STR} -DHAVE_CONFIG_H
            `pkg-config --cflags gtk+-2.0 gdk-2.0 glib-2.0 gthread-2.0`)
    set (ORX_SYSLIB_DL dl)
    set (ORX_SYSLIB_PTHREAD pthread)
endif ()

# Check header files and functions. These are all defined to the config.h file.
check_include_file(attr/xattr.h HAVE_ATTR_XATTR_H)
check_function_exists(catopen HAVE_CATOPEN)
check_include_file(ctype.h HAVE_CTYPE_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(features.h HAVE_FEATURES_H)
check_include_file(filehdr.h HAVE_FILEHDR_H)
check_function_exists(fstat HAVE_FSTAT)
check_function_exists(gcvt HAVE_GCVT)
check_function_exists(geteuid HAVE_GETEUID)
check_function_exists(getpgrp HAVE_GETPGRP)
check_function_exists(getpwuid HAVE_GETPUID)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(getuserattr HAVE_GETUSERATTR)
check_function_exists(IDtouser HAVE_IDTOUSER)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(float.h HAVE_FLOAT_H)
find_library(HAVE_LIBPTHREAD pthread)
find_library(HAVE_LIBRT rt)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(locale.h HAVE_LOCALE_H)
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_function_exists(memset HAVE_MEMSET)
check_include_file(mesg.h HAVE_MESG_H)
check_function_exists(nanosleep HAVE_NANOSLEEP)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(nl_types.h HAVE_NL_TYPES_H)
check_function_exists(nsleep HAVE_NSLEEP)
check_include_file(pthread.h HAVE_PTHREAD_H)
set(CMAKE_REQUIRED_LIBRARIES "pthread;pthread_nonshared")
check_function_exists(pthread_mutexattr_settype HAVE_PTHREAD_MUTEXATTR_SETTYPE)
check_c_source_compiles("#include <pthread.h>
                         int main(int arg, char **argv) {
                         int tryme;
                         tryme = PTHREAD_MUTEX_ERRORCHECK;
                         return 0;}"
                        HAVE_PTHREAD_MUTEX_ERRORCHECK)
check_c_source_compiles("#include <pthread.h>
                         int main(int arg, char **argv) {
                         int tryme;
                         tryme = PTHREAD_MUTEX_RECURSIVE;
                         return 0;}"
                        HAVE_PTHREAD_MUTEX_RECURSIVE)
check_c_source_compiles("#include <pthread.h>
                         int main(int arg, char **argv) {
                         int tryme;
                         tryme = PTHREAD_MUTEX_RECURSIVE_NP;
                         return 0;}"
                        HAVE_PTHREAD_MUTEX_RECURSIVE_NP)
check_symbol_exists(PTHREAD_MUTEX_RECURSIVE_NP "pthread.h" HAVE_PTHREAD_MUTEX_RECURSIVE_NP)
check_include_file(pwd.h HAVE_PWD_H)
check_include_file(sched.h HAVE_SCHED_H)
check_function_exists(setlocale HAVE_SETLOCALE)
check_function_exists(sighold HAVE_SIGHOLD)
check_include_file(signal.h HAVE_SIGNAL_H)
check_include_file(stdarg.h HAVE_STDARG_H)
check_include_file(stddef.h HAVE_STDDEF_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdio.h HAVE_STDIO_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_function_exists(strdup HAVE_STRDUP)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(stropts.h HAVE_STROPTS_H)
check_include_file(sys/filio.h HAVE_SYS_FILIO_H)
check_include_file(sys/ldr.h HAVE_SYS_LDR_H)
check_include_file(sys/resource.h HAVE_SYS_RESOURCE_H)
check_include_file(sys/select.h HAVE_SYS_SELECT_H)
check_include_file(sys/sem.h HAVE_SYS_SEM_H)
check_include_file(sys/signal.h HAVE_SYS_SIGNAL_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/syscall.h HAVE_SYS_SYSCALL_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(sys/utsname.h HAVE_SYS_UTSNAME_H)
check_include_file(sys/wait.h HAVE_SYS_WAIT_H)
check_include_file(time.h HAVE_TIME_H)
check_c_source_compiles("#include <sys/types.h>
                         #include <sys/sem.h>
                         int main(int arg, char **argv) {
                         union semun semopts;
                         return 0;}"
                        HAVE_UNION_SEMUN)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(usersec.h HAVE_USERSEC_H)
check_function_exists(vprintf HAVE_VPRINTF)
set (PACKAGE ${PROJECT_NAME})
set (PACKAGE_BUGREPORT "http://sourceforge.net/projects/oorexx/")
set (PACKAGE_NAME ${PROJECT_NAME})
set (PACKAGE_STRING ${PROJECT_NAME})
set (PACKAGE_TARNAME "ooRexx.tar")
set (PACKAGE_URL "http://www.oorexx.org/")
set (PACKAGE_VERSION ${ORX_VER_STR})
set (VERSION ${ORX_VER_STR})
set (STDC_HEADERS 1)


# Configure files config.h
configure_file(config.h.in.cmake ${PROJECT_BINARY_DIR}/config.h)

if (WIN32)
# This replaces the /MD compile flag with /MT.
set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()
endif ()

#/*----------------------------------------------------------------------------*/
#/* Build targets                                                              */
#/*----------------------------------------------------------------------------*/


#################### liboorexxgtk2.so ##########################
# Full set of files used in all platforms.
set (common_oorexxgtk2_sources
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxaccelgroup.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxadjustment.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxassistant.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxbutton.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxcalendar.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxcellrenderer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxclipboard.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxcombobox.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxcontainer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxdialog.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxentry.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxeventbox.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxevents.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxexpander.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxfixed.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxglib.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxhandlebox.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grximage.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxinterfaces.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxlabel.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxliststore.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxmenubar.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxmenu.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxmenuitem.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxmenushell.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxmenutoolbutton.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxnotebook.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxpaned.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxprogressbar.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxrange.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxscrolledwindow.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxstatusbar.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtable.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtextbuffer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtextview.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtoolbar.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtoolbutton.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtoolitem.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtreemodel.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtreeselection.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtreestore.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtreeviewcolumn.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxtreeview.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxwidget.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/grxwindow.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/rexxgtk.cpp)
# additional source files required by specific platforms
if (WIN32)
   set (platform_oorexxgtk2_sources
            ${CMAKE_CURRENT_SOURCE_DIR}/src/rexxgtk.def)
else ()
   set (platform_oorexxgtk2_libs ${ORX_SYSLIB_DL} ${ORX_SYSLIB_PTHREAD})
endif ()
# Library definition
add_library(oorexxgtk2 SHARED
            ${common_oorexxgtk2_sources}
            ${platform_oorexxgtk2_sources})
# Include file definition
target_include_directories(oorexxgtk2 PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src)
# Extra link library definitions
target_link_libraries(oorexxgtk2 ${platform_oorexxgtk2_libs} 
                        -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lfontconfig
                        -lfreetype -lgdk-x11-2.0 -lpangocairo-1.0 -lpango-1.0 -lcairo -lgdk_pixbuf-2.0
                        -lgobject-2.0 -lgthread-2.0 -pthread -lglib-2.0)
install(TARGETS oorexxgtk2 RUNTIME DESTINATION ${INSTALL_LIB_DIR} COMPONENT Core
                        LIBRARY DESTINATION ${INSTALL_LIB_DIR} COMPONENT Core
                        ARCHIVE DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT DevLib)
set_target_properties(oorexxgtk2 PROPERTIES VERSION ${ORX_VERSION})


#################### misc files ##########################

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/oorexxgtk2.cls COMPONENT DevLib DESTINATION ${INSTALL_EXECUTABLE_DIR})


#################### NSIS Package Targets ##########################
#################### NSIS Package Targets ##########################
#################### NSIS Package Targets ##########################
#################### NSIS Package Targets ##########################
#################### NSIS Package Targets ##########################

if (WIN32)

endif ()


#################### Unix Package Targets ##########################
#################### Unix Package Targets ##########################
#################### Unix Package Targets ##########################
#################### Unix Package Targets ##########################
#################### Unix Package Targets ##########################

# Build an RPM
if (BUILD_RPM)
   # This pevents the rpm from generating these subdirectories in the install.
   set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION 
      ${CMAKE_INSTALL_PREFIX}/share/man 
      ${CMAKE_INSTALL_PREFIX}/share/man/man1
      ${CMAKE_INSTALL_PREFIX}/share/applications
      ${CMAKE_INSTALL_PREFIX}/share/icons)
   set(CPACK_GENERATOR RPM)
   set(CPACK_RPM_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
   if (DEFINED PROD)
      set(CPACK_PACKAGE_RELEASE 1)
   else ()
      set(CPACK_PACKAGE_RELEASE ${ORX_BLD_LVL})
   endif ()
   set(CPACK_RPM_PACKAGE_LICENSE "CPL 1.0")
   set(CPACK_RPM_PACKAGE_URL "http://www.oorexx.org/")
   set(CPACK_PACKAGE_CONTACT "wdashley@sourceforge.net")
   set(CPACK_PACKAGING_INSTALL_PREXIX "${CMAKE_INSTALL_PREFIX}")
   if (${INSTALL_LIB_DIR} STREQUAL ${CMAKE_INSTALL_PREFIX}/lib64)
      set(orx_install_arch "x86_64")
   else ()
      set(orx_install_arch "i386")
   endif ()
   set(CPACK_RPM_PACKAGE_ARCHITECTURE ${orx_install_arch})
   if (DEFINED OS_DIST)
      set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${ORX_MAJOR}.${ORX_MINOR}.${ORX_MOD_LVL}-${CPACK_PACKAGE_RELEASE}.${orx_install_arch}.${OS_DIST}")
   else ()
      set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${ORX_MAJOR}.${ORX_MINOR}.${ORX_MOD_LVL}-${CPACK_PACKAGE_RELEASE}.${orx_install_arch}")
   endif ()
   set(CPACK_RPM_PACKAGE_DESCRIPTION
"Open Object Rexx GTK interface adds support to ooRexx for creating and
manipulating GTK windows and dialogs.

For more information on ooRexx, visit http://www.oorexx.org/
For more information on Rexx, visit http://www.rexxla.org/")
endif ()

# Build a DEB
if (BUILD_DEB)
   set(CPACK_GENERATOR DEB)
   # The package name MUST be in lower case.
   set(CPACK_DEBIAN_PACKAGE_NAME oorexx)
   if (DEFINED PROD)
      set(CPACK_PACKAGE_RELEASE 1)
   else ()
      set(CPACK_PACKAGE_RELEASE ${ORX_BLD_LVL})
   endif ()
   set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
   set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.oorexx.org/")
   set(CPACK_PACKAGE_CONTACT "wdashley@sourceforge.net")
   set(CPACK_DEBIAN_PACKAGE_MAINTAINER "David Ashley <wdashley@sourceforge.net>")
   set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
   if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
      set(orx_install_arch "x86_64")
   else ()
      set(orx_install_arch "i386")
   endif ()
   if (DEFINED OS_DIST)
      set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${ORX_MAJOR}.${ORX_MINOR}.${ORX_MOD_LVL}-${CPACK_PACKAGE_RELEASE}.${orx_install_arch}.${OS_DIST}")
   else ()
      set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${ORX_MAJOR}.${ORX_MINOR}.${ORX_MOD_LVL}-${CPACK_PACKAGE_RELEASE}.${orx_install_arch}")
   endif ()
   # Be sure to do this AFTER you set the CPACK_PACKAGE_FILE_NAME
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/platform/unix/debian/cmake/fixup_deb.sh.in ${CMAKE_BINARY_DIR}/fixup_deb.sh)
   # The DEB generator does not support a multi-line description.
   set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Open Object Rexx GTK interface adds support to ooRexx for creating and manipulating GTK windows and dialogs.")
endif ()





# Do not move the following statement!! It must be the last statement in this file.
include(CPack)
