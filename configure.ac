AC_INIT([yummy], [1.0], [mateus@example.org])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

#AM_INIT_AUTOMAKE([-Wall -Werror])
#LT_INIT

AC_PROG_CC
PKG_PROG_PKG_CONFIG
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CHECK_HEADERS([windows.h zlib.h png.h expat.h])
AC_CHECK_TOOL([LD], [ld])
AC_CHECK_TOOL([AR], [ar])
# TODO: check if it's mingw.
AC_CHECK_TOOL([WINDRES], [windres], AC_MSG_ERROR("windres was not found"))
AC_CHECK_TOOL([DLLTOOL], [dlltool], AC_MSG_ERROR("dlltool was not found"))
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

LIBS+=''

# autoconf does not detect stdcall
#AC_CHECK_LIB([kernel32], [HeapAlloc])
#AC_CHECK_LIB([user32], [CreateWindowExA])
#AC_CHECK_LIB([gdi32], [BitBlt])

#AC_CHECK_LIB([z], [inflate])
#AC_CHECK_LIB([png], [png_read_image])
#AC_CHECK_LIB([expat], [XML_Parse])
#AC_CHECK_LIB([jpeg], [jpeg_create_decompress])
#AC_CHECK_LIB([gif], [DGifGetLine])
#AC_CHECK_LIB([tiff], [TIFFReadRGBAImage])
#AC_CHECK_LIB([avformat], [av_register_all])

PKG_CHECK_MODULES([ZLIB], [zlib])
PKG_CHECK_MODULES([LIBPNG], [libpng])
PKG_CHECK_MODULES([EXPAT], [expat])
#PKG_CHECK_MODULES([LIBAVFORMAT], [libavformat])
#PKG_CHECK_MODULES([LIBAVCODEC], [libavcodec])
#PKG_CHECK_MODULES([LIBAVFILTER], [libavfilter])
#PKG_CHECK_MODULES([AO], [ao])
#PKG_CHECK_MODULES([OPENAL], [openal])

AC_OUTPUT
