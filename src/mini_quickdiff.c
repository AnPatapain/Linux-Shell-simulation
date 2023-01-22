#include "mini_lib.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        mini_perror("argument error in mini_quickdiff.c");
    }

    mini_quickdiff(argv[1], argv[2]);
    return 0;
}
