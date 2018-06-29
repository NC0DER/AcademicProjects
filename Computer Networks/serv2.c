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
int shared_mem_id;
int data_semaphore;
struct sembuf up;
struct sembuf down;

char *get(char *key){
    int i = 0;
    if(kvstore == NULL)
        return NULL; //kvstore is empty, no element to return.
    for(i = 0; i < 2000; ++i){
        if(strncmp(key, kvstore[i].key, 1025) == 0) //Key found, return the value.
            return kvstore[i].value;
    }
    /*If for doesn't return, then there is no next node.
      No key has been found on record, and the record is finished. */
    return NULL;
}

void put(char *key, char *value){
    int i = 0, found = 0,new_index = 0;
    for(i = 0; i < 2000; ++i){
        if(strncmp(key, kvstore[i].key, 1025) == 0){ //Key found, overwrite new value.
            memmove(kvstore[i].value , value, 1025);
            kvstore[i].value[1024] = '\0'; //Null terminate for safety
            found = 1;
            break;
        }
        else if(kvstore[i].key[0] == '\0')
            if(new_index == 0) //Only for the first empty node
                new_index = i; //reached empty node of current record
    }
    if(found) return;
    /*If key is found, the call simply returns with no value(void put).
      i currently points to last empty next_node.
      The new element is added to the end of the list. */
    i = new_index;
    memmove(kvstore[i].key, key, 1025);
    kvstore[i].key[1024] = '\0'; //Null terminate for safety
    memmove(kvstore[i].value, value, 1025);
    kvstore[i].value[1024] = '\0'; //Null terminate for safety
    return; //New node successfully created return.
}

ssize_t writen(int fd, const void *vptr, size_t n){ //Wrapper function from Stevens book, page 78.
    size_t nleft;
    ssize_t nwriten;
    const char *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0){
        if ( ((nwriten = write(fd, ptr, nleft)) <= 0 ) ){
            if (errno == EINTR) nwriten = 0; /* and call write() again */
            else return -1; /* error */
        }
        nleft -= nwriten;
        ptr += nwriten;
    }
    return n;
}

