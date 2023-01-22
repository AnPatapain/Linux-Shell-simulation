#include "mini_lib.h"
#include <unistd.h>
#include <sys/wait.h>

int mini_help_exec(char **args) {
    mini_printf("\n++++++++++++++++++++++++++++++++++MINI_SHELL MANUAL+++++++++++++++++++++++++++++++++++++\n");
    mini_printf("\n+ mini_touch file_name: Creer fichier                                                  +\n");
    mini_printf("\n+ mini_cp source_file des_file: Copier source_file to des_file                         +\n");
    mini_printf("\n+ mini_echo line: Ecrire line to stdout                                                +\n");
    mini_printf("\n+ mini_cat file_name: Ecrire fichier to stdout                                         +\n");
    mini_printf("\n+ mini_head -n <number> file_name: Ecrire number premieres lignes du fichier a stdout  +\n");
    mini_printf("\n+ mini_tail -n <number> line: Ecrire number dernieres lignes du fichier a stdout       +\n");
    mini_printf("\n+ mini_clean file_name: Vider fichier s'il en existe et creer un fichier sinon         +\n");
    mini_printf("\n+ mini_grep pattern file_name: Afficher tous les lignes contenant pattern              +\n");
    mini_printf("\n+ mini_wc file_name: Afficher le nombre de mots dans un fichier                        +\n");
    mini_printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    mini_printf("\n");
}


int execute_command(char** args) {
    /*
    @brief execute command using args passed as params
    @params args: char**
    @return status
    */
    int status;
    pid_t pid, wpid;


    pid = fork();
    if (pid == 0) {
        // Child process
        int status = execve(args[0], args, NULL);
        if(status == -1) {
            mini_perror("fatal: command not found in mini_shell");
        }
        mini_exit();
    } else if (pid < 0) {
        mini_perror("in execute_command in mini_shell.c pid < 0");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


#define BUF_SIZE 64

char **split_command(char *line){
    /* 
    @brief split command into function name, arguments 
    @params command: char*
    @return pointer to pointer to char: char**
    */
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

int remove_enter_in_buffer(char* buffer) {
    /*
    replace '\n' in buffer by '\0'
    Params: buffer
    Return: the len of buffer from beginning to '\0'
    */
    int k;
    for(k = 0; k < mini_strlen(buffer); k++) {
        if(buffer[k] == '\n') {
            buffer[k] = '\0';
            break;
        }
    }
    return k;
}


char *read_command(void) {
    /* 
    @brief read stdin
    @params void
    @return char*
    */
    struct MYFILE *mini_stdin = mini_fopen("/dev/stdin", 'b');

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



void mini_shell_loop(void){
    /* 
    @brief read from stdin and store command_line -> split into function name and arguments -> execute in child process
    @params void
    @return void
    */
    char exit[] = "exit";
    char *command = mini_calloc(sizeof(char), 1024);
    // char* command;
    char **args;
    int status;

    do {
        mini_printf("> ");
        mini_exit_string();
        // command = read_command();
        mini_scanf(command, 1024);
        remove_enter_in_buffer(command);

        if(mini_strcmp(command, exit) == 0) {
            break;
        }
        
        args = split_command(command);
        status = execute_command(args);

    } while (mini_strcmp(command, exit) != 0);
}
