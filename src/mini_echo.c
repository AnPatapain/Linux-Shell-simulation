#include "mini_lib.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        mini_perror("Invalid number of arguments");
    }
    for (int i = 1; i < argc; ++i) {
        mini_printf(argv[i]);
        mini_printf(" ");
    }
    mini_printf("\n");
    mini_exit();
}