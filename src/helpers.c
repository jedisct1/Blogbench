#include <config.h>
#include "blogbench.h"

unsigned long long get_new_blog_id(void)
{
    unsigned long long blog_id;

    pthread_mutex_lock(&mx_next_blog_id);
    blog_id = next_blog_id;
    next_blog_id++;
    pthread_mutex_unlock(&mx_next_blog_id);
    
    return blog_id;
}

unsigned long long get_last_blog_id(void)
{
    unsigned long long blog_id;

    pthread_mutex_lock(&mx_next_blog_id);
    blog_id = next_blog_id;
    pthread_mutex_unlock(&mx_next_blog_id);
    if (blog_id == 0ULL) {
        return 0;
    }
    return blog_id - 1ULL;
}

unsigned long long get_random_blog_id(void)
{
    unsigned long long last_blog_id;
    unsigned long long rnd;

    if ((last_blog_id = get_last_blog_id()) <= 0ULL) {
        return 0ULL;
    }
    rnd = (unsigned long long) rand() ^
        ((unsigned long long) rand() << 16) ^
        ((unsigned long long) rand() << 32) ^
        ((unsigned long long) rand() << 48);
    
    return rnd % last_blog_id;
}

int create_dummy_file(const char * const file_name, size_t size)
{
    static char garbage[WRITE_CHUNK_SIZE];
    size_t towrite;
    ssize_t written;    
    int fd;
    
    if ((fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY,
                   (mode_t) 0600)) == -1) {
        if (errno != ENOENT) {      
            reentrant_printf("open(\"%s\"): %s\n",
                             file_name, strerror(errno));
        }
        return -1;
    }
    while (size > (size_t) 0U) {
        if (sizeof garbage >= size) {
            towrite = size;
        } else {
            towrite = sizeof garbage;
        }
        while ((written = write(fd, garbage, towrite)) < (ssize_t) 0 &&
               errno == EINTR);
        if (written < (ssize_t) 0) {
            reentrant_perror("write()");
            (void) close(fd);
            (void) unlink(file_name);
            return -1;
        }
        size -= written;
    }
    if (close(fd) != 0) {
        reentrant_perror("close()");
        (void) unlink(file_name);
        return -1;
    }
    return 0;
}

int create_atomic_file(const char * const file_name,
                       const size_t article_min_size,
                       const size_t article_max_size)
{
    char tmp_file_name[MAXPATHLEN];
    size_t size;
    
    if (article_min_size > article_max_size) {
        abort();
    }
    snprintf(tmp_file_name, sizeof tmp_file_name,
             "%s" TMP_SUFFIX, file_name);
    size = article_min_size + rand() % (article_max_size - article_min_size);
    if (create_dummy_file(tmp_file_name, size) != 0) {
        return -1;
    }
    if (rename(tmp_file_name, file_name) != 0) {
        (void) unlink(tmp_file_name);
        (void) unlink(file_name);
        return -1;
    }    
    return 0;
}

int read_dummy_file(const char * const file_name)
{
    static char garbage[READ_CHUNK_SIZE];
    ssize_t readen;
    int fd;
    
    if ((fd = open(file_name, O_RDONLY)) == -1) {
        return -1;
    }
    do {
        if ((readen = read(fd, garbage, sizeof garbage))
            < (ssize_t) 0) {
            if (errno == EINTR) {
                continue;
            }
            reentrant_perror("read()");
            break;
        }
    } while (readen > (ssize_t) 0);
    if (close(fd) != 0) {
        reentrant_perror("close()");
        return -1;
    }
    return 0;
}

static int safe_write(const int fd, const void *buf_, size_t count)
{
    ssize_t written;
    register const char *buf = (const char *) buf_;
    
    while (count > (size_t) 0U) {
        for (;;) {
            if ((written = write(fd, buf, count)) <= (ssize_t) 0) {
                if (errno == EAGAIN) {
                    sleep(1);
                } else if (errno != EINTR) {
                    return -1;
                }
                continue;
            }
            break;
        }
        buf += written;
        count -= written;
    }
    return 0;
}

static int write_str(const char * const str)
{
    return safe_write(1, str, strlen(str));
}

int reentrant_puts(const char * const str)
{
    char line[MAX_OUTPUT_LINE];
    
    strncpy(line, str, sizeof line);
    line[sizeof line - (size_t) 1U] = 0;
    write_str(line);
    write_str("\n");
    
    return 0;
}

int reentrant_printf(const char * const format, ...)
{
    char line[MAX_OUTPUT_LINE];
    va_list va;
    
    va_start(va, format);
    vsnprintf(line, sizeof line, format, va);
    write_str(line);
    va_end(va);
    
    return 0;
}

int reentrant_perror(const char * const str)
{
    return reentrant_printf("%s: %s", str, strerror(errno));
}
