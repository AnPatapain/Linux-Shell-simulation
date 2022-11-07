#include "mini_lib.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define IOBUFFER_SIZE 2048

struct MYFILE {
    int fd;
    void *buffer_read;
    void *buffer_write;
    int ind_read;
    int ind_write;
};

struct MYFILE *mini_open(char *file, char mode) {

    if(file == NULL) {
        perror("ERROR");
        mini_exit();
    }

    int file_descriptor;

    switch (mode)
    {
    case 'r':
        file_descriptor =  open(file, O_RDONLY);
        if (file_descriptor == -1) {
            mini_exit();
        }
        break;
    case 'w':
        file_descriptor = open(file, O_WRONLY);
        if (file_descriptor == -1) {
            mini_exit();
        }
        break;
    case 'b':
        file_descriptor = open(file, O_RDWR);
        if (file_descriptor == -1) {
            mini_exit();
        }
        break;
    case 'a':
        file_descriptor = open(file, O_APPEND);
        if (file_descriptor == -1) {
            mini_exit();
        }
        break;
    default:
        perror("ERROR");
        mini_exit();
        break;
    }

    struct MYFILE* myFile = mini_calloc(sizeof(struct MYFILE), 1);

    myFile->fd = file_descriptor;
    myFile->ind_read = -1;
    myFile->ind_write = -1;

    return myFile;

}