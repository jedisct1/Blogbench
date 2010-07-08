#include <config.h>
#include "blogbench.h"

static int create_comments(const unsigned long long blog_id)
{
    char file_name[MAXPATHLEN];
    int comment_nb;
    
    while ((comment_nb = 1 + rand() % MAX_COMMENTS) <= 0);
    do {
        snprintf(file_name, sizeof file_name,
                 BLOG_PREFIX "%llu/" BLOG_SUFFIX
                 COMMENT_PREFIX "%d" COMMENT_SUFFIX,
                 blog_id, comment_nb);
        if (create_atomic_file(file_name,
                               COMMENT_MIN_SIZE, COMMENT_MAX_SIZE) != 0) {
            return -1;
        }
        stats_comments_write++;
    } while (--comment_nb != 0);
    
    return 0;
}

int create_random_comment(void)
{    
    if (create_comments(get_random_blog_id()) != 0) {
        return -1;
    }
    return 0;
}

void *commenter(void * const fodder)
{    
    (void) fodder;
    
    do {
        if (create_random_comment() != 0) {
            sleep(1);
        }
#if USLEEP_COMMENTER > 0
        sleep(USLEEP_COMMENTER);
#endif
    } while (stop == (sig_atomic_t) 0);
    
    return NULL;
}

