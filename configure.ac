AC_PREREQ([2.71])

AC_INIT([blogbench],[1.2],[https://github.com/jedisct1/Blogbench])
AM_INIT_AUTOMAKE([1.8 dist-bzip2])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_LIBOBJ_DIR(src)

AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_USE_SYSTEM_EXTENSIONS
AC_SEARCH_LIBS([strerror],[cposix])

AC_CHECK_HEADERS_ONCE([sys/time.h])
AC_CHECK_HEADERS_ONCE([unistd.h])
AC_CHECK_HEADERS_ONCE([sys/socket.h sys/time.h])
AC_CHECK_HEADERS_ONCE([fcntl.h sys/fcntl.h sys/param.h])

dnl Types
AC_CHECK_TYPE(sig_atomic_t, , [AC_DEFINE(sig_atomic_t, signed char,
                                         [sig_atomic_t type])],
[
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <sys/types.h>
#include <signal.h>
])

dnl Options

AC_ARG_WITH(debug,
[AS_HELP_STRING(--with-debug,For maintainers only - please do not use)],
[ if test "x$withval" = "xyes" ; then
    CFLAGS="$CFLAGS -DDEBUG=1 -g -Wall -W -Wcast-align -Wbad-function-cast -Wstrict-prototypes -Wwrite-strings -Wreturn-type "
  fi ])

dnl Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MEMCMP
AC_FUNC_STRFTIME
AC_FUNC_STAT
AC_FUNC_VPRINTF
AX_PTHREAD([
  LIBS="$PTHREAD_LIBS $LIBS"
  CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
  CC="$PTHREAD_CC"
  CXX="$PTHREAD_CXX"
])

AC_CHECK_FUNCS(setrlimit setproctitle getopt_long)
AC_CHECK_FUNCS(strtoull strtoq)
AC_CHECK_FUNCS(gethostname)
AC_CHECK_FUNCS(setlocale)
AC_CHECK_FUNCS(sync)

AC_SYS_LARGEFILE

dnl Output

AC_CONFIG_FILES(Makefile src/Makefile man/Makefile)

AC_OUTPUT
