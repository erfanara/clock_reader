#include "../include/dir_utils.h"
# if defined(__unix__)
        extern int make_dir(char *path) { return mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO); }
# elif defined(_WIN32)
        extern int make_dir(char *path) { return mkdir(path); }
# endif
