#ifndef __BLOGBENCH_P_H__
#define __BLOGBENCH_P_H__ 1

static const char *GETOPT_OPTIONS = "c:d:hi:r:W:s:w:";

static struct option long_options[] = {
    { "commenters", 1, NULL, 'c' },
    { "directory", 1, NULL, 'd' },
    { "help", 0, NULL, 'h' },    
    { "iterations", 1, NULL, 'i' },
    { "readers", 1, NULL, 'r' },
    { "rewriters", 0, NULL, 'W' },
    { "sleep", 1, NULL, 's' },
    { "writers", 1, NULL, 'w' },
    { NULL, 0, NULL, 0 }
};

#endif
