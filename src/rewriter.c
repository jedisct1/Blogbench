#include <config.h>
#include "blogbench.h"

static int modify_random_picture(const unsigned long long blog_id)
{
    char file_name[MAXPATHLEN];    
    int picture_id;
    
    while ((picture_id = 1 + rand() % MAX_PICTURES) <= 0);
    snprintf(file_name, sizeof file_name,
             BLOG_PREFIX "%llu/" BLOG_SUFFIX
             PICTURE_PREFIX "%d" PICTURE_SUFFIX,
             blog_id, picture_id);
    if (create_atomic_file(file_name,
                           PICTURE_MIN_SIZE, PICTURE_MAX_SIZE) != 0) {
        return -1;
    }
    stats_pictures_write++;

    return 0;
}

static int modify_random_article(const unsigned long long blog_id)
{
    char file_name[MAXPATHLEN];    
    int article_id;

    while ((article_id = 1 + rand() % MAX_ARTICLES) <= 0);
    snprintf(file_name, sizeof file_name,
             BLOG_PREFIX "%llu/" BLOG_SUFFIX
             ARTICLE_PREFIX "%d" ARTICLE_SUFFIX,
             blog_id, article_id);
    if (create_atomic_file(file_name,
                           ARTICLE_MIN_SIZE, ARTICLE_MAX_SIZE) != 0) {
        return -1;
    }
    stats_articles_write++;

    return 0;
}

static int modify_random_blog(void)
{
    unsigned long long blog_id;

    if ((blog_id = get_random_blog_id()) == 0ULL) {
        return -1;
    }
    if ((rand() & 1) == 0 && modify_random_picture(blog_id) != 0) {
        return -1;
    } else if (modify_random_article(blog_id) != 0) {
        return -1;
    }
    return 0;
}

void *rewriter(void * const fodder)
{    
    (void) fodder;
    
    do {
        if (modify_random_blog() != 0) {
            sleep(1);
        }
#if USLEEP_REWRITER > 0
        usleep(USLEEP_REWRITER);
#endif
    } while (stop == (sig_atomic_t) 0);
    
    return NULL;
}

