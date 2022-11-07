#include <stdio.h>
#include "mini_lib.h"
#include <string.h>
#include <stdlib.h>


void run_use_case(char c);

/*Test for module mini_memory */
void test_mini_memory();

/*Test for module mini_string*/
void test_mini_string();




int main() {

    // test_mini_memory();

    test_mini_string();
    mini_exit();
}

void test_mini_string() {

    /* Test for printf */
    // mini_printf("unique");
    // mini_printf("abc\nde");
    // mini_printf("Ciao");
    // mini_printf("Bonjour");
    // mini_printf("tout le monde\n");
    // mini_printf("\nhehe\n");

    /* Test for scanf */
    // char *text = mini_calloc(sizeof(char), 6);
    // int count = mini_scanf(text, 6);

    char source[] = "ciao";
    char *des = mini_calloc(sizeof(char), 2);
    printf("\n%d\n", mini_strcpy(source, des));
    

}

void test_mini_memory() {

    puts("* Menu [>: allocate new block memory, <: free actuel block memory, !: malloc_liste count] *");

    while (1)
    {

        int c = getchar();
        if (c == EOF)
            break;

        run_use_case(c);
    }

}

void run_use_case(char c) {

    static struct malloc_element* temp;
    static char* buffer;
    static int i;

    switch (c)
    {
    case '>':
        buffer =  mini_calloc(sizeof(char), 5);
        temp = malloc_list;
        break;

    case '<':
        mini_free(buffer);
        break;

    case '!':
        struct malloc_element* traversing = temp;
        int i = 0;
        while(traversing != NULL){
            traversing = traversing->next_zone;
            i++;
        }
        printf("\n%d\n", i);
    case '\n':
        break;

    case '\t':
        break;

    default:
        if(temp->statut == 1) {
            *(((char*)buffer) + i) = c;
        }else {
            perror("segmentation fault");
            mini_exit();
        }
        
        break;
    }
}