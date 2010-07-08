AC_PREREQ(2.58)

AC_INIT([blogbench],[1.1],[j@pureftpd.org])
AM_INIT_AUTOMAKE([1.8 dist-bzip2])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_LIBOBJ_DIR(src)

AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_AIX
AC_ISC_POSIX

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([memory.h strings.h unistd.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([stdlib.h string.h sys/socket.h sys/time.h])
AC_CHECK_HEADERS([fcntl.h sys/fcntl.h sys/param.h ioctl.h sys/ioctl.h])

dnl Check for types
AC_TYPE_SIGNAL
AC_TYPE_SIZE_T
AC_CHECK_TYPE(dev_t, , [AC_DEFINE(dev_t, unsigned int, [dev_t type])],
[
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
])

dnl Check for sizes
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)

dnl Types - continued

AC_CHECK_TYPE(ssize_t, , [AC_DEFINE(ssize_t, long int, [ssize_t type])],
[
#include <stdio.h>
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <sys/types.h>
])

AC_CHECK_TYPE(sig_atomic_t, , [AC_DEFINE(sig_atomic_t, signed char,
                                         [sig_atomic_t type])],
[
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <sys/types.h>
#include <signal.h>
])

dnl Check for typedefs, structures, and compiler characteristics.
AC_PROG_GCC_TRADITIONAL
AC_C_CONST
AC_C_INLINE

dnl Options

AC_ARG_WITH(debug,
[AS_HELP_STRING(--with-debug,For maintainers only - please do not use)],
[ if test "x$withval" = "xyes" ; then
    CFLAGS="$CFLAGS -DDEBUG=1 -g -Wall -W -Wcast-align -Wbad-function-cast -Wstrict-prototypes -Wwrite-strings -Wreturn-type "
  fi ])

AC_ARG_WITH(largefile,
[AS_HELP_STRING(--with-largefile,Support for files > 2 Gb)],
[ if test "x$withval" = "xyes" ; then
    AC_SYS_LARGEFILE
    AC_DEFINE(WITH_LARGE_FILES,,[with largefile])
  fi ])

dnl Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MEMCMP
AC_FUNC_STRFTIME
AC_FUNC_STAT
AC_FUNC_VPRINTF
ACX_PTHREAD

AC_CHECK_FUNCS(setrlimit setproctitle getopt_long)
AC_CHECK_FUNCS(strtoull strtoq)
AC_CHECK_FUNCS(snprintf vsnprintf vfprintf gethostname)
AC_CHECK_FUNCS(setlocale)
AC_CHECK_FUNCS(sync)

AC_MSG_CHECKING(whether snprintf is C99 conformant)
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdio.h>
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

int main(void)
{
    char buf[4];
    
    (void) fprintf(fopen("conftestval", "w"), "%d\n",
        (int) snprintf(buf, sizeof buf, "12345678"));
    return 0;
}
]])],[CONF_SNPRINTF_TYPE=`cat conftestval`
],[],[])
AC_MSG_RESULT(done)
if test "x$CONF_SNPRINTF_TYPE" = "x" ; then
  AC_MSG_WARN(your operating system doesn't implement snprintf)
else
  AC_DEFINE_UNQUOTED(CONF_SNPRINTF_TYPE, $CONF_SNPRINTF_TYPE,
                     [return value of an overflowed snprintf])
fi

LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CC="$PTHREAD_CC"

dnl Output

AC_CONFIG_FILES(Makefile src/Makefile man/Makefile)

AC_OUTPUT
