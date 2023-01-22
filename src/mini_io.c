#include "mini_lib.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
// #include <sys/stat.h>
// #include<grp.h>
// #include<pwd.h>


#define IOBUFFER_SIZE 10

struct FILE_elm_list* file_list = NULL;

struct MYFILE *mini_fopen(char *file, char mode) {

    if(file == NULL) {
        mini_perror("file name is NULL");
        
    }

    int file_descriptor;

    switch (mode)
    {
    case 'r':
        file_descriptor =  open(file, O_RDONLY);
        if (file_descriptor == -1) {
            mini_perror("error open file");
            
        }
        break;
    case 'w':
        file_descriptor = open(file, O_WRONLY);
        if (file_descriptor == -1) {
            mini_perror("error open file");
            
        }
        break;
    case 'b':
        file_descriptor = open(file, O_RDWR);
        if (file_descriptor == -1) {
            mini_perror("error open file");
            
        }
        break;
    case 'a':
        file_descriptor = open(file, O_APPEND);
        if (file_descriptor == -1) {
            mini_perror("error open file");
            
        }
        break;
    default:
        mini_perror("UNKNOWN ERROR");
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

int mini_fclose(struct MYFILE* file) {

    // Close file
    if(close(file->fd) == -1) {
        return -1;
    }
    // Flush file
    mini_fflush(file);


    // Traverse file_list to find this file, then delete it from file_list
    struct FILE_elm_list* temp = file_list;
    struct FILE_elm_list* prev_temp;

    if(temp != NULL && temp->file->fd == file->fd) {
        file_list = file_list->next;
        mini_free(temp);
        return 0;
    }

    while(temp != NULL && temp->file->fd != file->fd){
        prev_temp = temp;
        temp = temp->next;
    }

    if(temp == NULL) {
        mini_perror("inside mini_fclose file does not existe in file_liste");
    }

    prev_temp->next = temp->next;
    mini_free(temp);

    return 0;
}

int mini_fread(void *buffer, int size_element, int number_element, struct MYFILE *file) {


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

int mini_fwrite(void *buffer, int size_element, int number_element, struct MYFILE *file) {
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
    if (mini_fwrite(buffer, sizeof(char), 1, file) == -1) {
        return -1;
    }
    return 1;
}

int mini_fflush(struct MYFILE *file) {
    int count;
    if (file->ind_write != -1) {
        if ( ( count = write(file->fd, file->buffer_write, file->ind_write) ) == -1) {
            mini_perror("error when flush buffer_write");
            // return -1;
        }
        file->ind_write = 0;
    }

    return count;
}

struct MYFILE *mini_touch(char *file_name) {
    struct MYFILE* file = mini_calloc(sizeof(struct MYFILE), 1);
    int fd;

    if(( fd = open(file_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) == -1) {
        mini_perror("error create file");
        _Exit(1);
    }

    file->fd = fd;
    file->ind_read = -1;
    file->ind_write = -1;
    return file;
}

void mini_cp(char *src, char *dst) {
    struct MYFILE* src_file = mini_fopen(src, 'r');
    struct MYFILE* dst_file = mini_fopen(dst, 'w');

    char c = mini_fgetc(src_file);
    
    while(c != -1) {
        mini_fputc(dst_file, c);
        c = mini_fgetc(src_file);
    }
    mini_exit_io();
}

void mini_echo(char *buffer) {
    struct MYFILE *mini_stdout = mini_fopen("/dev/stdout", 'b');
    mini_fwrite(buffer, sizeof(char), mini_strlen(buffer), mini_stdout);
    mini_exit_io();
}

void mini_cat(char *file_name) {
    struct MYFILE *file = mini_fopen(file_name, 'b');
    char c = mini_fgetc(file);

    while(c != -1) {
        char *buffer = mini_calloc(sizeof(char), 2);
        buffer[0] = c;
        mini_echo(buffer);
        c = mini_fgetc(file);
    }
}

void mini_head(int n, char *file_name) {
    struct MYFILE *file = mini_fopen(file_name, 'b');
    
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

    //Tout d'abord on mettre file_position a bonne position
    struct MYFILE *file = mini_fopen(file_name, 'a');
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

    //Lire a partir cette file_position
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
    struct stat file_stat_buff;
    struct MYFILE* file = mini_fopen(file_name, 'b');

    if(stat(file_name, &file_stat_buff) == -1) {
        mini_perror("stat error at mini_grep");
    }

    //Write file byte to byte to temp_buffer 
    char *temp_buffer = mini_calloc(sizeof(char), file_stat_buff.st_size + 1);
    int k = 0;
    char c = mini_fgetc(file);
    while(c != -1) {
        temp_buffer[k++] = c;
        c = mini_fgetc(file);
    }

    //Find pattern and print ligne containing this pattern
    int i = 0;
    int j = 0;
    int mot_len = mini_strlen(mot);
    int ligne = 0;

    while(temp_buffer[i] != '\0') {
        
        if(mot[j] == temp_buffer[i]) {
            while(mot[j] == temp_buffer[i] && temp_buffer[i] != '\0') {
                i++;
                j++;
            }

            if(j == mot_len && (temp_buffer[i] == ' ' || temp_buffer[i] == '\n' || temp_buffer[i] == '\0')) {
                if(temp_buffer[ligne] == '\n') {
                    i = ligne + 1;
                }else {
                    i = ligne;
                }
                while(temp_buffer[i] != '\n') {
                    char *echo_buffer = mini_calloc(sizeof(char), 2);
                    echo_buffer[0] = temp_buffer[i];
                    mini_echo(echo_buffer);
                    i++;
                }
                mini_printf("\n");
                mini_exit_string();
            }
        }else {

            if(temp_buffer[i] == '\n') {
                ligne = i;
            }
            i++;
            j = 0;
        }
    }



}

void mini_wc(char *file_name) {
    struct stat file_stat_buff;
    struct MYFILE* file = mini_fopen(file_name, 'b');

    if(stat(file_name, &file_stat_buff) == -1) {
        mini_perror("stat error at mini_grep");
    }
    //Write file byte to byte to temp_buffer 
    char *temp_buffer = mini_calloc(sizeof(char), file_stat_buff.st_size + 1);
    int k = 0;
    char c = mini_fgetc(file);
    while(c != -1) {
        temp_buffer[k++] = c;
        c = mini_fgetc(file);
    }
    
    
    //Now we work on string temp_buffer : )
    int word_count = 0;
    int i = 0;
    while(temp_buffer[i] != '\0') {
        if((temp_buffer[i] == ' ' && temp_buffer[i + 1] != ' ') || temp_buffer[i] == '\n') {
            word_count++;
        }
        i++;
    }
    if(word_count == 0) {
        mini_printf("\n0\n");
        return;
    }
    char *word_count_string = mini_itoa(word_count);
    mini_echo(word_count_string);
    mini_echo("\n");
}


void mini_exit_io() {
    if(file_list != NULL) {
        struct FILE_elm_list* temp_file = file_list;
        while(temp_file != NULL) {
            mini_fflush(temp_file->file);
            temp_file = temp_file->next;
        }
    }
}

void mini_print_info(struct stat* stat_buf, char* name) {
    struct group* users = getgrgid(stat_buf->st_gid);
	struct passwd* user = getpwuid(stat_buf->st_uid);
	
	int nb_link = stat_buf->st_nlink;
	char *user_name = user->pw_name;
	char *groupe_name = users->gr_name;
	int size = stat_buf->st_size;
	char* file_name_ = name;


    mini_printf(stat_buf->st_mode&S_IRUSR?"r":"-");
    mini_printf(stat_buf->st_mode&S_IWUSR?"w":"-");
    mini_printf(stat_buf->st_mode&S_IXUSR?"x":"-");
    mini_printf(stat_buf->st_mode&S_IRGRP?"r":"-");
    mini_printf(stat_buf->st_mode&S_IWGRP?"w":"-");
    mini_printf(stat_buf->st_mode&S_IXGRP?"x":"-");
    mini_printf(stat_buf->st_mode&S_IROTH?"r":"-");
    mini_printf(stat_buf->st_mode&S_IWOTH?"w":"-");
    mini_printf(stat_buf->st_mode&S_IXOTH?"x":"-");
    
    mini_printf(" ");
    mini_printf(mini_itoa(nb_link));
    mini_printf(" ");
    mini_printf(user_name);
    mini_printf(" ");
    mini_printf(groupe_name);
    mini_printf(" ");
    mini_printf(mini_itoa(size));
    mini_printf(" ");
    mini_printf(file_name_);
    
    mini_printf("\n");
    mini_exit_string();

	// printf("\n%c%c%c%c%c%c%c%c%c %d  %s  %s  %d  %s\n",       stat_buf->st_mode&S_IRUSR?'r':'-',
	// 														  stat_buf->st_mode&S_IWUSR?'w':'-',
	// 														  stat_buf->st_mode&S_IXUSR?'x':'-',
	// 														  stat_buf->st_mode&S_IRGRP?'r':'-',
	// 														  stat_buf->st_mode&S_IWGRP?'w':'-',
	// 														  stat_buf->st_mode&S_IXGRP?'x':'-',
	// 														  stat_buf->st_mode&S_IROTH?'r':'-',
 	// 														  stat_buf->st_mode&S_IWOTH?'w':'-',
 	// 														  stat_buf->st_mode&S_IXOTH?'x':'-',
	// 														  nb_link, user_name, groupe_name, size, file_name_);

}

int mini_quickdiff(char* source, char* dest) {
    // struct MYFILE* fp_src = mini_fopen(source, 'r');
    // if (fp_src == NULL) {
    //     mini_perror("fopen source");
    // }
    // struct MYFILE* fp_dst = mini_fopen(dest, 'r');
    // if (fp_dst == NULL) {
    //     mini_perror("fopen dest");
    // }

    // char line_src[256];
    // char line_dst[256];

    // int line_number = 0;

    // while (mini_fread(line_src, sizeof(char), 256, fp_src) != -1) {
    //     line_number++;
    //     if (mini_fread(line_dst, sizeof(char), 256, fp_dst) == -1) {
    //         printf("line %d: only in %s\n", line_number, source);
    //         break;
    //     }
    //     if (mini_strcmp(line_src, line_dst) != 0) {
    //         printf("line %d: %s", line_number, line_src);
    //         printf("line %d: %s", line_number, line_dst);
    //     }
    // }
    // while (mini_fread(line_dst, sizeof(char), 256, fp_dst) != -1) {
    //     line_number++;
    //     printf("line %d: only in %s\n", line_number, dest);
    // }

    // mini_fclose(fp_src);
    // mini_fclose(fp_dst);
    FILE* fp_src = fopen(source, "r");
    if (fp_src == NULL) {
        perror("fopen source");
        return 1;
    }
    FILE* fp_dst = fopen(dest, "r");
    if (fp_dst == NULL) {
        perror("fopen dest");
        fclose(fp_src);
        return 1;
    }

    char line_src[256];
    char line_dst[256];

    int line_number = 0;

    while (fgets(line_src, 256, fp_src) != NULL) {
        line_number++;
        if (fgets(line_dst, 256, fp_dst) == NULL) {
            printf("line %d: only in %s\n", line_number, source);
            break;
        }
        if (mini_strcmp(line_src, line_dst) != 0) {
            printf("line %d: %s", line_number, line_src);
            printf("line %d: %s", line_number, line_dst);
        }
    }
    while (fgets(line_dst, 256, fp_dst) != NULL) {
        line_number++;
        printf("line %d: only in %s\n", line_number, dest);
    }

    fclose(fp_src);
    fclose(fp_dst);

    return 0;
}



