#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([RomNumCalcApp], [0.0.1], [hndrcksn@italcomputations.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC
AM_PROG_AR
LT_INIT

# Look for Check
PKG_CHECK_MODULES([CHECK], [check])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/app/Makefile
                 src/lib/Makefile
                 src/tests/Makefile])
AC_OUTPUT
