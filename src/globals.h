#ifndef __GLOBALS_H__
#define __GLOBALS_H__ 1

#ifdef DEFINE_GLOBALS
# define GLOBAL0(A) A
# define GLOBAL(A, B) A = B
#else
# define GLOBAL0(A) extern A
# define GLOBAL(A, B) extern A
#endif

GLOBAL(pthread_mutex_t mx_next_blog_id, PTHREAD_MUTEX_INITIALIZER);
GLOBAL(unsigned long long next_blog_id, 0ULL);

/* XXX - FIXME : these counters should be atomic */

GLOBAL(unsigned long long stats_blogs_read, 0ULL);
GLOBAL(unsigned long long stats_blogs_write, 0ULL);

GLOBAL(unsigned long long stats_articles_read, 0ULL);
GLOBAL(unsigned long long stats_articles_write, 0ULL);

GLOBAL(unsigned long long stats_pictures_read, 0ULL);
GLOBAL(unsigned long long stats_pictures_write, 0ULL);

GLOBAL(unsigned long long stats_comments_read, 0ULL);
GLOBAL(unsigned long long stats_comments_write, 0ULL);

/* XXX - END OF FIXME : no more need for atomic operations */

GLOBAL(unsigned long long stats_total_blogs_read, 0ULL);
GLOBAL(unsigned long long stats_total_blogs_write, 0ULL);

GLOBAL(sig_atomic_t stop, (sig_atomic_t) 0);

GLOBAL(unsigned int nb_commenters, DEFAULT_NB_COMMENTERS);
GLOBAL(char *scratch_dir, DEFAULT_SCRATCH_DIR);
GLOBAL(unsigned int stats_iterations, DEFAULT_STATS_ITERATIONS);
GLOBAL(unsigned int nb_readers, DEFAULT_NB_READERS);
GLOBAL(unsigned int nb_rewriters, DEFAULT_NB_REWRITERS);
GLOBAL(unsigned int sleep_stats, DEFAULT_SLEEP_STATS);
GLOBAL(unsigned int nb_writers, DEFAULT_NB_WRITERS);

#endif
