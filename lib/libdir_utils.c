#include "../include/dir_utils.h"
# if defined(__unix__)
        extern int make_dir(const char *path) { return mkdir(path, S_IRWXU | S_IRGRP | S_IROTH); }
# elif defined(_WIN32)
        extern int make_dir(const char *path) { return mkdir(path); }
# endif
