#include "mini_lib.h"
#include <unistd.h>
#include <sys/wait.h>

void test_mini_memory();

void test_mini_string();

void test_mini_io();


int main() {
    char **args;
    mini_printf("\nSHELL STARTS, TYPE HELP TO SEE AVAILABLE COMMAND\n");
    mini_help_exec(args);
    mini_shell_loop();
    return 0;
    
}
/* ========================================= TEST FUNCTION ============================== */

void test_mini_memory() {
    char* buffer1 = mini_calloc(sizeof(char), 3);
    buffer1[0] = 'a';
    buffer1[1] = 'b';

    mini_free(buffer1);

    char* buffer2 = mini_calloc(sizeof(char), 2);
    
    if(buffer2[0] == 'a' && buffer2[1] == 'b') {
        mini_printf("memory block was not clean before being allocated");
    }
}

void test_mini_io() {

    // /*Combine mini_read mini_write to test*/
    // mini_touch("text.txt");
    // struct MYFILE *rw_file = mini_open("text.txt", 'b');
    // char buffer_to_write[] = "c is language for prog-system";
    // mini_fwrite(buffer_to_write, sizeof(char), sizeof(buffer_to_write)-1, rw_file);
    // mini_fflush(rw_file);

    // rw_file = mini_open("text.txt", 'b');

    // char *buffer_to_read = mini_calloc(sizeof(char), sizeof(buffer_to_write));
    // mini_fread(buffer_to_read, sizeof(char), sizeof(buffer_to_write)-1, rw_file);
    // mini_printf(buffer_to_read);
    // mini_exit_string();
    // mini_exit_io();

    struct MYFILE* file = mini_fopen("text.txt", 'r');
    if (mini_fclose(file) == -1) {
        mini_printf("\nmini_fclose error\n");
    }else {
        mini_printf("\nsuccessfully close file\n");
    }

}


void test_mini_string() {

    /* Test for printf */
    mini_printf("unique\n");
    mini_printf("abc\nde");
    mini_printf("Ciao");
    mini_printf("Bonjour");
    mini_printf("tout le monde\n");
    mini_printf("\nhehe\n");

    /* Test for scanf */
    // char *text = mini_calloc(sizeof(char), 100);
    // int count = mini_scanf(text, 100);
    // mini_printf(text);

    /* Test for mini_strlen mini_strcmp mini_strcpy */

    mini_exit_string();

}

