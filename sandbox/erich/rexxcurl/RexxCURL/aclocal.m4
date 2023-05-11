dnl
dnl include our own common routines
dnl
sinclude(common/accommon.m4)

dnl ---------------------------------------------------------------------------
dnl Shows where cURL header/libraries are
dnl Called from MH_SHOW_STATUS
dnl ---------------------------------------------------------------------------
AC_DEFUN([MH_SHOW_CURL],
[
echo "                     cURL version: $MH_PACKAGE_VER (using curl-config)"
echo "              cURL compiler flags: $MH_PACKAGE_INC (using curl-config)"
echo "                   cURL libraries: $MH_PACKAGE_LIBS (using curl-config)"
])
