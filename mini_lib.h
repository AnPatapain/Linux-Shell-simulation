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

int mini_strcmp(char *s1, char *s2);

// mini_io's function signatures

struct MYFILE* mini_open(char *, char); 

#endif