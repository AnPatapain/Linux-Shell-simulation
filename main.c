#include <stdio.h>
#include "mini_lib.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void run_use_case(char c);

void test_mini_memory();

void test_mini_string();

char* test_mini_io();


/*================= PARTIE SHELL exercise 39 =================================*/

int mini_touch_exec(char **args) {
    mini_touch(args[1]);
    return 1;
}

int mini_cp_exec(char **args) {
    if (args[1] != NULL && args[2] != NULL) {
        mini_cp(args[1], args[2]);
    }
    return 1;
}

char *command_list[] = {"mini_touch", "mini_cp"};
 
int (*command_list_exec[]) (char **) = {&mini_touch_exec, &mini_cp_exec};

int num_command_list() {
  return sizeof(command_list) / sizeof(char *);
}

/*
@brief execute command using args passed as params
@params args: char**
@return status
*/
int execute_command(char** args) {
    int status;
    pid_t pid, wpid;

    pid = fork();
    if (pid == 0) {
        // Child process
        // if (execvp(args[0], args) == -1) {
        //     perror("lsh");
        // }
        for (int i = 0; i < num_command_list(); i++) {
            if (strcmp(args[0], command_list[i]) == 0) {
                (*command_list_exec[i])(args);
            }
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("lsh");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


#define BUF_SIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/* 
@brief split command into function name, arguments
@params command: char*
@return pointer to pointer to char: char**
*/
char **split_command(char *line)
{
    char **tokens = mini_calloc(sizeof(char *), BUF_SIZE);
    for(int i = 0; i < BUF_SIZE; i++) {
        tokens[i] = mini_calloc(sizeof(char), BUF_SIZE);
    }
    int i = 0; // for traversing line
    int j = 0; // for traversing tokens
    int k = 0; // for traversing tokens[j]

    while(line[i] != '\0') {
        if(line[i] == ' ') {
            tokens[j][k] = '\0';
            j++;
            k = 0;
        }else {
            tokens[j][k++] = line[i];
        }
        i++;
    }
    tokens[j][k] = '\0';
    tokens[j+1] = NULL;
    
    return tokens;
}

/* 
@brief read stdin
@params void
@return char*
*/
char *read_command(void) {
    struct MYFILE *mini_stdin = mini_open("/dev/stdin", 'r');

    int maxsize = 1024;
    int position = 0;
    char *buffer = mini_calloc(sizeof(char), maxsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character
        c = mini_fgetc(mini_stdin);
        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;
    }
}


/* 
@brief read from stdin and store command_line -> split into function name and arguments -> execute in child process
@params void
@return void
*/
void mini_shell_loop(void){
    char exit[] = "exit";
    char *command;
    char **args;
    int status;
    
    do {

        printf("\nEntrez la commande\n");
        command = read_command();
        args = split_command(command);
        status = execute_command(args);

    } while (mini_strcmp(command, exit) != 0);
}

/* ========================================= MAIN ======================================= */
int main() {
    // mini_cp("text_to_read.txt", "hehehehe.txt");
    mini_printf(">>\n");
    mini_printf("\n---------------------------------------MINI_SHELL------------------------------------\n");
    mini_printf("\nAvailable Command\n");
    mini_printf("\n<< mini_touch file_location: create file, mini_cp source dest: copy source to dest >>\n");
    mini_shell_loop();
    mini_exit();
    // return 0;
}




/* ========================================= TEST FUNCTION ============================== */


char* test_mini_io() {

    /* just test read */
    // struct MYFILE *file_to_read = mini_open("text_to_read.txt", 'r');
    // char *buffer = mini_calloc(sizeof(char), 30);
    // mini_read(buffer, 1, 29, file_to_read);
    // mini_printf(buffer);
    struct MYFILE *mini_stdin = mini_open("/dev/stdin", 'r');
    
    int bufsize = 1024;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character
        c = mini_fgetc(mini_stdin);

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;
        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize) {
            bufsize += 1024;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    // printf("\n%c\n", c);



    /* just test write */
    // struct MYFILE *file = mini_open("text_to_write.txt", 'b');

    /*Combine mini_read mini_write to test*/
    // struct MYFILE *rw_file = mini_open("text.txt", 'b');
    // char buffer_to_write[] = "c is language for prog-system";
    // mini_write(buffer_to_write, sizeof(char), sizeof(buffer_to_write)-1, rw_file);
    // mini_flush(rw_file);

    // rw_file = mini_open("text.txt", 'b');

    // char *buffer_to_read = mini_calloc(sizeof(char), sizeof(buffer_to_write));
    // mini_read(buffer_to_read, sizeof(char), sizeof(buffer_to_write)-1, rw_file);

    // mini_printf(buffer_to_read);

    /* Partie Commande systeme test */
    // struct MYFILE *newfile = mini_touch("/home/kean/work/prog-system/TP/TP1_Nguyen_ke_an/new_file.txt");
    //mini_cp("/home/kean/work/prog-system/TP/TP1_Nguyen_ke_an/text_to_read.txt", "/home/kean/work/prog-system/TP/TP1_Nguyen_ke_an/new_file.txt");
    // mini_exit_io();

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