#include <stdio.h>
#include "mini_lib.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void run_use_case(char c);

void test_mini_memory();

void test_mini_string();

void test_mini_io();

int main() {

    test_mini_io();
    mini_exit();
    return 0;
}

void test_mini_io() {

    /*
    Si vous voulez tester mini_read, Décommentez la partie "just test read"
    Si vous voulez tester mini_write, Décommentez la partie "just test write"
    Si vous voulez tester la combinaison entre mini_write et mini_read, Décommentez la partie "Combine mini_read mini_write to test"
    */

    /* just test read */
    // struct MYFILE *file_to_read = mini_open("text_to_read.txt", 'r');
    // char *buffer = mini_calloc(sizeof(char), 30);
    // mini_read(buffer, 1, 29, file_to_read);
    // mini_printf(buffer);
    // char c1 = mini_fgetc(file_to_read);
    // char c2 = mini_fgetc(file_to_read);
    // char c3 = mini_fgetc(file_to_read);
    // char c4 = mini_fgetc(file_to_read);
    // mini_fputc(file_to_read, 'k');


    /* just test write */
    // struct MYFILE *file_to_write = mini_open("text_to_write.txt", 'w');
    // char buffer[] = "prog-system";
    // printf("\n%d\n", sizeof(buffer) - 1);
    // mini_write(buffer, sizeof(char), sizeof(buffer)-1, file_to_write);


    /*Combine mini_read mini_write to test*/
    struct MYFILE *rw_file = mini_open("text.txt", 'b');
    char buffer_to_write[] = "c is language for prog-system";
    mini_write(buffer_to_write, sizeof(char), sizeof(buffer_to_write)-1, rw_file);
    mini_flush(rw_file);

    // reload file
    rw_file = mini_open("text.txt", 'b');

    char *buffer_to_read = mini_calloc(sizeof(char), sizeof(buffer_to_write));
    mini_read(buffer_to_read, sizeof(char), sizeof(buffer_to_write)-1, rw_file);

    mini_printf(buffer_to_read);

    mini_exit_io();

}


void test_mini_string() {

    /* Test for printf */
    mini_printf("unique");
    mini_printf("abc\nde");
    mini_printf("Ciao");
    mini_printf("Bonjour");
    mini_printf("tout le monde\n");
    mini_printf("\nhehe\n");

    /* Test for scanf */
    char *text = mini_calloc(sizeof(char), 100);
    int count = mini_scanf(text, 100);
    mini_printf(text);

    /* Test for mini_strlen mini_strcmp mini_strcpy */
    printf("\n%d\n", mini_strlen(text));

    mini_exit_string();

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