#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Check for OS
# AC_CANONICAL_HOST is needed to access the 'host_os' variable
AC_CANONICAL_HOST

build_linux=no
build_windows=no
build_mac=no
build_freebsd=no
build_netbsd=no

# Detect the target system
case "${host_os}" in
    linux*)
        AC_DEFINE([linux_HOST_OS], [1], [Define if host OS is linux.])
        build_linux=yes
        ;;
    cygwin*|mingw*)
        build_windows=yes
        ;;
    darwin*)
        AC_DEFINE([darwin_HOST_OS], [1], [Define if host OS is Darwin.])
        build_mac=yes
        ;;
    *freebsd*)
        AC_DEFINE([freebsd_HOST_OS], [1], [Define if host OS is FreeBSD.])
        build_freebsd=yes
        ;;
    *netbsd*)
        AC_DEFINE([netbsd_HOST_OS], [1], [Define if host OS is NetBSD.])
        build_netbsd=yes
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac

AC_CHECK_FUNCS([sched_getaffinity sysconf])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h sys/param.h unistd.h sys/cpuset.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.

AC_OUTPUT
