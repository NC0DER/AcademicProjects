#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define MAX_SIZE 800

struct command {
    const char ** argv;
};

void make_process(int input, int output, struct command * com);
void com_pipes(int n, struct command * com);

int main() {
    int i;
    int space;
    int pipes = 1;
    int size;
    int status;
    char c;
    char sh_buffer[MAX_SIZE + 1];
    char * token = NULL;
    char * rest = sh_buffer;
    pid_t pid;

    while (1) {
        //---Shell Input Parsing---
        printf("$ ");
        if (fgets(sh_buffer, MAX_SIZE + 1, stdin) == NULL) break;
        if (sh_buffer == NULL) {
            perror("Not Enough Memory for allocation");
            return 0;
        }
        size = strlen(sh_buffer);
        if (size == MAX_SIZE + 1) {
            //Consuming extra chars that were not parsed to avoid oveflow
            while ((c = fgetc(stdin)) != '\n' && c != EOF); //fgetc(stdin) equivalent with getchar()
        }
        if (sh_buffer == NULL) {
            perror("Not Enough Memory for allocation");
            return 0;
        }
        //Formatting Input for execution by removing newline chars and ';'
        for (i = 0; i < size; ++i)
            if ((sh_buffer[i] == ';') | (sh_buffer[i] == '\n') | (sh_buffer[i] == '\t') | (sh_buffer[i] == '\r'))
                sh_buffer[i] = ' ';
        sh_buffer[size - 1] = '\0';

        //Pipe Counting & Execution
        pipes = 1;
        for (i = 1; i < size; ++i) { //First char is presumed to be non space
            if (sh_buffer[i] == '|')
                ++pipes;
        }
        if (pipes >= 2) {
            int i;
            int k = 0;
            pid_t pid;
            int j = 0;
            char * token_ = NULL;
            char * rest_ = NULL;
            char temp[(size + (2 * pipes)) + 1];

            for (i = 0; i < size; i++) {
                temp[k] = sh_buffer[i];
                if (sh_buffer[i] == '|') {
                    temp[i] = ' ';
                    temp[i + 1] = '|';
                    temp[i + 2] = ' ';
                    k = k + 2;
                }
                k = k + 1;
            }
            temp[k] = '\0';

            pid = fork();
            if (pid < 0) {
                perror("Fork error: Child Process not created:");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) { //Child Process
                //Special word Counting for pipes
                space = 0;
                for (i = 1; i < size; ++i) { //First char is presumed to be non space
                    if ((temp[i] == ' ') && (temp[i - 1] != ' ') && (temp[i - 1] != '|'))
                        ++space;
                }
                if (temp[size - 2] != ' ') //word' '\n is counted as extra word falsely but space needs to increment once at the end
                    ++space; //n-1 spaces inbetween indicate n words

                if (space >= 0) {
                    const char * args1[space + 1];
                    const char * args2[space + 1];
                    struct command cmd[2];

                    rest_ = NULL;
                    token_ = NULL;
                    j = 0;
                    i = 0;
                    rest_ = temp;

                    while ((token_ = strtok_r(rest_, " ", & rest_))) {
                        if (strstr(token_, "|") == NULL) {
                            args1[j] = token_;
                            j = j + 1;
                        } else break;
                    }
                    args1[j] = NULL;

                    j = 0;
                    token_ = NULL;

                    while ((token_ = strtok_r(rest_, " ", & rest_))) {
                        if (strstr(token_, "|") == NULL) {
                            args2[j] = token_;
                            j = j + 1;
                        } else break;
                    }
                    args2[j] = NULL;

                    cmd[0].argv = args1;
                    cmd[1].argv = args2;
                    com_pipes(2, cmd);

                    continue; //Next loop execution
                }
            } else { //Parent Process, waits for all childs
                while ((pid = wait( & status)) > 0);
                continue;
            }
        }
        //Word Counting
        space = 0;
        for (i = 1; i < size; ++i) { //First char is presumed to be non space
            if ((sh_buffer[i] == ' ') && (sh_buffer[i - 1] != ' '))
                ++space;
        }
        if (sh_buffer[size - 2] != ' ') //word' '\n is counted as extra word falsely but space needs to increment once at the end
            ++space; //n-1 spaces inbetween indicate n words

        //cd shell built-in command
        if (strstr(sh_buffer, "cd") != NULL) {
            int j = 0;
            char * token_ = NULL;
            char * rest_ = NULL;
            char * arg[space];

            rest_ = sh_buffer;
            while ((token_ = strtok_r(rest_, " ", & rest_))) {
                arg[j] = token_;
                j = j + 1;
            }
            if (space == 1) { // "cd \n" , "cd\n"
                if (chdir("/home") == -1) {
                    //char* args[1] = getcwd(NULL,PATH_MAX);
                    if (arg[1])
                        fprintf(stderr, "cd: %s: %s\n", arg[1], strerror(errno));
                    else
                        fprintf(stderr, "cd: %s\n", strerror(errno));

                }
                continue; //goto the end of loop because cd should not be passed at exec()
            }
            if (space == 2) {
                if (strcmp(arg[1], "~") == 0) { // "cd ~\n"
                    if (chdir("/home") == -1) {
                        if (arg[1])
                            fprintf(stderr, "cd: %s: %s\n", arg[1], strerror(errno));
                        else
                            fprintf(stderr, "cd: %s\n", strerror(errno));
                    }
                    continue;
                }
                if(chdir(arg[1]) == -1){
                    if (arg[1])
                        fprintf(stderr, "cd: %s: %s\n", arg[1], strerror(errno));
                    else
                        fprintf(stderr, "cd: %s\n", strerror(errno));
		
                }
                continue;
            }
        }
        {
            //Allocating char array for execvp depending on the word count (variably-sized number of args)
            char * params[space];
            //Tokenization of Input
            i = 0;
            while ((token = strtok_r(rest, " ", & rest))) {
                params[i] = token;
                i = i + 1;
            }
            params[i] = NULL;
            if (params == NULL) perror("Parameters were not recognized: ");
            if (strcmp(params[0], "exit") == 0) break; //Exit command exits this shell

            pid = fork();
            if (pid < 0) {
                perror("Fork error: Child Process not created:");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) { //Child Process
                execvp(params[0], params);
                perror("Exec error");
                exit(EXIT_FAILURE);

            } else { //Parent Process, waits for all childs
                while ((pid = wait( & status)) > 0) {
                    continue;
                }
            }
            rest = NULL;
            token = NULL;
            rest = sh_buffer;
        }
    }
    return 0;
}

void make_process(int input, int output, struct command * com) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        if (input != 0) {
            dup2(input, 0);
            close(input);
        }
        if (output != 1) {
            dup2(output, 1);
            close(output);
        }
        execvp(com -> argv[0], (char * const * ) com -> argv);
    }
}

void com_pipes(int n, struct command * com) {
    int i;
    int input, fd[2];

    input = 0;
    // Make all processes but the last of the pipeline
    for (i = 0; i < n - 1; ++i) {
        pipe(fd);
        // fd[1](write end) gets input from the previous iteration
        make_process(input, fd[1], com + i);
        //No need for the write end of the pipe, the child will write here
        close(fd[1]);
        //The next child will read from this
        input = fd[0];
    }
    // Stdin becomes the read end of the previous pipe and outputs to fd[1]
    if (input != 0)
        dup2(input, 0);
    //Execute current process
    execvp(com[i].argv[0], (char * const * ) com[i].argv);
}
