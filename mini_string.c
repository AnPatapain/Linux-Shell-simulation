#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "mini_lib.h"


#define BUF_SIZE 1024

char *buffer;

int ind = -1;

void mini_printf(char *text) {

    if(ind == -1) {
        buffer = mini_calloc(sizeof(char), BUF_SIZE);
        ind = 0;
    }

    for(int i=0; i <= strlen(text); i++) {

        if(text[i] == '\n' || ind == BUF_SIZE || text[i] == '\0') {
            write(1, buffer, ind);
            ind = 0;
        }

        buffer[ind++] = text[i];
    }
}

int mini_scanf(char *buffer, int size_buffer) {
    if (size_buffer <= 0) {
        perror("ERROR");
        mini_exit();
    }
    return read(1, buffer, size_buffer);
}

int mini_strlen(char *s) {
    if (s == NULL) {
        perror("ERROR");
        mini_exit();
    }
    char *temp = s;
    int len = 0;
    while(*temp != '\0') {
        len++;
        temp++;
    }
    return len;
}

int mini_strcpy(char *s, char *d) {

    if(s==NULL || d==NULL) {
        perror("ERROR");
        mini_exit();
    }

    int count = 0;

    char *temp_s = s;
    char *temp_d = d;
    while (*temp_s != '\0') {
        *(temp_s++) = *temp_s;
        temp_s++;
        count++;
    }

    return count;
}

int mini_strcmp(char *s1, char *s2) {

    if(s1 == NULL || s2 == NULL) {
        perror("ERROR");
        mini_exit();
    }

    char *temp_s1 = s1;
    char *temp_s2 = s2;

    while(*temp_s1 != '\0') {
        if (*temp_s1 != *temp_s2 || *temp_s2 == '\0') {
            return -1;
        }
        temp_s1++;
        temp_s2++;
    }

    if(*temp_s2 != '\0') {
        return -1;
    }

    return 0;
}

// A B C
// A B C D

