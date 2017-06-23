AC_PREREQ(2.58)
AC_INIT([zt820], [0.01], [Alexandre Mulatinho <alex@mulatinho.net>])

AM_INIT_AUTOMAKE([1.11 subdir-objects foreign])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/main.c])

AC_ARG_ENABLE(openssl,
	[AS_HELP_STRING([--enable-openssl[=PATH]],[enable use of openSSL])],
        openssl=$enableval, openssl_active=yes)

AS_IF([test "$openssl_active" != yes], [
	openssl_path=$openssl
    CFLAGS="$CFLAGS -L$openssl/lib -I$openssl/include"
    LDFLAGS="$LDFLAGS -lcrypto -lssl"
])

AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([pthread.h])
AC_CHECK_HEADER(openssl/ssl.h,,[AC_MSG_ERROR([You need the OpenSSL library to compile this software.])])
AC_HEADER_STDC

AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC

AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_LIB(ssl, SSL_library_init)

AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_LN_S
AC_PROG_INSTALL
#AC_PROG_INTLTOOL
AC_PROG_AWK
AC_PROG_MAKE_SET

AC_CONFIG_FILES([Makefile src/Makefile libs/Makefile tests/Makefile])
AC_OUTPUT()
