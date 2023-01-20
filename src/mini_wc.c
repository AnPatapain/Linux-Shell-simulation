#include "mini_lib.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        mini_perror("argument error in mini_touch");
    }
    int count;
    if(argv[1] != NULL) {
        mini_wc(argv[1]);
    }

    return 0;
}