#include "mini_lib.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define IOBUFFER_SIZE 10

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
        int count = read(file->fd, file->buffer_read, IOBUFFER_SIZE);
        if( count == -1 ) {
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
        if(file->ind_write == IOBUFFER_SIZE) {
            if(write(file->fd, file->buffer_write, file->ind_write) == -1) {
                return -1;
            }
            file->ind_write = 0;
        }
        ((char*)file->buffer_write)[file->ind_write++] = ((char*)buffer)[i++];
    }
    return i;

}

int mini_fgetc(struct MYFILE *file) {
    char buf[1];
    int count = read(file->fd, buf, 1);
    if (count == -1) {
        return -1;
    }
    if (count == 0) {
        return -1;
    }
    return buf[0];

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

struct MYFILE *mini_touch(char *file_name) {
    struct MYFILE* file = mini_calloc(sizeof(struct MYFILE), 1);
    int fd;

    if(fd = open(file_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
        perror("error create file");
        _Exit(1);
    }

    file->fd = fd;
    file->ind_read = -1;
    file->ind_write = -1;
    return file;
}

void mini_cp(char *src, char *dst) {
    struct MYFILE* src_file = mini_open(src, 'r');
    struct MYFILE* dst_file = mini_open(dst, 'w');

    char c = mini_fgetc(src_file);
    
    while(c != -1) {
        mini_fputc(dst_file, c);
        c = mini_fgetc(src_file);
    }
    mini_exit_io();
}

void mini_echo(char *buffer) {
    struct MYFILE *mini_stdout = mini_open("/dev/stdout", 'b');
    mini_write(buffer, sizeof(char), mini_strlen(buffer), mini_stdout);
    mini_exit_io();
    // mini_printf(buffer);
    // mini_exit_string();
}

void mini_cat(char *file_name) {
    struct MYFILE *file = mini_open(file_name, 'b');
    char c = mini_fgetc(file);

    while(c != -1) {
        char *buffer = mini_calloc(sizeof(char), 2);
        buffer[0] = c;
        mini_echo(buffer);
        c = mini_fgetc(file);
    }
}

void mini_head(int n, char *file_name) {
    struct MYFILE *file = mini_open(file_name, 'b');
    
    for(int i = 0; i < n; i++) {
        char c = mini_fgetc(file);
        while(c != '\n') {
            if(c == -1) {
                break;
            }
            char *buffer = mini_calloc(sizeof(char), 2);
            buffer[0] = c;
            mini_echo(buffer);
            c = mini_fgetc(file);
        }
        if(c == -1) {
            break;
        }else {
            mini_echo("\n");
        }
    }
}

void mini_tail(int n, char *file_name) {
    struct MYFILE *file = mini_open(file_name, 'a');
    off_t pos = lseek(file->fd, 0, SEEK_END);
    int count = 0;
    while(pos) {
        lseek(file->fd, pos--, SEEK_SET);
        if(mini_fgetc(file) == '\n') {
            if(count++ == n) {
                break;
            }
        }
    }
    for(int i = 0; i < n; i++) {
        char c = mini_fgetc(file);
        while(c != '\n') {
            if(c == -1) {
                break;
            }
            char *buffer = mini_calloc(sizeof(char), 2);
            buffer[0] = c;
            mini_echo(buffer);
            c = mini_fgetc(file);
        }
        if(c == -1) {
            break;
        }else {
            mini_echo("\n");
        }
    }
    
}

void mini_clean(char *file_name) {
    if(open(file_name, O_TRUNC) == -1) {
        mini_touch(file_name);
    }
}

void mini_grep(char *file_name, char *mot) {
    struct MYFILE *file = mini_open(file_name, 'b');
    
    char c = mini_fgetc(file);
    int len = mini_strlen(mot);
    int i = 0;
    int ligne = 0;
    int j = 0;

    //traverser file jusqu'a EOF, sur mon ordi EOF est -1
    while(c != -1) {
        if(c == mot[j]) {
            while(c == mot[j] && c != -1) {
                j++;
                c = mini_fgetc(file);
            }
            if (j == len && (c == ' ' || c == -1)) {
                lseek(file->fd, ligne, SEEK_SET);
                c = mini_fgetc(file);
                while(c != '\n') {
                    char *buffer = mini_calloc(sizeof(char), 2);
                    buffer[0] = c;
                    mini_echo(buffer);
                    c = mini_fgetc(file);
                    i++;
                }
                mini_printf("\n");

            }
    
        }
        ligne = i - 1;
        c = mini_fgetc(file);
        i++;
        j=0;
    }
}


int wc(char *file_name) {
    struct MYFILE* file = mini_open(file_name, 'b');
    
    int count = 0;
    char c = mini_fgetc(file);
    while(c != -1) {
        if(c == ' ' || c == '\n') {
            count++;
        }
        c = mini_fgetc(file);
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



