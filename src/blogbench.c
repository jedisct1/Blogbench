#include <config.h>
#define DEFINE_GLOBALS 1
#include "blogbench.h"
#ifndef HAVE_GETOPT_LONG
# include "bsd-getopt_long.h"
#else
# include <getopt.h>
#endif
#include "blogbench_p.h"

static void usage(void)
{
    puts("\n" PACKAGE_STRING " - " __DATE__ "\n");
    fputs("--commenters=<n> (-c <n>): number of comments posters\n"
          "--directory=<directory> (-d <directory>): working directory\n"
          "--help (-h): usage\n"
          "--iterations=<n> (-i <n>): number of iterations\n"
          "--readers=<n> (-r <n>): number of writers\n"
          "--rewriters=<n> (-W <n>): number of rewriters\n"
          "--sleep=<secs> (-s <secs>): delay after every iteration\n"
          "--writers=<n> (-w <n>): number of writers\n"
          "\n"
          "Sample usage:\n"
          "\n"
          "blogbench -d /mnt/bench\n"
          "\n\n"    
          "Please report bugs to ", stdout);
    puts(PACKAGE_BUGREPORT ".\n");

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int option_index = 0;
    int fodder;
    
    if (argc <= 1) {
        usage();
    }        
    while ((fodder = getopt_long(argc, argv, GETOPT_OPTIONS, long_options,
                                 &option_index)) != -1) {
        switch (fodder) {
        case 'h':
            usage();
        case 'c':
            nb_commenters = (unsigned int) strtoul(optarg, NULL, 10);
            break;
        case 'd':
            free(scratch_dir);
            if ((scratch_dir = strdup(optarg)) == NULL) {
                perror("Out of memory for strdup(<scratch dir>)");
                return 1;
            }
            break;
        case 'i':
            stats_iterations = (unsigned int) strtoul(optarg, NULL, 10);
            break;
        case 'r':
            nb_readers = (unsigned int) strtoul(optarg, NULL, 10);
            break;
        case 'W':
            nb_rewriters = (unsigned int) strtoul(optarg, NULL, 10);
            break;
        case 's':
            sleep_stats = (unsigned int) strtoul(optarg, NULL, 10);
            break;
        case 'w':
            nb_writers = (unsigned int) strtoul(optarg, NULL, 10);
            break;          
        default:
            usage();
        }
    }
    if (scratch_dir == NULL) {
        fputs("Missing scratch directory (--directory=... option).\n\n",
              stderr);
        return 1;
    }
    if (process() != 0) {
        return 1;
    }    
    return 0;
}

