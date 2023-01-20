#include "mini_lib.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        mini_perror("argument error in mini_cp");
    }
    struct MYFILE* src_file = mini_fopen(argv[1], 'r');
    struct MYFILE* dst_file = mini_fopen(argv[2], 'w');

    char c = mini_fgetc(src_file);
    
    while(c != -1) {
        mini_fputc(dst_file, c);
        c = mini_fgetc(src_file);
    }
    mini_exit_io();
    return 0;
}