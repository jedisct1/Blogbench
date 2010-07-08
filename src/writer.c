#include <config.h>
#include "blogbench.h"

static int create_articles(char * const file_name,
                           const size_t sizeof_file_name,
                           const unsigned long long blog_id)
{
    int article_nb;
    
    while ((article_nb = 1 + rand() % MAX_ARTICLES) <= 0);
    do {
        snprintf(file_name, sizeof_file_name,
                 BLOG_PREFIX "%llu/" BLOG_SUFFIX
                 ARTICLE_PREFIX "%d" ARTICLE_SUFFIX,
                 blog_id, article_nb);
        if (create_atomic_file(file_name,
                               ARTICLE_MIN_SIZE, ARTICLE_MAX_SIZE) != 0) {
            return -1;
        }
        stats_articles_write++;
    } while (--article_nb != 0);
    
    return 0;
}

static int create_pictures(char * const file_name,
                           const size_t sizeof_file_name,
                           const unsigned long long blog_id)
{
    int picture_nb;
    
    while ((picture_nb = 1 + rand() % MAX_PICTURES) <= 0);
    do {
        snprintf(file_name, sizeof_file_name,
                 BLOG_PREFIX "%llu/" BLOG_SUFFIX
                 PICTURE_PREFIX "%d" PICTURE_SUFFIX,
                 blog_id, picture_nb);
        if (create_atomic_file(file_name,
                               PICTURE_MIN_SIZE, PICTURE_MAX_SIZE) != 0) {
            return -1;
        }
        stats_pictures_write++;
    } while (--picture_nb != 0);
    
    return 0;
}

static int create_blog(void)
{
    char file_name[MAXPATHLEN];
    unsigned long long blog_id = get_new_blog_id();

    snprintf(file_name, sizeof file_name,
             BLOG_PREFIX "%llu" BLOG_SUFFIX, blog_id);
    if (mkdir(file_name, (mode_t) 0700) != 0 &&
        errno != EEXIST) {
        reentrant_perror("mkdir()");
        sleep(1);       
        return -1;
    }
    if (create_articles(file_name, sizeof file_name, blog_id) != 0 ||
        create_pictures(file_name, sizeof file_name, blog_id) != 0) {
        return -1;
    }
    stats_blogs_write++;
    
    return 0;
}

void *writer(void * const fodder)
{    
    (void) fodder;
    
    do {
        if (create_blog() != 0) {
            sleep(1);
        }
#if USLEEP_WRITER > 0
        usleep(USLEEP_WRITER);
#endif
    } while (stop == (sig_atomic_t) 0);
    
    return NULL;
}

