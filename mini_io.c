#include "mini_lib.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define IOBUFFER_SIZE 2048

struct FILE_elm_list* file_list = NULL;

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
            perror("error open file");
            mini_exit();
        }
        break;
    case 'w':
        file_descriptor = open(file, O_WRONLY);
        if (file_descriptor == -1) {
            perror("error open file");
            mini_exit();
        }
        break;
    case 'b':
        file_descriptor = open(file, O_RDWR);
        if (file_descriptor == -1) {
            perror("error open file");
            mini_exit();
        }
        break;
    case 'a':
        file_descriptor = open(file, O_APPEND);
        if (file_descriptor == -1) {
            perror("error open file");
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

    // To memory which file is open that we can flush it when mini_exit_io being called
    if(file_list == NULL) {
        file_list = mini_calloc(sizeof(struct FILE_elm_list), 1);
        file_list->file = myFile;
        file_list->next = NULL;
    }else {
        struct FILE_elm_list* next_file = mini_calloc(sizeof(struct FILE_elm_list), 1);
        next_file->file = myFile;
        next_file->next = file_list;
        file_list = next_file;
    }

    return myFile;
}

int mini_read(void *buffer, int size_element, int number_element, struct MYFILE *file) {

    if(file->ind_read == -1) {
        file->buffer_read = mini_calloc(sizeof(char), IOBUFFER_SIZE);
        file->ind_read = 0;
        if( read(file->fd, file->buffer_read, IOBUFFER_SIZE) == -1 ) {
            return -1;
        }
    }

    int num_bytes_to_read = size_element * number_element;
    int i = 0;

    while(i < num_bytes_to_read) {
        ((char*)buffer)[i] = ((char*)file->buffer_read)[file->ind_read];
        file->ind_read++;

        if(file->ind_read >= IOBUFFER_SIZE) {
            if( read(file->fd, file->buffer_read, IOBUFFER_SIZE) == -1 ) {
                return -1;
            }

            file->ind_read = 0;
        }
        i++;
    }

    return file->ind_read - 1;
}

int mini_write(void *buffer, int size_element, int number_element, struct MYFILE *file) {
    
    if(file->ind_write == -1) {
        file->buffer_write = mini_calloc(sizeof(char), IOBUFFER_SIZE);
        file->ind_write = 0;
    }

    int num_bytes_written = size_element * number_element;

    int i = 0;

    while(i < num_bytes_written) {

        ((char*)file->buffer_write)[file->ind_write] = ((char*)buffer)[i];
        file->ind_write++;

        if(file->ind_write >= IOBUFFER_SIZE) {
            if(write(file->fd, file->buffer_write, file->ind_write) == -1) {
                return -1;
            }
            file->ind_write = 0;
        }

        i++;
    }
    return i;

}

int mini_fgetc(struct MYFILE *file) {
    char *buffer = mini_calloc(sizeof(char), 1);
    if (mini_read(buffer, sizeof(char), 1, file) == -1) {
        return -1;
    }
    return *buffer;
}

int mini_fputc(struct MYFILE *file, char c) {
    char *buffer = mini_calloc(sizeof(char), 1);
    *buffer = c;
    if (mini_write(buffer, sizeof(char), 1, file) == -1) {
        return -1;
    }
    return 1;
}

int mini_flush(struct MYFILE *file) {
    int count;
    if (file->ind_write != -1) {
        if ( count = write(file->fd, file->buffer_write, file->ind_write) == -1) {
            perror("error when flush buffer_write");
            return -1;
        }
        file->ind_write = 0;
    }

    return count;
}


void mini_exit_io() {
    if(file_list != NULL) {
        struct FILE_elm_list* temp_file = file_list;
        while(temp_file != NULL) {
            mini_flush(temp_file->file);
            temp_file = temp_file->next;
        }
    }
}



