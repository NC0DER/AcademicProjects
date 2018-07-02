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

    return 0;
}
