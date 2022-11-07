#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "mini_lib.h"
#include<stdio.h>


#define BUF_SIZE 1024

char *buffer;

int ind = -1;

/*
mini_printf("unique");
mini_printf("abc\ndef");
mini_printf("Ciao");
mini_printf("Bonjour");
mini_printf("tout le monde\n");
mini_printf("\nhehe\n");
*/

void mini_printf(char *text) {

    if(ind == -1) {
        buffer = mini_calloc(sizeof(char), BUF_SIZE);
        ind = 0;
    }

    for(int i=0; i <= strlen(text); i++) {

        if(text[i] == '\n' || ind == BUF_SIZE) {
            write(1, buffer, ind);
            while(ind >= 0) {
                buffer[ind--] = '\0';
            }
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

    int count = read(1, buffer, size_buffer-1);
    if(count == -1) {
        perror("Read error");
        mini_exit();
    }
    buffer[count] = '\0';
    return count;
}

int mini_strlen(char *s) {
    if (s == NULL) {
        perror("String doesn't exist");
        mini_exit();
    }
    char *temp = s;
    int len = 0;
    while(*(temp++) != '\0') {
        len++;
    }
    return len;
}


int mini_strcpy(char *s, char *d) {
    

    if(s==NULL || d==NULL) {
        perror("ERROR");
        mini_exit();
    }
    
    //Make sure buffer overflow never occur!
    d = mini_calloc(sizeof(char), mini_strlen(s) + 1);

    int count = 0;

    while(*s != '\0') {

        *(d++) = *(s++);
        count++;
    }
    
    return count;
}

int mini_strcmp(char *s1, char *s2) {

    if(s1 == NULL || s2 == NULL) {
        perror("ERROR");
        mini_exit();
    }

    // char *temp_s1 = s1;
    // char *temp_s2 = s2;

    while(*s1 != '\0') {
        if (*s1 != *s2 || *s2 == '\0') {
            return -1;
        }
        s1++;
        s2++;
    }

    if(*s2 != '\0') {
        return -1;
    }

    return 0;
}

//s1 A B C D E
//s2 A B C D

