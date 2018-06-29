#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>  //System V semaphore
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "keyvalue.h"

#define BACKLOG 5
#define BUFFERSIZE 2055

void signal_child_handler(int signal_flag){
    while (waitpid(-1, NULL, WNOHANG) > 0); //Alternative parameter -1 instead of 0
}

volatile sig_atomic_t uninterrupted = 1;
void handle_cleanup(int signal_flag)
{
    uninterrupted = 0;
}

struct kvstore_node{
    char key[1025];
    char value[1025];
};

struct kvstore_node* kvstore = NULL;
size_t shared_mem_size;
int shared_mem_id, data_semaphore, connect_semaphore;
struct sembuf up;
struct sembuf down;

char *get(char *key){
    int i = 0;
    if(kvstore == NULL)
        return NULL; //kvstore is empty, no element to return.
    for(i = 0; i < 1000; ++i){
        if(kvstore[i].key[0] == '\0')
            break; //reached last node of current record which is empty
        else if(strncmp(key, kvstore[i].key, 1025) == 0) //Key found, return the value.
            return kvstore[i].value;
    }
    /*If for doesn't return, then there is no next node.
      No key has been found on record, and the record is finished. */
    return NULL;
}

