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
