#include <stdio.h>
#include "mini_lib.h"
#include <string.h>
#include <stdlib.h>
int main() {

    // char *test1 = mini_calloc(sizeof(char), 5);
    // char *test2 = mini_calloc(sizeof(char), 5);
    

    // for(int i = 0; i < 5; i++) {
    //     test1[i] = 'a';
    //     test2[i] = 'b';
    // }

    // for(int i = 0; i < 5; i++) {
    //     printf("\n%c\n" , test1[i]);
    //     printf("\n%c\n" , test2[i]);
    // }

    // mini_free(test2);

    // char *test3 = mini_calloc(sizeof(char), 3);
    
    // for(int i = 0; i < 3; i++) {
    //     test3[i] = 'c';
    // }
    
    // for(int i = 0; i < 3; i++) {
    //     printf("\n%c\n" , test3[i]);
    // }

    mini_printf("xinchao\n");
	mini_printf("bonjour\n");
	mini_printf("ciao\n");
	mini_printf("\nem\n");
    

    char s1[20] = "";
    char s2[20] = "bonjour";
    printf("\n%d\n", mini_strlen(s1));

    printf("\n%d\n", mini_strcmp(s1, s2));
    // mini_scanf(c, 10);
    // mini_printf(c);
    return 0;
}