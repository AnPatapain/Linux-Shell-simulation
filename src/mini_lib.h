#include <stdio.h>
#include <sys/stat.h>
#include<grp.h>
#include<pwd.h>

#ifndef MINI_LIB
#define MINI_LIB

// mini_memory's function signatures
struct malloc_element {
    void *zone;
    int taille;
    int statut;
    struct malloc_element* next_zone;
};
extern struct malloc_element* malloc_list;

void* mini_calloc (int size_element, int number_element);

void mini_free (void *ptr);

void mini_exit();

// mini_string's function signatures

void mini_printf(char *);

int mini_scanf(char *, int);

int mini_strlen(char *);

int mini_strcpy(char *s, char *d);

int mini_strncpy(char *s, char *d, int n);

int mini_strcmp(char *s1, char *s2);

void mini_perror(char *message);

void mini_exit_string();

// mini_io's function signatures
struct MYFILE {
    int fd;
    void *buffer_read;
    void *buffer_write;
    int ind_read;
    int ind_write;
};

struct FILE_elm_list {
    struct MYFILE* file;
    struct FILE_elm_list* next;
};

extern struct FILE_elm_list* file_list;

struct MYFILE* mini_fopen(char *, char); 

int mini_fclose(struct MYFILE* file);

int mini_fread(void* buffer, int size_element, int number_element, struct MYFILE* file);

int mini_fwrite(void* buffer, int size_element, int number_element, struct MYFILE* file);

int mini_fflush(struct MYFILE* file);

int mini_fgetc(struct MYFILE* file);

int mini_fputc(struct MYFILE* file, char c);

struct MYFILE* mini_touch(char *file_name);

void mini_cp(char *src, char *dst);

void mini_echo(char *string);

void mini_cat(char *file_name);

void mini_head(int n, char *file_name);

void mini_tail(int n, char *file_name);

void mini_clean(char *file_name);

void mini_grep(char *file_name, char *mot);

char* mini_itoa(int a);

void mini_wc(char *file_name);

void mini_exit_io();

void print_info(struct stat* stat_buf, char* name);

void mini_ls(int argc, char** argv);


/* MINI_SHELL PARTIE */
void mini_shell_loop(void);

char *read_command(void);

char **split_command(char *line);

int execute_command(char **args);

int num_command_list();

int mini_wc_exec(char **args);

int mini_grep_exec(char **args);

int mini_clean_exec(char **args);

int mini_tail_exec(char **args);

int mini_head_exec(char **args);

int mini_cat_exec(char **args);

int mini_echo_exec(char **args);

int mini_cp_exec(char **args);

int mini_touch_exec(char **args);

int mini_help_exec(char **args);

#endif