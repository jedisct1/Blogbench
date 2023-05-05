#ifndef __BLOGBENCH_H__
#define __BLOGBENCH_H__ 1

#ifndef __GNUC__
# ifdef __attribute__
#  undef __attribute__
# endif
# define __attribute__(a)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <time.h>
#if HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#elif defined(HAVE_SYS_FCNTL_H)
# include <sys/fcntl.h>
#endif
#ifdef HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif
#include <pthread.h>
#include <fcntl.h>

#ifdef HAVE_ALLOCA
# ifdef HAVE_ALLOCA_H
#  include <alloca.h>
# endif
# define ALLOCA(X) alloca(X)
# define ALLOCA_FREE(X) do { } while (0)
#else
# define ALLOCA(X) malloc(X)
# define ALLOCA_FREE(X) free(X)
#endif

#ifndef HAVE_STRTOULL
# ifdef HAVE_STRTOQ
#  define strtoull(X, Y, Z) strtoq(X, Y, Z)
# else
#  define strtoull(X, Y, Z) strtoul(X, Y, Z)
# endif
#endif

#ifndef ULONG_LONG_MAX
# define ULONG_LONG_MAX (1ULL << 63)
#endif

#ifndef O_NOFOLLOW
# define O_NOFOLLOW 0
#endif

#ifndef O_DIRECTORY
# define O_DIRECTORY 0
#endif

#define DEFAULT_SCRATCH_DIR NULL

#define DEFAULT_NB_WRITERS 3
#define DEFAULT_NB_REWRITERS 1
#define DEFAULT_NB_READERS 100
#define DEFAULT_NB_COMMENTERS 5

#define USLEEP_WRITERS 10000
#define USLEEP_REWRITERS 1000000
#define USLEEP_READERS 0
#define USLEEP_COMMENTERS 10000

#define DEFAULT_SLEEP_STATS 10
#define DEFAULT_STATS_ITERATIONS 30

#define MAX_ARTICLES 100
#define ARTICLE_MIN_SIZE 1024
#define ARTICLE_MAX_SIZE 32768

#define MAX_PICTURES 100
#define PICTURE_MIN_SIZE 8192
#define PICTURE_MAX_SIZE 65536

#define MAX_COMMENTS 100
#define COMMENT_MIN_SIZE 2048
#define COMMENT_MAX_SIZE 10240

#define WRITE_CHUNK_SIZE 8192
#define READ_CHUNK_SIZE 65536

#define BLOG_PREFIX "blog-"
#define BLOG_SUFFIX ""

#define ARTICLE_PREFIX "article-"
#define ARTICLE_SUFFIX ".xml"

#define PICTURE_PREFIX "picture-"
#define PICTURE_SUFFIX ".jpg"

#define COMMENT_PREFIX "comment-"
#define COMMENT_SUFFIX ".xml"

#define TMP_SUFFIX ".tmp"

#ifndef MAXPATHLEN
# ifdef PATH_MAX
#  define MAXPATHLEN PATH_MAX
# else
#  define MAXPATHLEN 65536U
Warning: neither PATH_MAX nor MAXPATHLEN were found.
Remove these lines if you really want to compile the server, but
the server may be insecure if a wrong value is set here.
# endif
#endif
#if (MAXPATHLEN) >= (INT_MAX)
Your platform has a very large MAXPATHLEN, we should not trust it.
#endif

#define MAX_OUTPUT_LINE 4096U

int process(void);

unsigned long long get_new_blog_id(void);
unsigned long long get_last_blog_id(void);
unsigned long long get_random_blog_id(void);
int reentrant_puts(const char * const str);
int reentrant_printf(const char * const format, ...);
int reentrant_perror(const char * const str);

int create_atomic_file(const char * const file_name,
                       const size_t article_min_size,
                       const size_t article_max_size);

int create_dummy_file(const char * const file_name, size_t size);
int read_dummy_file(const char * const file_name);

void *writer(void * const fodder);
void *rewriter(void * const fodder);
void *reader(void * const fodder);
void *commenter(void * const fodder);

#include "globals.h"

#endif
