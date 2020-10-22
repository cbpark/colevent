#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([colevent], [0.1.0.0], [cbpark@gmail.com])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config/config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX(17,noext,mandatory)
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT

AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_LANG([C++])
AC_CHECK_FUNCS([sqrt],,AC_MSG_ERROR([Required math functions not found]))

# ROOT (http://root.cern.ch/)
ROOT_PATH(,AC_MSG_NOTICE([ROOT $ROOTVERSION is found])
           AC_DEFINE([HAVE_ROOT], [1], [Define to 1 if you have ROOT])
           HAVE_ROOT=1, HAVE_ROOT=0)
AM_CONDITIONAL([USE_ROOT], [test "$HAVE_ROOT" -eq 1])

AC_ARG_ENABLE([debug],
[  --enable-debug          turn on debugging],
[case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AC_CONFIG_FILES([colevent.pc Makefile src/Makefile])
PKG_INSTALLDIR

AC_OUTPUT
