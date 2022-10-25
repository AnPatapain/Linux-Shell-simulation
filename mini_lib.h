#ifndef MINI_LIB
#define MINI_LIB

void* mini_calloc (int size_element, int number_element);

void mini_free (void *ptr);

void mini_exit();

void mini_printf(char *);

int mini_scanf(char *, int);

int mini_strlen(char *);

int mini_strcpy(char *s, char *d);

int mini_strcmp(char *s1, char *s2);

#endif