#include "mini_lib.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    struct dirent* dp;
    DIR *dir = opendir(".");
    struct stat *stat_buf = mini_calloc(sizeof(struct stat), 1);

    while((dp = readdir(dir)) != NULL ) {
        if(stat(dp->d_name, stat_buf) == -1) {
            mini_perror("error in mini_ls.c");
        }
        mini_print_info(stat_buf, dp->d_name);
    }

    closedir(dir);
    return 0;
}