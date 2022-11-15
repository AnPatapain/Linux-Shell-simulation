#include "mini_lib.h"
#include <unistd.h>
#include <sys/wait.h>


void run_use_case(char c);

void test_mini_memory();

void test_mini_string();

void test_mini_io();




/* ========================================= MAIN ======================================= */
int main() {
    char **args;
    mini_printf("\nSHELL STARTS, TYPE HELP TO SEE AVAILABLE COMMAND\n");
    mini_help_exec(args);
    mini_shell_loop();
    return 0;
}










/* ========================================= TEST FUNCTION ============================== */

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

void test_mini_memory() {

    while (1)
    {
        struct MYFILE *mini_stdin = mini_fopen("/dev/stdin", 'b');
        // int c = getchar();
        int c = mini_fgetc(mini_stdin);
        if (c == -1)
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
        buffer =  mini_calloc(sizeof(char), 1024);
        temp = malloc_list;
        break;

    case '<':
        mini_free(buffer);
        break;

    case '\n':
        break;

    case '\t':
        break;

    default:
        if(temp->statut == 1) {
            buffer[i++] = c;
        }else {
            mini_perror("buffer was free");
        }
        
        break;
    }
}