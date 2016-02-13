#/*----------------------------------------------------------------------------*/
#/* Global settings                                                            */
#/*----------------------------------------------------------------------------*/
cmake_minimum_required (VERSION 2.6)

#/*----------------------------------------------------------------------------*/
#/* Project settings                                                           */
#/*----------------------------------------------------------------------------*/
# project (mpzlib)

set (oorexx_source_dir $ENV{HOMEPATH}/Downloads/ooRexxSVN)
set (oorexx_build_dir $ENV{HOMEPATH}/Downloads/ooRexxBuild)

set (build_api_dir ${oorexx_source_dir}/main/trunk/api)
set (build_api_platform_dir ${build_api_dir}/platform/windows)

if (CMAKE_SIZEOF_VOID_P MATCHES 8)
  set (build_lib_dir ${oorexx_build_dir}/retail64/bin)
# set (gmp_lib ${PROJECT_SOURCE_DIR}/mpir/x64/release_dll_mpir_gc)
  set (gmp_lib ${PROJECT_SOURCE_DIR}/mpir/x64/release_dll_mpir_sandybridge)
else ()
  set (build_lib_dir ${oorexx_build_dir}/retail32/bin)
  set (gmp_lib ${PROJECT_SOURCE_DIR}/mpir/win32/release_dll_mpir_gc)
endif ()

if (WIN32)
  set (PLATFORM_EXTRA  ${PROJECT_SOURCE_DIR}/mpz.def)
endif ()

add_library(mpz SHARED
  ${PLATFORM_EXTRA}
  ${PROJECT_SOURCE_DIR}/mpz)

# Include file definition
target_include_directories(mpz PUBLIC
  ${build_api_dir}
  ${build_api_platform_dir})

# Extra link library definitions
target_link_libraries(mpz
  ${build_lib_dir}/rexx.lib
  ${build_lib_dir}/rexxapi.lib
  ${gmp_lib}/mpir.lib)

