#include <unistd.h>
#include <errno.h>
#include "mini_lib.h"


#define BUF_SIZE 10

char *buffer;

int ind = -1;

void mini_printf(char *text) {

    if(ind == -1) {
        buffer = mini_calloc(sizeof(char), BUF_SIZE);
        ind = 0;
    }

    int i = 0;
    while(text[i] != '\0') {
        // printf("\n%c\n", text[i]);
        if(text[i] == '\n' || ind == BUF_SIZE) {
            if(text[i] == '\n') {
                buffer[ind++] = text[i++];
            }

            write(1, buffer, ind);
            while(ind >= 0) {
                buffer[ind--] = '\0';
            }
            ind = 0;
        }else {
            buffer[ind++] = text[i++];
        }
    }

}


int mini_scanf(char *buffer, int size_buffer) {
    if (size_buffer <= 0) {
        mini_perror("in mini_scanf size_buffer must be greater than 0");
        mini_exit();
    }

    int count = read(STDIN_FILENO, buffer, size_buffer-1);
    
    if(count == -1) {
        mini_perror("in mini_scanf Read error");
        mini_exit();
    }
    buffer[count] = '\0';

    return count;
}

int mini_strlen(char *s) {
    if (s == NULL) {
        mini_perror("in mini_strlen String doesn't exist");
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
        return -1;
    }

    int count = 0;

    while(*s != '\0') {

        *(d++) = *(s++);
        count++;
    }
    
    return count;
}

int mini_strncpy(char *s, char *d, int n) {
    
    if(s==NULL || d==NULL || n <= 0) {
        return -1;
    }

    int i = 0;
    while(i < n && s[i] != '\0') {
        d[i] = s[i];
        i++;
    }
    d[i] = '\0';
}

int mini_strcmp(char *s1, char *s2) {

    if(s1 == NULL || s2 == NULL) {
        mini_perror("in mini_strcmp");
        mini_exit();
    }

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

char *mini_itoa(int a)
{
    int digits = 0;
    int _a = a;
    while (_a != 0)
    {
        _a = _a / 10;
        digits++;
    }
    char *returning = mini_calloc(sizeof(char), (digits + 1));
    // char *returning = calloc(1, digits + 1);
    *(returning + digits) = '\0';
    for (int i = 0; a != 0; i++)
    {
        int temp = a % 10;
        *(returning + digits - (i + 1)) = (char)(temp + 48);
        a = a / 10;
    }
    return returning;
}

int mini_atoi(char* s, int base) {
    int final = 0;

    for (int i = 0; s[i] != '\0'; ++i){
        final = final * base + s[i] - '0';
    }
        
    return final;
}

void mini_perror(char *message) {
    
    // Error number of errno has maximum 3 digits
    char *errno_str = mini_itoa(errno);
    mini_printf(message);
    mini_printf(" with error code: ");
    mini_printf(errno_str);
    mini_printf("\n");
    _Exit(0);
    // mini_exit();
}

void mini_exit_string() {
    if(ind != -1) {
        write(1, buffer, ind);
        ind = -1;
    }
}

