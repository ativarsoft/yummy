AC_INIT([yummy], [1.0], [m4t3uz@outlook.com])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

#AM_INIT_AUTOMAKE([-Wall -Werror])
#LT_INIT

AC_PROG_CC
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CHECK_HEADERS([zlib.h png.h expat.h])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CHECK_TOOL([LD], [ld])
AC_CHECK_TOOL([AR], [ar])
if test "$host_os" == 'mingw32' || test "$host_os" == 'cygwin'
then
AC_CHECK_HEADERS([windows.h])
AC_CHECK_TOOL([WINDRES], [windres], AC_MSG_ERROR("windres was not found"))
AC_CHECK_TOOL([DLLTOOL], [dlltool], AC_MSG_ERROR("dlltool was not found"))
fi
if test "$host_os" == 'linux' || test "$host_os" == 'linux-gnu'
then
AC_CHECK_LIB([dl], [dlopen])
fi

# detect dependencies
m4_include([.config])
PKG_CHECK_MODULES([ZLIB], [zlib])
PKG_CHECK_MODULES([LIBPNG], [libpng])
PKG_CHECK_MODULES([EXPAT], [expat])
if test "$CONFIG_IN_LIBAV" == 'y' || test "$CONFIG_IN_LIBAV" == 'm'
then
PKG_CHECK_MODULES([LIBAVFORMAT], [libavformat])
PKG_CHECK_MODULES([LIBAVCODEC], [libavcodec])
PKG_CHECK_MODULES([LIBAVFILTER], [libavfilter])
fi
if test "$CONFIG_OUT_AO" == 'y' || test "$CONFIG_OUT_AO" == 'm'
then
PKG_CHECK_MODULES([AO], [ao])
fi
if test "$CONFIG_OUT_OPENAL" == 'y' || test "$CONFIG_OUT_OPENAL" == 'm'
then
PKG_CHECK_MODULES([OPENAL], [openal])
fi

# dependencies for the final executable
LIBS="$LIBS $ZLIB_LIBS $LIBPNG_LIBS $EXPAT_LIBS"
if test "$host_os" == 'mingw32' || test "$host_os" == 'cygwin'
then
LIBS="$LIBS -lgdi32 -lcomctl32"
#LIBS="$LIBS -lole32 -loleaut32 /lib/w32api/libuuid.a"
fi
if test "$host_os" == 'linux' || test "$host_os" == 'linux-gnu'
then
PKG_CHECK_MODULES([X11], [x11])
CFLAGS="$CFLAGS -fPIC"
LIBS="$LIBS -ldl $X11_LIBS"
fi

AC_OUTPUT
