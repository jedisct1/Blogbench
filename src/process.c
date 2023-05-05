#include "blogbench.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

static pthread_t *writers_threads;
static pthread_t *rewriters_threads;
static pthread_t *readers_threads;
static pthread_t *commenters_threads;

static int
spawn_writers(void)
{
    unsigned int t = nb_writers;

    do {
        t--;
        if (pthread_create(&writers_threads[t], NULL, writer, NULL) != 0) {
            return -1;
        }
    } while (t != 0U);

    return 0;
}

static int
wait_writers(void)
{
    unsigned int t = nb_writers;

    do {
        t--;
        pthread_join(writers_threads[t], NULL);
    } while (t != 0U);

    return 0;
}

static int
spawn_rewriters(void)
{
    unsigned int t = nb_rewriters;

    do {
        t--;
        if (pthread_create(&rewriters_threads[t], NULL, rewriter, NULL) != 0) {
            return -1;
        }
    } while (t != 0U);

    return 0;
}

static int
wait_rewriters(void)
{
    unsigned int t = nb_rewriters;

    do {
        t--;
        pthread_join(rewriters_threads[t], NULL);
    } while (t != 0U);

    return 0;
}

static int
spawn_readers(void)
{
    unsigned int t = nb_readers;

    do {
        t--;
        if (pthread_create(&readers_threads[t], NULL, reader, NULL) != 0) {
            return -1;
        }
    } while (t != 0U);

    return 0;
}

static int
wait_readers(void)
{
    unsigned int t = nb_readers;

    do {
        t--;
        pthread_join(readers_threads[t], NULL);
    } while (t != 0U);

    return 0;
}

static int
spawn_commenters(void)
{
    unsigned int t = nb_commenters;

    do {
        t--;
        if (pthread_create(&commenters_threads[t], NULL, commenter, NULL) != 0) {
            return -1;
        }
    } while (t != 0U);

    return 0;
}

static int
wait_commenters(void)
{
    unsigned int t = nb_commenters;

    do {
        t--;
        pthread_join(commenters_threads[t], NULL);
    } while (t != 0U);

    return 0;
}

static void
show_grand_total(void)
{
    reentrant_printf(
        "\n"
        "Final score for writes:%14llu\n"
        "Final score for reads :%14llu\n\n",
        stats_total_blogs_write, stats_total_blogs_read);
}

static int
init_threads_area(void)
{
    if ((writers_threads = malloc(nb_writers * sizeof writers_threads[0])) == NULL ||
        (rewriters_threads = malloc(nb_rewriters * sizeof rewriters_threads[0])) == NULL ||
        (readers_threads = malloc(nb_readers * sizeof readers_threads[0])) == NULL ||
        (commenters_threads = malloc(nb_commenters * sizeof commenters_threads[0])) == NULL) {
        reentrant_perror("Out of memory to create threads");
        return -1;
    }
    return 0;
}

int
process(void)
{
    unsigned int iterations = stats_iterations;

    if (init_threads_area() != 0) {
        return -1;
    }
    reentrant_printf(
        "\n"
        "Frequency = %u secs\n"
        "Scratch dir = [%s]\n",
        sleep_stats, scratch_dir);
    if (chdir(scratch_dir) != 0) {
        reentrant_printf("Unable to enter the scratch dir [%s]: [%s]\n", scratch_dir,
                         strerror(errno));
        return -1;
    }
#ifdef HAVE_SYNC
    sync();
#endif
    reentrant_printf("Spawning %d writers...\n", nb_writers);
    if (spawn_writers() != 0) {
        reentrant_perror("Unable to create writers");
        return -1;
    }
    reentrant_printf("Spawning %d rewriters...\n", nb_rewriters);
    if (spawn_rewriters() != 0) {
        reentrant_perror("Unable to create rewriters");
        return -1;
    }
    reentrant_printf("Spawning %d commenters...\n", nb_commenters);
    if (spawn_commenters() != 0) {
        reentrant_perror("Unable to create commenters");
        return -1;
    }
    reentrant_printf("Spawning %d readers...\n", nb_readers);
    if (spawn_readers() != 0) {
        reentrant_perror("Unable to create readers");
        return -1;
    }
    reentrant_printf(
        "Benchmarking for %u iterations.\n"
        "The test will run during %u minutes.\n",
        stats_iterations, stats_iterations * sleep_stats / 60U);
    reentrant_puts(
        "\n"
        "  Nb blogs   R articles    W articles    R pictures"
        "    W pictures    R comments    W comments");
    do {
        sleep(sleep_stats);

        stats_total_blogs_read += stats_blogs_read;
        stats_total_blogs_write += stats_blogs_write;

        reentrant_printf("%10llu %12llu %13llu %13llu %13llu %13llu %13llu\n",
                         stats_total_blogs_write, stats_articles_read, stats_articles_write,
                         stats_pictures_read, stats_pictures_write, stats_comments_read,
                         stats_comments_write);

        stats_blogs_read = stats_blogs_write = 0ULL;
        stats_articles_read = stats_articles_write = 0ULL;
        stats_pictures_read = stats_pictures_write = 0ULL;
        stats_comments_read = stats_comments_write = 0ULL;
    } while (--iterations != 0U && stop == (sig_atomic_t) 0);
    stop++;
    wait_commenters();
    wait_readers();
    wait_rewriters();
    wait_writers();
    show_grand_total();

    return 0;
}
