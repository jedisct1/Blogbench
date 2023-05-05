#include "blogbench.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

static int
read_articles(const unsigned long long blog_id)
{
    char file_name[MAXPATHLEN];
    int  article_nb = 1;

    for (;;) {
        snprintf(file_name, sizeof file_name,
                 BLOG_PREFIX "%llu/" BLOG_SUFFIX PICTURE_PREFIX "%d" PICTURE_SUFFIX, blog_id,
                 article_nb);
        if (read_dummy_file(file_name) == 0) {
            stats_pictures_read++;
        }
        snprintf(file_name, sizeof file_name,
                 BLOG_PREFIX "%llu/" BLOG_SUFFIX ARTICLE_PREFIX "%d" ARTICLE_SUFFIX, blog_id,
                 article_nb);
        if (read_dummy_file(file_name) != 0) {
            break;
        }
        stats_articles_read++;
        article_nb++;
    }
    return 0;
}

static int
read_comments(const unsigned long long blog_id)
{
    char file_name[MAXPATHLEN];
    int  comment_nb = 1;

    for (;;) {
        snprintf(file_name, sizeof file_name,
                 BLOG_PREFIX "%llu/" BLOG_SUFFIX COMMENT_PREFIX "%d" COMMENT_SUFFIX, blog_id,
                 comment_nb);
        if (read_dummy_file(file_name) != 0) {
            break;
        }
        stats_comments_read++;
        comment_nb++;
    }
    return 0;
}

static int
read_random_blog(void)
{
    unsigned long long blog_id;

    if ((blog_id = get_random_blog_id()) == 0ULL) {
        return -1;
    }
    if ((rand() & 1) == 0 && read_comments(blog_id) != 0) {
        return -1;
    } else if (read_articles(blog_id) != 0) {
        return -1;
    }
    stats_blogs_read++;

    return 0;
}

void *
reader(void *const fodder)
{
    (void) fodder;

    do {
        if (read_random_blog() != 0) {
            sleep(1);
        }
#if USLEEP_READER > 0
        usleep(USLEEP_READER);
#endif
    } while (stop == (sig_atomic_t) 0);

    return NULL;
}
