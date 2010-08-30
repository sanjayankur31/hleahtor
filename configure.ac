dnl configure.ac for hleahtor
dnl Author : Ankur Sinha 

AC_INIT([hleahtor],[0.2.2],[sanjay.ankur@gmail.com])
AM_INIT_AUTOMAKE([-W -Wall -Werror dist-bzip2 dist-zip check-news])    
HLEAHTOR_MAJOR_VERSION=0
HLEAHTOR_MINOR_VERSION=2
HLEAHTOR_MICRO_VERSION=2

HLEAHTOR_VERSION=$HLEAHTOR_MAJOR_VERSION.$HLEAHTOR_MINOR_VERSION.$HLEAHTOR_MICRO_VERSION

VERSION=$HLEAHTOR_VERSION
PACKAGE=hleahtor

AC_CONFIG_SRCDIR([src/hleahtor.c])    

AM_MAINTAINER_MODE    
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

dnl AC_CHECK_LIB([config],[config_init],,[AC_MSG_ERROR([Could not find libconfig Please read the install notes])])
dnl AC_CHECK_LIB([curl],[curl_easy_init],,[AC_MSG_ERROR([Could not find libcurl Please read the install notes])])
dnl AC_CHECK_LIB([notify],[notify_init],,[AC_MSG_ERROR([Could not find libnotify Please read the install notes])])

PKG_CHECK_MODULES(PACKAGE, libcurl libnotify libconfig)
AC_SUBST(PACKAGE_CFLAGS)    
AC_SUBST(PACKAGE_LIBS)    

AC_CONFIG_FILES([Makefile src/Makefile data/Makefile])    
AC_OUTPUT
    
    
