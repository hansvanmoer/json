#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([json], [0.1], [hansvm.projects@gmail.com])
AC_CONFIG_SRCDIR([src/main/JSON.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_PROG_RANLIB

#prepares for automake
AM_INIT_AUTOMAKE

AC_CONFIG_FILES([
	Makefile
	src/main/Makefile
        src/test/flags/Makefile
	src/test/read/Makefile
        src/test/tokens/Makefile
        src/test/type_policy/Makefile
        src/test/storage_policy/Makefile
        src/test/input_buffer/Makefile
])

AC_OUTPUT
