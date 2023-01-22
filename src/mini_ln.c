#include "mini_lib.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        mini_perror("argument error in mini_chmod");
    }

    const char* source = argv[1];
    const char* dest = argv[2];

    if (symlink(source, dest) == -1) {
        mini_perror("symlink error in mini_ln.c");
    }

    return 0;
}