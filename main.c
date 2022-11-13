#include "mini_lib.h"
#include <unistd.h>
#include <sys/wait.h>

void run_use_case(char c);

void test_mini_memory();

void test_mini_string();

char* test_mini_io();


/*================= PARTIE SHELL exercise 39 =================================*/
int mini_help_exec(char **args) {
    mini_printf("\n++++++++++++++++++++++MINI_SHELL MANUAL+++++++++++++++++++++++++++++++++++++++++++++++++\n");
    mini_printf("\n+ mini_touch file_name: Creer fichier                                                  +\n");
    mini_printf("\n+ mini_cp source_file des_file: Copier source_file to des_file                         +\n");
    mini_printf("\n+ mini_echo line: Ecrire line to stdout                                                +\n");
    mini_printf("\n+ mini_cat file_name: Ecrire fichier to stdout                                         +\n");
    mini_printf("\n+ mini_head -n <number> file_name: Ecrire number premieres lignes du fichier a stdout  +\n");
    mini_printf("\n+ mini_tail -n <number> line: Ecrire number dernieres lignes du fichier a stdout       +\n");
    mini_printf("\n+ mini_clean file_name: Vider fichier s'il en existe et creer un fichier sinon         +\n");
    mini_printf("\n+ mini_grep pattern file_name: Afficher tous les lignes contenant pattern              +\n");
    mini_printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    mini_printf("\n");
}

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

int mini_echo_exec(char **args) {
    int i = 1;
    while(args[i] != NULL) {
        mini_echo(args[i]);
        mini_echo(" ");
        i++;
    }
    return 1;
}

int mini_cat_exec(char **args) {
    if(args[1] != NULL) {
        mini_cat(args[1]);
    }
    return 1;
}

int mini_head_exec(char **args) {
    if(mini_strcmp(args[1], "-n")==0) {
        int num_line = atoi(args[2]);
        mini_head(num_line, args[3]);
    } else {
        perror("error: -n lacking");
    }
    
    return 1;
}

int mini_tail_exec(char **args) {
    if(mini_strcmp(args[1], "-n")==0) {
        int num_line = atoi(args[2]);
        mini_tail(num_line, args[3]);
    } else {
        perror("error: -n lacking");
    }
    return 1;
}

int mini_clean_exec(char **args) {
    if(args[1] != NULL) {
        mini_clean(args[1]);
    }
    return 1;
}

int mini_grep_exec(char **args) {
    if(args[1] != NULL && args[2] != NULL){
        mini_grep(args[2], args[1]);
    }
    return 1;
}

int mini_wc_exec(char **args) {
    int count;
    if(args[1] != NULL) {
        struct MYFILE* mini_stdout = mini_open("/dev/stdout", 'b');
        char *temp = mini_calloc(sizeof(char), 2);
        count = wc(args[1]);
        write(1, &count, sizeof(count));

    }
    return 1;
}

char *command_list[] = {
                            "help", 
                            "mini_touch", 
                            "mini_cp", 
                            "mini_echo", 
                            "mini_cat", 
                            "mini_head", 
                            "mini_tail", 
                            "mini_clean",
                            "mini_grep",
                            "mini_wc"
                        };
 
int (*command_list_exec[]) (char **) = {
                                            &mini_help_exec, 
                                            &mini_touch_exec, 
                                            &mini_cp_exec, 
                                            &mini_echo_exec, 
                                            &mini_cat_exec,
                                            &mini_head_exec,
                                            &mini_tail_exec,
                                            &mini_clean_exec,
                                            &mini_grep_exec,
                                            &mini_wc_exec
                                        };

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
        for (int i = 0; i < num_command_list(); i++) {
            if (mini_strcmp(args[0], command_list[i]) == 0) {
                (*command_list_exec[i])(args);
            }
        }
        mini_exit();
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
    struct MYFILE *mini_stdin = mini_open("/dev/stdin", 'b');

    int maxsize = 1024;
    int position = 0;
    char *buffer = mini_calloc(sizeof(char), maxsize);
    int c;

    while (1) {
        // Read a character
        c = mini_fgetc(mini_stdin);
        // If we hit EOF, replace it with a null character and return.
        if (c == -1 || c == '\n') {
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
        mini_printf("> ");
        mini_exit_string();
        command = read_command();
        args = split_command(command);
        status = execute_command(args);

    } while (mini_strcmp(command, exit) != 0);
}

/* ========================================= MAIN ======================================= */
int main() {
    char **args;
    mini_printf("\nSHELL STARTS TYPE HELP TO SEE AVAILABLE COMMAND\n");
    mini_help_exec(args);
    mini_shell_loop();

    return 0;
}


/* ========================================= TEST FUNCTION ============================== */


char* test_mini_io() {

    /*Combine mini_read mini_write to test*/
    mini_touch("text.txt");
    struct MYFILE *rw_file = mini_open("text.txt", 'b');
    char buffer_to_write[] = "c is language for prog-system";
    mini_write(buffer_to_write, sizeof(char), sizeof(buffer_to_write)-1, rw_file);
    mini_flush(rw_file);

    rw_file = mini_open("text.txt", 'b');

    char *buffer_to_read = mini_calloc(sizeof(char), sizeof(buffer_to_write));
    mini_read(buffer_to_read, sizeof(char), sizeof(buffer_to_write)-1, rw_file);

    mini_printf(buffer_to_read);
    mini_exit_string();
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

    mini_exit_string();

}

void test_mini_memory() {

    while (1)
    {
        struct MYFILE *mini_stdin = mini_open("/dev/stdin", 'b');

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
            perror("buffer was free");
            mini_exit();
        }
        
        break;
    }
}