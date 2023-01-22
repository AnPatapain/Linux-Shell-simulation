#include "mini_lib.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        mini_perror("argument error in mini_chmod");
    }

    mode_t permission = (mode_t) mini_atoi(argv[1], 8);

    const char* file_name = argv[2];

    if(chmod(file_name, permission) == -1) {
        mini_perror("chmod func error in mini_chmod.c");
    }

    mini_printf("\nfile permission changed\n");

    return 0;
}