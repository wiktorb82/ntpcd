dnl Process this file with autoconf to produce a configure script.

AC_INIT(udpdc.c)
AM_INIT_AUTOMAKE(udpdc, 0.1.0)
AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/param.h sys/time.h time.h sys/mkdev.h sys/sysmacros.h string.h memory.h fcntl.h dirent.h sys/ndir.h ndir.h alloca.h locale.h )

AC_HEADER_MAJOR
AC_FUNC_ALLOCA
AC_STRUCT_TM
AC_STRUCT_ST_BLOCKS
AC_FUNC_CLOSEDIR_VOID

AC_OUTPUT(Makefile)
