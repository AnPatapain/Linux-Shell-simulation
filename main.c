#include <stdio.h>
#include "mini_lib.h"
#include <string.h>
#include <stdlib.h>
int main() {
    
    char *test1 = mini_calloc(sizeof(char), 5);
    char *test2 = mini_calloc(sizeof(char), 5);
    

    for(int i = 0; i < 5; i++) {
        test1[i] = 'a';
        test2[i] = 'b';
    }

    for(int i = 0; i < 5; i++) {
        printf("\n%c\n" , test1[i]);
        printf("\n%c\n" , test2[i]);
    }

    mini_free(test2);

    char *test3 = mini_calloc(sizeof(char), 3);
    
    for(int i = 0; i < 3; i++) {
        test3[i] = 'c';
    }
    
    for(int i = 0; i < 3; i++) {
        printf("\n%c\n" , test3[i]);
    }
    
    return 0;
}