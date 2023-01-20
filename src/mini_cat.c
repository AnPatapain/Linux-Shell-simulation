#include "mini_lib.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        mini_perror("argument error in mini_cat");
    }
    struct MYFILE *file = mini_fopen(argv[1], 'b');
    char c = mini_fgetc(file);

    while(c != -1) {
        char *buffer = mini_calloc(sizeof(char), 2);
        buffer[0] = c;
        mini_printf(buffer);
        c = mini_fgetc(file);
    }
    
    return 0;
}